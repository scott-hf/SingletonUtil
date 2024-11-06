//$ Copyright 2023-24, Scott Dunbar - Hello Friend LLC - All Rights Reserved $//


#include "SdSingletonSubsystem.h"

#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>



void USdSingletonSubsystem::PostInitialize()
{
	ClearLookupCache();
	Super::PostInitialize();
}
void USdSingletonSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	ClearLookupCache();
	Super::OnWorldBeginPlay(InWorld);
}

void USdSingletonSubsystem::ClearLookupCache()
{
	CacheMap.Empty();
	SingletonActorCacheMap.Empty();
	GlobalObjectRegistry.RegisteredObjects.Empty();
	SingletonInterfaceCacheMap.Empty();
}

void USdSingletonSubsystem::CacheLookupResult(TSubclassOf<UObject> Class, TArray<UClass*> Results)
{
	if (!CacheMap.Contains(Class))
	{
		CacheMap.Add(Class, FSdDerivedClassCache());
	}
	for (auto& Result : Results)
	{
		CacheMap[Class].Classes.AddUnique(Result);
	}
}


TArray<UClass*> USdSingletonSubsystem::GetDerivedClassesFromObject(UObject* Outer)
{
	TArray<UClass*> ChildClasses;
	if (!IsValid(Outer))
	{
		return ChildClasses;
	}

	TSubclassOf<UObject> Class = GetClass();

	if (CacheMap.Contains(Class))
	{
		return CacheMap[Class].Classes;
	}

	GetDerivedClasses(Class, ChildClasses, false);
	CacheLookupResult(Class, ChildClasses);
	return ChildClasses;
}


TArray<UClass*> USdSingletonSubsystem::K2_GetDerivedClassesFromClass(TSubclassOf<UObject> Class)
{
	TArray<UClass*> ChildClasses;
	if (!IsValid(Class))
	{
		return ChildClasses;
	}

	if (CacheMap.Contains(Class))
	{
		return CacheMap[Class].Classes;
	}

	GetDerivedClasses(Class, ChildClasses, false);
	CacheLookupResult(Class, ChildClasses);
	return ChildClasses;
}


TScriptInterface<UInterface> USdSingletonSubsystem::K2_GetSingletonInterface(TSubclassOf<UInterface> InInterfaceClass, UObject*& OutObject, const FSD_SingletonSearchParams& SearchParams, bool bIgnoreCache)
{
	TScriptInterface<UInterface> OutInterface;
	
	FSD_SingletonInterfaceHashKey SingletonInterfaceHashKey = FSD_SingletonInterfaceHashKey(InInterfaceClass, SearchParams);

	if (!IsValid(SingletonInterfaceHashKey.InterfaceClass) || SingletonInterfaceHashKey.InterfaceClass == UInterface::StaticClass())
	{
		return OutInterface;
	}

	if (!bIgnoreCache && SingletonInterfaceCacheMap.Contains(SingletonInterfaceHashKey))
	{
		UObject* CachedObject = SingletonInterfaceCacheMap[SingletonInterfaceHashKey];
		if (CachedObject && IsValid(CachedObject))
		{
			OutInterface.SetObject(CachedObject);
			OutInterface = CachedObject;
			OutObject = CachedObject;
			return OutInterface;

		}
	}

	if (SearchParams.bIncludeOnlyActors)
	{	
		TArray<AActor*> WorldActors;
		const UWorld* SingletonWorld = GetWorld();

		UGameplayStatics::GetAllActorsWithInterface(SingletonWorld, InInterfaceClass, WorldActors);
		for (auto ActorRef : WorldActors)
		{			
			OutInterface.SetObject(ActorRef);
			SingletonInterfaceCacheMap.Add(SingletonInterfaceHashKey, ActorRef);
			OutInterface = ActorRef;
			OutObject = ActorRef;
			break;
		}
	}
	else
	{

		UObject* CheckOuter = nullptr;
		UPackage* InsidePackage = nullptr;
		for (FThreadSafeObjectIterator It; It; ++It)
		{
			if (!It->GetClass()->ImplementsInterface(SingletonInterfaceHashKey.InterfaceClass))
			{
				continue;
			}

			if (It->IsTemplate(RF_ClassDefaultObject))
			{
				if (!SearchParams.bShouldIncludeDefaultObjects)
				{
					continue;
				}
			}
			else if (SearchParams.bOnlyDefaultObjects)
			{
				continue;
			}

			if (SearchParams.bOnlyGCObjects && GUObjectArray.IsDisregardForGC(*It))
			{
				continue;
			}

			if (SearchParams.bOnlyRootObjects && !It->IsRooted())
			{
				continue;
			}

			if (SearchParams.FilterClass && !It->IsA(SearchParams.FilterClass))
			{
				continue;
			}

			if (!SearchParams.FilterString.IsEmpty() && !It->GetName().Contains(SearchParams.FilterString))
			{
				continue;
			}

			if (!SearchParams.bIncludeTransient)
			{
				UPackage* ContainerPackage = It->GetOutermost();
				if (ContainerPackage == GetTransientPackage() || ContainerPackage->HasAnyFlags(RF_Transient))
				{
					continue;
				}
			}

			auto FoundInterfaceObject = *It;
			OutInterface.SetObject(FoundInterfaceObject);
			SingletonInterfaceCacheMap.Add(SingletonInterfaceHashKey, FoundInterfaceObject);
			OutInterface = FoundInterfaceObject;
			OutObject = FoundInterfaceObject;
			break;
		}
	}

	return OutInterface;
}

AActor* USdSingletonSubsystem::K2_GetSingletonActor(TSubclassOf<AActor> Class, bool bCreateIfMissing, bool bIgnoreCache)
{
	AActor* OutActor = nullptr;
	if (!IsValid(Class))
	{
		return OutActor;
	}

	if (!bIgnoreCache && SingletonActorCacheMap.Contains(Class))
	{
		UObject* CachedObject = SingletonActorCacheMap[Class];
		if (CachedObject && IsValid(CachedObject))
		{
			return SingletonActorCacheMap[Class];
		}
	}

	bool bActorFound = false;
	TArray<AActor*> WorldActors;
	AActor* BackupOption = nullptr;
	
	const UWorld* SingletonWorld = GetWorld();

	UGameplayStatics::GetAllActorsOfClass(SingletonWorld, Class, WorldActors);
	for (auto ActorRef : WorldActors)
	{
		// this is for angelscript returning uninstantiated base default objects over world-spawned BP-derived objects
		// because the .AS objects always seem to be available when calling GetActorsOfClass(), even if they're not in the world
		// therefore, we iterate through all classes and prefer the deepest level of descendent
		// 
		// loop through actor classes
		// check if root component is valid
		// if iterated actor is a child actor class of backup option class, set backup option class
		
		if (!IsValid(BackupOption))
		{
			BackupOption = ActorRef;
		}
		auto ActorClass = ActorRef->GetClass();
		auto BackupOptionActorClass = BackupOption->GetClass();
		if (!IsValid(ActorRef->GetRootComponent()))
		{
			if (ActorClass->IsChildOf(BackupOptionActorClass))
			{
				BackupOption = ActorRef;
			}
			continue;
		}
		OutActor = ActorRef;
		bActorFound = true;
		break;		
	}

	if (!(OutActor && IsValid(OutActor)))
	{
		if ((BackupOption && IsValid(BackupOption))) 
		{
			OutActor = BackupOption;
			bActorFound = true;
		}		
	}

	if (!bCreateIfMissing && !IsValid(OutActor))
	{
		return nullptr;
	}
	else if (!IsValid(OutActor))
	{
		FActorSpawnParameters SpawnParams;
		OutActor = GetWorld()->SpawnActor(Class);
	}

	if (IsValid(OutActor))
	{
		SingletonActorCacheMap.Add(Class, OutActor);
	}

	return OutActor;

}



bool USdSingletonSubsystem::IsGlobalObjectInRegistry(TSubclassOf<UObject> InObjectClass, FName InGlobalId)
{
	FSdGlobalObjectHashKey ObjHashKey = FSdGlobalObjectHashKey(InObjectClass, InGlobalId);
	return GlobalObjectRegistry.RegisteredObjects.Contains(ObjHashKey);
}

void USdSingletonSubsystem::RegisterGlobalObjectInRegistry(TSubclassOf<UObject> InObjectClass, UObject* InObject, FName InGlobalId)
{
	FSdGlobalObjectHashKey ObjHashKey = FSdGlobalObjectHashKey(InObjectClass, InGlobalId);
	GlobalObjectRegistry.RegisteredObjects.Add(ObjHashKey, InObject);
}

UObject* USdSingletonSubsystem::K2_GetGlobalObjectInRegistry(TSubclassOf<UObject> InObjectClass, FName InGlobalId)
{
	FSdGlobalObjectHashKey ObjHashKey = FSdGlobalObjectHashKey(InObjectClass, InGlobalId);
	if (GlobalObjectRegistry.RegisteredObjects.Contains(ObjHashKey))
	{
		return GlobalObjectRegistry.RegisteredObjects[ObjHashKey];
	}
	return nullptr;
}

bool USdSingletonSubsystem::IsGlobalObjectInRegistrySoft(TSoftClassPtr<UObject> InObjectSoftClass, FName InGlobalId)
{	
	if (InObjectSoftClass.Get() == nullptr)
	{
		UObject* LoadedObject = InObjectSoftClass.ToSoftObjectPath().TryLoad();
	}
	TSubclassOf<UObject> InObjectClass = InObjectSoftClass.Get();

	FSdGlobalObjectHashKey ObjHashKey = FSdGlobalObjectHashKey(InObjectClass, InGlobalId);
	return GlobalObjectRegistry.RegisteredObjects.Contains(ObjHashKey);
}

TMap<FString, UObject*> USdSingletonSubsystem::DebugGetObjectCacheSnapshot()
{
	TMap<FString, UObject*> OutCache;
	
	for (auto MapItx : GlobalObjectRegistry.RegisteredObjects)
	{
		OutCache.Add(MapItx.Key.GetHashKeyDisplayName(), MapItx.Value);
	}
	return OutCache;
}

TMap<TSubclassOf<UObject>, AActor*> USdSingletonSubsystem::DebugGetActorCacheSnapshot()
{
	TMap<TSubclassOf<UObject>, AActor*> OutCache;
	for (auto MapItx : SingletonActorCacheMap)
	{
		OutCache.Add(MapItx.Key, MapItx.Value);
	}
	return OutCache;
}
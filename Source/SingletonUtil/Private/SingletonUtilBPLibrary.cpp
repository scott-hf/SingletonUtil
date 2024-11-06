//$ Copyright 2023-24, Scott Dunbar - Hello Friend LLC - All Rights Reserved $//

#include "SingletonUtilBPLibrary.h"
#include "SdSingletonSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SingletonUtil.h"


DEFINE_LOG_CATEGORY(LogSingletonUtilBPLibrary);

USingletonUtilBPLibrary::USingletonUtilBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

TScriptInterface<UInterface> USingletonUtilBPLibrary::USingletonUtilBPLibrary::K2_GetSingletonInterface(UObject* WorldContextObject, TSubclassOf<UInterface> InClass, UObject*& OutObject)
{
	if (!(WorldContextObject && IsValid(WorldContextObject)))
	{
		UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::WorldContextObject Is not valid"));
		return nullptr;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!(World && IsValid(World)))
	{
		UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::World Is not valid"));
		return nullptr;
	}

	if (!(InClass && IsValid(InClass )))
	{
		UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::Class Is not valid"));
		return nullptr;
	}
	USdSingletonSubsystem* SingletonSubsystem = UWorld::GetSubsystem<USdSingletonSubsystem>(World);
	if (IsValid(SingletonSubsystem))
	{
		FSD_SingletonSearchParams SearchParams;
		auto FoundActor = SingletonSubsystem->K2_GetSingletonInterface(InClass, OutObject, SearchParams, false);
		if (OutObject && IsValid(OutObject))
		{
			return FoundActor;
		}
	}		
	
	UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::Could not get/create the requested singleton interface"));	
	return nullptr;
}


AActor* USingletonUtilBPLibrary::K2_GetSingletonActor(UObject* WorldContextObject, TSubclassOf<AActor> Class, bool bCreateIfMissing)
{	
	if (!(WorldContextObject && IsValid(WorldContextObject)))
	{
		UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::WorldContextObject Is not valid"));
		return nullptr;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!(World && IsValid(World)))
	{
		UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::World Is not valid"));
		return nullptr;
	}

	if (!(Class && IsValid(Class)))
	{
		UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::Class Is not valid"));
		return nullptr;
	}
	USdSingletonSubsystem* SingletonSubsystem = UWorld::GetSubsystem<USdSingletonSubsystem>(World);
	if (IsValid(SingletonSubsystem))
	{
		auto FoundActor = SingletonSubsystem->K2_GetSingletonActor(Class, bCreateIfMissing);
		if (FoundActor && IsValid(FoundActor))
		{
			return FoundActor;
		}
	}		
	
	UE_LOG(LogSingletonUtilBPLibrary, Log, TEXT("USingletonUtilBPLibrary::Could not get/create the requested singleton actor"));	
	return nullptr;
}

//$ Copyright 2023-24, Scott Dunbar - Hello Friend LLC - All Rights Reserved $//

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Runtime/CoreUObject/Public/UObject/ObjectMacros.h"
#include "Runtime/CoreUObject/Public/UObject/Interface.h"
#include "SdSingletonSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FSD_ObjectWrapper
{
	GENERATED_USTRUCT_BODY()
public:
	TWeakObjectPtr<UObject> Object;
};



USTRUCT(BlueprintType)
struct FSD_SingletonSearchParams
{
	GENERATED_USTRUCT_BODY()

public:
	FSD_SingletonSearchParams() {}

	// Copy constructor
	FSD_SingletonSearchParams(const FSD_SingletonSearchParams& InSearchParams)
		: FilterString(InSearchParams.FilterString), bIncludeOnlyActors(InSearchParams.bIncludeOnlyActors), FilterClass(InSearchParams.FilterClass), bShouldIncludeDefaultObjects(InSearchParams.bShouldIncludeDefaultObjects), bOnlyDefaultObjects(InSearchParams.bOnlyDefaultObjects), bOnlyRootObjects(InSearchParams.bOnlyRootObjects), bOnlyGCObjects(InSearchParams.bOnlyGCObjects), bIncludeTransient(InSearchParams.bIncludeTransient)
	{
	}

	FSD_SingletonSearchParams(TSubclassOf<UObject> InInterfaceClass, const FString& InFilterString, bool InIncludeOnlyActors = false,
		UClass* InFilterClass = nullptr, bool InShouldIncludeDefaultObjects = false, bool InOnlyDefaultObjects = false,
		bool InOnlyRootObjects = false, bool InOnlyGCObjects = false, bool InIncludeTransient = true)
		: FilterString(InFilterString), bIncludeOnlyActors(InIncludeOnlyActors), FilterClass(InFilterClass), bShouldIncludeDefaultObjects(InShouldIncludeDefaultObjects), bOnlyDefaultObjects(InOnlyDefaultObjects), bOnlyRootObjects(InOnlyRootObjects), bOnlyGCObjects(InOnlyGCObjects), bIncludeTransient(InIncludeTransient) {}

	// A string to filter the results by name.
	// Only objects with names containing this string will be included in the search results.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	FString FilterString;

	// When true, only actors will be included in the search results.
	// Non-actor objects will be excluded if this is set to true.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	bool bIncludeOnlyActors = false;

	// Used to filter by object class type, NOT by interface class.
	// Only objects of this specific class or its subclasses will be included in the search results.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	UClass* FilterClass = nullptr;

	// When true, includes default objects (like class default objects) in the search results.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	bool bShouldIncludeDefaultObjects = false;

	// When true, only default objects (such as class default objects) will be included in the search results.
	// Non-default objects will be excluded.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	bool bOnlyDefaultObjects = false;

	// When true, only root objects will be included in the search results.
	// Root objects are those without any outer object.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	bool bOnlyRootObjects = false;

	// When true, only garbage-collectable (GC) objects will be included in the search results.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	bool bOnlyGCObjects = false;

	// When true, includes transient objects in the search results.
	// Transient objects are those that are not meant to be saved or serialized.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SingletonUtil")
	bool bIncludeTransient = false;

	friend bool operator==(const FSD_SingletonSearchParams& A, const FSD_SingletonSearchParams& B)
	{
		return A.FilterString == B.FilterString && A.bIncludeOnlyActors == B.bIncludeOnlyActors && A.FilterClass == B.FilterClass && A.bShouldIncludeDefaultObjects == B.bShouldIncludeDefaultObjects && A.bOnlyDefaultObjects == B.bOnlyDefaultObjects && A.bOnlyRootObjects == B.bOnlyRootObjects && A.bOnlyGCObjects == B.bOnlyGCObjects && A.bIncludeTransient == B.bIncludeTransient;
	}

	friend uint32 GetTypeHash(const FSD_SingletonSearchParams& Key)
	{
		uint32 Hash = GetTypeHash(Key.FilterString);
		Hash = HashCombine(Hash, GetTypeHash(Key.bIncludeOnlyActors));
		Hash = HashCombine(Hash, GetTypeHash(Key.FilterClass));
		Hash = HashCombine(Hash, GetTypeHash(Key.bShouldIncludeDefaultObjects));
		Hash = HashCombine(Hash, GetTypeHash(Key.bOnlyDefaultObjects));
		Hash = HashCombine(Hash, GetTypeHash(Key.bOnlyRootObjects));
		Hash = HashCombine(Hash, GetTypeHash(Key.bOnlyGCObjects));
		Hash = HashCombine(Hash, GetTypeHash(Key.bIncludeTransient));
		return Hash;
	}

	FString GetHashKeyDisplayName() const
	{
		FString OutDisplayName;

		// Append all other parameters for a complete display name
		OutDisplayName += TEXT("_") + (FilterString.IsEmpty() ? FString(TEXT("NoFilter")) : FilterString);
		OutDisplayName += TEXT("_") + (bIncludeOnlyActors ? FString(TEXT("ActorsOnly")) : FString(TEXT("IncludeAll")));
		OutDisplayName += TEXT("_") + (FilterClass ? FilterClass->GetName() : FString(TEXT("NoFilterClass")));
		OutDisplayName += TEXT("_") + (bShouldIncludeDefaultObjects ? FString(TEXT("IncludeDefault")) : FString(TEXT("ExcludeDefault")));
		OutDisplayName += TEXT("_") + (bOnlyDefaultObjects ? FString(TEXT("OnlyDefault")) : FString(TEXT("NotOnlyDefault")));
		OutDisplayName += TEXT("_") + (bOnlyRootObjects ? FString(TEXT("OnlyRoot")) : FString(TEXT("IncludeAllRoots")));
		OutDisplayName += TEXT("_") + (bOnlyGCObjects ? FString(TEXT("OnlyGC")) : FString(TEXT("IncludeAllGC")));
		OutDisplayName += TEXT("_") + (bIncludeTransient ? FString(TEXT("IncludeTransient")) : FString(TEXT("ExcludeTransient")));

		return OutDisplayName;
	}
};

USTRUCT(BlueprintType)
struct FSD_SingletonInterfaceHashKey
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "SingletonUtil")
	FSD_SingletonSearchParams SingletonSearchParams;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "SingletonUtil")
	TSubclassOf<UInterface> InterfaceClass;

	// Default constructor
	FSD_SingletonInterfaceHashKey() {}

	// Constructor with InterfaceClass only
	FSD_SingletonInterfaceHashKey(TSubclassOf<UInterface> InInterfaceClass)
		: InterfaceClass(InInterfaceClass) {}

	// Constructor with InterfaceClass and SingletonSearchParams
	FSD_SingletonInterfaceHashKey(TSubclassOf<UInterface> InInterfaceClass, const FSD_SingletonSearchParams& InSingletonSearchParams)
		: InterfaceClass(InInterfaceClass)
	{
		SingletonSearchParams = InSingletonSearchParams;
	}

	// Equality operator
	friend bool operator==(const FSD_SingletonInterfaceHashKey& A, const FSD_SingletonInterfaceHashKey& B)
	{
		return A.InterfaceClass == B.InterfaceClass && A.SingletonSearchParams == B.SingletonSearchParams;
	}

	// Hash function
	friend uint32 GetTypeHash(const FSD_SingletonInterfaceHashKey& Key)
	{
		uint32 InterfaceHash = GetTypeHash(Key.InterfaceClass);
		uint32 ParamsHash = GetTypeHash(Key.SingletonSearchParams);
		return HashCombine(InterfaceHash, ParamsHash);
	}

	// Display name function for debugging
	FString GetHashKeyDisplayName() const
	{
		FString OutDisplayName;

		// Start with the name of the interface class
		if (InterfaceClass)
		{
			OutDisplayName = InterfaceClass->GetName();
		}
		else
		{
			OutDisplayName = TEXT("NoInterfaceClass");
		}

		// Append search params display name
		OutDisplayName += TEXT("_") + SingletonSearchParams.GetHashKeyDisplayName();

		return OutDisplayName;
	}
};


USTRUCT(BlueprintType)
struct FSdGlobalObjectHashKey
{
	GENERATED_USTRUCT_BODY()

public:
	FSdGlobalObjectHashKey() {}
	FSdGlobalObjectHashKey(TSubclassOf<UObject> InObjectClass)
	{
		ObjectClass = InObjectClass;
	}

	FSdGlobalObjectHashKey(TSubclassOf<UObject> InObjectClass, FName InGlobalId)
	{
		ObjectClass = InObjectClass;
		GlobalId = InGlobalId;
	}

	UPROPERTY()
	TSubclassOf<UObject> ObjectClass = UObject::StaticClass();

	UPROPERTY()
	FName GlobalId = NAME_None;

	friend bool operator==(const FSdGlobalObjectHashKey& A, const FSdGlobalObjectHashKey& B)
	{
		return A.ObjectClass == B.ObjectClass && A.GlobalId == B.GlobalId;
	}

	friend uint32 GetTypeHash(const FSdGlobalObjectHashKey& Key)
	{
		const uint32 StartLocationHash = GetTypeHash(Key.ObjectClass);
		const uint32 EndLocationHash = GetTypeHash(Key.GlobalId);
		return HashCombine(StartLocationHash, EndLocationHash);
	}

	FString GetHashKeyDisplayName() const
	{
		FString OutDisplayName;
		ObjectClass->GetName(OutDisplayName);
		if (GlobalId != NAME_None)
		{
			OutDisplayName = OutDisplayName.Append(TEXT("_"));
			OutDisplayName = OutDisplayName.Append(GlobalId.ToString());
		}
		return OutDisplayName;
	}
};

USTRUCT(BlueprintType)
struct SINGLETONUTIL_API FSdDerivedClassCache
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		TArray<UClass*> Classes; 
};

USTRUCT(BlueprintType)
struct SINGLETONUTIL_API FSdGlobalObjectRegistry
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		TMap<FSdGlobalObjectHashKey, UObject*> RegisteredObjects;
};

/**
 * SingletonUtil
 */
UCLASS(Category = "SingletonUtil", DisplayName = "SD Singleton Subsystem")
class SINGLETONUTIL_API USdSingletonSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UPROPERTY()
	TMap<TSubclassOf<UObject>, AActor*> SingletonActorCacheMap;

	UPROPERTY()
	TMap<FSD_SingletonInterfaceHashKey, UObject*> SingletonInterfaceCacheMap;

	UPROPERTY()
	FSdGlobalObjectRegistry GlobalObjectRegistry;

	UPROPERTY()
	TMap<TSubclassOf<UObject>, FSdDerivedClassCache> CacheMap;

	UFUNCTION()
	void CacheLookupResult(TSubclassOf<UObject> Class, TArray<UClass*> Results);

	UFUNCTION(BlueprintCallable, Category = "SingletonUtil")
	void ClearLookupCache();

	// SINGLETON ACTOR FUNCTIONS

	/**
	 * Retrieves all classes derived from a given object within the specified outer scope.
	 * @param Outer - The outer object scope to search within.
	 * @return An array of UClass representing the derived classes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil")
	TArray<UClass*> GetDerivedClassesFromObject(UObject* Outer);

	/**
	 * Retrieves all classes derived from the specified class.
	 * @param InClass - The base class to find derived classes from.
	 * @return An array of UClass representing the derived classes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil", DisplayName = "Get Derived Classes From Class", meta = (DeterminesOutputType = InClass))
	TArray<UClass*> K2_GetDerivedClassesFromClass(TSubclassOf<UObject> InClass);

	/**
	 * Retrieves the singleton actor instance of a specified class. Assumes only one actor of that class exists.
	 * @param InClass - The class of the singleton actor to retrieve.
	 * @param bCreateIfMissing - If true, creates the actor if it doesn't already exist.
	 * @param bIgnoreCache - If true, bypasses the cache to find the actor.
	 * @return A pointer to the singleton actor instance, or nullptr if not found/created.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil", DisplayName = "GetSingletonActor", meta = (DeterminesOutputType = InClass))
	AActor* K2_GetSingletonActor(TSubclassOf<AActor> InClass, bool bCreateIfMissing = false, bool bIgnoreCache = false);

	/**
	 * Retrieves the singleton interface instance of a specified class. Assumes only one instance of that interface class exists.
	 *
	 * @param InClass          The specific interface class to retrieve as a singleton.
	 * @param OutObject        Output parameter that receives the actual UObject implementing the interface.
	 *                         Useful if you need direct access to the object itself, not just the "interfaced" object.
	 * @param SearchParams     Optional search parameters that allow for refined filtering criteria, such as filtering by object class,
	 *                         name string, or only including actors. If not provided, default search settings are used.
	 * @param bIgnoreCache     If true, bypasses any cached instances and performs a fresh search.
	 *                         Typically only used to troubleshoot issues
	 *
	 * @return                 A TScriptInterface<UInterface> pointer to the singleton interface instance, or nullptr if no matching
	 *                         instance was found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Singleton Util", DisplayName = "Get Singleton Interface", meta = (DeterminesOutputType = InClass))
	TScriptInterface<UInterface> K2_GetSingletonInterface(TSubclassOf<UInterface> InClass, UObject*& OutObject, const FSD_SingletonSearchParams& SearchParams = FSD_SingletonSearchParams(), bool bIgnoreCache = false);

	// SINGLETON UOBJECT FUNCTIONS

	/**
	 * Checks if a global object of the specified class is registered in the global registry.
	 * @param InObjectClass - The class type of the object to check.
	 * @param InGlobalId - Optional ID to identify the specific global object.
	 * @return True if the object is registered, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SingletonUtil")
	bool IsGlobalObjectInRegistry(TSubclassOf<UObject> InObjectClass, FName InGlobalId = NAME_None);

	/**
	 * Registers a global object instance in the registry for a given class and optional ID.
	 * @param InObjectClass - The class type of the object to register.
	 * @param InObject - The object instance to register.
	 * @param InGlobalId - Optional ID to identify the specific global object.
	 */
	UFUNCTION(BlueprintCallable, Category = "SingletonUtil")
	void RegisterGlobalObjectInRegistry(TSubclassOf<UObject> InObjectClass, UObject* InObject, FName InGlobalId = NAME_None);

	/**
	 * Retrieves a global object from the registry by its class type and optional ID.
	 * @param InObjectClass - The class type of the object to retrieve.
	 * @param InGlobalId - Optional ID to identify the specific global object.
	 * @return The object instance if found, or nullptr otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil", DisplayName = "GetGlobalObjectInRegistry", meta = (DeterminesOutputType = InObjectClass))
	UObject* K2_GetGlobalObjectInRegistry(TSubclassOf<UObject> InObjectClass, FName InGlobalId = NAME_None);

	/**
	 * Checks if a global object represented by a soft class is registered in the global registry.
	 * @param InObjectClass - The soft class representing the object's class.
	 * @param InGlobalId - Optional ID to identify the specific global object.
	 * @return True if the object is registered, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "SingletonUtil")
	bool IsGlobalObjectInRegistrySoft(TSoftClassPtr<UObject> InObjectClass, FName InGlobalId = NAME_None);

	// DEBUG FUNCTIONS

	/**
	 * Retrieves a snapshot of the current object cache as a map of object names to their instances.
	 * @return A TMap containing object names as keys and their corresponding UObject as values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil|Debug")
	TMap<FString, UObject*> DebugGetObjectCacheSnapshot();

	/**
	 * Retrieves a snapshot of the current actor cache as a map of class types to actor instances.
	 * @return A TMap containing UClass as keys and their corresponding AActor as values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil|Debug")
	TMap<TSubclassOf<UObject>, AActor*> DebugGetActorCacheSnapshot();

	/**
	 * Retrieves a snapshot of the current interface cache as a map of class types to object instances.
	 * @return A TMap containing SingletonInterfaceHashKeys as keys and their corresponding UObjects as values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SingletonUtil|Debug")
	TMap<FSD_SingletonInterfaceHashKey, UObject*> DebugGetInterfaceCacheSnapshot();
};

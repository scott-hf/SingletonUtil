//$ Copyright 2023-24, Scott Dunbar - Hello Friend LLC - All Rights Reserved $//

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SingletonUtilBPLibrary.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogSingletonUtilBPLibrary, Log, All);

UCLASS()
class USingletonUtilBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	/**
	 * Retrieves a singleton actor of the specified class within the current world context.
	 * Assumes only one instance of the actor class exists globally, or optionally creates one if missing.
	 *
	 * @param WorldContextObject   The world context object, typically a reference to the current world, used to
	 *                             determine the scope of the singleton search. Required for Blueprint compatibility.
	 * @param Class                The actor class to retrieve as a singleton. This should be a subclass of AActor
	 *                             that represents the desired singleton actor type.
	 * @param bCreateIfMissing     If true, creates a new instance of the actor if no existing singleton instance is found.
	 *                             Useful for ensuring the actor's availability without additional setup in the level.
	 *
	 * @return                     A pointer to the singleton actor instance, or nullptr if no matching actor was found
	 *                             (and `bCreateIfMissing` was false).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Singleton Util", DisplayName = "Get Singleton Actor", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = Class))
	static AActor* K2_GetSingletonActor(UObject* WorldContextObject, TSubclassOf<AActor> Class, bool bCreateIfMissing = false);

	/**
	 * Retrieves the singleton interface instance of a specified class. Assumes only one instance of that interface class exists.
	 * For more advanced search params, access the singleton subsystem
	 *
	 * @param WorldContextObject   The world context object, typically a reference to the current world, used to
	 *                             determine the scope of the singleton search. Required for Blueprint compatibility.
	 * @param InClass          The specific interface class to retrieve as a singleton.
	 * @param OutObject        Output parameter that receives the actual UObject implementing the interface.
	 *                         Useful if you need direct access to the object itself, not just the "interfaced" object.
	 *
	 * @return                 A TScriptInterface<UInterface> pointer to the singleton interface instance, or nullptr if no matching
	 *                         instance was found.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Singleton Util", DisplayName = "Get Singleton Interface", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = InterfaceClass))
	static TScriptInterface<UInterface> K2_GetSingletonInterface(UObject* WorldContextObject, TSubclassOf<UInterface> InterfaceClass, UObject*& OutObject);
};

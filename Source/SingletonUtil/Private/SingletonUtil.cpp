//$ Copyright 2023-24, Scott Dunbar - Hello Friend LLC - All Rights Reserved $//

#include "SingletonUtil.h"

#define LOCTEXT_NAMESPACE "FSingletonUtilModule"

void FSingletonUtilModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FSingletonUtilModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSingletonUtilModule, SingletonUtil)
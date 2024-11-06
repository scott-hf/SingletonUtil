//$ Copyright 2023-24, Scott Dunbar - Hello Friend LLC - All Rights Reserved $//

#pragma once

#include "Modules/ModuleManager.h"

class FSingletonUtilModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

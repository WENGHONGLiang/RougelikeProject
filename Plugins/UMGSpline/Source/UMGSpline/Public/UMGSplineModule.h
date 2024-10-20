// Copyright Qibo Pang 2022. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FUMGSplineModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
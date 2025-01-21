// Copyright CityColor 2024

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MeshToolStyle.h"

class FMeshToolCommands : public TCommands<FMeshToolCommands>
{
public:

	FMeshToolCommands()
		: TCommands<FMeshToolCommands>(TEXT("MeshTool"), NSLOCTEXT("Contexts", "MeshTool", "MeshTool Plugin"), NAME_None, FMeshToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};
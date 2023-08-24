// Copyright (c) Yevhenii Selivanov

#pragma once

#include "UObject/Interface.h"
//---
#include "MetaCheatManagerInterface.generated.h"

struct FMetaCheatCommand;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UMetaCheatManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for classes that manage meta cheat commands.
 * Provides a blueprint-exposed method to retrieve all cheat commands.
 * In general, is helpful when working with Cheat Manager classes.
 */
class METACHEATMANAGER_API IMetaCheatManagerInterface
{
	GENERATED_BODY()

public:
	/** Returns the array of cheat commands. */
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual const TArray<FMetaCheatCommand>& GetAllCheatCommands() const = 0;

	/** It has to be bound to UConsole::RegisterConsoleAutoCompleteEntries to register the cheat in the console. */
	virtual void RegisterAutoCompleteEntries(TArray<struct FAutoCompleteCommand>& OutCommands) const = 0;
};

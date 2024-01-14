// Copyright (c) Yevhenii Selivanov

#include "MetaCheatManager.h"
//---
#include "MetaCheatManagerUtils.h"
//---
#include "Engine/Console.h"
//---
#include UE_INLINE_GENERATED_CPP_BY_NAME(MetaCheatManager)

// Is overridden to initialize all cheat commands on editor startup
void UMetaCheatManager::PostInitProperties()
{
	Super::PostInitProperties();

	UMetaCheatManagerUtils::InitAllCheatCommands(this, /*Out*/AllCheatCommands);

	if (!UConsole::RegisterConsoleAutoCompleteEntries.IsBoundToObject(this))
	{
		UConsole::RegisterConsoleAutoCompleteEntries.AddUObject(this, &ThisClass::RegisterAutoCompleteEntries);
	}
}

// Is overridden to convert meta CheatName Your.Cheat.Name to the function name YourCheatFunction whenever user enters the command
bool UMetaCheatManager::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	const bool bProcessed = UMetaCheatManagerUtils::TryProcessConsoleExec(this, Cmd, Ar, Executor);
	return bProcessed || Super::ProcessConsoleExec(Cmd, Ar, Executor);
}

// Garbage things before destroying the Cheat Manager
void UMetaCheatManager::BeginDestroy()
{
	UConsole::RegisterConsoleAutoCompleteEntries.RemoveAll(this);

	Super::BeginDestroy();
}

// Is bound to return all initialized meta cheat commands to see them in the console
void UMetaCheatManager::RegisterAutoCompleteEntries(TArray<FAutoCompleteCommand>& OutCommands) const
{
	UMetaCheatManagerUtils::RegisterAutoCompleteEntries(/*out*/OutCommands, AllCheatCommands);
}

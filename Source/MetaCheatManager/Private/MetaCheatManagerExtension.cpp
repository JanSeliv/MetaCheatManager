// Copyright (c) Yevhenii Selivanov

#include "MetaCheatManagerExtension.h"
//---
#include "MetaCheatManagerUtils.h"
//---
#include "Engine/Console.h"
//---
#include UE_INLINE_GENERATED_CPP_BY_NAME(MetaCheatManagerExtension)

// Is overridden to initialize all cheat commands on startup
void UMetaCheatManagerExtension::PostInitProperties()
{
	Super::PostInitProperties();

	UMetaCheatManagerUtils::InitAllCheatCommands(this, /*Out*/AllCheatCommands);

	if (!UConsole::RegisterConsoleAutoCompleteEntries.IsBoundToObject(this))
	{
		UConsole::RegisterConsoleAutoCompleteEntries.AddUObject(this, &ThisClass::RegisterAutoCompleteEntries);
	}
}

// Is overridden to convert meta CheatName Your.Cheat.Name to the function name YourCheatFunction whenever user enters the command
bool UMetaCheatManagerExtension::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	const bool bProcessed = UMetaCheatManagerUtils::TryProcessConsoleExec(this, Cmd, Ar, Executor);
	return bProcessed || Super::ProcessConsoleExec(Cmd, Ar, Executor);
}

// Garbage things before destroying the Cheat Manager
void UMetaCheatManagerExtension::BeginDestroy()
{
	UConsole::RegisterConsoleAutoCompleteEntries.RemoveAll(this);

	Super::BeginDestroy();
}

// Is bound to return all initialized meta cheat commands to see them in the console
void UMetaCheatManagerExtension::RegisterAutoCompleteEntries(TArray<FAutoCompleteCommand>& OutCommands) const
{
	UMetaCheatManagerUtils::RegisterAutoCompleteEntries(/*out*/OutCommands, AllCheatCommands);
}

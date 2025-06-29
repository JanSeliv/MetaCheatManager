// Copyright (c) Yevhenii Selivanov

#include "MetaCheatManagerUtils.h"
//---
#include "MetaCheatCommand.h"
#include "MetaCheatManagerInterface.h"
//---
#include "ConsoleSettings.h"
//---
#include UE_INLINE_GENERATED_CPP_BY_NAME(MetaCheatManagerUtils)

void UMetaCheatManagerUtils::InitAllCheatCommands(const TScriptInterface<IMetaCheatManagerInterface> CheatManager, TArray<FMetaCheatCommand>& OutAllCheatCommands)
{
#if WITH_EDITOR
	if (!ensureMsgf(CheatManager, TEXT("ASSERT: [%i] %s:\n'CheatManager' is not valid!"), __LINE__, *FString(__FUNCTION__)))
	{
		return;
	}

	UObject* CheatManagerObj = CheatManager.GetObject();
	checkf(CheatManagerObj, TEXT("ERROR: [%i] %s:\n'CheatManagerObj' is null!"), __LINE__, *FString(__FUNCTION__));

	// It automatically adds DefaultMetaCheatManager.ini config on the editor startup to the Config folder on your project
	// to have your cheat commands with custom Cheat Names in the packaged build as well, you don't need to do anything specific about it.
	// Such solution is used because any metadata can be obtained only in the Editor, so we store it in the config file for the build.

	if (!CheatManagerObj->HasAllFlags(RF_ClassDefaultObject))
	{
		// Do not init cheat commands for instances since we save them as default values into config file
		return;
	}

	if (!OutAllCheatCommands.IsEmpty())
	{
		OutAllCheatCommands.Empty();
	}

	// Find all cheat commands
	for (TFieldIterator<UFunction> FunctionIt(CheatManagerObj->GetClass(), EFieldIteratorFlags::ExcludeSuper); FunctionIt; ++FunctionIt)
	{
		FMetaCheatCommand CheatCommand = FMetaCheatCommand::Create(*FunctionIt);
		if (CheatCommand.IsValid())
		{
			OutAllCheatCommands.Emplace(MoveTemp(CheatCommand));
		}
	}

	CheatManagerObj->TryUpdateDefaultConfigFile();
#endif // WITH_EDITOR
}

// Returns the cheat command associated with specified CheatName meta value
const FMetaCheatCommand& UMetaCheatManagerUtils::GetCheatCommandByCheatName(FName CheatName, const TArray<FMetaCheatCommand>& InAllCheatCommands)
{
	for (const FMetaCheatCommand& CheatCommandIt : InAllCheatCommands)
	{
		if (CheatCommandIt.CheatName.IsEqual(CheatName))
		{
			return CheatCommandIt;
		}
	}

	return FMetaCheatCommand::EmptyCommand;
}

// Registers auto-complete entries for the cheat commands
void UMetaCheatManagerUtils::RegisterAutoCompleteEntries(TArray<FAutoCompleteCommand>& OutCommands, const TArray<FMetaCheatCommand>& InAllCheatCommands)
{
	for (const FMetaCheatCommand& CheatCommandIt : InAllCheatCommands)
	{
		const FAutoCompleteCommand& NewCommand = CheatCommandIt.ToAutoCompleteCommand();
		const bool bIsNew = !OutCommands.ContainsByPredicate([&NewCommand](const FAutoCompleteCommand& CommandIt) { return CommandIt.Command == NewCommand.Command; });
		if (bIsNew)
		{
			OutCommands.Emplace(NewCommand);
		}
	}
}

// Processes the console execution of meta cheat commands
bool UMetaCheatManagerUtils::TryProcessConsoleExec(const TScriptInterface<IMetaCheatManagerInterface>& CheatManager, const TCHAR* const Cmd, FOutputDevice& Ar, UObject* Executor)
{
	if (!ensureMsgf(CheatManager, TEXT("ASSERT: [%i] %s:\n'CheatManager' is not valid!"), __LINE__, *FString(__FUNCTION__)))
	{
		return false;
	}

	UObject* CheatManagerObj = CheatManager.GetObject();
	checkf(CheatManagerObj, TEXT("ERROR: [%i] %s:\n'CheatManagerObj' is null!"), __LINE__, *FString(__FUNCTION__));

	constexpr bool bUseEscape = true;
	const TCHAR* ParsedCmd = Cmd;
	FString CommandName = TEXT("");
	if (!FParse::Token(/*InOut*/ParsedCmd, /*Out*/CommandName, bUseEscape))
	{
		return false;
	}

	// CommandName: is the CheatName (Your.Cheat.Name)
	// Cmd: is the value (if any) that was passed to the cheat
	const FMetaCheatCommand& CheatCommand = GetCheatCommandByCheatName(*CommandName, CheatManager->GetAllCheatCommands());
	if (!CheatCommand.IsValid())
	{
		return false;
	}

	// Get the function name (YourCheatFunction) from the CheatName (Your.Cheat.Name)
	// and append it with the value that was passed to the cheat to process the call
	// YourFunctionCheat Value
	constexpr bool bForceCallWithNonExec = true;
	const FString CmdString = FString::Printf(TEXT("%s %s"), *CheatCommand.FunctionName.ToString(), ParsedCmd);
	return CheatManagerObj->CallFunctionByNameWithArguments(*CmdString, Ar, Executor, bForceCallWithNonExec);
}

// Copyright (c) Yevhenii Selivanov

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
//---
#include "MetaCheatManagerUtils.generated.h"

struct FMetaCheatCommand;

class IMetaCheatManagerInterface;

/**
 * Utility class for shared meta cheat manager functions.
 * Contains static methods used by classes that manage meta cheat commands.
 */
UCLASS()
class METACHEATMANAGER_API UMetaCheatManagerUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Finds and saves all cheat commands marked with 'CheatName' metadata.
	 * @warning its implementation is editor-only since we don't have access to any meta data in builds. */
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void InitAllCheatCommands(const TScriptInterface<IMetaCheatManagerInterface> CheatManager, TArray<FMetaCheatCommand>& OutAllCheatCommands);

	/** Returns the cheat command associated with specified CheatName meta value. */
	UFUNCTION(BlueprintPure, Category = "C++")
	static const FMetaCheatCommand& GetCheatCommandByCheatName(FName CheatName, const TArray<FMetaCheatCommand>& InAllCheatCommands);

	/** Registers auto-complete entries for the cheat commands. */
	static void RegisterAutoCompleteEntries(TArray<struct FAutoCompleteCommand>& OutCommands, const TArray<FMetaCheatCommand>& InAllCheatCommands);

	/** Processes the console execution of meta cheat commands. */
	static bool TryProcessConsoleExec(const TScriptInterface<IMetaCheatManagerInterface>& CheatManager, const TCHAR* const Cmd, FOutputDevice& Ar, UObject* Executor);
};

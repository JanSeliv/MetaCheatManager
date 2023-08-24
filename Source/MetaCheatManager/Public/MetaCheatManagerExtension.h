// Copyright (c) Yevhenii Selivanov

#pragma once

#include "GameFramework/CheatManager.h"
#include "MetaCheatManagerInterface.h"
//---
#include "MetaCheatCommand.h"
//---
#include "MetaCheatManagerExtension.generated.h"

/**
 * Allows to call commands with meta names for Extensions as well.
 * This could be useful for plugins and Game Feature modules that are populating their own cheats.
 *
 * Similar to UMetaCheatManager, UMetaCheatManagerExtension provides functionality to handle
 * cheat commands with custom meta names from the console command, giving flexibility for
 * modular cheat management across different modules of the game.
 */
UCLASS(Config = "MetaCheatManager", DefaultConfig)
class METACHEATMANAGER_API UMetaCheatManagerExtension : public UCheatManagerExtension
                                                        , public IMetaCheatManagerInterface
{
	GENERATED_BODY()

public:
	/** Returns all cheat commands exposed by this cheat manager.
	 * @see UMetaCheatManager::AllCheatCommands */
	virtual const FORCEINLINE TArray<FMetaCheatCommand>& GetAllCheatCommands() const override { return AllCheatCommands; }

protected:
	/** Contains all cheat commands exposed by this cheat manager.
	 * Is automatically saved into config file while in editor to have these commands available in builds where is no access to meta data. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config, meta = (BlueprintProtected))
	TArray<FMetaCheatCommand> AllCheatCommands;

	/** Is overridden to initialize all cheat commands on startup. */
	virtual void PostInitProperties() override;

	/** Is overridden to convert a meta CheatName 'Your.Cheat.Name'
	 * to the function name 'YourCheatFunction' to process the call whenever user enters the command. */
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	/** Garbage things before destroying the Cheat Manager. */
	virtual void BeginDestroy() override;

	/** Is bound to return all initialized meta cheat commands to see them in the console. */
	virtual void RegisterAutoCompleteEntries(TArray<FAutoCompleteCommand>& OutCommands) const override;
};

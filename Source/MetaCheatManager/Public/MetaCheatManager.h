// Copyright (c) Yevhenii Selivanov

#pragma once

#include "GameFramework/CheatManager.h"
#include "MetaCheatManagerInterface.h"
//---
#include "MetaCheatCommand.h"
//---
#include "MetaCheatManager.generated.h"

/**
 * The Meta Cheat Manager plugin introduces a feature to Unreal Engine 5
 * that allows you to call cheat functions with custom meta names from the console command,
 * similar to the 'Your.Cheat.Name' format. This functionality helps replace the
 * 'meta=(OverrideNativeName="Your.Cheat.Name"))' feature, which was available in Unreal Engine 4
 * but became unavailable in Unreal Engine 5 due to the removal of Blueprint Nativization.
 *
 * To use this feature, follow these steps:
 *
 * Step 1: Create a new class that inherits from UMetaCheatManager (not 'UCheatManager').
 *
 * Step 2: In your Player Controller's constructor, set your new cheat manager:
 *         ```
 *         AMyPlayerController::AMyPlayerController()
 *         {
 *             CheatClass = UMyCheatManager::StaticClass();
 *         }
 *         ```
 * Step 3: Add meta=(CheatName="Your.Cheat.Name") to your subclass's functions to
 *         customize command names, for example:
 *         ```
 *         UFUNCTION(meta = (CheatName = "Your.Cheat.Name"))
 *         void YourCheatFunction();
 *         ```
 * Things to keep in mind:
 * - The 'Exec' specifier is not required for these functions. This differs from the
 *   default cheat manager where it's necessary. In this case, 'CheatName' automatically
 *   implies these are 'Exec' functions.
 * - This setup works even if you have your cheat manager implemented only in code.
 *   However, you can also have an inherited blueprint if you prefer.
 * - Upon editor startup, the plugin automatically creates a DefaultMetaCheatManager.ini
 *   config. This file, stored in your project's Config folder, ensures your custom cheat
 *   commands work in the packaged build without any extra steps on your part. This is
 *   necessary as metadata can only be read in the Editor, so it's saved in the config file
 *   for the build.
 */
UCLASS(Config = "MetaCheatManager", DefaultConfig)
class METACHEATMANAGER_API UMetaCheatManager : public UCheatManager
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

	/** Is overridden to initialize all cheat commands on editor startup. */
	virtual void PostInitProperties() override;

	/** Is overridden to convert a meta CheatName 'Your.Cheat.Name'
	 * to the function name 'YourCheatFunction' to process the call whenever user enters the command. */
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

	/** Garbage things before destroying the Cheat Manager. */
	virtual void BeginDestroy() override;

	/** Is bound to return all initialized meta cheat commands to see them in the console. */
	virtual void RegisterAutoCompleteEntries(TArray<FAutoCompleteCommand>& OutCommands) const override;
};

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif

#include "ModuleManager.h"

#include "Runtime/SlateCore/Public/Styling/SlateStyle.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPython, Log, All);


class UNREALENGINEPYTHON_API FUnrealEnginePythonModule : public IModuleInterface
{
public:

	bool PythonGILAcquire();
	void PythonGILRelease();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RunString(char *);
	void RunStringSandboxed(char *);
	void RunFile(char *);
	void RunFileSandboxed(char *, void(*callback)(void *arg), void *arg);

	void UESetupPythonInterpreter(bool);

	FString ScriptsPath;
	FString ZipPath;
	FString AdditionalModulesPath;

private:
	void *ue_python_gil;
	// used by console
	void *main_dict;
	void *local_dict;
	void *main_module;

	TSharedPtr<FSlateStyleSet> StyleSet;
};

struct FScopePythonGIL {
	FScopePythonGIL()
	{
#if defined(UEPY_THREADING)
		UnrealEnginePythonModule = FModuleManager::LoadModuleChecked<FUnrealEnginePythonModule>("UnrealEnginePython");
		safeForRelease = UnrealEnginePythonModule.PythonGILAcquire();
#endif
	}

	~FScopePythonGIL()
	{
#if defined(UEPY_THREADING)
		if (safeForRelease) {
			UnrealEnginePythonModule.PythonGILRelease();
		}
#endif
	}

	FUnrealEnginePythonModule UnrealEnginePythonModule;
	bool safeForRelease;
};





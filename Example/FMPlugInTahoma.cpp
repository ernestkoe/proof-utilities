
/*
  Copyright 1998-2006 FileMaker, Inc.  All Rights Reserved.
  
*/



#include "Support/FMPluginGlobalDefines.h"

#if defined(FMX_WIN_TARGET)
	#include "Windows.h"
#endif

#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXCalcEngine.h"

#include "Support/FMPluginFunctions.h"
#include "Support/FMPluginPrefs.h"



// ====== "FMPluginTahoma.h" =================================================================================
// Header-like information for FMPlugInTahoma.cpp

// General String IDs
	
	// The following strings are usually not localized, but it is your option.
	enum
	{ 
			kPGrp_AddStringID = 102, 
			kPGrp_AppendStringID = 103, 
			kPGrp_EvaluateStringID = 104, 
			kPGrp_StartScriptStringID = 105, 
			kPGrp_UserFormatNumberStringID = 106, 
			kPGrp_FormatNumberStringID = 107, 
			kPGrp_NumToWordsStringID = 108,
			kPGrp_VersionStringID = 109,
			kPGrp_LDistanceStringID = 110,
			kPGrp_SoundexAmericanStringID = 111,
			kPGrp_SoundexDMStringID = 112,
			kPGrp_ExecuteSQLStringID = 113,
			kPGrp_ReduxStringID = 114
	};

	// The following strings are usually localized, but it is your option.
	enum { PGrp_PluginNameStringID = 128, kPGrp_PluginDescStringID = 129 }; 


	FMX_ExternCallPtr	gFMX_ExternCallPtr=NULL;



/* ::=- Do_PluginInit =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=:: */
static fmx::int32 Do_PluginInit(fmx::int16 version) 
{
	// Check the app API version
	if ((version < k70ExtnVersion) || (version > kMaxExtnVersion)) 
	{
		// This version of FileMaker is not supported; let FileMaker disable this 
		// plug-in and report the problem to the user.
		return (kBadExtnVersion);
	}


	// Setup a preference file (Mac OS X) or make changes to preferences settings in the Windows Registry for the plug-in.  
	{
		FMPluginTahomaPrefsWidget		prefsLoader;
		if(prefsLoader.Exists() == false)
		{
			prefsLoader.Create();
		}

		if(prefsLoader.Read() == false)
		{
			return (kDoNotEnable);

		}// prefsLoader Read
	}


	// Register plug-in functions
	fmx::QuadCharAutoPtr	pluginID('P', 'G', 'r', 'p');
	fmx::TextAutoPtr		name;
	fmx::TextAutoPtr		prototype;
	fmx::errcode			err;
	fmx::uint32				regFunctionFlags = fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs;

	Do_GetString(kPGrp_AddStringID, name, true);
	Do_GetString(kPGrp_AddStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_Add, *name, *prototype, 2, 2, regFunctionFlags, Do_PGrp_Add );
	//	if (err != 0)   // You should warn the user or recover. For the purposes of this example, we ignore the result.

	Do_GetString(kPGrp_LDistanceStringID, name, true);
	Do_GetString(kPGrp_LDistanceStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_LDistance, *name, *prototype, 2, 2, regFunctionFlags, Do_PGrp_LDistance );

	Do_GetString(kPGrp_SoundexAmericanStringID, name, true);
	Do_GetString(kPGrp_SoundexAmericanStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_SoundexAmerican, *name, *prototype, 1, 1, regFunctionFlags, Do_PGrp_SoundexAmerican );

	Do_GetString(kPGrp_SoundexDMStringID, name, true);
	Do_GetString(kPGrp_SoundexDMStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_SoundexDM, *name, *prototype, 1, 1, regFunctionFlags, Do_PGrp_SoundexDM );

	Do_GetString(kPGrp_ExecuteSQLStringID, name, true);
	Do_GetString(kPGrp_ExecuteSQLStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_ExecuteSQL, *name, *prototype, 3, 3, regFunctionFlags, Do_PGrp_ExecuteSQL );

	Do_GetString(kPGrp_ReduxStringID, name, true);
	Do_GetString(kPGrp_ReduxStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_Redux, *name, *prototype, 1, 1, regFunctionFlags, Do_PGrp_Redux );

	Do_GetString(kPGrp_AppendStringID, name, true);
	Do_GetString(kPGrp_AppendStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_Append, *name, *prototype, 1, -1, regFunctionFlags, Do_PGrp_Append );

	Do_GetString(kPGrp_EvaluateStringID, name, true);
	Do_GetString(kPGrp_EvaluateStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_Evaluate, *name, *prototype, 1, 1, regFunctionFlags, Do_PGrp_Evaluate );

	Do_GetString(kPGrp_StartScriptStringID, name, true);
	Do_GetString(kPGrp_StartScriptStringID, prototype);
	regFunctionFlags = fmx::ExprEnv::kDisplayInAllDialogs;
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_StartScript, *name, *prototype, 2, 3, regFunctionFlags, Do_PGrp_StartScript );

	Do_GetString(kPGrp_UserFormatNumberStringID, name, true);
	Do_GetString(kPGrp_UserFormatNumberStringID, prototype);
	regFunctionFlags = fmx::ExprEnv::kDisplayCalcFields |  fmx::ExprEnv::kDisplayAutoEnter | fmx::ExprEnv::kDisplayCustomFunctions | fmx::ExprEnv::kDisplayGeneric;
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_UserFormatNumber, *name, *prototype, 1, 1, regFunctionFlags, Do_PGrp_CommonFormatNumber );

	Do_GetString(kPGrp_FormatNumberStringID, name, true);
	Do_GetString(kPGrp_FormatNumberStringID, prototype);
	regFunctionFlags = fmx::ExprEnv::kDisplayAutoEnter;
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_FormatNumber, *name, *prototype, 2, 2, regFunctionFlags, Do_PGrp_CommonFormatNumber );

	Do_GetString(kPGrp_NumToWordsStringID, name, true);
	Do_GetString(kPGrp_NumToWordsStringID, prototype);
	regFunctionFlags = fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs;
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_NumToWords, *name, *prototype, 1, 1, regFunctionFlags, Do_PGrp_NumToWords );

	Do_GetString(kPGrp_VersionStringID, name, true);
	Do_GetString(kPGrp_VersionStringID, prototype);
	err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, kPGrp_Version, *name, *prototype, 0, 0, regFunctionFlags, Do_PGrp_Version );


	// Return kCurrentExtnVersion to enable the plug-in in FileMaker.
	return (kCurrentExtnVersion);
	
} // Do_PluginInit



/* ::=- Do_PluginIdle =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=:: */
static void Do_PluginIdle(FMX_IdleLevel idleLevel) 
{
	// Check idle state
	switch (idleLevel)
	{
		case kFMXT_UserIdle:
		{
			// The plug-in is free to do idle processing at this time.
		}
		break;

		case kFMXT_UserNotIdle:
		case kFMXT_ScriptPaused:
		case kFMXT_ScriptRunning:
		case kFMXT_Unsafe:
		{
			// The plug-in should wait and do nothing at this time.
		}
		break;
	}// switch idleLevel

} // Do_PluginIdle



/* ::=- Do_PluginShutdown =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=:: */
static void Do_PluginShutdown(void) 
{

	// Unregister plug-in functions
	fmx::QuadCharAutoPtr	pluginID('P', 'G', 'r', 'p');
	fmx::errcode			err;

	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_Add );
	// if (err != 0)  // There is likely nothing to do in response. For the purposes of this example, we ignore the result.

	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_LDistance );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_SoundexAmerican );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_SoundexDM );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_Append );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_Evaluate );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_ExecuteSQL );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_Redux );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_StartScript );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_UserFormatNumber );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_FormatNumber );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_NumToWords );
	err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, kPGrp_Version );

} // Do_PluginShutdown



/* ::=- FMExternCallProc "Main" =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=:: */
void FMX_ENTRYPT FMExternCallProc(FMX_ExternCallPtr pb)
{
	// Setup global defined in FMxExtern.h
	gFMX_ExternCallPtr = pb;

	// Message dispatcher
	switch (gFMX_ExternCallPtr->whichCall) 
	{
		case kFMXT_Init:
		{
			gFMX_ExternCallPtr->result = Do_PluginInit(gFMX_ExternCallPtr->extnVersion);
		}
		break;
			
		case kFMXT_Idle:
		{
			Do_PluginIdle(gFMX_ExternCallPtr->parm1);
		}
		break;
			
		case kFMXT_Shutdown:
		{
			Do_PluginShutdown();
		}
		break;
			
		case kFMXT_DoAppPreferences:
		{
			Do_PluginPrefs();
		}
		break;

		case kFMXT_GetString:
		{
			Do_GetString(gFMX_ExternCallPtr->parm1, gFMX_ExternCallPtr->parm2,
						gFMX_ExternCallPtr->parm3, reinterpret_cast<FMX_Unichar*>(gFMX_ExternCallPtr->result));
		}
		break;

	}// switch whichCall

} // FMExternCallProc
	

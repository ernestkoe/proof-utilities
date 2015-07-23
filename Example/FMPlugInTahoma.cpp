
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

/****************************************************************************
 * Functions                                                                *
 ****************************************************************************/

static const unsigned long DEFAULT_FLAGS = 
    fmx::ExprEnv::kMayEvaluateOnServer
  | fmx::ExprEnv::kDisplayInAllDialogs;

static struct plugin_function_definition {
  short id; const char *signature; short min, max; fmx::uint32 flags; 
  fmx::errcode (__stdcall *callable)(short, const fmx::ExprEnv&, 
  const fmx::DataVect&, fmx::Data&);
} plugin_functions[] = {
  {  110, "PGrp_LDistance( text1; text2 )", 2, 2, DEFAULT_FLAGS,
       &Do_PGrp_LDistance },
  {  111, "PGrp_SoundexAM( text )"        , 1, 1, DEFAULT_FLAGS,
       &Do_PGrp_SoundexAmerican },
  {  112, "PGrp_SoundexDM( text )"        , 1, 1, DEFAULT_FLAGS,
       &Do_PGrp_SoundexDM },
  {  109, "PGrp_Version"                  , 0, 0, DEFAULT_FLAGS,
       &Do_PGrp_Version },
  {  114, "PGrp_Redux( text )"            , 1, 1, DEFAULT_FLAGS,
       &Do_PGrp_Redux },
  {    0, NULL }
};

static fmx::int32 
Do_PluginInit2(fmx::int16 version) 
{
    // Check the app API version
    if ((version < k70ExtnVersion) || (kMaxExtnVersion < version))
        return (kBadExtnVersion);

    // Register plug-in functions
    fmx::QuadCharAutoPtr plugin('P', 'G', 'r', 'p');
    fmx::errcode error;
    struct plugin_function_definition *cursor;

    for (cursor = plugin_functions; cursor->signature; ++cursor) {
        fmx::TextAutoPtr name, desc; const char *parenthesis;

        desc->Assign(cursor->signature, fmx::Text::kEncoding_UTF8);
        if (!(parenthesis = strchr(cursor->signature, '(')))
            name->Assign(cursor->signature, fmx::Text::kEncoding_UTF8);
        else
            name->AssignWithLength(cursor->signature, parenthesis -
                    cursor->signature, fmx::Text::kEncoding_UTF8);
        if ((error = fmx::ExprEnv::RegisterExternalFunction(*plugin,
                cursor->id, *name, *desc, cursor->min, cursor->max, 
                cursor->flags, cursor->callable))) {
            while (cursor-- != plugin_functions)
                fmx::ExprEnv::UnRegisterExternalFunction(*plugin, cursor->id);
            return kDoNotEnable;
        }
    }
    return kCurrentExtnVersion;
}

static void
Do_PluginShutdown2(void)
{
    fmx::QuadCharAutoPtr plugin('P', 'G', 'r', 'p');
    plugin_function_definition *cursor; 

    for (cursor = plugin_functions; cursor->signature; ++cursor)
        fmx::ExprEnv::UnRegisterExternalFunction(*plugin, cursor->id);
}



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



static void 
Do_PluginIdle(FMX_IdleLevel idleLevel) 
{
    // Check idle state
    switch (idleLevel) {
        case kFMXT_UserIdle:
            // The plug-in is free to do idle processing at this time.
            break;
        case kFMXT_UserNotIdle:
        case kFMXT_ScriptPaused:
        case kFMXT_ScriptRunning:
        case kFMXT_Unsafe:
            // The plug-in should wait and do nothing at this time.
            break;
    }
}

static void 
Do_PluginShutdown(void) 
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

static char* plugin_strings[] = {
    "PGrp"   /* Plugin-ID */
       "1"   /* fixed value */
       "n"   /* no configuration */
       "n"   /* no old API */
       "Y"   /* fixed value */
       "n"   /* no idle time */
       "nn", /* fixed value */
    "Tahoma",
    "Tahoma String Utilities plug-in. This plug-in was derived from the "
        "FMExample plug-in."
};

static void
Do_GetString2(fmx::uchar type, fmx::ptrtype limit, fmx::unichar *target)
{
    const char *source; fmx::ptrtype i;

    switch (type) {
        case kFMXT_OptionsStr  : source = plugin_strings[0]; break;
        case kFMXT_NameStr     : source = plugin_strings[1]; break;
        case kFMXT_AppConfigStr: source = plugin_strings[2]; break;
    }
    for (i = 0; i < limit && source[i] != '\0'; ++i)
        target[i] = source[i];
}



void FMX_ENTRYPT 
FMExternCallProc(FMX_ExternCallPtr pb)
{
    // Setup global defined in FMxExtern.h
    gFMX_ExternCallPtr = pb;

    // Message dispatcher
    switch (gFMX_ExternCallPtr->whichCall) {
        case kFMXT_Init:
            gFMX_ExternCallPtr->result = Do_PluginInit2(
                    gFMX_ExternCallPtr->extnVersion);
            break;
        case kFMXT_Idle:
            Do_PluginIdle(gFMX_ExternCallPtr->parm1);
            break;
        case kFMXT_Shutdown:
            Do_PluginShutdown2();
            break;
        case kFMXT_DoAppPreferences:
            Do_PluginPrefs();
            break;
        case kFMXT_GetString:
            Do_GetString2(pb->parm1, pb->parm3, 
                reinterpret_cast<FMX_Unichar*>(pb->result));
           break;
    }
}
	

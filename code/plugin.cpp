
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

/****************************************************************************
 * Functions                                                                *
 ****************************************************************************/

static const unsigned long DEFAULT_FLAGS = 
    fmx::ExprEnv::kMayEvaluateOnServer
  | fmx::ExprEnv::kDisplayInAllDialogs;

static struct plugin_function_definition {
  short id; const char *signature; short min, max; fmx::uint32 flags; 
  fmx::errcode (
#ifdef FMX_WIN_TARGET
  __stdcall
#endif  
  *callable)(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
} plugin_functions[] = {
  {    4, "PGrp_LDistance( text1; text2 )", 2, 2, DEFAULT_FLAGS,
       &Do_PGrp_LDistance },
  {    5, "PGrp_SoundexAM( text )"        , 1, 1, DEFAULT_FLAGS,
       &Do_PGrp_SoundexAmerican },
  {    6, "PGrp_SoundexDM( text )"        , 1, 1, DEFAULT_FLAGS,
       &Do_PGrp_SoundexDM },
  {    1, "PGrp_Version"                  , 0, 0, DEFAULT_FLAGS,
       &Do_PGrp_Version },
  {    7, "PGrp_Redux( text )"            , 1, 1, DEFAULT_FLAGS,
       &Do_PGrp_Redux },
  {    2, "PGrp_ExecuteSQL( expression )", 
                                            1, 1, DEFAULT_FLAGS,
       &Do_PGrp_ExecuteSQL },
  {    3, "PGrp_ExecuteScript( filename; scriptname {; parameter} )", 
                                            2, 3, DEFAULT_FLAGS,
       &Do_PGrp_StartScript },
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


static const char* plugin_strings[] = {
    "PGrp"   /* Plugin-ID */
       "1"   /* fixed value */
       "n"   /* no configuration */
       "n"   /* no old API */
       "Y"   /* fixed value */
       "n"   /* no idle time */
       "nn", /* fixed value */
    "Proof Utilities",
    "The Proof Utilities plug-in adds a number of utility functions to "
            "FileMaker Pro."
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
        case kFMXT_Shutdown:
            Do_PluginShutdown2();
            break;
        case kFMXT_GetString:
            Do_GetString2(pb->parm1, pb->parm3, 
                reinterpret_cast<FMX_Unichar*>(pb->result));
           break;
    }
}
	

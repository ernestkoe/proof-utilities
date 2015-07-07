//////////////////////////////////////////////////////////////////////////////////////
//  
//  FMTConfig.h - Definitions used to configure 24U FM Template
//
//  Version 2.2, Copyright �2002-2006 by 24U Software. All rights reserved.
//
//  Written by Tomas Zahradnicky, HOnza Koudelka, and Josef Andrysek
//
////////////////////////////////////////////////////////////////////////////////
//
//  The latest version of 24U FM Template is available for download from:
//
//  http://24uSoftware.com/FMTemplate  or  http://www.NewMillennium.com/
//
////////////////////////////////////////////////////////////////////////////////
//
//  24U Sample Code - Terms of use
//
//  You may incorporate this sample code into your products without
//  restriction, though the code has been provided "AS IS" and the
//  responsibility for its operation is 100% yours.
//
//  You are not however permitted to redistribute the source
//  as "24U Sample Code" after having made changes.
//
//  If you are going to redistribute the source with references to
//  24U Software, we require that you make it clear in the source that
//  the code was descended from 24U Sample Code, but that you've made changes.
//
////////////////////////////////////////////////////////////////////////

//
//  This header file contains all necessary constant definitions for building
//  your plug-in. Start your plug-in development by carefully configuring this
//  file, and you'll be ready to start coding for your external functions.
//  This header should not be included into your sources. It's only purpose
//  is to configure the template. If you need to define some constatns to be
//  used by both your code and the template, define them in your own header
//  file, such as FMPlugin.h, and include that header file here.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __FMTConfig_h__
#define __FMTConfig_h__

#include "FMPlugin.h"


////////////////////////////////////////////////////////////////////////////////
//
//  Shared constants
//
/////////////////////////////////////////////////////////////////////////////
//
//  The following constants configure your plug-in to behave well under
//  FileMaker applications and to be well recognized on both Mac OS and
//  Windows operating systems. It is recommended to always set all these
//  contants up to contain accurate information about your plug-in.
//

#define PLUGIN_ID_STRING        "PGrp"

#define PLUGIN_NAME             "Tahoma"
#define PLUGIN_VERSION          "0.9"

#define PLUGIN_VERSION_MAJOR		0x0
#define PLUGIN_VERSION_MINOR		0x9
#define PLUGIN_VERSION_BUILD		0x0


#define PLUGIN_INFO             "This plug-in adds a number of utility functions to FileMaker Pro."

#define COMPANY_NAME            "The Proof Group LLC"
#define COPY_STATEMENT          "Copyright �2007 by " COMPANY_NAME

#define WANT_IDLE               0           // set to 1 if you want idle-processing
#define WANT_PREFERENCES        0           // set to 1 if you want to handle preferences


////////////////////////////////////////////////////////////////////////////////
//
//  Mac OS X constants
//
/////////////////////////////////////////////////////////////////////////////
//
//  Define Mac OS X specific information for you plug-in below.
//

#define PLUGIN_BUNDLE_ID    "com.proofgroup.tahoma"


////////////////////////////////////////////////////////////////////////////////
//
//  Windows constants
//
/////////////////////////////////////////////////////////////////////////////
//
//  Define Windows specific information for you plug-in below.
//

#define FILE_NAME_PC        "Tahoma.fmx"  // A hint for user who accidentally renames your plug-in

#define PLUGIN_INFO_PC      "This file is a FileMaker applications plug-in. To make this file effective, drop it into FileMaker's Extensions folder and launch the FileMaker application."

#define COMPANY_URL         "http://www.proofgroup.com/"


////////////////////////////////////////////////////////////////////////////////
//
//  Function definitions
//
/////////////////////////////////////////////////////////////////////////////
//
//  Do not modify the following enum, use the defined constants to configure
//  minimum and maximum parameters required for each function
//  (the FUNCTION_X_PARAMS macros)
//

#ifndef __PLIST__
enum {
    kAtLeast_0_Parameters = 0,
    kAtLeast_1_Parameter  = 1,
    kAtLeast_2_Parameters = 2,
    kAtLeast_3_Parameters = 3,
    kAtLeast_4_Parameters = 4,
    kAtLeast_5_Parameters = 5,
    kAtLeast_6_Parameters = 6,
    kAtLeast_7_Parameters = 7,
    kAtLeast_8_Parameters = 8,
    kAtLeast_9_Parameters = 9,
    kAtLeast_10_Parameters = 10,

    kAtWorst_0_Parameters = 0,
    kAtWorst_1_Parameter  = 1,
    kAtWorst_2_Parameters = 2,
    kAtWorst_3_Parameters = 3,
    kAtWorst_4_Parameters = 4,
    kAtWorst_5_Parameters = 5,
    kAtWorst_6_Parameters = 6,
    kAtWorst_7_Parameters = 7,
    kAtWorst_8_Parameters = 8,
    kAtWorst_9_Parameters = 9,
    kAtWorst_10_Parameters = 10,
    kAtWorst_Unlimited_Parameters = -1
};
#endif // __PLIST__

/////////////////////////////////////////////////////////////////////////////
//
//  Define your external functions here. For each of your functions,
//  uncomment the first commented block of defines, and set up the
//  function's name, flags, parameters and prototype.
//

#define FUNCTION_1_C_NAME         PGrp_Version
#define FUNCTION_1_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_1_PARAMS         kAtLeast_1_Parameter, kAtWorst_1_Parameter
#define FUNCTION_1_PROTOTYPE      "PGrp_Version( versionFormat )"

#define FUNCTION_2_C_NAME         PGrp_ExecuteSQL
#define FUNCTION_2_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_2_PARAMS         kAtLeast_1_Parameter, kAtWorst_3_Parameters
#define FUNCTION_2_PROTOTYPE      "PGrp_ExecuteSQL( expression )"

#define FUNCTION_3_C_NAME         PGrp_ExecuteScript
#define FUNCTION_3_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_3_PARAMS         kAtLeast_2_Parameters, kAtWorst_3_Parameters
#define FUNCTION_3_PROTOTYPE      "PGrp_ExecuteScript( filename; scriptname {; parameter} )"

#define FUNCTION_4_C_NAME         PGrp_LDistance
#define FUNCTION_4_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_4_PARAMS         kAtLeast_2_Parameters, kAtWorst_2_Parameters
#define FUNCTION_4_PROTOTYPE      "PGrp_LDistance( text1 ; text2 )"

#define FUNCTION_5_C_NAME         PGrp_SoundexAmerican
#define FUNCTION_5_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_5_PARAMS         kAtLeast_1_Parameter, kAtWorst_1_Parameter
#define FUNCTION_5_PROTOTYPE      "PGrp_SoundexAmerican( text )"

#define FUNCTION_6_C_NAME         PGrp_SoundexDM
#define FUNCTION_6_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_6_PARAMS         kAtLeast_1_Parameter, kAtWorst_1_Parameter
#define FUNCTION_6_PROTOTYPE      "PGrp_SoundexDM( text )"

#define FUNCTION_7_C_NAME         PGrp_StringRedux
#define FUNCTION_7_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_7_PARAMS         kAtLeast_1_Parameter, kAtWorst_1_Parameter
#define FUNCTION_7_PROTOTYPE      "PGrp_StringRedux( text )"

//#define FUNCTION_8_C_NAME         X24FT_Function8
//#define FUNCTION_8_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
//#define FUNCTION_8_PARAMS         kAtLeast_0_Parameters, kAtWorst_Unlimited_Parameters
//#define FUNCTION_8_PROTOTYPE      "X24FT_Function8( param1 ; ... )"

//#define FUNCTION_9_C_NAME         X24FT_Function9
//#define FUNCTION_9_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
//#define FUNCTION_9_PARAMS         kAtLeast_0_Parameters, kAtWorst_Unlimited_Parameters
//#define FUNCTION_9_PROTOTYPE      "X24FT_Function9( param1 ; ... )"

//#define FUNCTION_10_C_NAME         X24FT_Function10
//#define FUNCTION_10_FLAGS          fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs
//#define FUNCTION_10_PARAMS         kAtLeast_0_Parameters, kAtWorst_Unlimited_Parameters
//#define FUNCTION_10_PROTOTYPE      "X24FT_Function10( param1 ; ... )"

////////////////////////////////////////////////////////////////////////////////

#endif // __FMTConfig_h__

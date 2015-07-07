//////////////////////////////////////////////////////////////////////////////////////
//  
//  FMPlugin.h - Definitions shared across the plug-in sources
//
//  Version 2.2, Copyright �2002-2006 by 24U Software. All rights reserved.
//
//  Written by Tomas Zahradnicky & HOnza Koudelka
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
//  This header file is dedicated for constants that need to be globally
//  available across all plug-in sources. This is the only file expected
//  to define constants to be used by both the template sources and your
//  own sources.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __FMPlugin_h__
#define __FMPlugin_h__

/////////////////////////////////////////////////////////////////////////////
//
//  Don't modify the following constats, they are not yet fully customizable
//  in this version of 24U FM Template. These constants define IDs of
//  localizable string resources, and are required for the plug-in to
//  work properly.
//

#define PLUGIN_NAME_ID          1
#define PLUGIN_PREFERENCES_ID   2
#define PLUGIN_VERSION_ID       3

#define FUNCTION_OFFSET       100


/////////////////////////////////////////////////////////////////////////////
//
//  We are including this header in FMTConfig.h which is included by
//  FMTemplate.plc. The following definitions are for use in code only,
//  so we are excluding them from Plist definition from.
//

#ifndef __PLIST__

  #if __cplusplus
    extern "C" {
  #endif
  
  ////////////////////////////////////////////
  //
  //  Place your own global definitions here
  //
  ////////////////////////////////////////////
  
  #if __cplusplus
    }
  #endif

#endif //__PLIST__

/////////////////////////////////////////////////////////////////////////////

#endif // __FMPlugin_h__

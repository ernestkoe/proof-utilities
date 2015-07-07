//////////////////////////////////////////////////////////////////////////////////////
//  
//  FMTemplatePrefix.h - Prefix header file for 24U FM Template on Mac OS X 10.4
//
//  Version 2.2, Copyright ©2006 by 24U Software. All rights reserved.
//
//  Written by Tomas Rycl
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
// This prefix is used to override new CFSTR calls in Mac OS 10.4 which
// casuses FM crash when unloading plugin.
//
////////////////////////////////////////////////////////////////////////////////
 
#if defined( __GNUC__ )

  #undef __CONSTANT_CFSTRINGS__
  
  #include <CoreServices/CoreServices.h>
  #include <Carbon/Carbon.h>

#endif
    




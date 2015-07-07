//////////////////////////////////////////////////////////////////////////////////////
//  
//  FMUtils.cpp - Useful utility functions for plug-ins based on 24U FM Template
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
//  Use this file as a part of all compiled parts of your plug-in.
//  It contains a few functions you might find handy when writing
//  your plug-in's code.
//
////////////////////////////////////////////////////////////////////////////////


#include "FMTConfig.h"

#include "FMTemplate/FMTemplate.h" 

void ReadString( FMX_Unichar* ioBuffer, FMX_ULong iBufferSize, FMX_ULong iStringID )
{
    #if FMX_WIN_TARGET
        LoadStringW(GetPluginInstance(), iStringID, (LPWSTR)ioBuffer, iBufferSize);
    #endif
    
    #if FMX_MAC_TARGET
        
        ioBuffer[0] = 0;

        // Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
        CFStringRef     strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR( "String%d" ), iStringID );
        
        // Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
        CFStringRef     osxStr = CFBundleCopyLocalizedString( GetPluginBundle(), strIdStr, strIdStr, CFSTR ("Localizable") );

        if((osxStr != NULL) && (osxStr != strIdStr))
        {
            long    osxStrLen = CFStringGetLength(osxStr);
            if( osxStrLen < (long)(iBufferSize-1) )
            {
                CFStringGetCharacters(osxStr, CFRangeMake(0,osxStrLen), (UniChar*)(ioBuffer));
                ioBuffer[osxStrLen] = 0;
                
            };// osxStrLen
            
            CFRelease( osxStr );
            
        };// osxStr
        
        CFRelease( strIdStr );        
    
    #endif
}

#if FMX_MAC_TARGET

    unsigned long Sub_OSXLoadString(unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax)
    {
        unsigned long       returnResult = 0;
        
        if( (intoHere != NULL) && (intoHereMax > 1) )
        {
			// Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
            CFStringRef     strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR( "String%d" ), stringID);
            
            // Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
            CFStringRef     osxStr = CFBundleCopyLocalizedString( reinterpret_cast<CFBundleRef>(gFMX_ExternCallPtr->instanceID), strIdStr, strIdStr, CFSTR("Localizable") );
            if((osxStr != NULL) && (osxStr != strIdStr))
            {
                long    osxStrLen = CFStringGetLength(osxStr);
                if( osxStrLen < (intoHereMax-1) )
                {
                    CFRange     allChars;
                    allChars.location = 0;
                    allChars.length = osxStrLen;
                    
                    CFStringGetCharacters(osxStr, allChars, (UniChar*)(intoHere));
                    intoHere[osxStrLen] = 0x0000;
                    returnResult = (unsigned long)osxStrLen;
                    
                };// osxStrLen
                
                CFRelease( osxStr );
                
            };// osxStr
            
			CFRelease( strIdStr );
            
        };// intoHere
            
    return(returnResult);
    
    } // Sub_OSXLoadString
    
#endif



static void Do_GetString(unsigned long whichString, FMX_ULong winLangID, FMX_Long resultsize, FMX_Unichar* string)
{
   // bool        processedSpecialStringID = false;
    
    // UNUSED
    winLangID=winLangID;

    
    #if FMX_WIN_TARGET
        LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)whichString, (LPWSTR)string, resultsize);
    #endif

    #if FMX_MAC_TARGET
        Sub_OSXLoadString(whichString, string, resultsize);
    #endif


} // Do_GetString (FMX_Unichar* version)



enum { kXMpl_GetStringMaxBufferSize = 1024 };

static void Do_GetString(unsigned long whichStringID, fmx::TextAutoPtr& intoHere, bool stripFunctionParams)
{
    FMX_Unichar         tempBuffer[kXMpl_GetStringMaxBufferSize] = {0,};

    Do_GetString(whichStringID, 0, kXMpl_GetStringMaxBufferSize, tempBuffer);
    intoHere->AssignUnicode(tempBuffer);
    
    if(stripFunctionParams)
    {
        // The string for this whichStringID is a Function Prototype, but all the plug-in needs now is the Function Name by itself.

        fmx::TextAutoPtr        parenToken;
        parenToken->Assign("(");

        unsigned long       originalSize = intoHere->GetSize();
        unsigned long       firstParenLocation; 
        firstParenLocation = intoHere->Find(*parenToken, 0);
    
        intoHere->DeleteText(firstParenLocation, originalSize-firstParenLocation);

    };// stripFunctionParams

} // Do_GetString (TextAutoPtr version)


fmx::errcode RegisterExternalFunction( FMX_UShort iFunctionID, FMX_Short iMinArgs, FMX_Short iMaxArgs, FMX_ULong iTypeFlags, fmx::ExtPluginType iFunctionPtr)
{
    fmx::QuadCharAutoPtr    pluginID(PLUGIN_ID_STRING[0], PLUGIN_ID_STRING[1], PLUGIN_ID_STRING[2], PLUGIN_ID_STRING[3]);
    fmx::TextAutoPtr        name;
    fmx::TextAutoPtr        prototype;
    fmx::errcode            err;

    Do_GetString(iFunctionID+FUNCTION_OFFSET, name, true);
    Do_GetString(iFunctionID+FUNCTION_OFFSET, prototype, false);

    err = fmx::ExprEnv::RegisterExternalFunction ( *pluginID, iFunctionID, *name, *prototype, iMinArgs, iMaxArgs, iTypeFlags, iFunctionPtr );
    
    return err;
}

fmx::errcode UnRegisterExternalFunction( FMX_UShort iFunctionID )
{
    fmx::QuadCharAutoPtr    pluginID(PLUGIN_ID_STRING[0], PLUGIN_ID_STRING[1], PLUGIN_ID_STRING[2], PLUGIN_ID_STRING[3]);

    return fmx::ExprEnv::UnRegisterExternalFunction( *pluginID, iFunctionID );
}
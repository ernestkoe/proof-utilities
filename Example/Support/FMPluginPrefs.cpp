
/*
  Copyright 1998-2006 FileMaker, Inc.  All Rights Reserved.
  
*/



#include "FMPluginGlobalDefines.h"


#if defined(FMX_WIN_TARGET)
	#include "Windows.h"
	#include <locale.h>
#endif // FMX_WIN_TARGET

#if defined(FMX_MAC_TARGET)
	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>
#endif // FMX_MAC_TARGET


#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXCalcEngine.h"

#include "FMPluginPrefs.h"
#include "FMPluginFunctions.h"


#include "resource.h"


PGrp_Preferences	gFMPluginTahomaPrefs;

#if defined(FMX_MAC_TARGET)

	EventHandlerUPP		gDialogUPP;
	
#endif // FMX_MAC_TARGET



/* ::=- Do_PluginPrefs =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=:: */
void Do_PluginPrefs(void)
{
	// The Preferences dialog box is put up here.
	FMPluginTahomaPrefsDialog		prefsDialog;
	if(prefsDialog.DisplayAndGetPrefs())
	{
		// Update preferences on file.
		FMPluginTahomaPrefsWidget		prefsUpdater;
		if(prefsUpdater.Exists())
		{
			prefsUpdater.Write();

		}// prefsLoader Exists

	}// DisplayAndGetPrefs

} // Do_PluginPrefs



FMPluginTahomaPrefsDialog::FMPluginTahomaPrefsDialog(void) : 
		mDialogRef(NULL)
{
#if defined(FMX_WIN_TARGET)
	mParentDialogRef = (unsigned long *)(GetActiveWindow());
#endif // FMX_WIN_TARGET

#if defined(FMX_MAC_TARGET)
	gDialogUPP =  NewEventHandlerUPP(PrefsDialog_MacProc);
	

	// The Preferences dialog box is set up here but does not display or activate.
		IBNibRef	tempNibRef;
    	WindowRef 	tempDialogRef = NULL;
    	OSStatus	osxResult;
    	
		osxResult = CreateNibReferenceWithCFBundle(reinterpret_cast<CFBundleRef>(gFMX_ExternCallPtr->instanceID), CFSTR("FMPluginTahoma"), &tempNibRef);
    	if(osxResult == noErr)
    	{
   			osxResult = CreateWindowFromNib( tempNibRef, CFSTR("FMPluginTahoma"), &tempDialogRef );  // FMPluginTahoma
        	DisposeNibReference(tempNibRef);
        	
        	if((osxResult == noErr) && (tempDialogRef != NULL))
        	{
       			mDialogRef = (unsigned long*)(tempDialogRef);

 		    	EventTypeSpec	dialogSpec = { kEventClassCommand, kEventCommandProcess };
				osxResult = InstallWindowEventHandler( tempDialogRef, gDialogUPP, 1, &dialogSpec, static_cast<void*>(tempDialogRef), NULL );
				
				OSX_SetEditWithDefaultFormatString(tempDialogRef, true);

        	}// osxResult CreateWindowFromNib
   			
    	}// osxResult CreateNibReferenceWithCFBundle
		
#endif // FMX_MAC_TARGET

} // FMPluginTahomaPrefsDialog::FMPluginTahomaPrefsDialog



FMPluginTahomaPrefsDialog::~FMPluginTahomaPrefsDialog(void) 
{
#if defined(FMX_WIN_TARGET)
	mDialogRef = NULL;
	SetFocus((HWND)mParentDialogRef);
	mParentDialogRef = NULL;
#endif // FMX_WIN_TARGET

#if defined(FMX_MAC_TARGET)
	DisposeWindow( (WindowRef)(mDialogRef) );
    DisposeEventHandlerUPP(gDialogUPP);
#endif // FMX_MAC_TARGET

} // FMPluginTahomaPrefsDialog::~FMPluginTahomaPrefsDialog



bool FMPluginTahomaPrefsDialog::DisplayAndGetPrefs(void)
{
	bool			tempReturn = false;
	fmx::uint16*	textEditResult = NULL;


	#if defined(FMX_WIN_TARGET)
		textEditResult = (fmx::uint16 *)(DialogBox((HINSTANCE)(gFMX_ExternCallPtr->instanceID), MAKEINTRESOURCE(kFMEX_RES_DIALOGID), (HWND)mParentDialogRef, (DLGPROC)PrefsDialog_WinProc));
	#endif

	#if defined(FMX_MAC_TARGET)
		SetWRefCon( (WindowRef)(mDialogRef), 0 );
 		ShowWindow( (WindowRef)(mDialogRef) );
 		
 		RunAppModalLoopForWindow( (WindowRef)(mDialogRef) );
 		textEditResult = (fmx::uint16 *)( GetWRefCon((WindowRef)(mDialogRef)) );
 		
	    HideWindow((WindowRef)(mDialogRef));
	#endif // FMX_MAC_TARGET


	if(textEditResult != NULL)
	{
		gFMPluginTahomaPrefs.formattingStringTxt->AssignUnicode(textEditResult);
		delete [] textEditResult;

		tempReturn = true;

	} // textEditResult


return(tempReturn);
} // FMPluginTahomaPrefsDialog::DisplayAndGetPrefs



#if defined(FMX_WIN_TARGET)
	static const wchar_t *sWinRegPathChars = L"SOFTWARE\\FileMaker\\Tahoma Plug";
	static const wchar_t *sWinRegSubKeyChars = L"Format";
#endif // FMX_WIN_TARGET



FMPluginTahomaPrefsWidget::FMPluginTahomaPrefsWidget(void)
{
}



FMPluginTahomaPrefsWidget::~FMPluginTahomaPrefsWidget(void)
{
}



bool FMPluginTahomaPrefsWidget::Exists(void)
{
	bool	tempReturn = false;


	#if defined(FMX_WIN_TARGET)
		long		regResult = ERROR_BADKEY;
		HKEY		tempKey;
		
		regResult = RegOpenKeyExW(HKEY_CURRENT_USER, (LPCWSTR)sWinRegPathChars, 0, KEY_READ, &tempKey);
		if(regResult == ERROR_SUCCESS)
		{
			long debugErr = RegCloseKey(tempKey);
			tempReturn = true;

		}// regResult is ERROR_SUCCESS
		
	#endif // FMX_WIN_TARGET


	#if defined(FMX_MAC_TARGET)
		FSRef			prefsFileFSRef;
		HFSUniStr255	prefsFileName;
		
		if( OSX_GetPrefsFileLocationAndName(&prefsFileFSRef, &prefsFileName) )
		{
			// Only FSSpecs that exist on disk can be made into FSRefs.
			FSRef		tempOSXfileref;
			OSErr		tempFSerr = FSMakeFSRefUnicode(&prefsFileFSRef, prefsFileName.length, prefsFileName.unicode, 0, &tempOSXfileref);
						 
			tempReturn = (tempFSerr == noErr);
			
		}// prefFileSpec
		
	#endif // FMX_MAC_TARGET


return(tempReturn);
} // FMPluginTahomaPrefsWidget::Exists



bool FMPluginTahomaPrefsWidget::Create(void) 
{
	bool	tempReturn = false;


	#if defined(FMX_WIN_TARGET)
		long		regResult = ERROR_BADKEY;
		HKEY		tempKey;
		
		// This creates the parent key path where all the plug-in preferencess are stored.
		regResult = RegCreateKeyExW(HKEY_CURRENT_USER,
									(LPCWSTR)sWinRegPathChars, 
									0,
									NULL,
									REG_OPTION_NON_VOLATILE,
									KEY_ALL_ACCESS,
									NULL,
									&tempKey,
									NULL
									);

		if(regResult == ERROR_SUCCESS)
		{
			long debugErr = RegCloseKey(tempKey);

			// This creates the formatting string preference.
			tempReturn = Write();

		}// regResult is ERROR_SUCCESS
		
	#endif // FMX_WIN_TARGET


	#if defined(FMX_MAC_TARGET) 
		GetDefaults(); // This gets the default string if none has been specified already.
		
		tempReturn = Exists();
		FSRef			prefsFileFSRef;
		HFSUniStr255	prefsFileName;

		if( (!tempReturn) && (OSX_GetPrefsFileLocationAndName(&prefsFileFSRef, &prefsFileName)) )
		{
			FSCatalogInfo	fileMetaData;
			FileInfo*		fileInfo = (FileInfo*)&fileMetaData.finderInfo;
			fileInfo->fileType = 'TEXT';
			fileInfo->fileCreator = 'Xmpl';
			fileInfo->finderFlags = 0;
			fileInfo->location.h = 0;
			fileInfo->location.v = 0;
			fileInfo->reservedField = 0;

			OSErr		tempFSerr = FSCreateFileUnicode(&prefsFileFSRef, prefsFileName.length, prefsFileName.unicode, kFSCatInfoFinderInfo, &fileMetaData, NULL, NULL);
			tempReturn = (tempFSerr == noErr);
			
			if(tempReturn)
			{
				tempReturn = Write(); // Currently the default is set, so this writes that out to the currently empty file.
			}
			
		}// OSX_GetPrefsFileLocationAndName
		
	#endif // FMX_MAC_TARGET


return(tempReturn);
} // FMPluginTahomaPrefsWidget::Create



bool FMPluginTahomaPrefsWidget::Read(void) 
{
	bool	tempReturn = false;


	GetLocaleSettings();
	

	#if defined(FMX_WIN_TARGET)
		long		regResult = ERROR_BADKEY;
		HKEY		tempKey;

		regResult = RegOpenKeyExW(HKEY_CURRENT_USER, (LPCWSTR)sWinRegPathChars, 0, KEY_READ | KEY_QUERY_VALUE, &tempKey);
		if(regResult == ERROR_SUCCESS)
		{
			// This creates the formatting string preference which is the only preference the example plug-in has.
				
				DWORD lpExpectedSize =
				(kPGrp_FilterMaxBufferSize-1) *
				sizeof(fmx::uint16);
				DWORD lpExpectedType = REG_SZ;
				mDefaultFormatString[0] = 0;
				mDefaultFormatResult = 0;

				regResult = RegQueryValueExW(tempKey,
								(LPCWSTR)sWinRegSubKeyChars,
								0,
								&lpExpectedType,
								(LPBYTE)mDefaultFormatString,
								&lpExpectedSize );

				if(regResult == ERROR_SUCCESS)
				{
					mDefaultFormatResult = (lpExpectedSize
					/ sizeof(fmx::uint16) - 1); // -1 to ignore trailing null
					gFMPluginTahomaPrefs.formattingStringTxt->AssignUnicode(mDefaultFormatString);
					tempReturn = true;

				}// regResult

			long debugErr = RegCloseKey(tempKey);

		}// regResult is ERROR_SUCCESS

	#endif // FMX_WIN_TARGET



	#if defined(FMX_MAC_TARGET)
		FSRef			prefsFileFSRef;
		HFSUniStr255	prefsFileName;
		
		tempReturn = OSX_GetPrefsFileLocationAndName(&prefsFileFSRef, &prefsFileName);
		if(tempReturn)
		{
			FSRef			fileFSRef;
			HFSUniStr255	dataFork;

			FSMakeFSRefUnicode( &prefsFileFSRef, prefsFileName.length, prefsFileName.unicode, 0, &fileFSRef );
			FSGetDataForkName( &dataFork );

			short	fRefNum;
			OSErr	tempFErr = FSOpenFork(&fileFSRef, dataFork.length, dataFork.unicode, fsCurPerm, &fRefNum);
			if(tempFErr == noErr)
			{
				long				maxChars = kPGrp_FilterMaxBufferSize-1;
				ByteCount			maxPosLen = (long)(maxChars * sizeof(fmx::unichar));
				ByteCount			gotLen;
				
				tempFErr = FSReadFork(fRefNum, fsAtMark, 0, maxPosLen, mDefaultFormatString, &gotLen);
				if(  ((tempFErr == noErr) || (tempFErr == -39)) && (gotLen <= maxPosLen)  )
				{
					FSCloseFork(fRefNum);
					
					// This makes sure the read-in data is null terminated.
					bool		isNullTermAsIs = false;
					for(long x=0; x<maxChars && !isNullTermAsIs ; x++)
					{
						isNullTermAsIs = (mDefaultFormatString[x] == 0);
						if(isNullTermAsIs)
						{
							mDefaultFormatResult = x+1;
							
						}// isNullTermAsIs
						
					}// for x<maxChars
					
					if(!isNullTermAsIs)
					{
						mDefaultFormatString[maxChars] = 0;
						mDefaultFormatResult = maxChars;
						
					}// !isNullTermAsIs
				
				
					gFMPluginTahomaPrefs.formattingStringTxt->AssignUnicode(mDefaultFormatString);
					tempReturn = true;
					
				}// tempFErr FSRead
			
			}// tempFErr FSpOpenDF
						
		}// tempReturn OSX_GetPrefsFileLocationAndName

	#endif // FMX_MAC_TARGET



return(tempReturn);
} // FMPluginTahomaPrefsWidget::Read



bool FMPluginTahomaPrefsWidget::Write(void) 
{
	bool	tempReturn = false;


	#if defined(FMX_WIN_TARGET)
		long		regResult = ERROR_BADKEY;
		HKEY		tempKey;


		regResult = RegOpenKeyExW(HKEY_CURRENT_USER, (LPCWSTR)sWinRegPathChars, 0, KEY_SET_VALUE | KEY_WRITE, &tempKey);
		if(regResult == ERROR_SUCCESS)
		{

			// This creates the formatting string preference, which is the only preference the example plug-in has.

				GetDefaults(); // This gets the default string, if none has been specified already.

				regResult = RegSetValueExW(tempKey,
								(LPCWSTR)sWinRegSubKeyChars,
								0,
								REG_SZ,
								(BYTE*)mDefaultFormatString,
								mDefaultFormatResult
								*
								sizeof(fmx::uint16));

				tempReturn = (regResult == ERROR_SUCCESS);

			long debugErr = RegCloseKey(tempKey);

		}// regResult is ERROR_SUCCESS

	#endif // FMX_WIN_TARGET



	#if defined(FMX_MAC_TARGET)
		// This creates the formatting string preference, which is the only preference the example plug-in has.
		FSRef			prefsFileFSRef;
		HFSUniStr255	prefsFileName;
		
		tempReturn = OSX_GetPrefsFileLocationAndName(&prefsFileFSRef, &prefsFileName);
		if(tempReturn)
		{
			GetDefaults(); // This gets the default string, if none has been specified already.
			
			FSRef			fileFSRef;
			HFSUniStr255	dataFork;

			FSMakeFSRefUnicode( &prefsFileFSRef, prefsFileName.length, prefsFileName.unicode, 0, &fileFSRef );
			FSGetDataForkName( &dataFork );
			
			short	fRefNum;
			OSErr	tempFErr = FSOpenFork(&fileFSRef, dataFork.length, dataFork.unicode, fsCurPerm, &fRefNum);
			if(tempFErr == noErr)
			{
				ByteCount	maxCharBytes = kPGrp_FilterMaxBufferSize * sizeof(fmx::unichar);
				ByteCount	actualBytes;

				tempFErr = FSWriteFork( fRefNum, fsAtMark, 0, maxCharBytes, mDefaultFormatString, &actualBytes ); 
			
				tempReturn = (tempFErr == noErr);
				
				FSCloseFork(fRefNum);
				
			}// tempFErr FSpOpenDF
			
		}// tempReturn OSX_GetPrefsFileLocationAndName
					
	#endif // FMX_MAC_TARGET



return(tempReturn);
} // FMPluginTahomaPrefsWidget::Write



void FMPluginTahomaPrefsWidget::GetDefaults(void)
{
	unsigned long	curFormatStrSize = gFMPluginTahomaPrefs.formattingStringTxt->GetSize();
	if(curFormatStrSize == 0)
	{
		mDefaultFormatString[0] = 0;
		mDefaultFormatResult = 0;

		#if defined(FMX_WIN_TARGET)
			mDefaultFormatResult = LoadStringW((HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)kFMEX_RES_DIALOGPREFS_TXT, (LPWSTR)mDefaultFormatString, kPGrp_FilterMaxBufferSize-1 );
		#endif

		#if defined(FMX_MAC_TARGET)
			mDefaultFormatResult = (int)(Sub_OSXLoadString(kFMEX_RES_DIALOGPREFS_TXT, mDefaultFormatString, kPGrp_FilterMaxBufferSize-1));
		#endif


		if(mDefaultFormatResult != 0)
		{
			gFMPluginTahomaPrefs.formattingStringTxt->AssignUnicode(mDefaultFormatString);

		}// defaultFormatResult

	}
	else
	{
			gFMPluginTahomaPrefs.formattingStringTxt->GetUnicode(mDefaultFormatString, 0, curFormatStrSize);
			mDefaultFormatString[curFormatStrSize] = 0;
			mDefaultFormatResult = (int)(curFormatStrSize);

	}// GetSize == 0


	// This gets Locale settings needed by Do_PGrp_NumToWords.
	GetLocaleSettings();

} // FMPluginTahomaPrefsWidget::GetDefaults



void FMPluginTahomaPrefsWidget::GetLocaleSettings(void)
{
	#if defined(FMX_WIN_TARGET)
		struct lconv* localeInfoPtr;
		localeInfoPtr = localeconv();

		gFMPluginTahomaPrefs.decimalPoint =  *(localeInfoPtr->decimal_point);
		gFMPluginTahomaPrefs.thousandsSep =  *(localeInfoPtr->thousands_sep);		
	#endif // FMX_WIN_TARGET

	#if defined(FMX_MAC_TARGET)
		Intl0Rec	intlResourceRec;
		intlResourceRec = (**((Intl0Hndl)GetIntlResource(0)));  
		
		gFMPluginTahomaPrefs.decimalPoint = intlResourceRec.decimalPt;
		gFMPluginTahomaPrefs.thousandsSep= intlResourceRec.thousSep;
	#endif // FMX_MAC_TARGET

} // FMPluginTahomaPrefsWidget::GetLocaleSettings



#if defined(FMX_WIN_TARGET)

void Win_SetEditWithDefaultFormatString(HWND mydlg, bool fromGlobalPrefs)
{
	fmx::uint16		defaultFormatString[kPGrp_FilterMaxBufferSize];
	int				defaultFormatResult = 0;

	if(fromGlobalPrefs)
	{
		defaultFormatResult = gFMPluginTahomaPrefs.formattingStringTxt->GetSize();
		if(defaultFormatResult >= kPGrp_FilterMaxBufferSize)
		{
			defaultFormatResult = kPGrp_FilterMaxBufferSize-1;
			
		} // defaultFormatResult  >= kPGrp_FilterMaxBufferSize
		
		if(defaultFormatResult != 0)
		{
			gFMPluginTahomaPrefs.formattingStringTxt->GetUnicode(defaultFormatString, 0, defaultFormatResult);
			defaultFormatString[defaultFormatResult] = 0;

		}// defaultFormatResult in range
	}
	else
	{
		defaultFormatResult = LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)kFMEX_RES_DIALOGPREFS_TXT, (LPWSTR)defaultFormatString, kPGrp_FilterMaxBufferSize-1 );

	}// fromGlobalPrefs
		

	if(defaultFormatResult != 0)
	{
		SetDlgItemTextW(mydlg, kFMEX_RES_DIALOGPREFS_EDIT, (LPCWSTR)defaultFormatString);
		
	}// defaultFormatResult

} // Win_SetEditWithDefaultFormatString



BOOL APIENTRY PrefsDialog_WinProc(HWND mydlg, UINT dlgMsg, WPARAM wparm, LONG lparm)
{
	switch (dlgMsg)
	{
		case WM_INITDIALOG:
		{
			SetFocus(GetDlgItem( mydlg, kFMEX_RES_DIALOGPREFS_EDIT));
			Win_SetEditWithDefaultFormatString(mydlg, true);	// Taken from global preferences.

			return FALSE;
		}
		break;// WM_INITDIALOG
		

		case WM_CLOSE:
		{
			PostMessage(mydlg, WM_COMMAND, IDCANCEL, 0L);
			return FALSE;
		}
		break;// WM_CLOSE
		

		case WM_COMMAND:
		{
			switch (wparm)
			{
				case IDCANCEL:
				{
					EndDialog(mydlg, NULL);  // don't save changes
				}
				break;// IDCANCEL
		

				case IDOK:
				{
					// This gets the format string from the edit text item.
					fmx::uint16*	textEditString =
					new(std::nothrow) fmx::uint16[kPGrp_FilterMaxBufferSize];
					if(textEditString != NULL)
					{
						int				textEditResult = 0;
						bool			hasFormatSymbols = false;
						textEditString[0] = 0;


						textEditResult = GetDlgItemTextW( mydlg, kFMEX_RES_DIALOGPREFS_EDIT, (LPWSTR)textEditString, (kPGrp_FilterMaxBufferSize-1));
						for (int x=0; x<textEditResult; x++)
						{
							if (textEditString[x] == 0x0023) // 0x0023 = '#'
							{
								hasFormatSymbols = true;
							}
						}
						
						// This function tests item text to make sure it's a valid format.  It must not be empty. 
						// It must not be bigger than the maximum size of the formatter string, and it must have format symbols.
						if ( (textEditResult > 0) && (textEditResult < kPGrp_FilterMaxBufferSize) && hasFormatSymbols)
						{ 						
							EndDialog(mydlg, (INT_PTR)textEditString); // Returns a ptr to the allocated wchar string.
						}
						else
						{ // This resets the contents of item 3 to the format string
							//  and ignores the pressing of the OK button.
							MessageBeep(0);
							Win_SetEditWithDefaultFormatString(mydlg, true);
							SetFocus (GetDlgItem( mydlg, kFMEX_RES_DIALOGPREFS_EDIT));
							delete [] textEditString;
						} // valid prefs string
					}
					else
					{
						MessageBeep(0);
						MessageBeep(0);

						EndDialog ( mydlg, NULL );

					}// textEditString != NULL
				}
				break;// IDOK
				
				default:
					return FALSE;

			} // switch wparm
		}
		break;// WM_COMMAND
		
		default:
			return FALSE;

	} // switch dlgmsg
		

return FALSE;
} // PrefsDialog_WinProc

#endif // FMX_WIN_TARGET



#if defined(FMX_MAC_TARGET)

void OSX_SetEditWithDefaultFormatString(WindowRef dialogRef, bool fromGlobalPrefs)
{
	fmx::uint16		defaultFormatString[kPGrp_FilterMaxBufferSize];
	int				defaultFormatResult = 0;


	if(dialogRef != NULL)
	{
		ControlRef			tempControlRef = NULL;
		const ControlID		controlID = { 'PGrp', kFMEX_RES_DIALOGPREFS_EDIT };
		
		if(GetControlByID(dialogRef, &controlID, &tempControlRef) == noErr)
		{
			if(fromGlobalPrefs)
			{
				defaultFormatResult = (int)(gFMPluginTahomaPrefs.formattingStringTxt->GetSize());
				if(defaultFormatResult >= kPGrp_FilterMaxBufferSize)
				{
					defaultFormatResult = kPGrp_FilterMaxBufferSize-1;
				}
				if(defaultFormatResult != 0)
				{
					gFMPluginTahomaPrefs.formattingStringTxt->GetUnicode(defaultFormatString, 0, (unsigned long)defaultFormatResult);
					defaultFormatString[defaultFormatResult] = 0;

				}// defaultFormatResult in range
			}
			else
			{
				defaultFormatResult = (int)(Sub_OSXLoadString(kFMEX_RES_DIALOGPREFS_TXT, defaultFormatString, kPGrp_FilterMaxBufferSize-1));
				
			}// fromGlobalPrefs



			if(defaultFormatResult != 0)
			{
				CFStringRef		tempCFString = CFStringCreateWithCharacters(kCFAllocatorDefault, (UniChar*)defaultFormatString, defaultFormatResult);
				if(tempCFString != NULL)
				{
					SetControlData(tempControlRef, kControlFocusNoPart, kControlEditTextCFStringTag, sizeof(CFStringRef), &tempCFString);
					CFRelease(tempCFString);
						
				}// tempCFString
				
			}// defaultFormatResult
			
		}// GetControlByID
			
	}// dialogRef

} // OSX_SetEditWithDefaultFormatString



int OSX_GetEditFormatString(WindowRef dialogRef, fmx::uint16* intoHere, long maxAlloced)
{
	long		tempResult = 0;
	
	
	if((dialogRef != NULL) && (intoHere != NULL) && (maxAlloced > 1))
	{
		ControlRef			tempControlRef = NULL;
		const ControlID		controlID = { 'PGrp', kFMEX_RES_DIALOGPREFS_EDIT };
		
		if(GetControlByID(dialogRef, &controlID, &tempControlRef) == noErr)
		{
			CFStringRef		tempOSXStr;

			if(GetControlData(tempControlRef, kControlFocusNoPart, kControlEditTextCFStringTag, sizeof(CFStringRef), &tempOSXStr, &tempResult) == noErr)
			{
				tempResult = CFStringGetLength(tempOSXStr);
				if(tempResult < maxAlloced)
				{
					CFRange		allChars;
					allChars.location = 0;
					allChars.length = tempResult;
						
					CFStringGetCharacters(tempOSXStr, allChars, (UniChar*)(intoHere));
					intoHere[tempResult] = 0x0000;
					
				}// maxAlloced
										
			}// GetControlDataSize
			
		}// GetControlByID
			
	}// dialogRef


return(tempResult);
} // OSX_SetEditWithDefaultFormatString



pascal OSStatus PrefsDialog_MacProc(EventHandlerCallRef /* myHandler */, EventRef eventRef, void* userData)
{
    OSStatus 		results = eventNotHandledErr;
    bool			stopModalLoop = false;
    HICommand		command;
   
    GetEventParameter( eventRef, kEventParamDirectObject, typeHICommand, NULL, sizeof (HICommand), NULL, &command );
    switch (command.commandID)
    {
        case 'ok  ': // User done
        {
			// This gets the format string from the edit text item.
			fmx::uint16*	textEditString = new(std::nothrow)
			fmx::uint16[kPGrp_FilterMaxBufferSize];
			if(textEditString != NULL)
			{
				int				textEditResult = 0;
				bool			hasFormatSymbols = false;
				textEditString[0] = 0;


				textEditResult = OSX_GetEditFormatString(static_cast<WindowRef>(userData), textEditString, (kPGrp_FilterMaxBufferSize-1));
				for (int x=0; x<textEditResult; x++)
				{
					if (textEditString[x] == 0x0023) // 0x0023 = '#'
					{
						hasFormatSymbols = true;
					}
					
				}// for x<textEditResult
				
				// This function tests item text to make sure it's a valid format for our function.  It must not be empty. 
				//    It must not be bigger than the max size of the format string, and it must have format symbols.
				if ( (textEditResult > 0) && (textEditResult < kPGrp_FilterMaxBufferSize) && hasFormatSymbols)
				{ 						
					SetWRefCon(static_cast<WindowRef>(userData), (long)(textEditString));
	                stopModalLoop = true;
	                results = noErr;
				}
				else
				{ // This resets the contents of item 3 to the format string
					//  and ignores the pressing of the OK button.
					SysBeep(0);
					OSX_SetEditWithDefaultFormatString(static_cast<WindowRef>(userData), true);
					delete [] textEditString;
				} // valid prefs string

				
			}
			else
			{
				SysBeep(0);
				SysBeep(0);

				SetWRefCon(static_cast<WindowRef>(userData), 0);
                stopModalLoop = true;
                results = cTempMemErr;

			}// textEditString != NULL
        }
        break;


        case 'not!': // User cancel
        {
			SetWRefCon(static_cast<WindowRef>(userData), 0);
	        stopModalLoop = true;
	        results = noErr;
     	}
        break;

    }// switch commandID
    
    
    // Stop the modal loop.
    if (stopModalLoop)
    {
        QuitAppModalLoopForWindow( static_cast<WindowRef>(userData) );
    }

return (results);
} // PrefsDialog_MacProc



bool FMPluginTahomaPrefsWidget::OSX_GetPrefsFileLocationAndName(FSRef* prefsFileFSRef, HFSUniStr255* prefsFileName)
{
	bool	tempReturn = false;
	OSErr	macOSerr;
	
	short		vRefNum = kUserDomain;
	OSType		folderType = kPreferencesFolderType; // This value makes FindFolder look for the proper folder for preferences.
	Boolean		createFolder = kCreateFolder;  // This value makes FindFolder create the folder if it doesn't already exist.
	
	macOSerr = FSFindFolder( vRefNum, folderType, createFolder, prefsFileFSRef );
	tempReturn = (macOSerr == noErr);

	if (tempReturn)
	{
		static const char* kName = "FMPluginTahoma Prefs";
		prefsFileName->length = std::strlen( kName );
		for (UniCharCount i = 0; i < prefsFileName->length; ++i)
		{
			prefsFileName->unicode[i] = (UniChar) kName[i];
		}
	}
	else
	{
		prefsFileName->length = 0;
	}
	
return(tempReturn);
} // FMPluginTahomaPrefsWidget::OSX_GetPrefsFileLocationAndName



#endif // FMX_MAC_TARGET



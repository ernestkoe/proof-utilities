
/*
  Copyright 1998-2006 FileMaker, Inc.  All Rights Reserved.
  
*/


#if !defined(FMPLUGINPREFS_H)
	#define FMPLUGINPREFS_H


	#if defined(FMX_WIN_TARGET)
		#include "Windows.h"
	#endif

	#if defined(FMX_MAC_TARGET)
		#include <CoreServices/CoreServices.h>
		#include <Carbon/Carbon.h>
	#endif 


	#include "FMWrapper/FMXExtern.h"
	#include "FMWrapper/FMXTypes.h"
	#include "FMWrapper/FMXText.h"

	#include "FMPluginGlobalDefines.h"



	enum { kPGrp_FilterMaxBufferSize = 260 };

	typedef struct PGrp_Preferences
	{
			fmx::TextAutoPtr	formattingStringTxt;
			unsigned short		decimalPoint;
			unsigned short		thousandsSep;
			
	}PGrp_Preferences;
	extern PGrp_Preferences	gFMPluginTahomaPrefs;



	void Do_PluginPrefs(void);



	// This is used by Do_PluginPrefs to create and display the Preferences dialog box.
		class FMPluginTahomaPrefsDialog
		{
			public:
				FMPluginTahomaPrefsDialog();
				~FMPluginTahomaPrefsDialog();

				bool DisplayAndGetPrefs(void);

			private:

				unsigned long*		mDialogRef;
		#if defined(FMX_WIN_TARGET)
				unsigned long*		mParentDialogRef;
		#endif

		};// FMPluginTahomaPrefsDialog

		// This is used by FMPluginTahomaPrefsDialog.
		#if defined(FMX_WIN_TARGET)
			BOOL APIENTRY PrefsDialog_WinProc(HWND mydlg, UINT dlgMsg, WPARAM wparm, LONG lparm);
		#endif
		
		
		#if defined(FMX_MAC_TARGET)
		
			pascal OSStatus PrefsDialog_MacProc(EventHandlerCallRef myHandler, EventRef eventRef, void* userData);
			void OSX_SetEditWithDefaultFormatString(WindowRef dialogRef, bool fromGlobalPrefs);
			int OSX_GetEditFormatString(WindowRef dialogRef, fmx::ushort* intoHere, long maxAlloced);
			
		#endif // FMX_MAC_TARGET



	// This is used by Do_PluginInit and FMPluginTahomaPrefsDialog to 
	//		create and read the Formatting String for Do_PGrp_UserFormatNumber.
		class FMPluginTahomaPrefsWidget
		{
			public:
				FMPluginTahomaPrefsWidget();
				~FMPluginTahomaPrefsWidget();

				bool Exists(void); 
				bool Create(void); 

				bool Read(void); 
				bool Write(void); 

			private:
				fmx::uint16		mDefaultFormatString[kPGrp_FilterMaxBufferSize];
				int				mDefaultFormatResult;
			
				void GetDefaults(void);
				void GetLocaleSettings(void);
				
				#if defined(FMX_MAC_TARGET)
					bool OSX_GetPrefsFileLocationAndName(FSRef* prefsFileFSRef, HFSUniStr255* prefsFileName);
				#endif

		};// FMPluginTahomaPrefsWidget



#endif // FMPLUGINPREFS_H

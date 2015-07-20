
/*
  Copyright 1998-2005 FileMaker, Inc.  All Rights Reserved.
  
*/



#if !defined(FMPLUGINFUNCTIONS_H)
	#define FMPLUGINFUNCTIONS_H


#if defined(FMX_WIN_TARGET)
	#include "Windows.h"
#endif


#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"


#include "FMPluginGlobalDefines.h"

#pragma once

#include <string>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#include <ctype.h>
#include <assert.h>



// General String IDs

	// The following string cannot be localized.
	enum { kPGrp_OptionsStringID = 1 };



// Function IDs
	enum
	{ 
		kPGrp_Add, 
		kPGrp_LDistance,
		kPGrp_SoundexAmerican,
		kPGrp_SoundexDM,
		kPGrp_ExecuteSQL,
//		kPGrp_SortSet,
//		kPGrp_SetsOfSet,
		kPGrp_Redux,
		kPGrp_Append, 
		kPGrp_Evaluate, 
		kPGrp_StartScript, 
		kPGrp_UserFormatNumber, 
		kPGrp_FormatNumber, 
		kPGrp_NumToWords,
		kPGrp_Version
	};



	// The following functions are registered with FileMaker as the external functions made available via the plug-in.

		FMX_PROC(fmx::errcode) Do_PGrp_Add(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_LDistance(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_SoundexAmerican(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_SoundexDM(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_ExecuteSQL(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

	//	FMX_PROC(fmx::errcode) Do_PGrp_SortSet(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

	//	FMX_PROC(fmx::errcode) Do_PGrp_SetsOfSet(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_Redux(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_Append(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_Evaluate(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_StartScript(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_CommonFormatNumber(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_NumToWords(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);

		FMX_PROC(fmx::errcode) Do_PGrp_Version(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);


	// Used by Do_PGrp_CommonFormatNumber, based on funcId.
		FMX_PROC(fmx::errcode) Do_PGrp_UserFormatNumber(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);
		FMX_PROC(fmx::errcode) Do_PGrp_FormatNumber(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results);


	// Used by Do_PluginInit, FMExternCallProc, and the example functions listed above.
		void Do_GetString(unsigned long whichString, FMX_ULong winLangID, FMX_Long resultsize, FMX_Unichar* string);
		void Do_GetString(unsigned long whichString,  fmx::TextAutoPtr& intoHere, bool stripFunctionParams = false);


	// Used by Do_PGrp_UserFormatNumber and Do_PGrp_FormatNumber.
		fmx::errcode FormatNumberWithMask(const fmx::TextAutoPtr& formatThis, const fmx::TextAutoPtr& withThis, fmx::TextAutoPtr& intoThis);

	// Used by FormatNumberWithMask and Do_PGrp_NumToWords.
		void Sub_GetFilteredChars(const fmx::TextAutoPtr& filterThis,
		fmx::ushort* filteredIntoHere, fmx::ulong& filteredIntoHereMaxSize, bool saveDecimal=false);

	// Used by Do_PGrp_LDistance.
		void FMTextToString(const fmx::Text &, std::string &);
		int LevenshteinDistance(const std::string &, const std::string &);

	// Used by Do_PGrp_SoundexAmerican and Do_PGrp_SoundexDM.
		std::string SoundexAmerican(const std::string &);
		std::string SoundexDM(const std::string &);
		
	// Used by Do_PGrp_SortSet, Do_PGrp_SetsOfSet, and Do_PGrp_Redux
	//	void SortSet(const vector<string> &, bool bAscending, vector<string> &);
	//	void SetsOfSet(const vector<string> &, vector<string> &, const string &);
		void Redux(const string &, string &);

	// Used by Do_PGrp_NumToWords.
		bool Sub_GetWordFromNum(bool firstPos, int mag, 
						unsigned short firstch, unsigned short secondch, 
						fmx::TextAutoPtr& returnText, 
						bool& skipch, 
						bool& displayMillions, bool& displayThousands, bool& pluralcents);

	// Used by Do_GetString.
		#if defined(FMX_MAC_TARGET)
			unsigned long  Sub_OSXLoadString(unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax);
		#endif

#endif // FMPLUGINFUNCTIONS_H

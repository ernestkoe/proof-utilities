/*
 *  Soundex.cpp
 *  FMTemplate.XC23
 *
 *  Created by Corn Walker on 11/6/07.
 *  Copyright 2007 Proof Group LLC. All rights reserved.
 *
 */

#if defined(FMX_WIN_TARGET)
//	#include <windows.h>
	#include <locale.h>
#endif

#if defined(FMX_MAC_TARGET)
	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>
#endif 

#include "FMPlugin.h"
#include "FMTemplate/FMTemplate.h"
#include "Soundex.h"
#include "StringUtils.h"

#include <string>
using namespace std;

#define Min(x,y)	(((x) < (y)) ? (x) : (y))

//private defs
void SetRecurse(stack<string> uc, stack<string> c, vector<string> &result, const string &delim);

FMX_PROC(fmx::errcode) PGrp_SoundexAmerican(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	//PGrp_SoundexAmerican(string1)
	// string1
	//Return:
	// American soundex encoding of string1
	
	// UNUSED
	//funcId;
	//environment;

	//grab inputs
	string sIn1;
	FMTextToString(dataVect.AtAsText(0), sIn1);

	string sSoundexCode = SoundexAmerican(sIn1);


	fmx::TextAutoPtr	responseData;
	responseData->AssignWithLength( sSoundexCode.c_str(), sSoundexCode.size() );
	results.SetAsText(*responseData, dataVect.At(0).GetLocale());

	return errorResult;
} // PGrp_SoundexAmerican


FMX_PROC(fmx::errcode) PGrp_SoundexDM(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	//PGrp_SoundexDM(string1)
	// string1
	//Return:
	// DM soundex encoding of string1
	
	// UNUSED
	//funcId;
	//environment;

	//grab inputs
	string sIn1;
	FMTextToString(dataVect.AtAsText(0), sIn1);

	string sSoundexCode = SoundexDM(sIn1);

	fmx::TextAutoPtr	responseData;
	responseData->AssignWithLength( sSoundexCode.c_str(), sSoundexCode.size() );
	results.SetAsText(*responseData, dataVect.At(0).GetLocale());

	return errorResult;
} //PGrp_SoundexDM

//The American Soundex Table
static const char *gk_rgchSoundexTable[] = { 
	"AEIOUHWY",			//0
	"BFPV",				//1
	"CGJKQSXZ",			//2
	"DT",				//3
	"L",				//4
	"MN",				//5
	"R"					//6
};

string SoundexAmerican(const string &in)
{
	//Returns a string encoded per the American Soundex algo

	string enc;

	unsigned int i = 0;

	//scan until we hit first alphabet character
	while( i < in.size() && !isalpha(in[i]) ) { i++; }

	//copy the first letter of the input to the output
	enc += toupper( in[i++] );

	//remaining 3 characters are numerical encoding of string
	int lastFound = -1;
	
	while(enc.size() < 4 && i < in.size())
	{
		char c = toupper( in[i++] );
		
		//skip non-alphabet characters
		if(!isalpha(c))
		{
			lastFound = -1;
			continue;
		}

		//scan thru soundex table rows 1-6
		for(int k=1; k <= 6; k++)
		{
			if(strchr(gk_rgchSoundexTable[k], c))
			{
				//we have a match: add digit if not duplicate
				if(k != lastFound)
				{
					enc += '0' + k;
					lastFound = k; //avoide duplicates
				}
				
				break;
			}
		}
	}

	//pad output with zeroes as necessary
	for(size_t j=enc.size(); j < 4; j++)
	{
		enc += '0';
	}

	return enc;
} //SoundexAmerican

/// [ Daitch-Mokotoff Soundex System ] //////////////////////////////////////////////

//DM splitting table
static const char *gk_rgszDMSplitTable[] =
{
	//branch pt		brch 1		brch2
	"CH",			"5",		"4",
	"CK",			"5",		"45",
	"RZ",			"94",		"4",
	"RS",			"94",		"4",
	"C",			"5",		"4",
	"J",			"Y",		"4",  //special case
};

#define NUM_DM_SPLITS	6

//DM substitution table
static const char *gk_rgszDMSubTable[] = 
{ 
	//string	start		before v	aoc
	"SCHTSCH",	"2",		"4",		"4",
	
	"SCHTSH",	"2",		"4",		"4",
	"SCHTCH",	"2",		"4",		"4",
	
	"SHTCH",	"2",		"4",		"4",
	"SHTSH",	"2",		"4",		"4",
	"STSCH",	"2",		"4",		"4",
	"TTSCH",	"4",		"4",		"4",
	"ZHDZH",	"2",		"4",		"4",
	
	"SHCH",		"2",		"4",		"4",
	"SCHT",		"2",		"43",		"43",
	"SCHD",		"2",		"43",		"43",
	"STCH",		"2",		"4",		"4",
	"STRZ",		"2",		"4",		"4",
	"STRS",		"2",		"4",		"4",
	"STSH",		"2",		"4",		"4",
	"SZCZ",		"2",		"4",		"4",
	"SZCS",		"2",		"4",		"4",
	"TTCH",		"4",		"4",		"4",
	"TSCH",		"4",		"4",		"4",
	"TTSZ",		"4",		"4",		"4",
	"ZDZH",		"2",		"4",		"4",
	"ZSCH",		"4",		"4",		"4",
	
	"CHS",		"5",		"54",		"54",
	"CSZ",		"4",		"4",		"4",
	"CZS",		"4",		"4",		"4",
	"DRZ",		"4",		"4",		"4",
	"DRS",		"4",		"4",		"4",
	"DSH",		"4",		"4",		"4",
	"DSZ",		"4",		"4",		"4",
	"DZH",		"4",		"4",		"4",
	"DZS",		"4",		"4",		"4",
	"SCH",		"4",		"4",		"4",
	"SHT",		"2",		"43",		"43",
	"SZT",		"2",		"43",		"43",
	"SHD",		"2",		"43",		"43",
	"SZD",		"2",		"43",		"43",
	"TCH",		"4",		"4",		"4",
	"TRZ",		"4",		"4",		"4",
	"TRS",		"4",		"4",		"4",
	"TSH",		"4",		"4",		"4",
	"TTS",		"4",		"4",		"4",
	"TTZ",		"4",		"4",		"4",
	"TZS",		"4",		"4",		"4",
	"TSZ",		"4",		"4",		"4",
	"ZDZ",		"2",		"4",		"4",
	"ZHD",		"2",		"43",		"43",
	"ZSH",		"4",		"4",		"4",
	
	"AI",		"0",		"1",		"",
	"AJ",		"0",		"1",		"",
	"AY",		"0",		"1",		"",
	"AU",		"0",		"7",		"",
	"CH",		"-0",		"-",		"-",	//substitute! (note number is index above)
	"CK",		"-1",		"-",		"-",	//substitute!
	"CZ",		"4",		"4",		"4",
	"CS",		"4",		"4",		"4",
	"DS",		"4",		"4",		"4",
	"DZ",		"4",		"4",		"4",
	"DT",		"3",		"3",		"3",
	"EI",		"0",		"1",		"",
	"EJ",		"0",		"1",		"",
	"EY",		"0",		"1",		"",
	"EU",		"1",		"1",		"",
	"FB",		"7",		"7",		"7",
	"IA",		"1",		"",			"",
	"IE",		"1",		"",			"",
	"IO",		"1",		"",			"",
	"IU",		"1",		"",			"",
	"KS",		"5",		"54",		"54",
	"KH",		"5",		"5",		"5",
	"MN",		"66",		"66",		"66",
	"NM",		"66",		"66",		"66",
	"OI",		"0",		"1",		"",
	"OJ",		"0",		"1",		"",
	"OY",		"0",		"1",		"",
	"PF",		"7",		"7",		"7",
	"PH",		"7",		"7",		"7",
	"RS",		"-3",		"-",		"-",	//substitute
	"RZ",		"-2",		"-",		"-",	//substitute
	"SH",		"4",		"4",		"4",
	"SC",		"2",		"4",		"4",
	"ST",		"2",		"43",		"43",
	"SD",		"2",		"43",		"43",
	"SZ",		"4",		"4",		"4",
	"TH",		"3",		"3",		"3",
	"TC",		"4",		"4",		"4",
	"TZ",		"4",		"4",		"4",
	"TS",		"4",		"4",		"4",
	"UI",		"0",		"1",		"",
	"UJ",		"0",		"1",		"",
	"UY",		"0",		"1",		"",
	"UE",		"0",		"",			"",
	"ZD",		"2",		"43",		"43",
	"ZH",		"4",		"4",		"4",
	"ZS",		"4",		"4",		"4",
	
	"A",		"0",		"",			"",
	"B",		"7",		"7",		"7",
	"C",		"-4",		"-",		"-",	//substitute!
	"D",		"3",		"3",		"3",
	"E",		"0",		"",			"",
	"F",		"7",		"7",		"7",
	"G",		"5",		"5",		"5",
	"H",		"5",		"5",		"",
	"I",		"0",		"",			"",
	"J",		"-5",		"-",		"-",	//substitute!
	"K",		"5",		"5",		"5",
	"L",		"8",		"8",		"8",
	"M",		"6",		"6",		"6",
	"N",		"6",		"6",		"6",
	"O",		"0",		"",			"",
	"P",		"7",		"7",		"7",
	"Q",		"5",		"5",		"5",
	"R",		"9",		"9",		"9",
	"S",		"4",		"4",		"4",
	"T",		"3",		"3",		"3",
	"U",		"0",		"",			"",
	"V",		"7",		"7",		"7",
	"W",		"7",		"7",		"7",
	"X",		"5",		"54",		"54",
	"Y",		"1",		"",			"",
	"Z",		"4",		"4",		"4",
};

#define	NUM_DM_SUBSTITUTIONS	120


static const char gk_rgchVowels[] = "AEIOUY";

#define SOUNDEX_DELIM	"^"
#define SOUNDEX_OUTPUT_DELIM "\r"


/////////////////////////////////////////////////

/*
	POSSIBLE BUGS:
	
	- sequential 'mn' strings...reference website http://stevemorse.org/census/soundex.html
	  'mnmn' -> '6666'
	  but my stuff does 'mnmn' -> 666 ... not a big deal I think
*/

string SoundexDM(const string &in)
{
	//Returns a string encoded per The Daitch-Mokotoff algorithm
	
	string enc;
	
	//Step 1: make initial string uppercase and eliminate non-[a-zA-Z] characters
	string s;
	for(size_t j=0; j < in.size(); j++)
	{
		if( isalpha( in[j] ) )
		{
			s += toupper( in[j] );
		}
	}
	
	
	//Step 3: start doing substitutions
	for(int i=0; i < NUM_DM_SUBSTITUTIONS; i++)
	{
		while(1)
		{
			size_t loc = s.find(gk_rgszDMSubTable[i * 4]);
			if(string::npos != loc)
			{
				size_t origLen = strlen( gk_rgszDMSubTable[i * 4] );
				
				//CASE 0: substitution requred!
				if('-' == *(gk_rgszDMSubTable[i * 4 + 1]))
				{
					//duplicate the substring and perform the substitutions
					s += SOUNDEX_DELIM;

					size_t ssStart	= s.rfind(SOUNDEX_DELIM, loc);
					size_t ssEnd	= s.find(SOUNDEX_DELIM, loc);  //note we should ALWAYS find this

					size_t loc2;

					assert(string::npos != ssEnd);

					if(string::npos != ssStart)
					{
						//append copy of this Soundex code after original string
						ssStart += strlen(SOUNDEX_DELIM);
						loc2 =  loc - ssStart + s.size();
						s.append(s, ssStart, ssEnd - ssStart);
					}
					else
					{
						//this is the first soundex code
						loc2 = loc + s.size();
						s.append(s, 0, ssEnd); //duplicate string except for trailing DELIM
					}

					//replace substrings
					int splitTableIdx = *(gk_rgszDMSubTable[i * 4 + 1] + 1) - '0';
					s.replace(loc2, origLen, gk_rgszDMSplitTable[splitTableIdx * 3 + 2]);
					s.replace(loc, origLen, gk_rgszDMSplitTable[splitTableIdx * 3 + 1]);
					
				}
				//CASE 1: is this beginning of word?
				else if(0 == loc)
				{
					s.replace(loc, origLen, gk_rgszDMSubTable[i * 4 + 1] );
				}
				//CASE 2: vowel follows string
				else if ( (loc+origLen) < s.size() && strchr(gk_rgchVowels, s[loc+origLen]) )
				{
					//if preceeding digit same as first replacement digit, skip
					int offset = ( s[loc - 1] == *(gk_rgszDMSubTable[i * 4 + 2]) ) ? 1 : 0;
					//do replacement
					s.replace(loc, origLen, gk_rgszDMSubTable[i * 4 + 2] + offset );
				}
				//ALL OTHER CASES
				else
				{
					//if preceeding digit same as first replacement digit, skip
					int offset = ( s[loc - 1] == *(gk_rgszDMSubTable[i * 4 + 3]) ) ? 1 : 0;
					//do replacement
					s.replace(loc, origLen, gk_rgszDMSubTable[i * 4 + 3] + offset );
				}
			}
			else
			{
				break;	//next search string
			}
		}
	}
	
	//Step 4: format output string
	string temp;
	size_t spos = 0; 
	size_t epos = s.find(SOUNDEX_DELIM);
	bool done = false;
	while(!done)
	{
		if(string::npos != epos)
		{
			temp.assign(s, spos, (epos - spos));
			spos = epos + strlen(SOUNDEX_DELIM);
			epos = s.find(SOUNDEX_DELIM, spos);
		}
		else
		{
			//end of chain
			temp.assign(s, spos, s.size());
			done = true;
		}

		//pad with zeroes
		for(size_t j=temp.size(); j < 6; j++)
		{
			temp += '0';
		}

		if(!enc.empty())
		{
			enc += SOUNDEX_OUTPUT_DELIM;
		}
		enc.append(temp, 0, 6);
	}

	return enc;
} // SoundexDM


/*
  Copyright 1998-2006 FileMaker, Inc.  All Rights Reserved.
  
*/



#include "FMPluginGlobalDefines.h"

#if defined(FMX_WIN_TARGET)
	#include "Windows.h"
	#include <locale.h>
#endif

#if defined(FMX_MAC_TARGET)
	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>
#endif 

#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXCalcEngine.h"

#include "FMPluginFunctions.h"
#include "FMPluginPrefs.h"

#include "resource.h"

#include <string>
using namespace std;

#define Min(x,y)	(((x) < (y)) ? (x) : (y))

//private defs
void SetRecurse(stack<string> uc, stack<string> c, vector<string> &result, const string &delim);

//Utility function to convert FM text to string type
void FMTextToString(const fmx::Text &text, string &s)
{
	//clear string
	s.clear();

	if(text.GetSize())
	{
		unsigned int	iBuffLen = text.GetSize();
		FMX_Unichar		*rgwchInText = new FMX_Unichar[iBuffLen];
		if(rgwchInText)
		{
			text.GetUnicode(rgwchInText, 0, iBuffLen);

			//convert from unicode to standard chars
			//TODO: use UNICODE!
			for(unsigned int i=0; i<iBuffLen; i++)
			{
				s += (char)(rgwchInText[i] & 0x00ff);
			}

			delete [] rgwchInText;
		}
	}
}// FMTextToString

//Helper function: splits a string into vector
void SetStringToVector(const string &inputSet, const string &elementDelim, vector<string> &ret)
{
	//clear output
	ret.clear();
	
	string t;
	size_t pos		= inputSet.find(elementDelim);
	size_t lastPos	= 0;
	while(string::npos != pos)
	{
		t.assign(inputSet, lastPos, pos - lastPos);
		ret.push_back(t);
		
		lastPos = pos + elementDelim.size();
		pos = inputSet.find(elementDelim, lastPos);
	}
	pos = inputSet.size();
	t.assign(inputSet, lastPos, pos);
	ret.push_back(t);
}// SetStringToVector

/*
//helper function: vector to string using delimiter
void SetVectorToString(const vector<string> &vStr, const string &elementDelim, string &ret)
{
	//erase result string
	ret.erase();
	
	for( vector<string>::iterator it = vStr.begin(); it != vStr.end(); it++ )
	{
		if(vStr.begin() != it)
		{
			ret += elementDelim;
		}
		ret += *it;
	}
}// SetVectorToString
*/


FMX_PROC(fmx::errcode) Do_PGrp_Version(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& /* dataVect */, fmx::Data& results)
{
	fmx::errcode errorResult = 0;
	
	fmx::FixPtAutoPtr	num;
	num->AssignInt( 8 );

	results.SetAsNumber(*num);

return(errorResult);
} // Do_PGrp_Version


FMX_PROC(fmx::errcode) Do_PGrp_LDistance(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	//PGrp_LDistance(string1; string2)
	// string1
	// string2
	//Return:
	// Levenshtein distance between string1 and string2
	
	// UNUSED
	//funcId;
	//environment;

	//grab inputs
	string sIn1;
	string sIn2;
	FMTextToString(dataVect.AtAsText(0), sIn1);
	FMTextToString(dataVect.AtAsText(1), sIn2);

	int iLDistance = LevenshteinDistance(sIn1, sIn2);

	fmx::FixPtAutoPtr	num;
	num->AssignInt( iLDistance );
	results.SetAsNumber(*num);

	return(errorResult);
} // Do_PGrp_LDistance

FMX_PROC(fmx::errcode) Do_PGrp_SoundexAmerican(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
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
} // Do_PGrp_SoundexAmerican

FMX_PROC(fmx::errcode) Do_PGrp_SoundexDM(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
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
} //Do_PGrp_SoundexDM


FMX_PROC(fmx::errcode) Do_PGrp_ExecuteSQL(
                short  /* funcId */, 
  const  fmx::ExprEnv& environment, 
  const fmx::DataVect& dataVect, 
            fmx::Data& results)
{
    fmx::TextAutoPtr file_name; fmx::DataVectAutoPtr sql_parameters;

    return environment.ExecuteFileSQLTextResult(dataVect.AtAsText(0),
            *file_name, *sql_parameters, results, '\v', '\r');
}

/* FMX_PROC(fmx::errcode) Do_SetsOfSet(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	//iRset_SetsOfSet( inputSet; elementDelim; outputDelim)
	// inputSet
	// elementDelim
	// outputDelim
	//Return:
	// returns all subsets of input set
	
	// UNUSED
	//funcId;
	//environment;
	
	//grab inputs
	string inputSet;
	string elementDelim;
	string outputDelim;
	FMTextToString(dataVect.AtAsText(0), inputSet);
	FMTextToString(dataVect.AtAsText(1), elementDelim);
	FMTextToString(dataVect.AtAsText(2), outputDelim);
	
	//split set elements into vector
	vector<string> vInputSet;
	SetStringToVector(inputSet, elementDelim, vInputSet);
	
	//generate sets of set
	vector<string> vOutputSet;
	SetsOfSet(vInputSet, vOutputSet, outputDelim);

	//convert vector to string
	string returnString;
	SetVectorToString(vOutputSet, elementDelim, returnString);

	fmx::TextAutoPtr responseData;
	responseData->AssignWithLength( returnString.c_str(), returnString.size() );
	results.SetAsText(*responseData, dataVect.At(0).GetLocale());

	return errorResult;
}// Do_SetsOfSet


FMX_PROC(fmx::errcode) Do_SortSet(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	//iRset_SortSet( inputSet; A/D; elementDelim)
	//	inputSet
	//	A or D
	//  elementDelim
	//Return:
	// sorted set
		
	// UNUSED
	//funcId;
	//environment;

	//grab inputs
	string inputSet;
	string sortOrder;
	string elementDelim;
	FMTextToString(dataVect.AtAsText(0), inputSet);
	FMTextToString(dataVect.AtAsText(1), sortOrder);
	FMTextToString(dataVect.AtAsText(2), elementDelim);
	
	string returnString;

	//check parameter 2:
	if(sortOrder != "A" && sortOrder != "a" && sortOrder != "D" && sortOrder != "d")
	{
		returnString = "iRset_SortSet: Invalid Parameter 2 (sort order).  Must be 'A' or 'D'";
	}
	else
	{
		//split set elements into vector
		vector<string> vInputSet;
		SetStringToVector(inputSet, elementDelim, vInputSet);
		
		//generate sets of set
		vector<string> vOutputSet;
		SortSet(vInputSet, (sortOrder == "A") || (sortOrder == "a"), vOutputSet);

		//convert vector to string
		SetVectorToString(vOutputSet, elementDelim, returnString);
	}
	
	fmx::TextAutoPtr responseData;
	responseData->AssignWithLength( returnString.c_str(), returnString.size() );
	results.SetAsText(*responseData, dataVect.At(0).GetLocale());

	return errorResult;
}// Do_SortSet
*/

FMX_PROC(fmx::errcode) Do_PGrp_Redux(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	// Do_PGrp_Redux( string )
	// Return:
	// Redux
	
	// UNUSED
	// funcId;
	// environment;

	//grab inputs
	string str1;
	FMTextToString(dataVect.AtAsText(0), str1);

	//do redux
	string returnString;
	Redux(str1, returnString);

	fmx::TextAutoPtr responseData;
	responseData->AssignWithLength( returnString.c_str(), returnString.size() );
	results.SetAsText(*responseData, dataVect.At(0).GetLocale());

	return errorResult;
}// Do_PGrp_Redux



FMX_PROC(fmx::errcode) Do_PGrp_Add(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	// Add the first two parameters together.
		fmx::FixPtAutoPtr	num;
		num->AssignFixPt( dataVect.AtAsNumber(0) );

		num->Add( dataVect.AtAsNumber(1), *num );

		results.SetAsNumber(*num);

return(errorResult);
} // Do_PGrp_Add



FMX_PROC(fmx::errcode) Do_PGrp_Append(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;
	
	// if (funcId != kPGrp_Append) // Then somehow this function was called incorrectly. This is not likely
	//		to happen unless the plugin registers a different number of functions depending on some condition.

	// Append an arbitrary number of strings together.
		fmx::TextAutoPtr	resultTxt;
		fmx::uint32 nParams = dataVect.Size();
		fmx::uint32 j;

		if (nParams > 0)
		{
			for ( j = 0; j < nParams; j++ )
			{
				resultTxt->AppendText( dataVect.AtAsText(j),
				0, static_cast<fmx::uint32>(-1) );
			}

			results.SetAsText( *resultTxt, dataVect.At(0).GetLocale() );
		}
		else
		{
			errorResult = -1;	// This is just an example of returning an error.

		}// nParams > 0


return(errorResult);
} // Do_PGrp_Append



FMX_PROC(fmx::errcode) Do_PGrp_Evaluate(short /* funcId */, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	// environment.Evaluate() is identical to the Evaluate function built into FileMaker.
	errorResult = environment.Evaluate( dataVect.AtAsText(0), results );


return(errorResult);
} // Do_PGrp_Evaluate



FMX_PROC(fmx::errcode) Do_PGrp_StartScript(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& /* results */)
{
	fmx::errcode errorResult = 0;
	
	fmx::uint32 nParams = dataVect.Size();

	if (nParams > 1)
	{
		// This function will trigger the execution of a script in FileMaker Pro.
		errorResult = FMX_StartScript( &(dataVect.AtAsText(0)), &(dataVect.AtAsText(1)), kFMXT_Pause, &(dataVect.At(2)) );
	}
	else
	{
		errorResult = -1;	// This is just an example of returning an error

	}// nParams > 1


return(errorResult);
} // Do_PGrp_StartScript



FMX_PROC(fmx::errcode) Do_PGrp_CommonFormatNumber(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = -1;
	
	
	switch(funcId)
	{
		case kPGrp_UserFormatNumber :
			{
				errorResult = Do_PGrp_UserFormatNumber(funcId, environment, dataVect, results);
			}
			break;

		case kPGrp_FormatNumber :
			{
				errorResult = Do_PGrp_FormatNumber(funcId, environment, dataVect, results);
			}
			break;
			
	}// switch funcId
	
	
return(errorResult);
} // Do_PGrp_CommonFormatNumber



FMX_PROC(fmx::errcode) Do_PGrp_UserFormatNumber(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode		errorResult = -1;
	fmx::uint32			numParams = dataVect.Size();
	
	if(numParams == 1) // This function should have been given only one parameter.
	{
		fmx::TextAutoPtr	enteredTxt;
		enteredTxt->SetText( dataVect.AtAsText(0), 0, dataVect.AtAsText(0).GetSize() );

		fmx::TextAutoPtr	formattedResultTxt;

		errorResult = FormatNumberWithMask(enteredTxt, gFMPluginTahomaPrefs.formattingStringTxt, formattedResultTxt);
		results.SetAsText( *formattedResultTxt, dataVect.At(0).GetLocale() ); 

	}// numParams == 1


return(errorResult);
} // Do_PGrp_UserFormatNumber



FMX_PROC(fmx::errcode) Do_PGrp_FormatNumber(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode		errorResult = -1;
	fmx::uint32			numParams = dataVect.Size();
	
	if(numParams == 2) // This function needs at least two parameters. 
	{
		fmx::TextAutoPtr	formattingStringTxt;
		formattingStringTxt->SetText( dataVect.AtAsText(0), 0, dataVect.AtAsText(0).GetSize() );

		fmx::TextAutoPtr	enteredTxt;
		enteredTxt->SetText( dataVect.AtAsText(1), 0, dataVect.AtAsText(1).GetSize() );

		fmx::TextAutoPtr	formattedResultTxt;

		errorResult = FormatNumberWithMask(enteredTxt, formattingStringTxt, formattedResultTxt);
		results.SetAsText( *formattedResultTxt, dataVect.At(0).GetLocale() );

	}// numParams == 2


return(errorResult);
} // Do_PGrp_FormatNumber

fmx::errcode FormatNumberWithMask(const fmx::TextAutoPtr& formatThis, const fmx::TextAutoPtr& withThis, fmx::TextAutoPtr& intoThis)
{
	fmx::uint32		enteredTxtSize = formatThis->GetSize();
	fmx::uint32		formatterStringTxtSize = withThis->GetSize();


	// By default, this function will return "-01", which means an error occurred, and will also
	// return an error code.
	
	//	However, note that if you return an error back to the application, the result
	//	of your function will be "unknown" (i.e. "?"), no matter what the external 
	//	function passes back as data.  It is up to you to decide if you want your
	//	functions to pass back an error code and have the result be "?", or pass
	//	your error code back as data, but return no error so that the data will 
	//	be used as the result of the function.
	fmx::errcode	errorResult = -1;
	{
		fmx::uint16	tempErrChars[4];
		tempErrChars[0] = 0x002D; // '-'
		tempErrChars[1] = 0x0030; // '0'
		tempErrChars[2] = 0x0031; // '1'

		intoThis->AssignUnicodeWithLength(tempErrChars, 3);
	}


	// If the plug-in has something to parse, and it isn't too large, apply the formatter string
	//		to the text that was provided, padding any extra #s in the formatter string with zeros.
	if( (enteredTxtSize > 0) && (formatterStringTxtSize > 0) && 
		(enteredTxtSize < kPGrp_FilterMaxBufferSize) && (formatterStringTxtSize < kPGrp_FilterMaxBufferSize) )
	{
		//  Now filter input from the user and loop through the parameter, copying only 0-9 into filteredTxt.
		fmx::uint32		filteredTxtSize = kPGrp_FilterMaxBufferSize;
		fmx::uint16		filteredTxt[kPGrp_FilterMaxBufferSize];
		Sub_GetFilteredChars(formatThis, filteredTxt, filteredTxtSize);

		if(filteredTxtSize > 0)
		{
			fmx::uint16		formatterStringTxt[kPGrp_FilterMaxBufferSize];
			withThis->GetUnicode(formatterStringTxt, 0, formatterStringTxtSize);

			fmx::uint16		tempUnichar = 0;
			unsigned long	onChar = 0;
			unsigned long	numOfnumerics = 0;

			// If the input had numbers in it, and if the input (numbers only) was 
			//		less than or equal to the formatter string, we can process the input. 
			if(filteredTxtSize <= formatterStringTxtSize)
			{
				// Count the number of #s.
				for (onChar=0; (onChar < formatterStringTxtSize); onChar++) 
				{
					tempUnichar = formatterStringTxt[onChar];
					if (tempUnichar == 0x0023)	//  0x0023  = '#'
					{
						numOfnumerics++;
					}

				} // for onChar <= formatterStringTxtSize

				
				if( (numOfnumerics > 0) && (filteredTxtSize <= numOfnumerics) )
				{
					// The output is always the same as the formatting string.
					fmx::uint16		formattedTxt[kPGrp_FilterMaxBufferSize];

					// Loop backwards, replacing the #s of the formatter string with the numbers of the user provided string.
					unsigned long		filteredTxtCharsLeft = filteredTxtSize;
					for(onChar=formatterStringTxtSize; (onChar > 0); onChar--) 
					{
						tempUnichar = formatterStringTxt[onChar-1];
						if (tempUnichar == 0x0023)	//  0x0023  = '#'
						{
							if(filteredTxtCharsLeft > 0)
							{								
								formattedTxt[onChar-1] = filteredTxt[ filteredTxtCharsLeft-1 ];
								filteredTxtCharsLeft--;
							}
							else
							{
								// If there are #s left in the formatter string, but no numbers left in the filtered text, then use zeros.
								formattedTxt[onChar-1] = 0x0030; 

							}// filteredTxtCharsLeft
						}
						else
						{
							// Copy over from formatterStringTxt directly into formattedTxt.
							formattedTxt[onChar-1] = tempUnichar;

						}// tempUnichar == 0x0023 

					}// onChar < formatterStringTxtSize

					intoThis->AssignUnicodeWithLength(formattedTxt, formatterStringTxtSize);
					errorResult = 0; // User input has been formatted successfully.

				}// numOfnumerics && filteredTxtSize <= numOfnumerics

			}// filteredTxtSize <= formatterStringTxtSize

		}// filteredTxtSize

	}// TxtSize > 0
	

return(errorResult);
} // FormatNumberWithMask



FMX_PROC(fmx::errcode) Do_PGrp_NumToWords(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode		errorResult = -1;
	fmx::uint32			numParams = dataVect.Size();
	fmx::TextAutoPtr	tempResult;

	if(numParams == 1) // The plug-in should have been given only one parameter.
	{
		fmx::TextAutoPtr	enteredTxt;
		unsigned long		enteredTxtSize = dataVect.AtAsText(0).GetSize();
		enteredTxt->SetText( dataVect.AtAsText(0), 0, enteredTxtSize);


		// By default, this function will return "-01", which means an error occurred and will also
		// return an error code. (See also FormatNumberWithMask.)
		{
			fmx::uint16	tempErrChars[4];
			tempErrChars[0] = 0x002D; // '-'
			tempErrChars[1] = 0x0030; // '0'
			tempErrChars[2] = 0x0031; // '1'

			tempResult->AssignUnicodeWithLength(tempErrChars, 3);
		}


		//First "compact" the number by removing all non-numbers. Locate the decimal 
		//	separator (stored in gFMPluginTahomaPrefs.decimalPoint) to use as a point of reference and cut off anything
		//	beyond two digits to the right of the decimal point.
		fmx::uint32		filteredTxtSize = kPGrp_FilterMaxBufferSize-4; // Leave room for trailing ".00" if needed.
		fmx::uint16		filteredTxt[kPGrp_FilterMaxBufferSize];
		Sub_GetFilteredChars(enteredTxt, filteredTxt, filteredTxtSize, true); 

		if(filteredTxtSize > 0)
		{
			unsigned long		decAt = 0;
			unsigned long		onChar;


			// Locate the decimal point, if any.  Remove any extra decimal points, and anything beyond two digits to the right of the decimal point.
			bool		foundADecimal = false;
			for(onChar = 0; (onChar<filteredTxtSize) && (!foundADecimal); onChar++)
			{
				if(filteredTxt[onChar] == gFMPluginTahomaPrefs.decimalPoint)
				{
					foundADecimal = true;
					decAt = onChar;

				}// gFMPluginTahomaPrefs.decimalPoint

			}// onChar<filteredTxtSize

			if(foundADecimal)
			{
				unsigned long	charsRightOfDecimal = filteredTxtSize-decAt-1;
				switch(charsRightOfDecimal)
				{
					case 0:
					{
						filteredTxt[filteredTxtSize] = 0x0030;
						filteredTxt[filteredTxtSize+1] = 0x0030;
						filteredTxtSize += 2;
					}
					break;

					case 1:
					{
						filteredTxt[filteredTxtSize] = 0x0030;
						filteredTxtSize++;
					}
					break;

					default: // There were two or more, adjust length appropriately.
					{
						filteredTxtSize = filteredTxtSize - (charsRightOfDecimal-2);
					}

				}// switch charsRightOfDecimal
			}
			else
			{
				// There was no decimal, so add one with trailing zeros.
				filteredTxt[filteredTxtSize] = gFMPluginTahomaPrefs.decimalPoint;
				filteredTxt[filteredTxtSize+1] = 0x0030;
				filteredTxt[filteredTxtSize+2] = 0x0030;

				decAt = filteredTxtSize;

				filteredTxtSize += 3;

			}// foundADecimal



		// At this point, filteredTxt should contain a number from ".00" to "999999999999.99", but with no more than 12 digits to the left of the decimal point.
			if(decAt < 13)
			{			
				int				magnitudePos;
				unsigned short	nextUnichar;
				bool			skipPlace;
				bool			millionsFlag=false;
				bool			thousandsFlag=false;
				bool			pluralCentsFlag=false;


				// At this point, the plug-in will return a result, so clear out the "-01"
				//		because the following code only appends text into tempResult.
				tempResult->DeleteText(0, 3);
				errorResult = 0;

				// The following code processes each character in filteredTxt, translating it into words.
				for (onChar=0; onChar < filteredTxtSize; onChar++)
				{
					skipPlace = false;   // Used for teens, when the character is 1 (i.e. 14 = fourteen).
					magnitudePos = (int)(decAt - onChar);  // Used for the significance of the decimal place.
					
					if ( (onChar+1) < filteredTxtSize ) 
					{
						nextUnichar = filteredTxt[onChar+1];
					}
					else
					{
						nextUnichar = 0;
					}

					// The remaining logic is contained in the function below:
					
					// magnitudePos will determine hundreds, thousands, etc.
					// filteredTxt[onChar] will determine three, four, etc.
					//		Whenthe number is one, nextUnichar will determine (4)fourteen, (5)fifteen, etc.
					// skipPlace will be false, unless both onThisNumChar and onNextNumChar are used.
					Sub_GetWordFromNum(onChar==0, magnitudePos, filteredTxt[onChar], nextUnichar, tempResult, skipPlace, millionsFlag, thousandsFlag, pluralCentsFlag);
					if (skipPlace)
					{
						onChar++;
					}								
					
				} // for onChar < filteredTxtSize

			}// decAt < 13


		}// filteredTxtSize
	

	}// numParams


	if(errorResult == 0)
	{
		results.SetAsText( *tempResult, dataVect.At(0).GetLocale() );
	}

return(errorResult);
} // Do_PGrp_NumToWords



bool Sub_GetWordFromNum(bool firstPos, int mag, 
						unsigned short firstch, unsigned short secondch, 
						fmx::TextAutoPtr& returnText, 
						bool& skipch, 
						bool& displayMillions, bool& displayThousands, bool& pluralcents)
{
	int					tempMag(mag);	// This function needs to change magnitude during processing.
	bool				tempReturn = false;
	fmx::TextAutoPtr	wordsToAdd;
	unsigned long		wordsToAddSize;
	unsigned long		wordsToAddID = 0;


// First, take care of displayMillions, displayThousands and pluralcents flags.
	if( (!displayMillions) && (firstch > 0x0030) )
	{
		displayMillions = ((tempMag >= 7) && (tempMag <= 9));

	}// displayMillions

	if( (!displayThousands) && (firstch > 0x0030) )
	{
		displayThousands = ((tempMag >= 4) && (tempMag <= 6));

	}// displayThousands

	if( (!pluralcents) && (firstch > 0x0030) )
	{
		// .23 -> Cents
		// .10 -> Cents
		// .02 -> Cents
		// .01 -> Cent
		pluralcents = (  ((tempMag == -2) && (firstch > 0x0031))  || (tempMag == -1) );

	}// pluralcents

	
// Determine hundreds, tens and teens, or ones.
	switch (tempMag) 
	{
		// Hundreds
		case 12 :  
		case  9 :
		case  6 :
		case  3 :
		{
			if((firstch >= 0x0031) && (firstch <= 0x0039)) // '1'..'9'
			{
				// Map firstch to kFMEX_NTW_1Hundred..kFMEX_NTW_9Hundred
				wordsToAddID = (unsigned long)(kFMEX_NTW_BaseHundred + ( (firstch-0x0031) * 100 ));

			}// firstch  '1'..'9'

			if(wordsToAddID != 0)
			{
				Do_GetString(wordsToAddID,  wordsToAdd);
				wordsToAddSize = wordsToAdd->GetSize();
				if(wordsToAddSize > 0)
				{
					returnText->AppendText(*wordsToAdd, 0, wordsToAddSize);
					tempReturn = true;

				}// wordsToAddSize

			}// wordsToAddID
		}
		break;// Hundreds


		// Tens / Teens
		case 11 :  
		case  8 :
		case  5 :
		case  2 :
		case -1 :
		{
			if((firstch >= 0x0032) && (firstch <= 0x0039)) // '2'..'9'
			{
				// Map firstch to kFMEX_NTW_Twenty..kFMEX_NTW_Ninty.
				wordsToAddID = (unsigned long)(kFMEX_NTW_BaseTens + ( (firstch-0x0032) * 10 ));
			}
			else
			{
				// This is the teens, so include the secondch and skip it in the next loop.
				if(firstch == 0x0031) // '1'
				{
					if((secondch >= 0x0030) && (secondch <= 0x0039)) // '0'..'9'
					{
						// Map firstch to kFMEX_NTW_Ten..kFMEX_NTW_Nineteen.
						wordsToAddID = (unsigned long)(kFMEX_NTW_BaseTeens + (secondch-0x0030));
						skipch = true;
						tempMag--;

					}// secondch		

				}// firstch == '1'

			}// firstch  '2'..'9'

			if((tempMag == -1) && (firstch > 0x0030))
			{
				pluralcents = true;
			}

			if(wordsToAddID != 0)
			{
				Do_GetString(wordsToAddID,  wordsToAdd);
				wordsToAddSize = wordsToAdd->GetSize();
				if(wordsToAddSize > 0)
				{
					tempReturn = true;
					returnText->AppendText(*wordsToAdd, 0, wordsToAddSize);
					if((secondch > 0x0030) && (!skipch))// Second digit means more is on the way ("Ninety" vs "Ninety-Five").
					{
						Do_GetString(kFMEX_NTW_Hyphen,  wordsToAdd);
						wordsToAddSize = wordsToAdd->GetSize();
						if(wordsToAddSize > 0)
						{
							// Remove the previous trailing space.
							unsigned long	returnTextCurSize = returnText->GetSize();
							if(returnTextCurSize > 0)
							{
								returnText->DeleteText( returnTextCurSize-1, 1);
							}
		
							// Add the hyphen.
							returnText->AppendText(*wordsToAdd, 0, wordsToAddSize);

						}// wordsToAddSize

					}// returnText[tempLen] = '-';

				}// wordsToAddSize

			}// wordsToAddID
		}
		break;// Tens / Teens


		// Ones
		case 10 :  
		case  7 :
		case  4 :
		case  1 :
		case -2 :
		{
			if((firstch >= 0x0031) && (firstch <= 0x0039)) // '1'..'9'
			{
				// Map firstch to kFMEX_NTW_One..kFMEX_NTW_Nine
				wordsToAddID = (unsigned long)(kFMEX_NTW_BaseOnes + (firstch-0x0031));

			}// firstch  '1'..'9'

			if( (tempMag == -2) && (firstch == 0x0030) && (!pluralcents) )
			{
				// ".00" -> No Cents
				wordsToAddID = kFMEX_NTW_NoCents;

			}// No Cents

			if(wordsToAddID != 0)
			{
				Do_GetString(wordsToAddID,  wordsToAdd);
				wordsToAddSize = wordsToAdd->GetSize();
				if(wordsToAddSize > 0)
				{
					returnText->AppendText(*wordsToAdd, 0, wordsToAddSize);
					tempReturn = true;

				}// wordsToAddSize

			}// wordsToAddID
		}
		break;// Hundreds

		case 0 :
		{
			// This is the decimal place, so don't do any processing here.
			tempReturn = true;
		}
		break;

	}// switch tempMag  Determine hundreds, tens and teens, or ones.



// Billions, Millions, Thousands, Dollars, And, Cents	

	// Always display even if 0 encountered in ones, thousands, millions or billions place.
	if(!tempReturn)
	{
		if(tempMag == 1)
		{
			tempReturn = true;
		}

		if((tempMag == 4) && displayThousands )
		{
			tempReturn = true;
		}
		
		if((tempMag == 7) && displayMillions )
		{
			tempReturn = true;
		}
		
		if(tempMag == 10)
		{
			tempReturn = true;
		}
		
	}// !tempReturn

	if(tempReturn)
	{
		wordsToAddID = 0;

		switch (tempMag) 
		{
			case 10 :
			{
				wordsToAddID = kFMEX_NTW_Billion;
			}
			break;

			case 7 :
			{
				if(displayMillions)// In cases like 8000000000, we don't want to display "million".
					wordsToAddID = kFMEX_NTW_Million;
			}
			break;

			case 4 :
			{
				if(displayThousands)// In cases like 8000000, we don't want to display "thousand".
					wordsToAddID = kFMEX_NTW_Thousand;
			}
			break;

			case 1 :
			{
				// If there is only one dollar, then display "dollar".
				if( (firstPos) && (firstch < 0x0032) && (!skipch) )
				{
					wordsToAddID = kFMEX_NTW_Dollar;
				}
				else
				{
					wordsToAddID = kFMEX_NTW_Dollars;
				}
			}
			break;

			case 0 :
			{
				if  (!firstPos)  // If there are cents, but no dollars, do not display "and".
				{
					wordsToAddID = kFMEX_NTW_And;
				}		
			}
			break;

			case -1 :
			{
				if( (firstch == 0x0030) && (secondch == 0x0030) )
				{
					skipch = true;
					wordsToAddID = kFMEX_NTW_NoCents;
				}
				else
				{
					if( (secondch == 0x0030) || ( pluralcents && skipch ) )
					{
						wordsToAddID = kFMEX_NTW_Cents;
						if(secondch == 0x0030)
						{
							skipch = true; // Processing is now complete, so skip the last decimal place.

						}// secondch == 0x0030

					}// x Cents

				}// "00"
			}
			break;

			case -2 :
			{
				// As in 02 or 11 cents.
				if((firstch > 0x0031) || (pluralcents))
				{
					wordsToAddID = kFMEX_NTW_Cents;
				}
				else
				{
					if(firstch == 0x0031) // This is needed for the special "No Cents" case, where firstChar will be '0' and pluralcents == false.
					{
						wordsToAddID = kFMEX_NTW_Cent;
					}

				}// pluralcents
			}
			break;

		}// switch tempMag


		if(wordsToAddID != 0)
		{
			Do_GetString(wordsToAddID,  wordsToAdd);
			wordsToAddSize = wordsToAdd->GetSize();
			if(wordsToAddSize > 0)
			{
				returnText->AppendText(*wordsToAdd, 0, wordsToAddSize);
				tempReturn = true;

			}// wordsToAddSize

		}// wordsToAddID

	}// Billions, Millions, Thousands, Dollars, Cents	


return(tempReturn);
} // Sub_GetWordFromNum



void Sub_GetFilteredChars(const fmx::TextAutoPtr& filterThis, fmx::uint16*
filteredIntoHere, fmx::uint32& filteredIntoHereMaxSize, bool saveDecimal)
{
	unsigned long	filterThisSize(filterThis->GetSize());
	if((filterThisSize > 0) && (filteredIntoHere != NULL) && (filteredIntoHereMaxSize > 0))
	{
		unsigned long	tempSize = 0;
		filteredIntoHere[0] = 0;

		fmx::uint16		tempScanBuffer[kPGrp_FilterMaxBufferSize];
		filterThis->GetUnicode(tempScanBuffer, 0, filterThisSize);

		bool			gotNonLeadingZero = false;
		bool			alreadyGotADecimal = false;
		bool			curUnicharIsValidNumber = false;
		fmx::uint16		tempUnichar = 0;
		for(unsigned long onChar=0; (onChar<filterThisSize) && (tempSize<filteredIntoHereMaxSize); onChar++)
		{
			tempUnichar = tempScanBuffer[onChar];

			// To this routine, a valid character is anything from'0'..'9', but only if we have already been given
			//		a character in the range of '1'..'9'. If saveDecimal is true, the character stored in
			//		gFMPluginTahomaPrefs.decimalPoint is also a valid character, but only once.  More than
			//		one decimal point is not valid.
			curUnicharIsValidNumber = (  ((tempUnichar >= 0x0030) && (tempUnichar <= 0x0039)) || ((saveDecimal) && (tempUnichar==gFMPluginTahomaPrefs.decimalPoint)) );
			if( (curUnicharIsValidNumber) && (alreadyGotADecimal) && (tempUnichar == gFMPluginTahomaPrefs.decimalPoint))
			{
				curUnicharIsValidNumber = false;
			}


			if( (!gotNonLeadingZero) && (tempUnichar != 0x0030) && (curUnicharIsValidNumber) )
			{
				gotNonLeadingZero = true; // Got a non-zero leading number character ('1'..'9'), or a decimal point, if applicable.

			}// !gotNonLeadingZero

			if((gotNonLeadingZero) && (curUnicharIsValidNumber))
			{
				filteredIntoHere[tempSize] = tempUnichar;
				tempSize++;

				if(tempUnichar == gFMPluginTahomaPrefs.decimalPoint)
				{
					alreadyGotADecimal = true;
				}

			}// gotNonLeadingZero

		}// for onChar<enteredTxtSize

		filteredIntoHereMaxSize = tempSize;
		
	}// filterThisSize

} // Sub_GetFilteredChars


//LevenshteinDistance: returns the edit-distance between strings a and b, or -1 if error
int LevenshteinDistance(const std::string &a, const std::string &b)
{
	int iLenA = (int)a.size();
	int iLenB = (int)b.size();

	int *c = new int[(iLenA + 1) * (iLenB + 1)];
	if(NULL == c)
	{
		return -1; //memory error
	}

	for(int j=0; j <= iLenA; j++)
	{
		for(int k=0; k <= iLenB; k++)
		{
			if(0 == j)
			{
				c[k] = k;
			}
			else if (0 == k)
			{
				c[j * (iLenB + 1)] = j;
			}
			else
			{
				//TODO: consider transposition mod, i.e., swapping two adjacent characters w/ cost 1
				int change	= c[ (j - 1) * (iLenB + 1) + (k - 1) ] + ( ( toupper( a[j - 1] ) == toupper( b[k - 1] ) ) ? 0 : 1 ); //note use of touppers...not necessarily efficient
				int insert	= c[ (j - 1) * (iLenB + 1) + (k)	 ] + 1;
				int del		= c[ (j)     * (iLenB + 1) + (k - 1) ] + 1;
				
				c[ j * (iLenB + 1) + k ] = Min( Min( change, insert ), del );
			}
		}
	}

	int result = c[ (iLenA + 1) * (iLenB + 1) - 1 ];

	delete [] c; //free memory

	return result;
} //LevenshteinDistance

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
	  but this function does 'mnmn' -> 666 ... not a big deal I think
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

/*
void SortSet(const vector<string> &in, bool bAscending, vector<string> &result)
{
    //use STL to sort string vector 
    result = in;	    
	
	if(bAscending)
	{
		sort(result.begin(), result.end());
	}
	else
	{
		sort(result.rbegin(), result.rend());
	}
} // SortSet



void SetsOfSet(const vector<string> &in, vector<string> &result, const string &delim)
{   
	//returns all subsets of the input set
	result.clear();
	
    if(in.size() > 16)
    {
    	result.push_back("ERROR: too many elements (>16) in input set");
    	return;
    }
    
    //now convert input vector into stack
	stack<string> theSet;
	for(vector<string>::iterator it = in.begin(); it != in.end(); it++)
	{
        theSet.push(*it);
	}
    
    //now we have a stack with our set, so we can process the thing
    stack<string> temp; //empty stack    
    SetRecurse(theSet, temp, result, delim);

    //all set!
}// SetsOfSet
*/

void Redux(const string &in, string &result)
{
	//String redux
    int len = in.size();
	
	//temp storage string
	char *t = new char[len + 1];
	char *p = t;

	result.clear();
	
	memcpy(t, in.c_str(), len);
	
	while(*p)
	{
		for(int i=len; i > p-t; i--)
		{
			result.append(p, i - (p - t) );
			result += '\r'; //linefeed
		}
		
		while(*p != ' ' && *p)  //split at spaces
		    p++;
				
		if(!*p)
			break;
		
		p++;
	}
		
	delete t;
}// Redux

void SetRecurse(stack<string> uc, stack<string> c, vector<string> &result, const string &delim)
{
	//recursive function to actually build sets of set
	string t;
	
    if(uc.empty())
    {
        //if nothing has been left unconsidered, generate the next result
        while(!c.empty())
        {
			if(0 != t.size())
			{
				t += delim;
			}
            
            t += c.top();
            c.pop();
        }
		
        result.push_back(t);
        
		return;
    }
    
    //otherwise, consider each possibility for the top element
    t = uc.top();
    uc.pop();
    
    //generate set(s) without this element
    SetRecurse(uc,c,result,delim);
    
    //generate set(s) with this element
    c.push(t);
    SetRecurse(uc,c,result,delim);

}// SetRecurse



void Do_GetString(unsigned long whichString, fmx::uint32 /* winLangID */,
fmx::int32 resultsize, fmx::unichar* string)
{
	bool		processedSpecialStringID = false;

	// Map whichString (if needed).
	// kFMXT_OptionsStr is not the same as kPGrp_OptionsStringID.
	switch (whichString)
	{
		case kFMXT_OptionsStr:
		{
			#if defined(FMX_WIN_TARGET)
				LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), kPGrp_OptionsStringID, (LPWSTR)string, resultsize);
				processedSpecialStringID = true;
			#endif
			
			#if defined(FMX_MAC_TARGET)
				Sub_OSXLoadString(kPGrp_OptionsStringID, string, resultsize);
				processedSpecialStringID = true;
			#endif
		}
		break;

	}// switch (whichString)

	
	if( !processedSpecialStringID )
	{
		#if defined(FMX_WIN_TARGET)
			LoadStringW( (HINSTANCE)(gFMX_ExternCallPtr->instanceID), (unsigned int)whichString, (LPWSTR)string, resultsize);
		#endif

		#if defined(FMX_MAC_TARGET)
			Sub_OSXLoadString(whichString, string, resultsize);
		#endif

	}// !processedSpecialStringID

} // Do_GetString (FMX_Unichar* version)



enum { kPGrp_GetStringMaxBufferSize = 1024 };

void Do_GetString(unsigned long whichStringID, fmx::TextAutoPtr& intoHere, bool stripFunctionParams)
{
	FMX_Unichar			tempBuffer[kPGrp_GetStringMaxBufferSize];

	Do_GetString(whichStringID, 0, kPGrp_GetStringMaxBufferSize, tempBuffer);
	intoHere->AssignUnicode(tempBuffer);
	
	if(stripFunctionParams)
	{
		// The string for this whichStringID is a Function Prototype, but all the plug-in needs now is the Function Name by itself.

		fmx::TextAutoPtr		parenToken;
		parenToken->Assign("(");

		unsigned long		originalSize = intoHere->GetSize();
		unsigned long		firstParenLocation; 
		firstParenLocation = intoHere->Find(*parenToken, 0);
	
		intoHere->DeleteText(firstParenLocation, originalSize-firstParenLocation);

	}// stripFunctionParams

} // Do_GetString (TextAutoPtr version)



#if defined(FMX_MAC_TARGET)

	unsigned long Sub_OSXLoadString(unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax)
	{
		unsigned long		returnResult = 0;
		
		
		if( (intoHere != NULL) && (intoHereMax > 1) )
		{
			// Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
			CFStringRef 	strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR("FMPluginTahoma %d"), stringID );
			
			// Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
			CFStringRef 	osxStr = CFBundleCopyLocalizedString( reinterpret_cast<CFBundleRef>(gFMX_ExternCallPtr->instanceID), strIdStr, strIdStr, CFSTR("FMPluginTahoma") );
			if((osxStr != NULL) && (osxStr != strIdStr))
			{
				long	osxStrLen = CFStringGetLength(osxStr);
				if( osxStrLen < (intoHereMax-1) )
				{
					CFRange		allChars;
					allChars.location = 0;
					allChars.length = osxStrLen;
					
					CFStringGetCharacters(osxStr, allChars, (UniChar*)(intoHere));
					intoHere[osxStrLen] = 0x0000;
					returnResult = (unsigned long)osxStrLen;
					
				}// osxStrLen
				
				CFRelease( osxStr );
				
			}// osxStr
			
			CFRelease( strIdStr );
			
		}// intoHere
			
	return(returnResult);
	
	} // Sub_OSXLoadString
	
#endif



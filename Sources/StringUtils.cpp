/*
 *  StringUtils.cpp
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

#include "StringUtils.h"
#include "FMPlugin.h"
#include "FMTemplate/FMTemplate.h"

#define Min(x,y)	(((x) < (y)) ? (x) : (y))

FMX_PROC(fmx::errcode) PGrp_StringRedux(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	// PGrp_StringRedux( text )
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
}// PGrp_StringRedux


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

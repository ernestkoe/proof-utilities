/*
 *  Levenshtein.cpp
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

#include "Levenshtein.h"
#include "FMPlugin.h"
#include "FMTemplate/FMTemplate.h"
#include "StringUtils.h"

#define Min(x,y)	(((x) < (y)) ? (x) : (y))

FMX_PROC(fmx::errcode) PGrp_LDistance(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& results)
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
} // PGrp_LDistance

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

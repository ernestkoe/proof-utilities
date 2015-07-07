/*
 *  ExecuteSQL.cpp
 *  FMTemplate.XC23
 *
 *  Created by Corn Walker on 11/6/07.
 *  Copyright 2007 Proof Group LLC. All rights reserved.
 *
 */

#if defined(FMX_WIN_TARGET)
	#include <windows.h>
	#include <locale.h>
#endif

#if defined(FMX_MAC_TARGET)
	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>
#endif 

//#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXData.h"

#include "ExecuteSQL.h"

#define Min(x,y)	(((x) < (y)) ? (x) : (y))

/* Very bare-bones implementation of the ExecuteSQL function
 * Not for general consumption
 *
 * To Do:
 *  Allow user-specified column/row separators
 *  Include SQL syntax parsing library
 *  Error handling
 */

FMX_PROC(fmx::errcode) PGrp_ExecuteSQL(short /* funcId */, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& results)
{
	fmx::errcode errorResult = 0;

	//Grab the row and column separators
		fmx::ushort		col = static_cast<fmx::ushort>('\v'); // delimiters are hardcoded for now 
		fmx::ushort		row = static_cast<fmx::ushort>('\r'); 
	//	col->AssignUnicode( dataVect.AtAsText(1) );
	//	row->AssignUnicode( dataVect.AtAsText(2) );

	//	fmx::FixPtAutoPtr		test;
	//	test->AssignFixPt( dataVect.AtAsNumber(1) ); // we can get it as FixPt, but then ???


	errorResult = environment.ExecuteSQL( dataVect.AtAsText(0), results, col, row );


return(errorResult);
} // PGrp_ExecuteSQL

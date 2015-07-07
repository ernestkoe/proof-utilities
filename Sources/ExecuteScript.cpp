/*
 *  ExecuteScript.cpp
 *  
 *
 *  Created by Corn Walker on 11/7/07.
 *  Copyright 2007 Proof Group LLC. All rights reserved.
 *
 */

#include "FMPlugin.h"
#include "FMTemplate/FMTemplate.h"

FMX_PROC(fmx::errcode) PGrp_ExecuteScript(short /* funcId */, const fmx::ExprEnv& /* environment */, const fmx::DataVect& dataVect, fmx::Data& /* results */)
{
	fmx::errcode errorResult = 0;
	
	fmx::ulong nParams = dataVect.Size();

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
} // PGrp_ExecuteScript

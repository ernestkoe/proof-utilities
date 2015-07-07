/*
 *  StringUtils.h
 *  FMTemplate.XC23
 *
 *  Created by Corn Walker on 11/6/07.
 *  Copyright 2007 Proof Group LLC. All rights reserved.
 *
 */

#if !defined(STRINGUTILS_H)
	#define STRINGUTILS_H

#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"

#pragma once

#include <string>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#include <ctype.h>
#include <assert.h>


// Used by PGrp_LDistance, PGrp_SoundexAmerican, PGrp_SoundexDM
	void FMTextToString(const fmx::Text &, std::string &);
// Used by ?
    void SetStringToVector(const string &, const string &, vector<string> &);
// Used by PGrp_Redux
	void Redux(const string &, string &);
	void SetRecurse(stack<string>, stack<string>, vector<string> &, const string &);

#endif // STRINGUTILS_H
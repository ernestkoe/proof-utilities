/*
 *  Soundex.h
 *  FMTemplate.XC23
 *
 *  Created by Corn Walker on 11/6/07.
 *  Copyright 2007 Proof Group LLC. All rights reserved.
 *
 */

#if !defined(SOUNDEX_H)
	#define SOUNDEX_H

#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"


#pragma once

#include <string>
//#include <stack>
//#include <vector>
//#include <algorithm>

using namespace std;

#include <ctype.h>
#include <assert.h>

// Used by PGrp_SoundexAmerican and PGrp_SoundexDM.
std::string SoundexAmerican(const std::string &);
std::string SoundexDM(const std::string &);

#endif //SOUNDEX_H
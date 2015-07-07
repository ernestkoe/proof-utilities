/*
 *  Levenshtein.h
 *  FMTemplate.XC23
 *
 *  Created by Corn Walker on 11/6/07.
 *  Copyright 2007 Proof Group LLC. All rights reserved.
 *
 */

#if !defined(LEVENSHTEIN_H)
	#define LEVENSHTEIN_H

#pragma once

#include <string>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#include <ctype.h>
#include <assert.h>


// Used by PGrp_LDistance.
	int LevenshteinDistance(const std::string &, const std::string &);

#endif // LEVENSHTEIN_H
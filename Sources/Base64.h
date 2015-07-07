/*
 *  Base64.h
 *
 *  Created by Corn Walker on 04JUL2008.
 *  Copyright 2008 Proof Group LLC. All rights reserved.
 *
 */

#ifndef BASE64_H_
#define BASE64_H_

#include <inttypes.h>
#include <string>
#include <sstream>
#include <memory>

using namespace std;

typedef unsigned char guint8;
typedef unsigned char uint8;

//Used by PGrp_Base64_Encode
std::string encode(uint8_t* , size_t );
//Used by PGrp_Base64_Decode
std::auto_ptr< uint8_t > decode(const std::string& );


#endif // BASE64_H_
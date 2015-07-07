/*
 *	Copyright 1998-2005 FileMaker, Inc.  All Rights Reserved.
 * 
 * FMXClient.h is part of the FileMaker Plug-in SDK.
 *
 * THIS FILE CANNOT BE REDISTRIBUTED in source code (or human readable)
 * form, it cannot be modified, and it is only provided to enable licensees of
 * FileMaker Pro Advanced to compile plug-ins for use with FileMaker products.
 *
 */

#ifndef _h_Client_
#define _h_Client_

#include <stdexcept>
#include <memory>
#include <new>

#include "FMWrapper/FMXExtern.h"

extern "C++"
{
	namespace fmx
	{
		// This is used by other files for exception handling.
		struct _fmxcpt
		{
			enum
			{
				kNoErr,
				kBadAlloc,
				kUnknown
			};
			
			inline _fmxcpt ()
				: m_vers ( 1 )
				, m_code ( kNoErr )
			{
			}
			
			inline void Check ()
			{
				switch ( m_code )
				{
				case kNoErr:
					break;
				case kBadAlloc:
					throw std::bad_alloc ();
				case kUnknown:
					throw std::exception ();
				}
			}
			
			long m_vers;
			long m_code;
		};
	}
}

#endif
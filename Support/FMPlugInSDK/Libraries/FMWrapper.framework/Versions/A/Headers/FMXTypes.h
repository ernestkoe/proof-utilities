/*
 *	Copyright 1998-2005 FileMaker, Inc.  All Rights Reserved.
 * 
 * FMXTypes.h is part of the FileMaker Plug-in SDK.
 *
 * THIS FILE CANNOT BE REDISTRIBUTED in source code (or human readable)
 * form, it cannot be modified, and it is only provided to enable licensees of
 * FileMaker Pro Advanced to compile plug-ins for use with FileMaker products.
 *
 */

#ifndef _h_Types_
#define _h_Types_

#include "FMWrapper/FMXClient.h"

extern "C++"
{
    namespace fmx
    {
        class QuadChar
        {
        public:
            inline const QuadChar &operator = ( const QuadChar &value );
            inline uchar operator [] ( int i );
            inline const uchar operator [] ( int i ) const;
            inline bool operator == ( const QuadChar &value ) const;
            inline bool operator != ( const QuadChar &value ) const;
            inline bool operator < ( const QuadChar &value ) const;
            inline bool operator <= ( const QuadChar &value ) const;
            inline bool operator > ( const QuadChar &value ) const;
            inline bool operator >= ( const QuadChar &value ) const;
            inline ulong GetMacType () const;
            inline void SetMacType ( ulong value );
            inline void operator delete ( void *obj );

        private:
            QuadChar ();
            QuadChar ( const QuadChar &value );

        };

        class Locale
        {
        public:

            enum Type
            {
				kType_None				= 0,	// Empty
				kType_System			= 1,	// Uses system settings
				kType_UnicodeRaw		= 2,	// Use raw unicode bytes (like the old ASCII ordering)
				kType_UnicodeStandard	= 3,	// Standard unicode rules
				kType_Catalog			= 4,	// FileMaker list rules
				kType_CAT				= 16,	// Catalan
				kType_HRV				= 17,	// Croatian
				kType_CES				= 18,	// Czech
				kType_DAN				= 19,	// Danish
				kType_NLD				= 20,	// Dutch
				kType_ENG				= 21,	// English
				kType_FIN				= 22,	// Finnish
				kType_FIN_fmi			= 23,	// Finnish (FileMaker custom)
				kType_FRA				= 24,	// French
				kType_DEU				= 25,	// German
				kType_DEU_dictionary	= 26,	// German (dictionary ordering)
				kType_ELL				= 27,	// Greek
				kType_HUN				= 28,	// Hungarian
				kType_ISL				= 29,	// Icelandic
				kType_ITA				= 30,	// Italian
				kType_JPN				= 31,	// Japanese
				kType_NOR				= 32,	// Norwegian
				kType_POL				= 33,	// Polish
				kType_POR				= 34,	// Portuguese
				kType_RON				= 35,	// Romanian
				kType_RUS				= 36,	// Russian
				kType_SLK				= 37,	// Slovak
				kType_SLV				= 38,	// Slovenian
				kType_SPA				= 39,	// Spanish
				kType_SPA_traditional	= 40,	// Spanish (traditional)
				kType_SWE				= 41,	// Swedish
				kType_SWE_fmi			= 42,	// Swedish (FileMaker custom)
				kType_TUR				= 43,	// Turkish
				kType_UKR				= 44,	// Ukrainian
 
				// Chinese was added in 8.5
				kType_CHI				= 45,	// Chinese (PinYin)
				kType_CHI_stroke		= 46,	// Chinese (Stroke-radical)

				// These returned by versions distributed by WinSoft
				kType_HE				= 47,
				kType_HI				= 48,
				kType_AR				= 49,
				kType_ET				= 50,
				kType_LT				= 51,
				kType_LV				= 52,
				kType_SR				= 53,
				kType_FA				= 54,
				kType_BG				= 55,
				kType_VI				= 56,
				kType_TH				= 57,
				kType_Invalid			= 0xFFFF
           };

            inline Locale &operator = ( const Locale &rhs );
            inline void operator delete ( void *obj );

        private:
            inline Locale ( Type inputType );
            inline Locale ( const Locale &copyConstruct );

        };

        class QuadCharAutoPtr : public std::auto_ptr<QuadChar>
        {
			typedef QuadCharAutoPtr	UpCaster;
        public:
            inline QuadCharAutoPtr ();
            inline QuadCharAutoPtr ( char c0, char c1, char c2, char c3 );
            inline QuadCharAutoPtr ( const QuadChar &value );

        };

        class LocaleAutoPtr : public std::auto_ptr<Locale>
        {
			typedef LocaleAutoPtr	UpCaster;
        public:
            inline LocaleAutoPtr ( Locale::Type inputType = Locale::kType_System );
            inline LocaleAutoPtr ( const Locale &copyConstruct );

        };
    }
}


// These functions are only useful for C-only environments or for porting to other languages (ex., Pascal).
extern "C"
{
    fmx::QuadChar FMX_API *FM_QuadChar_Constructor1 ( fmx::_fmxcpt &_x ) throw ();
    fmx::QuadChar FMX_API *FM_QuadChar_Constructor2 ( char c0, char c1, char c2, char c3, fmx::_fmxcpt &_x ) throw ();
    fmx::QuadChar FMX_API *FM_QuadChar_Constructor3 ( const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    const fmx::QuadChar FMX_API &FM_QuadChar_operatorAS ( void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
	fmx::uchar FMX_API FM_QuadChar_operatorAR ( void *_self, int i, fmx::_fmxcpt &_x ) throw ();
    const fmx::uchar FMX_API FM_QuadChar_operatorCAR ( const void *_self, int i, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_QuadChar_operatorEQ ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_QuadChar_operatorNE ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_QuadChar_operatorLT ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_QuadChar_operatorLE ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_QuadChar_operatorGT ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    bool FMX_API FM_QuadChar_operatorGE ( const void *_self, const fmx::QuadChar &value, fmx::_fmxcpt &_x ) throw ();
    fmx::ulong FMX_API FM_QuadChar_GetMacType ( const void *_self, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_QuadChar_SetMacType ( void *_self, fmx::ulong value, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_QuadChar_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

    fmx::Locale FMX_API *FM_Locale_Constructor1 ( long inputType, fmx::_fmxcpt &_x ) throw ();
    fmx::Locale FMX_API *FM_Locale_Constructor2 ( const fmx::Locale &copyConstruct, fmx::_fmxcpt &_x ) throw ();
    fmx::Locale FMX_API &FM_Locale_operatorAS ( void *_self, const fmx::Locale &rhs, fmx::_fmxcpt &_x ) throw ();
    void FMX_API FM_Locale_Delete ( void *_self, fmx::_fmxcpt &_x ) throw ();

}


extern "C++"
{
    namespace fmx
    {		
        inline const QuadChar &QuadChar::operator = ( const QuadChar &value )
        {
            _fmxcpt _x;
            const QuadChar &_rtn = FM_QuadChar_operatorAS ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline uchar QuadChar::operator [] ( int i )
        {
            _fmxcpt _x;
            uchar _rtn = FM_QuadChar_operatorAR ( this, i, _x );
            _x.Check ();
            return _rtn;
        }
        inline const uchar QuadChar::operator [] ( int i ) const
        {
            _fmxcpt _x;
            const uchar _rtn = FM_QuadChar_operatorCAR ( this, i, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool QuadChar::operator == ( const QuadChar &value ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_QuadChar_operatorEQ ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool QuadChar::operator != ( const QuadChar &value ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_QuadChar_operatorNE ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool QuadChar::operator < ( const QuadChar &value ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_QuadChar_operatorLT ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool QuadChar::operator <= ( const QuadChar &value ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_QuadChar_operatorLE ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool QuadChar::operator > ( const QuadChar &value ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_QuadChar_operatorGT ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline bool QuadChar::operator >= ( const QuadChar &value ) const
        {
            _fmxcpt _x;
            bool _rtn = FM_QuadChar_operatorGE ( this, value, _x );
            _x.Check ();
            return _rtn;
        }
        inline ulong QuadChar::GetMacType () const
        {
            _fmxcpt _x;
            ulong _rtn = FM_QuadChar_GetMacType ( this, _x );
            _x.Check ();
            return _rtn;
        }
        inline void QuadChar::SetMacType ( ulong value )
        {
            _fmxcpt _x;
            FM_QuadChar_SetMacType ( this, value, _x );
            _x.Check ();
        }
        inline void QuadChar::operator delete ( void *obj )
        {
            _fmxcpt _x;
            FM_QuadChar_Delete ( obj, _x );
            _x.Check ();
        }

        inline QuadCharAutoPtr::QuadCharAutoPtr ()
        {
            _fmxcpt _x;
            FMX_AUTOPTRRESET ( FM_QuadChar_Constructor1 ( _x ) );
            _x.Check ();
        }
        inline QuadCharAutoPtr::QuadCharAutoPtr ( char c0, char c1, char c2, char c3 )
        {
            _fmxcpt _x;
            FMX_AUTOPTRRESET ( FM_QuadChar_Constructor2 ( c0, c1, c2, c3, _x ) );
            _x.Check ();
        }
        inline QuadCharAutoPtr::QuadCharAutoPtr ( const QuadChar &value )
        {
            _fmxcpt _x;
            FMX_AUTOPTRRESET ( FM_QuadChar_Constructor3 ( value, _x ) );
            _x.Check ();
        }

        inline Locale &Locale::operator = ( const Locale &rhs )
        {
            _fmxcpt _x;
            Locale &_rtn = FM_Locale_operatorAS ( this, rhs, _x );
            _x.Check ();
            return _rtn;
        }
        inline void Locale::operator delete ( void *obj )
        {
            _fmxcpt _x;
            FM_Locale_Delete ( obj, _x );
            _x.Check ();
        }

        inline LocaleAutoPtr::LocaleAutoPtr ( Locale::Type inputType )
        {
            _fmxcpt _x;
            FMX_AUTOPTRRESET ( FM_Locale_Constructor1 ( inputType, _x ) );
            _x.Check ();
        }
        inline LocaleAutoPtr::LocaleAutoPtr ( const Locale &copyConstruct )
        {
            _fmxcpt _x;
            FMX_AUTOPTRRESET ( FM_Locale_Constructor2 ( copyConstruct, _x ) );
            _x.Check ();
        }

    }
}


#endif /* _h_Types_ */

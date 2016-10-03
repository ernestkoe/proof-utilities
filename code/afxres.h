/* This is a replacement for 'afxres.h' that is a part of Microsoft Foundation
 * Classes (MFC). It's referenced in the sample code because Microsoft Visual
 * Studio seems to insert it into default templates regardless of whether they
 * really use MFC or not. At the same time the only (legal) way to obtain MFC
 * is to have the professional Visual Studio; the Windows SDK or the Express
 * edition of Visual Studio do not include them. Suprisingly it's a very
 * shallow header. */

#ifndef _AFXRES_H
#define _AFXRES_H
#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WINDOWS_H
#include <windows.h>
#endif

/* IDC_STATIC is documented in winuser.h, but not defined. */
#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

#ifdef __cplusplus
}
#endif
#endif   

// -------------------------------------------------------------
// (p) & (c) 2000: Roland J. Graf, AUSTRIA
//                 eSplines.com  
//                 roland.graf@aon.at
// -------------------------------------------------------------

// Extended time string formatting with millisecond

#ifndef __EXTTIMEFORMAT_H__
#define __EXTTIMEFORMAT_H__

#include <minwinbase.h>

extern "C" {
   const char* strfscode( const char* code );
   size_t strfstime( char *strDest, size_t maxsize, const char *format, const SYSTEMTIME *stimeptr );

   const char* wcsfscode( const wchar_t* code );
   size_t wcsfstime( wchar_t *strDest, size_t maxsize, const wchar_t *format, const SYSTEMTIME *stimeptr );
};

// -------------------------------------------------------------

#ifdef  _UNICODE
   #define _tcsfstime   wcsftime
#else // _MBCS
   #define _tcsfstime   strfstime
#endif

// -------------------------------------------------------------


#endif // __EXTTIMEFORMAT_H__


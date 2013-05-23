// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// CSTRINGT.H - Framework-independent, templateable string class

#ifndef __CSTRINGT_H__
#define __CSTRINGT_H__

#pragma once

#ifndef _ATL_NO_PRAGMA_WARNINGS
#pragma warning (push)
#pragma warning(disable : 4668)	// is not defined as a preprocessor macro, replacing with '0' for '#if/#elif
#pragma warning(disable : 4820)	// padding added after member
#pragma warning(disable : 4127)	// 
#pragma warning(disable : 4995)
#pragma warning(disable : 4996)
#endif //!_ATL_NO_PRAGMA_WARNINGS


#pragma warning(disable:4786)	// avoid 255-character limit warnings

#ifdef _MANAGED
#include <vcclr.h>  // For PtrToStringChars
#endif

#include <atlsimpstr.h>
#include <stddef.h>

#ifndef _INC_NEW
#include <new.h>
#endif

#include <stdio.h>
#include <wchar.h>
#include <limits.h>

#ifndef _ATL_MIN_CRT
#include <mbstring.h>
#endif

#ifdef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define CSTRING_EXPLICIT explicit
#else
#define CSTRING_EXPLICIT
#endif

#include <atlconv.h>
#include <atlmem.h>

#ifndef DWORD_MAX
#define DWORD_MAX     0xffffffffUL  /* maximum DWORD value */
#endif

struct tagVARIANT;
typedef tagVARIANT VARIANT;

#ifndef _AFX
#define _AFX_FUNCNAME(_Name) _Name
#endif

#pragma push_macro("new")
#undef new

#if defined(_AFX)
#pragma push_macro("FormatMessage")
#undef FormatMessage
#endif

/////////////////////////////////////////////////////////////////////////////
// Naming conventions:
//    The term "length" can be confusing when dealing with ANSI, Unicode, and
//    MBCS character sets, so this file will use the following naming 
//    conventions to differentiate between the different meanings of 
//    "length":
//
//    'Byte Length' - Length of a buffer in bytes, regardless of character 
//       size
//    'Char Length' - Number of distinct characters in string.  For wide-
//       character strings, this is equivalent to half the 'Byte Length'.  
//       For ANSI strings, this is equivalent to the 'Byte Length'.  For MBCS
//       strings, 'Char Length' counts a lead-byte/trail-byte combination
//       as one character.
//    'Length' - When neither of the above terms is used, 'Length' refers to 
//       length in XCHARs, which is equal to 'Byte Length'/sizeof(XCHAR).
/////////////////////////////////////////////////////////////////////////////

namespace ATL
{

// This struct have the same memory layout as CString and is used to enable
// const statically initialized CString without making a copy on the heap
template <class StringType,int t_nChars> struct CConstFixedStringT
{
	CStringData m_data;
	typename StringType::XCHAR m_achData[t_nChars];
};

#define IMPLEMENT_CONST_STRING_PTR(StringType, value, name) const CConstFixedStringT<StringType, sizeof(value)/sizeof(StringType::XCHAR)> _init##name ={ 	{NULL,		sizeof(value)/sizeof(StringType::XCHAR)-1, 	sizeof(value)/sizeof(StringType::XCHAR)-1, 	-1},			value	};	const StringType::XCHAR* const _value##name = _init##name.m_achData;	extern const StringType* const name = CONST_STRING_PTR(StringType, name);
#define DECLARE_CONST_STRING_PTR(StringType, name) extern const StringType* const name;
#define CONST_STRING_PTR(StringType, name) reinterpret_cast<const StringType* const>(&_value##name)

/////////////////////////////////////////////////////////////////////////////
// inline helpers

inline int _wcstombsz(__out_ecount(count) char* mbstr, __in_z const wchar_t* wcstr, __in ULONG count) throw()
{
	// count is number of bytes
	if (count == 0 && mbstr != NULL)
		return 0;

	int result = ::WideCharToMultiByte(_AtlGetConversionACP(), 0, wcstr, -1,
		mbstr, count, NULL, NULL);
	ATLASSERT(mbstr == NULL || result <= (int)count);
	return result;
}

inline int _mbstowcsz(__out_ecount_z(count) wchar_t* wcstr, __in_z const char* mbstr, __in ULONG count)
{
	// count is number of wchar_t's
	if (count == 0 && wcstr != NULL)
		return 0;

	int result = ::MultiByteToWideChar(_AtlGetConversionACP(), 0, mbstr, -1,
		wcstr, count);
	ATLENSURE(wcstr == NULL || result <= (int)count);
	if ((result > 0) && (wcstr != NULL) && (ULONG)result <= count)
	{
		wcstr[result-1] = 0;
	}

	return result;
}

#if !defined(_UNICODE) || defined(_CSTRING_ALWAYS_THUNK)
// Win9x doesn't support Unicode versions of these useful string functions.
// If the app was built without _UNICODE defined, we thunk at runtime to
// either the real Unicode implementation (on NT), or a conversion helper
// (on Win9x).

#pragma warning( push )
#pragma warning( disable : 4793 )

inline void _AtlInstallStringThunk(void** ppThunk, void* pfnWin9x, void* pfnNT) throw()
{
#pragma warning (push)
#pragma warning (disable : 4640)	// construction of local static object is not thread-safe

	static bool s_bWin9x = (::GetVersion()&0x80000000) != 0;
	
#pragma warning (pop)

	void* pfn;
	if (s_bWin9x)
		pfn = pfnWin9x;
	else
	{
#ifdef _CSTRING_ALWAYS_THUNK
		pfn = pfnWin9x;
		(void)pfnNT;
#else
		pfn = pfnNT;
#endif
	}
	InterlockedExchangePointer(ppThunk, pfn);
}

#pragma warning( pop )

typedef int (WINAPI* ATLCOMPARESTRINGW)(LCID, DWORD, LPCWSTR, int, LPCWSTR, int);
typedef BOOL (WINAPI* ATLGETSTRINGTYPEEXW)(LCID, DWORD, LPCWSTR, int, LPWORD);
typedef int (WINAPI* ATLLSTRCMPIW)(LPCWSTR, LPCWSTR);
typedef LPWSTR (WINAPI* ATLCHARLOWERW)(LPWSTR);
typedef LPWSTR (WINAPI* ATLCHARUPPERW)(LPWSTR);
typedef DWORD (WINAPI* ATLGETENVIRONMENTVARIABLEW)(LPCWSTR, LPWSTR, DWORD);

struct _AtlStringThunks
{
	ATLCOMPARESTRINGW pfnCompareStringW;
	ATLGETSTRINGTYPEEXW pfnGetStringTypeExW;
	ATLLSTRCMPIW pfnlstrcmpiW;
	ATLCHARLOWERW pfnCharLowerW;
	ATLCHARUPPERW pfnCharUpperW;
	ATLGETENVIRONMENTVARIABLEW pfnGetEnvironmentVariableW;
};

extern _AtlStringThunks _strthunks;

inline DWORD WINAPI GetEnvironmentVariableWFake(__in_z LPCWSTR pszName, 
	__out_ecount_part_opt(nSize, return) LPWSTR pszBuffer, __in DWORD nSize)
{
	ULONG nSizeA;
	ULONG nSizeW;
	CTempBuffer<char> pszBufferA;
	CW2A pszNameA(pszName);

	nSizeA = ::GetEnvironmentVariableA(pszNameA, NULL, 0);
	if( nSizeA == 0 || nSizeA==ULONG_MAX)
	{
		return 0;
	}

	pszBufferA.Allocate(nSizeA + 1);
	if( 0 == ::GetEnvironmentVariableA(pszNameA, pszBufferA, nSizeA))
	{
		return 0;
	}

	nSizeW = ::MultiByteToWideChar(_AtlGetConversionACP(), 0, pszBufferA, -1, NULL, 0);
	if( nSize == 0 )
	{
		return nSizeW > DWORD_MAX ? 0 : DWORD(nSizeW);
	}
	
	if( nSize >= nSizeW )
	{
		nSizeW = ::MultiByteToWideChar(_AtlGetConversionACP(), 0, pszBufferA, -1, pszBuffer, nSize );
	}

	return min(nSize, nSizeW);
}

inline DWORD WINAPI GetEnvironmentVariableWThunk(__in_z LPCWSTR pszName, 
	__out_ecount_part_opt(nSize, return) LPWSTR pszBuffer, __in DWORD nSize)
{
	_AtlInstallStringThunk(reinterpret_cast<void**>(&_strthunks.pfnGetEnvironmentVariableW), 
		GetEnvironmentVariableWFake, ::GetEnvironmentVariableW);

	return _strthunks.pfnGetEnvironmentVariableW(pszName, pszBuffer, nSize);
}

inline int WINAPI CompareStringWFake(__in LCID lcid, __in DWORD dwFlags, 
	__in_ecount(nLength1) LPCWSTR pszString1, __in int nLength1, __in_ecount(nLength2) LPCWSTR pszString2, __in int nLength2)
{
	USES_CONVERSION_EX;
	
	LPCSTR pszAString1 = NULL;
	if(pszString1 != NULL)
	{
		pszAString1 = W2A_EX(pszString1,_ATL_SAFE_ALLOCA_DEF_THRESHOLD);
		if(pszAString1 == NULL)
			AtlThrow(E_OUTOFMEMORY);
	}

	LPCSTR pszAString2 = NULL;
	if(pszString2 != NULL)
	{
		pszAString2 = W2A_EX(pszString2,_ATL_SAFE_ALLOCA_DEF_THRESHOLD);
		if(pszAString2 == NULL)
			AtlThrow(E_OUTOFMEMORY);
	}

	return ::CompareStringA(lcid, dwFlags, pszAString1, nLength1, pszAString2, nLength2);
}

inline int WINAPI CompareStringWThunk(__in LCID lcid, __in DWORD dwFlags, 
	__in_ecount(nLength1) LPCWSTR pszString1, __in int nLength1, __in_ecount(nLength2) LPCWSTR pszString2, __in int nLength2)
{
	_AtlInstallStringThunk(reinterpret_cast<void**>(&_strthunks.pfnCompareStringW), CompareStringWFake, ::CompareStringW);

	return _strthunks.pfnCompareStringW(lcid, dwFlags, pszString1, nLength1, pszString2, nLength2);
}

inline BOOL WINAPI GetStringTypeExWFake(__in LCID lcid, __in DWORD dwInfoType, __in_xcount(nLength) LPCWSTR pszSrc,
	__in int nLength, __out_xcount(nLength) LPWORD pwCharType)
{
	int nLengthA;
	CTempBuffer<char> pszA;

	nLengthA = ::WideCharToMultiByte(_AtlGetConversionACP(), 0, pszSrc, nLength, NULL, 0, NULL, NULL);
	pszA.Allocate(nLengthA);
	::WideCharToMultiByte(_AtlGetConversionACP(), 0, pszSrc, nLength, pszA, nLengthA, NULL, NULL);

	if (nLength == -1 || nLengthA > nLength)
		nLengthA = nLength;

	return ::GetStringTypeExA(lcid, dwInfoType, pszA, nLengthA, pwCharType);
}

inline BOOL WINAPI GetStringTypeExWThunk(__in LCID lcid, __in DWORD dwInfoType, __in_ecount(nLength) LPCWSTR pszSrc,
	__in int nLength, __out LPWORD pwCharType)
{
	_AtlInstallStringThunk(reinterpret_cast<void**>(&_strthunks.pfnGetStringTypeExW), GetStringTypeExWFake, ::GetStringTypeExW);

	return _strthunks.pfnGetStringTypeExW(lcid, dwInfoType, pszSrc, nLength, pwCharType);
}

inline int WINAPI lstrcmpiWFake(__in_z LPCWSTR psz1, __in_z LPCWSTR psz2)
{
	USES_CONVERSION_EX;

	return ::lstrcmpiA(W2A_EX_DEF(psz1), W2A_EX_DEF(psz2));
}

inline int WINAPI lstrcmpiWThunk(__in_z LPCWSTR psz1, __in_z LPCWSTR psz2)
{
	_AtlInstallStringThunk(reinterpret_cast<void**>(&_strthunks.pfnlstrcmpiW), lstrcmpiWFake, ::lstrcmpiW);

	return _strthunks.pfnlstrcmpiW(psz1, psz2);
}

inline LPWSTR WINAPI CharLowerWFake(__inout_z LPWSTR psz)
{
	ATLENSURE(psz);
	USES_CONVERSION_EX;
	LPSTR pszA;

	pszA = W2A_EX_DEF(psz);
	::CharLowerA(pszA);

	Checked::wcscpy_s(psz, wcslen(psz) + 1, A2W_EX_DEF(pszA));
	return psz;
}

inline LPWSTR WINAPI CharLowerWThunk(__inout LPWSTR psz)
{
	_AtlInstallStringThunk(reinterpret_cast<void**>(&_strthunks.pfnCharLowerW), CharLowerWFake, ::CharLowerW);

	return _strthunks.pfnCharLowerW(psz);
}

inline LPWSTR WINAPI CharUpperWFake(__inout_z LPWSTR psz)
{
	ATLENSURE(psz);
	USES_CONVERSION_EX;
	LPSTR pszA;

	pszA = W2A_EX_DEF(psz);
	::CharUpperA(pszA);

	Checked::wcscpy_s(psz, wcslen(psz) + 1, A2W_EX_DEF(pszA));
	return psz;
}

inline LPWSTR WINAPI CharUpperWThunk(__inout LPWSTR psz)
{
	_AtlInstallStringThunk(reinterpret_cast<void**>(&_strthunks.pfnCharUpperW), CharUpperWFake, ::CharUpperW);

	return _strthunks.pfnCharUpperW(psz);
}

__declspec(selectany) _AtlStringThunks _strthunks =
{
	CompareStringWThunk,
	GetStringTypeExWThunk,
	lstrcmpiWThunk,
	CharLowerWThunk,
	CharUpperWThunk,
	GetEnvironmentVariableWThunk
};

#endif  // !_UNICODE

/////////////////////////////////////////////////////////////////////////////
//

#ifndef _ATL_MIN_CRT
template< typename _CharType = char >
class ChTraitsCRT :
	public ChTraitsBase< _CharType >
{
public:
	static char* __cdecl CharNext( __in_z const char* p ) throw()
	{
		return reinterpret_cast< char* >( _mbsinc( reinterpret_cast< const unsigned char* >( p ) ) );
	}

	static int __cdecl IsDigit( __in char ch ) throw()
	{
		return _ismbcdigit( ch );
	}

	static int __cdecl IsSpace( __in char ch ) throw()
	{
		return _ismbcspace( ch );
	}

	static int __cdecl StringCompare( __in_z LPCSTR pszA, __in_z LPCSTR pszB ) throw()
	{
		return _mbscmp( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static int __cdecl StringCompareIgnore( __in_z LPCSTR pszA, __in_z LPCSTR pszB ) throw()
	{
		return _mbsicmp( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static int __cdecl StringCollate( __in_z LPCSTR pszA, __in_z LPCSTR pszB ) throw()
	{
		return _mbscoll( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static int __cdecl StringCollateIgnore( __in_z LPCSTR pszA, __in_z LPCSTR pszB ) throw()
	{
		return _mbsicoll( reinterpret_cast< const unsigned char* >( pszA ), reinterpret_cast< const unsigned char* >( pszB ) );
	}

	static LPCSTR __cdecl StringFindString( __in_z LPCSTR pszBlock, __in_z LPCSTR pszMatch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbsstr( reinterpret_cast< const unsigned char* >( pszBlock ),
			reinterpret_cast< const unsigned char* >( pszMatch ) ) );
	}

	static LPSTR __cdecl StringFindString( __in_z LPSTR pszBlock, __in_z LPCSTR pszMatch ) throw()
	{
		return( const_cast< LPSTR >( StringFindString( const_cast< LPCSTR >( pszBlock ), pszMatch ) ) );
	}

	static LPCSTR __cdecl StringFindChar( __in_z LPCSTR pszBlock, __in char chMatch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbschr( reinterpret_cast< const unsigned char* >( pszBlock ), (unsigned char)chMatch ) );
	}

	static LPCSTR __cdecl StringFindCharRev( __in_z LPCSTR psz, __in char ch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbsrchr( reinterpret_cast< const unsigned char* >( psz ), (unsigned char)ch ) );
	}

	static LPCSTR __cdecl StringScanSet( __in_z LPCSTR pszBlock, __in_z LPCSTR pszMatch ) throw()
	{
		return reinterpret_cast< LPCSTR >( _mbspbrk( reinterpret_cast< const unsigned char* >( pszBlock ),
			reinterpret_cast< const unsigned char* >( pszMatch ) ) );
	}

	static int __cdecl StringSpanIncluding( __in_z LPCSTR pszBlock, __in_z LPCSTR pszSet ) throw()
	{
		return (int)_mbsspn( reinterpret_cast< const unsigned char* >( pszBlock ), reinterpret_cast< const unsigned char* >( pszSet ) );
	}

	static int __cdecl StringSpanExcluding( __in_z LPCSTR pszBlock, __in_z LPCSTR pszSet ) throw()
	{
		return (int)_mbscspn( reinterpret_cast< const unsigned char* >( pszBlock ), reinterpret_cast< const unsigned char* >( pszSet ) );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringUppercase")
	static LPSTR __cdecl StringUppercase( __inout LPSTR psz ) throw()
	{
		return reinterpret_cast< LPSTR >( _mbsupr( reinterpret_cast< unsigned char* >( psz ) ) );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringLowercase")
	static LPSTR __cdecl StringLowercase( __inout LPSTR psz ) throw()
	{
		return reinterpret_cast< LPSTR >( _mbslwr( reinterpret_cast< unsigned char* >( psz ) ) );
	}

	static LPSTR __cdecl StringUppercase( __inout_ecount(size) LPSTR psz, __in size_t size ) throw()
	{
		Checked::mbsupr_s(reinterpret_cast< unsigned char* >( psz ), size);
		return psz;
	}

	static LPSTR __cdecl StringLowercase( __inout_ecount(size) LPSTR psz, __in size_t size ) throw()
	{
		Checked::mbslwr_s( reinterpret_cast< unsigned char* >( psz ), size );
		return psz;
	}

	static LPSTR __cdecl StringReverse( __inout LPSTR psz ) throw()
	{
		return reinterpret_cast< LPSTR >( _mbsrev( reinterpret_cast< unsigned char* >( psz ) ) );
	}

	static int __cdecl GetFormattedLength( __in __format_string LPCSTR pszFormat, va_list args ) throw()
	{
		return _vscprintf( pszFormat, args );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::Format")
	static int __cdecl Format( __out LPSTR pszBuffer, __in __format_string LPCSTR pszFormat, va_list args ) throw()
	{
		return vsprintf( pszBuffer, pszFormat, args );
	}

    static int __cdecl Format
		( __out_ecount_part(nlength, return) LPSTR pszBuffer, __in size_t nlength, __in __format_string LPCSTR pszFormat, va_list args ) throw()
	{
		return vsprintf_s( pszBuffer, nlength, pszFormat, args );
	}

	static int __cdecl GetBaseTypeLength( __in_z LPCSTR pszSrc ) throw()
	{
		// Returns required buffer length in XCHARs
		return int( strlen( pszSrc ) );
	}

	static int __cdecl GetBaseTypeLength( __in_ecount(nLength) LPCSTR pszSrc, int nLength ) throw()
	{
		(void)pszSrc;
		// Returns required buffer length in XCHARs
		return nLength;
	}

	static int __cdecl GetBaseTypeLength( __in_z LPCWSTR pszSource ) throw()
	{
		// Returns required buffer length in XCHARs
		return ::WideCharToMultiByte( _AtlGetConversionACP(), 0, pszSource, -1, NULL, 0, NULL, NULL )-1;
	}

	static int __cdecl GetBaseTypeLength( __in_ecount(nLength) LPCWSTR pszSource, int nLength ) throw()
	{
		// Returns required buffer length in XCHARs
		return ::WideCharToMultiByte( _AtlGetConversionACP(), 0, pszSource, nLength, NULL, 0, NULL, NULL );
	}

	static void __cdecl ConvertToBaseType( __out_ecount(nDestLength) LPSTR pszDest, __in int nDestLength,
		__in_z LPCSTR pszSrc, int nSrcLength = -1 ) throw()
	{
		if (nSrcLength == -1) { nSrcLength=1 + GetBaseTypeLength(pszSrc); }
		// nLen is in XCHARs
		Checked::memcpy_s( pszDest, nDestLength*sizeof( char ), 
			pszSrc, nSrcLength*sizeof( char ) );
	}

	static void __cdecl ConvertToBaseType( __out_ecount(nDestLength) LPSTR pszDest, __in int nDestLength,
		__in_z LPCWSTR pszSrc, __in int nSrcLength = -1) throw()
	{
		// nLen is in XCHARs
		::WideCharToMultiByte( _AtlGetConversionACP(), 0, pszSrc, nSrcLength, pszDest, nDestLength, NULL, NULL );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::ConvertToOem")
	static void ConvertToOem(__inout _CharType* pstrString) throw()
	{
		BOOL fSuccess=::CharToOemA(pstrString, pstrString);
		// old version can't report error
		ATLASSERT(fSuccess);
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::ConvertToAnsi")
	static void ConvertToAnsi(__inout _CharType* pstrString) throw()
	{
		BOOL fSuccess=::OemToCharA(pstrString, pstrString);
		// old version can't report error
		ATLASSERT(fSuccess);
	}

	static void ConvertToOem(__inout_ecount(size) _CharType* pstrString, __in size_t size)
	{
		if(size>UINT_MAX)
		{
			// API only allows DWORD size
			AtlThrow(E_INVALIDARG);
		}
		DWORD dwSize=static_cast<DWORD>(size);
		BOOL fSuccess=::CharToOemBuffA(pstrString, pstrString, dwSize);
		if(!fSuccess)
		{
			AtlThrowLastWin32();
		}
	}

	static void ConvertToAnsi(__inout_ecount(size) _CharType* pstrString, __in size_t size)
	{
		if(size>UINT_MAX)
		{
			// API only allows DWORD size
			AtlThrow(E_INVALIDARG);
		}
		DWORD dwSize=static_cast<DWORD>(size);
		BOOL fSuccess=::OemToCharBuffA(pstrString, pstrString, dwSize);
		if(!fSuccess)
		{
			AtlThrowLastWin32();
		}
	}

	static void __cdecl FloodCharacters( __in char ch, __in int nLength, __out_ecount_full(nLength) char* pch ) throw()
	{
		// nLength is in XCHARs
		memset( pch, ch, nLength );
	}

	static BSTR __cdecl AllocSysString( __in_ecount(nDataLength) const char* pchData, int nDataLength ) throw()
	{
		int nLen = ::MultiByteToWideChar( _AtlGetConversionACP(), 0, pchData, nDataLength,
			NULL, NULL );
		BSTR bstr = ::SysAllocStringLen( NULL, nLen );
		if( bstr != NULL )
		{
			::MultiByteToWideChar( _AtlGetConversionACP(), 0, pchData, nDataLength,
				bstr, nLen );
		}

		return bstr;
	}

	static BOOL __cdecl ReAllocSysString( __in_ecount(nDataLength) const char* pchData, __out BSTR* pbstr, __in int nDataLength ) throw()
	{
		int nLen = ::MultiByteToWideChar( _AtlGetConversionACP(), 0, pchData, nDataLength, NULL, NULL );
		BOOL bSuccess = ::SysReAllocStringLen( pbstr, NULL, nLen );
		if( bSuccess )
		{
			::MultiByteToWideChar( _AtlGetConversionACP(), 0, pchData, nDataLength, *pbstr, nLen );
		}

		return bSuccess;
	}

	static DWORD __cdecl _AFX_FUNCNAME(FormatMessage)( __in DWORD dwFlags, LPCVOID pSource,
		__in DWORD dwMessageID, __in DWORD dwLanguageID, __out_ecount(nSize) LPSTR pszBuffer,
		DWORD nSize, va_list* pArguments ) throw()
	{
		return ::FormatMessageA( dwFlags, pSource, dwMessageID, dwLanguageID,
				pszBuffer, nSize, pArguments );
	}

#if defined(_AFX)
	static DWORD __cdecl FormatMessage( __in DWORD dwFlags, LPCVOID pSource,
		__in DWORD dwMessageID, __in DWORD dwLanguageID, __out_ecount(nSize) LPSTR pszBuffer,
		DWORD nSize, va_list* pArguments ) throw()
	{
		return _AFX_FUNCNAME(FormatMessage)(dwFlags, pSource, dwMessageID, dwLanguageID, pszBuffer, nSize, pArguments);
	}
#endif

	static int __cdecl SafeStringLen( __in_z_opt LPCSTR psz ) throw()
	{
		// returns length in bytes
		return (psz != NULL) ? int( strlen( psz ) ) : 0;
	}

	static int __cdecl SafeStringLen( __in_z_opt LPCWSTR psz ) throw()
	{
		// returns length in wchar_ts
		return (psz != NULL) ? int( wcslen( psz ) ) : 0;
	}

	static int __cdecl GetCharLen( __in_z const wchar_t* pch ) throw()
	{
		(void)pch;
		// returns char length
		return 1;
	}

	static int __cdecl GetCharLen( __in_z const char* pch ) throw()
	{
		// returns char length
		return int( _mbclen( reinterpret_cast< const unsigned char* >( pch ) ) );
	}

	static DWORD __cdecl GetEnvironmentVariable( __in_z LPCSTR pszVar,
		__out_ecount_opt(dwSize) LPSTR pszBuffer, __in DWORD dwSize ) throw()
	{
		return ::GetEnvironmentVariableA( pszVar, pszBuffer, dwSize );
	}
};

// specialization for wchar_t
template<>
class ChTraitsCRT< wchar_t > :
	public ChTraitsBase< wchar_t >
{
#if defined(_UNICODE) && !defined(_CSTRING_ALWAYS_THUNK)
	static DWORD __cdecl _GetEnvironmentVariableW( __in_z LPCWSTR pszName, __out_ecount_part_opt(nSize, return) LPWSTR pszBuffer, __in DWORD nSize ) throw()
	{
		return ::GetEnvironmentVariableW( pszName, pszBuffer, nSize );
	}
#else  // !_UNICODE
	static DWORD WINAPI _GetEnvironmentVariableW( __in_z LPCWSTR pszName, 
		__out_ecount_part_opt(nSize,return) LPWSTR pszBuffer, __in DWORD nSize ) throw()
	{
		return _strthunks.pfnGetEnvironmentVariableW( pszName, pszBuffer, nSize );
	}
#endif  // !_UNICODE

public:
	static LPWSTR __cdecl CharNext( __in_z LPCWSTR psz ) throw()
	{
		return const_cast< LPWSTR >( psz+1 );
	}

	static int __cdecl IsDigit( __in wchar_t ch ) throw()
	{
		return iswdigit( static_cast<unsigned short>(ch) );
	}

	static int __cdecl IsSpace( __in wchar_t ch ) throw()
	{
		return iswspace( static_cast<unsigned short>(ch) );
	}

	static int __cdecl StringCompare( __in_z LPCWSTR pszA, __in_z LPCWSTR pszB ) throw()
	{
		return wcscmp( pszA, pszB );
	}

	static int __cdecl StringCompareIgnore( __in_z LPCWSTR pszA, __in_z LPCWSTR pszB ) throw()
	{
		return _wcsicmp( pszA, pszB );
	}

	static int __cdecl StringCollate( __in_z LPCWSTR pszA, __in_z LPCWSTR pszB ) throw()
	{
		return wcscoll( pszA, pszB );
	}

	static int __cdecl StringCollateIgnore( __in_z LPCWSTR pszA, __in_z LPCWSTR pszB ) throw()
	{
		return _wcsicoll( pszA, pszB );
	}

	static LPCWSTR __cdecl StringFindString( __in_z LPCWSTR pszBlock, __in_z LPCWSTR pszMatch ) throw()
	{
		return wcsstr( pszBlock, pszMatch );
	}

	static LPWSTR __cdecl StringFindString( __in_z LPWSTR pszBlock, __in_z LPCWSTR pszMatch ) throw()
	{
		return( const_cast< LPWSTR >( StringFindString( const_cast< LPCWSTR >( pszBlock ), pszMatch ) ) );
	}

	static LPCWSTR __cdecl StringFindChar( __in_z LPCWSTR pszBlock, __in wchar_t chMatch ) throw()
	{
		return wcschr( pszBlock, chMatch );
	}

	static LPCWSTR __cdecl StringFindCharRev( __in_z LPCWSTR psz, __in wchar_t ch ) throw()
	{
		return wcsrchr( psz, ch );
	}

	static LPCWSTR __cdecl StringScanSet( __in_z LPCWSTR pszBlock, __in_z LPCWSTR pszMatch ) throw()
	{
		return wcspbrk( pszBlock, pszMatch );
	}

	static int __cdecl StringSpanIncluding( __in_z LPCWSTR pszBlock, __in_z LPCWSTR pszSet ) throw()
	{
		return (int)wcsspn( pszBlock, pszSet );
	}

	static int __cdecl StringSpanExcluding( __in_z LPCWSTR pszBlock, __in_z LPCWSTR pszSet ) throw()
	{
		return (int)wcscspn( pszBlock, pszSet );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringUppercase")
	static LPWSTR __cdecl StringUppercase( __inout LPWSTR psz ) throw()
	{
		return _wcsupr( psz );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::StringLowercase")
	static LPWSTR __cdecl StringLowercase( __inout LPWSTR psz ) throw()
	{
		return _wcslwr( psz );
	}

	static LPWSTR __cdecl StringUppercase( __inout_ecount(size) LPWSTR psz, __in size_t size ) throw()
	{
#if _SECURE_ATL
		errno_t err = _wcsupr_s( psz, size );
		return (err == 0) ? psz : NULL;
#else
		size;
		return _wcsupr( psz );
#endif
	}

	static LPWSTR __cdecl StringLowercase( __inout_ecount(size) LPWSTR psz, __in size_t size ) throw()
	{
#if _SECURE_ATL
		errno_t err = _wcslwr_s( psz, size );
		return (err == 0) ? psz : NULL;
#else
		size;
		return _wcslwr( psz );
#endif
	}

	static LPWSTR __cdecl StringReverse( __inout LPWSTR psz ) throw()
	{
		return _wcsrev( psz );
	}

	static int __cdecl GetFormattedLength( __in __format_string LPCWSTR pszFormat, va_list args) throw()
	{
		return _vscwprintf( pszFormat, args );
	}

	_ATL_INSECURE_DEPRECATE("You must pass an output size to ChTraitsCRT::Format")
	static int __cdecl Format( __out LPWSTR pszBuffer, __in __format_string LPCWSTR pszFormat, va_list args) throw()
	{
		#pragma prefast(push)
		#pragma prefast(disable:__WARNING_BANNED_API_USAGE, "Deprecated function replaced by buffer-length version")
		return vswprintf( pszBuffer, pszFormat, args );
		#pragma prefast (pop)
	}
	static int __cdecl Format
		( __out_ecount(nLength) LPWSTR pszBuffer, __in size_t nLength, __in __format_string LPCWSTR pszFormat, va_list args) throw()
	{
		return vswprintf_s( pszBuffer, nLength, pszFormat, args );
	}

	static int __cdecl GetBaseTypeLength( __in_z LPCSTR pszSrc ) throw()
	{
		// Returns required buffer size in wchar_ts
		return ::MultiByteToWideChar( _AtlGetConversionACP(), 0, pszSrc, -1, NULL, 0 )-1;
	}

	static int __cdecl GetBaseTypeLength( __in_ecount(nLength) LPCSTR pszSrc, __in int nLength ) throw()
	{
		// Returns required buffer size in wchar_ts
		return ::MultiByteToWideChar( _AtlGetConversionACP(), 0, pszSrc, nLength, NULL, 0 );
	}

	static int __cdecl GetBaseTypeLength( __in_z LPCWSTR pszSrc ) throw()
	{
		// Returns required buffer size in wchar_ts
		return (int)wcslen( pszSrc );
	}

	static int __cdecl GetBaseTypeLength( __in_ecount(nLength) LPCWSTR pszSrc, __in int nLength ) throw()
	{
		(void)pszSrc;
		// Returns required buffer size in wchar_ts
		return nLength;
	}

	static void __cdecl ConvertToBaseType( __out_ecount(nDestLength) LPWSTR pszDest, __in int nDestLength,
		__in_z LPCSTR pszSrc, __in int nSrcLength = -1) throw()
	{
		// nLen is in wchar_ts
		::MultiByteToWideChar( _AtlGetConversionACP(), 0, pszSrc, nSrcLength, pszDest, nDestLength );
	}

	static void __cdecl ConvertToBaseType( __out_ecount(nDestLength) LPWSTR pszDest, __in int nDestLength,
		__in_z LPCWSTR pszSrc, int nSrcLength = -1 ) throw()
	{		
		if (nSrcLength == -1) { nSrcLength=1 + GetBaseTypeLength(pszSrc); }
		// nLen is in wchar_ts
		Checked::wmemcpy_s(pszDest, nDestLength, pszSrc, nSrcLength);
	}

	static void __cdecl FloodCharacters( __in wchar_t ch, __in int nLength, __out_ecount_full(nLength) LPWSTR psz ) throw()
	{
		// nLength is in XCHARs
		for( int i = 0; i < nLength; i++ )
		{
			psz[i] = ch;
		}
	}

	static BSTR __cdecl AllocSysString( __in_ecount(nDataLength) const wchar_t* pchData, __in int nDataLength ) throw()
	{
		return ::SysAllocStringLen( pchData, nDataLength );
	}

	static BOOL __cdecl ReAllocSysString( __in_ecount(nDataLength) const wchar_t* pchData, __inout BSTR* pbstr, __in int nDataLength ) throw()
	{
		return ::SysReAllocStringLen( pbstr, pchData, nDataLength );
	}

	static int __cdecl SafeStringLen( __in_z_opt LPCSTR psz ) throw()
	{
		// returns length in bytes
		return (psz != NULL) ? (int)strlen( psz ) : 0;
	}

	static int __cdecl SafeStringLen( __in_opt LPCWSTR psz ) throw()
	{
		// returns length in wchar_ts
		return (psz != NULL) ? (int)wcslen( psz ) : 0;
	}

	static int __cdecl GetCharLen( __in_z const wchar_t* pch ) throw()
	{
		(void)pch;
		// returns char length
		return 1;
	}

	static int __cdecl GetCharLen( __in_z const char* pch ) throw()
	{
		// returns char length
		return (int)( _mbclen( reinterpret_cast< const unsigned char* >( pch ) ) );
	}

	static DWORD __cdecl GetEnvironmentVariable( __in_z LPCWSTR pszVar, __out_ecount_opt(dwSize) LPWSTR pszBuffer, __in DWORD dwSize ) throw()
	{
		return _GetEnvironmentVariableW( pszVar, pszBuffer, dwSize );
	}

	static void __cdecl ConvertToOem( __reserved LPWSTR /*psz*/ )
	{
//		ATLENSURE(FALSE); // Unsupported Feature 
	}

	static void __cdecl ConvertToAnsi( __reserved LPWSTR /*psz*/ )
	{
//		ATLENSURE(FALSE); // Unsupported Feature 
	}

	static void __cdecl ConvertToOem( __reserved LPWSTR /*psz*/, size_t )
	{
		ATLENSURE(FALSE); // Unsupported Feature 
	}

	static void __cdecl ConvertToAnsi( __reserved LPWSTR /*psz*/, size_t ) 
	{
		ATLENSURE(FALSE); // Unsupported Feature 
	}

#ifdef _UNICODE
public:
	static DWORD __cdecl _AFX_FUNCNAME(FormatMessage)( __in DWORD dwFlags, LPCVOID pSource,
		__in DWORD dwMessageID, __in DWORD dwLanguageID, __out_ecount(nSize) LPWSTR pszBuffer,
		__in DWORD nSize, va_list* pArguments ) throw()
	{
		return ::FormatMessageW( dwFlags, pSource, dwMessageID, dwLanguageID,
				pszBuffer, nSize, pArguments );
	}

#if defined(_AFX)
	static DWORD __cdecl FormatMessage( __in DWORD dwFlags, LPCVOID pSource,
		__in DWORD dwMessageID, __in DWORD dwLanguageID, __out_ecount(nSize) LPWSTR pszBuffer,
		__in DWORD nSize, va_list* pArguments ) throw()
	{
		return _AFX_FUNCNAME(FormatMessage)(dwFlags, pSource, dwMessageID, dwLanguageID, pszBuffer, nSize, pArguments);
	}
#endif

#else
	static DWORD __cdecl _AFX_FUNCNAME(FormatMessage)( DWORD /*dwFlags*/, LPCVOID /*pSource*/,
		DWORD /*dwMessageID*/, DWORD /*dwLanguageID*/, __reserved LPWSTR /*pszBuffer*/,
		DWORD /*nSize*/, va_list* /*pArguments*/ )
	{
//		ATLENSURE(FALSE); // Unsupported Feature 
		return 0;
	}

#if defined(_AFX)
	static DWORD __cdecl FormatMessage( DWORD dwFlags, LPCVOID pSource,
		DWORD dwMessageID, DWORD dwLanguageID, __reserved LPWSTR pszBuffer,
		DWORD nSize, va_list* pArguments )
	{
		return _AFX_FUNCNAME(FormatMessage)(dwFlags, pSource, dwMessageID, dwLanguageID, pszBuffer, nSize, pArguments);
	}
#endif

#endif

};
#endif  // _ATL_MIN_CRT

}	// namespace ATL

#ifndef _ATL_MIN_CRT

// Forward declare
template< typename _CharType = char, class StringIterator = ATL::ChTraitsCRT< _CharType > >
class StrTraitMFC_DLL;

#endif	// _ATL_MIN_CRT

namespace ATL
{

namespace _CSTRING_IMPL_
{
template <typename _CharType, class StringTraits>
struct _MFCDLLTraitsCheck
{
	const static bool c_bIsMFCDLLTraits = false;
};

#ifndef _ATL_MIN_CRT

template<typename _CharType> 
struct _MFCDLLTraitsCheck<_CharType, StrTraitMFC_DLL<_CharType, ATL::ChTraitsCRT< _CharType > > >
{
	const static bool c_bIsMFCDLLTraits = true;
};

#endif	// _ATL_MIN_CRT
}

// The CStringT class has a few varargs member functions that will cause 4793
// warnings if compiled /clr. Because of the way template classes are parsed,
// we need to disable the warning for the entire class.
//#pragma warning( push )
//#pragma warning( disable : 4793 )


template< typename BaseType, class StringTraits >
class CStringT :
	public CSimpleStringT< BaseType, _CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits >
{
public:
	typedef CSimpleStringT< BaseType, _CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits > CThisSimpleString;
	typedef StringTraits StrTraits;
	typedef typename CThisSimpleString::XCHAR XCHAR;
	typedef typename CThisSimpleString::PXSTR PXSTR;
	typedef typename CThisSimpleString::PCXSTR PCXSTR;
	typedef typename CThisSimpleString::YCHAR YCHAR;
	typedef typename CThisSimpleString::PYSTR PYSTR;
	typedef typename CThisSimpleString::PCYSTR PCYSTR;

public:
	CStringT() throw() :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
	}
	explicit CStringT( IAtlStringMgr* pStringMgr ) throw() :
		CThisSimpleString( pStringMgr )
	{ 
	}

	CStringT( const VARIANT& varSrc );
	CStringT( const VARIANT& varSrc, IAtlStringMgr* pStringMgr );

	static void __cdecl Construct( CStringT* pString )
	{
		new( pString ) CStringT;
	}

	// Copy constructor
	CStringT( const CStringT& strSrc ) :
		CThisSimpleString( strSrc )
	{
	}

	// Construct from CSimpleStringT
	operator CSimpleStringT<BaseType, !_CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits >&()
	{
		return *(CSimpleStringT<BaseType, !_CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits >*)this;
	}
	template <bool bMFCDLL>
	CStringT( __in const CSimpleStringT<BaseType, bMFCDLL>& strSrc ) :
		CThisSimpleString( strSrc )
	{
	}

	CStringT( __in const XCHAR* pszSrc ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		if( !CheckImplicitLoad( pszSrc ) )
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}
	}

	CStringT( __in_z LPCSTR pszSrc, __in IAtlStringMgr* pStringMgr ) :
		CThisSimpleString( pStringMgr )
	{
		if( !CheckImplicitLoad( pszSrc ) )
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}
	}

	CSTRING_EXPLICIT CStringT( __in const YCHAR* pszSrc ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		if( !CheckImplicitLoad( pszSrc ) )
		{
			*this = pszSrc;
		}
	}

	CStringT( __in_z LPCWSTR pszSrc, __in IAtlStringMgr* pStringMgr ) :
		CThisSimpleString( pStringMgr )
	{
		if( !CheckImplicitLoad( pszSrc ) )
		{
			*this = pszSrc;
		}
	}

#if defined(__cplusplus_cli)

	template <class SystemString>
	CStringT( SystemString^ pString ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		pin_ptr<const System::Char> pChar = PtrToStringChars( pString );
		const wchar_t *psz = pChar;
		*this = psz;
	}

#elif defined(_MANAGED)

	template<class SystemString>
	CStringT( SystemString __gc* pString ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{		
		const wchar_t __pin* psz = PtrToStringChars( pString );
		*this = psz;
	}

#endif

	CSTRING_EXPLICIT CStringT( const unsigned char* pszSrc ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		*this = reinterpret_cast< const char* >( pszSrc );
	}
//ctors to prevent from oldSyntax template ctor (above) hijack certain types.
//MFC dll instantiate all CStringT methods inside the dll and declares dllimport for 
//all methods in user build (see afxstr.h), so need to include the methods in MFC dll builds.
#if defined(_AFXDLL) && defined(_MFC_DLL_BLD) || !defined(__cplusplus_cli) && defined(_MANAGED)

	/*CSTRING_EXPLICIT*/ CStringT( __in_z char* pszSrc ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		const char *psz = reinterpret_cast< const char* >( pszSrc );
		if (!CheckImplicitLoad( psz ))
		{
			*this = psz;
		}
	}

	CSTRING_EXPLICIT CStringT( __in unsigned char* pszSrc ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		const char *psz = reinterpret_cast< const char* >( pszSrc );
		if (!CheckImplicitLoad( psz ))
		{
			*this = psz;
		}
	}

	CSTRING_EXPLICIT CStringT( __in_z wchar_t* pszSrc ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		const wchar_t *psz = reinterpret_cast< const wchar_t* >( pszSrc );
		if (!CheckImplicitLoad( psz ))
		{
			*this = psz;
		}
	}
#endif

	CStringT( __in const unsigned char* pszSrc, __in IAtlStringMgr* pStringMgr ) :
		CThisSimpleString( pStringMgr )
	{
		*this = reinterpret_cast< const char* >( pszSrc );
	}

	CSTRING_EXPLICIT CStringT( __in char ch, __in int nLength = 1 ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{
			PXSTR pszBuffer = GetBuffer( nLength );
			StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
			ReleaseBufferSetLength( nLength );
		}
	}

	CSTRING_EXPLICIT CStringT( __in wchar_t ch, __in int nLength = 1 ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{			
			//Convert ch to the BaseType
			wchar_t pszCh[2] = { ch , 0 };
			int nBaseTypeCharLen = 1;

			if(ch != L'\0')
			{
				nBaseTypeCharLen = StringTraits::GetBaseTypeLength(pszCh);
			}

			CTempBuffer<XCHAR,10> buffBaseTypeChar;			
			buffBaseTypeChar.Allocate(nBaseTypeCharLen+1);
			StringTraits::ConvertToBaseType( buffBaseTypeChar, nBaseTypeCharLen+1, pszCh, 1 );
			//Allocate enough characters in String and flood (replicate) with the (converted character)*nLength
			PXSTR pszBuffer = GetBuffer( nLength*nBaseTypeCharLen );
			if (nBaseTypeCharLen == 1)
			{   //Optimization for a common case - wide char translates to 1 ansi/wide char.
				StringTraits::FloodCharacters( buffBaseTypeChar[0], nLength, pszBuffer );				
			} else
			{
				XCHAR* p=pszBuffer;
				for (int i=0 ; i < nLength ;++i)
				{
					for (int j=0 ; j < nBaseTypeCharLen ;++j)
					{	
						*p=buffBaseTypeChar[j];
						++p;
					}
				}
			}
			ReleaseBufferSetLength( nLength*nBaseTypeCharLen );			
		}
	}

	CStringT( __in_ecount(nLength) const XCHAR* pch, __in int nLength ) :
		CThisSimpleString( pch, nLength, StringTraits::GetDefaultManager() )
	{
	}

	CStringT( __in_ecount(nLength) const XCHAR* pch, __in int nLength, __in IAtlStringMgr* pStringMgr ) :
		CThisSimpleString( pch, nLength, pStringMgr )
	{
	}

	CStringT( __in_ecount(nLength) const YCHAR* pch, __in int nLength ) :
		CThisSimpleString( StringTraits::GetDefaultManager() )
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{
			ATLASSERT( AtlIsValidAddress( pch, nLength*sizeof( YCHAR ), FALSE ) );
			if(pch == NULL)
				AtlThrow(E_INVALIDARG);

			int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
			PXSTR pszBuffer = GetBuffer( nDestLength );
			StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
			ReleaseBufferSetLength( nDestLength );
		}
	}

	CStringT( __in_ecount(nLength) const YCHAR* pch, __in int nLength, __in IAtlStringMgr* pStringMgr ) :
		CThisSimpleString( pStringMgr )
	{
		ATLASSERT( nLength >= 0 );
		if( nLength > 0 )
		{
			ATLASSERT( AtlIsValidAddress( pch, nLength*sizeof( YCHAR ), FALSE ) );
			if(pch == NULL)
				AtlThrow(E_INVALIDARG);

			int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
			PXSTR pszBuffer = GetBuffer( nDestLength );
			StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
			ReleaseBufferSetLength( nDestLength );
		}
	}

	// Destructor
	~CStringT() throw()
	{
	}

	// Assignment operators
	CStringT& operator=( __in const CStringT& strSrc )
	{
		CThisSimpleString::operator=( strSrc );

		return( *this );
	}
	
	template <bool bMFCDLL>
	CStringT& operator=( __in const CSimpleStringT<BaseType, bMFCDLL>& strSrc )
	{
		CThisSimpleString::operator=( strSrc );

		return( *this );
	}

	CStringT& operator=( __in PCXSTR pszSrc )
	{
		CThisSimpleString::operator=( pszSrc );

		return( *this );
	}

	CStringT& operator=( __in PCYSTR pszSrc )
	{
		// nDestLength is in XCHARs
		int nDestLength = (pszSrc != NULL) ? StringTraits::GetBaseTypeLength( pszSrc ) : 0;
		if( nDestLength > 0 )
		{
			PXSTR pszBuffer = GetBuffer( nDestLength );
			StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pszSrc);
			ReleaseBufferSetLength( nDestLength );
		}
		else
		{
			Empty();
		}

		return( *this );
	}

	CStringT& operator=( __in const unsigned char* pszSrc )
	{
		return( operator=( reinterpret_cast< const char* >( pszSrc ) ) );
	}

	CStringT& operator=( __in char ch )
	{
		char ach[2] = { ch, 0 };

		return( operator=( ach ) );
	}

	CStringT& operator=( __in wchar_t ch )
	{
		wchar_t ach[2] = { ch, 0 };

		return( operator=( ach ) );
	}

	CStringT& operator=( __in const VARIANT& var );

	CStringT& operator+=( __in const CThisSimpleString& str )
	{
		CThisSimpleString::operator+=( str );

		return( *this );
	}
	template <bool bMFCDLL>
	CStringT& operator+=( __in const CSimpleStringT<BaseType, bMFCDLL>& str )
	{
		CThisSimpleString::operator+=( str );

		return( *this );
	}
	
	CStringT& operator+=( __in PCXSTR pszSrc )
	{
		CThisSimpleString::operator+=( pszSrc );

		return( *this );
	}
	template< int t_nSize >
	CStringT& operator+=( __in const CStaticString< XCHAR, t_nSize >& strSrc )
	{
		CThisSimpleString::operator+=( strSrc );

		return( *this );
	}
	CStringT& operator+=( __in PCYSTR pszSrc )
	{
		CStringT str( pszSrc, GetManager() );

		return( operator+=( str ) );
	}

	CStringT& operator+=( __in char ch )
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

	CStringT& operator+=( __in unsigned char ch )
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

	CStringT& operator+=( __in wchar_t ch )
	{
		CThisSimpleString::operator+=( ch );

		return( *this );
	}

	CStringT& operator+=( __in const VARIANT& var );

	// Override from base class
	IAtlStringMgr* GetManager() const throw()
	{
		IAtlStringMgr* pStringMgr = CThisSimpleString::GetManager();
		if(pStringMgr) { return pStringMgr; }

		pStringMgr = StringTraits::GetDefaultManager();
		return pStringMgr->Clone();
	}

	// Comparison

	int Compare( __in PCXSTR psz ) const
	{
		ATLENSURE( AtlIsValidString( psz ) );
		return( StringTraits::StringCompare( GetString(), psz ) );
	}

	int CompareNoCase( __in PCXSTR psz ) const throw()
	{
		ATLASSERT( AtlIsValidString( psz ) );
		return( StringTraits::StringCompareIgnore( GetString(), psz ) );
	}

	int Collate( __in PCXSTR psz ) const throw()
	{
		ATLASSERT( AtlIsValidString( psz ) );
		return( StringTraits::StringCollate( GetString(), psz ) );
	}

	int CollateNoCase( __in PCXSTR psz ) const throw()
	{
		ATLASSERT( AtlIsValidString( psz ) );
		return( StringTraits::StringCollateIgnore( GetString(), psz ) );
	}

	// Advanced manipulation

	// Delete 'nCount' characters, starting at index 'iIndex'
	int Delete( __in int iIndex, __in int nCount = 1 )
	{
		if( iIndex < 0 )
			iIndex = 0;
		
		if( nCount < 0 )
			nCount = 0;

		int nLength = GetLength();
		if( (::ATL::AtlAddThrow(nCount, iIndex)) > nLength )
		{
			nCount = nLength-iIndex;
		}
		if( nCount > 0 )
		{
			int nNewLength = nLength-nCount;
			int nXCHARsToCopy = nLength-(iIndex+nCount)+1;
			PXSTR pszBuffer = GetBuffer();
			Checked::memmove_s( pszBuffer+iIndex, nXCHARsToCopy*sizeof( XCHAR ), 
				pszBuffer+iIndex+nCount, nXCHARsToCopy*sizeof( XCHAR ) );
			ReleaseBufferSetLength( nNewLength );
		}

		return( GetLength() );
	}

	// Insert character 'ch' before index 'iIndex'
	int Insert( __in int iIndex, __in XCHAR ch )
	{
		if( iIndex < 0 )
			iIndex = 0;
			
		if( iIndex > GetLength() )
		{
			iIndex = GetLength();
		}
		int nNewLength = GetLength()+1;

		PXSTR pszBuffer = GetBuffer( nNewLength );

		// move existing bytes down 
        Checked::memmove_s( pszBuffer+iIndex+1, (nNewLength-iIndex)*sizeof( XCHAR ), 
			pszBuffer+iIndex, (nNewLength-iIndex)*sizeof( XCHAR ) );
		pszBuffer[iIndex] = ch;

		ReleaseBufferSetLength( nNewLength );

		return( nNewLength );
	}

	// Insert string 'psz' before index 'iIndex'
	int Insert( __in int iIndex, __in PCXSTR psz )
	{
		if( iIndex < 0 )
			iIndex = 0;

		if( iIndex > GetLength() )
		{
			iIndex = GetLength();
		}

		// nInsertLength and nNewLength are in XCHARs
		int nInsertLength = StringTraits::SafeStringLen( psz );
		int nNewLength = GetLength();
		if( nInsertLength > 0 )
		{
			nNewLength += nInsertLength;

			PXSTR pszBuffer = GetBuffer( nNewLength );
			// move existing bytes down 
            Checked::memmove_s( pszBuffer+iIndex+nInsertLength, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ), 
				pszBuffer+iIndex, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ) );
			Checked::memcpy_s( pszBuffer+iIndex, nInsertLength*sizeof( XCHAR ), 
				psz, nInsertLength*sizeof( XCHAR ) );
			ReleaseBufferSetLength( nNewLength );
		}

		return( nNewLength );
	}

	// Replace all occurrences of character 'chOld' with character 'chNew'
	int Replace( __in XCHAR chOld, __in XCHAR chNew )
	{
		int nCount = 0;

		// short-circuit the nop case
		if( chOld != chNew )
		{
			// otherwise modify each character that matches in the string
			bool bCopied = false;
			PXSTR pszBuffer = const_cast< PXSTR >( GetString() );  // We don't actually write to pszBuffer until we've called GetBuffer().

			int nLength = GetLength();
			int iChar = 0;
			while( iChar < nLength )
			{
				// replace instances of the specified character only
				if( pszBuffer[iChar] == chOld )
				{
					if( !bCopied )
					{
						bCopied = true;
						pszBuffer = GetBuffer( nLength );
					}
					pszBuffer[iChar] = chNew;
					nCount++;
				}
				iChar = int( StringTraits::CharNext( pszBuffer+iChar )-pszBuffer );
			}
			if( bCopied )
			{
				ReleaseBufferSetLength( nLength );
			}
		}

		return( nCount );
	}

	// Replace all occurrences of string 'pszOld' with string 'pszNew'
	int Replace( __in PCXSTR pszOld, __in PCXSTR pszNew )
	{
		// can't have empty or NULL lpszOld

		// nSourceLen is in XCHARs
		int nSourceLen = StringTraits::SafeStringLen( pszOld );
		if( nSourceLen == 0 )
			return( 0 );
		// nReplacementLen is in XCHARs
		int nReplacementLen = StringTraits::SafeStringLen( pszNew );

		// loop once to figure out the size of the result string
		int nCount = 0;
		{
			PCXSTR pszStart = GetString();
			PCXSTR pszEnd = pszStart+GetLength();
			while( pszStart < pszEnd )
			{
				PCXSTR pszTarget;
				while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL)
				{
					nCount++;
					pszStart = pszTarget+nSourceLen;
				}
				pszStart += StringTraits::SafeStringLen( pszStart )+1;
			}
		}

		// if any changes were made, make them
		if( nCount > 0 )
		{
			// if the buffer is too small, just
			//   allocate a new buffer (slow but sure)
			int nOldLength = GetLength();
			int nNewLength = nOldLength+(nReplacementLen-nSourceLen)*nCount;

			PXSTR pszBuffer = GetBuffer( __max( nNewLength, nOldLength ) );

			PXSTR pszStart = pszBuffer;
			PXSTR pszEnd = pszStart+nOldLength;

			// loop again to actually do the work
			while( pszStart < pszEnd )
			{
				PXSTR pszTarget;
				while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL )
				{
					int nBalance = nOldLength-int(pszTarget-pszBuffer+nSourceLen);
                    Checked::memmove_s( pszTarget+nReplacementLen, nBalance*sizeof( XCHAR ), 
						pszTarget+nSourceLen, nBalance*sizeof( XCHAR ) );
					Checked::memcpy_s( pszTarget, nReplacementLen*sizeof( XCHAR ), 
						pszNew, nReplacementLen*sizeof( XCHAR ) );
					pszStart = pszTarget+nReplacementLen;
					pszTarget[nReplacementLen+nBalance] = 0;
					nOldLength += (nReplacementLen-nSourceLen);
				}
				pszStart += StringTraits::SafeStringLen( pszStart )+1;
			}
			ATLASSERT( pszBuffer[nNewLength] == 0 );
			ReleaseBufferSetLength( nNewLength );
		}

		return( nCount );
	}

	// Remove all occurrences of character 'chRemove'
	int Remove( __in XCHAR chRemove )
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );

		PXSTR pszSource = pszBuffer;
		PXSTR pszDest = pszBuffer;
		PXSTR pszEnd = pszBuffer+nLength;

		while( pszSource < pszEnd )
		{
			PXSTR pszNewSource = StringTraits::CharNext( pszSource );
			if( *pszSource != chRemove )
			{
				// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
	   			// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
				size_t NewSourceGap = (pszNewSource-pszSource);
				PXSTR pszNewDest = pszDest + NewSourceGap;
				size_t i = 0;
				for (i = 0;  pszDest != pszNewDest && i < NewSourceGap; i++)
				{
					*pszDest = *pszSource;
					pszSource++;
					pszDest++;
				}
			}
			pszSource = pszNewSource;
		}
		*pszDest = 0;
		int nCount = int( pszSource-pszDest );
		ReleaseBufferSetLength( nLength-nCount );

		return( nCount );
	}

	CStringT Tokenize( __in PCXSTR pszTokens, __inout int& iStart ) const
	{
		ATLASSERT( iStart >= 0 );
			
		if(iStart < 0)
			AtlThrow(E_INVALIDARG);			
			
		if( (pszTokens == NULL) || (*pszTokens == (XCHAR)0) )
		{
			if (iStart < GetLength())
			{
				return( CStringT( GetString()+iStart, GetManager() ) );
			}
		}
		else
		{
			PCXSTR pszPlace = GetString()+iStart;
			PCXSTR pszEnd = GetString()+GetLength();
			if( pszPlace < pszEnd )
			{
				int nIncluding = StringTraits::StringSpanIncluding( pszPlace,
					pszTokens );

				if( (pszPlace+nIncluding) < pszEnd )
				{
					pszPlace += nIncluding;
					int nExcluding = StringTraits::StringSpanExcluding( pszPlace, pszTokens );

					int iFrom = iStart+nIncluding;
					int nUntil = nExcluding;
					iStart = iFrom+nUntil+1;

					return( Mid( iFrom, nUntil ) );
				}
			}
		}

		// return empty string, done tokenizing
		iStart = -1;

		return( CStringT( GetManager() ) );
	}

	// find routines

	// Find the first occurrence of character 'ch', starting at index 'iStart'
	int Find( __in XCHAR ch, __in int iStart = 0 ) const throw()
	{
		// iStart is in XCHARs
		ATLASSERT( iStart >= 0 );

		// nLength is in XCHARs
		int nLength = GetLength();
		if( iStart < 0 || iStart >= nLength)
		{
			return( -1 );
		}

		// find first single character
		PCXSTR psz = StringTraits::StringFindChar( GetString()+iStart, ch );

		// return -1 if not found and index otherwise
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// look for a specific sub-string

	// Find the first occurrence of string 'pszSub', starting at index 'iStart'
	int Find( __in PCXSTR pszSub, __in int iStart = 0 ) const throw()
	{
		// iStart is in XCHARs
		ATLASSERT( iStart >= 0 );
		ATLASSERT( AtlIsValidString( pszSub ) );

		if(pszSub == NULL)
		{
			return( -1 );
		}
		// nLength is in XCHARs
		int nLength = GetLength();
		if( iStart < 0 || iStart > nLength )
		{
			return( -1 );
		}

		// find first matching substring
		PCXSTR psz = StringTraits::StringFindString( GetString()+iStart, pszSub );

		// return -1 for not found, distance from beginning otherwise
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// Find the first occurrence of any of the characters in string 'pszCharSet'
	int FindOneOf( __in PCXSTR pszCharSet ) const throw()
	{
		ATLASSERT( AtlIsValidString( pszCharSet ) );
		PCXSTR psz = StringTraits::StringScanSet( GetString(), pszCharSet );
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// Find the last occurrence of character 'ch'
	int ReverseFind( __in XCHAR ch ) const throw()
	{
		// find last single character
		PCXSTR psz = StringTraits::StringFindCharRev( GetString(), ch );

		// return -1 if not found, distance from beginning otherwise
		return( (psz == NULL) ? -1 : int( psz-GetString() ) );
	}

	// manipulation

	// Convert the string to uppercase
	CStringT& MakeUpper()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::StringUppercase( pszBuffer, nLength+1 );
		ReleaseBufferSetLength( nLength );

		return( *this );
	}

	// Convert the string to lowercase
	CStringT& MakeLower()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::StringLowercase( pszBuffer, nLength+1 );
		ReleaseBufferSetLength( nLength );

		return( *this );
	}

	// Reverse the string
	CStringT& MakeReverse()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::StringReverse( pszBuffer );
		ReleaseBufferSetLength( nLength );

		return( *this );
	}

	// trimming

	// Remove all trailing whitespace
	CStringT& TrimRight()
	{
		// find beginning of trailing spaces by starting
		// at beginning (DBCS aware)

		PCXSTR psz = GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( StringTraits::IsSpace( *psz ) )
			{
				if( pszLast == NULL )
					pszLast = psz;
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at trailing space start
			int iLast = int( pszLast-GetString() );

			Truncate( iLast );
		}

		return( *this );
	}

	// Remove all leading whitespace
	CStringT& TrimLeft()
	{
		// find first non-space character

		PCXSTR psz = GetString();

		while( StringTraits::IsSpace( *psz ) )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != GetString() )
		{
			// fix up data and length
			int iFirst = int( psz-GetString() );
			PXSTR pszBuffer = GetBuffer( GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = GetLength()-iFirst;
            Checked::memmove_s( pszBuffer, (nDataLength+1)*sizeof( XCHAR ), 
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			ReleaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Remove all leading and trailing whitespace
	CStringT& Trim()
	{
		return( TrimRight().TrimLeft() );
	}

	// Remove all leading and trailing occurrences of character 'chTarget'
	CStringT& Trim( __in XCHAR chTarget )
	{
		return( TrimRight( chTarget ).TrimLeft( chTarget ) );
	}

	// Remove all leading and trailing occurrences of any of the characters in the string 'pszTargets'
	CStringT& Trim( __in PCXSTR pszTargets )
	{
		return( TrimRight( pszTargets ).TrimLeft( pszTargets ) );
	}

	// trimming anything (either side)

	// Remove all trailing occurrences of character 'chTarget'
	CStringT& TrimRight( __in XCHAR chTarget )
	{
		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		PCXSTR psz = GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( *psz == chTarget )
			{
				if( pszLast == NULL )
				{
					pszLast = psz;
				}
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at left-most matching character  
			int iLast = int( pszLast-GetString() );
			Truncate( iLast );
		}

		return( *this );
	}

	// Remove all trailing occurrences of any of the characters in string 'pszTargets'
	CStringT& TrimRight( __in PCXSTR pszTargets )
	{
		// if we're not trimming anything, we're not doing any work
		if( (pszTargets == NULL) || (*pszTargets == 0) )
		{
			return( *this );
		}

		// find beginning of trailing matches
		// by starting at beginning (DBCS aware)

		PCXSTR psz = GetString();
		PCXSTR pszLast = NULL;

		while( *psz != 0 )
		{
			if( StringTraits::StringFindChar( pszTargets, *psz ) != NULL )
			{
				if( pszLast == NULL )
				{
					pszLast = psz;
				}
			}
			else
			{
				pszLast = NULL;
			}
			psz = StringTraits::CharNext( psz );
		}

		if( pszLast != NULL )
		{
			// truncate at left-most matching character  
			int iLast = int( pszLast-GetString() );
			Truncate( iLast );
		}

		return( *this );
	}

	// Remove all leading occurrences of character 'chTarget'
	CStringT& TrimLeft( __in XCHAR chTarget )
	{
		// find first non-matching character
		PCXSTR psz = GetString();

		while( chTarget == *psz )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != GetString() )
		{
			// fix up data and length
			int iFirst = int( psz-GetString() );
			PXSTR pszBuffer = GetBuffer( GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = GetLength()-iFirst;
            Checked::memmove_s( pszBuffer, (nDataLength+1)*sizeof( XCHAR ), 
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			ReleaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Remove all leading occurrences of any of the characters in string 'pszTargets'
	CStringT& TrimLeft( __in PCXSTR pszTargets )
	{
		// if we're not trimming anything, we're not doing any work
		if( (pszTargets == NULL) || (*pszTargets == 0) )
		{
			return( *this );
		}

		PCXSTR psz = GetString();
		while( (*psz != 0) && (StringTraits::StringFindChar( pszTargets, *psz ) != NULL) )
		{
			psz = StringTraits::CharNext( psz );
		}

		if( psz != GetString() )
		{
			// fix up data and length
			int iFirst = int( psz-GetString() );
			PXSTR pszBuffer = GetBuffer( GetLength() );
			psz = pszBuffer+iFirst;
			int nDataLength = GetLength()-iFirst;
            Checked::memmove_s( pszBuffer, (nDataLength+1)*sizeof( XCHAR ), 
				psz, (nDataLength+1)*sizeof( XCHAR ) );
			ReleaseBufferSetLength( nDataLength );
		}

		return( *this );
	}

	// Convert the string to the OEM character set
	void AnsiToOem()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::ConvertToOem( pszBuffer, nLength+1 );
		ReleaseBufferSetLength( nLength );
	}

	// Convert the string to the ANSI character set
	void OemToAnsi()
	{
		int nLength = GetLength();
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::ConvertToAnsi( pszBuffer, nLength+1 );
		ReleaseBufferSetLength( nLength );
	}

	// Very simple sub-string extraction

	// Return the substring starting at index 'iFirst'
	CStringT Mid( __in int iFirst ) const
	{
		return( Mid( iFirst, GetLength()-iFirst ) );
	}

	// Return the substring starting at index 'iFirst', with length 'nCount'
	CStringT Mid( __in int iFirst, __in int nCount ) const
	{
		// nCount is in XCHARs

		// out-of-bounds requests return sensible things
		if (iFirst < 0)
			iFirst = 0;
		if (nCount < 0)
			nCount = 0;

		if( (::ATL::AtlAddThrow(iFirst,nCount)) > GetLength() )
		{
			nCount = GetLength()-iFirst;
		}
		if( iFirst > GetLength() )
		{
			nCount = 0;
		}

		ATLASSERT( (nCount == 0) || ((iFirst+nCount) <= GetLength()) );

		// optimize case of returning entire string
		if( (iFirst == 0) && ((iFirst+nCount) == GetLength()) )
		{
			return( *this );
		}

		return( CStringT( GetString()+iFirst, nCount, GetManager() ) );
	}

	// Return the substring consisting of the rightmost 'nCount' characters
	CStringT Right( __in int nCount ) const
	{
		// nCount is in XCHARs
		if (nCount < 0)
			nCount = 0;

		int nLength = GetLength();
		// prefast does not know about GetLength()
		if (nLength < 0)
			nLength = 0;
		if( nCount >= nLength )
		{
			return( *this );
		}

		return( CStringT( GetString()+nLength-nCount, nCount, GetManager() ) );
	}

	// Return the substring consisting of the leftmost 'nCount' characters
	CStringT Left( __in int nCount ) const
	{
		// nCount is in XCHARs
		if (nCount < 0)
			nCount = 0;

		int nLength = GetLength();
		if( nCount >= nLength )
		{
			return( *this );
		}

		return( CStringT( GetString(), nCount, GetManager() ) );
	}

	// Return the substring consisting of the leftmost characters in the set 'pszCharSet'
	CStringT SpanIncluding( __in PCXSTR pszCharSet ) const
	{
		ATLASSERT( AtlIsValidString( pszCharSet ) );
		if(pszCharSet == NULL)
			AtlThrow(E_INVALIDARG);

		return( Left( StringTraits::StringSpanIncluding( GetString(), pszCharSet ) ) );
	}

	// Return the substring consisting of the leftmost characters not in the set 'pszCharSet'
	CStringT SpanExcluding( __in PCXSTR pszCharSet ) const
	{
		ATLASSERT( AtlIsValidString( pszCharSet ) );
		if(pszCharSet == NULL)
			AtlThrow(E_INVALIDARG);

		return( Left( StringTraits::StringSpanExcluding( GetString(), pszCharSet ) ) );
 	}

	// Format data using format string 'pszFormat'
	void __cdecl Format( __in __format_string PCXSTR pszFormat, ... );

	// Format data using format string loaded from resource 'nFormatID'
	void __cdecl Format( __format_string UINT nFormatID, ... );

	// Append formatted data using format string loaded from resource 'nFormatID'
	void __cdecl AppendFormat( __in UINT nFormatID, ... );

	// Append formatted data using format string 'pszFormat'
	void __cdecl AppendFormat( __in __format_string PCXSTR pszFormat, ... );
	void AppendFormatV( __in __format_string PCXSTR pszFormat, va_list args )
	{
		ATLASSERT( AtlIsValidString( pszFormat ) );

		int nCurrentLength = GetLength();
		int nAppendLength = StringTraits::GetFormattedLength( pszFormat, args );

		if (nAppendLength == -1)
			AtlThrow(E_OUTOFMEMORY);

		PXSTR pszBuffer = GetBuffer( nCurrentLength+nAppendLength );
#if _SECURE_ATL
        StringTraits::Format( pszBuffer+nCurrentLength, 
			nAppendLength+1, pszFormat, args );
#else
		StringTraits::Format( pszBuffer+nCurrentLength, pszFormat, args );
#endif
		ReleaseBufferSetLength( nCurrentLength+nAppendLength );
	}

	void FormatV( __in __format_string PCXSTR pszFormat, va_list args )
	{
		ATLASSERT( AtlIsValidString( pszFormat ) );
		if(pszFormat == NULL)
			AtlThrow(E_INVALIDARG);

		int nLength = StringTraits::GetFormattedLength( pszFormat, args );

		if (nLength == -1)
			AtlThrow(E_OUTOFMEMORY);

		PXSTR pszBuffer = GetBuffer( nLength );
#if _SECURE_ATL
        StringTraits::Format( pszBuffer, nLength+1, pszFormat, args );
#else
		StringTraits::Format( pszBuffer, pszFormat, args );
#endif
		ReleaseBufferSetLength( nLength );
	}

	// Format a message using format string 'pszFormat'
	void __cdecl _AFX_FUNCNAME(FormatMessage)( __in __format_string PCXSTR pszFormat, ... );

	// Format a message using format string loaded from resource 'nFormatID'
	void __cdecl _AFX_FUNCNAME(FormatMessage)( __in UINT nFormatID, ... );

#if defined(_AFX)
	void __cdecl FormatMessage( __in __format_string PCXSTR pszFormat, ... );

	void __cdecl FormatMessage( __in UINT nFormatID, ... );
#endif

	// Format a message using format string 'pszFormat' and va_list
	void FormatMessageV( __in __format_string PCXSTR pszFormat, va_list* pArgList )
	{
		// format message into temporary buffer pszTemp
		CHeapPtr< XCHAR, CLocalAllocator > pszTemp;
		DWORD dwResult = StringTraits::_AFX_FUNCNAME(FormatMessage)( FORMAT_MESSAGE_FROM_STRING|
			FORMAT_MESSAGE_ALLOCATE_BUFFER, pszFormat, 0, 0, reinterpret_cast< PXSTR >( &pszTemp ),
			0, pArgList );
		if( dwResult == 0 )
		{
			ThrowMemoryException();
		}

		*this = pszTemp;
	}

	// OLE BSTR support

	// Allocate a BSTR containing a copy of the string
	BSTR AllocSysString() const
	{
		BSTR bstrResult = StringTraits::AllocSysString( GetString(), GetLength() );

#pragma warning(push)
#pragma warning(disable:4068)
#pragma prefast(push)
#pragma prefast(disable:325, "We are deliberately checking if this has already been allocated")
		if( bstrResult == NULL )
		{
			ThrowMemoryException();
		}
#pragma prefast(pop)
#pragma warning(pop)

		return( bstrResult );
	}

	BSTR SetSysString( __out BSTR* pbstr ) const
	{
		ATLASSERT( AtlIsValidAddress( pbstr, sizeof( BSTR ) ) );

		if( !StringTraits::ReAllocSysString( GetString(), pbstr,
			GetLength() ) )
		{
			ThrowMemoryException();
		}

#pragma warning(push)
#pragma warning(disable:4068)
#pragma prefast(push)
#pragma prefast(disable:325, "We are deliberately checking if this has already been allocated")
		ATLASSERT( *pbstr != NULL );
#pragma prefast(pop)
#pragma warning(pop)

        return( *pbstr );
	}

	// Set the string to the value of environment variable 'pszVar'
	__checkReturn BOOL GetEnvironmentVariable( __in PCXSTR pszVar )
	{
		ULONG nLength = StringTraits::GetEnvironmentVariable( pszVar, NULL, 0 );
		BOOL bRetVal = FALSE;

		if( nLength == 0 )
		{
			Empty();
		}
		else
		{
			PXSTR pszBuffer = GetBuffer( nLength );
			StringTraits::GetEnvironmentVariable( pszVar, pszBuffer, nLength );
			ReleaseBuffer();
			bRetVal = TRUE;
		}

		return( bRetVal );
	}

	// Load the string from resource 'nID'
	__checkReturn BOOL LoadString( __in UINT nID )
	{
		HINSTANCE hInst = StringTraits::FindStringResourceInstance( nID );
		if( hInst == NULL )
		{
			return( FALSE );
		}

		return( LoadString( hInst, nID ) );		
	}

	// Load the string from resource 'nID' in module 'hInstance'
	__checkReturn BOOL LoadString( __in HINSTANCE hInstance, __in UINT nID )
	{
		const ATLSTRINGRESOURCEIMAGE* pImage = AtlGetStringResourceImage( hInstance, nID );
		if( pImage == NULL )
		{
			return( FALSE );
		}

		int nLength = StringTraits::GetBaseTypeLength( pImage->achString, pImage->nLength );
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::ConvertToBaseType( pszBuffer, nLength, pImage->achString, pImage->nLength );
		ReleaseBufferSetLength( nLength );

		return( TRUE );
	}

	// Load the string from resource 'nID' in module 'hInstance', using language 'wLanguageID'
	__checkReturn BOOL LoadString( __in HINSTANCE hInstance, __in UINT nID, __in WORD wLanguageID )
	{
		const ATLSTRINGRESOURCEIMAGE* pImage = AtlGetStringResourceImage( hInstance, nID, wLanguageID );
		if( pImage == NULL )
		{
			return( FALSE );
		}

		int nLength = StringTraits::GetBaseTypeLength( pImage->achString, pImage->nLength );
		PXSTR pszBuffer = GetBuffer( nLength );
		StringTraits::ConvertToBaseType( pszBuffer, nLength, pImage->achString, pImage->nLength );
		ReleaseBufferSetLength( nLength );

		return( TRUE );
	}

	friend CStringT operator+( __in const CStringT& str1, __in const CStringT& str2 )
	{
		CStringT strResult( str1.GetManager() );

		Concatenate( strResult, str1, str1.GetLength(), str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+( __in const CStringT& str1, __in PCXSTR psz2 )
	{
		CStringT strResult( str1.GetManager() );

		Concatenate( strResult, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

		return( strResult );
	}

	friend CStringT operator+( __in PCXSTR psz1, __in const CStringT& str2 )
	{
		CStringT strResult( str2.GetManager() );

		Concatenate( strResult, psz1, StringLength( psz1 ), str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+( __in const CStringT& str1, __in wchar_t ch2 )
	{
		CStringT strResult( str1.GetManager() );
		XCHAR chTemp = XCHAR( ch2 );

		Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

		return( strResult );
	}

	friend CStringT operator+( __in const CStringT& str1, __in char ch2 )
	{
		CStringT strResult( str1.GetManager() );
		XCHAR chTemp = XCHAR( ch2 );

		Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

		return( strResult );
	}

	friend CStringT operator+( __in wchar_t ch1, __in const CStringT& str2 )
	{
		CStringT strResult( str2.GetManager() );
		XCHAR chTemp = XCHAR( ch1 );

		Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

		return( strResult );
	}

	friend CStringT operator+( __in char ch1, __in const CStringT& str2 )
	{
		CStringT strResult( str2.GetManager() );
		XCHAR chTemp = XCHAR( ch1 );

		Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

		return( strResult );
	}

	friend bool operator==( __in const CStringT& str1, __in const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) == 0 );
	}

	friend bool operator==(
		__in const CStringT& str1, __in PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) == 0 );
	}

	friend bool operator==(
		__in PCXSTR psz1, __in const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) == 0 );
	}

	friend bool operator==(
		__in const CStringT& str1, __in PCYSTR psz2 ) throw( ... )
	{
		CStringT str2( psz2, str1.GetManager() );

		return( str1 == str2 );
	}

	friend bool operator==(
		__in PCYSTR psz1, __in const CStringT& str2 ) throw( ... )
	{
		CStringT str1( psz1, str2.GetManager() );

		return( str1 == str2 );
	}

	friend bool operator!=(
		__in const CStringT& str1, __in const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) != 0 );
	}

	friend bool operator!=(
		__in const CStringT& str1, __in PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) != 0 );
	}

	friend bool operator!=(
		__in PCXSTR psz1, __in const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) != 0 );
	}

	friend bool operator!=(
		__in const CStringT& str1, __in PCYSTR psz2 ) throw( ... )
	{
		CStringT str2( psz2, str1.GetManager() );

		return( str1 != str2 );
	}

	friend bool operator!=(
		__in PCYSTR psz1, __in const CStringT& str2 ) throw( ... )
	{
		CStringT str1( psz1, str2.GetManager() );

		return( str1 != str2 );
	}

	friend bool operator<( __in const CStringT& str1, __in const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) < 0 );
	}

	friend bool operator<( __in const CStringT& str1, __in PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) < 0 );
	}

	friend bool operator<( __in PCXSTR psz1, __in const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) > 0 );
	}

	friend bool operator>( __in const CStringT& str1, __in const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) > 0 );
	}

	friend bool operator>( __in const CStringT& str1, __in PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) > 0 );
	}

	friend bool operator>( __in PCXSTR psz1, __in const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) < 0 );
	}

	friend bool operator<=( __in const CStringT& str1, __in const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) <= 0 );
	}

	friend bool operator<=( __in const CStringT& str1, __in PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) <= 0 );
	}

	friend bool operator<=( __in PCXSTR psz1, __in const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) >= 0 );
	}

	friend bool operator>=( __in const CStringT& str1, __in const CStringT& str2 ) throw()
	{
		return( str1.Compare( str2 ) >= 0 );
	}

	friend bool operator>=( __in const CStringT& str1, __in PCXSTR psz2 ) throw()
	{
		return( str1.Compare( psz2 ) >= 0 );
	}

	friend bool operator>=( __in PCXSTR psz1, __in const CStringT& str2 ) throw()
	{
		return( str2.Compare( psz1 ) <= 0 );
	}

	friend bool operator==( __in XCHAR ch1, __in const CStringT& str2 ) throw()
	{
		return( (str2.GetLength() == 1) && (str2[0] == ch1) );
	}

	friend bool operator==( __in const CStringT& str1, __in XCHAR ch2 ) throw()
	{
		return( (str1.GetLength() == 1) && (str1[0] == ch2) );
	}

	friend bool operator!=( __in XCHAR ch1, __in const CStringT& str2 ) throw()
	{
		return( (str2.GetLength() != 1) || (str2[0] != ch1) );
	}

	friend bool operator!=( __in const CStringT& str1, __in XCHAR ch2 ) throw()
	{
		return( (str1.GetLength() != 1) || (str1[0] != ch2) );
	}

private:
	bool CheckImplicitLoad( __in_opt const void* pv )
	{
		bool bRet = false;

		if( (pv != NULL) && IS_INTRESOURCE( pv ) )
		{
			UINT nID = LOWORD( reinterpret_cast< DWORD_PTR >( pv ) );
			if( !LoadString( nID ) )
			{
				ATLTRACE( atlTraceString, 2, _T( "Warning: implicit LoadString(%u) failed\n" ), nID );
			}
			bRet = true;
		}

		return( bRet );
	}

};

#ifndef _CSTRING_BUFFER_SIZE
#define _CSTRING_BUFFER_SIZE(_CStringObj) ((_CStringObj).GetAllocLength() + 1)
#endif

#pragma warning(push)
#pragma warning(disable : 4793)
// Format data using format string 'pszFormat'
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::Format( __in __format_string PCXSTR pszFormat, ... )
{
	ATLASSERT( AtlIsValidString( pszFormat ) );

	va_list argList;
	va_start( argList, pszFormat );
	FormatV( pszFormat, argList );
	va_end( argList );
}

// Format data using format string loaded from resource 'nFormatID'
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::Format( __format_string UINT nFormatID, ... )
{
	CStringT strFormat( GetManager() );
	ATLVERIFY( strFormat.LoadString( nFormatID ) );

	va_list argList;
	va_start( argList, nFormatID );
	FormatV( strFormat, argList );
	va_end( argList );
}

// Append formatted data using format string loaded from resource 'nFormatID'
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::AppendFormat( __in UINT nFormatID, ... )
{
	va_list argList;
	va_start( argList, nFormatID );

	CStringT strFormat( GetManager() );
	ATLVERIFY( strFormat.LoadString( nFormatID ) ); 

	AppendFormatV( strFormat, argList );

	va_end( argList );
}


// Append formatted data using format string 'pszFormat'
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::AppendFormat( __in __format_string PCXSTR pszFormat, ... )
{
	ATLASSERT( AtlIsValidString( pszFormat ) );

	va_list argList;
	va_start( argList, pszFormat );

	AppendFormatV( pszFormat, argList );

	va_end( argList );
}

// Format a message using format string 'pszFormat'
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::_AFX_FUNCNAME(FormatMessage)( __in __format_string PCXSTR pszFormat, ... )
{
	if(pszFormat == NULL)
		AtlThrow(E_INVALIDARG);

	va_list argList;
	va_start( argList, pszFormat );

	FormatMessageV( pszFormat, &argList );

	va_end( argList );
}

#if defined(_AFX)
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::FormatMessage( __in __format_string PCXSTR pszFormat, ... )
{
	if(pszFormat == NULL)
		AtlThrow(E_INVALIDARG);

	va_list argList;
	va_start( argList, pszFormat );

	FormatMessageV( pszFormat, &argList );

	va_end( argList );
}
#endif

// Format a message using format string loaded from resource 'nFormatID'
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::_AFX_FUNCNAME(FormatMessage)( __in UINT nFormatID, ... )
{
	// get format string from string table
	CStringT strFormat( GetManager() );
	ATLVERIFY( strFormat.LoadString( nFormatID ) );

	if(strFormat.GetLength() == 0)
		AtlThrow(E_INVALIDARG);

	va_list argList;
	va_start( argList, nFormatID );

	FormatMessageV( strFormat, &argList );

	va_end( argList );
}

#if defined(_AFX)
template< typename BaseType, class StringTraits >
inline void __cdecl CStringT<BaseType, StringTraits>::FormatMessage( __in UINT nFormatID, ... )
{
	// get format string from string table
	CStringT strFormat( GetManager() );
	ATLVERIFY( strFormat.LoadString( nFormatID ) );

	if(strFormat.GetLength() == 0)
		AtlThrow(E_INVALIDARG);

	va_list argList;
	va_start( argList, nFormatID );

	FormatMessageV( strFormat, &argList );

	va_end( argList );
}
#endif

#pragma warning( pop )

class IFixedStringLog
{
public:
	virtual void OnAllocateSpill( int nActualChars, int nFixedChars, const CStringData* pData ) throw() = 0;
	virtual void OnReallocateSpill( int nActualChars, int nFixedChars, const CStringData* pData ) throw() = 0;
};

class CFixedStringMgr :
	public IAtlStringMgr
{
public:
	CFixedStringMgr( __in CStringData* pData, __in int nChars, __in_opt IAtlStringMgr* pMgr = NULL ) throw() :
		m_pData( pData ),
		m_pMgr( pMgr )
	{
		m_pData->nRefs = -1;
		m_pData->nDataLength = 0;
		m_pData->nAllocLength = nChars;
		m_pData->pStringMgr = this;
		*static_cast< wchar_t* >( m_pData->data() ) = 0;
	}
	virtual ~CFixedStringMgr() throw()
	{
	}

// IAtlStringMgr
public:
	virtual CStringData* Allocate( __in int nChars, __in int nCharSize ) throw()
	{
		ATLASSUME( m_pData->nRefs == -1 );
		ATLASSUME( m_pData->nDataLength == 0 );
		if( m_pData->nAllocLength >= 0 && m_pData->nAllocLength < nChars )
		{
			if( s_pLog != NULL )
			{
				s_pLog->OnAllocateSpill( nChars, m_pData->nAllocLength, m_pData );
			}
			CStringData* pData = m_pMgr->Allocate( nChars, nCharSize );
			if( pData != NULL )
			{
				pData->pStringMgr = this;
				pData->nRefs = -1;  // Locked
			}

			return pData;
		}

		m_pData->nRefs = -1;  // Locked
		m_pData->nDataLength = 0;
		m_pData->pStringMgr = this;

		return m_pData;
	}
	virtual void Free( __inout CStringData* pData ) throw()
	{
		ATLASSERT( pData->nRefs <= 0 );
		if( pData != m_pData )
		{
			// Must have been allocated from the backup manager
			pData->pStringMgr = m_pMgr;
			m_pMgr->Free( pData );
		}

		// Always make sure the fixed buffer is ready to be used as the nil string.
		m_pData->nRefs = -1;
		m_pData->nDataLength = 0;
		*static_cast< wchar_t* >( m_pData->data() ) = 0;
	}
	virtual CStringData* Reallocate( __inout CStringData* pData, __in int nChars, __in int nCharSize ) throw()
	{
		CStringData* pNewData;

		ATLASSERT( pData->nRefs < 0 );
		if( pData != m_pData )
		{
			pData->pStringMgr = m_pMgr;
			pNewData = m_pMgr->Reallocate( pData, nChars, nCharSize );
			if( pNewData == NULL )
			{
				pData->pStringMgr = this;
			}
			else
			{
				pNewData->pStringMgr = this;
			}
		}
		else
		{
			if( pData->nAllocLength >= 0 && pData->nAllocLength < nChars )
			{
                size_t CopySize = 0;

				if( s_pLog != NULL )
				{
					s_pLog->OnReallocateSpill( nChars, pData->nAllocLength, pData );
				}
				pNewData = m_pMgr->Allocate( nChars, nCharSize );
				if( pNewData == NULL )
				{
					return NULL;
				}

                if (FAILED(SizeTMult(nChars, nCharSize, &CopySize)))
                {
                    return NULL;
                }

				// Copy the string data 
                Checked::memcpy_s( pNewData->data(), CopySize, 
					pData->data(), (pData->nAllocLength+1)*nCharSize );
				pNewData->nRefs = pData->nRefs;  // Locked
				pNewData->pStringMgr = this;
				pNewData->nDataLength = pData->nDataLength;
			}
			else
			{
				// Don't do anything if the buffer is already big enough.
				pNewData = pData;
			}
		}

		return pNewData;
	}
	virtual CStringData* GetNilString() throw()
	{
		ATLASSUME( m_pData->nRefs == -1 );
		ATLASSUME( m_pData->nDataLength == 0 );

		return m_pData;
	}
	virtual IAtlStringMgr* Clone() throw()
	{
		return m_pMgr;
	}

public:
	static IFixedStringLog* s_pLog;

	IAtlStringMgr* GetBackupManager() const throw()
	{
		return m_pMgr;
	}

protected:
	IAtlStringMgr* m_pMgr;
	CStringData* m_pData;
};

__declspec( selectany ) IFixedStringLog* CFixedStringMgr::s_pLog = NULL;

#pragma warning( push )
#pragma warning( disable: 4355 )  // 'this' used in base member initializer list

template< class StringType, int t_nChars >
class CFixedStringT :
	private CFixedStringMgr,  // This class must be first, since it needs to be initialized before StringType
	public StringType
{
public:
	CFixedStringT() throw() :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( static_cast< IAtlStringMgr* >( this ) )
	{
	}

	explicit CFixedStringT( __in IAtlStringMgr* pStringMgr ) throw() :
		CFixedStringMgr( &m_data, t_nChars, pStringMgr ),
		StringType( static_cast< IAtlStringMgr* >( this ) )
	{
	}

	CFixedStringT( __in const CFixedStringT< StringType, t_nChars >& str ) :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( str.GetString(), str.GetLength(), static_cast< CFixedStringMgr* >( this ) )
	{
	}

	CFixedStringT( __in const StringType& str ) :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( str.GetString(), str.GetLength(), static_cast< CFixedStringMgr* >( this ) )
	{
	}

	CFixedStringT( __in const typename StringType::XCHAR* psz ) :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( psz, static_cast< CFixedStringMgr* >( this ) )
	{
	}

	CFixedStringT( __in_ecount(nLength) const typename StringType::XCHAR* psz, __in int nLength ) :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( psz, nLength, static_cast< CFixedStringMgr* >( this ) )
	{
	}

	explicit CFixedStringT( __in const typename StringType::YCHAR* psz ) :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( psz, static_cast< CFixedStringMgr* >( this ) )
	{
	}

	explicit CFixedStringT( __in const unsigned char* psz ) :
		CFixedStringMgr( &m_data, t_nChars, StrTraits::GetDefaultManager() ),
		StringType( psz, static_cast< CFixedStringMgr* >( this ) )
	{
	}

	~CFixedStringT() throw()
	{
		Empty();
	}

	CFixedStringT< StringType, t_nChars >& operator=( __in const CFixedStringT< StringType, t_nChars >& str )
	{
		StringType::operator=( str );
		return *this;
	}

	CFixedStringT< StringType, t_nChars >& operator=( __in_z const char* psz )
	{
		StringType::operator=( psz );
		return *this;
	}

	CFixedStringT< StringType, t_nChars >& operator=( __in_z const wchar_t* psz )
	{
		StringType::operator=( psz );
		return *this;
	}

	CFixedStringT< StringType, t_nChars >& operator=( __in_z const unsigned char* psz )
	{
		StringType::operator=( psz );
		return *this;
	}

	CFixedStringT< StringType, t_nChars >& operator=( __in const StringType& str )
	{
		StringType::operator=( str );
		return *this;
	}

// Implementation
protected:
	CStringData m_data;
	typename StringType::XCHAR m_achData[t_nChars+1];
};

#pragma warning( pop )
class CFixedStringLog :
	public IFixedStringLog
{
public:
	CFixedStringLog() throw()
	{
		CFixedStringMgr::s_pLog = this;
	}
	virtual ~CFixedStringLog() throw()
	{
		CFixedStringMgr::s_pLog = NULL;
	}

public:
	void OnAllocateSpill( __in int nActualChars, __in int nFixedChars, __in const CStringData* pData ) throw()
	{
		(void)nActualChars;
		(void)nFixedChars;
		(void)pData;
		ATLTRACE( atlTraceString, 0, _T( "CFixedStringMgr::Allocate() spilling to heap.  %d chars (fixed size = %d chars)\n" ), nActualChars, nFixedChars );
	}
	void OnReallocateSpill( __in int nActualChars, __in int nFixedChars, __in const CStringData* pData ) throw()
	{
		(void)nActualChars;
		(void)nFixedChars;
		(void)pData;
		ATLTRACE( atlTraceString, 0, _T( "CFixedStringMgr::Reallocate() spilling to heap.  %d chars (fixed size = %d chars)\n" ), nActualChars, nFixedChars );
	}
};

template< typename T >
class CStringElementTraits
{
public:
	typedef typename T::PCXSTR INARGTYPE;
	typedef T& OUTARGTYPE;

	static void __cdecl CopyElements( __out_ecount_full(nElements) T* pDest, const __in_ecount(nElements) T* pSrc, __in size_t nElements )
	{
		for( size_t iElement = 0; iElement < nElements; iElement++ )
		{
			pDest[iElement] = pSrc[iElement];
		}
	}

	static void __cdecl RelocateElements( __out_ecount_full(nElements) T* pDest, __in_ecount(nElements) T* pSrc, __in size_t nElements )
	{
		Checked::memmove_s( pDest, nElements*sizeof( T ), pSrc, nElements*sizeof( T ) );
	}

	static ULONG __cdecl Hash( __in INARGTYPE str )
	{
		ATLENSURE( str != NULL );
		ULONG nHash = 0;
		const T::XCHAR* pch = str;
		while( *pch != 0 )
		{
			nHash = (nHash<<5)+nHash+(*pch);
			pch++;
		}

		return( nHash );
	}

	static bool __cdecl CompareElements( __in INARGTYPE str1, __in INARGTYPE str2 )
	{
		return( T::StrTraits::StringCompare( str1, str2 ) == 0 );
	}

	static int __cdecl CompareElementsOrdered( __in INARGTYPE str1, __in INARGTYPE str2 )
	{
		return( T::StrTraits::StringCompare( str1, str2 ) );
	}
};

#pragma pop_macro("new")

#if defined(_AFX)
#pragma pop_macro("FormatMessage")
#endif

};  // namespace ATL



#ifdef __ATLCOMCLI_H__
#include <cstringt.inl>
#endif	// __ATLCOMCLI_H__


#ifndef _ATL_NO_PRAGMA_WARNINGS
#pragma warning (pop)
#endif



#endif	// __CSTRINGT_H__ (whole file)


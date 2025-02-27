/*
build.c - returns a engine build number
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common.h"

//#define XASH_GENERATE_BUILDNUM

#if defined(XASH_GENERATE_BUILDNUM)
static const char *date = __DATE__;
static const char *mon[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static char mond[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
#endif

#define YEAR (\
(__DATE__ [7] - '0') * 1000 + \
(__DATE__ [8] - '0') * 100 + \
(__DATE__ [9] - '0') * 10 + \
(__DATE__ [10] - '0') * 1 \
)

#define MONTH (\
  __DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
: __DATE__ [2] == 'b' ? 2 \
: __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
: __DATE__ [2] == 'y' ? 5 \
: __DATE__ [2] == 'l' ? 7 \
: __DATE__ [2] == 'g' ? 8 \
: __DATE__ [2] == 'p' ? 9 \
: __DATE__ [2] == 't' ? 10 \
: __DATE__ [2] == 'v' ? 11 \
: 12)

#define DAY ( \
(__DATE__ [4] == ' ' ? 0 : (__DATE__ [4] - '0') * 10) + \
(__DATE__ [5] - '0') * 1 \
)

#define BUILD (std::type_identity_t<int[]>{ 0, 0,  31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 }[MONTH] + (DAY - 1) + (YEAR - 1900 - 1) * 365.25 + (YEAR % 4 == 0 && MONTH >= 2) - 41728)

constexpr int g_iBuildNum = BUILD;

extern "C" const char SO_FILE_VERSION[5]
#if __ANDROID__
	__attribute__ ((section (".bugly_version")))
#endif
= { ((g_iBuildNum / 1000) % 10) + '0', ((g_iBuildNum / 100) % 10) + '0', ((g_iBuildNum / 10) % 10) + '0', ((g_iBuildNum / 1) % 10) + '0', '\0' };

// returns days since Feb 13 2007
int Q_buildnum( void )
{
// do not touch this! Only author of Xash3D can increase buildnumbers!
// Xash3D SDL: HAHAHA! I TOUCHED THIS!
#if defined(XASH_GENERATE_BUILDNUM)
	int m = 0, d = 0, y = 0;
	static int b = 0;

	if( b != 0 ) return b;

	for( m = 0; m < 11; m++ )
	{
		if( !Q_strnicmp( &date[0], mon[m], 3 ))
			break;
		d += mond[m];
	}

	d += Q_atoi( &date[4] ) - 1;
	y = Q_atoi( &date[7] ) - 1900;
	b = d + (int)((y - 1) * 365.25f );

	if((( y % 4 ) == 0 ) && m > 1 )
	{
		b += 1;
	}
	//b -= 38752; // Feb 13 2007
	b -= 41728; // Apr 1 2015. Date of first release of crossplatform Xash3D

	return b;
#else
	return g_iBuildNum; // Aug 13 2016
#endif
}

/*
============
Q_buildos

Returns current name of operating system. Without any spaces.
============
*/
const char *Q_buildos( void )
{
	const char *osname;

#if defined(_WIN32) && defined(XASH_WINRT)
	osname = "WinRT";
#elif defined(_WIN32) && defined(_MSC_VER)
	osname = "Win32";
#elif defined(_WIN32) && defined(__MINGW32__)
	osname = "Win32-MinGW";
#elif defined(__ANDROID__)
	osname = "Android";
#elif defined(__SAILFISH__)
	osname = "SailfishOS";
#elif defined(__HAIKU__)
	osname = "HaikuOS";
#elif defined(__linux__)
	osname = "Linux";
#elif defined(__APPLE__) && ( TARGET_IPHONE_SIMULATOR )
	osname = "iOS-Simulator";
#elif defined(__APPLE__) && ( TARGET_OS_IOS || TARGET_OS_IPHONE )
	osname = "iOS";
#elif defined(__APPLE__) && ( TARGET_OS_OSX || TARGET_OS_MAC )
	osname = "macOS";
#elif defined(__APPLE__)
	osname = "Apple";
#elif defined(__FreeBSD__)
	osname = "FreeBSD";
#elif defined(__NetBSD__)
	osname = "NetBSD";
#elif defined(__OpenBSD__)
	osname = "OpenBSD";
#elif defined __EMSCRIPTEN__
	osname = "emscripten";
#else
#error "Place your operating system name here! If this is a mistake, try to fix conditions above and report a bug"
#endif
	
	return osname;
}

/*
============
Q_buildos

Returns current name of operating system. Without any spaces.
============
*/
const char *Q_buildarch( void )
{
	const char *archname;

#if defined( __x86_64__) || defined(_M_X64)
	archname = "amd64";
#elif defined(__i386__) || defined(_X86_) || defined(_M_IX86)
	archname = "i386";
#elif defined(__aarch64__) || defined(_M_ARM64)
	archname = "arm64";
#elif defined __arm__ || defined _M_ARM
	archname = "arm";
#elif defined __mips__
	archname = "mips";
#elif defined __EMSCRIPTEN__
	archname = "javascript";
#else
#error "Place your architecture name here! If this is a mistake, try to fix conditions above and report a bug"
#endif
	
	return archname;
}

/*
=============
Q_buildcommit

Returns a short hash of current commit in VCS as string.
XASH_BUILD_COMMIT must be passed in quotes

if XASH_BUILD_COMMIT is not defined,
Q_buildcommit will identify this build as release or "notset"
=============
*/
const char *Q_buildcommit( void )
{
#ifdef XASH_BUILD_COMMIT
	return XASH_BUILD_COMMIT;
#elif defined(XASH_RELEASE) // don't check it elsewhere to avoid random bugs
	return "release";
#else
	return "notset";
#endif
}

/*
=============
Q_buildnum_compat

Returns a Xash3D build number. This is left for compatibility with original Xash3D.
IMPORTANT: this value must be changed ONLY after updating to newer Xash3D
IMPORTANT: this value must be acquired through "build" cvar.
=============
*/
int Q_buildnum_compat( void )
{
	return 3366;
}

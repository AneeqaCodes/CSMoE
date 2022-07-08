//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================
#pragma once
#if !defined( IN_DEFSH )
#define IN_DEFSH

#include "angledef.h"

#ifdef WINAPI_FAMILY
#if (!WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP))
#define XASH_WINRT
#endif
#endif

#if defined(_WIN32) && !defined(XASH_WINRT)
#include <WinUser.h>
#else
#ifndef PORT_H
typedef struct tagPOINT{
	int x;
	int y;
} POINT;
#endif
inline void GetCursorPos(...) {}
inline void SetCursorPos(...) {}
#endif

#endif

// $Id$
// Copyright (c) 2009 Oliver Lau <oliver@von-und-fuer-lau.de>
// Alle Rechte vorbehalten.

#ifndef __COMPAT_WIN32_H_
#define __COMPAT_WIN32_H_

#ifdef _WIN32

#if _MSC_VER > 1000
#pragma once
#endif

/// Detect if the running Windows version supports the rand_s() API call.
/// @return true if rand_s() is supported, false otherwise
extern bool hasRand_s(void);

#endif // _WIN32

#endif // __COMPAT_WIN32_H_

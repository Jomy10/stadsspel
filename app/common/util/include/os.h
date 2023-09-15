/******************************************************************************
 * os.h
 *
 * Authors: Jonas Everaert
 *
 * Detect OS
******************************************************************************/

// http://web.archive.org/web/20160306052035/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system#Howtolistpredefinedmacros
// Apple: Availability.h for OS versions (see usage https://opensource.apple.com/source/Libc/Libc-1081.1.3/include/regex.h.auto.html)

#ifndef _OS_DEF_H
#define _OS_DEF_H

#define osAIX 0
#define osDragonFlyBSD 0
#define osFreeBSD 0
#define osNetBSD 0
#define osOpenBSD 0
#define osBSD 0
#define osHP_UX 0
#define osLinux 0
#define osApple 0
#define osiOSSimulator 0
#define osiOS 0
#define osMac 0
#define osSolaris 0
#define osCygwin 0
#define osWindows 0
#define osWin64 0
#define osWin32 0
#define isUnix 0
#define isPOSIX 0

#ifdef _AIX
#undef osAIX
#define osAIX 1
#endif


#ifdef __DragonFly
#undef osDragonFlyBSD
#define osDragonFlyBSD 1
#endif

#ifdef __FreeBSD__
#unde osFreeBSD
#define osFreeBSD 1
#endif

#ifdef __NetBSD__
#undef osNetBSD
#define osNetBSD 1
#endif

#ifdef __OpenBSD__
#undef osOpenBSD
#define osOpenBSD 1
#endif

#if osFreeBSD || osNetBSD || osOpenBSD || osDragonFlyBSD || defined(__bsdi__)
// any BSD type
#undef osBSD
#define osBSD 1
#endif


#ifdef __hpux
#undef osHP_UX
#define osHP_UX 1
#endif


#ifdef __linux__
#undef osLinux
#define osLinux 1
#endif


#if defined(__APPLE__) && defined(__MACH__)
  #undef osApple
  #define osApple 1
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR
    #undef osiOSSimulator
    #define osiOSSimulator 1
    #undef osiOS
    #define osiOS 1
  #elif TARGET_OS_IPHONE
    #undef osiOS
    #define osiOS 1
  #elif TARGET_OS_MAC
    #undef osMac
    #define osMac 1
  #endif
#endif


#if defined(__sun) && defined(__SVR4)
#undef osSolaris
#define osSolaris 1
#endif


#if defined(__CYGWIN__) && !defined(_WIN32)
// Cygwin POSIX under Microsoft Windows
#undef osCygwin
#define osCygwin 1
#endif


#if defined(_WIN32)
  #undef osWindows
  #define osWindows 1
  #if defined(_WIN64)
  #undef osWin64
  #define osWin64 1
  #elif defined(_WIN32)
  #undef osWin32
  #define osWin32 1
  #endif
#endif

// TODO: should include android?
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
  #undef isUnix
  #define isUnix 1
  #include <unistd.h>
  #if defined(_POSIX_VERSION)
    // Posix compliant
    #undef isPOSIX
    #define isPOSIX 1
  #endif
#endif

#endif

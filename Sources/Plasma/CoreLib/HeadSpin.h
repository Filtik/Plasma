/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/
#ifndef HeadSpinHDefined
#define HeadSpinHDefined

#if (defined(_DEBUG) || defined(UNIX_DEBUG))
#   define HS_DEBUGGING
#endif // defined(_DEBUG) || defined(UNIX_DENUG)

//======================================
// Some standard includes
//======================================
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdarg>
#include <cstdint>

//======================================
// Winblows Hacks
//======================================
#ifdef HS_BUILD_FOR_WIN32
    // 4244: Conversion
    // 4305: Truncation
    // 4503: 'identifier' : decorated name length exceeded, name was truncated
    // 4018: signed/unsigned mismatch
    // 4786: 255 character debug limit
    // 4284: STL template defined operator-> for a class it doesn't make sense for (int, etc)
    // 4800: 'int': forcing value to bool 'true' or 'false' (performance warning)
#   ifdef _MSC_VER
#      pragma warning( disable : 4305 4503 4018 4786 4284 4800)
#   endif // _MSC_VER

    // Kind of nasty looking forward declarations, but this is Win32.... it'll never change!
    // If you want to argue: would you rather pull in the entire Windows.h? Windows 8 makes it
    // even more bloated than before!
    struct HWND__; typedef struct HWND__ *HWND;
    struct HINSTANCE__; typedef struct HINSTANCE__ *HINSTANCE;

    typedef HWND hsWindowHndl;
    typedef HINSTANCE hsWindowInst;
    typedef HINSTANCE HMODULE;
    typedef long HRESULT;
    typedef void* HANDLE;
#else
    typedef int32_t* hsWindowHndl;
    typedef int32_t* hsWindowInst;
#endif // HS_BUILD_FOR_WIN32

//======================================
// Basic macros
//======================================
#ifdef __cplusplus
    #define hsCTypeDefStruct(foo)
#else
    #define hsCTypeDefStruct(foo)       typedef struct foo foo;
#endif

#ifdef HS_BUILD_FOR_WIN32
#    ifndef CDECL
#        define CDECL __cdecl
#    endif
#else
#   define CDECL
#endif

#define kPosInfinity16      (32767)
#define kNegInfinity16      (-32768)

#define kPosInfinity32      (0x7fffffff)
#define kNegInfinity32      (0x80000000)

#ifndef M_PI
#   define M_PI       3.14159265358979323846
#endif

#ifndef nil
#   define nil (nullptr)
#endif

typedef int32_t   hsError;
typedef uint32_t  hsGSeedValue;

#define hsOK                0
#define hsFail              -1
#define hsFailed(r)         ((hsError)(r)<hsOK)
#define hsSucceeded(r)      ((hsError)(r)>=hsOK)

#define hsLongAlign(n)      (((n) + 3) & ~3L)

#define hsMaximum(a, b)     ((a) > (b) ? (a) : (b))
#define hsMinimum(a, b)     ((a) < (b) ? (a) : (b))
#define hsABS(x)            ((x) < 0 ? -(x) : (x))
#define hsSGN(x)            (((x) < 0) ? -1 : ( ((x) > 0) ? 1 : 0 ))

#define hsBitTst2Bool(value, mask)      (((value) & (mask)) != 0)

#define hsFourByteTag(a, b, c, d)       (((uint32_t)(a) << 24) | ((uint32_t)(b) << 16) | ((uint32_t)(c) << 8) | (d))


//======================================
// Endian swap funcitions
//======================================
inline uint16_t hsSwapEndian16(uint16_t value)
{
    return (value >> 8) | (value << 8);
}
inline uint32_t hsSwapEndian32(uint32_t value)
{
    return ((value)              << 24) | 
            ((value & 0x0000ff00) << 8)  |
            ((value & 0x00ff0000) >> 8)  |
            ((value)              >> 24);
}
inline uint64_t hsSwapEndian64(uint64_t value)
{
    return ((value)                      << 56) |
            ((value & 0x000000000000ff00) << 40) |
            ((value & 0x0000000000ff0000) << 24) |
            ((value & 0x00000000ff000000) << 8)  |
            ((value & 0x000000ff00000000) >> 8)  |
            ((value & 0x0000ff0000000000) >> 24) |
            ((value & 0x00ff000000000000) >> 40) |
            ((value)                      >> 56);
}
inline float hsSwapEndianFloat(float fvalue)
{
    uint32_t value = *(uint32_t*)&fvalue;
    value = hsSwapEndian32(value);
    return *(float*)&value;
}
inline double hsSwapEndianDouble(double dvalue)
{
    uint64_t value = *(uint64_t*)&dvalue;
    value = hsSwapEndian64(value);
    return *(double*)&value;
}

#if LITTLE_ENDIAN
    #define hsToBE16(n)         hsSwapEndian16(n)
    #define hsToBE32(n)         hsSwapEndian32(n)
    #define hsToBE64(n)         hsSwapEndian64(n)
    #define hsToBEFloat(n)      hsSwapEndianFloat(n)
    #define hsToBEDouble(n)     hsSwapEndianDouble(n)
    #define hsToLE16(n)         (n)
    #define hsToLE32(n)         (n)
    #define hsToLE64(n)         (n)
    #define hsToLEFloat(n)      (n)
    #define hsToLEDouble(n)     (n)
#else
    #define hsToBE16(n)         (n)
    #define hsToBE32(n)         (n)
    #define hsToBE64(n)         (n)
    #define hsToBEFloat(n)      (n)
    #define hsToBEDouble(n)     (n)
    #define hsToLE16(n)         hsSwapEndian16(n)
    #define hsToLE32(n)         hsSwapEndian32(n)
    #define hsToLE64(n)         hsSwapEndian64(n)
    #define hsToLEFloat(n)      hsSwapEndianFloat(n)
    #define hsToLEDouble(n)     hsSwapEndianDouble(n)
#endif

inline void hsSwap(int32_t& a, int32_t& b)
{
    int32_t   c = a;
    a = b;
    b = c;
}

inline void hsSwap(uint32_t& a, uint32_t& b)
{
    uint32_t  c = a;
    a = b;
    b = c;
}

inline void hsSwap(float& a, float& b)
{
    float   c = a;
    a = b;
    b = c;
}

//===========================================================================
// Define a NOOP (null) statement
//===========================================================================
#ifdef _MSC_VER
# define  NULL_STMT  __noop
#else
# define  NULL_STMT  ((void)0)
#endif

//===========================================================================
template<class T>
inline T max (const T & a, const T & b) {
    return (a > b) ? a : b;
}

//===========================================================================
inline unsigned max (int a, unsigned b) {
    return ((unsigned)a > b) ? a : b;
}

//===========================================================================
inline unsigned max (unsigned a, int b) {
    return (a > (unsigned)b) ? a : b;
}

//===========================================================================
template<class T>
inline T min (const T & a, const T & b) {
    return (a < b) ? a : b;
}

//===========================================================================
inline unsigned min (int a, unsigned b) {
    return ((unsigned)a < b) ? a : b;
}

//===========================================================================
inline unsigned min (unsigned a, int b) {
    return (a < (unsigned)b) ? a : b;
}


/****************************************************************************
*
*   MAX/MIN macros
*   These are less safe than the inline function versions, since they
*   evaluate parameters twice. However, they can be used to produce
*   compile-time constants.
*
***/
#define  MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define  MIN(a, b)  (((a) < (b)) ? (a) : (b))


/****************************************************************************
*
*   SWAP
*   Swaps the values of two variables
*
***/

//===========================================================================
template<class T>
void SWAP (T & a, T & b) {
    T temp = a;
    a = b;
    b = temp;
}


/****************************************************************************
*
*   AUTO_INIT_FUNC
*   Declares a function that is automatically called at program startup time
*
*   Example:
*
*   AUTO_INIT_FUNC(BuildLookupTables) {
*       ...
*   }
*
***/

#define AUTO_INIT_FUNC(name)  namespace { struct name { name (); } name##_instance; } name::name ()


/****************************************************************************
*
*   arrsize
*   arrsize returns the number of elements in an array variable
*
*   Example:
*
*   StrPrintf(buffer, arrsize(buffer), "%u", value);
*
***/
#define  arrsize(a)     (sizeof(a) / sizeof((a)[0]))


/****************************************************************************
*
*   IS_POW2
*
***/
#define IS_POW2(val) (!((val) & ((val) - 1)))

#ifdef PLASMA_EXTERNAL_RELEASE
#   define hsStatusMessage(x)                  NULL_STMT
#   define hsStatusMessageF(x, ...)            NULL_STMT
#else
    void    hsStatusMessage(const char* message);
    void    hsStatusMessageF(const char * fmt, ...);
#endif // PLASMA_EXTERNAL_RELEASE

char*   hsStrcpy(char* dstOrNil, const char* src);
void    hsStrLower(char *s);

inline char* hsStrcpy(const char* src)
{
    return hsStrcpy(nil, src);
}

inline char *hsStrncpy(char *strDest, const char *strSource, size_t count)
{
    char *temp = strncpy(strDest, strSource, count-1);
    strDest[count-1] = 0;
    return temp;
}

wchar_t *hsStringToWString( const char *str );
char    *hsWStringToString( const wchar_t *str );

enum {              // Kind of MessageBox...passed to hsMessageBox
    hsMessageBoxAbortRetyIgnore,
    hsMessageBoxNormal,             // Just Ok
    hsMessageBoxOkCancel,
    hsMessageBoxRetryCancel,
    hsMessageBoxYesNo,
    hsMessageBoxYesNoCancel,
};

enum {
    hsMessageBoxIconError,
    hsMessageBoxIconQuestion,
    hsMessageBoxIconExclamation,
    hsMessageBoxIconAsterisk,
};

enum {          // RETURN VALUES FROM hsMessageBox
    hsMBoxOk = 1,       // OK button was selected. 
    hsMBoxCancel,   // Cancel button was selected. 
    hsMBoxAbort,    // Abort button was selected. 
    hsMBoxRetry,    // Retry button was selected. 
    hsMBoxIgnore,   // Ignore button was selected. 
    hsMBoxYes,      // Yes button was selected. 
    hsMBoxNo        // No button was selected. 
};

extern bool hsMessageBox_SuppressPrompts;
int hsMessageBox(const char* message, const char* caption, int kind, int icon=hsMessageBoxIconAsterisk);
int hsMessageBox(const wchar_t* message, const wchar_t* caption, int kind, int icon=hsMessageBoxIconAsterisk);
int hsMessageBoxWithOwner(hsWindowHndl owner, const char* message, const char* caption, int kind, int icon=hsMessageBoxIconAsterisk);
int hsMessageBoxWithOwner(hsWindowHndl owner, const wchar_t* message, const wchar_t* caption, int kind, int icon=hsMessageBoxIconAsterisk);

// flag testing / clearing
#define hsCheckBits(f,c) ((f & c)==c)
#define hsTestBits(f,b)  ( (f) & (b)   )
#define hsSetBits(f,b)   ( (f) |= (b)  )
#define hsClearBits(f,b) ( (f) &= ~(b) )
#define hsToggleBits(f,b) ( (f) ^= (b) )
#define hsChangeBits(f,b,t) ( t ? hsSetBits(f,b) : hsClearBits(f,b) )


#if HS_BUILD_FOR_WIN32
     // This is for Windows
#    define hsVsnprintf     _vsnprintf
#    define hsVsnwprintf    _vsnwprintf
#    define hsSnprintf      _snprintf
#    define hsSnwprintf     _snwprintf

#    define snprintf        _snprintf
#    define snwprintf       _snwprintf
#    define swprintf        _snwprintf

#    ifndef fileno
#        define fileno(__F)       _fileno(__F)
#    endif

#   define hsWFopen(name, mode)     _wfopen(name, mode)
#else
     // This is for Unix, Linux, OSX, etc.
#    define hsVsnprintf     vsnprintf
#    define hsVsnwprintf    vswprintf
#    define hsSnprintf      snprintf
#    define hsSnwprintf     swprintf

#   define hsWFopen(name, mode)     fopen(hsWStringToString(name), hsWStringToString(mode))

#   include <limits.h>
#   define MAX_PATH PATH_MAX
#endif
#define MAX_EXT     (256)

// Useful floating point utilities
inline float hsDegreesToRadians(float deg) { return float(deg * (M_PI / 180)); }
inline float hsRadiansToDegrees(float rad) { return float(rad * (180 / M_PI)); }
#define hsInvert(a) (1 / (a))

#ifdef _MSC_VER
#   define ALIGN(n) __declspec(align(n))
#else
#   define ALIGN(n) __atribute__(aligned(n))
#endif

#define hsFopen(name, mode) fopen(name, mode)

/************************ Debug/Error Macros **************************/

typedef void (*hsDebugMessageProc)(const char message[]);
extern hsDebugMessageProc gHSDebugProc;
#define HSDebugProc(m)  { if (gHSDebugProc) gHSDebugProc(m); }
hsDebugMessageProc hsSetDebugMessageProc(hsDebugMessageProc newProc);

extern hsDebugMessageProc gHSStatusProc;
hsDebugMessageProc hsSetStatusMessageProc(hsDebugMessageProc newProc);

void ErrorEnableGui (bool enabled);
void ErrorAssert (int line, const char* file, const char* fmt, ...);

bool DebugIsDebuggerPresent ();
void DebugBreakIfDebuggerPresent ();
void DebugMsg(const char* fmt, ...);

#ifdef HS_DEBUGGING
    
    void    hsDebugMessage(const char* message, long refcon);
    #define hsDebugCode(code)                   code
    #define hsIfDebugMessage(expr, msg, ref)    (void)( ((expr) != 0) || (hsDebugMessage(msg, ref), 0) )
    #define hsAssert(expr, msg)                 (void)( ((expr) != 0) || (ErrorAssert(__LINE__, __FILE__, msg), 0) )
    #define ASSERT(expr)                        (void)( ((expr) != 0) || (ErrorAssert(__LINE__, __FILE__, #expr), 0) )
    #define ASSERTMSG(expr, msg)                (void)( ((expr) != 0) || (ErrorAssert(__LINE__, __FILE__, msg), 0) )
    #define FATAL(msg)                          ErrorAssert(__LINE__, __FILE__, msg)
    #define DEBUG_MSG                           DebugMsg
    #define DEBUG_BREAK_IF_DEBUGGER_PRESENT     DebugBreakIfDebuggerPresent
    
#else   /* Not debugging */

    #define hsDebugMessage(message, refcon)     NULL_STMT
    #define hsDebugCode(code)                   /* empty */
    #define hsIfDebugMessage(expr, msg, ref)    NULL_STMT
    #define hsAssert(expr, msg)                 NULL_STMT
    #define ASSERT(expr)                        NULL_STMT
    #define ASSERTMSG(expr, msg)                NULL_STMT
    #define FATAL(msg)                          NULL_STMT
    #define DEBUG_MSG                           (void)
    #define DEBUG_MSGV                          NULL_STMT
    #define DEBUG_BREAK_IF_DEBUGGER_PRESENT     NULL_STMT

#endif  // HS_DEBUGGING


#ifdef _MSC_VER
#define  DEFAULT_FATAL(var)  default: FATAL("No valid case for switch variable '" #var "'"); __assume(0); break;
#else
#define  DEFAULT_FATAL(var)  default: FATAL("No valid case for switch variable '" #var "'"); break;
#endif

/*****************************************************************************
*
*  Atomic Operations
*  FIXME: Replace with std::atomic when VS2012 supports WinXP
*
***/

#ifdef _MSC_VER
#   define AtomicAdd(value, increment) InterlockedExchangeAdd(value, increment)
#   define AtomicSet(value, set) InterlockedExchange(value, set)
#elif __GNUC__
#   define AtomicAdd(value, increment) __sync_fetch_and_add(value, increment)
#   define AtomicSet(value, set) __sync_lock_test_and_set(value, set)
#endif

#endif

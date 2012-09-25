/**
 * @file platform_config.h
 *
 * The platform-specific configuration for BCSLib
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTARR_PLATFORM_CONFIG_H
#define LIGHTARR_PLATFORM_CONFIG_H

#define LIGHTARR_MSVC 0x01
#define LIGHTARR_GCC 0x02
#define LIGHTARR_CLANG 0x03

#define LIGHTARR_WIN32 0x11
#define LIGHTARR_POSIX 0x12

#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
	#if _MSC_VER < 1600
		#error Microsoft Visual C++ of version lower than MSVC 2010 is not supported.
	#endif
	#define LIGHTARR_COMPILER LIGHTARR_MSVC

	#define LIGHTARR_PLATFORM LIGHTARR_WIN32

	#define LARR_USE_C11_STDLIB
	#define LARR_USE_STATIC_ASSERT
	#define LARR_HAS_NULLPTR
	#define LARR_HAS_DECLTYPE

#elif (defined(__GNUC__))

	#if (defined(__clang__))
		#if ((__clang_major__ < 2) || (__clang_major__ == 2 && __clang_minor__ < 9))
			#error CLANG of version lower than 2.9.0 is not supported
		#endif
		#define LIGHTARR_COMPILER LIGHTARR_CLANG

		#define LARR_USE_C11_STDLIB
		#define LARR_USE_STATIC_ASSERT
		#define LARR_HAS_CXX11_MATH
		#define LARR_HAS_DECLTYPE

		#if (__clang_major__ >= 3)
			#define LARR_HAS_NULLPTR
		#endif

	#else
		#if ((__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 2))
			#error GCC of version lower than 4.2.0 is not supported
		#endif
		#define LIGHTARR_COMPILER LIGHTARR_GCC

		#if (defined(__GXX_EXPERIMENTAL_CXX0X__))
			#define LARR_USE_C11_STDLIB
			#define LARR_USE_STATIC_ASSERT
			#define LARR_HAS_CXX11_MATH
			#define LARR_HAS_DECLTYPE

			#if (__GNUC__ == 4 && __GNUC_MINOR >= 6)
				#define LARR_HAS_NULLPTR
			#endif
		#endif
	#endif

	#define LIGHTARR_PLATFORM LIGHTARR_POSIX

#else
	#error Light-Matrix can only be used with Microsoft Visual C++, GCC (G++), or clang (clang++).
#endif


#ifdef LARR_USE_C11_STDLIB
	#define LARR_TR1 std
#else
	#define LARR_TR1 std::tr1
#endif


#endif


/**
 * @file lang_base.h
 *
 * @brief Basic language support facilities.
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTARR_LANG_BASE_H_
#define LIGHTARR_LANG_BASE_H_

#include <light_array/config/config.h>

// useful macros

#if LIGHTARR_PLATFORM == LIGHTARR_POSIX

	#define LARR_ALIGN(a) __attribute__((aligned(a)))
	#define LARR_ENSURE_INLINE __attribute__((always_inline))

#elif LIGHTARR_PLATFORM == LIGHTARR_WIN32

	#define LARR_ALIGN(a) __declspec(align(a))
	#define LARR_ENSURE_INLINE __forceinline

#endif

#ifdef LARR_HAS_NULLPTR
#define LARR_NULL nullptr
#else
#define LARR_NULL NULL
#endif

#define LARR_CRTP_REF \
		LARR_ENSURE_INLINE const Derived& derived() const { return *(static_cast<const Derived*>(this)); } \
		LARR_ENSURE_INLINE Derived& derived() { return *(static_cast<Derived*>(this)); }

namespace larr
{
	/**
	 * @brief The base class to ensure derived classes to be non-copyable.
	 */
	class noncopyable
	{
	protected:
		noncopyable() { }
		~noncopyable() { }

	private:
		noncopyable(const noncopyable& );
		noncopyable& operator= (const noncopyable& );
	};

}

#endif

/**
 * @file mem_alloc.h
 *
 * @brief Facilities for aligned memory allocation.
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTARR_MEM_ALLOC_H_
#define LIGHTARR_MEM_ALLOC_H_

#include <light_array/common/basic_defs.h>

#if LIGHTARR_PLATFORM == LIGHTARR_POSIX
#include <stdlib.h>
#elif LIGHTARR_PLATFORM == LIGHTARR_WIN32
#include <malloc.h>
#endif

#include <new>
#include <limits>

#define LARR_DEFAULT_ALIGNMENT 16

namespace larr
{

#if LIGHTARR_PLATFORM == LIGHTARR_POSIX

	LARR_ENSURE_INLINE
	inline void* aligned_allocate(size_t nbytes, unsigned int alignment)
	{
		char* p = 0;
		if (::posix_memalign((void**)(&p), alignment, nbytes) != 0)
		{
			throw std::bad_alloc();
		}
		return p;
	}

	LARR_ENSURE_INLINE
	inline void aligned_release(void *p)
	{
		::free(p);
	}

#elif LIGHTARR_PLATFORM == LIGHTARR_WIN32

	LARR_ENSURE_INLINE
	inline void* aligned_allocate(size_t nbytes, unsigned int alignment)
	{
		void* p = ::_aligned_malloc(nbytes, alignment));
		if (!p)
		{
			throw std::bad_alloc();
		}
		return p;
	}

	LARR_ENSURE_INLINE
	inline void aligned_release(void *p)
	{
		::_aligned_free(p);
	}

#endif


    template<typename T, unsigned int Align=LARR_DEFAULT_ALIGNMENT>
    class aligned_allocator
    {
    public:
    	typedef T value_type;
    	typedef T* pointer;
    	typedef T& reference;
    	typedef const T* const_pointer;
    	typedef const T& const_reference;
    	typedef size_t size_type;
    	typedef ptrdiff_t difference_type;

    	template<typename TOther>
    	struct rebind
    	{
    		typedef aligned_allocator<TOther> other;
    	};

    public:
    	LARR_ENSURE_INLINE
    	aligned_allocator() { }

    	template<typename U>
    	LARR_ENSURE_INLINE
    	aligned_allocator(const aligned_allocator<U>& r) { }

    	LARR_ENSURE_INLINE
    	unsigned int alignment() const
    	{
    		return Align;
    	}

    	LARR_ENSURE_INLINE
    	pointer address( reference x ) const
    	{
    		return &x;
    	}

    	LARR_ENSURE_INLINE
    	const_pointer address( const_reference x ) const
    	{
    		return &x;
    	}

    	LARR_ENSURE_INLINE
    	size_type max_size() const
    	{
    		return std::numeric_limits<size_type>::max() / sizeof(value_type);
    	}

    	LARR_ENSURE_INLINE
    	pointer allocate(size_type n, const void* hint=0)
    	{
    		return (pointer)aligned_allocate(n * sizeof(value_type), Align);
    	}

    	LARR_ENSURE_INLINE
    	void deallocate(pointer p, size_type)
    	{
    		aligned_release(p);
    	}

    	LARR_ENSURE_INLINE
    	void construct (pointer p, const_reference val)
    	{
    		new (p) value_type(val);
    	}

    	LARR_ENSURE_INLINE
    	void destroy (pointer p)
    	{
    		p->~value_type();
    	}

    }; // end class aligned_allocator

}

#endif

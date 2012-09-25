/**
 * @file block.h
 *
 * Memory block classes
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTARR_BLOCK_H_
#define LIGHTARR_BLOCK_H_

#include <light_array/common/mem_op.h>
#include <light_array/common/mem_alloc.h>

#include <algorithm>

namespace larr
{
	/**
	 * @brief The interface for blocks.
	 */
	template<class Derived, typename T>
	class IBlock
	{
	public:
		typedef T value_type;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;

		typedef index_t index_type;

		LARR_CRTP_REF

	public:
		LARR_ENSURE_INLINE size_type size() const
		{
			return (size_type)(derived().nelems());
		}

		LARR_ENSURE_INLINE index_type nelems() const
		{
			return derived().nelems();
		}

		LARR_ENSURE_INLINE const_pointer ptr_begin() const
		{
			return derived().ptr_begin();
		}

		LARR_ENSURE_INLINE pointer ptr_begin()
		{
			return derived().ptr_begin();
		}

		LARR_ENSURE_INLINE const_pointer ptr_end() const
		{
			return derived().ptr_end();
		}

		LARR_ENSURE_INLINE pointer ptr_end()
		{
			return derived().ptr_end();
		}

		LARR_ENSURE_INLINE const_reference operator[] (const index_type i) const
		{
			return derived().operator[](i);
		}

		LARR_ENSURE_INLINE reference operator[] (const index_type i)
		{
			return derived().operator[](i);
		}

	}; // end class IBlock


	// Operations on blocks

	template<typename T, class Derived>
	LARR_ENSURE_INLINE
	inline void copy(const T* src, IBlock<Derived, T>& a)
	{
		copy_mem(a.nelems(), src, a.ptr_begin());
	}

	template<typename T, class Derived>
	LARR_ENSURE_INLINE
	void copy(const IBlock<Derived, T>& a, T* dst)
	{
		copy_mem(a.nelems(), a.ptr_begin(), dst);
	}

	template<typename T, class Derived>
	LARR_ENSURE_INLINE
	void fill(IBlock<Derived, T>& a, const T& v)
	{
		fill_mem(a.nelems(), a.ptr_begin(), v);
	}

	template<typename T, class Derived>
	LARR_ENSURE_INLINE
	void zero(IBlock<Derived, T>& a)
	{
		zero_mem(a.nelems(), a.ptr_begin());
	}

	template<typename T, class LDerived, class RDerived>
	LARR_ENSURE_INLINE
	inline bool operator == (const IBlock<LDerived, T>& a, const IBlock<RDerived, T>& b)
	{
		return a.nelems() == b.nelems() && mem_equal(a.nelems(), a.ptr_begin(), b.ptr_begin());
	}

	template<typename T, class LDerived, class RDerived>
	LARR_ENSURE_INLINE
	inline bool operator != (const IBlock<LDerived, T>& a, const IBlock<RDerived, T>& b)
	{
		return !(a == b);
	}

	template<typename T, class Derived>
	LARR_ENSURE_INLINE
	inline bool elems_equal(const IBlock<Derived, T>& B, const T& v)
	{
		return mem_equal(B.nelems(), B.ptr_begin(), v);
	}

	template<typename T, class Derived>
	LARR_ENSURE_INLINE
	inline bool elems_equal(const IBlock<Derived, T>& B, const T* r)
	{
		return mem_equal(B.nelems(), B.ptr_begin(), r);
	}


    /********************************************
     *
     *   Dynamic block
     *
     ********************************************/

	template<typename T, typename Allocator=aligned_allocator<T> >
	class dblock : public IBlock<dblock<T, Allocator>,  T>
	{
	public:
		typedef T value_type;
		typedef Allocator allocator_type;

		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::const_reference const_reference;

		typedef index_t index_type;

	public:
		LARR_ENSURE_INLINE
		explicit dblock(const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(0)
		, m_ptr(LARR_NULL)
		{
		}

		LARR_ENSURE_INLINE
		explicit dblock(index_type len, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(len)
		, m_ptr(alloc(len))
		{
		}

		LARR_ENSURE_INLINE
		dblock(index_type len, const value_type& v, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(len)
		, m_ptr(alloc(len))
		{
			fill_mem(len, m_ptr, v);
		}

		LARR_ENSURE_INLINE
		dblock(index_type len, const_pointer src, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(len)
		, m_ptr(alloc(len))
		{
			copy_mem(m_len, src, m_ptr);
		}

		LARR_ENSURE_INLINE
		dblock(const dblock& s, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(s.nelems())
		, m_ptr(alloc(m_len))
		{
			copy_mem(m_len, s.ptr_begin(), m_ptr);
		}

		template<class OtherDerived>
		LARR_ENSURE_INLINE
		dblock(const IBlock<OtherDerived, T>& s, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(s.nelems())
		, m_ptr(alloc(m_len))
		{
			copy_mem(m_len, s.ptr_begin(), m_ptr);
		}

		LARR_ENSURE_INLINE
		~dblock()
		{
			dealloc(m_ptr);
			m_ptr = LARR_NULL;  // make sure that it alarms when access after destructed
		}

		LARR_ENSURE_INLINE
		void swap(dblock& r)
		{
			using std::swap;

			swap(m_allocator, r.m_allocator);
			swap(m_len, r.m_len);
			swap(m_ptr, r.m_ptr);
		}

		LARR_ENSURE_INLINE
		dblock& operator = (const dblock& r)
		{
			if (this != &r) assign(r);
			return *this;
		}

		template<class OtherDerived>
		LARR_ENSURE_INLINE
		dblock& operator = (const IBlock<OtherDerived, T>& r)
		{
			assign(r);
			return *this;
		}

		LARR_ENSURE_INLINE
		const allocator_type& get_allocator() const
		{
			return m_allocator;
		}

		LARR_ENSURE_INLINE
		void resize(index_type n)
		{
			if (n != this->nelems())
			{
				reset_size(n);
			}
		}

	public:
		LARR_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_type>(m_len);
		}

		LARR_ENSURE_INLINE index_type nelems() const
		{
			return m_len;
		}

		LARR_ENSURE_INLINE const_pointer ptr_begin() const
		{
			return m_ptr;
		}

		LARR_ENSURE_INLINE pointer ptr_begin()
		{
			return m_ptr;
		}

		LARR_ENSURE_INLINE const_pointer ptr_end() const
		{
			return m_ptr + m_len;
		}

		LARR_ENSURE_INLINE pointer ptr_end()
		{
			return m_ptr + m_len;
		}

		LARR_ENSURE_INLINE
		const_reference operator[] (const index_type i) const
		{
			return m_ptr[i];
		}

		LARR_ENSURE_INLINE
		reference operator[] (const index_type i)
		{
			return m_ptr[i];
		}


	private:
		LARR_ENSURE_INLINE
		pointer alloc(index_type n)
		{
			return n > 0 ? m_allocator.allocate(size_type(n)) : LARR_NULL;
		}

		LARR_ENSURE_INLINE
		void dealloc(pointer p)
		{
			if (m_ptr) m_allocator.deallocate(p, (size_type)m_len);
		}

		LARR_ENSURE_INLINE
		void reset_size(index_type n)
		{
			pointer p = n > 0 ? m_allocator.allocate(size_type(n)) : LARR_NULL;
			dealloc(m_ptr);
			m_ptr = p;
			m_len = n;
		}

		template<class OtherDerived>
		LARR_ENSURE_INLINE
		void assign(const IBlock<OtherDerived, T>& r)
		{
			if (nelems() == r.nelems())  // no need to re-allocate memory
			{
				copy_mem(nelems(), r.ptr_begin(), this->ptr_begin());
			}
			else
			{
				dblock tmp(r);
				swap(tmp);
			}
		}

	private:
		allocator_type m_allocator;
		index_type m_len;
		pointer m_ptr;

	}; // end class dblock


	template<typename T, class Allocator>
	LARR_ENSURE_INLINE
	void swap(dblock<T, Allocator>& a, dblock<T, Allocator>& b)
	{
		a.swap(b);
	}


    /********************************************
     *
     *   Scoped Block
     *
     ********************************************/

	template<typename T, typename Allocator=aligned_allocator<T> >
	class scoped_block : public IBlock<scoped_block<T, Allocator>, T>, private noncopyable
	{
	public:
		typedef T value_type;
		typedef Allocator allocator_type;

		typedef typename allocator_type::size_type size_type;
		typedef typename allocator_type::difference_type difference_type;

		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef typename allocator_type::const_reference const_reference;

		typedef index_t index_type;

	public:
		LARR_ENSURE_INLINE
		explicit scoped_block(index_type len, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(len)
		, m_ptr(alloc(len))
		{
		}

		LARR_ENSURE_INLINE
		scoped_block(index_type len, const value_type& v, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(len)
		, m_ptr(alloc(len))
		{
			fill_mem(len, m_ptr, v);
		}

		LARR_ENSURE_INLINE
		scoped_block(index_type len, const_pointer src, const allocator_type& allocator = allocator_type())
		: m_allocator(allocator)
		, m_len(len)
		, m_ptr(alloc(len))
		{
			copy_mem(len, src, m_ptr);
		}

		LARR_ENSURE_INLINE
		~scoped_block()
		{
			dealloc(m_ptr);
		}

		LARR_ENSURE_INLINE
		const allocator_type& get_allocator() const
		{
			return m_allocator;
		}

	public:
		LARR_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_type>(m_len);
		}

		LARR_ENSURE_INLINE index_type nelems() const
		{
			return m_len;
		}

		LARR_ENSURE_INLINE const_pointer ptr_begin() const
		{
			return m_ptr;
		}

		LARR_ENSURE_INLINE pointer ptr_begin()
		{
			return m_ptr;
		}

		LARR_ENSURE_INLINE const_pointer ptr_end() const
		{
			return m_ptr + m_len;
		}

		LARR_ENSURE_INLINE pointer ptr_end()
		{
			return m_ptr + m_len;
		}

		LARR_ENSURE_INLINE
		const_reference operator[] (const index_type i) const
		{
			return m_ptr[i];
		}

		LARR_ENSURE_INLINE
		reference operator[] (const index_type i)
		{
			return m_ptr[i];
		}

	private:
		LARR_ENSURE_INLINE
		pointer alloc(index_type n)
		{
			return n > 0 ? m_allocator.allocate(size_type(n)) : LARR_NULL;
		}

		LARR_ENSURE_INLINE
		void dealloc(pointer p)
		{
			if (m_ptr) m_allocator.deallocate(p, (size_type)m_len);
		}

	private:
		allocator_type m_allocator;
		index_type m_len;
		pointer m_ptr;

	}; // end class scoped_block



    /********************************************
     *
     *   Static Block
     *
     ********************************************/

	template<typename T, int N>
	class sblock : public IBlock<sblock<T, N>, T>
	{
	public:
		typedef T value_type;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;

		typedef index_t index_type;

	public:
		LARR_ENSURE_INLINE
		explicit sblock()
		{
		}

		LARR_ENSURE_INLINE
		explicit sblock(const value_type& v)
		{
			fill_mem(N, m_arr, v);
		}

		LARR_ENSURE_INLINE
		explicit sblock(const_pointer src)
		{
			copy_mem(N, src, m_arr);
		}

		LARR_ENSURE_INLINE
		sblock(const sblock& src)
		{
			copy_mem(N, src.m_arr, m_arr);
		}

		template<class OtherDerived>
		LARR_ENSURE_INLINE
		sblock(const IBlock<OtherDerived, T>& src)
		{
			copy_mem(N, src.ptr_begin(), m_arr);
		}

		LARR_ENSURE_INLINE
		sblock& operator = (const sblock& src)
		{
			if (this != &src)
			{
				copy_mem(N, src.m_arr, m_arr);
			}
			return *this;
		}

		template<class OtherDerived>
		LARR_ENSURE_INLINE
		sblock& operator = (const IBlock<OtherDerived, T>& src)
		{
			copy_mem(N, src.ptr_begin(), m_arr);
			return *this;
		}

		LARR_ENSURE_INLINE
		void swap(sblock& r)
		{
			T tmp[N];
			copy_mem(N, m_arr, tmp);
			copy_mem(N, r.m_arr, m_arr);
			copy_mem(N, tmp, r.m_arr);
		}

	public:
		LARR_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_t>(N);
		}

		LARR_ENSURE_INLINE index_type nelems() const
		{
			return N;
		}

		LARR_ENSURE_INLINE const_pointer ptr_begin() const
		{
			return m_arr;
		}

		LARR_ENSURE_INLINE pointer ptr_begin()
		{
			return m_arr;
		}

		LARR_ENSURE_INLINE const_pointer ptr_end() const
		{
			return m_arr + N;
		}

		LARR_ENSURE_INLINE pointer ptr_end()
		{
			return m_arr + N;
		}

		LARR_ENSURE_INLINE
		const_reference operator[] (index_type i) const
		{
			return m_arr[i];
		}

		LARR_ENSURE_INLINE
		reference operator[] (index_type i)
		{
			return m_arr[i];
		}

	private:
		T m_arr[N];

	}; // end class sblock

	template<typename T, int N>
	LARR_ENSURE_INLINE
	void swap(sblock<T, N>& a, sblock<T, N>& b)
	{
		a.swap(b);
	}

}

#endif /* BLOCK_H_ */

/**
 * @file range.h
 *
 * The classes to represent ranges
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTARR_RANGE_H_
#define LIGHTARR_RANGE_H_

#include <light_array/common/basic_defs.h>

namespace larr
{
	template<class Derived>
	class IRange
	{
	public:
		LARR_CRTP_REF
	};


	struct whole : public IRange<whole>
	{
	};

	class range : public IRange<range>
	{
	public:
		LARR_ENSURE_INLINE
		range(const index_t i, const index_t n)
		: m_begin(i), m_num(n)
		{
		}

		LARR_ENSURE_INLINE index_t begin_index() const
		{
			return m_begin;
		}

		LARR_ENSURE_INLINE index_t end_index() const
		{
			return m_begin + m_num;
		}

		LARR_ENSURE_INLINE index_t num() const
		{
			return m_num;
		}

	private:
		const index_t m_begin;
		const index_t m_num;
	};


	class step_range : public IRange<step_range>
	{
	public:
		LARR_ENSURE_INLINE
		step_range(const index_t i, const index_t n, const index_t s)
		: m_begin(i), m_num(n), m_step(s)
		{
		}

		LARR_ENSURE_INLINE index_t begin_index() const
		{
			return m_begin;
		}

		LARR_ENSURE_INLINE index_t end_index() const
		{
			return m_begin + m_num * m_step;
		}

		LARR_ENSURE_INLINE index_t num() const
		{
			return m_num;
		}

		LARR_ENSURE_INLINE index_t step() const
		{
			return m_step;
		}

	private:
		const index_t m_begin;
		const index_t m_num;
		const index_t m_step;
	};


	/********************************************
	 *
	 *  MATLAB-like colon expression
	 *
	 *  colon(a, b) --> [a, b-1]
	 *
	 *  e.g. colon(0, n) --> the whole range
	 *
	 *  colon(a, s, b) --> [a, a+s, ..., b)
	 *
	 *  Here, b is not included
	 *
	 ********************************************/

	LARR_ENSURE_INLINE
	inline range colon(index_t a, index_t b)
	{
		return range(a, b-a);
	}

	LARR_ENSURE_INLINE
	inline step_range colon(index_t a, index_t s, index_t b)
	{
		index_t n = a <= b ?
				(s > 0 ? (b - a) / s : 0) :
				(s < 0 ? (a - b) / (-s) : 0);

		return step_range(a, n, s);
	}
}

#endif

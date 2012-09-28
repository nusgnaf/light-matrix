/**
 * @file array_fill.h
 *
 * Filling elements to arrays
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_FILL_H_
#define LIGHTMAT_ARRAY_FILL_H_

#include <light_mat/array/array_interf.h>
#include "bits/array_fill_internal.h"

namespace lmat
{
	template<typename T, class Arr>
	LMAT_ENSURE_INLINE
	void zero(IDenseArray<Arr, T>& dst)
	{
		typedef typename detail::array_filler<T,
				typename array_traits<Arr>::shape_type>::type filler_t;

		if (dst.has_continuous_layout())
		{
			filler_t::zero(dst.nrows(), dst.ncolumns(), dst.ptr_data());
		}
		else
		{
			filler_t::zero(dst.nrows(), dst.ncolumns(),
					dst.ptr_data(), dst.lead_dim());
		}
	}

	template<typename T, class Arr>
	LMAT_ENSURE_INLINE
	void fill(IDenseArray<Arr, T>& dst, const T& val)
	{
		typedef typename detail::array_filler<T,
				typename array_traits<Arr>::shape_type>::type filler_t;

		if (dst.has_continuous_layout())
		{
			filler_t::fill(val, dst.nrows(), dst.ncolumns(), dst.ptr_data());
		}
		else
		{
			filler_t::fill(val, dst.nrows(), dst.ncolumns(),
					dst.ptr_data(), dst.lead_dim());
		}
	}


	template<typename T, class DArr>
	LMAT_ENSURE_INLINE
	DArr& operator << (IDenseArray<DArr, T>& dmat, const T& v)
	{
		fill(dmat.derived(), v);
		return dmat.derived();
	}
}

#endif

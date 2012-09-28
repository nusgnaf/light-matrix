/*
 * @file array_copy.h
 *
 * Functions to copy arrays
 *
 * @author Dahua Lin
 */

#ifndef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_COPY_H_
#define LIGHTMAT_ARRAY_COPY_H_

#include <light_mat/array/array_interf.h>
#include "bits/array_copy_internal.h"

namespace lmat
{
	template<typename T, class RArr>
	LMAT_ENSURE_INLINE
	void copy(const T *ps, IDenseArray<RArr, T>& dst)
	{
		typedef typename detail::array_copier<T,
				typename array_traits<RArr>::shape_type>::type copier_t;

		if (dst.has_continuous_layout())
		{
			copier_t::copy(dst.nrows(), dst.ncolumns(),
					ps, dst.ptr_data());
		}
		else
		{
			copier_t::copy(dst.nrows(), dst.ncolumns(),
					ps, dst.ptr_data(), dst.lead_dim());
		}
	}

	template<typename T, class LArr>
	LMAT_ENSURE_INLINE
	void copy(const IDenseArray<LArr, T>& src, T* pd)
	{
		typedef typename detail::array_copier<T,
				typename array_traits<LArr>::shape_type>::type copier_t;

		if (src.has_continuous_layout())
		{
			copier_t::copy(src.nrows(), src.ncolumns(),
					src.ptr_data(), pd);
		}
		else
		{
			copier_t::copy(src.nrows(), src.ncolumns(),
					src.ptr_data(), src.lead_dim(), pd);
		}
	}


	template<typename T, class LArr, class RArr>
	inline
	void copy(const IDenseArray<LArr, T>& src, IDenseArray<RArr, T>& dst)
	{
		LMAT_CHECK_SAME_SHAPE(src, dst)

		typedef typename detail::array_copier<T,
				typename ct_binary_shape_of_arrays<LArr, RArr>::type>::type copier_t;

		if (src.has_continuous_layout())
		{
			if (dst.has_continuous_layout())
			{
				copier_t::copy(src.nrows(), src.ncolumns(),
						src.ptr_data(), dst.ptr_data());
			}
			else
			{
				copier_t::copy(src.nrows(), src.ncolumns(),
						src.ptr_data(), dst.ptr_data(), dst.lead_dim());
			}
		}
		else
		{
			if (dst.has_continuous_layout())
			{
				copier_t::copy(src.nrows(), src.ncolumns(),
						src.ptr_data(), src.lead_dim(), dst.ptr_data());
			}
			else
			{
				copier_t::copy(src.nrows(), src.ncolumns(),
						src.ptr_data(), src.lead_dim(),
						dst.ptr_data(), dst.lead_dim());
			}
		}
	}


	template<typename T, class DMat>
	LMAT_ENSURE_INLINE
	DMat& operator << (IDenseArray<DMat, T>& dmat, const T *src)
	{
		copy(src, dmat.derived());
		return dmat.derived();
	}


	struct matrix_copy_policy { };

	template<typename T, class LArr, class RArr>
	LMAT_ENSURE_INLINE
	void evaluate(const IDenseArray<LArr, T>& src, IDenseArray<RArr, T>& dst, matrix_copy_policy)
	{
		copy(src.derived(), dst.derived());
	}
}

#endif 

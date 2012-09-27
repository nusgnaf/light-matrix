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
	template<typename T, class RMat>
	LMAT_ENSURE_INLINE
	void copy(const T *ps, IDenseArray<RMat, T>& dst)
	{
		typedef typename detail::mat_copier<T,
				ct_nrows<RMat>::value,
				ct_ncols<RMat>::value>::type copier_t;

		if (has_continuous_layout(dst))
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

	template<typename T, class LMat>
	LMAT_ENSURE_INLINE
	void copy(const IDenseArray<LMat, T>& src, T* pd)
	{
		typedef typename detail::mat_copier<T,
				ct_nrows<LMat>::value,
				ct_ncols<LMat>::value>::type copier_t;

		if (has_continuous_layout(src))
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


	template<typename T, class LMat, class RMat>
	inline
	void copy(const IDenseArray<LMat, T>& src, IDenseArray<RMat, T>& dst)
	{
		check_same_size(src, dst, "copy: inconsistent sizes of src and dst.");

		typedef typename detail::mat_copier<T,
				binary_ct_rows<LMat, RMat>::value,
				binary_ct_cols<LMat, RMat>::value>::type copier_t;

		if (has_continuous_layout(src))
		{
			if (has_continuous_layout(dst))
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
			if (has_continuous_layout(dst))
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

	template<typename T, class LMat, class RMat>
	LMAT_ENSURE_INLINE
	void evaluate(const IDenseArray<LMat, T>& src, IDenseArray<RMat, T>& dst, matrix_copy_policy)
	{
		copy(src.derived(), dst.derived());
	}
}

#endif 

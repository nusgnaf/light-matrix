/**
 * @file array_compare.h
 *
 * Functions for array comparison
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_COMPARE_H_
#define LIGHTMAT_ARRAY_COMPARE_H_

#include <light_mat/array/array_interf.h>
#include "bits/array_compare_internal.h"
#include <cmath>

namespace lmat
{
	template<typename T, class LArr, class RArr>
	LMAT_ENSURE_INLINE
	bool is_equal(const IDenseArray<LArr, T>& a, const IDenseArray<RArr, T>& b)
	{
		typedef typename detail::array_comparer<T,
				typename ct_binary_shape_of_arrays<LArr, RArr>::type>::type comparer_t;

		return (a.shape() == b.shape()) &&
				comparer_t::is_equal(a.nrows(), a.ncolumns(),
				a.ptr_data(), a.lead_dim(),
				b.ptr_data(), b.lead_dim());
	}

	template<typename T>
	LMAT_ENSURE_INLINE
	bool is_approx_scalar(const T& a, const T& b, const T& tol)
	{
		return std::fabs(a - b) <= tol;
	}

	template<typename T, class LArr, class RArr>
	inline
	bool is_approx(const IDenseArray<LArr, T>& a, const IDenseArray<RArr, T>& b, const T& tol)
	{
		if (a.shape() == b.shape())
		{
			const index_t m = a.nrows();
			const index_t n = a.ncolumns();

			if (n == 1)
			{
				const T *ca = a.ptr_data();
				const T *cb = b.ptr_data();

				for (index_t i = 0; i < m; ++i)
				{
					if (!is_approx_scalar(ca[i], cb[i], tol))
						return false;
				}
			}
			else
			{
				for (index_t j = 0; j < n; ++j)
				{
					const T *ca = a.ptr_col(j);
					const T *cb = b.ptr_col(j);

					for (index_t i = 0; i < m; ++i)
					{
						if (!is_approx_scalar(ca[i], cb[i], tol))
							return false;
					}
				}
			}

			return true;
		}
		else return false;
	}

}

#endif

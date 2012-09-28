/**
 * @file array_fill_internal.h
 *
 * Internal implementation of array filling
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_FILL_INTERNAL_H_
#define LIGHTMAT_ARRAY_FILL_INTERNAL_H_

#include <light_mat/common/memory.h>

namespace lmat { namespace detail {

	template<typename T, int M, int N>
	struct fixed_mat_fill
	{
		LMAT_ENSURE_INLINE
		static void zero(const index_t, const index_t, T *dst)
		{
			zero_mem(M * N, dst);
		}

		LMAT_ENSURE_INLINE
		static void zero(const index_t, const index_t, T *dst, const index_t ldim)
		{
			for (index_t j = 0; j < N; ++j, dst += ldim)
			{
				zero_mem(M, dst);
			}
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t, const index_t, T *dst)
		{
			fill_val(v, M * N, dst);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t, const index_t, T *dst, const index_t ldim)
		{
			for (index_t j = 0; j < N; ++j, dst += ldim)
			{
				fill_val(v, M, dst);
			}
		}
	};


	template<typename T>
	struct scalar_fill
	{
		LMAT_ENSURE_INLINE
		static void zero(const index_t, const index_t, T *dst)
		{
			dst[0] = T(0);
		}

		LMAT_ENSURE_INLINE
		static void zero(const index_t, const index_t, T *dst, const index_t)
		{
			dst[0] = T(0);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t, const index_t, T *dst)
		{
			dst[0] = v;
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t, const index_t, T *dst, const index_t)
		{
			dst[0] = v;
		}
	};


	template<typename T>
	struct col_fill
	{
		LMAT_ENSURE_INLINE
		static void zero(const index_t m, const index_t, T *dst)
		{
			zero_mem(m, dst);
		}

		LMAT_ENSURE_INLINE
		static void zero(const index_t m, const index_t, T *dst, const index_t)
		{
			zero_mem(m, dst);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t m, const index_t, T *dst)
		{
			fill_val(v, m, dst);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t m, const index_t, T *dst, const index_t ldim)
		{
			fill_val(v, m, dst);
		}
	};


	template<typename T>
	struct row_fill
	{
		LMAT_ENSURE_INLINE
		static void zero(const index_t, const index_t n, T *dst)
		{
			zero_mem(n, dst);
		}

		LMAT_ENSURE_INLINE
		static void zero(const index_t, const index_t n, T *dst, const index_t ldim)
		{
			for (index_t j = 0; j < n; ++j)
				dst[j * ldim] = T(0);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t, const index_t n, T *dst)
		{
			fill_val(v, n, dst);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t, const index_t n, T *dst, const index_t ldim)
		{
			for (index_t j = 0; j < n; ++j)
				dst[j * ldim] = v;
		}
	};


	template<typename T>
	struct mat_fill
	{
		LMAT_ENSURE_INLINE
		static void zero(const index_t m, const index_t n, T *dst)
		{
			zero_mem(m * n, dst);
		}

		LMAT_ENSURE_INLINE
		static void zero(const index_t m, const index_t n, T *dst, const index_t ldim)
		{
			for (index_t j = 0; j < n; ++j, dst += ldim)
			{
				zero_mem(m, dst);
			}
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t m, const index_t n, T *dst)
		{
			fill_val(v, m * n, dst);
		}

		LMAT_ENSURE_INLINE
		static void fill(const T& v,
				const index_t m, const index_t n, T *dst, const index_t ldim)
		{
			for (index_t j = 0; j < n; ++j, dst += ldim)
			{
				fill_val(v, m, dst);
			}
		}
	};

	template<typename T, class Shape>
	struct array_filler
	{
		static const int M = ct_shape_nrows<Shape>::value;
		static const int N = ct_shape_ncols<Shape>::value;

		typedef typename
				if_c<(N == 1),
					typename
					if_c<(M == 1),
						scalar_fill<T>, // N == 1 && M == 1
						col_fill<T> 	// N == 1 && M != 1
					>::type,
					typename
					if_c<(M == 1),
						row_fill<T>,	// N != 1 && M == 1
						typename
						if_c<((M > 0) && (N > 0)),
							fixed_mat_fill<T, M, N>, 	// M > 1 && N > 1
							mat_fill<T> 				// otherwise
						>::type
					>::type
				>::type type;
	};

} }

#endif



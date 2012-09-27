/*
 * @file dense_matrix.h
 *
 * The matrix to represent a dense matrix.
 *
 * @author Dahua Lin
 */

#ifndef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_DENSE_MATRIX_H_
#define LIGHTMAT_DENSE_MATRIX_H_

#include "bits/dense_matrix_internal.h"

namespace lmat
{

	/********************************************
	 *
	 *  matrix traits
	 *
	 ********************************************/

	template<typename T, int CTRows, int CTCols, typename Align>
	struct matrix_traits<tarray<T, CTRows, CTCols, Align> >
	{
		static const int num_dimensions = 2;
		static const int compile_time_num_rows = CTRows;
		static const int compile_time_num_cols = CTCols;

		static const bool is_readonly = false;

		typedef T value_type;
		typedef cpu_domain domain;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct ct_has_continuous_layout<tarray<T, CTRows, CTCols, Align> >
	{
		static const bool value = true;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_base_aligned<tarray<T, CTRows, CTCols, Align> >
	{
		static const bool value = true;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_percol_aligned<tarray<T, CTRows, CTCols, Align> >
	{
		static const bool value = is_same<Align, percol_aligned>::value;
	};

	template<typename T, int CTRows, int CTCols, typename Align>
	struct is_linear_accessible<tarray<T, CTRows, CTCols, Align> >
	{
		static const bool value = true;
	};

	template<typename T, int CTRows, int CTCols, typename Align, class DMat>
	struct default_array_eval_policy<tarray<T, CTRows, CTCols, Align>, DMat>
	{
		typedef matrix_copy_policy type;
	};


	/********************************************
	 *
	 *  dense_matrix
	 *
	 ********************************************/

	template<typename T, int CTRows, int CTCols, typename Align>
	class tarray : public IDenseArray<tarray<T, CTRows, CTCols, Align>, T>
	{
#ifdef LMAT_USE_STATIC_ASSERT
		static_assert(is_supported_matrix_value_type<T>::value,
				"T must be a supported matrix value type.");

		static_assert(CTRows >= 0 && CTCols >= 0,
				"CTRows and CTCols must be non-negative numbers.");

		static_assert(is_same<Align, base_aligned>::value,
				"Align must be base_aligned in current version.");
#endif

	public:
		LMAT_MAT_TRAITS_DEFS(T)

	public:
		LMAT_ENSURE_INLINE tarray()
		: m_internal()
		{
		}

		LMAT_ENSURE_INLINE tarray(index_t m, index_t n)
		: m_internal(m, n)
		{
		}

		LMAT_ENSURE_INLINE tarray(index_t m, index_t n, zero_t)
		: m_internal(m, n)
		{
			zero_mem(m * n, m_internal.ptr_data());
		}

		template<class Setter>
		LMAT_ENSURE_INLINE dense_matrix(index_t m, index_t n,
				const IMemorySetter<Setter, T>& setter)
		: m_internal(m, n)
		{
			setter.set(m * n, m_internal.ptr_data());
		}

		LMAT_ENSURE_INLINE tarray(const tarray& s)
		: m_internal(s.m_internal)
		{
		}

		template<class Expr>
		LMAT_ENSURE_INLINE dense_matrix(const IArrayXpr<Expr, T>& r)
		: m_internal(r.nrows(), r.ncolumns())
		{
			default_evaluate(r, *this);
		}

		LMAT_ENSURE_INLINE void swap(tarray& s)
		{
			m_internal.swap(s.m_internal);
		}

	public:

		LMAT_ENSURE_INLINE tarray& operator = (const tarray& r)
		{
			if (this != &r)
			{
				default_assign(*this, r);
			}

			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE tarray& operator = (const IArrayXpr<Expr, T>& r)
		{
			default_assign(*this, r);
			return *this;
		}

	public:
		LMAT_ENSURE_INLINE index_type nelems() const
		{
			return m_internal.nelems();
		}

		LMAT_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_type>(nelems());
		}

		LMAT_ENSURE_INLINE index_type nrows() const
		{
			return m_internal.nrows();
		}

		LMAT_ENSURE_INLINE index_type ncolumns() const
		{
			return m_internal.ncolumns();
		}

		LMAT_ENSURE_INLINE index_type lead_dim() const
		{
			return m_internal.nrows();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_data() const
		{
			return m_internal.ptr_data();
		}

		LMAT_ENSURE_INLINE pointer ptr_data()
		{
			return m_internal.ptr_data();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_col(const index_type j) const
		{
			return ptr_data() + j * lead_dim();
		}

		LMAT_ENSURE_INLINE pointer ptr_col(const index_type j)
		{
			return ptr_data() + j * lead_dim();
		}

		LMAT_ENSURE_INLINE index_type offset(const index_type i, const index_type j) const
		{
			return m_internal.offset(i, j);
		}

		LMAT_ENSURE_INLINE const_reference elem(const index_type i, const index_type j) const
		{
			return ptr_data()[offset(i, j)];
		}

		LMAT_ENSURE_INLINE reference elem(const index_type i, const index_type j)
		{
			return ptr_data()[offset(i, j)];
		}

		LMAT_ENSURE_INLINE const_reference operator[] (const index_type i) const
		{
			return ptr_data()[i];
		}

		LMAT_ENSURE_INLINE reference operator[] (const index_type i)
		{
			return ptr_data()[i];
		}

		LMAT_ENSURE_INLINE void require_size(index_type m, index_type n)
		{
			m_internal.resize(m, n);
		}

	private:
		detail::dense_matrix_internal<T, CTRows, CTCols> m_internal;
	};


	template<typename T, int CTRows, int CTCols>
	LMAT_ENSURE_INLINE
	inline void swap(tarray<T, CTRows, CTCols>& a, tarray<T, CTRows, CTCols>& b)
	{
		a.swap(b);
	}


	/********************************************
	 *
	 *  derived vectors
	 *
	 ********************************************/

	template<typename T, int CTRows, typename Align>
	class tcol : public tarray<T, CTRows, 1, Align>
	{
		typedef tarray<T, CTRows, 1, Align> base_mat_t;

	public:
		LMAT_ENSURE_INLINE tcol() : base_mat_t(CTRows, 1) { }

		LMAT_ENSURE_INLINE explicit tcol(index_t m) : base_mat_t(m, 1) { }

		LMAT_ENSURE_INLINE tcol(index_t m, zero_t) : base_mat_t(m, 1, zero_t()) { }

		template<class Setter>
		LMAT_ENSURE_INLINE
		tcol(index_t m, const IMemorySetter<Setter, T>& setter) : base_mat_t(m, 1, setter) { }

		LMAT_ENSURE_INLINE tcol(const base_mat_t& s) : base_mat_t(s) { }

		LMAT_ENSURE_INLINE tcol(const tcol& s) : base_mat_t(s) { }

		template<class Other>
		LMAT_ENSURE_INLINE tcol(const IMatrixView<Other, T>& r) : base_mat_t(r) { }

		template<class Expr>
		LMAT_ENSURE_INLINE tcol(const IArrayXpr<Expr, T>& r) : base_mat_t(r) { }

	public:

		LMAT_ENSURE_INLINE tcol& operator = (const base_mat_t& r)
		{
			base_mat_t::operator = (r);
			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE tcol& operator = (const IArrayXpr<Expr, T>& r)
		{
			base_mat_t::operator = (r);
			return *this;
		}

	public:
		LMAT_ENSURE_INLINE
		void require_size(index_t m, index_t n)
		{
			base_mat_t::require_size(m, n);
		}

		LMAT_ENSURE_INLINE
		void require_size(index_t n)
		{
			base_mat_t::require_size(n, 1);
		}
	};


	template<typename T, int CTCols, typename Align>
	class trow : public tarray<T, 1, CTCols, Align>
	{
		typedef tarray<T, 1, CTCols, Align> base_mat_t;

	public:
		LMAT_ENSURE_INLINE trow() : base_mat_t(1, CTCols) { }

		LMAT_ENSURE_INLINE explicit trow(index_t n) : base_mat_t(1, n) { }

		LMAT_ENSURE_INLINE trow(index_t n, zero_t) : base_mat_t(1, n, zero_t()) { }

		template<class Setter>
		LMAT_ENSURE_INLINE
		trow(index_t n, const IMemorySetter<Setter,T>& setter) : base_mat_t(1, n, setter) { }

		LMAT_ENSURE_INLINE trow(const base_mat_t& s) : base_mat_t(s) { }

		LMAT_ENSURE_INLINE trow(const trow& s) : base_mat_t(s) { }

		template<class Expr>
		LMAT_ENSURE_INLINE trow(const IArrayXpr<Expr, T>& r) : base_mat_t(r) { }

	public:

		LMAT_ENSURE_INLINE trow& operator = (const base_mat_t& r)
		{
			base_mat_t::operator = (r);
			return *this;
		}

		template<class Expr>
		LMAT_ENSURE_INLINE trow& operator = (const IArrayXpr<Expr, T>& r)
		{
			base_mat_t::operator = (r);
			return *this;
		}

	public:
		LMAT_ENSURE_INLINE
		void require_size(index_t m, index_t n)
		{
			base_mat_t::require_size(m, n);
		}

		LMAT_ENSURE_INLINE
		void require_size(index_t n)
		{
			base_mat_t::require_size(1, n);
		}
	};


	/********************************************
	 *
	 *  evaluation
	 *
	 ********************************************/

	template<typename T, class Expr>
	LMAT_ENSURE_INLINE
	tarray<T, ct_nrows<Expr>::value, ct_ncols<Expr>::value>
	eval(const IArrayXpr<Expr, T>& expr)
	{
		return tarray<T, ct_nrows<Expr>::value, ct_ncols<Expr>::value>(
				expr.derived());
	}

	template<typename T, class Expr, class Context>
	LMAT_ENSURE_INLINE
	tarray<T, ct_nrows<Expr>::value, ct_ncols<Expr>::value>
	eval(const IArrayXpr<Expr, T>& expr, const Context& ctx)
	{
		tarray<T, ct_nrows<Expr>::value, ct_ncols<Expr>::value> r(expr.nrows(), expr.ncolumns());
		evaluate(expr.derived(), r, ctx);
	}

	template<typename T, class Expr>
	LMAT_ENSURE_INLINE
	T to_scalar(const IArrayXpr<Expr, T>& expr)
	{
		tarray<T,1,1> r(expr);
		return r[0];
	}


	/********************************************
	 *
	 *  Typedefs
	 *
	 ********************************************/

	LMAT_MATRIX_TYPEDEFS2(tarray, mat, 0, 0)
	LMAT_MATRIX_TYPEDEFS2(tarray, mat22, 2, 2)
	LMAT_MATRIX_TYPEDEFS2(tarray, mat23, 2, 3)
	LMAT_MATRIX_TYPEDEFS2(tarray, mat32, 3, 2)
	LMAT_MATRIX_TYPEDEFS2(tarray, mat33, 3, 3)

	LMAT_MATRIX_TYPEDEFS1(tcol, col, 0)
	LMAT_MATRIX_TYPEDEFS1(tcol, col2, 2)
	LMAT_MATRIX_TYPEDEFS1(tcol, col3, 3)

	LMAT_MATRIX_TYPEDEFS1(trow, row, 0)
	LMAT_MATRIX_TYPEDEFS1(trow, row2, 2)
	LMAT_MATRIX_TYPEDEFS1(trow, row3, 3)

}


#endif 

/*
 * @file tarray.h
 *
 * The class to represent a dense array
 *
 * @author Dahua Lin
 */

#ifndef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_TARRAY_H_
#define LIGHTMAT_TARRAY_H_

#include <light_mat/array/array_base.h>
#include <light_mat/common/block.h>

namespace lmat
{

	namespace detail
	{
		template<typename T, int CTSize>
		struct tarray_storage
		{
			typedef sblock<T, CTSize> block_type;

			block_type block;

			LMAT_ENSURE_INLINE explicit tarray_storage() : block() { }

			LMAT_ENSURE_INLINE tarray_storage(index_t, index_t) : block() { }

			LMAT_ENSURE_INLINE const T* pdata() const { return block.ptr_data(); }

			LMAT_ENSURE_INLINE T* pdata() { return block.ptr_data(); }

			LMAT_ENSURE_INLINE void resize(index_t siz)
			{
				throw invalid_operation("Attempted to resize an static array.");
			}

			LMAT_ENSURE_INLINE void swap(tarray_storage& other)
			{
				block.swap(other.block);
			}
		};

		template<typename T>
		struct tarray_storage<T, 0>
		{
			typedef dblock<T> block_type;

			block_type block;

			LMAT_ENSURE_INLINE explicit tarray_storage() : block() { }

			LMAT_ENSURE_INLINE tarray_storage(index_t m, index_t n) : block(m * n) { }

			LMAT_ENSURE_INLINE const T* pdata() const { return block.ptr_data(); }

			LMAT_ENSURE_INLINE T* pdata() { return block.ptr_data(); }

			LMAT_ENSURE_INLINE void resize(index_t siz)
			{
				block.resize(siz);
			}

			LMAT_ENSURE_INLINE void swap(tarray_storage& other)
			{
				block.swap(other.block);
			}
		};

	};



	/********************************************
	 *
	 *  matrix traits
	 *
	 ********************************************/

	template<typename T, int CTRows, int CTCols>
	struct array_traits<tarray<T, CTRows, CTCols> >
	{
		typedef array_shape<CTRows, CTCols> shape_type;
		typedef continuous_column_major_layout<CTRows, CTCols> layout_type;

		typedef T value_type;
		typedef T access_type;
		typedef cpu_domain domain;
	};

	template<typename T, int CTRows, int CTCols>
	struct ct_has_continuous_layout<tarray<T, CTRows, CTCols> >
	{
		static const bool value = true;
	};

	template<typename T, int CTRows, int CTCols, class DMat>
	struct default_array_eval_policy<tarray<T, CTRows, CTCols>, DMat>
	{
		typedef matrix_copy_policy type;
	};


	/********************************************
	 *
	 *  dense_matrix
	 *
	 ********************************************/

	template<typename T, int CTRows, int CTCols>
	class tarray : public IDenseArray<tarray<T, CTRows, CTCols>, T>
	{
#ifdef LMAT_USE_STATIC_ASSERT
		static_assert(is_supported_matrix_value_type<T>::value,
				"T must be a supported matrix value type.");

		static_assert(CTRows >= 0 && CTCols >= 0,
				"CTRows and CTCols must be non-negative numbers.");
#endif

	public:
		LMAT_ARR_TRAITS_DEFS(T)

		typedef array_shape<CTRows, CTCols> shape_type;
		typedef continuous_column_major_layout<CTRows, CTCols> layout_type;

	public:
		LMAT_ENSURE_INLINE tarray()
		: m_store()
		, m_layout()
		{
		}

		LMAT_ENSURE_INLINE tarray(index_t m, index_t n)
		: m_store(m, n)
		, m_layout(m, n)
		{
		}

		LMAT_ENSURE_INLINE tarray(index_t m, index_t n, zero_t)
		: m_store(m, n)
		, m_layout(m, n)
		{
			zero_mem(m * n, m_store.pdata());
		}

		template<class Setter>
		LMAT_ENSURE_INLINE tarray(index_t m, index_t n,
				const IMemorySetter<Setter, T>& setter)
		: m_store(m, n)
		, m_layout(m, n)
		{
			setter.set(m * n, m_store.pdata());
		}

		LMAT_ENSURE_INLINE tarray(const tarray& s)
		: m_store(s.m_store)
		, m_layout(s.m_layout)
		{
		}

		template<class Expr>
		LMAT_ENSURE_INLINE tarray(const IArrayXpr<Expr, T>& r)
		: m_store(r.nrows(), r.ncolumns())
		, m_layout(r.nrows(), r.ncolumns())
		{
			default_evaluate(r, *this);
		}

		LMAT_ENSURE_INLINE void swap(tarray& s)
		{
			m_store.swap(s.m_store);

			using std::swap;
			swap(m_layout, s.m_layout);
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
		const shape_type& shape() const
		{
			return m_layout.shape();
		}

		const layout_type& layout() const
		{
			return m_layout;
		}

		const T* ptr_data() const
		{
			return m_store.pdata();
		}

		T *ptr_data()
		{
			return m_store.pdata();
		}

		void require_shape(index_t m, index_t n)
		{
			shape_type new_shape(m, n);

			if (new_shape != shape())
			{
				index_t new_size = new_shape.nelems();
				if (new_size != this->nelems())
					m_store.resize(new_size);

				m_layout.set_shape(new_shape);
			}
		}

	private:
		detail::tarray_storage<T, CTRows * CTCols> m_store;
		layout_type m_layout;
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

	template<typename T, int CTRows>
	class tcol : public tarray<T, CTRows, 1>
	{
		typedef tarray<T, CTRows, 1> base_mat_t;

	public:
		LMAT_ENSURE_INLINE tcol() : base_mat_t(CTRows, 1) { }

		LMAT_ENSURE_INLINE explicit tcol(index_t m) : base_mat_t(m, 1) { }

		LMAT_ENSURE_INLINE tcol(index_t m, zero_t) : base_mat_t(m, 1, zero_t()) { }

		template<class Setter>
		LMAT_ENSURE_INLINE
		tcol(index_t m, const IMemorySetter<Setter, T>& setter) : base_mat_t(m, 1, setter) { }

		LMAT_ENSURE_INLINE tcol(const base_mat_t& s) : base_mat_t(s) { }

		LMAT_ENSURE_INLINE tcol(const tcol& s) : base_mat_t(s) { }

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
	};


	template<typename T, int CTCols>
	class trow : public tarray<T, 1, CTCols>
	{
		typedef tarray<T, 1, CTCols> base_mat_t;

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
		return *(r.ptr_data());
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

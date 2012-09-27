/**
 * @file array_interf.h
 *
 * Basic array interfaces
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_INTERF_H_
#define LIGHTMAT_ARRAY_INTERF_H_

#include <light_mat/array/array_meta.h>

#define LMAT_ARR_TRAITS_DEFS_FOR_BASE(D) \
	typedef typename array_traits<D>::value_type value_type; \
	typedef typename array_traits<D>::access_type access_type; \
	typedef typename array_traits<D>::shape_type shape_type; \
	typedef const value_type* const_pointer; \
	typedef const value_type& const_reference; \
	typedef access_type* pointer; \
	typedef access_type& reference; \
	typedef size_t size_type; \
	typedef index_t difference_type;

#define LMAT_ARR_TRAITS_CDEFS(T) \
	typedef T value_type; \
	typedef const value_type* const_pointer; \
	typedef const value_type& const_reference; \
	typedef size_t size_type; \
	typedef index_t difference_type;

#define LMAT_ARR_TRAITS_DEFS(T) \
	typedef T value_type; \
	typedef const value_type* const_pointer; \
	typedef const value_type& const_reference; \
	typedef value_type* pointer; \
	typedef value_type& reference; \
	typedef size_t size_type; \
	typedef index_t difference_type;


namespace lmat
{

	/********************************************
	 *
	 *  Concepts
	 *
	 *  Each concept is associated with a
	 *  class as a static polymorphism base
	 *
	 ********************************************/

	template<class Derived, typename T>
	class IArrayXpr
	{
	public:
		LMAT_ARR_TRAITS_DEFS_FOR_BASE(Derived)
		LMAT_CRTP_REF

	public:
		LMAT_ENSURE_INLINE const shape_type& shape() const
		{
			return derived().shape();
		}

	public:
		LMAT_ENSURE_INLINE index_t nelems() const
		{
			return shape().nelems();
		}

		LMAT_ENSURE_INLINE size_type size() const
		{
			return static_cast<size_type>(nelems());
		}

		LMAT_ENSURE_INLINE index_t nrows() const
		{
			return shape().nrows();
		}

		LMAT_ENSURE_INLINE index_t ncolumns() const
		{
			return shape().ncolumns();
		}

		LMAT_ENSURE_INLINE bool is_matrix() const
		{
			return shape().is_matrix();
		}

		LMAT_ENSURE_INLINE bool is_row() const
		{
			return shape().is_row();
		}

		LMAT_ENSURE_INLINE bool is_column() const
		{
			return shape().is_column();
		}

		LMAT_ENSURE_INLINE bool is_vector() const
		{
			return shape().is_vector();
		}

		LMAT_ENSURE_INLINE bool is_scalar() const
		{
			return shape().is_scalar();
		}

		LMAT_ENSURE_INLINE bool is_empty() const
		{
			return shape().is_empty();
		}

		/*
		LMAT_ENSURE_INLINE
		typename unary_expr_map<transpose_t, ref_arg_t, Derived>::type
		trans() const
		{
			return unary_expr_map<transpose_t, ref_arg_t, Derived>::get(
					transpose_t(), ref_arg(derived()));
		}
		*/

	}; // end class IMatrixBase



	/**
	 * The interfaces for matrix blocks
	 */
	template<class Derived, typename T>
	class IDenseArray : public IArrayXpr<Derived, T>
	{
	public:
		LMAT_ARR_TRAITS_DEFS_FOR_BASE(Derived)
		typedef typename array_traits<Derived>::layout_type layout_type;

		LMAT_CRTP_REF

	public:
		const layout_type& layout() const
		{
			return derived().layout();
		}

		LMAT_ENSURE_INLINE const_pointer ptr_data() const
		{
			return derived().ptr_data();
		}

		LMAT_ENSURE_INLINE pointer ptr_data()
		{
			return derived().ptr_data();
		}

		LMAT_ENSURE_INLINE void require_shape(const index_t m, const index_t n)
		{
			derived().require_shape(m, n);
		}

	public:
		LMAT_ENSURE_INLINE index_t lead_dim() const
		{
			return layout().lead_dim();
		}

		LMAT_ENSURE_INLINE const_reference operator() (const index_t i) const
		{
			return ptr_data() + layout().offset(i);
		}

		LMAT_ENSURE_INLINE reference operator() (const index_t i)
		{
			return ptr_data() + layout().offset(i);
		}

		LMAT_ENSURE_INLINE const_reference operator() (const index_t i, const index_t j) const
		{
			return ptr_data() + layout().offset(i, j);
		}

		LMAT_ENSURE_INLINE reference operator() (const index_t i, const index_t j)
		{
			return ptr_data() + layout().offset(i, j);
		}

	}; // end class IDenseMatrixBlock


	/********************************************
	 *
	 *  matrix assignment
	 *
	 ********************************************/

	template<class SExpr, class DArr>
	struct array_eval_verifier
	{
		static const bool value =
				is_dense_array<DArr>::value &&
				is_array_xpr<SExpr>::value &&
				has_same_domain<SExpr, DArr>::value &&
				has_same_value_type<SExpr, DArr>::value &&
				have_compatible_shapes<DArr, SExpr>::value;
	};

	template<typename T, class SExpr, class DArr>
	LMAT_ENSURE_INLINE
	inline typename enable_if<array_eval_verifier<SExpr, DArr>, void>::type
	default_evaluate(const IArrayXpr<SExpr, T>& sexpr, IDenseArray<DArr, T>& dmat)
	{
		typedef typename default_array_eval_policy<SExpr, DArr>::type policy_t;
		evaluate(sexpr.derived(), dmat.derived(), policy_t());
	}

	template<typename T, class DArr, class RExpr>
	LMAT_ENSURE_INLINE
	inline typename enable_if<array_eval_verifier<RExpr, DArr>, void>::type
	default_assign(IDenseArray<DArr, T>& lhs, const IArrayXpr<RExpr, T>& rhs)
	{
		lhs.require_shape(rhs.nrows(), rhs.ncolumns());
		default_evaluate(rhs, lhs);
	}

}

#endif /* MATRIX_CONCEPTS_H_ */



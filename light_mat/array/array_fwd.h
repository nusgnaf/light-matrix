/**
 * @file array_fwd.h
 *
 * Forward declaration of array-related classes
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_FWD_H_
#define LIGHTMAT_ARRAY_FWD_H_

#include <light_mat/common/basic_defs.h>
#include <light_mat/common/range.h>
#include <light_mat/common/expr_base.h>
#include <light_mat/math/functor_base.h>

#include <light_mat/array/array_layout.h>

namespace lmat
{

	// forward declaration of concepts


	/****************************************************************
	 *
	 *   A specialized version of array_traits<C> must be provided,
	 *   which should contain the following static members:
	 *
	 *	 - shape_type:		the type of array shape
	 *
	 *	 - layout_type:		the type of array layout (only for dense)
	 *
	 *	 - value_type:		the type of element value
	 *
	 *	 - access_type:		For writable array, the same as value_type
	 *	 					For read-only ones, it is const value_type
	 *
	 *	 - domain:			the domain (e.g. cpu_domain, cuda_domain)
	 *
	 ****************************************************************/

	struct cpu_domain { };
	struct cuda_domain { };

	template<class Derived> struct array_traits;

	template<class Derived, typename T> class IArrayXpr;
	template<class Derived, typename T> class IDenseArray;

	// forward declaration of some important types

	template<typename T, int CTRows=0, int CTCols=0> class tarray;
	template<typename T, int CTRows=0> class tcol;
	template<typename T, int CTCols=0> class trow;

	template<typename T, int RowDim=0, int ColDim=0> class tarray_cref;
	template<typename T, int RowDim=0, int ColDim=0> class tarray_ref;

	template<typename T, int CTRows=0> class tcol_cref;
	template<typename T, int CTRows=0> class tcol_ref;
	template<typename T, int CTCols=0> class trow_cref;
	template<typename T, int CTCols=0> class trow_ref;

	template<typename T, int CTRows=0, int CTCols=0> class tarray_cref_ex;
	template<typename T, int CTRows=0, int CTCols=0> class tarray_ref_ex;
	template<typename T, int CTRows=0, int CTCols=0> class const_array;

	// sub-view expressions

	template<class Arr, typename RowRange> class colview;
	template<class Arr, typename ColRange> class rowview;
	template<class Arr, typename RowRange, typename ColRange> class subview;

	template<class Arr, typename RowRange> class mutable_colview;
	template<class Arr, typename ColRange> class mutable_rowview;
	template<class Arr, typename RowRange, typename ColRange> class mutable_subview;

	// expressions

	template<class Fun, typename Arg_HP, class Arg> class unary_ewise_expr;

	template<class Fun,
		typename Arg1_HP, class Arg1,
		typename Arg2_HP, class Arg2> class binary_ewise_expr;

	template<class Fun,
		typename Arg1_HP, class Arg1,
		typename Arg2_HP, class Arg2> class ternary_ewise_expr;

	struct transpose_t { };
	template<typename Arg_HP, class Arg> class transpose_expr;

	template<typename Arg_HP, class Arg, int N> class horizontal_repeat_expr;
	template<typename Arg_HP, class Arg, int M> class vertical_repeat_expr;

	struct rowwise { };
	struct colwise { };

	template<class Fun, typename Arg_HP, class Arg> class colwise_reduce_expr;
	template<class Fun, typename Arg_HP, class Arg> class rowwise_reduce_expr;

	// evaluation

	template<class Expr, class Dst> struct default_array_eval_policy;
}

// Useful macros

#define LMAT_MATRIX_TYPEDEFS0(TName, prefix) \
	typedef TName<double>   prefix##_f64; \
	typedef TName<float>    prefix##_f32; \
	typedef TName<int32_t>  prefix##_i32; \
	typedef TName<uint32_t> prefix##_u32; \
	typedef TName<int16_t>  prefix##_i16; \
	typedef TName<uint16_t> prefix##_u16; \
	typedef TName<int8_t>   prefix##_i8; \
	typedef TName<uint8_t>  prefix##_u8; \
	typedef TName<bool>     prefix##_bool;

#define LMAT_MATRIX_TYPEDEFS1(TName, prefix, Dim) \
	typedef TName<double,   Dim> prefix##_f64; \
	typedef TName<float,    Dim> prefix##_f32; \
	typedef TName<int32_t,  Dim> prefix##_i32; \
	typedef TName<uint32_t, Dim> prefix##_u32; \
	typedef TName<int16_t,  Dim> prefix##_i16; \
	typedef TName<uint16_t, Dim> prefix##_u16; \
	typedef TName<int8_t,   Dim> prefix##_i8; \
	typedef TName<uint8_t,  Dim> prefix##_u8; \
	typedef TName<bool,     Dim> prefix##_bool;

#define LMAT_MATRIX_TYPEDEFS2(TName, prefix, RDim, CDim) \
	typedef TName<double,   RDim, CDim> prefix##_f64; \
	typedef TName<float,    RDim, CDim> prefix##_f32; \
	typedef TName<int32_t,  RDim, CDim> prefix##_i32; \
	typedef TName<uint32_t, RDim, CDim> prefix##_u32; \
	typedef TName<int16_t,  RDim, CDim> prefix##_i16; \
	typedef TName<uint16_t, RDim, CDim> prefix##_u16; \
	typedef TName<int8_t,   RDim, CDim> prefix##_i8; \
	typedef TName<uint8_t,  RDim, CDim> prefix##_u8; \
	typedef TName<bool,     RDim, CDim> prefix##_bool;

#endif



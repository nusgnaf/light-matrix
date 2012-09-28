/**
 * @file array_meta.h
 *
 * Meta programming tools for array classes
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_META_H_
#define LIGHTMAT_ARRAY_META_H_

#include <light_mat/array/array_fwd.h>

namespace lmat
{

	/********************************************
	 *
	 *  Concept test tools
	 *
	 ********************************************/

	template<class T>
	struct is_supported_matrix_value_type
	{
		static const bool value = lmat::is_pod<T>::value;
	};

	template<class T>
	struct is_supported_matrix_value_type< mask_t<T> >
	{
		static const bool value = is_supported_matrix_value_type<T>::value;
	};

	template<class Derived, template<class D, typename T> class Interface>
	struct has_array_interface
	{
		typedef Interface<Derived, typename array_traits<Derived>::value_type> interface_t;
		static const bool value = is_base_of<interface_t, Derived>::value;
	};

	template<class Arr>
	struct is_array_xpr
	{
		static const bool value = has_array_interface<Arr, IArrayXpr>::value;
	};

	template<class Arr>
	struct is_dense_array
	{
		static const bool value = has_array_interface<Arr, IDenseArray>::value;
	};


	/********************************************
	 *
	 *  Domain related tools
	 *
	 ********************************************/

	template<class LArr, class RArr>
	struct has_same_domain
	{
		static const bool value = is_same<
				typename array_traits<LArr>::domain,
				typename array_traits<RArr>::domain>::value;
	};


	template<class Arr>
	struct has_cpu_domain
	{
		static const bool value = is_same<
				typename array_traits<Arr>::domain,
				cpu_domain>::value;
	};


	/********************************************
	 *
	 *  Type related tools
	 *
	 ********************************************/

	template<class LArr, class RArr>
	struct has_same_value_type
	{
		static const bool value = is_same<
			typename array_traits<LArr>::value_type,
			typename array_traits<RArr>::value_type>::value;
	};


	namespace detail
	{
		template<typename T1, typename T2> struct _binary_vtype;

		template<typename T>
		struct _binary_vtype<T, T>
		{
			typedef T type;
		};
	};


	template<class LArr, class RArr>
	struct binary_value_type
	{
		typedef typename detail::_binary_vtype<
			typename array_traits<LArr>::value_type,
			typename array_traits<RArr>::value_type>::type type;
	};


	/********************************************
	 *
	 *  Shape related tools
	 *
	 ********************************************/

	template<class Arr>
	struct ct_nrows
	{
		static const int value = ct_shape_nrows<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_ncols
	{
		static const int value = ct_shape_ncols<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_nelems
	{
		static const int value = ct_shape_nelems<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_is_row
	{
		static const int value = ct_is_row_shape<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_is_col
	{
		static const int value = ct_is_column_shape<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_is_vector
	{
		static const int value = ct_is_vector_shape<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_is_scalar
	{
		static const int value = ct_is_scalar_shape<
				typename array_traits<Arr>::shape_type>::value;
	};

	template<class Arr>
	struct ct_is_matrix
	{
		static const int value = ct_is_matrix_shape<
				typename array_traits<Arr>::shape_type>::value;
	};


	template<class LArr, class RArr>
	struct have_compatible_shapes
	{
		typedef typename array_traits<LArr>::shape_type left_shape_t;
		typedef typename array_traits<RArr>::shape_type right_shape_t;

		static const bool value =
				are_compatible_shapes<left_shape_t, right_shape_t>::value;
	};

	template<class LArr, class RArr>
	struct ct_binary_shape_of_arrays
	{
		typedef typename array_traits<LArr>::shape_type left_shape_t;
		typedef typename array_traits<RArr>::shape_type right_shape_t;

		typedef typename ct_binary_shape<left_shape_t, right_shape_t>::type type;
	};


	/********************************************
	 *
	 *  Layout related tools
	 *
	 ********************************************/

	template<class Arr>
	struct ct_has_continuous_layout
	{
		static const bool value = false;
	};



}

#endif

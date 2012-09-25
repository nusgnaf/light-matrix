/**
 * @file type_traits.h
 *
 * @brief Import of TR1 type traits.
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTARR_TYPE_TRAITS_H_
#define LIGHTARR_TYPE_TRAITS_H_

#include <light_array/config/config.h>

#ifdef LARR_USE_C11_STDLIB
#include <type_traits>
#else
#include <tr1/type_traits>
#endif

namespace larr
{
	using LARR_TR1::is_void;
	using LARR_TR1::is_integral;
	using LARR_TR1::is_floating_point;
	using LARR_TR1::is_array;
	using LARR_TR1::is_pointer;
	using LARR_TR1::is_reference;
	using LARR_TR1::is_member_function_pointer;
	using LARR_TR1::is_member_object_pointer;
	using LARR_TR1::is_enum;
	using LARR_TR1::is_union;
	using LARR_TR1::is_class;
	using LARR_TR1::is_function;

	using LARR_TR1::is_arithmetic;
	using LARR_TR1::is_fundamental;
	using LARR_TR1::is_object;
	// using LARR_TR1::is_scalar;
	using LARR_TR1::is_compound;
	using LARR_TR1::is_member_pointer;

	using LARR_TR1::is_signed;
	using LARR_TR1::is_unsigned;

	using LARR_TR1::is_const;
	using LARR_TR1::is_volatile;
	using LARR_TR1::is_pod;
	// using LARR_TR1::is_empty;
	using LARR_TR1::is_polymorphic;
	using LARR_TR1::is_abstract;

	using LARR_TR1::is_same;
	using LARR_TR1::is_base_of;

	using LARR_TR1::remove_const;
	using LARR_TR1::remove_volatile;
	using LARR_TR1::remove_cv;
	using LARR_TR1::remove_pointer;
	using LARR_TR1::remove_reference;

	using LARR_TR1::add_const;
	using LARR_TR1::add_volatile;
	using LARR_TR1::add_cv;

	using LARR_TR1::alignment_of;


	template<typename S, typename T>
	struct is_implicitly_convertible_int
	{
		static const bool value =
				is_integral<S>::value && is_integral<T>::value &&
				(
					( is_signed<S>::value == is_signed<T>::value && sizeof(S) <= sizeof(T) ) ||
					( is_signed<T>::value && sizeof(S) < sizeof(T))
				);
	};


	template<typename S, typename T>
	struct is_implicitly_convertible_real
	{
		static const bool value =
				(is_same<S, float>::value && (is_same<T, float>::value || is_same<T, double>::value))
				||
				(is_same<S, double>::value && (is_same<T, double>::value));
	};


	/**
	 * Indicates that implicitly converting
	 * value_type from S to D is ok.
	 */
	template<typename S_, typename T_>
	struct is_implicitly_convertible
	{
		typedef typename remove_cv<S_>::type S;
		typedef typename remove_cv<T_>::type T;

		static const bool value =
			is_same<S, T>::value
			||
			is_implicitly_convertible_int<S, T>::value
			||
			is_implicitly_convertible_real<S, T>::value
			||
			(is_integral<S>::value && is_floating_point<T>::value &&
					sizeof(S) <= sizeof(T)
			)
			||
			(is_same<S, bool>::value && is_integral<T>::value);
	};

}

#endif



/*
 * @file test_array_shape.cpp
 *
 * Unit testing for array shapes
 *
 * @author Dahua Lin
 */

#include "test_base.h"

#include <light_mat/array/array_layout.h>

using namespace lmat;
using namespace lmat::test;

// explicit instantiation

template class array_shape<0, 0>;
template class array_shape<0, 4>;
template class array_shape<3, 0>;
template class array_shape<3, 4>;

MN_CASE( arr_shape, basics )
{
	index_t m = M == 0 ? 3 : M;
	index_t n = N == 0 ? 4 : N;

	array_shape<M, N> s0;

	ASSERT_EQ( s0.nrows(), M );
	ASSERT_EQ( s0.ncolumns(), N );
	ASSERT_EQ( s0.nelems(), M * N );

	ASSERT_EQ( s0.is_matrix(), true );
	ASSERT_EQ( s0.is_row(), (M == 1) );
	ASSERT_EQ( s0.is_column(), (N == 1) );
	ASSERT_EQ( s0.is_scalar(), (M == 1 && N == 1) );
	ASSERT_EQ( s0.is_vector(), (M == 1 || N == 1) );
	ASSERT_EQ( s0.is_empty(), (M == 0 || N == 0) );

	array_shape<M, N> s1(m, n);

	ASSERT_EQ( s1.nrows(), m );
	ASSERT_EQ( s1.ncolumns(), n );
	ASSERT_EQ( s1.nelems(), m * n );

	ASSERT_EQ( s1.is_matrix(), true );
	ASSERT_EQ( s1.is_row(), (m == 1) );
	ASSERT_EQ( s1.is_column(), (n == 1) );
	ASSERT_EQ( s1.is_scalar(), (m == 1 && n == 1) );
	ASSERT_EQ( s1.is_vector(), (m == 1 || n == 1) );
	ASSERT_EQ( s1.is_empty(), (m == 0 || n == 0) );
}


MN_CASE( arr_shape, ct_properties )
{
	typedef array_shape<M, N> shape_t;

	ASSERT_EQ( ct_nrows<shape_t>::value, M );
	ASSERT_EQ( ct_ncols<shape_t>::value, N );

	int ct_ne = M > 0 && N > 0 ? M * N : 0;
	ASSERT_EQ( ct_nelems<shape_t>::value, ct_ne );

	ASSERT_EQ( is_array_shape<shape_t>::value, true );

	ASSERT_EQ( ct_is_matrix_shape<shape_t>::value, true );
	ASSERT_EQ( ct_is_vector_shape<shape_t>::value, (M == 1 || N == 1) );
	ASSERT_EQ( ct_is_row_shape<shape_t>::value, (M == 1) );
	ASSERT_EQ( ct_is_column_shape<shape_t>::value, (N == 1) );
	ASSERT_EQ( ct_is_scalar_shape<shape_t>::value, (M == 1 && N == 1) );

	typedef array_shape<0, 0> shape_t0;
	typedef array_shape<0, N> shape_t1;
	typedef array_shape<M, 0> shape_t2;

	ASSERT_TRUE( (are_compatible_shapes<shape_t, shape_t0>::value) );
	ASSERT_TRUE( (are_compatible_shapes<shape_t, shape_t1>::value) );
	ASSERT_TRUE( (are_compatible_shapes<shape_t, shape_t2>::value) );

	ASSERT_TRUE( (are_compatible_shapes<shape_t0, shape_t>::value) );
	ASSERT_TRUE( (are_compatible_shapes<shape_t1, shape_t>::value) );
	ASSERT_TRUE( (are_compatible_shapes<shape_t2, shape_t>::value) );
}


MN_CASE( arr_shape, compare )
{
	index_t m = M == 0 ? 3 : M;
	index_t n = N == 0 ? 4 : N;

	array_shape<M, N> s_mn(m, n);
	array_shape<M, 0> s_m0(m, n);
	array_shape<0, N> s_0n(m, n);
	array_shape<0, 0> s_00(m, n);

	ASSERT_TRUE( s_mn == s_m0 );
	ASSERT_TRUE( s_mn == s_0n );
	ASSERT_TRUE( s_mn == s_00 );

	ASSERT_FALSE( s_mn != s_m0 );
	ASSERT_FALSE( s_mn != s_0n );
	ASSERT_FALSE( s_mn != s_00 );
}


BEGIN_TPACK( arr_shape_basics )
	ADD_MN_CASE_3X3( arr_shape, basics, 3, 4 )
END_TPACK

BEGIN_TPACK( arr_shape_ctprops )
	ADD_MN_CASE_3X3( arr_shape, ct_properties, 3, 4 )
END_TPACK

BEGIN_TPACK( arr_shape_compare )
	ADD_MN_CASE_3X3( arr_shape, compare, 3, 4 )
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( arr_shape_basics )
	ADD_TPACK( arr_shape_ctprops )
	ADD_TPACK( arr_shape_compare )
END_MAIN_SUITE











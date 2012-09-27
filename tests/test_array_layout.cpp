/*
 * @file test_array_layout.cpp
 *
 * @brief Unit testing for array layout classes
 *
 * @author Dahua Lin
 */

#include "test_base.h"

#include <light_mat/array/array_layout.h>

using namespace lmat;
using namespace lmat::test;

// explicit instantiation

template class continuous_column_major_layout<0, 0>;
template class continuous_column_major_layout<0, 1>;
template class continuous_column_major_layout<1, 0>;
template class continuous_column_major_layout<1, 1>;

template class column_major_layout<0, 0>;
template class column_major_layout<0, 1>;
template class column_major_layout<1, 0>;
template class column_major_layout<1, 1>;


MN_CASE( cont_cm_layout, basics )
{
	const index_t m = M == 0 ? 3 : M;
	const index_t n = N == 0 ? 4 : N;

	array_shape<M, N> s(m, n);
	continuous_column_major_layout<M, N> a(m, n);

	ASSERT_EQ( a.shape(), s );
	ASSERT_EQ( a.nrows(), m );
	ASSERT_EQ( a.ncolumns(), n );
	ASSERT_EQ( a.nelems(), m * n );
	ASSERT_EQ( a.lead_dim(), m );
	ASSERT_EQ( a.is_continuous(), true );

	ASSERT_EQ( a.is_matrix(), true );
	ASSERT_EQ( a.is_row(), (m == 1) );
	ASSERT_EQ( a.is_column(), (n == 1) );
	ASSERT_EQ( a.is_scalar(), (m == 1 && n == 1) );
	ASSERT_EQ( a.is_vector(), (m == 1 || n == 1) );
	ASSERT_EQ( a.is_empty(), (m == 0 || n == 0) );
}


MN_CASE( cont_cm_layout, offsets )
{
	const index_t m = M == 0 ? 3 : M;
	const index_t n = N == 0 ? 4 : N;

	continuous_column_major_layout<M, N> a(m, n);

	for (index_t i = 0; i < m * n; ++i)
	{
		ASSERT_EQ(a.offset(i), i);
	}

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			ASSERT_EQ(a.offset(i, j), (i + j * m));
		}
	}
}


MN_CASE( cm_layout, basics )
{
	const index_t m = M == 0 ? 3 : M;
	const index_t n = N == 0 ? 4 : N;
	const index_t ldim = 7;

	array_shape<M, N> s(m, n);
	column_major_layout<M, N> a(m, n, ldim);

	ASSERT_EQ( a.shape(), s );
	ASSERT_EQ( a.nrows(), m );
	ASSERT_EQ( a.ncolumns(), n );
	ASSERT_EQ( a.nelems(), m * n );
	ASSERT_EQ( a.lead_dim(), ldim );
	ASSERT_EQ( a.is_continuous(), (m == ldim || n == 1) );

	ASSERT_EQ( a.is_matrix(), true );
	ASSERT_EQ( a.is_row(), (m == 1) );
	ASSERT_EQ( a.is_column(), (n == 1) );
	ASSERT_EQ( a.is_scalar(), (m == 1 && n == 1) );
	ASSERT_EQ( a.is_vector(), (m == 1 || n == 1) );
	ASSERT_EQ( a.is_empty(), (m == 0 || n == 0) );
}


MN_CASE( cm_layout, offsets )
{
	const index_t m = M == 0 ? 3 : M;
	const index_t n = N == 0 ? 4 : N;
	const index_t ldim = 7;

	column_major_layout<M, N> a(m, n, ldim);

	if (N == 1)
	{
		for (index_t i = 0; i < m; ++i)
		{
			ASSERT_EQ(a.offset(i), i);
		}
	}
	else if (M == 1)
	{
		for (index_t i = 0; i < n; ++i)
		{
			ASSERT_EQ(a.offset(i), i * ldim);
		}
	}

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			ASSERT_EQ(a.offset(i, j), (i + j * ldim));
		}
	}
}


BEGIN_TPACK( cont_cm_layout_basics )
	ADD_MN_CASE_3X3( cont_cm_layout, basics, 3, 4 )
END_TPACK

BEGIN_TPACK( cont_cm_layout_offsets )
	ADD_MN_CASE_3X3( cont_cm_layout, offsets, 3, 4 )
END_TPACK

BEGIN_TPACK( cm_layout_basics )
	ADD_MN_CASE_3X3( cm_layout, basics, 3, 4 )
END_TPACK

BEGIN_TPACK( cm_layout_offsets )
	ADD_MN_CASE_3X3( cm_layout, offsets, 3, 4 )
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( cont_cm_layout_basics )
	ADD_TPACK( cont_cm_layout_offsets )
	ADD_TPACK( cm_layout_basics )
	ADD_TPACK( cm_layout_offsets )
END_MAIN_SUITE




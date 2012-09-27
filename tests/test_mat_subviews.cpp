/**
 * @file test_mat_subviews.cpp
 *
 * Unit testing of matrix subviews
 *
 * @author Dahua Lin
 */

#include "test_base.h"
#include <light_mat/matrix/matrix_classes.h>

using namespace lmat;
using namespace lmat::test;

// auxiliary functions

void fill_lin(dblock<double>& arr)
{
	for (index_t i = 0; i < arr.nelems(); ++i)
		arr[i] = double(i + 1);
}

template<typename T, class LMat>
tarray<T> extract_block(const IDenseArray<LMat, T>& src,
		const index_t i0, const index_t bm,
		const index_t j0, const index_t bn)
{
	tarray<T> blk(bm, bn);
	for (index_t j = 0; j < bn; ++j)
	{
		for (index_t i = 0; i < bm; ++i)
		{
			blk(i, j) = src(i0 + i, j0 + j);
		}
	}
	return blk;
}


// test cases

MN_CASE( mat_subview, col )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	for (index_t j = 0; j < n; ++j)
	{
		mat_f64 r = extract_block(a, 0, m, j, 1);
		ASSERT_TRUE( is_equal(a.column(j), r) );
		ASSERT_TRUE( is_equal(ac.column(j), r) );
	}

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	for (index_t j = 0; j < n; ++j)
	{
		mat_f64 r = extract_block(b, 0, m, j, 1);
		ASSERT_TRUE( is_equal(b.column(j), r) );
		ASSERT_TRUE( is_equal(bc.column(j), r) );
	}

	tarray<double, M, 1> c(m, 1);
	for (index_t i = 0; i < m; ++i) c[i] = double(2 * i + 3);

	for (index_t j = 0; j < n; ++j)
	{
		b.column(j) = c;
		ASSERT_TRUE( is_equal(b.column(j), c) );
	}
}


MN_CASE( mat_subview, row )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	for (index_t i = 0; i < m; ++i)
	{
		mat_f64 r = extract_block(a, i, 1, 0, n);
		ASSERT_TRUE( is_equal(a.row(i), r) );
		ASSERT_TRUE( is_equal(ac.row(i), r) );
	}

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	for (index_t i = 0; i < m; ++i)
	{
		mat_f64 r = extract_block(b, i, 1, 0, n);
		ASSERT_TRUE( is_equal(b.row(i), r) );
		ASSERT_TRUE( is_equal(bc.row(i), r) );
	}

	tarray<double, 1, N> c(1, n);
	for (index_t j = 0; j < n; ++j) c[j] = double(2 * j + 3);

	for (index_t i = 0; i < m; ++i)
	{
		b.row(i) = c;
		ASSERT_TRUE( is_equal(b.row(i), c) );
	}
}


MN_CASE( mat_subview, col_whole )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	for (index_t j = 0; j < n; ++j)
	{
		mat_f64 r = extract_block(a, 0, m, j, 1);
		ASSERT_TRUE( is_equal(a(whole(), j), r) );
		ASSERT_TRUE( is_equal(ac(whole(), j), r) );
	}

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	for (index_t j = 0; j < n; ++j)
	{
		mat_f64 r = extract_block(b, 0, m, j, 1);
		ASSERT_TRUE( is_equal(b(whole(), j), r) );
		ASSERT_TRUE( is_equal(bc(whole(), j), r) );
	}

	tarray<double, M, 1> c(m, 1);
	for (index_t i = 0; i < m; ++i) c[i] = double(2 * i + 3);

	for (index_t j = 0; j < n; ++j)
	{
		b(whole(), j) = c;
		ASSERT_TRUE( is_equal(b(whole(), j), c) );
	}
}


MN_CASE( mat_subview, row_whole )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	for (index_t i = 0; i < m; ++i)
	{
		mat_f64 r = extract_block(a, i, 1, 0, n);
		ASSERT_TRUE( is_equal(a(i, whole()), r) );
		ASSERT_TRUE( is_equal(ac(i, whole()), r) );
	}

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	for (index_t i = 0; i < m; ++i)
	{
		mat_f64 r = extract_block(b, i, 1, 0, n);
		ASSERT_TRUE( is_equal(b(i, whole()), r) );
		ASSERT_TRUE( is_equal(bc(i, whole()), r) );
	}

	tarray<double, 1, N> c(1, n);
	for (index_t j = 0; j < n; ++j) c[j] = double(2 * j + 3);

	for (index_t i = 0; i < m; ++i)
	{
		b(i, whole()) = c;
		ASSERT_TRUE( is_equal(b(i, whole()), c) );
	}
}


MN_CASE( mat_subview, col_range )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	index_t i0, i1;
	if (m > 2)
	{
		i0 = 1;
		i1 = m - 1;
	}
	else
	{
		i0 = 0;
		i1 = m;
	}

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	for (index_t j = 0; j < n; ++j)
	{
		mat_f64 r = extract_block(a, i0, i1 - i0, j, 1);
		ASSERT_TRUE( is_equal(a(colon(i0, i1), j), r) );
		ASSERT_TRUE( is_equal(ac(colon(i0, i1), j), r) );
	}

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	for (index_t j = 0; j < n; ++j)
	{
		mat_f64 r = extract_block(b, i0, i1 - i0, j, 1);
		ASSERT_TRUE( is_equal(b(colon(i0, i1), j), r) );
		ASSERT_TRUE( is_equal(bc(colon(i0, i1), j), r) );
	}

	tarray<double, 0, 1> c(i1 - i0, 1);
	for (index_t i = 0; i < i1 - i0; ++i) c[i] = double(2 * i + 3);

	for (index_t j = 0; j < n; ++j)
	{
		b(colon(i0, i1), j) = c;
		ASSERT_TRUE( is_equal(b(colon(i0, i1), j), c) );
	}
}


MN_CASE( mat_subview, row_range )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	index_t j0, j1;
	if (n > 2)
	{
		j0 = 1;
		j1 = n - 1;
	}
	else
	{
		j0 = 0;
		j1 = n;
	}

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	for (index_t i = 0; i < m; ++i)
	{
		mat_f64 r = extract_block(a, i, 1, j0, j1 - j0);
		ASSERT_TRUE( is_equal(a(i, colon(j0, j1)), r) );
		ASSERT_TRUE( is_equal(ac(i, colon(j0, j1)), r) );
	}

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	for (index_t i = 0; i < m; ++i)
	{
		mat_f64 r = extract_block(b, i, 1, j0, j1 - j0);
		ASSERT_TRUE( is_equal(b(i, colon(j0, j1)), r) );
		ASSERT_TRUE( is_equal(bc(i, colon(j0, j1)), r) );
	}

	tarray<double, 1, 0> c(1, j1 - j0);
	for (index_t j = 0; j < j1 - j0; ++j) c[j] = double(2 * j + 3);

	for (index_t i = 0; i < m; ++i)
	{
		b(i, colon(j0, j1)) = c;
		ASSERT_TRUE( is_equal(b(i, colon(j0, j1)), c) );
	}
}


MN_CASE( mat_subview, whole_whole )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	mat_f64 ra = extract_block(a, 0, m, 0, n);
	ASSERT_TRUE( is_equal(a(whole(), whole()), ra) );
	ASSERT_TRUE( is_equal(ac(whole(), whole()), ra) );

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	mat_f64 rb = extract_block(b, 0, m, 0, n);
	ASSERT_TRUE( is_equal(b(whole(), whole()), rb) );
	ASSERT_TRUE( is_equal(bc(whole(), whole()), rb) );

	for (index_t i = 0; i < rb.nelems(); ++i) rb[i] = double(2 * i + 5);
	b(whole(), whole()) = rb;
	ASSERT_TRUE( is_equal(b(whole(), whole()), rb) );
}


MN_CASE( mat_subview, whole_range )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	index_t j0, j1;
	if (n > 2)
	{
		j0 = 1; j1 = n - 1;
	}
	else
	{
		j0 = 0; j1 = n;
	}

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	mat_f64 ra = extract_block(a, 0, m, j0, j1 - j0);
	ASSERT_TRUE( is_equal(a(whole(), colon(j0, j1)), ra) );
	ASSERT_TRUE( is_equal(ac(whole(), colon(j0, j1)), ra) );

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	mat_f64 rb = extract_block(b, 0, m, j0, j1 - j0);
	ASSERT_TRUE( is_equal(b(whole(), colon(j0, j1)), rb) );
	ASSERT_TRUE( is_equal(bc(whole(), colon(j0, j1)), rb) );

	for (index_t i = 0; i < rb.nelems(); ++i) rb[i] = double(2 * i + 5);
	b(whole(), colon(j0, j1)) = rb;
	ASSERT_TRUE( is_equal(b(whole(), colon(j0, j1)), rb) );
}


MN_CASE( mat_subview, range_whole )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	index_t i0, i1;
	if (m > 2)
	{
		i0 = 1; i1 = m - 1;
	}
	else
	{
		i0 = 0; i1 = m;
	}

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	mat_f64 ra = extract_block(a, i0, i1 - i0, 0, n);
	ASSERT_TRUE( is_equal(a(colon(i0, i1), whole()), ra) );
	ASSERT_TRUE( is_equal(ac(colon(i0, i1), whole()), ra) );

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	mat_f64 rb = extract_block(b, i0, i1 - i0, 0, n);
	ASSERT_TRUE( is_equal(b(colon(i0, i1), whole()), rb) );
	ASSERT_TRUE( is_equal(bc(colon(i0, i1), whole()), rb) );

	for (index_t i = 0; i < rb.nelems(); ++i) rb[i] = double(2 * i + 5);
	b(colon(i0, i1), whole()) = rb;
	ASSERT_TRUE( is_equal(b(colon(i0, i1), whole()), rb) );
}


MN_CASE( mat_subview, range_range )
{
	typedef tarray_ref<double, M, N> rmat;
	typedef tarray_ref_ex<double, M, N> rmat_ex;

	const index_t m = M == 0 ? 7 : M;
	const index_t n = N == 0 ? 8 : N;
	const index_t ldim_ex = 10;

	index_t i0, i1, j0, j1;

	if (m > 2)
	{
		i0 = 1; i1 = m - 1;
	}
	else
	{
		i0 = 0; i1 = m;
	}

	if (n > 2)
	{
		j0 = 1; j1 = n - 1;
	}
	else
	{
		j0 = 0; j1 = n;
	}

	dblock<double> s(ldim_ex * n);
	fill_lin(s);

	rmat a(s.ptr_data(), m, n);
	const rmat& ac = a;

	mat_f64 ra = extract_block(a, i0, i1 - i0, j0, j1 - j0);
	ASSERT_TRUE( is_equal(a(colon(i0, i1), colon(j0, j1)), ra) );
	ASSERT_TRUE( is_equal(ac(colon(i0, i1), colon(j0, j1)), ra) );

	rmat_ex b(s.ptr_data(), m, n, ldim_ex);
	const rmat_ex& bc = b;

	mat_f64 rb = extract_block(b, i0, i1 - i0, j0, j1 - j0);
	ASSERT_TRUE( is_equal(b(colon(i0, i1), colon(j0, j1)), rb) );
	ASSERT_TRUE( is_equal(bc(colon(i0, i1), colon(j0, j1)), rb) );

	for (index_t i = 0; i < rb.nelems(); ++i) rb[i] = double(2 * i + 5);
	b(colon(i0, i1), colon(j0, j1)) = rb;
	ASSERT_TRUE( is_equal(b(colon(i0, i1), colon(j0, j1)), rb) );
}



BEGIN_TPACK( mat_col_view )
	ADD_MN_CASE_3X3( mat_subview, col, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_row_view )
	ADD_MN_CASE_3X3( mat_subview, row, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_col_whole )
	ADD_MN_CASE_3X3( mat_subview, col_whole, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_row_whole )
	ADD_MN_CASE_3X3( mat_subview, row_whole, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_col_range )
	ADD_MN_CASE_3X3( mat_subview, col_range, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_row_range )
	ADD_MN_CASE_3X3( mat_subview, row_range, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_whole_whole )
	ADD_MN_CASE_3X3( mat_subview, whole_whole, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_whole_range )
	ADD_MN_CASE_3X3( mat_subview, whole_range, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_range_whole )
	ADD_MN_CASE_3X3( mat_subview, range_whole, 7, 8 );
END_TPACK

BEGIN_TPACK( mat_range_range )
	ADD_MN_CASE_3X3( mat_subview, range_range, 7, 8 );
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( mat_col_view )
	ADD_TPACK( mat_row_view )
	ADD_TPACK( mat_col_whole )
	ADD_TPACK( mat_row_whole )
	ADD_TPACK( mat_col_range )
	ADD_TPACK( mat_row_range )

	ADD_TPACK( mat_whole_whole )
	ADD_TPACK( mat_whole_range )
	ADD_TPACK( mat_range_whole )
	ADD_TPACK( mat_range_range )
END_MAIN_SUITE






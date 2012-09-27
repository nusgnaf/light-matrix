/**
 * @file test_ref_col.cpp
 *
 * Unit testing of cref_col and ref_col class.
 *
 * @author Dahua Lin
 */


#include "test_base.h"

#include <light_mat/matrix/ref_matrix.h>
#include <light_mat/common/block.h>

using namespace lmat;
using namespace lmat::test;

// explicit instantiation

template class lmat::tcol_ref<double, 0>;
template class lmat::tcol_ref<double, 4>;

#ifdef LMAT_USE_STATIC_ASSERT

static_assert(lmat::is_base_of<
		lmat::tarray_ref<double, 0, 1>,
		lmat::tcol_ref<double, 0> >::value, "Base verification failed.");

static_assert(lmat::is_base_of<
		lmat::tarray_ref<double, 4, 1>,
		lmat::tcol_ref<double, 4> >::value, "Base verification failed.");

#endif

N_CASE( tcol_cref, constructs )
{
	const index_t n = N == 0 ? 4 : N;

	dblock<double> s(n);
	const double *ps = s.ptr_data();

	tcol_cref<double, N> a(ps, n);

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);
	ASSERT_EQ(a.size(), (size_t)a.nelems() );

	ASSERT_EQ(a.ptr_data(), ps);

	tcol_cref<double, N> a2(a);

	ASSERT_EQ(a2.nrows(), n);
	ASSERT_EQ(a2.ncolumns(), 1);
	ASSERT_EQ(a2.nelems(), n);
	ASSERT_EQ(a2.lead_dim(), n);
	ASSERT_EQ(a2.size(), (size_t)a2.nelems() );

	ASSERT_EQ(a.ptr_data(), ps);
}


N_CASE( tcol_ref, constructs )
{
	const index_t n = N == 0 ? 4 : N;

	dblock<double> s(n);
	double *ps = s.ptr_data();

	tcol_ref<double, N> a(ps, n);

	ASSERT_EQ(a.nrows(), n);
	ASSERT_EQ(a.ncolumns(), 1);
	ASSERT_EQ(a.nelems(), n);
	ASSERT_EQ(a.lead_dim(), n);
	ASSERT_EQ(a.size(), (size_t)a.nelems() );

	ASSERT_EQ(a.ptr_data(), ps);

	tcol_ref<double, N> a2(a);

	ASSERT_EQ(a2.nrows(), n);
	ASSERT_EQ(a2.ncolumns(), 1);
	ASSERT_EQ(a2.nelems(), n);
	ASSERT_EQ(a2.lead_dim(), n);
	ASSERT_EQ(a2.size(), (size_t)a2.nelems() );

	ASSERT_EQ(a.ptr_data(), ps);
}


N_CASE( tcol_ref, assign )
{
	const index_t n = N == 0 ? 4 : N;

	dblock<double> s1(n);
	dblock<double> s2(n);

	double *ps1 = s1.ptr_data();
	double *ps2 = s2.ptr_data();

	for (index_t i = 0; i < n; ++i) s1[i] = double(i + 2);
	for (index_t i = 0; i < n; ++i) s2[i] = double(2 * i + 3);

	tcol_ref<double, N> a1(ps1, n);
	tcol_ref<double, N> a2(ps2, n);

	ASSERT_EQ( a1.ptr_data(), ps1 );
	ASSERT_EQ( a2.ptr_data(), ps2 );
	ASSERT_NE( ps1, ps2 );

	a1 = a2;

	ASSERT_EQ( a1.ptr_data(), ps1 );
	ASSERT_EQ( a2.ptr_data(), ps2 );

	ASSERT_VEC_EQ( n, a1, a2 );
}


N_CASE( tcol_ref, import )
{
	const index_t n = N == 0 ? 4 : N;

	dblock<double> ref(n);
	dblock<double> s(n, fill(-1.0));

	double *ps = s.ptr_data();
	tcol_ref<double, N> a(ps, n);

	// fill_value

	const double v1 = 2.5;
	a << v1;
	for (index_t i = 0; i < n; ++i) ref[i] = v1;

	ASSERT_EQ(a.ptr_data(), ps);
	ASSERT_VEC_EQ(n, a, ref);

	// copy_value

	for (index_t i = 0; i < n; ++i) ref[i] = double(i + 2);
	a << ref.ptr_data();

	ASSERT_EQ(a.ptr_data(), ps);
	ASSERT_VEC_EQ(n, a, ref);
}


BEGIN_TPACK( cref_col_constructs )
	ADD_N_CASE( tcol_cref, constructs, 0 )
	ADD_N_CASE( tcol_cref, constructs, 1 )
	ADD_N_CASE( tcol_cref, constructs, 4 )
END_TPACK

BEGIN_TPACK( ref_col_constructs )
	ADD_N_CASE( tcol_ref, constructs, 0 )
	ADD_N_CASE( tcol_ref, constructs, 1 )
	ADD_N_CASE( tcol_ref, constructs, 4 )
END_TPACK

BEGIN_TPACK( ref_col_assign )
	ADD_N_CASE( tcol_ref, assign, 0 )
	ADD_N_CASE( tcol_ref, assign, 1 )
	ADD_N_CASE( tcol_ref, assign, 4 )
END_TPACK

BEGIN_TPACK( ref_col_import )
	ADD_N_CASE( tcol_ref, import, 0 )
	ADD_N_CASE( tcol_ref, import, 1 )
	ADD_N_CASE( tcol_ref, import, 4 )
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( cref_col_constructs )

	ADD_TPACK( ref_col_constructs )
	ADD_TPACK( ref_col_assign )
	ADD_TPACK( ref_col_import )
END_MAIN_SUITE




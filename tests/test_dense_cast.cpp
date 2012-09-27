/**
 * @file test_dense_cast.cpp
 *
 * Unit testing of casting for dense matrices
 *
 * @author Dahua Lin
 */

#include "test_base.h"

#include <light_mat/matrix/matrix_classes.h>
#include <light_mat/matrix/matrix_cast.h>
#include <light_mat/matrix/matrix_ewise_eval.h>

using namespace lmat;
using namespace lmat::test;


MN_CASE( dense_cast, explicit_cast )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	tarray<double, M, N> A(m, n);
	tarray<float,  M, N> Br(m, n);

	for (index_t i = 0; i < m * n; ++i)
	{
		int v = i + 2;
		A[i] = double(v);
		Br[i] = float(v);
	}

	tarray<float, M, N> B = to_f32(A);

	ASSERT_TRUE( is_equal(B, Br) );
}


MN_CASE( ref_cast, explicit_cast )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	dblock<double> sA(m * n);
	dblock<float> sB(m * n);

	tarray_ref<double, M, N> A(sA.ptr_data(), m, n);
	tarray<float, M, N> Br(m, n);

	for (index_t i = 0; i < m * n; ++i)
	{
		index_t v = i + 2;
		A[i] = double(v);
		Br[i] = float(v);
	}

	tarray_ref<float, M, N> B(sB.ptr_data(), m, n);
	B = to_f32(A);

	ASSERT_TRUE( is_equal(B, Br) );
}



MN_CASE( ref_ex_cast, explicit_cast )
{
	const index_t ldim_a = 7;
	const index_t ldim_b = 8;
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	dblock<double> sA(ldim_a * n, zero());
	dblock<float> sB(ldim_b * n, zero());

	tarray_ref_ex<double,  M, N> A(sA.ptr_data(), m, n, ldim_a);
	tarray<float, M, N> Br(m, n);

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			index_t v = i + j + 2;
			A(i, j) = double(v);
			Br(i, j) = float(v);
		}
	}

	tarray_ref_ex<float, M, N> B(sB.ptr_data(), m, n, ldim_b);
	B = to_f32(A);

	ASSERT_TRUE( is_equal(B, Br) );
}



BEGIN_TPACK( dense_explicit_cast )
	ADD_MN_CASE_3X3( dense_cast, explicit_cast, 5, 6 );
END_TPACK


BEGIN_TPACK( ref_explicit_cast )
	ADD_MN_CASE_3X3( ref_cast, explicit_cast, 5, 6 );
END_TPACK

BEGIN_TPACK( ref_ex_explicit_cast )
	ADD_MN_CASE_3X3( ref_ex_cast, explicit_cast, 5, 6 );
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( dense_explicit_cast )
	ADD_TPACK( ref_explicit_cast )
	ADD_TPACK( ref_ex_explicit_cast )
END_MAIN_SUITE






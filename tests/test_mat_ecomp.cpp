/*
 * @file test_mat_ecomp.cpp
 *
 * Unit testing of element-wise comparison
 *
 * @author Dahua Lin
 */

#include "test_base.h"

#include <light_mat/matrix/matrix_cast.h>
#include <light_mat/matrix/matrix_ecomp.h>
#include <light_mat/matrix/matrix_ewise_eval.h>

using namespace lmat;
using namespace lmat::test;

const int default_m = 8;
const int default_n = 6;
const index_t LDim = 12;

MN_CASE( mat_ecomp, eq )
{
	typedef tarray<double, M, N> mat_t;
	typedef tarray<bool, M, N> bmat_t;

	const index_t m = M == 0 ? default_m : M;
	const index_t n = N == 0 ? default_n : N;

	mat_t A(m, n);
	mat_t B(m, n);
	double c = double( (m * n) / 2 );

	for (index_t i = 0; i < m * n; ++i) A[i] = double(i + 1);
	for (index_t i = 0; i < m * n; ++i) B[i] = double((i + 1) * (i & 1));

	// prepare ground-truth

	bmat_t AB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AB_r[i] = (A[i] == B[i]);

	bmat_t AC_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AC_r[i] = (A[i] == c);

	bmat_t CB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) CB_r[i] = (c == B[i]);

	// default evaluation

	bmat_t AB = to_bool(A == B);
	ASSERT_TRUE( is_equal(AB, AB_r) );

	bmat_t AC = to_bool(A == c);
	ASSERT_TRUE( is_equal(AC, AC_r) );

	bmat_t CB = to_bool(c == B);
	ASSERT_TRUE( is_equal(CB, CB_r) );

	// by-scalars evaluation

	bmat_t AB_s(m, n);
	linear_by_scalars_evaluate(to_bool(A == B), AB_s);
	ASSERT_TRUE( is_equal(AB_s, AB_r) );

	bmat_t AC_s(m, n);
	linear_by_scalars_evaluate(to_bool(A == c), AC_s);
	ASSERT_TRUE( is_equal(AC_s, AC_r) );

	bmat_t CB_s(m, n);
	linear_by_scalars_evaluate(to_bool(c == B), CB_s);
	ASSERT_TRUE( is_equal(CB_s, CB_r) );
}


MN_CASE( mat_ecomp, ne )
{
	typedef tarray<double, M, N> mat_t;
	typedef tarray<bool, M, N> bmat_t;

	const index_t m = M == 0 ? default_m : M;
	const index_t n = N == 0 ? default_n : N;

	mat_t A(m, n);
	mat_t B(m, n);
	double c = double( (m * n) / 2 );

	for (index_t i = 0; i < m * n; ++i) A[i] = double(i + 1);
	for (index_t i = 0; i < m * n; ++i) B[i] = double((i + 1) * (i & 1));

	// prepare ground-truth

	bmat_t AB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AB_r[i] = (A[i] != B[i]);

	bmat_t AC_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AC_r[i] = (A[i] != c);

	bmat_t CB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) CB_r[i] = (c != B[i]);

	// default evaluation

	bmat_t AB = to_bool(A != B);
	ASSERT_TRUE( is_equal(AB, AB_r) );

	bmat_t AC = to_bool(A != c);
	ASSERT_TRUE( is_equal(AC, AC_r) );

	bmat_t CB = to_bool(c != B);
	ASSERT_TRUE( is_equal(CB, CB_r) );

	// by-scalars evaluation

	bmat_t AB_s(m, n);
	linear_by_scalars_evaluate(to_bool(A != B), AB_s);
	ASSERT_TRUE( is_equal(AB_s, AB_r) );

	bmat_t AC_s(m, n);
	linear_by_scalars_evaluate(to_bool(A != c), AC_s);
	ASSERT_TRUE( is_equal(AC_s, AC_r) );

	bmat_t CB_s(m, n);
	linear_by_scalars_evaluate(to_bool(c != B), CB_s);
	ASSERT_TRUE( is_equal(CB_s, CB_r) );
}


MN_CASE( mat_ecomp, le )
{
	typedef tarray<double, M, N> mat_t;
	typedef tarray<bool, M, N> bmat_t;

	const index_t m = M == 0 ? default_m : M;
	const index_t n = N == 0 ? default_n : N;

	mat_t A(m, n);
	mat_t B(m, n);
	double c = double( (m * n) / 2 );

	for (index_t i = 0; i < m * n; ++i) A[i] = double(i + 1);
	for (index_t i = 0; i < m * n; ++i) B[i] = double((i + 1) * (i & 1));

	// prepare ground-truth

	bmat_t AB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AB_r[i] = (A[i] <= B[i]);

	bmat_t AC_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AC_r[i] = (A[i] <= c);

	bmat_t CB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) CB_r[i] = (c <= B[i]);

	// default evaluation

	bmat_t AB = to_bool(A <= B);
	ASSERT_TRUE( is_equal(AB, AB_r) );

	bmat_t AC = to_bool(A <= c);
	ASSERT_TRUE( is_equal(AC, AC_r) );

	bmat_t CB = to_bool(c <= B);
	ASSERT_TRUE( is_equal(CB, CB_r) );

	// by-scalars evaluation

	bmat_t AB_s(m, n);
	linear_by_scalars_evaluate(to_bool(A <= B), AB_s);
	ASSERT_TRUE( is_equal(AB_s, AB_r) );

	bmat_t AC_s(m, n);
	linear_by_scalars_evaluate(to_bool(A <= c), AC_s);
	ASSERT_TRUE( is_equal(AC_s, AC_r) );

	bmat_t CB_s(m, n);
	linear_by_scalars_evaluate(to_bool(c <= B), CB_s);
	ASSERT_TRUE( is_equal(CB_s, CB_r) );
}


MN_CASE( mat_ecomp, lt )
{
	typedef tarray<double, M, N> mat_t;
	typedef tarray<bool, M, N> bmat_t;

	const index_t m = M == 0 ? default_m : M;
	const index_t n = N == 0 ? default_n : N;

	mat_t A(m, n);
	mat_t B(m, n);
	double c = double( (m * n) / 2 );

	for (index_t i = 0; i < m * n; ++i) A[i] = double(i + 1);
	for (index_t i = 0; i < m * n; ++i) B[i] = double((i + 1) * (i & 1));

	// prepare ground-truth

	bmat_t AB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AB_r[i] = (A[i] < B[i]);

	bmat_t AC_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AC_r[i] = (A[i] < c);

	bmat_t CB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) CB_r[i] = (c < B[i]);

	// default evaluation

	bmat_t AB = to_bool(A < B);
	ASSERT_TRUE( is_equal(AB, AB_r) );

	bmat_t AC = to_bool(A < c);
	ASSERT_TRUE( is_equal(AC, AC_r) );

	bmat_t CB = to_bool(c < B);
	ASSERT_TRUE( is_equal(CB, CB_r) );

	// by-scalars evaluation

	bmat_t AB_s(m, n);
	linear_by_scalars_evaluate(to_bool(A < B), AB_s);
	ASSERT_TRUE( is_equal(AB_s, AB_r) );

	bmat_t AC_s(m, n);
	linear_by_scalars_evaluate(to_bool(A < c), AC_s);
	ASSERT_TRUE( is_equal(AC_s, AC_r) );

	bmat_t CB_s(m, n);
	linear_by_scalars_evaluate(to_bool(c < B), CB_s);
	ASSERT_TRUE( is_equal(CB_s, CB_r) );
}


MN_CASE( mat_ecomp, ge )
{
	typedef tarray<double, M, N> mat_t;
	typedef tarray<bool, M, N> bmat_t;

	const index_t m = M == 0 ? default_m : M;
	const index_t n = N == 0 ? default_n : N;

	mat_t A(m, n);
	mat_t B(m, n);
	double c = double( (m * n) / 2 );

	for (index_t i = 0; i < m * n; ++i) A[i] = double(i + 1);
	for (index_t i = 0; i < m * n; ++i) B[i] = double((i + 1) * (i & 1));

	// prepare ground-truth

	bmat_t AB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AB_r[i] = (A[i] >= B[i]);

	bmat_t AC_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AC_r[i] = (A[i] >= c);

	bmat_t CB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) CB_r[i] = (c >= B[i]);

	// default evaluation

	bmat_t AB = to_bool(A >= B);
	ASSERT_TRUE( is_equal(AB, AB_r) );

	bmat_t AC = to_bool(A >= c);
	ASSERT_TRUE( is_equal(AC, AC_r) );

	bmat_t CB = to_bool(c >= B);
	ASSERT_TRUE( is_equal(CB, CB_r) );

	// by-scalars evaluation

	bmat_t AB_s(m, n);
	linear_by_scalars_evaluate(to_bool(A >= B), AB_s);
	ASSERT_TRUE( is_equal(AB_s, AB_r) );

	bmat_t AC_s(m, n);
	linear_by_scalars_evaluate(to_bool(A >= c), AC_s);
	ASSERT_TRUE( is_equal(AC_s, AC_r) );

	bmat_t CB_s(m, n);
	linear_by_scalars_evaluate(to_bool(c >= B), CB_s);
	ASSERT_TRUE( is_equal(CB_s, CB_r) );
}


MN_CASE( mat_ecomp, gt )
{
	typedef tarray<double, M, N> mat_t;
	typedef tarray<bool, M, N> bmat_t;

	const index_t m = M == 0 ? default_m : M;
	const index_t n = N == 0 ? default_n : N;

	mat_t A(m, n);
	mat_t B(m, n);
	double c = double( (m * n) / 2 );

	for (index_t i = 0; i < m * n; ++i) A[i] = double(i + 1);
	for (index_t i = 0; i < m * n; ++i) B[i] = double((i + 1) * (i & 1));

	// prepare ground-truth

	bmat_t AB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AB_r[i] = (A[i] > B[i]);

	bmat_t AC_r(m, n);
	for (index_t i = 0; i < m * n; ++i) AC_r[i] = (A[i] > c);

	bmat_t CB_r(m, n);
	for (index_t i = 0; i < m * n; ++i) CB_r[i] = (c > B[i]);

	// default evaluation

	bmat_t AB = to_bool(A > B);
	ASSERT_TRUE( is_equal(AB, AB_r) );

	bmat_t AC = to_bool(A > c);
	ASSERT_TRUE( is_equal(AC, AC_r) );

	bmat_t CB = to_bool(c > B);
	ASSERT_TRUE( is_equal(CB, CB_r) );

	// by-scalars evaluation

	bmat_t AB_s(m, n);
	linear_by_scalars_evaluate(to_bool(A > B), AB_s);
	ASSERT_TRUE( is_equal(AB_s, AB_r) );

	bmat_t AC_s(m, n);
	linear_by_scalars_evaluate(to_bool(A > c), AC_s);
	ASSERT_TRUE( is_equal(AC_s, AC_r) );

	bmat_t CB_s(m, n);
	linear_by_scalars_evaluate(to_bool(c > B), CB_s);
	ASSERT_TRUE( is_equal(CB_s, CB_r) );
}


BEGIN_TPACK( mat_ecomp_eq )
	ADD_MN_CASE_3X3( mat_ecomp, eq, default_m, default_n )
END_TPACK

BEGIN_TPACK( mat_ecomp_ne )
	ADD_MN_CASE_3X3( mat_ecomp, ne, default_m, default_n )
END_TPACK

BEGIN_TPACK( mat_ecomp_le )
	ADD_MN_CASE_3X3( mat_ecomp, le, default_m, default_n )
END_TPACK

BEGIN_TPACK( mat_ecomp_lt )
	ADD_MN_CASE_3X3( mat_ecomp, lt, default_m, default_n )
END_TPACK

BEGIN_TPACK( mat_ecomp_ge )
	ADD_MN_CASE_3X3( mat_ecomp, ge, default_m, default_n )
END_TPACK

BEGIN_TPACK( mat_ecomp_gt )
	ADD_MN_CASE_3X3( mat_ecomp, gt, default_m, default_n )
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( mat_ecomp_eq )
	ADD_TPACK( mat_ecomp_ne )
	ADD_TPACK( mat_ecomp_le )
	ADD_TPACK( mat_ecomp_lt )
	ADD_TPACK( mat_ecomp_ge )
	ADD_TPACK( mat_ecomp_gt )
END_MAIN_SUITE




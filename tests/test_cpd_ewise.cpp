/**
 * @file test_cpd_ewise.cpp
 *
 * Unit testing of compound ewise expressions
 *
 * @author Dahua Lin
 */

#include "test_base.h"

#include <light_mat/matrix/matrix_eval.h>
#include <light_mat/math/math_constants.h>
#include <cstdlib>

using namespace lmat;
using namespace lmat::test;


// auxiliary functions

template<int M, int N>
void fill_ran(tarray<double, M, N>& X, double a, double b)
{
	for (index_t i = 0; i < X.nelems(); ++i)
	{
		X[i] = a + (double(std::rand()) / RAND_MAX) * (b - a);
	}
}

// test cases

MN_CASE( cpd_expr, ewise_sqdist )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	tarray<double, M, N> A(m, n);
	tarray<double, M, N> B(m, n);

	fill_ran(A, -5.0, 5.0);
	fill_ran(B, -5.0, 5.0);
	const double tol = 1.0e-12;

	tarray<double, M, N> Y = sqr(A - B);

	tarray<double, M, N> R(m, n);

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			R(i, j) = math::sqr(A(i,j) - B(i,j));
		}
	}

	ASSERT_TRUE( is_approx(Y, R, tol) );
}


MN_CASE( cpd_expr, ewise_normpdf )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	tarray<double, M, N> X(m, n);

	fill_ran(X, -4.0, 6.0);
	const double tol = 1.0e-12;

	const double mu = 1.2;
	const double sigma2 = 2.5;

	double hrpi = lmat::math::consts<double>::rcp_two_rpi();
	const double c = math::sqrt(hrpi / sigma2);

	tarray<double, M, N> Y = c * exp( - sqr(X - mu) / (2.0 * sigma2) );

	const_array<double, M, N> mu_mat(m, n, mu);
	tarray<double, M, N> Y2 = c * exp( - sqr(X - mu_mat) * (1 / (2.0 * sigma2)) );

	tarray<double, M, N> R(m, n);

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			R(i,j) = c * math::exp( - math::sqr(X(i,j) - mu) / (2.0 * sigma2) );
		}
	}

	ASSERT_TRUE( is_approx(Y, R, tol) );
	ASSERT_TRUE( is_approx(Y2, R, tol) );
}


MN_CASE( cpd_expr, axpy_cast )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	const double a = 1.5;
	tarray<double, M, N> X(m, n);
	tarray<double, M, N> Y(m, n);

	fill_ran(X, -5.0, 5.0);
	fill_ran(Y, -5.0, 5.0);
	const float tol = 1.0e-5f;

	tarray<float, M, N> Z = cast(a * X + Y, type<float>());
	tarray<float, M, N> R(m, n);

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			R(i, j) = float(a * X(i,j) + Y(i,j));
		}
	}

	ASSERT_TRUE( is_approx(Z, R, tol) );
}


MN_CASE( cpd_expr, pwise_sqdist )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	tcol<double, M> vx(m);
	trow<double, N> vy(n);

	fill_ran(vx, -5.0, 5.0);
	fill_ran(vy, -5.0, 5.0);
	const double tol = 1.0e-12;

	tarray<double, M, N> Y =
			  hrep(sqr(vx), n)
			+ vrep(sqr(vy), m)
			- 2.0 * hrep(vx, n) * vrep(vy, m);

	tarray<double, M, N> R(m, n);

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			R(i,j) = math::sqr(vx[i] - vy[j]);
		}
	}

	ASSERT_TRUE( is_approx(Y, R, tol) );
}


MN_CASE( cpd_expr, pwise_sqdist2 )
{
	const index_t m = M == 0 ? 5 : M;
	const index_t n = N == 0 ? 6 : N;

	tcol<double, M> vx(m);
	tcol<double, N> vy(n);

	fill_ran(vx, -5.0, 5.0);
	fill_ran(vy, -5.0, 5.0);


	tarray<double, M, N> Y =
			  hrep(sqr(vx), n)
			+ hrep(sqr(vy), m).trans()
			- 2.0 * hrep(vx, n) * vrep(vy.trans(), m);

	tarray<double, M, N> R(m, n);

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			R(i,j) = math::sqr(vx[i] - vy[j]);
		}
	}

	const double tol = 1.0e-12;
	ASSERT_TRUE( is_approx(Y, R, tol) );
}


BEGIN_TPACK( cpd_ewise_sqdist )
	ADD_MN_CASE_3X3( cpd_expr, ewise_sqdist, 5, 6 )
END_TPACK

BEGIN_TPACK( cpd_ewise_normpdf )
	ADD_MN_CASE_3X3( cpd_expr, ewise_normpdf, 5, 6 )
END_TPACK

BEGIN_TPACK( cpd_ewise_axpy_cast )
	ADD_MN_CASE_3X3( cpd_expr, axpy_cast, 5, 6 )
END_TPACK

BEGIN_TPACK( cpd_pwise_sqdist )
	ADD_MN_CASE_3X3( cpd_expr, pwise_sqdist, 5, 6 )
END_TPACK

BEGIN_TPACK( cpd_pwise_sqdist2 )
	ADD_MN_CASE_3X3( cpd_expr, pwise_sqdist2, 5, 6 )
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( cpd_ewise_sqdist )
	ADD_TPACK( cpd_ewise_normpdf )
	ADD_TPACK( cpd_ewise_axpy_cast )
	ADD_TPACK( cpd_pwise_sqdist )
	ADD_TPACK( cpd_pwise_sqdist2 )
END_MAIN_SUITE




/**
 * @file test_memory.cpp
 *
 * @brief Unit testing of memory operations
 *
 * @author Dahua Lin
 */

#include "../test_base.h"

#include <light_mat/common/memory.h>

using namespace lmat;
using namespace lmat::test;

inline void fill_lin(index_t n, double *x)
{
	for (index_t i = 0; i < n; ++i) x[i] = double(i + 1);
}


SIMPLE_CASE( zero_cont )
{
	const index_t n = 10;
	double *x = new double[n];
	double *xr = new double[n];

	for (index_t i = 0; i < n; ++i) x[i] = double(-1);
	for (index_t i = 0; i < n; ++i) xr[i] = double(0);

	zero_vec(n, x);

	ASSERT_VEC_EQ(n, x, xr);

	delete [] x;
	delete [] xr;
}


SIMPLE_CASE( zero_step )
{
	const index_t n = 10;
	const index_t m = 5;
	const index_t sx = 2;

	double *x = new double[n];
	double *xr = new double[n];

	for (index_t i = 0; i < n; ++i) x[i] = double(-1);
	for (index_t i = 0; i < n; ++i) xr[i] = double(-1);
	for (index_t i = 0; i < m; ++i) xr[i * sx] = double(0);

	zero_vec(m, step_ptr(x, sx));

	ASSERT_VEC_EQ(n, x, xr);

	delete [] x;
	delete [] xr;
}


SIMPLE_CASE( fill_cont )
{
	const index_t n = 10;
	double *x = new double[n];
	double *xr = new double[n];

	for (index_t i = 0; i < n; ++i) x[i] = double(-1);
	for (index_t i = 0; i < n; ++i) xr[i] = double(12);

	fill_vec(n, x, double(12));

	ASSERT_VEC_EQ(n, x, xr);

	delete [] x;
	delete [] xr;
}


SIMPLE_CASE( fill_step )
{
	const index_t n = 10;
	const index_t m = 5;
	const index_t sx = 2;

	double *x = new double[n];
	double *xr = new double[n];

	for (index_t i = 0; i < n; ++i) x[i] = double(-1);
	for (index_t i = 0; i < n; ++i) xr[i] = double(-1);
	for (index_t i = 0; i < m; ++i) xr[i * sx] = double(12);

	fill_vec(m, step_ptr(x, sx), double(12));

	ASSERT_VEC_EQ(n, x, xr);

	delete [] x;
	delete [] xr;
}


SIMPLE_CASE( copy_cont_cont )
{
	const index_t n = 10;

	double *x = new double[n];
	double *y = new double[n];

	zero_vec(n, y);

	fill_lin(n, x);
	copy_vec(n, x, y);

	ASSERT_VEC_EQ(n, x, y);

	delete [] x;
	delete [] y;
}


SIMPLE_CASE( copy_cont_step )
{
	const index_t n = 10;
	const index_t sy = 2;
	const index_t m = 5;

	double *x = new double[m];
	double *y = new double[n];
	double *yr = new double[n];

	zero_vec(n, y);
	zero_vec(n, yr);

	fill_lin(m, x);
	for (index_t i = 0; i < m; ++i) yr[i * sy] = x[i];

	copy_vec(m, x, step_ptr(y, sy));

	ASSERT_VEC_EQ(n, y, yr);

	delete [] x;
	delete [] y;
	delete [] yr;
}

SIMPLE_CASE( copy_step_cont )
{
	const index_t n = 10;
	const index_t sx = 2;
	const index_t m = 5;

	double *x = new double[n];
	double *y = new double[m];
	double *yr = new double[m];

	zero_vec(m, y);
	zero_vec(m, yr);

	fill_lin(n, x);
	for (index_t i = 0; i < m; ++i) yr[i] = x[i * sx];

	copy_vec(m, step_ptr(x, sx), y);

	ASSERT_VEC_EQ(m, y, yr);

	delete [] x;
	delete [] y;
	delete [] yr;
}


SIMPLE_CASE( copy_step_step )
{
	const index_t nx = 15;
	const index_t ny = 10;
	const index_t sx = 3;
	const index_t sy = 2;
	const index_t m = 5;

	double *x = new double[nx];
	double *y = new double[ny];
	double *yr = new double[ny];

	zero_vec(ny, y);
	zero_vec(ny, yr);

	fill_lin(nx, x);
	for (index_t i = 0; i < m; ++i) yr[i * sy] = x[i * sx];

	copy_vec(m, step_ptr(x, sx), step_ptr(y, sy));

	ASSERT_VEC_EQ(ny, y, yr);

	delete [] x;
	delete [] y;
	delete [] yr;
}


AUTO_TPACK( zerovec )
{
	ADD_SIMPLE_CASE( zero_cont )
	ADD_SIMPLE_CASE( zero_step )
}

AUTO_TPACK( fillvec )
{
	ADD_SIMPLE_CASE( fill_cont )
	ADD_SIMPLE_CASE( fill_step )
}

AUTO_TPACK( copyvec )
{
	ADD_SIMPLE_CASE( copy_cont_cont )
	ADD_SIMPLE_CASE( copy_cont_step )
	ADD_SIMPLE_CASE( copy_step_cont )
	ADD_SIMPLE_CASE( copy_step_step )
}



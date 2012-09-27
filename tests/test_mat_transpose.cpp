/**
 * @file test_mat_transpose.cpp
 *
 * Unit testing for matrix transpose
 *
 * @author Dahua Lin
 */

#include "test_base.h"
#include <light_mat/matrix/matrix_eval.h>
#include <string>

using namespace lmat;
using namespace lmat::test;

const int DM = 6;
const int DN = 7;
const int LDim = 9;

template<class Mat>
void fill_lin(Mat& A)
{
	const index_t m = A.nrows();
	const index_t n = A.ncolumns();

	int v = 1;
	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			A(i, j) = double(v++);
		}
	}
}

template<class SMat, class DMat>
void my_transpose(const SMat& S, DMat& D)
{
	const index_t m = S.nrows();
	const index_t n = S.ncolumns();

	for (index_t j = 0; j < n; ++j)
	{
		for (index_t i = 0; i < m; ++i)
		{
			D(j, i) = S(i, j);
		}
	}
}


MN_CASE( mat_trans, dense )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	std::string base_name =
			N == 1 ? "contcol" : (M == 1 ? "controw" : "dense");

	tarray<double, M, N> S(m, n);
	fill_lin(S);

	ASSERT_STREQ( trans_base_typename(S.trans()), base_name  );

	tarray<double, N, M> T0(n, m);
	my_transpose(S, T0);

	tarray<double, N, M> T = S.trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );

	ASSERT_MAT_EQ( n, m, T, T0 );

	tarray<double, M, N> R = T.trans();

	ASSERT_EQ( R.nrows(), m );
	ASSERT_EQ( R.ncolumns(), n );

	ASSERT_MAT_EQ( m, n, R, S );
}


MN_CASE( mat_trans, refex )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	const char *base_name =
			N == 1 ? "contcol" : (M == 1 ? "regular_row" : "dense");

	dblock<double> sarr(LDim * n, fill(-1.0));

	tarray_ref_ex<double, M, N> S(sarr.ptr_data(), m, n, LDim);
	fill_lin(S);

	ASSERT_STREQ( trans_base_typename(S.trans()), base_name  );

	tarray<double, N, M> T0(n, m);
	my_transpose(S, T0);

	tarray<double, N, M> T = S.trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );

	ASSERT_MAT_EQ( n, m, T, T0 );
}


MN_CASE( mat_trans, const )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	double v = 12.5;
	const_array<double, M, N> S(m, n, v);

	const_array<double, N, M> T = S.trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );
	ASSERT_EQ( T.value(), v );
}


MN_CASE( mat_trans, unary_ewise )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	std::string base_name =
			M == 1 ? "rowxpr" : (N == 1 ? "colxpr" : "generic");

	tarray<double, M, N> S(m, n);
	fill_lin(S);

	ASSERT_STREQ( trans_base_typename(sqr(S).trans()), base_name );

	tarray<double, M, N> R = sqr(S);
	tarray<double, N, M> T0(n, m);
	my_transpose(R, T0);

	tarray<double, N, M> T = sqr(S).trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );

	ASSERT_MAT_EQ( n, m, T, T0 );

	dblock<double> darr(LDim * m, fill(-1.0));
	tarray_ref_ex<double, N, M> T2(darr.ptr_data(), n, m, LDim);

	T2 = sqr(S).trans();

	ASSERT_MAT_EQ( n, m, T2, T0 );
}


MN_CASE( mat_trans, binary_ewise )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	std::string base_name =
			M == 1 ? "rowxpr" : (N == 1 ? "colxpr" : "generic");

	tarray<double, M, N> S(m, n);
	fill_lin(S);
	tarray<double, M, N> S2 = sqr(S);

	ASSERT_STREQ( trans_base_typename((S + S2).trans()), base_name );

	tarray<double, M, N> R = S + S2;
	tarray<double, N, M> T0(n, m);
	my_transpose(R, T0);

	tarray<double, N, M> T = (S + S2).trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );

	ASSERT_MAT_EQ( n, m, T, T0 );

	dblock<double> darr(LDim * m, fill(-1.0));
	tarray_ref_ex<double, N, M> T2(darr.ptr_data(), n, m, LDim);

	T2 = (S + S2).trans();

	ASSERT_MAT_EQ( n, m, T2, T0 );
}


MN_CASE( mat_trans, colwise_reduce )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	std::string base_name = "rowxpr";

	tarray<double, M, N> S(m, n);
	fill_lin(S);
	ASSERT_STREQ( trans_base_typename(sum(S, colwise()).trans()), base_name );

	tarray<double, 1, N> R = sum(S, colwise());
	tarray<double, N, 1> T0(n, 1);
	my_transpose(R, T0);

	tarray<double, N, 1> T = sum(S, colwise()).trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), 1 );

	ASSERT_MAT_EQ( n, 1, T, T0 );
}


MN_CASE( mat_trans, rowwise_reduce )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	std::string base_name = M == 1 ? "rowxpr" : "colxpr";

	tarray<double, M, N> S(m, n);
	fill_lin(S);

	ASSERT_STREQ( trans_base_typename(sum(S, rowwise()).trans()), base_name );

	tarray<double, M, 1> R = sum(S, rowwise());
	tarray<double, 1, M> T0(1, m);
	my_transpose(R, T0);

	tarray<double, 1, M> T = sum(S, rowwise()).trans();

	ASSERT_EQ( T.nrows(), 1 );
	ASSERT_EQ( T.ncolumns(), m );

	ASSERT_MAT_EQ( 1, m, T, T0 );
}


MN_CASE( mat_trans, unary_with_targ )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	tarray<double, M, N> S(m, n);
	fill_lin(S);

	tarray<double, M, N> R = sqr(S);
	tarray<double, N, M> T0(n, m);
	my_transpose(R, T0);

	tarray<double, N, M> T = sqr(S.trans());

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );

	ASSERT_MAT_EQ( n, m, T, T0 );
}


MN_CASE( mat_trans, binary_with_targ )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	tarray<double, M, N> A(m, n);
	fill_lin(A);
	tarray<double, N, M> AT = A.trans();

	tarray<double, M, N> B = sqr(A);
	tarray<double, N, M> BT = B.trans();

	tarray<double, M, N> R = A + B;

	tarray<double, M, N> T1 = AT.trans() + BT.trans();

	ASSERT_EQ( T1.nrows(), m );
	ASSERT_EQ( T1.ncolumns(), n );
	ASSERT_MAT_EQ( m, n, T1, R );

	tarray<double, M, N> T2 = AT.trans() + B;

	ASSERT_EQ( T2.nrows(), m );
	ASSERT_EQ( T2.ncolumns(), n );
	ASSERT_MAT_EQ( m, n, T2, R );

	tarray<double, M, N> T3 = A + BT.trans();

	ASSERT_EQ( T3.nrows(), m );
	ASSERT_EQ( T3.ncolumns(), n );
	ASSERT_MAT_EQ( m, n, T3, R );
}


MN_CASE( mat_trans, binary_c_with_targ )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	tarray<double, M, N> A(m, n);
	fill_lin(A);

	double bv = 12.5;

	tarray<double, M, N> R1 = A - bv;
	tarray<double, N, M> R1t(n, m);
	my_transpose(R1, R1t);

	tarray<double, M, N> R2 = bv - A;
	tarray<double, N, M> R2t(n, m);
	my_transpose(R2, R2t);

	tarray<double, N, M> T1 = A.trans() - bv;

	ASSERT_EQ( T1.nrows(), n );
	ASSERT_EQ( T1.ncolumns(), m );
	ASSERT_MAT_EQ( n, m, T1, R1t );

	tarray<double, N, M> T2 = bv - A.trans();

	ASSERT_EQ( T1.nrows(), n );
	ASSERT_EQ( T1.ncolumns(), m );
	ASSERT_MAT_EQ( n, m, T2, R2t );
}


MN_CASE( mat_trans, repcols )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	typedef tarray<double, M, 1> col_t;
	col_t col(m, 1);
	fill_lin(col);

	horizontal_repeat_expr<ref_arg_t, col_t, N> expr(ref_arg(col), n);
	tarray<double, M, N> S(expr);

	tarray<double, N, M> T0(n, m, zero());
	my_transpose(S, T0);

	tarray<double, N, M> T = expr.trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );
	ASSERT_MAT_EQ( n, m, T, T0 );
}

MN_CASE( mat_trans, reprows )
{
	const index_t m = M == 0 ? DM : M;
	const index_t n = N == 0 ? DN : N;

	typedef tarray<double, 1, N> row_t;
	row_t row(1, n);
	fill_lin(row);

	vertical_repeat_expr<ref_arg_t, row_t, M> expr(ref_arg(row), m);
	tarray<double, M, N> S(expr);

	tarray<double, N, M> T0(n, m, zero());
	my_transpose(S, T0);

	tarray<double, N, M> T = expr.trans();

	ASSERT_EQ( T.nrows(), n );
	ASSERT_EQ( T.ncolumns(), m );
	ASSERT_MAT_EQ( n, m, T, T0 );
}



BEGIN_TPACK( dense_trans )
	ADD_MN_CASE_3X3( mat_trans, dense, DM, DN )
END_TPACK

BEGIN_TPACK( refex_trans )
	ADD_MN_CASE_3X3( mat_trans, refex, DM, DN )
END_TPACK

BEGIN_TPACK( const_trans )
	ADD_MN_CASE_3X3( mat_trans, const, DM, DN )
END_TPACK


BEGIN_TPACK( unary_ewise_trans )
	ADD_MN_CASE_3X3( mat_trans, unary_ewise, DM, DN )
END_TPACK

BEGIN_TPACK( binary_ewise_trans )
	ADD_MN_CASE_3X3( mat_trans, binary_ewise, DM, DN )
END_TPACK


BEGIN_TPACK( colwise_reduce_trans )
	ADD_MN_CASE_3X3( mat_trans, colwise_reduce, DM, DN )
END_TPACK

BEGIN_TPACK( rowwise_reduce_trans )
	ADD_MN_CASE_3X3( mat_trans, rowwise_reduce, DM, DN )
END_TPACK


BEGIN_TPACK( unary_ewise_with_targ )
	ADD_MN_CASE_3X3( mat_trans, unary_with_targ, DM, DN )
END_TPACK

BEGIN_TPACK( binary_ewise_with_targ )
	ADD_MN_CASE_3X3( mat_trans, binary_with_targ, DM, DN )
END_TPACK

BEGIN_TPACK( binary_c_ewise_with_targ )
	ADD_MN_CASE_3X3( mat_trans, binary_c_with_targ, DM, DN )
END_TPACK


BEGIN_TPACK( repcols_trans )
	ADD_MN_CASE_3X3( mat_trans, repcols, DM, DN )
END_TPACK

BEGIN_TPACK( reprows_trans )
	ADD_MN_CASE_3X3( mat_trans, reprows, DM, DN )
END_TPACK


BEGIN_MAIN_SUITE
	ADD_TPACK( dense_trans )
	ADD_TPACK( refex_trans )
	ADD_TPACK( const_trans )

	ADD_TPACK( unary_ewise_trans )
	ADD_TPACK( binary_ewise_trans )
	ADD_TPACK( colwise_reduce_trans )
	ADD_TPACK( rowwise_reduce_trans )

	ADD_TPACK( unary_ewise_with_targ )
	ADD_TPACK( binary_ewise_with_targ )
	ADD_TPACK( binary_c_ewise_with_targ )

	ADD_TPACK( repcols_trans )
	ADD_TPACK( reprows_trans )
END_MAIN_SUITE


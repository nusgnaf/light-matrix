/**
 * @file array_print.h
 *
 * Functions to print arrays
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_ARRAY_PRINT_H_
#define LIGHTMAT_ARRAY_PRINT_H_

#include <light_mat/array/array_interf.h>
#include <cstdio>

namespace lmat
{
	template<typename T, class Arr>
	inline void printf_arr(const char *fmt, const IDenseArray<Arr, T>& X,
			const char *pre_line=LMAT_NULL, const char *delim="\n")
	{
		index_t m = X.nrows();
		index_t n = X.ncolumns();

		for (index_t i = 0; i < m; ++i)
		{
			if (pre_line) std::printf("%s", pre_line);
			for (index_t j = 0; j < n; ++j)
			{
				std::printf(fmt, X.elem(i, j));
			}
			if (delim) std::printf("%s", delim);
		}
	}
}

#endif /* MATRIX_PRINT_H_ */

# Welcome to LightMatrix

**LightMatrix** is a C++ template library for matrix computation, which provides a unique combination of *design-time productivity* and *run-time performance*.
With this library, user can write codes using a set of friendly APIs (just like in MATLAB). These codes will then be transparently translated to highly optimized routines (at compile-time), through a meta-programming engine. 

## Overview

Relying a carefully designed core framework, LightMatrix has a series of nice properties. Here is a brief summary:

 - **LightMatrix is easy to use**: one can perform computation easily using natural expressions
 - **LightMatrix is efficient:** high level codes will be mapped to tight loops of highly optimized SIMD instructions through meta-programming. No temporary matrices will be created in most cases.
 - **LightMatrix is convenient:** it supports various ways to access elements and sub parts of a matrix.
 - **LightMatrix is versatile:** it offers a broad range of functions out of box, which range from elementary functions, matrix reduction, to linear algebra.
 - **LightMatrix works nicely with other libraries:** it can directly work on external memories (without copying). Interfaces to work with STL vectors and MATLAB arrays are provided.
 - **LightMatrix supports kernel-based computation:** it has elegant syntax to define kernels and apply them to matrices in an element-wise manner. This provides a very efficient way for multi-input multi-output computation.
 - **LightMatrix is extensible:** one can easily introduce new matrix expressions and evaluation methods.
 - **LightMatrix is light-weight:** it is a pure header library, and it does not rely on third-party libraries such as BOOST.
 - **LightMatrix is reliable:** quality assured through extensive unit testing.
	
**Below are some specific examples/explanations to illustrate these features.**

### *LightMatrix* is easy to use

You can easily make matrices and perform computation using matrix expressions.

```c++	
using namespace lmat;

// generate a 2-by-3 matrix by inserting elements in row-major order
dense_matrix<double> a(2, 4, rm_({1., 2., 3., 4., 5., 6., 7., 8.}));
	
// generate a matrix by copying elements from a memory source 
double src[] = {1., 2., 3., 4., 5., 6., 7., 8.};
dense_matrix<double> b(2, 4, copy_from(src));
	
// do some computation
a += exp(b) * log(a + 1.0);
```

### *LightMatrix* is efficient

When AVX is enabled, the last statement above would be translated transparently at compile time to something like the following: (*note: it also supports SSE*)

``` c++
v1 = _mm256_set1_pd(1.0);
for (index_t i = 0; i < a.nelems(); i += 4) 
{
	va = _mm256_load_pd(a.ptr_data() + i);
	vb = _mm256_load_pd(b.ptr_data() + i);
	t1 = __svml_exp4(vb);
	t2 = __svml_log4(_mm256_add_pd(va, v1));
	r = _mm256_add_pd(t1, t2);
	_mm256_store_pd(a.ptr_data() + i, r); 
}
```

The performance is comparable to hand-optimized SIMD codes. (Node: ``__svml_exp4`` and ``__svml_log4`` are routines to compute exp and log on AVX vectors.)
When explicitly requested, it can also generate multi-threaded codes using OpenMP.


### *LightMatrix* is convenient 

LightMatrix supports a variety of convenient ways to access to elements and parts of a matrix. You may access individual elements using either subscripts or linear indices:
	
```c++
// Let a be a matrix 
x = a(i, j);   // access the element at i-th row and j-th column
x = a[idx];    // access the idx-th element over the entire matrix
```
		
You may access a row or a column of a matrix
	
```c++	
c = a.column(j);   // c is a view of the j-th column
r = a.row(i);      // r is a view of the i-th row
```
		
Miss MATLAB's colon syntax? LightMatrix provides similar ways for you to access parts of a matrix
		
```c++	
// get a view of a sub-matrix
u = a(colon(i0, i1), colon(j0, j1));  

// get a view of n consecutive columns from the j-th one
v = a(whole(), range(j, n)); 

// select a certain set of rows
si = dense_col<index_t>({3, 0, 2, 4, 6});
s = select_rows(a, si);

// add a value to each element on the diagonal
a.diag() += 2.0;
```

### *LightMatrix* is versatile

It comprises a broad range of functions for various kinds of computation (out of box):
	
- matrix manipulation, such as vector repeating and matrix transposition
- element-wise arithmetics, logical operations, comparison, and conditional selection
- element-wise elemental functions (including all those in C99/C++11 math)
- reduction functions (including sum, mean, maximum, minimum, minmax, norms, etc) in three forms: whole-matrix, column-wise, and row-wise reduction.
- linear algebraic computation:
	
	+ matrix-vector and matrix-matrix multiplication
	+ matrix factorization: LU, QR, and Cholesky
	+ eigenvalue and eigenvector 
	+ singular value decomposition
	+ solving linear equations and least square problems
	
- random matrices from different distributions

### *LightMatrix* works nicely with other libraries

Practical applications often involve multiple libraries. We realize this and consider interoperability with other libraries as an essential feature from the very beginning. 
	
The matrix class exposes the raw data pointer, so that you can use other functions, in particular those in C libraries, to process the data in a matrix, as
	
```c++	
a.ptr_data();  // returns the pointer to the base address
a.ptr_col(j);  // returns the pointer to the base of the j-th column
```		

You may also construct a matrix directly on external memory blocks:
	
```c++	
// let p be a pointer to some contiguous memory region
ref_matrix<double> a(p, m, n);  // construct an m-by-n matrix directly on p

// when the stride between consecutive columns is stride (instead of m)
ref_block<double> b(p, m, n, stride); 
```
		
LightMatrix works well with STL vector and MATLAB mxArray
	
```c++	
// directly adapts std::vector to a LightMatrix column vector (no copying)
std::vector<double> v {1.0, 2.0, 3.0};
std::dense_col<double> r = sin(as_col(v)  * 2.0);

// creates a LightMatrix view on a MATLAB array (handy for writing mex)
const mxArray *mx = prhs[0];
ref_matrix<double> a = view2d<double>(mx);
```

### *LightMatrix* supports *kernel-based* computation

In complex numerical computation that involves multiple inputs and outputs, kernel-based computation is often more efficient than matrix expressions.
	
Suppose you want to do the computation as expressed by the following matrix expressions:
	
```c++	
u = sqr(a + b) - sqr(a - b); 
v = sqr(a + b) + sqr(a - b); 
```
		
But then ``sqr(a + b)`` and ``sqr(a - b)`` are respectively calculated twice. Whereas one can temporarily store the the intermediate results, this way demands more memory and requires scanning each matrix more than once. The more optimal way is to apply a multi-input-multi-output kernel, as
	
```c++
// define a kernel (as a generic functor)
template<typename T>
struct my_kernel
{
	void operator() (const T& a, const T& b, T& u, T& v) const 
	{
		T t1 = sqr(a + b);
		T t2 = sqr(a - b);
		u = t1 - t2;
		v = t1 + t2;
	}
};

// introduce SIMD support
LMAT_DEF_SIMD_SUPPORT( my_kernel )

// apply the kernel in an element-wise way to matrices
ewise(my_kernel())(in_(a), in_(b), out_(u), out_(v));
```
		
LightMatrix will translate this (at compile-time) into tight loops of SIMD instructions, without creating any temporary matrices.


### *LightMatrix* is extensible
	
LightMatrix is designed with extensibility in mind. Developers can easily extend the library by introducing new matrix expressions or even new evaluation paradigms, following the Developer Guide. 
	
	
### *LightMatrix* is light-weight

LightMatrix is a *pure header* library. To use it, just check it out from GitHub, and include the relevant headers in your code. 
	
Note that linking with third-party libraries is needed for some functions. For example, you may have to link your code to BLAS/LAPACK or MKL when using the linear algebra module. 


### *LightMatrix* is reliable

The quality of the library is assured through thousands of unit-test cases. 



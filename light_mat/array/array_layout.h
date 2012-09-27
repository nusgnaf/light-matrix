/**
 * @file array_layout.h
 *
 * @brief Classes related to array layouts
 *
 * @author Dahua Lin
 */

#ifndef LIGHTMAT_ARRAY_LAYOUT_H_
#define LIGHTMAT_ARRAY_LAYOUT_H_

#include <light_mat/common/basic_defs.h>

namespace lmat
{

	/********************************************
	 *
	 *  Array shape classes
	 *
	 ********************************************/

	template<int M, int N>
	class array_shape
	{
	public:
		LMAT_ENSURE_INLINE array_shape()
		{ }

		LMAT_ENSURE_INLINE array_shape(index_t m, index_t n)
		{
			check_arg(m == M && n == N, "Invalid input dimensions.");
		}

		LMAT_ENSURE_INLINE index_t nrows() const
		{
			return M;
		}

		LMAT_ENSURE_INLINE index_t ncolumns() const
		{
			return N;
		}

		LMAT_ENSURE_INLINE index_t nelems() const
		{
			return M * N;
		}

		LMAT_ENSURE_INLINE bool is_matrix() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE bool is_row() const
		{
			return M == 1;
		}

		LMAT_ENSURE_INLINE bool is_column() const
		{
			return N == 1;
		}

		LMAT_ENSURE_INLINE bool is_vector() const
		{
			return M == 1 || N == 1;
		}

		LMAT_ENSURE_INLINE bool is_scalar() const
		{
			return M == 1 && N == 1;
		}

		LMAT_ENSURE_INLINE bool is_empty() const
		{
			return false;
		}

	}; // array_shape<M, N>


	template<int M>
	class array_shape<M, 0>
	{
	public:
		LMAT_ENSURE_INLINE array_shape()
		: m_d1(0) { }

		LMAT_ENSURE_INLINE array_shape(index_t m, index_t n)
		: m_d1(n)
		{
			check_arg(m == M, "Invalid input dimensions.");
		}

		LMAT_ENSURE_INLINE index_t nrows() const
		{
			return M;
		}

		LMAT_ENSURE_INLINE index_t ncolumns() const
		{
			return m_d1;
		}

		LMAT_ENSURE_INLINE index_t nelems() const
		{
			return M * m_d1;
		}

		LMAT_ENSURE_INLINE bool is_matrix() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE bool is_row() const
		{
			return M == 1;
		}

		LMAT_ENSURE_INLINE bool is_column() const
		{
			return m_d1 == 1;
		}

		LMAT_ENSURE_INLINE bool is_vector() const
		{
			return M == 1 || m_d1 == 1;
		}

		LMAT_ENSURE_INLINE bool is_scalar() const
		{
			return M == 1 && m_d1 == 1;
		}

		LMAT_ENSURE_INLINE bool is_empty() const
		{
			return m_d1 == 0;
		}

	private:
		const index_t m_d1;

	}; // array_shape<M, 0>


	template<int N>
	class array_shape<0, N>
	{
	public:
		LMAT_ENSURE_INLINE array_shape()
		: m_d0(0) { }

		LMAT_ENSURE_INLINE array_shape(index_t m, index_t n)
		: m_d0(m)
		{
			check_arg(n == N, "Invalid input dimensions.");
		}

		LMAT_ENSURE_INLINE index_t nrows() const
		{
			return m_d0;
		}

		LMAT_ENSURE_INLINE index_t ncolumns() const
		{
			return N;
		}

		LMAT_ENSURE_INLINE index_t nelems() const
		{
			return N * m_d0;
		}

		LMAT_ENSURE_INLINE bool is_matrix() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE bool is_row() const
		{
			return m_d0 == 1;
		}

		LMAT_ENSURE_INLINE bool is_column() const
		{
			return N == 1;
		}

		LMAT_ENSURE_INLINE bool is_vector() const
		{
			return N == 1 || m_d0 == 1;
		}

		LMAT_ENSURE_INLINE bool is_scalar() const
		{
			return N == 1 && m_d0 == 1;
		}

		LMAT_ENSURE_INLINE bool is_empty() const
		{
			return m_d0 == 0;
		}

	private:
		const index_t m_d0;

	}; // array_shape<0, N>


	template<>
	class array_shape<0, 0>
	{
	public:
		LMAT_ENSURE_INLINE array_shape()
		:m_d0(0), m_d1(0) { }

		LMAT_ENSURE_INLINE array_shape(index_t m, index_t n)
		: m_d0(m), m_d1(n) { }

		LMAT_ENSURE_INLINE index_t nrows() const
		{
			return m_d0;
		}

		LMAT_ENSURE_INLINE index_t ncolumns() const
		{
			return m_d1;
		}

		LMAT_ENSURE_INLINE index_t nelems() const
		{
			return m_d0 * m_d1;
		}

		LMAT_ENSURE_INLINE bool is_matrix() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE bool is_row() const
		{
			return m_d0 == 1;
		}

		LMAT_ENSURE_INLINE bool is_column() const
		{
			return m_d1 == 1;
		}

		LMAT_ENSURE_INLINE bool is_vector() const
		{
			return m_d0 == 1 || m_d1 == 1;
		}

		LMAT_ENSURE_INLINE bool is_scalar() const
		{
			return m_d0 == 1 && m_d1 == 1;
		}

		LMAT_ENSURE_INLINE bool is_empty() const
		{
			return m_d0 == 0 || m_d1 == 0;
		}

	private:
		const index_t m_d0, m_d1;

	}; // array_shape<0, 0>


	/********************************************
	 *
	 *  compile-time manipulation
	 *
	 ********************************************/

	// intrinsic values

	template<class Shape> struct ct_nrows;
	template<class Shape> struct ct_ncols;
	template<class Shape> struct ct_nelems;

	template<int M, int N>
	struct ct_nrows<array_shape<M, N> >
	{
		static const int value = M;
	};

	template<int M, int N>
	struct ct_ncols<array_shape<M, N> >
	{
		static const int value = N;
	};

	template<int M, int N>
	struct ct_nelems<array_shape<M, N> >
	{
		static const int value = M * N;
	};


	// shape testing

	template<class S> struct is_array_shape { static const bool value = false; };

	template<int M, int N>
	struct is_array_shape<array_shape<M, N> >
	{
		static const bool value = true;
	};

	template<class S> struct ct_is_matrix_shape
	{
		static const bool value = is_array_shape<S>::value;
	};

	template<class S> struct ct_is_vector_shape
	{
		static const bool value =
				ct_is_matrix_shape<S>::value &&
				(ct_nrows<S>::value == 1 || ct_ncols<S>::value == 1);
	};

	template<class S> struct ct_is_scalar_shape
	{
		static const bool value =
				ct_is_matrix_shape<S>::value &&
				(ct_nrows<S>::value == 1 && ct_ncols<S>::value == 1);
	};

	template<class S> struct ct_is_row_shape
	{
		static const bool value =
				ct_is_matrix_shape<S>::value &&
				(ct_nrows<S>::value == 1);
	};

	template<class S> struct ct_is_column_shape
	{
		static const bool value =
				ct_is_matrix_shape<S>::value &&
				(ct_ncols<S>::value == 1);
	};



	// dimension compatibility and result shape

	template<int LD, int RD>
	struct are_compatible_dims
	{
		static const bool value = (LD == RD || LD == 0 || RD == 0);
	};

	template<class L, class R>
	struct are_compatible_shapes
	{
		static const bool value =
				are_compatible_dims<ct_nrows<L>::value, ct_nrows<R>::value>::value &&
				are_compatible_dims<ct_ncols<L>::value, ct_ncols<R>::value>::value;
	};

	namespace detail
	{
		template<int LD, int RD, bool AreCompatibleDims>
		struct _ct_binary_dim;

		template<int LD, int RD>
		struct _ct_binary_dim<LD, RD, true>
		{
			static const bool value = LD != 0 ? LD : RD;
		};
	}

	template<int LD, int RD>
	struct ct_binary_dim
	{
		static const bool value =
				detail::_ct_binary_dim<LD, RD, are_compatible_dims<LD, RD>::value>::value;
	};

	template<class L, class R>
	struct ct_binary_shape
	{
		static const int nrows = ct_binary_dim<ct_nrows<L>::value, ct_nrows<R>::value>::value;
		static const int ncols = ct_binary_dim<ct_ncols<L>::value, ct_ncols<R>::value>::value;

		typedef array_shape<nrows, ncols> type;
	};


	/********************************************
	 *
	 *  run-time manipulation
	 *
	 ********************************************/

	template<int LD, int RD>
	LMAT_ENSURE_INLINE
	inline bool are_equal_dims(const index_t ld, const index_t rd)
	{
		return LD == RD || ld == rd;
	}

	template<int LM, int LN, int RM, int RN>
	LMAT_ENSURE_INLINE
	inline bool operator == (const array_shape<LM, LN>& lhs, const array_shape<RM, RN>& rhs)
	{
		return  are_equal_dims<LM, RM>(lhs.nrows(), rhs.nrows()) &&
				are_equal_dims<LN, RN>(lhs.ncolumns(), rhs.ncolumns());
	}

	template<int LM, int LN, int RM, int RN>
	LMAT_ENSURE_INLINE
	inline bool operator != (const array_shape<LM, LN>& lhs, const array_shape<RM, RN>& rhs)
	{
		return !(lhs == rhs);
	}


	/********************************************
	 *
	 *  Array layout classes
	 *
	 ********************************************/

	template<class Shape>
	class array_layout_base
	{
	public:
		typedef Shape shape_type;

		LMAT_ENSURE_INLINE
		array_layout_base() : m_shape() { }

		LMAT_ENSURE_INLINE
		array_layout_base(const index_t m, const index_t n)
		: m_shape(m, n) { }

	public:
		LMAT_ENSURE_INLINE const Shape& shape() const
		{
			return m_shape;
		}

		LMAT_ENSURE_INLINE index_t nrows() const
		{
			return m_shape.nrows();
		}

		LMAT_ENSURE_INLINE index_t ncolumns() const
		{
			return m_shape.ncolumns();
		}

		LMAT_ENSURE_INLINE index_t nelems() const
		{
			return m_shape.nelems();
		}

		LMAT_ENSURE_INLINE bool is_matrix() const
		{
			return m_shape.is_matrix();
		}

		LMAT_ENSURE_INLINE bool is_row() const
		{
			return m_shape.is_row();
		}

		LMAT_ENSURE_INLINE bool is_column() const
		{
			return m_shape.is_column();
		}

		LMAT_ENSURE_INLINE bool is_vector() const
		{
			return m_shape.is_vector();
		}

		LMAT_ENSURE_INLINE bool is_scalar() const
		{
			return m_shape.is_scalar();
		}

		LMAT_ENSURE_INLINE bool is_empty() const
		{
			return m_shape.is_empty();
		}

	protected:
		Shape m_shape;
	};


	// Specialized layout classes


	template<int M, int N>
	class continuous_column_major_layout
	: public array_layout_base<array_shape<M, N> >
	{
		typedef array_layout_base<array_shape<M, N> > base_t;

	public:
		LMAT_ENSURE_INLINE
		continuous_column_major_layout() : base_t() { }

		LMAT_ENSURE_INLINE
		continuous_column_major_layout(index_t m, index_t n) : base_t(m, n) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return this->nrows();
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return i;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return i + lead_dim() * j;
		}
	};


	template<int M>
	class continuous_column_major_layout<M, 1>
	: public array_layout_base<array_shape<M, 1> >
	{
		typedef array_layout_base<array_shape<M, 1> > base_t;

	public:
		LMAT_ENSURE_INLINE
		continuous_column_major_layout() : base_t() { }

		LMAT_ENSURE_INLINE
		continuous_column_major_layout(index_t m, index_t n) : base_t(m, n) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return this->nrows();
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return i;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return i;
		}
	};


	template<int N>
	class continuous_column_major_layout<1, N>
	: public array_layout_base<array_shape<1, N> >
	{
		typedef array_layout_base<array_shape<1, N> > base_t;

	public:
		LMAT_ENSURE_INLINE
		continuous_column_major_layout() : base_t() { }

		LMAT_ENSURE_INLINE
		continuous_column_major_layout(index_t m, index_t n) : base_t(m, n) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return 1;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return i;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return j;
		}
	};


	template<>
	class continuous_column_major_layout<1, 1>
	: public array_layout_base<array_shape<1, 1> >
	{
		typedef array_layout_base<array_shape<1, 1> > base_t;

	public:
		LMAT_ENSURE_INLINE
		continuous_column_major_layout() : base_t() { }

		LMAT_ENSURE_INLINE
		continuous_column_major_layout(index_t m, index_t n) : base_t(m, n) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return 1;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return 0;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return 0;
		}
	};


	template<int M, int N>
	class column_major_layout
	: public array_layout_base<array_shape<M, N> >
	{
		typedef array_layout_base<array_shape<M, N> > base_t;

	public:
		LMAT_ENSURE_INLINE
		column_major_layout(index_t m, index_t n, index_t ldim)
		: base_t(m, n), m_ldim(ldim) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return m_ldim == this->nrows();
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return m_ldim;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			throw invalid_operation(
					"Linear indexing is not allowed on this layout.");
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return i + m_ldim * j;
		}

	private:
		index_t m_ldim;
	};


	template<int M>
	class column_major_layout<M, 1>
	: public array_layout_base<array_shape<M, 1> >
	{
		typedef array_layout_base<array_shape<M, 1> > base_t;

	public:
		LMAT_ENSURE_INLINE
		column_major_layout(index_t m, index_t n, index_t ldim)
		: base_t(m, n), m_ldim(ldim) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return m_ldim;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return i;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return i;
		}

	private:
		index_t m_ldim;
	};


	template<int N>
	class column_major_layout<1, N>
	: public array_layout_base<array_shape<1, N> >
	{
		typedef array_layout_base<array_shape<1, N> > base_t;

	public:
		LMAT_ENSURE_INLINE
		column_major_layout(index_t m, index_t n, index_t ldim)
		: base_t(m, n), m_ldim(ldim) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return m_ldim == 1;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return m_ldim;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return m_ldim * i;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return m_ldim * j;
		}

	private:
		index_t m_ldim;
	};


	template<>
	class column_major_layout<1, 1>
	: public array_layout_base<array_shape<1, 1> >
	{
		typedef array_layout_base<array_shape<1, 1> > base_t;

	public:
		LMAT_ENSURE_INLINE
		column_major_layout(index_t m, index_t n, index_t ldim)
		: base_t(m, n), m_ldim(ldim) { }

	public:
		LMAT_ENSURE_INLINE
		bool is_continuous() const
		{
			return true;
		}

		LMAT_ENSURE_INLINE
		index_t lead_dim() const
		{
			return m_ldim;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i) const
		{
			LMAT_CHECK_SUBS1(i, this->nelems());
			return 0;
		}

		LMAT_ENSURE_INLINE
		index_t offset(index_t i, index_t j) const
		{
			LMAT_CHECK_SUBS2(i, j, this->nrows(), this->ncolumns());
			return 0;
		}

	private:
		index_t m_ldim;
	};



	/********************************************
	 *
	 *  Array layout compile time properties
	 *
	 ********************************************/

	template<class A>
	struct is_layout_class
	{
		static const bool value = false;
	};

	template<int M, int N>
	struct is_layout_class<continuous_column_major_layout<M, N> >
	{
		static const bool value = true;
	};

	template<int M, int N>
	struct is_layout_class<column_major_layout<M, N> >
	{
		static const bool value = true;
	};


	template<class Layout>
	struct ct_is_continuous_layout
	{
		static const bool value = false;
	};

	template<int M, int N>
	struct ct_is_continuous_layout<continuous_column_major_layout<M, N> >
	{
		static const bool value = false;
	};

	template<int M, int N>
	struct ct_is_continuous_layout<column_major_layout<M, N> >
	{
		static const bool value = (N == 1);
	};


}

#endif /* ARRAY_SHAPE_H_ */

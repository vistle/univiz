/* -*- c++ -*- (enables emacs c++ mode)                                    */
/* *********************************************************************** */
/*                                                                         */
/* Library :  Generic Matrix Methods  (gmm)                                */
/* File    :  gmm_matrix.h : matrices.                                     */
/*     									   */
/* Date : October 13, 2002.                                                */
/* Author : Yves Renard, Yves.Renard@gmm.insa-tlse.fr                      */
/*                                                                         */
/* *********************************************************************** */
/*                                                                         */
/* Copyright (C) 2002-2003 Yves Renard.                                    */
/*                                                                         */
/* This file is a part of GMM++                                            */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify    */
/* it under the terms of the GNU Lesser General Public License as          */
/* published by the Free Software Foundation; version 2.1 of the License.  */
/*                                                                         */
/* This program is distributed in the hope that it will be useful,         */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/* GNU Lesser General Public License for more details.                     */
/*                                                                         */
/* You should have received a copy of the GNU Lesser General Public        */
/* License along with this program; if not, write to the Free Software     */
/* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,  */
/* USA.                                                                    */
/*                                                                         */
/* *********************************************************************** */

#ifndef GMM_MATRIX_H__
#define GMM_MATRIX_H__

#include <ext/gmm/gmm_vector.h>
#include <ext/gmm/gmm_sub_vector.h>

namespace gmm
{

/* ******************************************************************** */
/*		                                            		  */
/*		Identity matrix                         		  */
/*		                                            		  */
/* ******************************************************************** */

struct identity_matrix
{
    template <class MAT>
    void build_with(const MAT &)
    {
    }
};

template <typename M>
inline void add(const identity_matrix &, M &v1)
{
    size_type n = std::min(gmm::mat_nrows(v1), gmm::mat_ncols(v1));
    for (size_type i = 0; i < n; ++i)
        v1(i, i) += typename linalg_traits<M>::value_type(1);
}
template <typename M>
inline void add(const identity_matrix &I, const M &v1)
{
    add(I, linalg_const_cast(v1));
}

template <typename V1, typename V2>
inline void mult(const identity_matrix &, const V1 &v1, V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void mult(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2, typename V3>
inline void mult(const identity_matrix &, const V1 &v1, const V2 &v2, V3 &v3)
{
    add(v1, v2, v3);
}
template <typename V1, typename V2, typename V3>
inline void mult(const identity_matrix &, const V1 &v1, const V2 &v2, const V3 &v3)
{
    add(v1, v2, v3);
}
template <typename V1, typename V2>
inline void left_mult(const identity_matrix &, const V1 &v1, V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void left_mult(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void right_mult(const identity_matrix &, const V1 &v1, V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void right_mult(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void transposed_left_mult(const identity_matrix &, const V1 &v1, V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void transposed_left_mult(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void transposed_right_mult(const identity_matrix &, const V1 &v1, V2 &v2)
{
    copy(v1, v2);
}
template <typename V1, typename V2>
inline void transposed_right_mult(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    copy(v1, v2);
}
template <typename M>
void copy_ident(const identity_matrix &, M &m)
{
    size_type i = 0, n = std::min(mat_nrows(m), mat_ncols(m));
    clear(m);
    for (; i < n; ++i)
        m(i, i) = typename linalg_traits<M>::value_type(1);
}
template <typename M>
inline void copy(const identity_matrix &, M &m)
{
    copy_ident(identity_matrix(), m);
}
template <typename M>
inline void copy(const identity_matrix &, const M &m)
{
    copy_ident(identity_matrix(), linalg_const_cast(m));
}
template <typename V1, typename V2>
inline typename linalg_traits<V1>::value_type
vect_sp(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    return vect_sp(v1, v2);
}
template <typename V1, typename V2>
inline typename linalg_traits<V1>::value_type
vect_hp(const identity_matrix &, const V1 &v1, const V2 &v2)
{
    return vect_hp(v1, v2);
}
template <typename M>
inline bool is_identity(const M &)
{
    return false;
}
inline bool is_identity(const identity_matrix &)
{
    return true;
}

/* ******************************************************************** */
/*		                                            		  */
/*		Row matrix                                   		  */
/*		                                            		  */
/* ******************************************************************** */

template <typename V>
class row_matrix
{
protected:
    std::vector<V> li; /* array of rows.                                  */
    size_type nc;

public:
    typedef typename linalg_traits<V>::reference reference;
    typedef typename linalg_traits<V>::value_type value_type;

    row_matrix(size_type r, size_type c)
        : li(r, V(c))
        , nc(c)
    {
    }
    row_matrix(void)
        : nc(0)
    {
    }
    reference operator()(size_type l, size_type c)
    {
        return li[l][c];
    }
    value_type operator()(size_type l, size_type c) const
    {
        return li[l][c];
    }

    void clear_mat();
    void resize(size_type m, size_type n);

    typename std::vector<V>::iterator begin(void)
    {
        return li.begin();
    }
    typename std::vector<V>::iterator end(void)
    {
        return li.end();
    }
    typename std::vector<V>::const_iterator begin(void) const
    {
        return li.begin();
    }
    typename std::vector<V>::const_iterator end(void) const
    {
        return li.end();
    }

    V &row(size_type i)
    {
        return li[i];
    }
    const V &row(size_type i) const
    {
        return li[i];
    }
    V &operator[](size_type i)
    {
        return li[i];
    }
    const V &operator[](size_type i) const
    {
        return li[i];
    }

    inline size_type nrows(void) const
    {
        return li.size();
    }
    inline size_type ncols(void) const
    {
        return nc;
    }

    void swap(row_matrix<V> &m)
    {
        std::swap(li, m.li);
        std::swap(nc, m.nc);
    }
    void swap_row(size_type i, size_type j)
    {
        std::swap(li[i], li[j]);
    }
};

template <typename V>
void row_matrix<V>::resize(size_type m, size_type n)
{
    li.resize(m);
    for (size_type i = 0; i < m; ++i)
        gmm::resize(li[i], n);
    nc = n;
}

template <typename V>
void row_matrix<V>::clear_mat()
{
    for (size_type i = 0; i < nrows(); ++i)
        clear(li[i]);
}

template <typename V>
struct linalg_traits<row_matrix<V> >
{
    typedef row_matrix<V> this_type;
    typedef this_type origin_type;
    typedef linalg_false is_reference;
    typedef abstract_matrix linalg_type;
    typedef typename linalg_traits<V>::value_type value_type;
    typedef typename linalg_traits<V>::reference reference;
    typedef typename linalg_traits<V>::storage_type storage_type;
    typedef simple_vector_ref<V *> sub_row_type;
    typedef simple_vector_ref<const V *> const_sub_row_type;
    typedef typename std::vector<V>::iterator row_iterator;
    typedef typename std::vector<V>::const_iterator const_row_iterator;
    typedef abstract_null_type sub_col_type;
    typedef abstract_null_type const_sub_col_type;
    typedef abstract_null_type col_iterator;
    typedef abstract_null_type const_col_iterator;
    typedef row_major sub_orientation;
    typedef linalg_true index_sorted;
    static size_type nrows(const this_type &m)
    {
        return m.nrows();
    }
    static size_type ncols(const this_type &m)
    {
        return m.ncols();
    }
    static row_iterator row_begin(this_type &m)
    {
        return m.begin();
    }
    static row_iterator row_end(this_type &m)
    {
        return m.end();
    }
    static const_row_iterator row_begin(const this_type &m)
    {
        return m.begin();
    }
    static const_row_iterator row_end(const this_type &m)
    {
        return m.end();
    }
    static const_sub_row_type row(const const_row_iterator &it)
    {
        return const_sub_row_type(*it);
    }
    static sub_row_type row(const row_iterator &it)
    {
        return sub_row_type(*it);
    }
    static origin_type *origin(this_type &m)
    {
        return &m;
    }
    static const origin_type *origin(const this_type &m)
    {
        return &m;
    }
    static void do_clear(this_type &m)
    {
        m.clear_mat();
    }
    static value_type access(const const_row_iterator &itrow, size_type j)
    {
        return (*itrow)[j];
    }
    static reference access(const row_iterator &itrow, size_type j)
    {
        return (*itrow)[j];
    }
    static void resize(this_type &v, size_type m, size_type n)
    {
        v.resize(m, n);
    }
    static void reshape(this_type &, size_type, size_type)
    {
        DAL_THROW(failure_error, "Sorry, to be done");
    }
};

template <typename V>
std::ostream &operator<<(std::ostream &o, const row_matrix<V> &m)
{
    gmm::write(o, m);
    return o;
}

/* ******************************************************************** */
/*		                                            		  */
/*		Column matrix                                		  */
/*		                                            		  */
/* ******************************************************************** */

template <typename V>
class col_matrix
{
protected:
    std::vector<V> li; /* array of columns.                               */
    size_type nr;

public:
    typedef typename linalg_traits<V>::reference reference;
    typedef typename linalg_traits<V>::value_type value_type;

    col_matrix(size_type r, size_type c)
        : li(c, V(r))
        , nr(r)
    {
    }
    col_matrix(void)
        : nr(0)
    {
    }
    reference operator()(size_type l, size_type c)
    {
        return li[c][l];
    }
    value_type operator()(size_type l, size_type c) const
    {
        return li[c][l];
    }

    void clear_mat();
    void resize(size_type, size_type);

    V &col(size_type i)
    {
        return li[i];
    }
    const V &col(size_type i) const
    {
        return li[i];
    }
    V &operator[](size_type i)
    {
        return li[i];
    }
    const V &operator[](size_type i) const
    {
        return li[i];
    }

    typename std::vector<V>::iterator begin(void)
    {
        return li.begin();
    }
    typename std::vector<V>::iterator end(void)
    {
        return li.end();
    }
    typename std::vector<V>::const_iterator begin(void) const
    {
        return li.begin();
    }
    typename std::vector<V>::const_iterator end(void) const
    {
        return li.end();
    }

    inline size_type ncols(void) const
    {
        return li.size();
    }
    inline size_type nrows(void) const
    {
        return nr;
    }

    void swap(col_matrix<V> &m)
    {
        std::swap(li, m.li);
        std::swap(nr, m.nr);
    }
    void swap_col(size_type i, size_type j)
    {
        std::swap(li[i], li[j]);
    }
};

template <typename V>
void col_matrix<V>::resize(size_type m, size_type n)
{
    li.resize(n);
    for (size_type i = 0; i < n; ++i)
        gmm::resize(li[i], m);
    nr = m;
}

template <typename V>
void col_matrix<V>::clear_mat()
{
    for (size_type i = 0; i < ncols(); ++i)
        clear(li[i]);
}

template <typename V>
struct linalg_traits<col_matrix<V> >
{
    typedef col_matrix<V> this_type;
    typedef this_type origin_type;
    typedef linalg_false is_reference;
    typedef abstract_matrix linalg_type;
    typedef typename linalg_traits<V>::value_type value_type;
    typedef typename linalg_traits<V>::reference reference;
    typedef typename linalg_traits<V>::storage_type storage_type;
    typedef simple_vector_ref<V *> sub_col_type;
    typedef simple_vector_ref<const V *> const_sub_col_type;
    typedef typename std::vector<V>::iterator col_iterator;
    typedef typename std::vector<V>::const_iterator const_col_iterator;
    typedef abstract_null_type sub_row_type;
    typedef abstract_null_type const_sub_row_type;
    typedef abstract_null_type row_iterator;
    typedef abstract_null_type const_row_iterator;
    typedef col_major sub_orientation;
    typedef linalg_true index_sorted;
    static size_type nrows(const this_type &m)
    {
        return m.nrows();
    }
    static size_type ncols(const this_type &m)
    {
        return m.ncols();
    }
    static col_iterator col_begin(this_type &m)
    {
        return m.begin();
    }
    static col_iterator col_end(this_type &m)
    {
        return m.end();
    }
    static const_col_iterator col_begin(const this_type &m)
    {
        return m.begin();
    }
    static const_col_iterator col_end(const this_type &m)
    {
        return m.end();
    }
    static const_sub_col_type col(const const_col_iterator &it)
    {
        return const_sub_col_type(*it);
    }
    static sub_col_type col(const col_iterator &it)
    {
        return sub_col_type(*it);
    }
    static origin_type *origin(this_type &m)
    {
        return &m;
    }
    static const origin_type *origin(const this_type &m)
    {
        return &m;
    }
    static void do_clear(this_type &m)
    {
        m.clear_mat();
    }
    static value_type access(const const_col_iterator &itcol, size_type j)
    {
        return (*itcol)[j];
    }
    static reference access(const col_iterator &itcol, size_type j)
    {
        return (*itcol)[j];
    }
    static void resize(this_type &v, size_type m, size_type n)
    {
        v.resize(m, n);
    }
    static void reshape(this_type &, size_type, size_type)
    {
        DAL_THROW(failure_error, "Sorry, to be done");
    }
};

template <typename V>
std::ostream &operator<<(std::ostream &o, const col_matrix<V> &m)
{
    gmm::write(o, m);
    return o;
}

/* ******************************************************************** */
/*		                                            		  */
/*		Dense matrix                                		  */
/*		                                            		  */
/* ******************************************************************** */

template <typename T>
class dense_matrix : public std::vector<T>
{
public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

protected:
    size_type nbc, nbl;

public:
    inline const T &operator()(size_type l, size_type c) const
    {
#ifdef GMM_VERIFY
        if (l >= nbl || c >= nbc)
            this->out_of_range_error();
#endif
        return *(this->begin() + c * nbl + l);
    }
    inline T &operator()(size_type l, size_type c)
    {
#ifdef GMM_VERIFY
        if (l >= nbl || c >= nbc)
            this->out_of_range_error();
#endif
        return *(this->begin() + c * nbl + l);
    }

    void out_of_range_error(void) const;

    void resize(size_type, size_type);
    void reshape(size_type, size_type);

    void fill(T a, T b = T(0));
    inline size_type nrows(void) const
    {
        return nbl;
    }
    inline size_type ncols(void) const
    {
        return nbc;
    }
    void swap(dense_matrix<T> &m)
    {
        std::vector<T>::swap(m);
        std::swap(nbc, m.nbc);
        std::swap(nbl, m.nbl);
    }

    dense_matrix(size_type l, size_type c)
        : std::vector<T>(c * l)
        , nbc(c)
        , nbl(l)
    {
    }
    dense_matrix(void)
    {
        nbl = nbc = 0;
    }
};

template <typename T>
void dense_matrix<T>::reshape(size_type m, size_type n)
{
    if (n * m != nbl * nbc)
        DAL_THROW(dimension_error, "dimensions mismatch");
    nbl = m;
    nbc = n;
}

template <typename T>
void dense_matrix<T>::resize(size_type m, size_type n)
{
    if (n * m > nbc * nbl)
        std::vector<T>::resize(n * m);
    if (m < nbl)
    {
        for (size_type i = 1; i < std::min(nbc, n); ++i)
            std::copy(this->begin() + i * nbl, this->begin() + (i * nbl + m),
                      this->begin() + i * m);
        for (size_type i = std::min(nbc, n); i < n; ++i)
            std::fill(this->begin() + (i * m), this->begin() + (i + 1) * m, T(0));
    }
    else if (m > nbl)
    { /* do nothing when the nb of rows does not change */
        for (size_type i = std::min(nbc, n); i > 1; --i)
            std::copy(this->begin() + (i - 1) * nbl, this->begin() + i * nbl,
                      this->begin() + (i - 1) * m);
        for (size_type i = 0; i < std::min(nbc, n); ++i)
            std::fill(this->begin() + (i * m + nbl), this->begin() + (i + 1) * m, T(0));
    }
    if (n * m < nbc * nbl)
        std::vector<T>::resize(n * m);
    nbl = m;
    nbc = n;
}

template <typename T>
void dense_matrix<T>::fill(T a, T b)
{
    std::fill(this->begin(), this->end(), b);
    iterator p = this->begin(), e = this->end();
    while (p < e)
    {
        *p = a;
        p += nbl + 1;
    }
}

template <typename T>
void dense_matrix<T>::out_of_range_error(void) const
{
    DAL_THROW(std::out_of_range, "out of range");
}

template <typename T>
struct linalg_traits<dense_matrix<T> >
{
    typedef dense_matrix<T> this_type;
    typedef this_type origin_type;
    typedef linalg_false is_reference;
    typedef abstract_matrix linalg_type;
    typedef T value_type;
    typedef T &reference;
    typedef abstract_dense storage_type;
    typedef tab_ref_reg_spaced_with_origin<typename this_type::iterator,
                                           this_type> sub_row_type;
    typedef tab_ref_reg_spaced_with_origin<typename this_type::const_iterator,
                                           this_type> const_sub_row_type;
    typedef dense_compressed_iterator<typename this_type::iterator,
                                      typename this_type::iterator,
                                      this_type *> row_iterator;
    typedef dense_compressed_iterator<typename this_type::const_iterator,
                                      typename this_type::iterator,
                                      const this_type *> const_row_iterator;
    typedef tab_ref_with_origin<typename this_type::iterator,
                                this_type> sub_col_type;
    typedef tab_ref_with_origin<typename this_type::const_iterator,
                                this_type> const_sub_col_type;
    typedef dense_compressed_iterator<typename this_type::iterator,
                                      typename this_type::iterator,
                                      this_type *> col_iterator;
    typedef dense_compressed_iterator<typename this_type::const_iterator,
                                      typename this_type::iterator,
                                      const this_type *> const_col_iterator;
    typedef col_and_row sub_orientation;
    typedef linalg_true index_sorted;
    static size_type nrows(const this_type &m)
    {
        return m.nrows();
    }
    static size_type ncols(const this_type &m)
    {
        return m.ncols();
    }
    static const_sub_row_type row(const const_row_iterator &it)
    {
        return const_sub_row_type(it.it, it.it + it.ncols * it.nrows,
                                  it.nrows, it.origin);
    }
    static const_sub_col_type col(const const_col_iterator &it)
    {
        return const_sub_col_type(it.it, it.it + it.nrows, it.origin);
    }
    static sub_row_type row(const row_iterator &it)
    {
        return sub_row_type(it.it, it.it + it.ncols * it.nrows,
                            it.nrows, it.origin);
    }
    static sub_col_type col(const col_iterator &it)
    {
        return sub_col_type(it.it, it.it + it.nrows, it.origin);
    }
    static row_iterator row_begin(this_type &m)
    {
        return row_iterator(m.begin(), 1, m.nrows(), m.ncols(), &m);
    }
    static row_iterator row_end(this_type &m)
    {
        return row_iterator(m.begin() + m.nrows(), 1, m.nrows(), m.ncols(), &m);
    }
    static const_row_iterator row_begin(const this_type &m)
    {
        return const_row_iterator(m.begin(), 1, m.nrows(), m.ncols(), &m);
    }
    static const_row_iterator row_end(const this_type &m)
    {
        return const_row_iterator(m.begin() + m.nrows(), 1, m.nrows(),
                                  m.ncols(), &m);
    }
    static col_iterator col_begin(this_type &m)
    {
        return col_iterator(m.begin(), m.nrows(), m.nrows(), m.ncols(), &m);
    }
    static col_iterator col_end(this_type &m)
    {
        return col_iterator(m.end(), m.nrows(), m.nrows(), m.ncols(), &m);
    }
    static const_col_iterator col_begin(const this_type &m)
    {
        return const_col_iterator(m.begin(), m.nrows(), m.nrows(), m.ncols(), &m);
    }
    static const_col_iterator col_end(const this_type &m)
    {
        return const_col_iterator(m.end(), m.nrows(), m.nrows(), m.ncols(), &m);
    }
    static origin_type *origin(this_type &m)
    {
        return &m;
    }
    static const origin_type *origin(const this_type &m)
    {
        return &m;
    }
    static void do_clear(this_type &m)
    {
        m.fill(value_type(0));
    }
    static value_type access(const const_col_iterator &itcol, size_type j)
    {
        return (*itcol)[j];
    }
    static reference access(const col_iterator &itcol, size_type j)
    {
        return (*itcol)[j];
    }
    static void resize(this_type &v, size_type m, size_type n)
    {
        v.resize(m, n);
    }
    static void reshape(this_type &v, size_type m, size_type n)
    {
        v.reshape(m, n);
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &o, const dense_matrix<T> &m)
{
    gmm::write(o, m);
    return o;
}

/* ******************************************************************** */
/*                                                                      */
/*	        Read only compressed sparse column matrix                 */
/*                                                                      */
/* ******************************************************************** */

template <typename T, int shift = 0>
struct csc_matrix
{
    typedef unsigned int IND_TYPE;

    T *pr; // values.
    IND_TYPE *ir; // row indices.
    IND_TYPE *jc; // column repartition on pr and ir.
    size_type nc, nr;

    typedef T value_type;
    typedef T &access_type;

    template <typename Matrix>
    void init_with_good_format(const Matrix &B);
    template <typename Matrix>
    void init_with(const Matrix &A);
    void init_with(const col_matrix<gmm::rsvector<T> > &B)
    {
        init_with_good_format(B);
    }
    void init_with(const col_matrix<wsvector<T> > &B)
    {
        init_with_good_format(B);
    }
    template <typename PT1, typename PT2, typename PT3, int cshift>
    void init_with(const csc_matrix_ref<PT1, PT2, PT3, cshift> &B)
    {
        init_with_good_format(B);
    }
    void init_with_identity(size_type n);

    csc_matrix(void)
        : pr(0)
        , ir(0)
        , jc(0)
        , nc(0)
        , nr(0)
    {
    }
    csc_matrix(size_type nnr, size_type nnc);
    ~csc_matrix()
    {
        if (pr)
        {
            delete[] pr;
            delete[] ir;
            delete[] jc;
        }
    }

    size_type nrows(void) const
    {
        return nr;
    }
    size_type ncols(void) const
    {
        return nc;
    }
    void swap(csc_matrix<T, shift> &m)
    {
        std::swap(pr, m.pr);
        std::swap(ir, m.ir);
        std::swap(jc, m.jc);
        std::swap(nc, m.nc);
        std::swap(nr, m.nr);
    }
    value_type operator()(size_type i, size_type j) const
    {
        return mat_col(*this, j)[i];
    }
};

template <typename T, int shift>
template <typename Matrix>
void csc_matrix<T, shift>::init_with_good_format(const Matrix &B)
{
    typedef typename linalg_traits<Matrix>::const_sub_col_type col_type;
    if (pr)
    {
        delete[] pr;
        delete[] ir;
        delete[] jc;
    }
    nc = mat_ncols(B);
    nr = mat_nrows(B);
    jc = new IND_TYPE[nc + 1];
    jc[0] = shift;
    for (size_type j = 0; j < nc; ++j)
    {
        jc[j + 1] = jc[j] + nnz(mat_const_col(B, j));
    }
    pr = new T[jc[nc]];
    ir = new IND_TYPE[jc[nc]];
    for (size_type j = 0; j < nc; ++j)
    {
        col_type col = mat_const_col(B, j);
        typename linalg_traits<col_type>::const_iterator
            it = vect_const_begin(col),
            ite = vect_const_end(col);
        for (size_type k = 0; it != ite; ++it, ++k)
        {
            pr[jc[j] - shift + k] = *it;
            ir[jc[j] - shift + k] = it.index() + shift;
        }
    }
}

template <typename T, int shift>
template <typename Matrix>
void csc_matrix<T, shift>::init_with(const Matrix &A)
{
    col_matrix<wsvector<T> > B(mat_nrows(A), mat_ncols(A));
    copy(A, B);
    init_with_good_format(B);
}

template <typename T, int shift>
void csc_matrix<T, shift>::init_with_identity(size_type n)
{
    if (pr)
    {
        delete[] pr;
        delete[] ir;
        delete[] jc;
    }
    nc = nr = n;
    pr = new T[nc];
    ir = new IND_TYPE[nc];
    jc = new IND_TYPE[nc + 1];
    for (size_type j = 0; j < nc; ++j)
    {
        ir[j] = jc[j] = shift + j;
        pr[j] = T(1);
    }
    jc[nc] = shift + nc;
}

template <typename T, int shift>
csc_matrix<T, shift>::csc_matrix(size_type nnr, size_type nnc)
    : nc(nnc)
    , nr(nnr)
{
    pr = new T[1];
    ir = new IND_TYPE[1];
    jc = new IND_TYPE[nc + 1];
    for (size_type j = 0; j <= nc; ++j)
        jc[j] = shift;
}

template <typename T, int shift>
struct linalg_traits<csc_matrix<T, shift> >
{
    typedef csc_matrix<T, shift> this_type;
    typedef typename this_type::IND_TYPE IND_TYPE;
    typedef linalg_const is_reference;
    typedef abstract_matrix linalg_type;
    typedef T value_type;
    typedef T origin_type;
    typedef T reference;
    typedef abstract_sparse storage_type;
    typedef abstract_null_type sub_row_type;
    typedef abstract_null_type const_sub_row_type;
    typedef abstract_null_type row_iterator;
    typedef abstract_null_type const_row_iterator;
    typedef abstract_null_type sub_col_type;
    typedef cs_vector_ref<const T *, const IND_TYPE *, shift>
        const_sub_col_type;
    typedef sparse_compressed_iterator<const T *, const IND_TYPE *,
                                       const IND_TYPE *, shift>
        const_col_iterator;
    typedef abstract_null_type col_iterator;
    typedef col_major sub_orientation;
    typedef linalg_true index_sorted;
    static size_type nrows(const this_type &m)
    {
        return m.nrows();
    }
    static size_type ncols(const this_type &m)
    {
        return m.ncols();
    }
    static const_col_iterator col_begin(const this_type &m)
    {
        return const_col_iterator(m.pr, m.ir, m.jc, m.nr, m.pr);
    }
    static const_col_iterator col_end(const this_type &m)
    {
        return const_col_iterator(m.pr, m.ir, m.jc + m.nc, m.nr, m.pr);
    }
    static const_sub_col_type col(const const_col_iterator &it)
    {
        return const_sub_col_type(it.pr + *(it.jc) - shift,
                                  it.ir + *(it.jc) - shift,
                                  *(it.jc + 1) - *(it.jc), it.n);
    }
    static const origin_type *origin(const this_type &m)
    {
        return m.pr;
    }
    static void do_clear(this_type &m)
    {
        m.do_clear();
    }
    static value_type access(const const_col_iterator &itcol, size_type j)
    {
        return col(itcol)[j];
    }
};

template <typename T, int shift>
std::ostream &operator<<(std::ostream &o, const csc_matrix<T, shift> &m)
{
    gmm::write(o, m);
    return o;
}

template <typename T, int shift>
inline void copy(const identity_matrix &, csc_matrix<T, shift> &M)
{
    M.init_with_identity(mat_nrows(M));
}

template <typename Matrix, typename T, int shift>
inline void copy(const Matrix &A, csc_matrix<T, shift> &M)
{
    M.init_with(A);
}

/* ******************************************************************** */
/*                                                                      */
/*	        Read only compressed sparse row matrix                    */
/*                                                                      */
/* ******************************************************************** */

template <typename T, int shift = 0>
struct csr_matrix
{

    typedef unsigned int IND_TYPE;

    T *pr; // values.
    IND_TYPE *ir; // col indices.
    IND_TYPE *jc; // row repartition on pr and ir.
    size_type nc, nr;

    typedef T value_type;
    typedef T &access_type;

    template <typename Matrix>
    void init_with_good_format(const Matrix &B);
    void init_with(const row_matrix<wsvector<T> > &B)
    {
        init_with_good_format(B);
    }
    void init_with(const row_matrix<rsvector<T> > &B)
    {
        init_with_good_format(B);
    }
    template <typename PT1, typename PT2, typename PT3, int cshift>
    void init_with(const csr_matrix_ref<PT1, PT2, PT3, cshift> &B)
    {
        init_with_good_format(B);
    }

    template <typename Matrix>
    void init_with(const Matrix &A);
    void init_with_identity(size_type n);

    csr_matrix(void)
        : pr(0)
        , ir(0)
        , jc(0)
        , nc(0)
        , nr(0)
    {
    }
    csr_matrix(size_type nnr, size_type nnc);
    ~csr_matrix()
    {
        if (pr)
        {
            delete[] pr;
            delete[] ir;
            delete[] jc;
        }
    }

    size_type nrows(void) const
    {
        return nr;
    }
    size_type ncols(void) const
    {
        return nc;
    }
    void swap(csr_matrix<T, shift> &m)
    {
        std::swap(pr, m.pr);
        std::swap(ir, m.ir);
        std::swap(jc, m.jc);
        std::swap(nc, m.nc);
        std::swap(nr, m.nr);
    }

    value_type operator()(size_type i, size_type j) const
    {
        return mat_row(*this, i)[j];
    }
};

template <typename T, int shift>
template <typename Matrix>
void csr_matrix<T, shift>::init_with_good_format(const Matrix &B)
{
    typedef typename linalg_traits<Matrix>::const_sub_row_type row_type;
    if (pr)
    {
        delete[] pr;
        delete[] ir;
        delete[] jc;
    }
    nc = mat_ncols(B);
    nr = mat_nrows(B);
    jc = new IND_TYPE[nr + 1];
    jc[0] = shift;
    for (size_type j = 0; j < nr; ++j)
    {
        jc[j + 1] = jc[j] + nnz(mat_const_row(B, j));
    }
    pr = new T[jc[nr]];
    ir = new IND_TYPE[jc[nr]];
    for (size_type j = 0; j < nr; ++j)
    {
        row_type row = mat_const_row(B, j);
        typename linalg_traits<row_type>::const_iterator
            it = vect_const_begin(row),
            ite = vect_const_end(row);
        for (size_type k = 0; it != ite; ++it, ++k)
        {
            pr[jc[j] - shift + k] = *it;
            ir[jc[j] - shift + k] = it.index() + shift;
        }
    }
}

template <typename T, int shift>
template <typename Matrix>
void csr_matrix<T, shift>::init_with(const Matrix &A)
{
    row_matrix<wsvector<T> > B(mat_nrows(A), mat_ncols(A));
    copy(A, B);
    init_with_good_format(B);
}

template <typename T, int shift>
void csr_matrix<T, shift>::init_with_identity(size_type n)
{
    if (pr)
    {
        delete[] pr;
        delete[] ir;
        delete[] jc;
    }
    nc = nr = n;
    pr = new T[nr];
    ir = new IND_TYPE[nr];
    jc = new IND_TYPE[nr + 1];
    for (size_type j = 0; j < nr; ++j)
    {
        ir[j] = jc[j] = shift + j;
        pr[j] = T(1);
    }
    jc[nr] = shift + nr;
}

template <typename T, int shift>
csr_matrix<T, shift>::csr_matrix(size_type nnr, size_type nnc)
    : nc(nnc)
    , nr(nnr)
{
    pr = new T[1];
    ir = new IND_TYPE[1];
    jc = new IND_TYPE[nr + 1];
    for (size_type j = 0; j < nr; ++j)
        jc[j] = shift;
    jc[nr] = shift;
}

template <typename T, int shift>
struct linalg_traits<csr_matrix<T, shift> >
{
    typedef csr_matrix<T, shift> this_type;
    typedef typename this_type::IND_TYPE IND_TYPE;
    typedef linalg_const is_reference;
    typedef abstract_matrix linalg_type;
    typedef T value_type;
    typedef T origin_type;
    typedef T reference;
    typedef abstract_sparse storage_type;
    typedef abstract_null_type sub_col_type;
    typedef abstract_null_type const_sub_col_type;
    typedef abstract_null_type col_iterator;
    typedef abstract_null_type const_col_iterator;
    typedef abstract_null_type sub_row_type;
    typedef cs_vector_ref<const T *, const IND_TYPE *, shift>
        const_sub_row_type;
    typedef sparse_compressed_iterator<const T *, const IND_TYPE *,
                                       const IND_TYPE *, shift>
        const_row_iterator;
    typedef abstract_null_type row_iterator;
    typedef row_major sub_orientation;
    typedef linalg_true index_sorted;
    static size_type nrows(const this_type &m)
    {
        return m.nrows();
    }
    static size_type ncols(const this_type &m)
    {
        return m.ncols();
    }
    static const_row_iterator row_begin(const this_type &m)
    {
        return const_row_iterator(m.pr, m.ir, m.jc, m.nr, m.pr);
    }
    static const_row_iterator row_end(const this_type &m)
    {
        return const_row_iterator(m.pr, m.ir, m.jc + m.nc, m.nr, m.pr);
    }
    static const_sub_row_type row(const const_row_iterator &it)
    {
        return const_sub_row_type(it.pr + *(it.jc) - shift,
                                  it.ir + *(it.jc) - shift,
                                  *(it.jc + 1) - *(it.jc), it.n);
    }
    static const origin_type *origin(const this_type &m)
    {
        return m.pr;
    }
    static void do_clear(this_type &m)
    {
        m.do_clear();
    }
    static value_type access(const const_row_iterator &itrow, size_type j)
    {
        return row(itrow)[j];
    }
};

template <typename T, int shift>
std::ostream &operator<<(std::ostream &o, const csr_matrix<T, shift> &m)
{
    gmm::write(o, m);
    return o;
}

template <typename T, int shift>
inline void copy(const identity_matrix &, csr_matrix<T, shift> &M)
{
    M.init_with_identity(mat_nrows(M));
}

template <typename Matrix, typename T, int shift>
inline void copy(const Matrix &A, csr_matrix<T, shift> &M)
{
    M.init_with(A);
}

/* ******************************************************************** */
/*		                                            		  */
/*		Block matrix                                		  */
/*		                                            		  */
/* ******************************************************************** */

template <typename MAT>
class block_matrix
{
protected:
    std::vector<MAT> blocks;
    size_type nrowblocks_;
    size_type ncolblocks_;
    std::vector<sub_interval> introw, intcol;

public:
    typedef typename linalg_traits<MAT>::value_type value_type;
    typedef typename linalg_traits<MAT>::reference reference;

    size_type nrows(void) const
    {
        return introw[nrowblocks_ - 1].max;
    }
    size_type ncols(void) const
    {
        return intcol[ncolblocks_ - 1].max;
    }
    size_type nrowblocks(void) const
    {
        return nrowblocks_;
    }
    size_type ncolblocks(void) const
    {
        return ncolblocks_;
    }
    const sub_interval &subrowinterval(size_type i) const
    {
        return introw[i];
    }
    const sub_interval &subcolinterval(size_type i) const
    {
        return intcol[i];
    }
    const MAT &block(size_type i, size_type j) const
    {
        return blocks[j * ncolblocks_ + i];
    }
    MAT &block(size_type i, size_type j)
    {
        return blocks[j * ncolblocks_ + i];
    }
    void do_clear(void);
    // to be done : read and write access to a component
    value_type operator()(size_type i, size_type j) const
    {
        size_type k, l;
        for (k = 0; k < nrowblocks_; ++k)
            if (i >= introw[k].min && i < introw[k].max)
                break;
        for (l = 0; l < nrowblocks_; ++l)
            if (j >= introw[l].min && j < introw[l].max)
                break;
        return (block(k, l))(i - introw[k].min, j - introw[l].min);
    }
    reference operator()(size_type i, size_type j)
    {
        size_type k, l;
        for (k = 0; k < nrowblocks_; ++k)
            if (i >= introw[k].min && i < introw[k].max)
                break;
        for (l = 0; l < nrowblocks_; ++l)
            if (j >= introw[l].min && j < introw[l].max)
                break;
        return (block(k, l))(i - introw[k].min, j - introw[l].min);
    }

    template <typename CONT>
    void resize(const CONT &c1, const CONT &c2);
    template <typename CONT>
    block_matrix(const CONT &c1, const CONT &c2)
    {
        resize(c1, c2);
    }
    block_matrix(void)
    {
    }
};

template <typename MAT>
struct linalg_traits<block_matrix<MAT> >
{
    typedef block_matrix<MAT> this_type;
    typedef linalg_false is_reference;
    typedef abstract_matrix linalg_type;
    typedef this_type origin_type;
    typedef typename linalg_traits<MAT>::value_type value_type;
    typedef typename linalg_traits<MAT>::reference reference;
    typedef typename linalg_traits<MAT>::storage_type storage_type;
    typedef abstract_null_type sub_row_type; // to be done ...
    typedef abstract_null_type const_sub_row_type; // to be done ...
    typedef abstract_null_type row_iterator; // to be done ...
    typedef abstract_null_type const_row_iterator; // to be done ...
    typedef abstract_null_type sub_col_type; // to be done ...
    typedef abstract_null_type const_sub_col_type; // to be done ...
    typedef abstract_null_type col_iterator; // to be done ...
    typedef abstract_null_type const_col_iterator; // to be done ...
    typedef abstract_null_type sub_orientation; // to be done ...
    typedef linalg_true index_sorted;
    static size_type nrows(const this_type &m)
    {
        return m.nrows();
    }
    static size_type ncols(const this_type &m)
    {
        return m.ncols();
    }
    static origin_type *origin(this_type &m)
    {
        return &m;
    }
    static const origin_type *origin(const this_type &m)
    {
        return &m;
    }
    static void do_clear(this_type &m)
    {
        m.do_clear();
    }
    // access to be done ...
    static void resize(this_type &, size_type, size_type)
    {
        DAL_THROW(failure_error, "Sorry, to be done");
    }
    static void reshape(this_type &, size_type, size_type)
    {
        DAL_THROW(failure_error, "Sorry, to be done");
    }
};

template <typename MAT>
void block_matrix<MAT>::do_clear(void)
{
    for (size_type j = 0, l = 0; j < ncolblocks_; ++j)
        for (size_type i = 0, k = 0; i < nrowblocks_; ++i)
            clear(block(i, j));
}

template <typename MAT>
template <typename CONT>
void block_matrix<MAT>::resize(const CONT &c1, const CONT &c2)
{
    nrowblocks_ = c1.size();
    ncolblocks_ = c2.size();
    blocks.resize(nrowblocks_ * ncolblocks_);
    intcol.resize(ncolblocks_);
    introw.resize(nrowblocks_);
    for (size_type j = 0, l = 0; j < ncolblocks_; ++j)
    {
        intcol[j] = sub_interval(l, c2[j]);
        l += c2[j];
        for (size_type i = 0, k = 0; i < nrowblocks_; ++i)
        {
            if (j == 0)
            {
                introw[i] = sub_interval(k, c1[i]);
                k += c1[i];
            }
            block(i, j) = MAT(c1[i], c2[j]);
        }
    }
}

template <typename M1, typename M2>
void copy(const block_matrix<M1> &m1, M2 &m2)
{
    for (size_type j = 0; j < m1.ncolblocks(); ++j)
        for (size_type i = 0; i < m1.nrowblocks(); ++i)
            copy(m1.block(i, j), sub_matrix(m2, m1.subrowinterval(i),
                                            m1.subcolinterval(j)));
}

template <typename M1, typename M2>
void copy(const block_matrix<M1> &m1, const M2 &m2)
{
    copy(m1, linalg_const_cast(m2));
}

template <typename MAT, typename V1, typename V2>
void mult(const block_matrix<MAT> &m, const V1 &v1, V2 &v2)
{
    clear(v2);
    typename sub_vector_type<V2 *, sub_interval>::vector_type sv;
    for (size_type i = 0; i < m.nrowblocks(); ++i)
        for (size_type j = 0; j < m.ncolblocks(); ++j)
        {
            sv = sub_vector(v2, m.subrowinterval(i));
            mult(m.block(i, j),
                 sub_vector(v1, m.subcolinterval(j)), sv, sv);
        }
}

template <typename MAT, typename V1, typename V2, typename V3>
void mult(const block_matrix<MAT> &m, const V1 &v1, const V2 &v2, V3 &v3)
{
    typename sub_vector_type<V3 *, sub_interval>::vector_type sv;
    for (size_type i = 0; i < m.nrowblocks(); ++i)
        for (size_type j = 0; j < m.ncolblocks(); ++j)
        {
            sv = sub_vector(v3, m.subrowinterval(i));
            if (j == 0)
                mult(m.block(i, j),
                     sub_vector(v1, m.subcolinterval(j)),
                     sub_vector(v2, m.subrowinterval(i)), sv);
            else
                mult(m.block(i, j),
                     sub_vector(v1, m.subcolinterval(j)), sv, sv);
        }
}

template <typename MAT, typename V1, typename V2>
void mult(const block_matrix<MAT> &m, const V1 &v1, const V2 &v2)
{
    mult(m, v1, linalg_const_cast(v2));
}

template <typename MAT, typename V1, typename V2, typename V3>
void mult(const block_matrix<MAT> &m, const V1 &v1, const V2 &v2,
          const V3 &v3)
{
    mult_const(m, v1, v2, linalg_const_cast(v3));
}
}

namespace std
{
template <typename V>
void swap(gmm::row_matrix<V> &m1, gmm::row_matrix<V> &m2)
{
    m1.swap(m2);
}
template <typename V>
void swap(gmm::col_matrix<V> &m1, gmm::col_matrix<V> &m2)
{
    m1.swap(m2);
}
template <typename T>
void swap(gmm::dense_matrix<T> &m1, gmm::dense_matrix<T> &m2)
{
    m1.swap(m2);
}
template <typename T, int shift>
void
swap(gmm::csc_matrix<T, shift> &m1, gmm::csc_matrix<T, shift> &m2)
{
    m1.swap(m2);
}
template <typename T, int shift>
void
swap(gmm::csr_matrix<T, shift> &m1, gmm::csr_matrix<T, shift> &m2)
{
    m1.swap(m2);
}
}

#endif /* GMM_MATRIX_H__ */

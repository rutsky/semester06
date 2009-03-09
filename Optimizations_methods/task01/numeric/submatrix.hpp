/*
 * submatrix.hpp
 * Submatrix of selected rows and columns class implementation.
 * Based on boost::numeric::ublas::matrix_range.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.03.2009
 */

#ifndef NUMERIC_SUBMATRIX_HPP
#define NUMERIC_SUBMATRIX_HPP
 
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/detail/vector_assign.hpp>
#include <boost/numeric/ublas/detail/matrix_assign.hpp>
#include <boost/numeric/ublas/detail/temporary.hpp>

#include "numeric_common.hpp"

namespace numeric
{
  template< class M >
  class matrix_submatrix 
    : public ublas::matrix_expression<matrix_submatrix<M> >
  {
  public:
    typedef matrix_submatrix<M> self_type;
    
    typedef M                           matrix_type;
    typedef typename M::size_type       size_type;
    typedef typename M::difference_type difference_type;
    typedef typename M::value_type      value_type;
    typedef typename M::const_reference const_reference;
    typedef typename boost::mpl::if_<boost::is_const<M>,
                                     typename M::const_reference,
                                     typename M::reference>::type reference;
    
    typedef typename boost::mpl::if_<boost::is_const<M>,
                                     typename M::const_closure_type,
                                     typename M::closure_type>::type matrix_closure_type;
    typedef const self_type             const_closure_type;
    typedef self_type                   closure_type;
    typedef typename ublas::storage_restrict_traits<typename M::storage_category, 
                                                    ublas::dense_proxy_tag>::storage_category storage_category;
    typedef typename M::orientation_category orientation_category;
    
  public:
    template< class RowsForwardIterator, class ColsForwardIterator >
    matrix_submatrix( matrix_type &data, 
                      RowsForwardIterator rowsBegin, RowsForwardIterator rowsEnd,
                      ColsForwardIterator colsBegin, ColsForwardIterator colsEnd )
      : data_       (data)
    {
      BOOST_ASSERT(rowsBegin != rowsEnd);
      BOOST_ASSERT(colsBegin != colsEnd);
      
      rowsIndices_.assign(rowsBegin, rowsEnd);
      colsIndices_.assign(colsBegin, colsEnd);
      
      // debug
      // TODO: Rewrite so that in release mode it will not produce overheads.
      for (size_t r = 0; r < rowsIndices_.size(); ++r)
        BOOST_ASSERT(rowsIndices_[r] < data_.size1());
      for (size_t c = 0; c < colsIndices_.size(); ++c)
        BOOST_ASSERT(colsIndices_[c] < data_.size2());
      // end of debug
    }
    
    // Accessors.
    size_type size1() const 
    {
      return rowsIndices_.size();
    }
    size_type size2() const 
    {
      return colsIndices_.size();
    }

    // Storage accessors.
    matrix_closure_type const & data() const 
    {
      return data_;
    }
    
    matrix_closure_type       & data () 
    {
      return data_;
    }

    // Element access.
    const_reference operator () ( size_type r, size_type c ) const 
    {
      BOOST_ASSERT(r < rowsIndices_.size());
      BOOST_ASSERT(c < colsIndices_.size());
      return data_(rowsIndices_[r], colsIndices_[c]);
    }

    reference       operator () ( size_type r, size_type c )
    {
      BOOST_ASSERT(r < rowsIndices_.size());
      BOOST_ASSERT(c < colsIndices_.size());
      return data_(rowsIndices_[r], colsIndices_[c]);
    }

    // Assignment.
    matrix_submatrix & operator = ( matrix_submatrix const &sm )
    {
      ublas::matrix_assign<ublas::scalar_assign>(*this, sm);
      return *this;
    }
    matrix_submatrix & assign_temporary( matrix_submatrix &sm )
    {
      return *this = sm;
    }
    template< class AE >
    matrix_submatrix & operator = ( ublas::matrix_expression<AE> const &ae ) 
    {
      ublas::matrix_assign<ublas::scalar_assign>(*this, typename ublas::matrix_temporary_traits<M>::type(ae));
      return *this;
    }
    template< class AE >
    matrix_submatrix & assign ( ublas::matrix_expression<AE> const &ae )  
    {
      ublas::matrix_assign<ublas::scalar_assign>(*this, ae);
      return *this;
    }
    template< class AE >
    matrix_submatrix & operator += ( ublas::matrix_expression<AE> const &ae ) 
    {
      ublas::matrix_assign<ublas::scalar_assign>(*this, typename ublas::matrix_temporary_traits<M>::type(*this + ae));
      return *this;
    }
    template< class AE >
    matrix_submatrix & plus_assign ( ublas::matrix_expression<AE> const &ae )
    {
      ublas::matrix_assign<ublas::scalar_plus_assign>(*this, ae);
      return *this;
    }
    template< class AE >
    matrix_submatrix & operator -= ( ublas::matrix_expression<AE> const &ae )
    {
      ublas::matrix_assign<ublas::scalar_assign>(*this, typename ublas::matrix_temporary_traits<M>::type(*this - ae));
      return *this;
    }
    template< class AE >
    matrix_submatrix & minus_assign( ublas::matrix_expression<AE> const &ae )
    {
      ublas::matrix_assign<ublas::scalar_minus_assign>(*this, ae);
      return *this;
    }
    template< class AT >
    matrix_submatrix & operator *= ( AT const &at ) 
    {
      ublas::matrix_assign_scalar<ublas::scalar_multiplies_assign>(*this, at);
      return *this;
    }
    template< class AT >
    matrix_submatrix & operator /= ( AT const &at )
    {
      ublas::matrix_assign_scalar<ublas::scalar_divides_assign>(*this, at);
      return *this;
    }

    // Closure comparison
    bool same_closure ( matrix_submatrix const &mr ) const
    {
      return (*this).data_.same_closure(mr.data_);
    }

    // Comparison
    bool operator == ( matrix_submatrix const &sm ) const
    {
      return 
          (*this).data_ == (sm.data_)            &&
          (*this).rowsIndices_ == sm.rowsIndices &&
          (*this).colsIndices_ == sm.colsIndices;
    }

    // Swapping
    void swap( matrix_submatrix sm )
    {
      if (this != &sm)
      {
        BOOST_ASSERT(size1() == sm.size1());
        BOOST_ASSERT(size2() == sm.size2());
        ublas::matrix_swap<ublas::scalar_swap>(*this, sm);
      }
    }
    friend void swap( matrix_submatrix sm1, matrix_submatrix sm2 )
    {
      sm1.swap(sm2);
    }

    // Iterator types
  private:
    typedef typename M::const_iterator1 const_subiterator1_type;
    typedef typename boost::mpl::if_<boost::is_const<M>,
                                      typename M::const_iterator1,
                                      typename M::iterator1>::type subiterator1_type;
    typedef typename M::const_iterator2 const_subiterator2_type;
    typedef typename boost::mpl::if_<boost::is_const<M>,
                                      typename M::const_iterator2,
                                      typename M::iterator2>::type subiterator2_type;

  public:
    class const_iterator1;
    class iterator1;
    class const_iterator2;
    class iterator2;
    typedef ublas::reverse_iterator_base1<const_iterator1> const_reverse_iterator1;
    typedef ublas::reverse_iterator_base1<iterator1> reverse_iterator1;
    typedef ublas::reverse_iterator_base2<const_iterator2> const_reverse_iterator2;
    typedef ublas::reverse_iterator_base2<iterator2> reverse_iterator2;

    // Element lookup
    // TODO: 'rank' argument.
    const_iterator1 find1( int rank, size_type r, size_type c ) const 
    {
      BOOST_ASSERT(rank == 0); // FIXME
      return const_iterator1(*this, r, c);
    }
    iterator1       find1( int rank, size_type r, size_type c )
    {
      BOOST_ASSERT(rank == 0); // FIXME
      return iterator1(*this, r, c);
    }
    const_iterator2 find2( int rank, size_type r, size_type c ) const 
    {
      BOOST_ASSERT(rank == 0); // FIXME
      return const_iterator2(*this, r, c);
    }
    iterator2       find2( int rank, size_type r, size_type c ) 
    {
      BOOST_ASSERT(rank == 0); // FIXME
      return iterator2(*this, r, c);
    }

    class const_iterator1
      : public ublas::container_const_reference<matrix_submatrix>
      , public ublas::iterator_base_traits<std::random_access_iterator_tag>::template
                  iterator_base<const_iterator1, value_type>::type 
    {
    public:
      typedef typename matrix_submatrix::value_type      value_type;
      typedef typename matrix_submatrix::difference_type difference_type;
      typedef typename matrix_submatrix::reference       reference;
      typedef typename matrix_submatrix::size_type       size_type;
      typedef const_iterator2                     dual_iterator_type;
      typedef const_reverse_iterator2             dual_reverse_iterator_type;

      // Construction and destruction
      const_iterator1()
        : ublas::container_const_reference<self_type>()
        , r_(-1)
        , c_(-1)
      {
      }
      const_iterator1( self_type const &sm, size_type r, size_type c )
        : ublas::container_const_reference<self_type>(sm)
        , r_(r)
        , c_(c) 
      {
      }

      // Arithmetic
      const_iterator1 & operator ++ () 
      {
        ++r_;
        return *this;
      }
      const_iterator1 & operator -- () 
      {
        --r_;
        return *this;
      }
      const_iterator1 & operator += ( difference_type n )
      {
        r_ += n;
        return *this;
      }
      const_iterator1 & operator -= ( difference_type n ) 
      {
        r_ -= n;
        return *this;
      }
      difference_type operator - ( const_iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_);
        return r_ - it.r_;
      }

      // Dereference
      const_reference operator * () const 
      {
        return (*this)()(r_, c_);
      }
      const_reference operator [] ( difference_type n ) const 
      {
        return *(*this + n);
      }

      // Indices
      size_type index1() const 
      {
        return r_;
      }
      size_type index2() const 
      {
        return c_;
      }

      // Assignment
      const_iterator1 & operator = ( const_iterator1 const &it ) 
      {
        ublas::container_const_reference<self_type>::assign(&it());
        r_ = it.r_;
        c_ = it.c_;
        return *this;
      }

      // Comparison
      bool operator == ( const_iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_); // TODO
        return r_ == it.r_;
      }
      bool operator < ( const_iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_); // TODO
        return r_ < it.r_;
      }

    private:
      size_type r_;
      size_type c_;
    };

    const_iterator1 begin1() const
    {
      return find1(0, 0, 0);
    }
    const_iterator1 end1() const
    {
      return find1(0, size1(), 0);
    }

    class iterator1
      : public ublas::container_reference<matrix_submatrix>
      , public ublas::iterator_base_traits<std::random_access_iterator_tag>::template
                  iterator_base<iterator1, value_type>::type 
    {
    public:
      typedef typename matrix_submatrix::value_type      value_type;
      typedef typename matrix_submatrix::difference_type difference_type;
      typedef typename matrix_submatrix::reference       reference;
      typedef typename matrix_submatrix::size_type       size_type;
      typedef iterator2                           dual_iterator_type;
      typedef reverse_iterator2                   dual_reverse_iterator_type;

      // Construction and destruction
      iterator1()
        : ublas::container_reference<self_type>()
        , r_(-1)
        , c_(-1)
      {
      }
      iterator1( self_type &sm, size_type r, size_type c )
        : ublas::container_reference<self_type>(sm)
        , r_(r)
        , c_(c) 
      {
      }

      // Arithmetic
      iterator1 & operator ++ () 
      {
        ++r_;
        return *this;
      }
      iterator1 & operator -- () 
      {
        --r_;
        return *this;
      }
      iterator1 & operator += ( difference_type n )
      {
        r_ += n;
        return *this;
      }
      iterator1 & operator -= ( difference_type n ) 
      {
        r_ -= n;
        return *this;
      }
      difference_type operator - ( iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_);
        return r_ - it.r_;
      }

      // Dereference
      reference operator * () const 
      {
        return (*this)()(r_, c_);
      }
      reference operator [] ( difference_type n ) const 
      {
        return *(*this + n);
      }

      // Indices
      size_type index1() const 
      {
        return r_;
      }
      size_type index2() const 
      {
        return c_;
      }

      // Assignment
      iterator1 & operator = ( iterator1 const &it ) 
      {
        ublas::container_reference<self_type>::assign(&it());
        r_ = it.r_;
        c_ = it.c_;
        return *this;
      }

      // Comparison
      bool operator == ( iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_); // TODO
        return r_ == it.r_;
      }
      bool operator < ( const_iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_); // TODO
        return r_ < it.r_;
      }

    private:
      size_type r_;
      size_type c_;
      
      friend class const_iterator1;
    };

    iterator1 begin1()
    {
      return find1(0, 0, 0);
    }
    iterator1 end1()
    {
      return find1(0, size1(), 0);
    }

    class const_iterator2
      : public ublas::container_const_reference<matrix_submatrix>
      , public ublas::iterator_base_traits<std::random_access_iterator_tag>::template
                  iterator_base<const_iterator2, value_type>::type 
    {
    public:
      typedef typename matrix_submatrix::value_type      value_type;
      typedef typename matrix_submatrix::difference_type difference_type;
      typedef typename matrix_submatrix::reference       reference;
      typedef typename matrix_submatrix::size_type       size_type;
      typedef const_iterator1                     dual_iterator_type;
      typedef const_reverse_iterator1             dual_reverse_iterator_type;

      // Construction and destruction
      const_iterator2()
        : ublas::container_const_reference<self_type>()
        , r_(-1)
        , c_(-1)
      {
      }
      const_iterator2( self_type const &sm, size_type r, size_type c )
        : ublas::container_const_reference<self_type>(sm)
        , r_(r)
        , c_(c) 
      {
      }

      // Arithmetic
      const_iterator2 & operator ++ () 
      {
        ++c_;
        return *this;
      }
      const_iterator2 & operator -- () 
      {
        --c_;
        return *this;
      }
      const_iterator2 & operator += ( difference_type n )
      {
        c_ += n;
        return *this;
      }
      const_iterator2 & operator -= ( difference_type n ) 
      {
        c_ -= n;
        return *this;
      }
      difference_type operator - ( const_iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(r_ == it.r_);
        return c_ - it.c_;
      }

      // Dereference
      const_reference operator * () const 
      {
        return (*this)()(r_, c_);
      }
      const_reference operator [] ( difference_type n ) const 
      {
        return *(*this + n);
      }

      // Indices
      size_type index1() const 
      {
        return r_;
      }
      size_type index2() const 
      {
        return c_;
      }

      // Assignment
      const_iterator2 & operator = ( const_iterator2 const &it ) 
      {
        ublas::container_const_reference<self_type>::assign(&it());
        r_ = it.r_;
        c_ = it.c_;
        return *this;
      }

      // Comparison
      bool operator == ( const_iterator2 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(r_ == it.r_); // TODO
        return c_ == it.c_;
      }
      bool operator < ( const_iterator2 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(r_ == it.r_); // TODO
        return c_ < it.c_;
      }

    private:
      size_type r_;
      size_type c_;
    };

    const_iterator2 begin2() const 
    {
      return find2(0, 0, 0);
    }
    const_iterator2 end2() const 
    {
      return find2(0, 0, size2());
    }

    class iterator2
      : public ublas::container_reference<matrix_submatrix>
      , public ublas::iterator_base_traits<std::random_access_iterator_tag>::template
                  iterator_base<iterator2, value_type>::type 
    {
    public:
      typedef typename matrix_submatrix::value_type      value_type;
      typedef typename matrix_submatrix::difference_type difference_type;
      typedef typename matrix_submatrix::reference       reference;
      typedef typename matrix_submatrix::size_type       size_type;
      typedef iterator1                           dual_iterator_type;
      typedef reverse_iterator1                   dual_reverse_iterator_type;

      // Construction and destruction
      iterator2()
        : ublas::container_reference<self_type>()
        , r_(-1)
        , c_(-1)
      {
      }
      iterator2( self_type &sm, size_type r, size_type c )
        : ublas::container_reference<self_type>(sm)
        , r_(r)
        , c_(c) 
      {
      }

      // Arithmetic
      iterator2 & operator ++ () 
      {
        ++c_;
        return *this;
      }
      iterator2 & operator -- () 
      {
        --c_;
        return *this;
      }
      iterator2 & operator += ( difference_type n )
      {
        c_ += n;
        return *this;
      }
      iterator2 & operator -= ( difference_type n ) 
      {
        c_ -= n;
        return *this;
      }
      difference_type operator - ( iterator1 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(r_ == it.r_);
        return c_ - it.c_;
      }

      // Dereference
      reference operator * () const 
      {
        return (*this)()(r_, c_);
      }
      reference operator [] ( difference_type n ) const 
      {
        return *(*this + n);
      }

      // Indices
      size_type index1() const 
      {
        return r_;
      }
      size_type index2() const 
      {
        return c_;
      }

      // Assignment
      iterator2 & operator = ( iterator2 const &it ) 
      {
        ublas::container_reference<self_type>::assign(&it());
        r_ = it.r_;
        c_ = it.c_;
        return *this;
      }

      // Comparison
      bool operator == ( iterator2 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(r_ == it.r_); // TODO
        return c_ == it.c_;
      }
      bool operator < ( const_iterator2 const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        BOOST_ASSERT(c_ == it.c_); // TODO
        return r_ < it.r_;
      }

    private:
      size_type r_;
      size_type c_;
      
      friend class const_iterator2;
    };

    iterator2 begin2() 
    {
      return find2(0, 0, 0);
    }
    iterator2 end2() 
    {
      return find2(0, 0, size2());
    }

    // Reverse iterators
    const_reverse_iterator1 rbegin1() const 
    {
      return const_reverse_iterator1(end1());
    }
    const_reverse_iterator1 rend1() const 
    {
      return const_reverse_iterator1(begin1());
    }

    reverse_iterator1 rbegin1() 
    {
      return reverse_iterator1(end1());
    }
    reverse_iterator1 rend1() 
    {
      return reverse_iterator1(begin1());
    }

    const_reverse_iterator2 rbegin2() const 
    {
      return const_reverse_iterator2(end2());
    }
    const_reverse_iterator2 rend2() const 
    {
      return const_reverse_iterator2(begin2());
    }

    reverse_iterator2 rbegin2() 
    {
      return reverse_iterator2(end2());
    }
    reverse_iterator2 rend2() 
    {
      return reverse_iterator2(begin2());
    }

  private:
    matrix_closure_type data_;
    std::vector<size_t> rowsIndices_, colsIndices_;
  };
  
  template< class MatrixType, class RowsForwardIterator, class ColsForwardIterator >
  inline
  matrix_submatrix<MatrixType> submatrix( MatrixType &data, 
                                          RowsForwardIterator rowsBegin, RowsForwardIterator rowsEnd,
                                          ColsForwardIterator colsBegin, ColsForwardIterator colsEnd )
  {
    return matrix_submatrix<MatrixType>(data, rowsBegin, rowsEnd, colsBegin, colsEnd);
  }
  
  template< class MatrixType, class RowsForwardIterator, class ColsForwardIterator >
  inline
  matrix_submatrix<MatrixType const> submatrix( MatrixType const &data, 
                                                RowsForwardIterator rowsBegin, RowsForwardIterator rowsEnd,
                                                ColsForwardIterator colsBegin, ColsForwardIterator colsEnd )
  {
    return matrix_submatrix<MatrixType const>(data, rowsBegin, rowsEnd, colsBegin, colsEnd);
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_SUBMATRIX_HPP

/*
 * iterator.hpp
 * Matrices and vectors iterators.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 24.02.2009
 */

#ifndef NUMERIC_ITERATOR_HPP
#define NUMERIC_ITERATOR_HPP

#include <iterator>

#include "numeric_common.hpp"

namespace numeric
{
  template< class MatrixType >
  class matrix_rows_iterator
    : public std::iterator<std::bidirectional_iterator_tag, // iterator_category
                           ublas::matrix_row<MatrixType>,   // value_type
                           std::ptrdiff_t,                  // difference_type
                           ublas::matrix_row<MatrixType> *, // pointer
                           ublas::matrix_row<MatrixType>    // reference
                           >
  {
  public:
    typedef MatrixType                        matrix_type;
    typedef matrix_rows_iterator              self_type;
    
  protected:
    typedef ublas::matrix_row<matrix_type>    matrix_row_type;
    
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
    
  public:
    typedef typename self_type::reference       reference;
    typedef typename self_type::pointer         pointer;
    typedef typename self_type::difference_type difference_type;
    
  public:
    matrix_rows_iterator()
      : m_  (0)
      , row_(-1)
    {}
    
    matrix_rows_iterator( self_type const &it )
      : m_  (it.m_)
      , row_(it.row_)
    {}
  
    explicit matrix_rows_iterator( matrix_type &m, size_t row = 0 )
      : m_  (&m)
      , row_(row)
    {}
    
  public:
    matrix_type const & matrix() const
    {
      return *m_;
    }
    
    long row() const
    {
      return row_;
    }
    
    bool valid() const
    {
      return (m_ != 0 && row_ >= 0 && row_ < m_->size2());
    }
  
    // Iterator operations.  
  public:
    reference operator*() const
    {
      BOOST_ASSERT(valid());
      return ublas::row(*m_, row_);
    }
    
    pointer operator->() const
    {
      return &(operator*());
    }
    
    // TODO: Add checks for moving far after end or far before begin of matrix.
    self_type & operator++()
    {
      ++row_;
      return *this;
    }
    
    self_type   operator++( int )
    {
      self_type tmp(*this);
      ++row_;
      return tmp;
    }
    
    self_type & operator--()
    {
      --row_;
      return *this;
    }
    
    self_type   operator--( int )
    {
      self_type tmp(*this);
      --row_;
      return tmp;
    }
    
    self_type & operator+=( difference_type n )
    {
      row_ += n;
      return *this;
    }
    
    self_type   operator+ ( difference_type n ) const
    {
      return self_type(*this) += n;
    }
    
    self_type & operator-=( difference_type n )
    {
      row_ -= n;
      return *this;
    }
    
    self_type   operator- ( difference_type n ) const
    {
      return self_type(*this) -= n;
    }
     
  private:
    matrix_type *m_;
    size_t       row_;
  };
  
  template< class MatrixType >
  bool operator==( matrix_rows_iterator<MatrixType> const &x, 
                   matrix_rows_iterator<MatrixType> const &y )
  {
    return (&x.matrix() == &y.matrix() &&
             x.row() == y.row());
  }
   
  template< class MatrixType >
  bool operator!=( matrix_rows_iterator<MatrixType> const &x, 
                   matrix_rows_iterator<MatrixType> const &y )
  {
    return !(x == y);
  }

  template< class MatrixType >
  bool operator<( matrix_rows_iterator<MatrixType> const &x, 
                  matrix_rows_iterator<MatrixType> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() < y.row();
  }
  
  template< class MatrixType >
  bool operator>( matrix_rows_iterator<MatrixType> const &x, 
                  matrix_rows_iterator<MatrixType> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() > y.row();
  }
  
  template< class MatrixType >
  bool operator<=( matrix_rows_iterator<MatrixType> const &x, 
                   matrix_rows_iterator<MatrixType> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() <= y.row();
  }
  
  template< class MatrixType >
  bool operator>=( matrix_rows_iterator<MatrixType> const &x, 
                   matrix_rows_iterator<MatrixType> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() >= y.row();
  }
  
  template< class MatrixType >
  typename matrix_rows_iterator<MatrixType>::difference_type 
    operator-(
      matrix_rows_iterator<MatrixType> const &x, 
      matrix_rows_iterator<MatrixType> const &y )
  {
    // TODO: Assertion with check on end() == end().
    return y.row() - x.row();
  }
  
  template< class MatrixType >
  matrix_rows_iterator<MatrixType>
    operator+(
      typename matrix_rows_iterator<MatrixType>::difference_type n,
      matrix_rows_iterator<MatrixType>                    const &x )
  {
    return x + n;
  }

  BOOST_CONCEPT_ASSERT((boost::BidirectionalIterator< matrix_rows_iterator<ublas::matrix<double> > >));
    
  template< class MatrixType >
  inline matrix_rows_iterator<MatrixType> matrix_rows_begin( MatrixType &m )
  {
    return matrix_rows_iterator<MatrixType>(m);
  }
    
  template< class MatrixType >
  inline matrix_rows_iterator<MatrixType> matrix_rows_end( MatrixType &m )
  {
    return matrix_rows_iterator<MatrixType>(m, m.size1());
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_ITERATOR_HPP

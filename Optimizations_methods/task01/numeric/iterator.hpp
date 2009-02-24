/*
 * iterator.hpp
 * Matrices and vectors iterators.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 24.02.2009
 */

#ifndef NUMERIC_ITERATOR_HPP
#define NUMERIC_ITERATOR_HPP

#include "numeric_common.hpp"

namespace numeric
{
  template< class MatrixType >
  class matrix_rows_iterator
    : public std::iterator<std::forward_iterator_tag, ublas::matrix_row<MatrixType> >
  {
  public:
    typedef MatrixType                     matrix_type;
    typedef ublas::matrix_row<matrix_type> matrix_row_type;
    
    typedef matrix_rows_iterator<matrix_type> self_type;
    
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
  
  public:
    matrix_rows_iterator( matrix_type &m, size_t row = 0 )
      : m_  (&m)
      , row_(row)
    {}
    
    matrix_rows_iterator( self_type const &it )
      : m_  (it.m_)
      , row_(it.row_)
    {}
    
    matrix_row_type operator * ()
    {
      BOOST_ASSERT(row_ < m_->size2());
      return ublas::row(*m_, row_);
    }
    
    self_type & operator ++ ()
    {
      ++row_;
      return *this;
    }
    
    self_type operator ++ ( int )
    {
      self_type tmp(*this);
      ++row_;
      return tmp;
    }
     
    bool operator == ( self_type const &it ) const
    {
      return (m_ == it.m_ && row_ == it.row_);
    }
     
    bool operator != ( self_type const &it ) const
    {
      return !operator == (it);
    }
     
  private:
    matrix_type *m_;
    size_t      row_;
  };
  
  // TODO: operator * () returns matrix_row object, not reference.
  //BOOST_CONCEPT_ASSERT((boost::ForwardIterator< matrix_rows_iterator<ublas::matrix<double> > >));
    
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

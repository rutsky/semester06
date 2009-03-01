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
  template< class Matrix, class Row >
  struct matrix_rows_iterator_traits
  {
    typedef Matrix matrix_type;
    typedef Row    matrix_row_type;
  };
  
  template< class MatrixRowsIteratorTraits, class Derived >
  class matrix_rows_iterator_impl
    : public std::iterator<std::bidirectional_iterator_tag,                      // iterator_category
                           typename MatrixRowsIteratorTraits::matrix_row_type,   // value_type
                           std::ptrdiff_t,                                       // difference_type
                           typename MatrixRowsIteratorTraits::matrix_row_type *, // pointer
                           typename MatrixRowsIteratorTraits::matrix_row_type    // reference
                           >
  {
  public:
    typedef MatrixRowsIteratorTraits                          matrix_rows_iterator_traits;
    typedef typename matrix_rows_iterator_traits::matrix_type matrix_type;
    typedef matrix_rows_iterator_impl                         self_type;
    typedef Derived                                           derived_type;
    
  protected:
    typedef typename matrix_rows_iterator_traits::matrix_row_type matrix_row_type;
    
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<matrix_type>));
    
  public:
    typedef typename self_type::reference       reference;
    typedef typename self_type::pointer         pointer;
    typedef typename self_type::difference_type difference_type;
    
  public:
    matrix_rows_iterator_impl()
      : m_  (0)
      , row_(-1)
    {}
    
    matrix_rows_iterator_impl( self_type const &it )
      : m_  (it.m_)
      , row_(it.row_)
    {}
  
    explicit matrix_rows_iterator_impl( matrix_type &m, size_t row = 0 )
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
      return (m_ != 0 && row_ < m_->size2());
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
    derived_type & operator++()
    {
      ++row_;
      return static_cast<derived_type &>(*this);
    }
    
    derived_type   operator++( int )
    {
      derived_type tmp(static_cast<derived_type &>(*this));
      ++row_;
      return tmp;
    }
    
    derived_type & operator--()
    {
      --row_;
      return static_cast<derived_type &>(*this);
    }
    
    derived_type   operator--( int )
    {
      derived_type tmp(static_cast<derived_type &>(*this));
      --row_;
      return tmp;
    }
    
    derived_type & operator+=( difference_type n )
    {
      row_ += n;
      return static_cast<derived_type &>(*this);
    }
    
    derived_type   operator+ ( difference_type n ) const
    {
      return static_cast<derived_type>(*this) += n;
    }
    
    derived_type & operator-=( difference_type n )
    {
      row_ -= n;
      return static_cast<derived_type &>(*this);
    }
    
    derived_type   operator- ( difference_type n ) const
    {
      return static_cast<derived_type>(*this) -= n;
    }
     
  private:
    matrix_type *m_;
    size_t       row_;
  };
  
  template< class Matrix >
  class matrix_rows_iterator
    : public matrix_rows_iterator_impl<matrix_rows_iterator_traits<Matrix, ublas::matrix_row<Matrix> >,
                                       matrix_rows_iterator<Matrix> >
  {
  protected:
    typedef matrix_rows_iterator            self_type;
    typedef typename self_type::matrix_type matrix_type;
    typedef matrix_rows_iterator_impl<matrix_rows_iterator_traits<Matrix, ublas::matrix_row<Matrix> >,
                                      matrix_rows_iterator<Matrix> > base_type;
    
  public:
    matrix_rows_iterator()
      : base_type()
    {}
    
    matrix_rows_iterator( self_type const &it )
      : base_type(it)
    {}
  
    explicit matrix_rows_iterator( matrix_type &m, size_t row = 0 )
      : base_type(m, row)
    {}
  };
  
  template< class Matrix >
  class matrix_rows_const_iterator
    : public matrix_rows_iterator_impl<matrix_rows_iterator_traits<Matrix const, ublas::matrix_row<Matrix const> const>,
                                       matrix_rows_const_iterator<Matrix> >
  {
  protected:
    typedef matrix_rows_const_iterator            self_type;
    typedef typename self_type::matrix_type matrix_type;
    typedef matrix_rows_iterator_impl<matrix_rows_iterator_traits<Matrix const, ublas::matrix_row<Matrix const> const>,
                                      matrix_rows_const_iterator<Matrix> > base_type;
    
  public:
    matrix_rows_const_iterator()
      : base_type()
    {}
    
    matrix_rows_const_iterator( self_type const &it )
      : base_type(it)
    {}
  
    explicit matrix_rows_const_iterator( matrix_type const &m, size_t row = 0 )
      : base_type(m, row)
    {}
  };
  
  //
  // Operators
  //
  
  template< class Matrix >
  bool operator==( matrix_rows_iterator<Matrix> const &x, 
                   matrix_rows_iterator<Matrix> const &y )
  {
    return (&x.matrix() == &y.matrix() &&
             x.row() == y.row());
  }
   
  template< class Matrix >
  bool operator!=( matrix_rows_iterator<Matrix> const &x, 
                   matrix_rows_iterator<Matrix> const &y )
  {
    return !(x == y);
  }

  template< class Matrix >
  bool operator<( matrix_rows_iterator<Matrix> const &x, 
                  matrix_rows_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() < y.row();
  }
  
  template< class Matrix >
  bool operator>( matrix_rows_iterator<Matrix> const &x, 
                  matrix_rows_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() > y.row();
  }
  
  template< class Matrix >
  bool operator<=( matrix_rows_iterator<Matrix> const &x, 
                   matrix_rows_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() <= y.row();
  }
  
  template< class Matrix >
  bool operator>=( matrix_rows_iterator<Matrix> const &x, 
                   matrix_rows_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() >= y.row();
  }
  
  template< class Matrix >
  typename matrix_rows_iterator<Matrix>::difference_type 
    operator-(
      matrix_rows_iterator<Matrix> const &x, 
      matrix_rows_iterator<Matrix> const &y )
  {
    // TODO: Assertion with check on end() == end().
    return y.row() - x.row();
  }
  
  template< class Matrix >
  matrix_rows_iterator<Matrix>
    operator+(
      typename matrix_rows_iterator<Matrix>::difference_type n,
      matrix_rows_iterator<Matrix>                    const &x )
  {
    return x + n;
  }

  BOOST_CONCEPT_ASSERT((boost::BidirectionalIterator< matrix_rows_iterator<ublas::matrix<double> > >));

  //
  // Constant interator operators.
  //

  template< class Matrix >
  bool operator==( matrix_rows_const_iterator<Matrix> const &x, 
                   matrix_rows_const_iterator<Matrix> const &y )
  {
    return (&x.matrix() == &y.matrix() &&
             x.row() == y.row());
  }
   
  template< class Matrix >
  bool operator!=( matrix_rows_const_iterator<Matrix> const &x, 
                   matrix_rows_const_iterator<Matrix> const &y )
  {
    return !(x == y);
  }

  template< class Matrix >
  bool operator<( matrix_rows_const_iterator<Matrix> const &x, 
                  matrix_rows_const_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() < y.row();
  }
  
  template< class Matrix >
  bool operator>( matrix_rows_const_iterator<Matrix> const &x, 
                  matrix_rows_const_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() > y.row();
  }
  
  template< class Matrix >
  bool operator<=( matrix_rows_const_iterator<Matrix> const &x, 
                   matrix_rows_const_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() <= y.row();
  }
  
  template< class Matrix >
  bool operator>=( matrix_rows_const_iterator<Matrix> const &x, 
                   matrix_rows_const_iterator<Matrix> const &y )
  {
    BOOST_ASSERT(x.valid());
    BOOST_ASSERT(y.valid());
    return x.row() >= y.row();
  }
  
  template< class Matrix >
  typename matrix_rows_const_iterator<Matrix>::difference_type 
    operator-(
      matrix_rows_const_iterator<Matrix> const &x, 
      matrix_rows_const_iterator<Matrix> const &y )
  {
    // TODO: Assertion with check on end() == end().
    return y.row() - x.row();
  }
  
  template< class Matrix >
  matrix_rows_const_iterator<Matrix>
    operator+(
      typename matrix_rows_const_iterator<Matrix>::difference_type n,
      matrix_rows_const_iterator<Matrix>                    const &x )
  {
    return x + n;
  }

  BOOST_CONCEPT_ASSERT((boost::BidirectionalIterator< matrix_rows_const_iterator<ublas::matrix<double> > >));

  //
  // Initializators.
  // 
    
  template< class Matrix >
  inline matrix_rows_iterator<Matrix> matrix_rows_begin( Matrix &m )
  {
    return matrix_rows_iterator<Matrix>(m);
  }
    
  template< class Matrix >
  inline matrix_rows_iterator<Matrix> matrix_rows_end( Matrix &m )
  {
    return matrix_rows_iterator<Matrix>(m, m.size1());
  }
  
  template< class Matrix >
  inline matrix_rows_const_iterator<Matrix> matrix_rows_begin( Matrix const &m )
  {
    return matrix_rows_const_iterator<Matrix>(m);
  }
    
  template< class Matrix >
  inline matrix_rows_const_iterator<Matrix> matrix_rows_end( Matrix const &m )
  {
    return matrix_rows_const_iterator<Matrix>(m, m.size1());
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_ITERATOR_HPP

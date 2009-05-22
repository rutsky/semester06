/*
 * subvector.hpp
 * Subvector of selected rows class implementation.
 * Based on boost::numeric::ublas::vector_range.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 10.03.2009
 */

#ifndef NUMERIC_SUBVECTOR_HPP
#define NUMERIC_SUBVECTOR_HPP
 
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/detail/vector_assign.hpp>
#include <boost/numeric/ublas/detail/temporary.hpp>

#include "numeric_common.hpp"

namespace numeric
{
  template< class V >
  class vector_subvector
    : public ublas::vector_expression<vector_subvector<V> >
  {
  private:
    typedef vector_subvector<V> self_type;
    
  public:
    typedef V                     const const_vector_type;
    typedef V                           vector_type;
    typedef typename V::size_type       size_type;
    typedef typename V::difference_type difference_type;
    typedef typename V::value_type      value_type;
    typedef typename V::const_reference const_reference;
    typedef typename V::pointer         pointer;
    typedef typename boost::mpl::if_<boost::is_const<V>,
                                      typename V::const_reference,
                                      typename V::reference>::type reference;
    typedef typename boost::mpl::if_<boost::is_const<V>,
                                      typename V::const_closure_type,
                                      typename V::closure_type>::type vector_closure_type;
    typedef ublas::basic_range<size_type, difference_type> range_type;
    typedef self_type const             const_closure_type;
    typedef self_type                   closure_type;
    typedef typename ublas::storage_restrict_traits<typename V::storage_category,
                                                    ublas::dense_proxy_tag>::storage_category storage_category;

    // Construction and destruction.
    template< class RowsForwardIterator >
    vector_subvector( vector_type &data, 
                      RowsForwardIterator rowsBegin, RowsForwardIterator rowsEnd )
      : data_       (data)
    {
      rowsIndices_.assign(rowsBegin, rowsEnd);
      
      // debug
      // TODO: Rewrite so that in release mode it will not produce overheads.
      for (size_t r = 0; r < rowsIndices_.size(); ++r)
        BOOST_ASSERT(rowsIndices_[r] < data_.size());
      // end of debug
    }

    // Accessors.
    size_type size() const 
    {
      return rowsIndices_.size();
    }

    // Storage accessors.
    vector_closure_type const & data() const
    {
      return data_;
    }
    vector_closure_type       & data () 
    {
      return data_;
    }

    // Element access.
    const_reference operator () ( size_type r ) const
    {
      BOOST_ASSERT(r < rowsIndices_.size());
      return data_(rowsIndices_[r]);
    }
    reference       operator () ( size_type r ) 
    {
      BOOST_ASSERT(r < rowsIndices_.size());
      return data_(rowsIndices_[r]);
    }

    const_reference operator [] ( size_type r ) const
    {
      return (*this)(r);
    }
    reference       operator [] ( size_type r ) 
    {
      return (*this)(r);
    }

    // Assignment.
    vector_subvector & operator = ( vector_subvector const &sv )
    {
      // ISSUE need a temporary, proxy can be overlaping alias.
      ublas::vector_assign<ublas::scalar_assign>(*this, typename ublas::vector_temporary_traits<V>::type(sv));
      return *this;
    }
    vector_subvector & assign_temporary( vector_subvector &sv )
    {
      // assign elements, proxied container remains the same
      ublas::vector_assign<ublas::scalar_assign> (*this, sv);
      return *this;
    }
    template< class AE >
    vector_subvector & operator = ( ublas::vector_expression<AE> const &ae ) 
    {
      ublas::vector_assign<ublas::scalar_assign>(*this, typename ublas::vector_temporary_traits<V>::type(ae));
      return *this;
    }
    template< class AE >
    vector_subvector & assign ( ublas::vector_expression<AE> const &ae )
    {
      ublas::vector_assign<ublas::scalar_assign>(*this, ae);
      return *this;
    }
    template< class AE >
    vector_subvector & operator += ( ublas::vector_expression<AE> const &ae ) 
    {
      ublas::vector_assign<ublas::scalar_assign>(*this, typename ublas::vector_temporary_traits<V>::type(*this + ae));
      return *this;
    }
    template< class AE >
    vector_subvector & plus_assign ( ublas::vector_expression<AE> const &ae ) 
    {
      ublas::vector_assign<ublas::scalar_plus_assign>(*this, ae);
      return *this;
    }
    template< class AE >
    vector_subvector & operator -= ( ublas::vector_expression<AE> const &ae ) 
    {
      ublas::vector_assign<ublas::scalar_assign>(*this, typename ublas::vector_temporary_traits<V>::type(*this - ae));
      return *this;
    }
    template< class AE >
    vector_subvector &minus_assign ( ublas::vector_expression<AE> const &ae ) 
    {
      ublas::vector_assign<ublas::scalar_minus_assign>(*this, ae);
      return *this;
    }
    template< class AT >
    vector_subvector & operator *= ( AT const &at ) 
    {
      ublas::vector_assign_scalar<ublas::scalar_multiplies_assign>(*this, at);
      return *this;
    }
    template< class AT >
    vector_subvector & operator /= ( AT const &at ) 
    {
      ublas::vector_assign_scalar<ublas::scalar_divides_assign>(*this, at);
      return *this;
    }

    // Closure comparison.
    bool same_closure( vector_subvector const &sv ) const
    {
      return (*this).data_.same_closure(sv.data_);
    }

    // Comparison.
    bool operator == ( vector_subvector const &sv ) const 
    {
      return ((*this).data_ == sv.data_) && (rowsIndices_ == sv.rowsIndices_);
    }

    // Swapping.
    void swap( vector_subvector sv ) 
    {
      if (this != &sv)
      {
        BOOST_UBLAS_CHECK(size() == sv.size(), ublas::bad_size());
        // Sparse ranges may be nonconformant now.
        // std::swap_ranges (begin (), end (), sv.begin ());
        ublas::vector_swap<ublas::scalar_swap>(*this, sv);
      }
    }
    friend void swap( vector_subvector sv1, vector_subvector sv2 )
    {
      sv1.swap(sv2);
    }

    // Iterator types.
/*
  private:
    typedef typename V::const_iterator const_subiterator_type;
    typedef typename boost::mpl::if_<boost::is_const<V>,
                                      typename V::const_iterator,
                                      typename V::iterator>::type subiterator_type;
 */
                                      
  public:
    class const_iterator;
    class iterator;

    // Element lookup.
    const_iterator find( size_type r ) const 
    {
      return const_iterator(*this, r);
    }
    iterator       find( size_type r )
    {
      return iterator      (*this, r);
    }

    class const_iterator
      : public ublas::container_const_reference<vector_subvector>
      , public ublas::iterator_base_traits<std::random_access_iterator_tag>::template
                 iterator_base<const_iterator, value_type>::type
    {
    public:
      typedef typename vector_subvector::difference_type difference_type;
      typedef typename vector_subvector::value_type      value_type;
      typedef typename vector_subvector::reference       reference;
      typedef typename vector_subvector::pointer         pointer;

      // Construction and destruction.
      const_iterator()
        : ublas::container_const_reference<self_type>()
        , r_() 
      {
      }
      const_iterator( self_type const &sv, size_type r )
        : ublas::container_const_reference<self_type>(sv)
        , r_(r) 
      {
      }
      const_iterator( typename self_type::iterator const  &it )
        : ublas::container_const_reference<self_type>(it())
        , r_(it.r_) 
      {
      }

      // Arithmetic.
      const_iterator & operator ++ () 
      {
        ++r_;
        return *this;
      }
      const_iterator & operator -- () 
      {
        --r_;
        return *this;
      }
      const_iterator & operator += ( difference_type n ) 
      {
        r_ += n;
        return *this;
      }
      const_iterator & operator -= ( difference_type n )
      {
        r_ -= n;
        return *this;
      }
      difference_type operator - ( const_iterator const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        return r_ - it.r_;
      }

      // Dereference.
      const_reference operator * () const 
      {
        return (*this)()(r_);
      }
      const_reference operator [] ( difference_type n ) const 
      {
        return *(*this + n);
      }

      // Index.
      size_type index() const 
      {
        return r_;
      }

      // Assignment
      const_iterator & operator = ( const_iterator const &it )
      {
        ublas::container_const_reference<self_type>::assign(&it());
        r_ = it.r_;
        return *this;
      }

      // Comparison.
      bool operator == ( const_iterator const &it ) const
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        return r_ == it.r_;
      }
      bool operator < ( const_iterator const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        return r_ < it.r_;
      }

    private:
      size_type r_;
    };

    const_iterator begin() const 
    {
      return find(0);
    }
    const_iterator end  () const 
    {
      return find(size());
    }

    class iterator
      : public ublas::container_reference<vector_subvector>
      , public ublas::iterator_base_traits<std::random_access_iterator_tag>::template
                 iterator_base<iterator, value_type>::type 
    {
  public:
      typedef typename vector_subvector::difference_type difference_type;
      typedef typename vector_subvector::value_type      value_type;
      typedef typename vector_subvector::reference       reference;
      typedef typename vector_subvector::pointer         pointer;

      // Construction and destruction.
      iterator()
        : ublas::container_reference<self_type>()
        , r_()
      {
      }
      iterator( self_type &sv, size_type r )
        : ublas::container_reference<self_type>(sv)
        , r_ (r)
      {
      }

      // Arithmetic.
      iterator & operator ++ () 
      {
        ++r_;
        return *this;
      }
      iterator & operator -- () 
      {
        --r_;
        return *this;
      }
      iterator & operator += ( difference_type n ) 
      {
        r_ += n;
        return *this;
      }
      iterator & operator -= ( difference_type n ) 
      {
        r_ -= n;
        return *this;
      }
      difference_type operator - ( iterator const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        return r_ - it.r_;
      }

      // Dereference.
      reference operator * () const 
      {
        return (*this)()(r_);
      }
      reference operator [] ( difference_type n ) const 
      {
        return *(*this + n);
      }

      // Index.
      size_type index() const 
      {
        return r_;
      }

      // Assignment.
      iterator & operator = ( iterator const &it ) 
      {
        ublas::container_reference<self_type>::assign(&it());
        r_ = it.r_;
        return *this;
      }

      // Comparison.
      bool operator == ( iterator const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        return r_ == it.r_;
      }
      bool operator < ( iterator const &it ) const 
      {
        BOOST_UBLAS_CHECK((*this)().same_closure(it()), ublas::external_logic());
        return r_ < it.r_;
      }

    private:
      size_type r_;

      friend class const_iterator;
    };

    iterator begin() 
    {
      return find(0);
    }
    iterator end  ()
    {
      return find(size());
    }

    // Reverse iterator.
    typedef ublas::reverse_iterator_base<const_iterator> const_reverse_iterator;
    typedef ublas::reverse_iterator_base<iterator>       reverse_iterator;

    const_reverse_iterator rbegin() const 
    {
      return const_reverse_iterator(end());
    }
    const_reverse_iterator rend  () const 
    {
      return const_reverse_iterator(begin());
    }
    reverse_iterator       rbegin() 
    {
      return reverse_iterator (end ());
    }
    reverse_iterator       rend  () 
    {
      return reverse_iterator(begin());
    }

  private:
    vector_closure_type data_;
    std::vector<size_t> rowsIndices_;
  };

  // Generic projections.
  template< class VectorType, class RowsForwardIterator >
  inline
  vector_subvector<VectorType>             subvector( VectorType &data, RowsForwardIterator first, RowsForwardIterator beyond )
  {
    return vector_subvector<VectorType>(data, first, beyond);
  }
  template< class VectorType, class RowsForwardIterator >
  inline
  vector_subvector<VectorType const> const subvector( VectorType const &data, RowsForwardIterator first, RowsForwardIterator beyond )
  {
    return vector_subvector<VectorType const>(data, first, beyond);
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_SUBVECTOR_HPP

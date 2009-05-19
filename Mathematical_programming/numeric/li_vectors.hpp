/*
 * li_vectors.hpp
 * Linear independent vectors stuff.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_LI_VECTORS_HPP
#define NUMERIC_LI_VECTORS_HPP

#include <vector>

#include <boost/optional.hpp>

#include "numeric_common.hpp"

namespace numeric
{
  // TODO: Check is inserting of zero vectors is handled.
  template< class VectorType >
  class linear_independent_vectors
  {
  public:
    typedef VectorType                       vector_type;
    typedef linear_independent_vectors       self_type;
    
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<vector_type>));
    
  private:
    typedef typename vector_type::value_type value_type;
    typedef ublas::scalar_traits<value_type> scalar_traits;
    
  public:
    bool empty() const
    {
      return liVectors_.empty();
    }
    
    size_t size() const 
    {
      return liVectors_.size();
    }
    
    void swap( self_type &x )
    {
      liVectors_.swap(x.liVectors_);
      unitPos_  .swap(x.unitPos_);
    }
    
    bool is_independent( vector_type const &v ) const
    {
      vector_type tmp(v);
      return eliminateVector(tmp);
    }
    
    bool insert( vector_type const &vOriginal )
    {
      BOOST_ASSERT(vOriginal.size() != 0);
      //std::cout << "Eliminating: " << vOriginal << "\n"; // debug
      
      vector_type v = vOriginal;
      if (eliminateVector(v))
      {
        // Searching first nonzero row in eliminated vector.
        size_t r(0);
        for (; r < v.size(); ++r)
        {
          if (!scalar_traits::equals(v[r], 0))
          {
            break;
          }
        }
        
        BOOST_ASSERT(r < v.size()); // because eliminateVector() returned true
        BOOST_ASSERT(scalar_traits::equals(v[r], 1));
        
        if (!liVectors_.empty())
        {
          // Eliminating founded row in vectors in storage.
          for (size_t i = 0; i < liVectors_.size(); ++i)
          {
            vector_type &w = liVectors_[i];
            
            if (!scalar_traits::equals(w[r], 0))
            {
              // Eliminating row.
              w = w - w[r] * v;
              BOOST_ASSERT(scalar_traits::equals(w[r], 0));
              w[r] = 0; // rounding
            }
            else
            {
              // Row of vector in storage is already eliminated.
            }
          }
          
          // Adding normalized vector to storage.
          liVectors_.push_back(v);
          BOOST_ASSERT(!unitPos_[r]);
          unitPos_[r] = liVectors_.size() - 1;
          
          return true;
        }
        else
        {
          // Storage is empty, adding normalized vector.
          liVectors_.push_back(v);
          unitPos_.resize(v.size());
          unitPos_[r] = 0;
          
          return true;
        }
      }
      else
      {
        // Vector is not linear independent with vectors in storage.
        return false;
      }
    }
    
    template< class InputIterator >
    bool insert( InputIterator first, InputIterator beyond )
    {
      BOOST_CONCEPT_ASSERT((boost::InputIterator<InputIterator>));
      BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<typename InputIterator::value_type>));
      
      typedef typename InputIterator::value_type::value_type value_type;
      typedef typename ublas::vector<value_type> vector_type;
      
      linear_independent_vectors<vector_type> liVectors(*this);
      
      bool success = true;
      while (first != beyond && success)
      {
        if (!liVectors.insert(*first))
          success = false;
        
        ++first;
      }
      
      if (success)
      {
        liVectors.swap(*this);
        return true;
      }
      else
        return false;
    }
    
  private:
    // Returns true if nonzero row was found.
    bool normalizeFirstNonZeroRow( vector_type &v ) const
    {
      // TODO: Rewrite using std::find and boost::bind.
      for (size_t r = 0; r < v.size(); ++r)
      {
        if (!scalar_traits::equals(v[r], 0))
        {
          // Found nonzero row, normalizing it and return success code.
          v = v / v[r];
          BOOST_ASSERT(scalar_traits::equals(v[r], 1));
          v[r] = 1; // rounding
          return true;
        }
      }
      
      // Nonzero row is not found.
      return false;
    }
  
    // Returns true, if provided vector is linear independent with vectors in storage.
    bool eliminateVector( vector_type &v ) const
    {
      BOOST_ASSERT(v.size() != 0);
      //std::cout << "Eliminating: " << v << "\n"; // debug
      //dumpStorage(); // debug
      
      if (!liVectors_.empty())
      {
        // Eliminating vector's rows.
        BOOST_ASSERT(liVectors_[0].size() == v.size());
        
        // TODO: Rewrite using std::find and boost::bind.
        for (size_t r = 0; r < v.size(); ++r)
        {
          if (unitPos_[r])
          {
            // Eliminating row with normalized vector from storage.
            BOOST_ASSERT(liVectors_[unitPos_[r].get()][r] == 1);
            v = v - v[r] * liVectors_[unitPos_[r].get()];
            BOOST_ASSERT(scalar_traits::equals(v[r], 0));
            v[r] = 0; // rounding
          }
          else
          {
            // Current row is zero or not normalized in storage's vectors. Skipping.
          }
        }
        
        // All vector's rows that can be eliminated with vectors from storage are eliminated,
        // so searching first nonzero row in left vector and normalizing it.
        return normalizeFirstNonZeroRow(v);
      }
      else
      {
        // Storage don't have any vectors, so just try to normalize first nonzero row.
        return normalizeFirstNonZeroRow(v);
      }
    }
    
    // debug
  public:
    void dumpStorage() const
    {
      for (size_t i = 0; i < liVectors_.size(); ++i)
      {
        std::cout << liVectors_[i];
        if (unitPos_[i])
          std::cout << " " << unitPos_[i].get();
        std::cout << "\n";
      }
    }
    
  private:
    // end of debug
    
  private:
    std::vector<vector_type>              liVectors_;
    std::vector<boost::optional<size_t> > unitPos_;
  };
  
  template< class VectorsForwardIterator >
  inline bool is_linear_independent( VectorsForwardIterator first, VectorsForwardIterator beyond )
  {
    BOOST_CONCEPT_ASSERT((boost::ForwardIterator<VectorsForwardIterator>));
    
    typedef typename VectorsForwardIterator::value_type::value_type value_type;
    typedef typename ublas::vector<value_type>                      vector_type;
    
    linear_independent_vectors<vector_type> liVectors;
    
    return liVectors.insert(first, beyond);
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_LI_VECTORS_HPP

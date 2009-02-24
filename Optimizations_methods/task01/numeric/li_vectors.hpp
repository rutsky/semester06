/*
 * li_vectors.hpp
 * Linear independent vectors stuff.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_LI_VECTORS_HPP
#define NUMERIC_LI_VECTORS_HPP

#include <vector>

#include "numeric_common.hpp"

namespace numeric
{
  template< class VectorType >
  class linear_independent_vectors
  {
  public:
    typedef VectorType vector_type;
    
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
    
    bool is_independent( vector_type const &v ) const
    {
      return false; // TODO
    }
    
    bool insert( vector_type const &v )
    {
      vector_type eliminatedV(v);
      if (!eliminateVector(eliminatedV))
      {
        // Vector with vectors in storage are linear dependent.
        return false;
      }
      else
      {
        // Vectors most rows were eliminated, adding to storage.
        liVectors_.insert(liVectors_.end(), eliminatedV);
        //std::cout << "Adding: " << eliminatedV << " (" << v << ")\n"; // debug
        return true;
      }
    }
    
    template< class InputIterator >
    bool insert( InputIterator first, InputIterator beyond )
    {
      BOOST_CONCEPT_ASSERT((boost::InputIterator<InputIterator>));
      BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<typename InputIterator::value_type>));
      
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
        liVectors.swap(*this); // TODO: this method is not implemented.
        return true;
      }
      else
        return false;
    }
    
  private:
    bool eliminateVector( vector_type &v ) const
    {
      BOOST_ASSERT(!v.empty());
      
      // Searching first non zero row in vector.
      size_t r = 0;
      while (r < v.size() && scalar_traits::equals(v[r], 0))
      {
        ++r;
      }
      
      if (r >= v.size())
      {
        // Vector is zero.
        BOOST_ASSERT(scalar_traits::equals(ublas::norm_inf(v), 0));
        return false;
      }
      
      // Normalizing first nonzero row.
      v /= v[r];
      BOOST_ASSERT(scalar_traits::equals(v[r], 1));
      v[r] = 1;
      
      if (empty())
      {
        // All possible vector rows eliminated, because there is no other vector to eliminate left nonzero rows.
        return true;
      }
      
      // Eliminating vectors rows.
      BOOST_ASSERT(liVectors_[0].size() == v.size());
      bool notEliminatableRowFound(false);
      for (; r < v.size(); ++r)
      {
        if (scalar_traits::equals(v[r], 0))
        {
          // Row already eliminated.
          v[r] = 0;
          continue;
        }
        
        bool eliminated(false);
        for (size_t i = 0; i < liVectors_.size(); ++i)
        {
          if (liVectors_[i][r] != 0)
          {
            BOOST_ASSERT(liVectors_[i][r] == 1);
            v -= liVectors_[i] * v[r];
            BOOST_ASSERT(scalar_traits::equals(v[r], 0));
            v[r] = 0;
            eliminated = true;
            break;
          }
        }
        
        if (!eliminated)
        {
          // Found not eliminatable row in vector.
          notEliminatableRowFound = true;
          break;
        }
      }
      
      if (notEliminatableRowFound)
      {
        // Storage vectors with given vector are linear independent.
        return true;
      }
      else
      {
        // Vector is linear combination of storage vectors.
        //std::cout << v << "\n"; // debug
        BOOST_ASSERT(scalar_traits::equals(ublas::norm_inf(v), 0));
        return false;
      }
    }
    
  private:
    std::vector<vector_type> liVectors_;
  };
  
  template< class VectorsForwardIterator >
  bool is_linear_independent( VectorsForwardIterator first, VectorsForwardIterator beyond )
  {
    BOOST_CONCEPT_ASSERT((boost::ForwardIterator<VectorsForwardIterator>));
    
    typedef typename VectorsForwardIterator::value_type vector_type;
    
    linear_independent_vectors<vector_type> liVectors;
    
    return liVectors.insert(first, beyond);
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_LI_VECTORS_HPP

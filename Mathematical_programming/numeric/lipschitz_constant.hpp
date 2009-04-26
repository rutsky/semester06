/*
 * lipschitz_constant.hpp
 * Numerically calculating Lipschitz constant for function.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 12.04.2009
 */

#ifndef NUMERIC_LIPSCHITZ_CONSTANT_HPP
#define NUMERIC_LIPSCHITZ_CONSTANT_HPP

#include "numeric_common.hpp"

#include <algorithm>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/bind.hpp>

namespace numeric
{
  // Calculating Lipschitz constant R defined by:
  //   || f(x) - f(y) || <= R || x - y || 
  template< class Func, class FuncResult, class FuncNorm, class V, class VNorm >
  inline
  typename V::value_type 
    lipschitz_constant( Func function, FuncNorm functionNorm, 
                        V const &x, V const &y, 
                        VNorm vNorm,
                        V const &step )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));

    typedef FuncResult                        function_result_type;
    typedef typename V::value_type            scalar_type;
    typedef vector<scalar_type>               vector_type;

    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<Func,     function_result_type, vector_type>));
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<FuncNorm, scalar_type,          function_result_type>));
    
    // Assert that for each `i' x[i] <= y[i].
    BOOST_ASSERT(std::find_if((y - x).begin(), (y - x).end(), boost::bind(std::less<scalar_type>(), _1, 0)) == (y - x).end());
    // Assert that step is positive. TODO: Implement negative step.
    BOOST_ASSERT(std::find_if(step.begin(), step.end(), boost::bind(std::less<scalar_type>(), _1, 0)) == step.end());
    // TODO: Assert that at least one step of vector is possible on each axis.
    // Assert vectors sizes.
    BOOST_ASSERT(x.size() == y.size());
    BOOST_ASSERT(x.size() == step.size());
    
    size_t const n = x.size();
    
    // Lipschitz constant projected by axes.
    vector_type R(n);
    
    // Initializing with zeroes.
    std::fill(R.begin(), R.end(), 0);
    
    vector_type v = x;
    while (true)
    {
      bool increased(false);
      for (size_t idx = 0; idx < n; ++idx)
      {
        if ((v(idx) += step(idx)) < y(idx))
          increased = true;
        else
          v(idx) = x(idx);
      }
      
      if (increased)
      {
        // Updating `R'.
        
        for (size_t dirIdx = 0; dirIdx < n; ++dirIdx)
        {
          vector_type dir(n);
          std::fill(dir.begin(), dir.end(), 0);
          dir(dirIdx) = step(dirIdx);
          
          function_result_type const funcDiff = function(v + dir) - function(v);
          scalar_type const funcDiffNorm = functionNorm(funcDiff);
          scalar_type const argDiffNorm  = vNorm(dir);
          
          BOOST_ASSERT(funcDiffNorm >= 0);
          BOOST_ASSERT(argDiffNorm  >  0 && !ublas::scalar_traits<scalar_type>::equals(argDiffNorm, 0));
          
          make_max(R(dirIdx), funcDiffNorm / argDiffNorm);
        }
      }
      else
      {
        // Passed all points.
        break;
      }
    }
    
    scalar_type const resultR = ublas::norm_2(R); // FIXME: I think this is incorrect.
    return resultR;
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_LIPSCHITZ_CONSTANT_HPP

/*
 * combination.hpp
 * Working with combinations.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 16.04.2008
 */

#ifndef COMBINATION_HPP
#define COMBINATION_HPP

#include <cassert>

// "Discrete Analysis", Joseph V. Romanovsky, 3rd edition, Saint-Petersburg 2004.
// 2.5 "Allocations and combinations".

namespace combination
{
  // Returns number of k-combinations from a set of n elements.
  // Rough asymptotic: O(k).
  template< class ScalarType >
  ScalarType combinations( ScalarType n, ScalarType k )
  {
    if (k < 0 || k > n)
      return 0;
    
    ScalarType numerator(1), denumerator(1);
    for (ScalarType i = 1; i <= k; ++i)
    {
      numerator   *= (n - i + 1);
      denumerator *= i;
    }
    
    return numerator / denumerator;
  }
  
  // Generates first combination in lexigraphicall order.
  template< class ScalarType, class OutIter > 
  OutIter first_combination( OutIter out, ScalarType k )
  {
    for (size_t i = 0; i < k; ++i)
      *out++ = i;
    
    return out;
  }
  
  // Tries to generate next lexigraphically greater combination.
  // Indexes are unique k numbers from 0 to n - 1.
  // Rough asymptotic: O(n).
  template< class ScalarType, class RndIter >
  bool next_combination( RndIter indexes, ScalarType n, ScalarType k )
  {
    // TODO: assert(n >= 0 && k >= 0);
    
    if (k == 0)
    {
      // None set of indexes has no next combination.
      return false;
    }
    assert(k <= n);
    
    // TODO: BiDi iterators
    
    ScalarType i = k - 1;
    while (i > 0 && indexes[i] >= n - k + i)
      --i;
    
    if (indexes[i] >= n - k + i)
      return false; // TODO: Should fill indexes with first combination like std::next_permutation does.
    
    ++indexes[i];
    for (ScalarType j = i + 1; j < k; ++j)
      indexes[j] = indexes[i] + (j - i);
    
    return true;
  }
  
  // TODO: Implement prev_combination() function
  
  // Returns number of combination in lexicographic order (thats also means that indexes are sorted).
  // Combination number is from 0 to C(n, k) - 1.
  // Indexes are unique k numbers from 0 to n - 1.
  // Rough asymptotic: O(n * k).
  template< class ScalarType, class InpIter >
  ScalarType number( InpIter indexes, ScalarType n, ScalarType k )
  {
    assert(n > 0 && k > 0 && k <= n);
    
    ScalarType num(0);
    ScalarType lastIndex = *indexes++;
    ScalarType indexCount(0);
    for (ScalarType i = 0; i < n; ++i)
    {
      if (lastIndex == i)
      {
        if (indexCount == k - 1)
          break;
        
        ScalarType newLastIndex = *indexes++;
        assert(newLastIndex > lastIndex);
        lastIndex = newLastIndex;
        ++indexCount;
      }
      else
        num += combinations(n - i - 1, k - indexCount - 1);
    }
    
    return num;
  }
  
  // Finds combination by its number in lexicographic order.
  // Combination number is from 0 to C(n, k) - 1.
  // Indexes will be unique k numbers from 0 to n - 1.
  // Rough asymptotic: O(n * k).
  template< class ScalarType, class OutIter >
  void combination( ScalarType n, ScalarType k, ScalarType num, OutIter indexesOut )
  {
    assert(num >= 0 && num < combinations(n, k));
    
    ScalarType oldN(n);
    for (ScalarType i = 0; i < oldN && k > 0; ++i)
    {
      ScalarType c = combinations(n - 1, k - 1);
      if (num < c)
      {
        *indexesOut++ = i;
        --n;
        --k;
      }
      else
      {
        num -= c;
        --n;
      }
    }
  }
} // End of 'combination' namespace

#endif // COMBINATION_HPP

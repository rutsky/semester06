/*
 * matrix_ops.hpp
 * Operations with matrices.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 21.05.2009
 */

#ifndef NUMERIC_MATRIX_OPS_HPP
#define NUMERIC_MATRIX_OPS_HPP
 
#include "numeric_common.hpp"

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

#include "submatrix.hpp"

namespace numeric
{
  // TODO: Maybe vector extending operations?

  template< class MatrixType, class ColsForwardIterator >
  inline
  matrix_submatrix<MatrixType> submatrixi( MatrixType &data, 
                                           size_t              rowsStart, size_t              rowsSize,
                                           ColsForwardIterator colsBegin, ColsForwardIterator colsEnd )
  {
    basic_range<size_t, long> range(rowsStart, rowsSize);
    return matrix_submatrix<MatrixType>(data, range.begin(), range.end(), colsBegin, colsEnd);
  }
  
  template< class MatrixType, class RowsForwardIterator >
  inline
  matrix_submatrix<MatrixType const> submatrixi( MatrixType const &data, 
                                                 RowsForwardIterator rowsBegin, RowsForwardIterator rowsEnd,
                                                 size_t              colsStart, size_t              colsSize )
  {
    basic_range<size_t, long> range(colsStart, colsSize);
    return matrix_submatrix<MatrixType const>(data, rowsBegin, rowsEnd, range.begin(), range.end());
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_MATRIX_OPS_HPP

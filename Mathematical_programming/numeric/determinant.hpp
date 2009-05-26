/*
 * determinant.hpp
 * Finding matrix determinant functions.
 * Based on http://lists.boost.org/MailArchives/ublas/2005/12/0916.php // TODO!
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 26.05.2009
 */

#ifndef NUMERIC_DETERMINANT_HPP
#define NUMERIC_DETERMINANT_HPP

#include "numeric_common.hpp"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace numeric
{
  template< class M >
  typename M::value_type matrix_determinant( matrix_expression<M> const &m )
  {
    typedef typename M::value_type scalar_type;
    typedef matrix<scalar_type>    matrix_type;
    
    ASSERT_EQ(m().size1(), m().size2());

    // create a working copy of the input
    matrix_type mLu(m());
    ublas::permutation_matrix<size_t> pivots(m().size1());

    lu_factorize(mLu, pivots);

    scalar_type det(1.0);

    for (size_t i = 0; i < pivots.size(); ++i)
    {
      if (pivots(i) != i)
        det *= -1.0;
      det *= mLu(i, i);
    }
    
    return det;
  } 
} // End of namespace 'numeric'.

#endif // NUMERIC_DETERMINANT_HPP

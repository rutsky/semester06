/*
 * invert_matrix.hpp
 * Finding matrix inverse finctions.
 * Based on Fredrik Orderud's "LU Matrix Inversion"
 * (http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?LU_Matrix_Inversion),
 * which based on Numerical Recipies in C, 2nd ed., by Press, Teukolsky, Vetterling & Flannery.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.03.2009
 */

#ifndef INVERT_MATRIX_HPP
#define INVERT_MATRIX_HPP

#include "numeric_common.hpp"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace numeric
{
  // Matrix inversion routine.
  // Uses lu_factorize and lu_substitute in uBLAS to invert a matrix.
  // Note: inverse matrix argument size should match input argument size.
  template< class T >
  inline
  bool invert_matrix( ublas::matrix<T> const &input, ublas::matrix<T> &inverse )
  {
    using namespace boost::numeric::ublas;

    typedef permutation_matrix<std::size_t> pmatrix;
    
    // Create a working copy of the input.
    matrix<T> A(input);
    // Create a permutation matrix for the LU-factorization.
    pmatrix pm(A.size1());

    // Perform LU-factorization.
    int const res = lu_factorize(A, pm);
    if (res != 0)
      return false;

    // Create identity matrix of "inverse".
    inverse.assign(ublas::identity_matrix<T>(A.size1()));

    // Backsubstitute to get the inverse.
    lu_substitute(A, pm, inverse);

    return true;
  }
} // End of namespace 'numeric'.

#endif // INVERT_MATRIX_HPP

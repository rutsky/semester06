/*
 * main.cpp
 * First task on optimization methods.
 * Different methods of solving linear programming problem.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/tuple/tuple.hpp>

#include "numeric/numeric.hpp"

// debug
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <iterator>
#include <algorithm>
// end of debug

int main( int argc, char *argv[] )
{
  typedef numeric::ublas::vector<double> vector_type;
  typedef numeric::ublas::matrix<double> matrix_type;
  
  //if (0)
  {
    std::ifstream is("data/mine");
    
    vector_type c, b, variablesSigns, constraintsSigns;
    matrix_type A;
    
    is >> c >> b >> A >> variablesSigns >> constraintsSigns;
    
    std::cout << "c:\n" << c << "\n";
    std::cout << "b:\n" << b << "\n";
    std::cout << "A:\n" << A << "\n";
    std::cout << "variablesSigns:\n" << variablesSigns << "\n";
    std::cout << "constraintsSigns:\n" << constraintsSigns << "\n";

    std::cout << "'A' rows:\n";
    std::copy(numeric::matrix_rows_begin(A), numeric::matrix_rows_end(A), std::ostream_iterator<vector_type>(std::cout, "\n"));
    
    matrix_type const X(A);
    std::cout << "Const 'A' rows:\n";
    std::copy(numeric::matrix_rows_begin(X), numeric::matrix_rows_end(X), std::ostream_iterator<vector_type>(std::cout, "\n"));
  }
  
  {
    /*
    numeric::matrix_type m(3, 3);
    std::vector<size_t> rows, cols;
    size_t nRows, nCols;
    boost::tie(nRows, nCols) = numeric::simplex::li_submatrix(m, std::back_inserter(rows), std::back_inserter(cols));
    */
  }

  return 0;
}

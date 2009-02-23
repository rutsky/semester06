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

#include "numeric/li_vectors.hpp"

// debug
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
// end of debug

int main( int argc, char *argv[] )
{
  if (0)
  {
    std::ifstream is("data/mine");
    
    numeric::vector_type c, b, variablesSigns, constraintsSigns;
    numeric::matrix_type A;
    
    is >> c >> b >> A >> variablesSigns >> constraintsSigns;
    
    std::cout << "c:\n" << c << "\n";
    std::cout << "b:\n" << b << "\n";
    std::cout << "A:\n" << A << "\n";
    std::cout << "variablesSigns:\n" << variablesSigns << "\n";
    std::cout << "constraintsSigns:\n" << constraintsSigns << "\n";
  }
  
  {
    numeric::matrix_type m(3, 3);
    std::vector<size_t> rows, cols;
    size_t nRows, nCols;
    boost::tie(nRows, nCols) = numeric::simplex::li_submatrix(m, std::back_inserter(rows), std::back_inserter(cols));
  }

  return 0;
}

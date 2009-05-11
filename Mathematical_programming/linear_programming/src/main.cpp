/*
 * main.cpp
 * First task on optimization methods.
 * Different methods of solving linear programming problem.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifdef USING_QT_CREATOR
// Only for debugging in Qt Creator.
#include </opt/qtcreator-1.1.0/share/qtcreator/gdbmacros/gdbmacros.cpp>
#endif // USING_QT_CREATOR

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>

#include <boost/tuple/tuple.hpp>
#include <boost/scoped_ptr.hpp>

#include "numeric/numeric.hpp"

/* The main program function */
int main()
{
  typedef numeric::ublas::vector     <double> vector_type;
  typedef numeric::ublas::unit_vector<double> unit_vector_type;
  typedef numeric::ublas::matrix     <double> matrix_type;
  
  //if (0)
  {
    std::ifstream is("../data/mine.in");
    
    vector_type c, b, variablesSigns, constraintsSigns;
    matrix_type A;
    
    is >> c >> b >> A >> variablesSigns >> constraintsSigns;
    
    {
      std::cout << "c:\n" << c << "\n";
      std::cout << "b:\n" << b << "\n";
      std::cout << "A:\n" << A << "\n";
      std::cout << "variablesSigns:\n" << variablesSigns << "\n";
      std::cout << "constraintsSigns:\n" << constraintsSigns << "\n";
  
      std::cout << "'A' rows:\n";
      std::copy(numeric::matrix_rows_begin(A), numeric::matrix_rows_end(A), 
        std::ostream_iterator<vector_type>(std::cout, "\n"));
      
      matrix_type const X(A);
      std::cout << "Const 'A' rows:\n";
      std::copy(numeric::matrix_rows_begin(X), numeric::matrix_rows_end(X), 
        std::ostream_iterator<vector_type>(std::cout, "\n"));
      
      vector_type resultV(A.size2());
      numeric::simplex::simplex_result_type const result = numeric::simplex::solve_augment(A, b, c, resultV);
      std::cout << "Solution: " << resultV << " (result=" << static_cast<int>(result) << ")\n";
    }
    
    {
      boost::scoped_ptr<std::ofstream> ofs;
    
      ofs.reset(new std::ofstream("../report/matrix_A.tex"));
      numeric::output_matrix_tex(*ofs, A);
      
      ofs.reset(new std::ofstream("../report/vector_b.tex"));
      numeric::output_vector_tex(*ofs, b, false);
      
      ofs.reset(new std::ofstream("../report/vector_c.tex"));
      numeric::output_vector_tex(*ofs, c, true);
    }
  }
  
  return 0;
}

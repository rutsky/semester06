/*
 * main.cpp
 * Fourth task on optimization methods.
 * Different methods of locating 2d functions minimum.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 29.03.2009
 */

#ifdef USING_QT_CREATOR
// Only for debugging in Qt Creator.
#include </opt/qtcreator-1.0.0/share/qtcreator/gdbmacros/gdbmacros.cpp>
#endif // USING_QT_CREATOR

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/scoped_ptr.hpp>

#include "numeric/numeric.hpp"

#include "data/function.hpp"

int main()
{
  typedef double                                   scalar_type; 
  typedef numeric::ublas::vector     <scalar_type> vector_type;
  typedef numeric::ublas::unit_vector<scalar_type> unit_vector_type;
  typedef numeric::ublas::matrix     <scalar_type> matrix_type;
  
  {
    char const *dataFileName = "../output/points.dat";
    boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
    if (!*ofs)
    {
      std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
      return 1;
    }
    
    vector_type startPoint(2);
    startPoint(0) = 1;
    startPoint(1) = 1;
    
    typedef scalar_type (*function_type     )( vector_type const & );
    typedef vector_type (*function_grad_type)( vector_type const & );
    
    function_type      f  = &function::function<vector_type>;
    function_grad_type df = &function::functionGrad<vector_type>;
    
    vector_type const xMin = numeric::gradient_descent::find_min
                               <function_type, function_grad_type, vector_type, std::ofstream>(
                                  f, df, startPoint, 1e-6, 1e-3, *ofs);
  }
  
  
  return 0;
}

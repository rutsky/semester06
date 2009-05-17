/*
 * main.cpp
 * Fifth task on optimization methods.
 * Different methods of locating 2d functions minimum with limitations.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 07.04.2009
 */

#ifdef USING_QT_CREATOR
// Only for debugging in Qt Creator.
//#include </opt/qtcreator-1.1.0/share/qtcreator/gdbmacros/gdbmacros.cpp>
#endif // USING_QT_CREATOR

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "numeric/numeric.hpp"

//#include "data/function.hpp"

int main()
{
  typedef double                              scalar_type; 
  typedef numeric::ublas::vector<scalar_type> vector_type;
  typedef numeric::ublas::matrix<scalar_type> matrix_type;
  
  
}

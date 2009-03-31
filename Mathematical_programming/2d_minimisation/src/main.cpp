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
#include <algorithm>
#include <iomanip>

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include "numeric/numeric.hpp"

#include "data/function.hpp"

int main()
{
  typedef double                                   scalar_type; 
  typedef numeric::ublas::vector     <scalar_type> vector_type;
  typedef numeric::ublas::unit_vector<scalar_type> unit_vector_type;
  typedef numeric::ublas::matrix     <scalar_type> matrix_type;
  
  {
    vector_type startPoint(2);
    startPoint(0) = function::startX;
    startPoint(1) = function::startY;
    
    typedef scalar_type (*function_type     )( vector_type const & );
    typedef vector_type (*function_grad_type)( vector_type const & );
    
    function_type      const f                 = &function::function<vector_type>;
    function_grad_type const df                = &function::functionGrad<vector_type>;
    scalar_type        const preferedPrecision = function::preferedPrecision;
    scalar_type        const step              = function::step;
    
    std::vector<vector_type> points;
    
    vector_type const xMin = numeric::gradient_descent::find_min
                               <function_type, function_grad_type, vector_type>(
                                  f, df, startPoint, preferedPrecision, step, std::back_inserter(points));
    
    {
      // Saving passed spots.
      char const *dataFileName = "../output/points.dat";
      boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
      if (ofs->fail())
      {
        std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
        return 1;
      }
      
      std::for_each(points.begin(), points.end(), 
                    boost::bind<void>(&numeric::output_vector_coordinates<std::ofstream, vector_type>, 
                                      boost::ref(*ofs), _1, " ", "\n"));
    }
    
    if (points.begin() != points.end())
    {
      // Saving passed spots function values (for contours).
      char const *dataFileName = "../output/contours.dat";
      boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
      if (ofs->fail())
      {
        std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
        return 1;
      }
      
      *ofs << "set cntrparam levels discrete ";
      
      //std::cout << f(*points.begin()) << std::endl; // debug
      
      std::transform(++points.begin(), points.end(), std::ostream_iterator<double>(*ofs, ","), f);
      *ofs << f(*points.begin());
      *ofs << std::endl;
    }
    
    {
      // Generating report data.
      
      // Saving passed spots function values (for contours).
      char const *dataFileName = "../output/gd_result.tex";
      boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
      if (ofs->fail())
      {
        std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
        return 1;
      }
      
      for (size_t p = 0; p < numeric::array_size(function::precisions); ++p)
      {
        scalar_type const precision = function::precisions[p];
        
        std::vector<vector_type> points;
        vector_type const xMin = numeric::gradient_descent::find_min
                                   <function_type, function_grad_type, vector_type>(
                                      f, df, startPoint, precision, step, std::back_inserter(points));
        
        // TODO: Formatting.
        *ofs << boost::format("%1$1.0e") % precision << " & " << points.size() << " & (";
        numeric::output_vector_coordinates(*ofs, xMin, ", ", "");
        *ofs << ") & " << boost::format("%1$15.8f") % f(xMin) << " \\\\\n";
      }
    }
  }
  
  return 0;
}

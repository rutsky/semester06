/*
 * main.cpp
 * Six task on optimization methods.
 * Constrained minimization using barrier method    .
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 04.05.2009
 */

#ifdef USING_QT_CREATOR
// Only for debugging in Qt Creator.
#include </opt/qtcreator-1.1.0/share/qtcreator/gdbmacros/gdbmacros.cpp>
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
  typedef double                            scalar_type; 
  typedef numeric::vector     <scalar_type> vector_type;
  typedef numeric::matrix     <scalar_type> matrix_type;
  
  typedef scalar_type (*function_type            )( vector_type const & );
  typedef vector_type (*function_grad_type       )( vector_type const & );
  typedef matrix_type (*function_inv_hessian_type)( vector_type const & );
    
  function_type             const f                  = &function::function<vector_type>;
  function_grad_type        const df                 = &function::functionGrad<vector_type>;
  scalar_type               const preferredPrecision = function::preferredPrecision;
  
  vector_type startPoint(2);
  startPoint(0) = function::startX;
  startPoint(1) = function::startY;
  
  scalar_type const startMu = function::startMu;
  scalar_type const beta    = function::beta;
  
  std::vector<function_type>      constrains;
  std::vector<function_grad_type> constrainsGrads;
  
  constrains.     push_back(&function::limitFunc1<scalar_type>);
  constrains.     push_back(&function::limitFunc2<scalar_type>);
  constrainsGrads.push_back(&function::limitFuncGrad1<scalar_type>);
  constrainsGrads.push_back(&function::limitFuncGrad2<scalar_type>);

  // TODO: Separate this constants.
  scalar_type               const gradientDescentPrecision = 1e-5;
  scalar_type               const gradientDescentStep      = 0.1;

  {
    //
    // Barrier method.
    //
    
    std::vector<vector_type> points;
    
    vector_type const xMin = numeric::barrier_method::find_min
                                 <function_type, function_grad_type, scalar_type>(
                                    f, df, 
                                    constrains.begin(),      constrains.end(), 
                                    constrainsGrads.begin(), constrainsGrads.end(),
                                    startPoint, 
                                    startMu, beta,
                                    preferredPrecision, 
                                    gradientDescentPrecision, gradientDescentStep, std::back_inserter(points));
    
    {
      // Saving passed spots.
      char const *dataFileName = "../output/ne_points.dat"; // TODO: Correct file name.
      boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
      if (ofs->fail())
      {
        std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
        return 1;
      }
      
      std::for_each(points.begin(), points.end(), 
                    boost::bind<void>(&numeric::output_vector_coordinates<std::ofstream, vector_type>, 
                                      boost::ref(*ofs), _1, " ", "\n", "%1$15.8f"));
    }
    
    if (points.begin() != points.end())
    {
      // Saving passed spots function values (for contours).
      char const *dataFileName = "../output/ne_contours.gp"; // TODO: Correct file name.
      boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
      if (ofs->fail())
      {
        std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
        return 1;
      }
      
      *ofs << "set cntrparam levels discrete ";
      
      std::transform(++points.begin(), points.end(), std::ostream_iterator<double>(*ofs, ","), f);
      *ofs << f(*points.begin());
      *ofs << std::endl;
    }
  }
  
  {
    // Generating report data.
    
    char const *dataFileName = "../output/ne_points.tex";
    boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
    if (ofs->fail())
    {
      std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
      return 1;
    }
    
    boost::optional<scalar_type> prevFMinVal;
    for (size_t p = 0; p < numeric::array_size(function::precisions); ++p)
    {
      scalar_type const precision = function::precisions[p];
      
      std::vector<vector_type> points;
      vector_type const xMin = numeric::barrier_method::find_min
                                 <function_type, function_grad_type, scalar_type>(
                                    f, df, 
                                    constrains.begin(),      constrains.end(), 
                                    constrainsGrads.begin(), constrainsGrads.end(),
                                    startPoint, 
                                    startMu, beta,
                                    precision, 
                                    gradientDescentPrecision, gradientDescentStep, std::back_inserter(points));
      
      *ofs << boost::format("%1$1.0e") % precision << " & " << points.size() << " & (";
      numeric::output_vector_coordinates(*ofs, xMin, ", ", "");
      *ofs << ") & ";
      
      scalar_type const curFMinVal = f(xMin);
      
      *ofs << boost::format("%1$15.8f") % curFMinVal << " & ";
      
      if (prevFMinVal)
        *ofs << boost::format("%1e") % (curFMinVal - *prevFMinVal);
      
      prevFMinVal = curFMinVal;
      
      vector_type const curGrad = df(xMin);
      *ofs << " & (";
      numeric::output_vector_coordinates(*ofs, curGrad, ", ", "", "%1e");
      *ofs << ")";

      *ofs << " \\\\\n";
    }
  }
}

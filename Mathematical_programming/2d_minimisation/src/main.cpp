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
  
  typedef scalar_type (*function_type     )( vector_type const & );
  typedef vector_type (*function_grad_type)( vector_type const & );
    
  function_type      const f                 = &function::function<vector_type>;
  function_grad_type const df                = &function::functionGrad<vector_type>;
  scalar_type        const preferedPrecision = function::preferedPrecision;
  scalar_type        const step              = function::step;

  {
    //
    // Gradient descent algorithm.
    //
    
    vector_type startPoint(2);
    startPoint(0) = function::startX;
    startPoint(1) = function::startY;
    
    std::vector<vector_type> points;
    
    vector_type const xMin = numeric::gradient_descent::find_min
                               <function_type, function_grad_type, vector_type>(
                                  f, df, startPoint, preferedPrecision, step, std::back_inserter(points));
    
    {
      // Saving passed spots.
      char const *dataFileName = "../output/gd_points.dat";
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
      char const *dataFileName = "../output/gd_contours.gp";
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
    
    {
      // Generating report data.
      
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
        
        *ofs << boost::format("%1$1.0e") % precision << " & " << points.size() << " & (";
        numeric::output_vector_coordinates(*ofs, xMin, ", ", "");
        *ofs << ") & " << boost::format("%1$15.8f") % f(xMin) << " \\\\\n";
      }
    }
  }
  
  {
    //
    // Genetic algorithm.
    //
    
    vector_type loGen(2);
    loGen(0) = function::loGenX;
    loGen(1) = function::loGenY;
    
    vector_type hiGen(2);
    hiGen(0) = function::hiGenX;
    hiGen(1) = function::hiGenY;
    
    vector_type mutations(2);
    mutations(0) = function::mutationX;
    mutations(1) = function::mutationY;
    
    size_t const nIndividuals = function::nIndividuals;
    double const liveRate     = function::liveRate;
    
    typedef std::vector<vector_type>     points_vec_type;
    typedef std::vector<points_vec_type> points_vecs_vec_type;
    points_vecs_vec_type selectedPointsVecs, notSelectedPointsVec;
    
    typedef numeric::genetic::ParallelepipedonUniformGenerator<vector_type> generator_type;
    typedef numeric::genetic::LCCrossOver                                   crossover_type;
    typedef numeric::genetic::ParallelepipedonMutation<scalar_type>         mutation_type;
    
    vector_type const xMin = numeric::genetic::vectorSpaceGeneticSearch
      <generator_type, crossover_type, mutation_type, vector_type, function_type, scalar_type>(
        generator_type(loGen, hiGen), crossover_type(), mutation_type(mutations.begin(), mutations.end()), f, 
        nIndividuals, liveRate, preferedPrecision,
        std::back_inserter(selectedPointsVecs), std::back_inserter(notSelectedPointsVec));
    
    {
      // Saving generations.
      for (size_t i = 0; i < selectedPointsVecs.size(); ++i)
      {
        // Selected.
        {
          std::string const dataFileName = (boost::format("../output/gen_populations/gen_selected_points_%1$03d.dat") % i).str();
          boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName.c_str()));
          if (ofs->fail())
          {
            std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
            return 1;
          }
          
          points_vec_type const &vec = selectedPointsVecs[i];
          
          for (size_t j = 0; j < vec.size(); ++j)
            numeric::output_vector_coordinates(*ofs, vec[j], ",");
        }
        
        // Not selected.
        {
          std::string const dataFileName = (boost::format("../output/gen_populations/gen_not_selected_points_%1$03d.dat") % i).str();
          boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName.c_str()));
          if (ofs->fail())
          {
            std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
            return 1;
          }
          
          points_vec_type const &vec = notSelectedPointsVec[i];
          
          for (size_t j = 0; j < vec.size(); ++j)
            numeric::output_vector_coordinates(*ofs, vec[j], ",");
        }
      }
    }
    
    {
      // Generating report data.
      
      char const *dataFileName = "../output/gen_result.tex";
      boost::scoped_ptr<std::ofstream> ofs(new std::ofstream(dataFileName));
      if (ofs->fail())
      {
        std::cerr << "Failed to open data file '" << dataFileName << "'.\n";
        return 1;
      }
      
      for (size_t p = 0; p < numeric::array_size(function::precisions); ++p)
      {
        scalar_type const precision = function::precisions[p];
        
        typedef std::vector<std::vector<vector_type> > points_vecs_vec_type;
        points_vecs_vec_type selectedPointsVecs, notSelectedPointsVec;
        
        vector_type const xMin = numeric::genetic::vectorSpaceGeneticSearch
          <generator_type, crossover_type, mutation_type, vector_type, function_type, scalar_type>(
            generator_type(loGen, hiGen), crossover_type(), mutation_type(mutations.begin(), mutations.end()), f, 
            nIndividuals, liveRate, precision,
            std::back_inserter(selectedPointsVecs), std::back_inserter(notSelectedPointsVec));
        
        *ofs << boost::format("%1$1.0e") % precision << " & " << selectedPointsVecs.size() << " & (";
        numeric::output_vector_coordinates(*ofs, xMin, ", ", "");
        *ofs << ") & " << boost::format("%1$15.8f") % f(xMin) << " \\\\\n";
      }
    }
  }
  
  return 0;
}

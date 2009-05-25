/*
 * main.cpp
 * Solving transportation problem.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 24.05.2009
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>

#include <boost/scoped_ptr.hpp>

#include "numeric/numeric.hpp"

// The main program function.
int main( int argc, char *argv[] )
{
  typedef double                                                               scalar_type;
  typedef numeric::vector<scalar_type>                                         vector_type;
  typedef numeric::matrix<scalar_type>                                         matrix_type;
  typedef numeric::transportation_problem::transportation_problem<scalar_type> transportation_problem_type;
  typedef numeric::linear_problem::canonical_linear_problem<scalar_type>       canonical_linear_problem_type;
  
  //if (0)
  {
    transportation_problem_type tp;
        
    {
      // Reading input data.
      
      char const *inputFileName = "data/mine.in";
      //char const *inputFileName = "data/book_sample.in";
      if (argc > 1)
        inputFileName = argv[1];
      
      std::ifstream is(inputFileName);
    
      if (!is)
      {
        // Failed to open input file.
        std::cerr << "Failed to open `" << inputFileName << "' file." << std::endl;
        return 1;
      }

      vector_type a, b;
      matrix_type C;
    
      is >> a >> b >> C;
    
      if (!is)
      {
        // Failed to read something.
        std::cerr << "Failed to read input data from `" << inputFileName << "' file." << std::endl;
        return 2;
      }
      
      tp = transportation_problem_type(a, b, C);
    }
    
    {
      //
      // Transportation problem.
      //
      
      std::cout << "Solving transportation problem:\n";
      numeric::output_transportation_problem(std::cout, tp);
      std::cout << "\n";
      
      transportation_problem_type closedTP;
      numeric::transportation_problem::to_closed(tp, closedTP);
      
      std::cout << "Closed transportation problem:\n";
      numeric::output_transportation_problem(std::cout, closedTP);
      std::cout << "\n";

      // Solving using potentials algorithm.
      matrix_type X;
      numeric::transportation_problem::solve_by_potentials(closedTP, X);
      
      std::cout << "Found solution:\n";
      numeric::output_matrix_console(std::cout, X);
      std::cout << "With cost: " << numeric::transportation_problem::transportation_cost(closedTP, X) << "\n\n";
      
      canonical_linear_problem_type clp;
      numeric::transportation_problem::to_linear_problem(closedTP, clp);
      
      std::cout << "Equivalent canonical linear problem:\n";
      numeric::output_common_linear_problem(std::cout, clp, "%1$g");

      {      
        boost::scoped_ptr<std::ofstream> ofs;
      
        ofs.reset(new std::ofstream("output/lp_canonical.m"));
        numeric::output_as_octave_source(*ofs, clp);
      }

      /*
      matrix_type XBySimplex;
      solve_by_simplex(closedTP, XBySimplex);
      
      std::cout << "Solution by simplex algorithm:\n";
      numeric::output_matrix_console(std::cout, XBySimplex);
      std::cout << "With cost: " << numeric::transportation_problem::transportation_cost(closedTP, XBySimplex) << "\n\n";
      */
    }
  }
}

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
  
  //if (0)
  {
    transportation_problem_type tp;
        
    {
      // Reading input data.
      
      char const *inputFileName = "data/mine.in";
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
      // Solving transportation problem.
      
      std::cout << "Solving transportation problem:\n";
      numeric::output_transportation_problem(std::cout, tp);
      std::cout << "\n";
      
      transportation_problem_type closedTP;
      numeric::transportation_problem::to_closed(tp, closedTP);
      std::cout << "Closed transportation problem:\n";
      numeric::output_transportation_problem(std::cout, closedTP);
      std::cout << "\n";
      
      
    }
  }
}

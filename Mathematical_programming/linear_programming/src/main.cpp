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
#include <iterator>
#include <algorithm>

#include <boost/scoped_ptr.hpp>

#include "numeric/numeric.hpp"

// TODO: Move in correct place.
template< class CLPTraits, class V >
inline
typename CLPTraits::variables_signs_vector_type
  convert_to_variable_signs( V const &variablesSigns )
{
  BOOST_CONCEPT_ASSERT((numeric::ublas::VectorExpressionConcept<V>));

  typedef CLPTraits                                             clp_traits_type;
  typedef typename V::value_type                                scalar_type;
  typedef typename clp_traits_type::variables_signs_vector_type variables_signs_vector_type;
  
  size_t const n = variablesSigns.size();
  variables_signs_vector_type result(n);
  for (size_t c = 0; c < n; ++c)
  {
    scalar_type const v = variablesSigns(c);
    if (v == -1)
      result(c) = numeric::linear_problem::variable_leq_zero;
    else if (v ==  0)
      result(c) = numeric::linear_problem::variable_any_sign;
    else if (v == +1)
      result(c) = numeric::linear_problem::variable_geq_zero;
    else
    {
      // TODO: Possible case: incorrect input.
      BOOST_ASSERT(0);
    }
  }
  
  return result;
}

template< class CLPTraits, class V >
inline
typename CLPTraits::inequality_signs_vector_type
  convert_to_inequality_signs( V const &inequalitySigns )
{
  BOOST_CONCEPT_ASSERT((numeric::ublas::VectorExpressionConcept<V>));

  typedef CLPTraits                                              clp_traits_type;
  typedef typename V::value_type                                 scalar_type;
  typedef typename clp_traits_type::inequality_signs_vector_type inequality_signs_vector_type;
  
  size_t const m = inequalitySigns.size();
  inequality_signs_vector_type result(m);
  for (size_t r = 0; r < m; ++r)
  {
    scalar_type const v = inequalitySigns(r);
    if (v == -1)
      result(r) = numeric::linear_problem::inequality_leq;
    else if (v ==  0)
      result(r) = numeric::linear_problem::inequality_eq;
    else if (v == +1)
      result(r) = numeric::linear_problem::inequality_geq;
    else
    {
      // TODO: Possible case: incorrect input.
      BOOST_ASSERT(0);
    }
  }
  
  return result;
}

// The main program function.
int main( int argc, char *argv[] )
{
  typedef double                       scalar_type;
  typedef numeric::vector<scalar_type> vector_type;
  typedef numeric::matrix<scalar_type> matrix_type;
  
  typedef numeric::linear_problem::common_linear_problem<scalar_type>    common_linear_problem_type;
  typedef numeric::linear_problem::canonical_linear_problem<scalar_type> canonical_linear_problem_type;
  
  //if (0)
  {
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
    
    bool min;
    vector_type c, b, variablesSigns, constraintsSigns;
    matrix_type A;
    
    is >> min >> c >> b >> A >> variablesSigns >> constraintsSigns;
    
    if (!is)
    {
      // Failed to read something.
      std::cerr << "Failed to read input data from `" << inputFileName << "' file." << std::endl;
      return 2;
    }
    
    {
      common_linear_problem_type directLP;
      directLP.min()   = min;
      directLP.c()     = c;
      directLP.cSign() = convert_to_variable_signs<numeric::linear_problem::common_linear_problem_traits<scalar_type>, vector_type>(variablesSigns);
      directLP.A()     = A;
      directLP.ASign() = convert_to_inequality_signs<numeric::linear_problem::common_linear_problem_traits<scalar_type>, vector_type>(constraintsSigns);
      directLP.b()     = b;
      BOOST_ASSERT(numeric::linear_problem::assert_valid(directLP));
      
      std::cout << "Direct problem:\n";
      numeric::output_common_linear_problem(std::cout, directLP);
      
      canonical_linear_problem_type directCanonicalLP;
      to_canonical(directLP, directCanonicalLP);
      
      std::cout << "Direct problem in canonical form:\n";
      numeric::output_common_linear_problem(std::cout, directCanonicalLP);
      
      std::cout << "Dual problem:\n";
      common_linear_problem_type dualLP;
      construct_dual(directLP, dualLP);
      numeric::output_common_linear_problem(std::cout, dualLP);
      
      canonical_linear_problem_type dualCanonicalLP;
      to_canonical(dualLP, dualCanonicalLP);
      
      std::cout << "Dual problem in canonical form:\n";
      numeric::output_common_linear_problem(std::cout, dualCanonicalLP);
      
      {
        // Outputting problems to files.
        boost::scoped_ptr<std::ofstream> ofs;
      
        ofs.reset(new std::ofstream("output/lp_direct.m"));
        numeric::output_as_octave_source(*ofs, directLP);
        
        ofs.reset(new std::ofstream("output/lp_direct_canonical.m"));
        numeric::output_as_octave_source(*ofs, directCanonicalLP);
        
        ofs.reset(new std::ofstream("output/lp_dual.m"));
        numeric::output_as_octave_source(*ofs, dualLP);
        
        ofs.reset(new std::ofstream("output/lp_dual_canonical.m"));
        numeric::output_as_octave_source(*ofs, dualCanonicalLP);
      }
      
      {
        // Solving problems by simplex method.
        
        std::cout << " === Simplex Method === " << std::endl;
        
        vector_type directResultV;
        numeric::simplex::simplex_result_type const directResult = 
            numeric::linear_problem::solve_by_simplex(directLP, directResultV);
        std::cout << "Direct problem solution: \n  " << directResultV << 
            " (result=" << static_cast<int>(directResult) << ")\n";
        
        vector_type directCanonicalResultV;
        numeric::simplex::simplex_result_type const directCanonicalResult = 
            numeric::linear_problem::solve_by_simplex(directCanonicalLP, directCanonicalResultV);
        std::cout << "Canonical direct problem solution: \n  " << directCanonicalResultV << 
            " (result=" << static_cast<int>(directCanonicalResult) << ")\n";
        
        vector_type dualResultV;
        numeric::simplex::simplex_result_type const dualResult = 
            numeric::linear_problem::solve_by_simplex(dualLP, dualResultV);
        std::cout << "Dual problem solution: \n  " << dualResultV << 
            " (result=" << static_cast<int>(dualResult) << ")\n";
        
        vector_type dualCanonicalResultV;
        numeric::simplex::simplex_result_type const dualCanonicalResult = 
            numeric::linear_problem::solve_by_simplex(dualCanonicalLP, dualCanonicalResultV);
        std::cout << "Canonical dual problem solution: \n  " << dualCanonicalResultV << 
            " (result=" << static_cast<int>(dualCanonicalResult) << ")\n";
        
        BOOST_ASSERT(numeric::linear_problem::is_simplex_solving_correct(directLP));
      }
      
      {
        // Solving problems by brute force.
        
        std::cout << " === Brute Force === " << std::endl;
        
        vector_type directResultV;
        numeric::simplex::simplex_result_type const directResult = 
            numeric::linear_problem::solve_by_brute_force(directLP, directResultV);
        std::cout << "Direct problem solution: \n  " << directResultV << 
            " (result=" << static_cast<int>(directResult) << ")\n";
        
        vector_type directCanonicalResultV;
        numeric::simplex::simplex_result_type const directCanonicalResult = 
            numeric::linear_problem::solve_by_brute_force(directCanonicalLP, directCanonicalResultV);
        std::cout << "Canonical direct problem solution: \n  " << directCanonicalResultV << 
            " (result=" << static_cast<int>(directCanonicalResult) << ")\n";
        
        vector_type dualResultV;
        numeric::simplex::simplex_result_type const dualResult = 
            numeric::linear_problem::solve_by_brute_force(dualLP, dualResultV);
        std::cout << "Dual problem solution: \n  " << dualResultV << 
            " (result=" << static_cast<int>(dualResult) << ")\n";
        
        vector_type dualCanonicalResultV;
        numeric::simplex::simplex_result_type const dualCanonicalResult = 
            numeric::linear_problem::solve_by_brute_force(dualCanonicalLP, dualCanonicalResultV);
        std::cout << "Canonical dual problem solution: \n  " << dualCanonicalResultV << 
            " (result=" << static_cast<int>(dualCanonicalResult) << ")\n";
        
        BOOST_ASSERT(numeric::linear_problem::is_brute_force_solving_correct(directLP));
      }
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
}

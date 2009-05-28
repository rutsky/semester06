/*
 * main.cpp
 * Final test.
 * Based on Kelley's cutting plane program.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 28.05.2009
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "numeric/numeric.hpp"

#include "data/function.hpp"

// TODO
template< class S >
struct AdditionalFunc
{
  typedef S                                 scalar_type;
  typedef numeric::vector<scalar_type>      vector_type;
  typedef numeric::basic_range<scalar_type> basic_range_type;
  
  typedef boost::function<scalar_type( vector_type const & )> function_type;
  
  AdditionalFunc( function_type f )
    : func_(f)
  {}
  
  template< class V >
  scalar_type operator () ( numeric::vector_expression<V> const &ve )
  {
    size_t const size = ve().size();
    BOOST_ASSERT(size >= 2);
    basic_range_type range(0, size - 1);
    return func_(numeric::subvector(ve(), range.begin(), range.end())) - ve()(size - 1);
  }
  
private:
  function_type func_;
};

template< class S >
struct AdditionalFuncGrad
{
  typedef S                                 scalar_type;
  typedef numeric::vector<scalar_type>      vector_type;
  typedef numeric::basic_range<scalar_type> basic_range_type;
  
  typedef boost::function<vector_type( vector_type const & )> function_grad_type;
  
  AdditionalFuncGrad( function_grad_type f )
    : funcGrad_(f)
  {}
  
  template< class V >
  vector_type operator () ( numeric::vector_expression<V> const &ve )
  {
    size_t const size = ve().size();
    BOOST_ASSERT(size >= 2);
    basic_range_type range(0, size - 1);
    
    vector_type grad(size);
    numeric::subvector(grad, range.begin(), range.end()) = funcGrad_(numeric::subvector(ve(), range.begin(), range.end()));
    grad(size - 1) = -1;
    return grad;
  }
  
private:
  function_grad_type funcGrad_;
};

int main()
{
  typedef double                              scalar_type; 
  typedef numeric::vector<scalar_type>        vector_type;
  typedef numeric::zero_vector<scalar_type>   zero_vector_type;
  typedef numeric::scalar_vector<scalar_type> scalar_vector_type;
  typedef numeric::zero_matrix<scalar_type>   zero_matrix_type;
  typedef numeric::matrix<scalar_type>        matrix_type;
  
  typedef boost::function<scalar_type( vector_type const & )> function_type;
  typedef boost::function<vector_type( vector_type const & )> function_grad_type;
  
  function_type             const f                  = &function::function<vector_type>;
  function_grad_type        const df                 = &function::functionGrad<vector_type>;
  scalar_type               const preferredPrecision = function::preferredPrecision;
  scalar_type               const minLB              = function::minimumLowerBound;
  
  typedef numeric::linear_problem::common_linear_problem<scalar_type> common_linear_problem_type;

  // Usual constraints.
  std::vector<function_type>      constraints;
  std::vector<function_grad_type> constraintsGrads;
  
  size_t const dimension    = 2;
  size_t const nConstraints = 0;
  
  /*
  constraints.     push_back(&function::limitFunc1<scalar_type>);
  constraints.     push_back(&function::limitFunc2<scalar_type>);
  constraintsGrads.push_back(&function::limitFuncGrad1<scalar_type>);
  constraintsGrads.push_back(&function::limitFuncGrad2<scalar_type>);
  */
  
  // Extended constraints.
  std::vector<function_type>      extendedConstraints;
  std::vector<function_grad_type> extendedConstraintsGrads;
  
  BOOST_ASSERT(constraints.size() == nConstraints);
  BOOST_ASSERT(constraints.size() == constraintsGrads.size());
  for (size_t i = 0; i < nConstraints; ++i)
  {
    boost::bind<vector_type>(numeric::vector_resizer<vector_type>(dimension), _1);
    
    function_type constraint = 
        boost::bind<scalar_type>(constraints[i],
                                 boost::bind<vector_type>(numeric::vector_resizer<vector_type>(dimension), _1));
    extendedConstraints.push_back(constraint);
    
    function_grad_type constraintGrad = 
        boost::bind<vector_type>(numeric::vector_resizer<vector_type>(dimension + 1),
                                 boost::bind<vector_type>(constraintsGrads[i],
                                                          boost::bind<vector_type>(numeric::vector_resizer<vector_type>(dimension), _1)));
    
    extendedConstraintsGrads.push_back(constraintGrad);
  }
  function_type extraConstraint = AdditionalFunc<scalar_type>(f);
  extendedConstraints.push_back(extraConstraint);
  function_grad_type extraConstraintGrad = AdditionalFuncGrad<scalar_type>(df);
  extendedConstraintsGrads.push_back(extraConstraintGrad);
  
  {
    //
    // Kelley's convex cutting plane algorithm.
    //
    
    // Constructing base LP.
    common_linear_problem_type commonLP;
    
    size_t const nArtificialConstrains = 5;
    
    commonLP.min() = true;
    commonLP.c() = zero_vector_type(dimension + 1);
    commonLP.c()(dimension) = 1.;
    commonLP.cSign().resize(dimension + 1);
    std::fill(commonLP.cSign().begin(), commonLP.cSign().end(), numeric::linear_problem::variable_any_sign);
    commonLP.A() = zero_matrix_type(1 + nArtificialConstrains, dimension + 1);
    commonLP.A()(0, dimension) = 1.;
    commonLP.b() = zero_vector_type(1 + nArtificialConstrains);
    commonLP.b()(0) = minLB;
    commonLP.ASign().resize(1 + nArtificialConstrains);
    commonLP.ASign()(0) = numeric::linear_problem::inequality_geq;
    
    // Adding into LP artificial constraints.
    commonLP.A()(1, 0) = -6; 
    commonLP.A()(1, 1) =  0;
    commonLP.b()(1)    =  0;
    commonLP.ASign()(1) = numeric::linear_problem::inequality_leq;

    commonLP.A()(2, 0) = -2; 
    commonLP.A()(2, 1) =  5;
    commonLP.b()(2)    = 30;
    commonLP.ASign()(2) = numeric::linear_problem::inequality_leq;

    commonLP.A()(3, 0) =  4;
    commonLP.A()(3, 1) =  5;
    commonLP.b()(3)    = 60;
    commonLP.ASign()(3) = numeric::linear_problem::inequality_leq;

    commonLP.A()(4, 0) =  4;
    commonLP.A()(4, 1) = -2;
    commonLP.b()(4)    = 32;
    commonLP.ASign()(4) = numeric::linear_problem::inequality_leq;

    commonLP.A()(5, 0) =  0;
    commonLP.A()(5, 1) = -8;
    commonLP.b()(5)    =  0;
    commonLP.ASign()(5) = numeric::linear_problem::inequality_leq;

    std::cout << "Constructed base LP problem:\n";
    numeric::output_common_linear_problem(std::cout, commonLP);
    
    vector_type const resultV = 
        numeric::kelley_cutting_plane::find_min(
            extendedConstraints.begin(),      extendedConstraints.end(),
            extendedConstraintsGrads.begin(), extendedConstraintsGrads.end(),
            commonLP, 
            preferredPrecision);
    
    std::cout << "Result:\n" << resultV << std::endl;
  }
}

/*
 * main.cpp
 * Fifth task on optimization methods.
 * Different methods of locating 2d functions minimum with constraints.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 07.04.2009
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
  //scalar_type               const preferredPrecision = function::preferredPrecision;
  scalar_type               const minLB              = function::minimumLowerBound;
  
  typedef numeric::linear_problem::common_linear_problem<scalar_type> common_linear_problem_type;

  // Usual constraints.
  std::vector<function_type>      constraints;
  std::vector<function_grad_type> constraintsGrads;
  
  size_t const dimension    = 2;
  size_t const nConstraints = 2;
  
  constraints.     push_back(&function::limitFunc1<scalar_type>);
  constraints.     push_back(&function::limitFunc2<scalar_type>);
  constraintsGrads.push_back(&function::limitFuncGrad1<scalar_type>);
  constraintsGrads.push_back(&function::limitFuncGrad2<scalar_type>);
  
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
    
    // Constructing goal linear function.
    common_linear_problem_type commonLP;
    
    commonLP.min() = true;
    commonLP.c() = zero_vector_type(dimension + 1);
    commonLP.c()(dimension) = 1.;
    commonLP.cSign().resize(dimension + 1);
    std::fill(commonLP.cSign().begin(), commonLP.cSign().end(), numeric::linear_problem::variable_any_sign);
    commonLP.A() = zero_matrix_type(1, dimension + 1);
    commonLP.A()(0, dimension) = 1.;
    commonLP.b() = scalar_vector_type(1, minLB);
    commonLP.ASign().resize(1);
    commonLP.ASign()(0) = numeric::linear_problem::inequality_geq;
    
    std::cout << "Constructed starting LP problem:\n";
    numeric::output_common_linear_problem(std::cout, commonLP);
    
    vector_type const resultV = 
        numeric::kelley_cutting_plane::find_min(
            extendedConstraints.begin(),      extendedConstraints.end(),
            extendedConstraintsGrads.begin(), extendedConstraintsGrads.end(),
            commonLP);
    
    std::cout << "Result:\n" << resultV << std::endl;
  }
}

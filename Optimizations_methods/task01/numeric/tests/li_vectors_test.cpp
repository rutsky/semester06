/*
 * li_vectors_test.cpp
 * Linear independent vectors facility tests.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 01.03.2009
 */
 
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../li_vectors.hpp"

BOOST_AUTO_TEST_SUITE( li_vectors_suite )
    
// TODO
using namespace boost::numeric;
typedef ublas::vector<double> vector_type;
typedef ublas::matrix<double> matrix_type;
    
BOOST_AUTO_TEST_CASE( empty_very_basic )
{
  numeric::linear_independent_vectors<vector_type> liVectors;
  
  boost::numeric::ublas::unit_vector<double> v(3);
  
  BOOST_REQUIRE(liVectors.insert(v));
  BOOST_REQUIRE(!liVectors.empty());
}  

BOOST_AUTO_TEST_CASE( unit_vectors_very_basic )
{
  numeric::linear_independent_vectors<vector_type> liVectors;
    
  vector_type v1(3), v2(3), v3(3);
  v1[0] = 1, v1[1] = 0, v1[2] = 0;
  v2[0] = 0, v2[1] = 1, v2[2] = 0;
  v3[0] = 0, v3[1] = 0, v3[2] = 1;
    
  BOOST_REQUIRE(liVectors.insert(v1));
  BOOST_REQUIRE(!liVectors.empty());
  BOOST_REQUIRE(liVectors.insert(v2));
  BOOST_REQUIRE(liVectors.insert(v3));
    
  BOOST_REQUIRE(!liVectors.insert(v1));
  BOOST_REQUIRE(!liVectors.insert(v2));
  BOOST_REQUIRE(!liVectors.insert(v3));
}
  
BOOST_AUTO_TEST_CASE( nonunit_vectors_very_basic )
{
  numeric::linear_independent_vectors<vector_type> liVectors;
    
  vector_type v1(3), v2(3), v3(3), v4(3);
  v1[0] =  5, v1[1] =   0, v1[2] =  0;
  v2[0] =  0, v2[1] =   7, v2[2] =  0;
  v3[0] =  0, v3[1] =   0, v3[2] = 21;
  v4[0] = 21, v4[1] = -34, v4[2] =  0;
    
  BOOST_REQUIRE(liVectors.insert(v1));
  BOOST_REQUIRE(!liVectors.empty());
  BOOST_REQUIRE(liVectors.insert(v2));
  BOOST_REQUIRE(liVectors.insert(v3));
    
  BOOST_REQUIRE(!liVectors.insert(v1));
  BOOST_REQUIRE(!liVectors.insert(v2));
  BOOST_REQUIRE(!liVectors.insert(v3));
  BOOST_REQUIRE(!liVectors.insert(v4));
}
    
BOOST_AUTO_TEST_SUITE_END()

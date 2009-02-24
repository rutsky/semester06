#include <iostream>

#include <boost/test/unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

#include "../li_vectors.hpp"

class li_vectors_test
{
private:
  typedef boost::numeric::ublas::vector<double> vector_type;
  typedef boost::numeric::ublas::matrix<double> matrix_type;

public:
  li_vectors_test()
  {
  }
  
  // TODO: Much and much more tests!
  
  void test_empty()
  {
    numeric::linear_independent_vectors<vector_type> liVectors;
    
    boost::numeric::ublas::unit_vector<double> v(3);
    
    BOOST_REQUIRE(liVectors.insert(v));
    BOOST_REQUIRE(!liVectors.empty());
    
    // TODO
  }
  
  void test_unit_vectors()
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
  
  void test_nonunit_vectors()
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
};

class li_vectors_test_suite
  : public test_suite
{
public:
  li_vectors_test_suite()
    : test_suite("li_vectors_test_suite")
  {
     // Create an instance of the test cases class.
     boost::shared_ptr<li_vectors_test> instance(new li_vectors_test());

     // Create the test cases.
     test_case *empty_test_case =
         BOOST_CLASS_TEST_CASE(&li_vectors_test::test_empty, instance);
     test_case *unit_vectors_test_case =
         BOOST_CLASS_TEST_CASE(&li_vectors_test::test_unit_vectors, instance);
     test_case *nonunit_vectors_test_case =
         BOOST_CLASS_TEST_CASE(&li_vectors_test::test_nonunit_vectors, instance);

     // Add the test cases to the test suite.
     add(empty_test_case);
     add(unit_vectors_test_case);
     add(nonunit_vectors_test_case);
  }
};

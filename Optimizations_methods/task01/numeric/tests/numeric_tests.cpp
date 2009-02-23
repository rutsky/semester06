#include <boost/test/included/unit_test.hpp>
#include <boost/concept_check.hpp>

using boost::unit_test_framework::test_suite;

#include "li_vectors_test.hpp"

test_suite* init_unit_test_suite( int argc, char *argv[] )
{
  boost::ignore_unused_variable_warning(argc);
  boost::ignore_unused_variable_warning(argv);

  // Create the top test suite.
  test_suite *top_test_suite(BOOST_TEST_SUITE("Master test suite"));

  // Add test suites to the top test suite.
  top_test_suite->add(new li_vectors_test_suite());

  return top_test_suite;
}

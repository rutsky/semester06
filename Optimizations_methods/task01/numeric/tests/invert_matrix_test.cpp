/*
 * invert_matrix_test.cpp
 * Matrix inverting test.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.03.2009
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../invert_matrix.hpp"

BOOST_AUTO_TEST_SUITE( invert_matrix_suite )

// TODO
using namespace boost::numeric;
typedef ublas::vector<double> vector_type;
typedef ublas::matrix<double> matrix_type;

BOOST_AUTO_TEST_CASE( very_basic )
{
  matrix_type m = ublas::identity_matrix<double>(6);
  
  matrix_type mInv(6, 6);
  numeric::invert_matrix(m, mInv);
  
  BOOST_REQUIRE(m == mInv);
}

BOOST_AUTO_TEST_SUITE_END()

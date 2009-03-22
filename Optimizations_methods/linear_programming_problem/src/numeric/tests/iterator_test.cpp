/*
 * iterator_test.cpp
 * Iterators test.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 01.03.2009
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../iterator.hpp"

BOOST_AUTO_TEST_SUITE( iterator_suite )

// TODO
using namespace boost::numeric;
typedef ublas::vector<double> vector_type;
typedef ublas::matrix<double> matrix_type;

    
BOOST_AUTO_TEST_CASE( very_basic )
{
  matrix_type m = ublas::identity_matrix<double>(6);
  
  numeric::matrix_rows_iterator<matrix_type> it = numeric::matrix_rows_begin(m);
  for (size_t i = 0; i < 6; ++i)
  {
    //BOOST_CHECK_EQUAL(*it, ublas::row(m, i)); // TODO
    BOOST_REQUIRE(*it == ublas::row(m, i));
    ++it;
  }
  
  //BOOST_CHECK_EQUAL(it, numeric::matrix_rows_end(m)); // TODO
  BOOST_REQUIRE(it == numeric::matrix_rows_end(m));
}

BOOST_AUTO_TEST_CASE( const_very_basic )
{
  matrix_type const m = ublas::identity_matrix<double>(6);
  
  numeric::matrix_rows_const_iterator<matrix_type> it = numeric::matrix_rows_begin(m);
  for (size_t i = 0; i < 6; ++i)
  {
    //BOOST_CHECK_EQUAL(*it, ublas::row(m, i)); // TODO
    BOOST_REQUIRE(*it == ublas::row(m, i));
    ++it;
  }
  
  //BOOST_CHECK_EQUAL(it, numeric::matrix_rows_end(m)); // TODO
  BOOST_REQUIRE(it == numeric::matrix_rows_end(m));
}
    
BOOST_AUTO_TEST_SUITE_END()

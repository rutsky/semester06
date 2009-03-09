/*
 * invert_matrix_test.cpp
 * Matrix inverting test.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 05.03.2009
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../submatrix.hpp"

BOOST_AUTO_TEST_SUITE( submatrix_suite )

// TODO
using namespace boost::numeric;
typedef ublas::identity_matrix<double> identity_matrix_type;
typedef ublas::vector<double> vector_type;
typedef ublas::matrix<double> matrix_type;
typedef numeric::matrix_submatrix<matrix_type> submatrix_type;

BOOST_AUTO_TEST_CASE( very_basic )
{
  matrix_type m = identity_matrix_type(3);
  
  size_t idxs[] = { 0, 1, -1 };
  submatrix_type sm(m, &idxs[0], &idxs[2], &idxs[0], &idxs[2]);
  
  BOOST_REQUIRE(sm == identity_matrix_type(2));
}

BOOST_AUTO_TEST_CASE( very_basic2 )
{
  matrix_type m = identity_matrix_type(4);
  
  static size_t const rows[] = { 0, 1, 2, -1 };
  static size_t const cols[] = { 1, 2, -1 };
  submatrix_type sm(m, &rows[0], &rows[3], &cols[0], &cols[2]);
  
  //std::cout << sm << "\n";
  
  BOOST_REQUIRE(sm(0, 0) == 0); BOOST_REQUIRE(sm(0, 1) == 0);
  BOOST_REQUIRE(sm(1, 0) == 1); BOOST_REQUIRE(sm(1, 1) == 0);
  BOOST_REQUIRE(sm(2, 0) == 0); BOOST_REQUIRE(sm(2, 1) == 1);
}

BOOST_AUTO_TEST_CASE( iterator_construct_very_basic )
{
  matrix_type m = identity_matrix_type(4);
  
  static size_t const rows[] = { 0, 1, 2, -1 };
  static size_t const cols[] = { 1, 2, -1 };
  submatrix_type sm(m, &rows[0], &rows[3], &cols[0], &cols[2]);
  
  sm.begin1();
  sm.end1();
  sm.begin2();
  sm.end2();
  
  const submatrix_type csm = sm;
  
  csm.begin1();
  csm.end1();
  csm.begin2();
  csm.end2();
}

BOOST_AUTO_TEST_CASE( iterator_very_basic )
{
  matrix_type m = identity_matrix_type(4);
  
  static size_t const rows[] = { 0, 1, 2, -1 };
  static size_t const cols[] = { 1, 2, -1 };
  submatrix_type sm(m, &rows[0], &rows[3], &cols[0], &cols[2]);
  
  {
    submatrix_type::iterator1 it = sm.find1(0, 0, 0);
    BOOST_ASSERT(*it == 0);
    ++it;
    BOOST_ASSERT(*it == 1);
    ++it;
    BOOST_ASSERT(*it == 0);
    ++it;
    BOOST_ASSERT(it == sm.end1());
  }
  
  {
    submatrix_type::iterator2 it = sm.find2(0, 0, 0);
    BOOST_ASSERT(*it == 0);
    ++it;
    BOOST_ASSERT(*it == 0);
    ++it;
    BOOST_ASSERT(it == sm.end2());
  }
}

BOOST_AUTO_TEST_SUITE_END()

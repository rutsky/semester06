/*
 * subvector_test.cpp
 * Subvector class test.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 10.03.2009
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

#include "../subvector.hpp"

BOOST_AUTO_TEST_SUITE( subvector_suite )

// TODO
using namespace boost::numeric;
typedef ublas::vector<double> vector_type;
typedef numeric::vector_subvector<vector_type> subvector_type;

BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<subvector_type>));

BOOST_AUTO_TEST_CASE( very_basic )
{
  vector_type v(5);
  for (size_t r = 0; r < v.size(); ++r)
    v(r) = r;
  
  size_t idxs[] = { 0, 3, 4, -1 };
  subvector_type sv(v, &idxs[0], &idxs[3]);
  
  BOOST_REQUIRE(sv.size() == 3);
  BOOST_REQUIRE(sv(0) == 0);
  BOOST_REQUIRE(sv(1) == 3);
  BOOST_REQUIRE(sv(2) == 4);
}

BOOST_AUTO_TEST_CASE( iterator_construct_very_basic )
{
  vector_type v(5);
  for (size_t r = 0; r < v.size(); ++r)
    v(r) = r;
  
  size_t idxs[] = { 0, 3, 4, -1 };
  subvector_type sv(v, &idxs[0], &idxs[3]);
  
  sv.begin();
  sv.end  ();
  
  subvector_type const csv = sv;
  
  csv.begin();
  csv.end  ();
}

BOOST_AUTO_TEST_CASE( iterator_very_basic )
{
  vector_type v(5);
  for (size_t r = 0; r < v.size(); ++r)
    v(r) = r;
  
  size_t idxs[] = { 0, 3, 4, -1 };
  subvector_type sv(v, &idxs[0], &idxs[3]);

  {
    subvector_type::iterator it = sv.begin();
    BOOST_ASSERT(*it == 0);
    ++it;
    BOOST_ASSERT(*it == 3);
    ++it;
    BOOST_ASSERT(*it == 4);
    ++it;
    BOOST_ASSERT(it == sv.end());
  }
  
  {
    subvector_type::reverse_iterator it = sv.rbegin();
    BOOST_ASSERT(*it == 4);
    ++it;
    BOOST_ASSERT(*it == 3);
    ++it;
    BOOST_ASSERT(*it == 0);
    ++it;
    BOOST_ASSERT(it == sv.rend());
  }
}

BOOST_AUTO_TEST_SUITE_END()

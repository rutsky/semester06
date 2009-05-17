/*
 * precompiled_header.h
 * Precompiled header.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 04.05.2009
 */

#if defined __cplusplus
// STL stuff.
#include <cstdlib>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <vector>
#include <map>
#include <set>

// Boost stuff.
#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <boost/numeric/ublas/traits.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/banded.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/hermitian.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/numeric/ublas/detail/concepts.hpp>

#endif

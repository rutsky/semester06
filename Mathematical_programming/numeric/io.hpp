/*
 * io.hpp
 * Input/output methods for matrices and vectors..
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 23.03.2009
 */

#ifndef NUMERIC_IO_HPP
#define NUMERIC_IO_HPP

#include <boost/format.hpp>

#include "numeric_common.hpp"

namespace numeric
{
  template< class OutStream, class Matrix >
  void output_matrix_tex( OutStream &ostr, Matrix m )
  {
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<Matrix>));
    BOOST_ASSERT(m.size1() > 0 && m.size2() > 0);
    
    ostr << "\\begin{array}{";
    for (size_t c = 0; c < m.size2(); ++c)
      ostr << "c";
    ostr << "}\n";
    
    for (size_t r = 0; r < m.size1(); ++r)
    {
      ostr << m(r, 0);
      for (size_t c = 1; c < m.size2(); ++c)
        ostr << " & " << m(r, c);
      ostr << " \\\\\n";
    }
    
    ostr << "\\end{array}\n";
  }
  
  template< class OutStream, class Vector >
  void output_vector_tex( OutStream &ostr, Vector v, bool const transposed = true )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<Vector>));
    BOOST_ASSERT(v.size() > 0);
    
    if (transposed)
    {
      ostr << "\\begin{array}{";
      for (size_t r = 0; r < v.size(); ++r)
        ostr << "c";
      ostr << "}\n";
    }
    else
      ostr << "\\begin{array}{c}\n";
    
    ostr << v(0);
    for (size_t r = 1; r < v.size(); ++r)
    {
      if (transposed)
        ostr << " & ";
      else
        ostr << " \\\\\n";
      
      ostr << v(r);
    }
    
    if (transposed)
      ostr << " \n";
    else
      ostr << " \\\\\n";
    
    ostr << "\\end{array}\n";
  }
  
  template< class OutStream, class Vector >
  void output_vector_coordinates( OutStream &ostr, Vector v,
                                  char const *delimiter = " ", 
                                  char const *end = "\n",
                                  char const *format = "%1$15.8f" )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<Vector>));
    BOOST_ASSERT(v.size() > 0);
    
    ostr << boost::format(format) % v(0);
    for (size_t r = 1; r < v.size(); ++r)
    {
      ostr << delimiter;
      ostr << boost::format(format) % v(r);
    }
    
    ostr << end;
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_IO_HPP

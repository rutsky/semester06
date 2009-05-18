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

#include "linear_problem.hpp"

namespace numeric
{
  template< class OutStream, class Matrix >
  OutStream &output_matrix_tex( OutStream &ostr, Matrix m )
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
    
    return ostr;
  }
  
  template< class OutStream, class Matrix >
  OutStream &output_matrix_pretty( OutStream &ostr, Matrix m,
                                   char const *delimiter = " ", 
                                   char const *stringStart = "",
                                   char const *stringEnd = "\n",
                                   char const *end = "",
                                   char const *format = "%1$15e" )
  {
    BOOST_CONCEPT_ASSERT((ublas::MatrixExpressionConcept<Matrix>));
    BOOST_ASSERT(m.size1() > 0 && m.size2() > 0);
    
    for (size_t r = 0; r < m.size1(); ++r)
    {
      ostr << stringStart;
      ostr << boost::format(format) % m(r, 0);
      for (size_t c = 1; c < m.size2(); ++c)
      {
        ostr << delimiter;
        ostr << boost::format(format) % m(r, c);
      }
      ostr << stringEnd;
    }
    ostr << end;
    
    return ostr;
  }
  
  template< class OutStream, class Vector >
  OutStream &output_vector_tex( OutStream &ostr, Vector v, bool const transposed = true )
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
    
    return ostr;
  }
  
  template< class OutStream, class Vector >
  OutStream &output_vector_coordinates( OutStream &ostr, Vector v,
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
    
    return ostr;
  }
  
  template< class OutStream, class Vector >
  OutStream &output_vector_pretty( OutStream &ostr, Vector v,
                                   char const *delimiter = " ", 
                                   char const *end = "\n",
                                   char const *format = "%1$15.8f" )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<Vector>));
    BOOST_ASSERT(v.size() > 0);

    return output_vector_coordinates(ostr, v, delimiter, end, format);
  }
  
  template< class OutStream, class CLPTraits >
  inline
  void output_common_linear_problem( OutStream &ostr, linear_problem::ICommonLinearProblem<CLPTraits> const &commonLP )
  {
    BOOST_ASSERT(linear_problem::is_valid(commonLP));
    
    size_t const m = linear_problem::constraints_count(commonLP);
    
    char const *format = "%1$6g";
    
    if (commonLP.min())
      ostr << "min    ";
    else
      ostr << "max    ";
    
                       output_vector_pretty(ostr, commonLP.c(),     " ", "\n\n", format);
    ostr << "       "; output_vector_pretty(ostr, commonLP.cSign(), " ", "\n\n", format);
    
    for (size_t r = 0; r < m; ++r)
    {
      ostr << "       "; output_vector_pretty(ostr, row(commonLP.A(), r), " ", "", format);
      ostr << "    " << boost::format(format) % commonLP.ASign()(r) << "    " << boost::format(format) % commonLP.b()(r) << "\n";
    }
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_IO_HPP

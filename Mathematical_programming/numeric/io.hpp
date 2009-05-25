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
#include "transportation_problem.hpp"

namespace numeric
{
  // TODO: Rewrite this whole module.
  
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
  
  template< class OutStream, class E >
  OutStream &output_matrix_pretty( OutStream &ostr, matrix_expression<E> const &m,
                                   char const *rowsDelimiter, 
                                   char const *columnsDelimiter, 
                                   char const *matrixStart,
                                   char const *matrixEnd,
                                   char const *rowStart,
                                   char const *rowEnd,
                                   char const *format )
  {
    ostr << matrixStart;
    for (size_t r = 0; r < m().size1(); ++r)
    {
      if (r != 0)
        ostr << rowsDelimiter;
      
      ostr << rowStart;
      for (size_t c = 0; c < m().size2(); ++c)
      {
        if (c != 0)
          ostr << columnsDelimiter;
        ostr << boost::format(format) % m()(r, c);
      }
      ostr << rowEnd;
    }
    ostr << matrixEnd;
    
    return ostr;
  }
  
  template< class OutStream, class V >
  OutStream &output_vector_pretty( OutStream &ostr, vector_expression<V> const &v,
                                   char const *delimiter, 
                                   char const *start,
                                   char const *end,
                                   char const *format )
  {
    ostr << start;
    for (size_t r = 0; r < v().size(); ++r)
    {
      if (r != 0)
        ostr << delimiter;
      ostr << boost::format(format) % v()(r);
    }
    ostr << end;
    
    return ostr;
  }
  
  template< class OutStream, class E >
  OutStream &output_matrix_console( OutStream &ostr, matrix_expression<E> const &m, char const *format = "%1$5g" )
  {
    char const *rowsDelimiter    = ";\n ";
    char const *columnsDelimiter = " ";
    char const *matrixStart      = "[";
    char const *matrixEnd        = "]\n";
    char const *rowStart         = "";
    char const *rowEnd           = "";
    return output_matrix_pretty(ostr, m, 
                                rowsDelimiter, columnsDelimiter, 
                                matrixStart, matrixEnd, 
                                rowStart, rowEnd, 
                                format);
  }
  
  // TODO: Rename to smth. like `output_octave'.
  template< class OutStream, class E >
  OutStream &output_matrix_octave( OutStream &ostr, matrix_expression<E> const &m, char const *format = "%1$5g" )
  {
    char const *rowsDelimiter    = "; ";
    char const *columnsDelimiter = " ";
    char const *matrixStart      = "[";
    char const *matrixEnd        = "]";
    char const *rowStart         = "";
    char const *rowEnd           = "";
    return output_matrix_pretty(ostr, m, 
                                rowsDelimiter, columnsDelimiter, 
                                matrixStart, matrixEnd, 
                                rowStart, rowEnd, 
                                format);
  }
  
  template< class OutStream, class V >
  OutStream &output_vector_console( OutStream &ostr, vector_expression<V> const &v, char const *format = "%1$5g" )
  {
    char const *delimiter   = " ";
    char const *start       = "[";
    char const *end         = "]";
    return output_vector_pretty(ostr, v, delimiter, start, end, format);
  }
  
  template< class OutStream, class V >
  OutStream &output_vector_octave( OutStream &ostr, vector_expression<V> const &v, char const *format = "%1$g" )
  {
    char const *delimiter   = " ";
    char const *start       = "[";
    char const *end         = "]";
    return output_vector_pretty(ostr, v, delimiter, start, end, format);
  }
  
  template< class OutStream, class CLPTraits >
  inline
  void output_common_linear_problem( OutStream &ostr, 
                                     linear_problem::ICommonLinearProblem<CLPTraits> const &commonLP,
                                     char const *format = "%1$6g" )
  {
    BOOST_ASSERT(linear_problem::is_valid(commonLP));
    
    size_t const m = linear_problem::constraints_count(commonLP);
    
    if (commonLP.min())
      ostr << "min    ";
    else
      ostr << "max    ";
    
                       output_vector_pretty(ostr, commonLP.c(),     " ", "", "\n\n", format);
    ostr << "       "; output_vector_pretty(ostr, commonLP.cSign(), " ", "", "\n\n", format);
    
    for (size_t r = 0; r < m; ++r)
    {
      ostr << "       "; output_vector_pretty(ostr, row(commonLP.A(), r), " ", "", "", format);
      ostr << "    " << boost::format(format) % commonLP.ASign()(r) << "    " << boost::format(format) % commonLP.b()(r) << "\n";
    }
  }
  
  template< class OutStream, class CLPTraits >
  inline
  void output_as_octave_source( OutStream &ostr, 
                                linear_problem::ICommonLinearProblem<CLPTraits> const &commonLP, 
                                char const *format = "%1$g",
                                char const *namesSuffix = "" )
  {
    BOOST_ASSERT(linear_problem::is_valid(commonLP));
    
    typedef CLPTraits                        clp_traits;
    typedef typename clp_traits::scalar_type scalar_type;
    typedef vector<scalar_type>              vector_type;
    
    size_t const n = linear_problem::variables_count(commonLP);
    size_t const m = linear_problem::constraints_count(commonLP);
    
    ostr << "c" << namesSuffix << " = ";
    output_vector_octave(ostr, commonLP.c(), format);
    ostr << "';\n";
    
    ostr << "A" << namesSuffix << " = ";
    output_matrix_octave(ostr, commonLP.A(), format);
    ostr << ";\n";
    
    ostr << "b" << namesSuffix << " = ";
    output_vector_octave(ostr, commonLP.b(), format);
    ostr << "';\n";
    
    vector_type lb(n), ub(n);
    for (size_t c = 0; c < n; ++c)
    {
      switch (commonLP.cSign()(c))
      {
      case linear_problem::variable_leq_zero:
        lb(c) = -infinity<scalar_type>();
        ub(c) = 0.;
        break;
      case linear_problem::variable_any_sign:
        lb(c) = -infinity<scalar_type>();
        ub(c) = +infinity<scalar_type>();
        break;
      case linear_problem::variable_geq_zero:
        lb(c) = 0.;
        ub(c) = +infinity<scalar_type>();
        break;
      default:
        // Impossible case.
        BOOST_ASSERT(0);
      }
    }
    
    ostr << "lb" << namesSuffix << " = ";
    output_vector_octave(ostr, lb, format);
    ostr << "';\n";
    
    ostr << "ub" << namesSuffix << " = ";
    output_vector_octave(ostr, ub, format);
    ostr << "';\n";
    
    ostr << "ctype" << namesSuffix << " = \"";
    for (size_t r = 0; r < m; ++r)
      switch (commonLP.ASign()(r))
      {
      case linear_problem::inequality_leq:
        ostr << "U" ;
        break;
      case linear_problem::inequality_eq:
        ostr << "S";
        break;
      case linear_problem::inequality_geq:
        ostr << "L" ;
        break;
      default:
        // Impossible case.
        BOOST_ASSERT(0);
      }
    ostr << "\";\n";
        
    ostr << "vartype" << namesSuffix << " = \"";
    std::fill_n(std::ostream_iterator<char>(ostr), n, 'C');
    ostr << "\";\n";
        
    ostr << "sense" << namesSuffix << " = " << (commonLP.min() ? 1 : -1) << ";\n";
    
    ostr << "\n";
    
    ostr << "[xmin, fmin, status, extra] = glpk(c, A, b, lb, ub, ctype, vartype, sense)\n";
  }
  
  template< class OutStream, class TPTraits >
  inline
  void output_transportation_problem( OutStream &ostr, 
                                      transportation_problem::ITransportationProblem<TPTraits> const &tp,
                                      char const *format = "%1$6g" )
  {
    size_t const m = transportation_problem::supplies_count(tp);
    
    output_vector_pretty(ostr, tp.b(),     " ", "", "\n\n", format);
    
    for (size_t r = 0; r < m; ++r)
    {
      output_vector_pretty(ostr, row(tp.C(), r), " ", "", "", format);
      ostr << "    " << boost::format(format) % tp.a()(r) << "\n";
    }
  }
} // End of namespace 'numeric'.

#endif // NUMERIC_IO_HPP

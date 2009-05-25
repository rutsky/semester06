/*
 * potentials_alg.hpp
 * Simplex algorithm.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 15.02.2009
 */

#ifndef NUMERIC_POTENTIALS_ALG_HPP
#define NUMERIC_POTENTIALS_ALG_HPP

#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <list>

#include "numeric_common.hpp"

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/utility.hpp>

#include "transportation_problem.hpp"
#include "matrix_ops.hpp"

namespace numeric
{
namespace lp_potentials
{
  template< class V1, class V2, class M >
  bool is_tp_valid( vector_expression<V1> const &a, vector_expression<V2> const &b,
                    matrix_expression<M>  const &C )
  {
    typedef typename V1::value_type scalar_type; // TODO

    if (!(C().size1() > 0 && C().size2() > 0 && C().size1() == a().size() && C().size2() == b().size()))
    {
      // Data sizes is not correspond.
      return false;
    }
    
    if (!(
        std::find_if(a().begin(), a().end(), 
                     boost::bind<bool>(std::less_equal<scalar_type>(), _1, scalar_type(0.))) == a().end() &&
        std::find_if(b().begin(), b().end(), 
                     boost::bind<bool>(std::less_equal<scalar_type>(), _1, scalar_type(0.))) == b().end() &&
        std::find_if(C().data().begin(), C().data().end(), 
                     boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == C().data().end()))
    {
      // One of data values is less than zero, which is incorrect.
      return false;
    }
    
    return true;
  }
  
  template< class V1, class V2, class M >
  bool assert_tp_valid( vector_expression<V1> const &a, vector_expression<V2> const &b,
                        matrix_expression<M>  const &C )
  {
    typedef typename V1::value_type scalar_type; // TODO

    // Asserting sizes.
    ASSERT_GT(a().size(), 0);           // a().size() > 0
    ASSERT_GT(b().size(), 0);           // b().size() > 0
    ASSERT_GT(C().size1(), 0);          // C().size1() > 0 
    ASSERT_GT(C().size2(), 0);          // C().size2() > 0
    ASSERT_EQ(C().size1(), a().size()); // C().size1() == a().size()
    ASSERT_EQ(C().size2(), b().size()); // C().size2() == b().size()

    ASSERT(std::find_if(a().begin(), a().end(), 
                        boost::bind<bool>(std::less_equal<scalar_type>(), _1, scalar_type(0.))) == a().end());
    ASSERT(std::find_if(b().begin(), b().end(), 
                        boost::bind<bool>(std::less_equal<scalar_type>(), _1, scalar_type(0.))) == b().end());
    ASSERT(std::find_if(C().data().begin(), C().data().end(), 
                        boost::bind<bool>(std::less<scalar_type>(), _1, scalar_type(0.))) == C().data().end());
    
    return is_tp_valid(a, b, C);
  }
  
  template< class V1, class V2, class M >
  bool is_tp_closed( vector_expression<V1> const &a, vector_expression<V2> const &b,
                     matrix_expression<M>  const &C )
  {
    typedef typename V1::value_type scalar_type; // TODO
    
    BOOST_ASSERT(assert_tp_valid(a, b, C));
    
    if (std::accumulate(a().begin(), a().end(), scalar_type(0.)) == 
        std::accumulate(b().begin(), b().end(), scalar_type(0.)))
    {
      // Sum of supplies equal to sum of demand, problem is closed.
      return true;
    }
    else
    {
      // Problem is unclosed.
      return false;
    }
  }
  
  template< class V1, class V2, class M >
  bool is_plan( vector_expression<V1> const &a, vector_expression<V2> const &b,
                matrix_expression<M> const &X )
  {
    typedef typename V1::value_type scalar_type;
    
    size_t const m = a().size(), n = b().size();
    
    ASSERT_EQ(X().size1(), m);
    ASSERT_EQ(X().size2(), n);
    
    for (size_t r = 0; r < m; ++r)
      if (!eq(std::accumulate(row   (X(), r).begin(), row   (X(), r).end(), scalar_type()), a()(r)))
        return false;

    for (size_t c = 0; c < n; ++c)
      if (!eq(std::accumulate(column(X(), c).begin(), column(X(), c).end(), scalar_type()), b()(c)))
        return false;
    
    return true;
  }
  
  template< class V1, class V2, class M >
  bool assert_plan( vector_expression<V1> const &a, vector_expression<V2> const &b,
                    matrix_expression<M> const &X )
  {
    typedef typename V1::value_type scalar_type;
    
    size_t const m = a().size(), n = b().size();
    
    ASSERT_EQ(X().size1(), m);
    ASSERT_EQ(X().size2(), n);
    
    // debug
    std::cout << "assert_plan()\n";
    std::cout << "    a: "; output_vector_console(std::cout, a()); std::cout << "\n";
    std::cout << "    b: "; output_vector_console(std::cout, b()); std::cout << "\n";
    std::cout << "    X:\n"; output_matrix_console(std::cout, X());
    // end of debug
    
    for (size_t r = 0; r < m; ++r)
    {
      ASSERT_FUZZY_EQ(std::accumulate(row   (X(), r).begin(), row   (X(), r).end(), scalar_type()), a()(r));
    }

    for (size_t c = 0; c < n; ++c)
    {
      ASSERT_FUZZY_EQ(std::accumulate(column(X(), c).begin(), column(X(), c).end(), scalar_type()), b()(c));
    }
    
    return is_plan(a, b, X);
  }

  // TODO: Use `details' namespace.
  namespace
  {
    struct cell_type
    {
      cell_type()
      {}
      
      cell_type( cell_type const &other )
        : r   (other.r)
        , c   (other.c)
        , mark(other.mark)
      {}
      
      cell_type( size_t newR, size_t newC )
        : r(newR)
        , c(newC)
      {}
      
      // Cell position in table.
      size_t r, c;
      
      // Cell mark, for DFS.
      bool mark;
    };
    
    typedef boost::shared_ptr<cell_type>                       cell_ptr_type;
    typedef std::map<size_t, cell_ptr_type>                    cells_map_type;
    typedef std::vector<cells_map_type>                        cells_maps_vector_type;
    
    typedef std::map<std::pair<size_t, size_t>, cell_ptr_type> all_cells_map_type;
    
    template< class V, class M >
    void build_start_plan( vector_expression<V> const &aVec, vector_expression<V> const &bVec,
                           matrix_expression<M> const &C,
                           matrix_expression<M>       &x,
                           all_cells_map_type         &planCells )
    {
      typedef typename V::value_type    scalar_type; // TODO
      typedef vector<scalar_type>       vector_type;
      typedef matrix<scalar_type>       matrix_type;
      typedef zero_matrix<scalar_type> zero_matrix_type;
      
      vector_type a = aVec(), b = bVec();
      
      ASSERT(assert_tp_valid(a, b, C));
      ASSERT(is_tp_closed(a, b, C));
      
      size_t const m = C().size1(), n = C().size2();
      
      // Initializing list of unprocessed columns indexes.
      typedef std::list<size_t> unprocessed_cols_list_type;
      unprocessed_cols_list_type unprocessedCols;
      basic_range<size_t, long> N(0, n);
      unprocessedCols.assign(N.begin(), N.end());
      
      // Resetting result data().
      planCells.clear();
      x() = zero_matrix_type(m, n);
      
      // Building start plan.
      for (size_t r = 0; r < m; ++r)
      {
        scalar_type &supply = a(r);
        
        while (true)
        {
          bool const fake = eq_zero(supply);
          
          // debug
          std::cout << "  row = " << r << "\n";
          std::cout << "a: "; output_vector_console(std::cout, a); std::cout << "\n";
          std::cout << "b: "; output_vector_console(std::cout, b); std::cout << "\n";
          std::cout << "X:\n"; output_matrix_console(std::cout, x());
          // end of debug
          
          ASSERT(!unprocessedCols.empty());
        
          // Locating column with lowest transfer cost from current row.
          boost::optional<size_t> minElemIdx;
          for (unprocessed_cols_list_type::const_iterator it = unprocessedCols.begin(); it != unprocessedCols.end(); ++it)
            if (!minElemIdx || C()(r, *it) < C()(r, minElemIdx.get()))
              minElemIdx = *it;
          size_t const minc = *minElemIdx;
          
          scalar_type &demand = b(minc);
          
          if (supply < demand)
          {
            // Left supply can't satisfy current demand.
            
            // Decreasing demand by left supply value.
            scalar_type const transfer = supply;
            ASSERT_EQ(x()(r, minc), scalar_type(0.));
            x()(r, minc) = transfer;
            demand      -= transfer;
            supply      -= transfer; supply = 0;
            
            // Adding current cell into plan.
            VERIFY(planCells.insert(std::make_pair(std::make_pair(r, minc), cell_ptr_type(new cell_type(r, minc)))).second);
            
            // Interrupting row processing.
            break;
          }
          else if (supply > demand)
          {
            // Left supply is greater that current demand.
            
            // Satisfying demand.
            scalar_type const transfer = demand;
            ASSERT_EQ(x()(r, minc), scalar_type(0.));
            x()(r, minc) = transfer;
            demand      -= transfer; demand = 0;
            supply      -= transfer;
            
            // Adding current cell into plan.
            VERIFY(planCells.insert(std::make_pair(std::make_pair(r, minc), cell_ptr_type(new cell_type(r, minc)))).second);
            
            // Removing current demand from unprocessed list.
            unprocessed_cols_list_type::iterator it = std::remove(unprocessedCols.begin(), unprocessedCols.end(), minc);
            ASSERT(it != unprocessedCols.end());
            ASSERT(boost::next(it, 1) == unprocessedCols.end());
            unprocessedCols.erase(it, unprocessedCols.end());
            
            // Continuing on current row.
          }
          else // supply == demand
          {
            // Left supply exactly satisfies demand.
            // Satisfying demand
            scalar_type const transfer = demand;
            ASSERT_EQ(x()(r, minc), scalar_type(0.));
            x()(r, minc) = transfer;
            demand      -= transfer; demand = 0;
            supply      -= transfer; supply = 0;

            // Adding current cell into plan
            VERIFY(planCells.insert(std::make_pair(std::make_pair(r, minc), cell_ptr_type(new cell_type(r, minc)))).second);
            
            // Removing current demand from unprocessed list.
            unprocessed_cols_list_type::iterator it = std::remove(unprocessedCols.begin(), unprocessedCols.end(), minc);
            ASSERT(it != unprocessedCols.end());
            ASSERT(boost::next(it, 1) == unprocessedCols.end());
            unprocessedCols.erase(it, unprocessedCols.end());
            
            // Continuing on current row (for adding fake plan cell).
            if (r == m - 1)
            {
              // But no fake elements at last row.
              break;
            }
          }
          
          if (fake)
          {
            // Not more than one fake element per row.
            break;
          }
        }
      }
      
      // Asserting that no supplies or demands are left.
      ASSERT_EQ(norm_2(a), 0);
      ASSERT_EQ(norm_2(b), 0);

      // Asserting that number of plan points is exactly m + n - 1.
      ASSERT_EQ(planCells.size(), m + n - 1);
      
      // Asserting that founded x is a plan.
      ASSERT(assert_plan(aVec, bVec, x));
    }
    
    template< class M, class V >
    void calculate_potentials_coefs( cells_maps_vector_type const &rows, cells_maps_vector_type const &cols,
                                     matrix_expression<M> const &C,
                                     vector_expression<V> &uVec, vector_expression<V> &vVec )
    {
      typedef typename M::value_type scalar_type; // TODO
      
      size_t const m = C().size1(), n = C().size2();
      
      ASSERT_EQ(rows.size(), m); // rows.size() == m
      ASSERT_EQ(cols.size(), n); // cols.size() == n
      
      // debug
      // Asserting that rows and columns data are valid.
      matrix<bool> _isPlanPointByRow = scalar_matrix<bool>(m, n, false);
      for (size_t r = 0; r < m; ++r)
        for (cells_map_type::const_iterator cellPtrIt = rows[r].begin(); cellPtrIt != rows[r].end(); ++cellPtrIt)
        {
          cell_type const &cell = *(cellPtrIt->second);
          size_t const c = cell.c;
          ASSERT_LT(c, n);      // c <= n
          ASSERT_EQ(cell.r, r); // cell.r == r
          
          _isPlanPointByRow(r, c) = true;
        }
      
      matrix<bool> _isPlanPointByColumn = scalar_matrix<bool>(m, n, false);
      for (size_t c = 0; c < n; ++c)
        for (cells_map_type::const_iterator cellPtrIt = cols[c].begin(); cellPtrIt != cols[c].end(); ++cellPtrIt)
        {
          cell_type const &cell = *(cellPtrIt->second);
          size_t const r = cell.r;
          ASSERT_LT(r, m);      // r < m
          ASSERT_EQ(cell.c, c); // cell.c == c
          
          _isPlanPointByColumn(r, c) = true;
        }
      
      ASSERT_EQ(_isPlanPointByRow, _isPlanPointByColumn);
      // end of debug

      
      std::vector<boost::optional<scalar_type> > u(m), v(n);
      std::queue<size_t> rowsQueue, colsQueue;
      
      // Setting u(0) to zero and adding first row to rows queue.
      u[0] = 0;
      rowsQueue.push(0);
      
      while (!rowsQueue.empty() || !colsQueue.empty())
      {
        if (!rowsQueue.empty())
        {
          // Poping row index from rows queue.
          size_t const r = rowsQueue.front();
          rowsQueue.pop();
          
          for (cells_map_type::const_iterator cellPtrIt = rows[r].begin(); cellPtrIt != rows[r].end(); ++cellPtrIt)
          {
            cell_type const &cell = *(cellPtrIt->second);
            size_t const c = cell.c;
            ASSERT_LT(c, n);      // c < n
            ASSERT_EQ(cell.r, r); // cell.r == r
            
            if (!v[c])
            {
              // For each unprocessed column calculating potential coefficient v[c]:
              //   u[r] + v[c] = C[r, c]   <=>
              //   v[c] = C[r, c] - u[r]
              v[c] = C()(r, c) - u[r].get();
              
              // And adding processed columns to columns queue.
              colsQueue.push(c);
            }
          }
        }
        
        if (!rowsQueue.empty())
        {
          // Poping row index from columns queue.
          size_t const c = colsQueue.front();
          colsQueue.pop();
          
          for (cells_map_type::const_iterator cellPtrIt = cols[c].begin(); cellPtrIt != cols[c].end(); ++cellPtrIt)
          {
            cell_type const &cell = *(cellPtrIt->second);
            size_t const r = cell.r;
            ASSERT_LT(r, m);      // r < m
            ASSERT_EQ(cell.c, c); // cell.c == c
            
            if (!u[r])
            {
              // For each unprocessed column calculating potential coefficient v[c]:
              //   u[r] + v[c] = C[r, c]   <=>
              //   u[r] = C[r, c] - v[c]
              u[r] = C()(r, c) - v[c].get();
              
              // And adding processed columns to columns queue.
              colsQueue.push(r);
            }
          }
        }
      }
      
      uVec().resize(m);
      std::transform(u.begin(), u.end(), uVec().begin(), boost::lambda::ret<scalar_type>(*boost::lambda::_1));
      vVec().resize(n);
      std::transform(v.begin(), v.end(), vVec().begin(), boost::lambda::ret<scalar_type>(*boost::lambda::_1));
      
      // debug
      // Asserting that potentials are valid.
      for (size_t r = 0; r < m; ++r)
        for (size_t c = 0; c < m; ++c)
        {
          scalar_type const p = uVec()(r) + vVec()(c) - C()(r, c);
          if (_isPlanPointByRow(r, c))
          {
            ASSERT_EQ(p, scalar_type(0));
          }
          else
          {
            ASSERT_GT(p, scalar_type(0));
          }
        }
      // end of debug
    }
    
    template< class M, class V >
    void calculate_potentials( vector_expression<V> const &u, vector_expression<V> const &v,
                               matrix_expression<M> const &C,
                               matrix_expression<M>       &P )
    {
      typedef typename V::value_type  scalar_type; // TODO
      typedef vector<scalar_type>      vector_type;
      typedef zero_vector<scalar_type> zero_vector_type;
      typedef matrix<scalar_type>      matrix_type;
      typedef zero_matrix<scalar_type> zero_matrix_type;
      
      size_t const m = u().size(), n = v().size();
      
      // Asserting sizes.
      ASSERT_GT(m, 0);           // m > 0
      ASSERT_GT(n, 0);           // n > 0
      ASSERT_GT(C().size1(), 0); // C().size1() > 0 
      ASSERT_GT(C().size2(), 0); // C().size2() > 0
      ASSERT_EQ(C().size1(), m); // C().size1() == m
      ASSERT_EQ(C().size2(), n); // C().size2() == n

      P().resize(m, n);
      
      for (size_t r = 0; r < m; ++r)
        for (size_t c = 0; c < n; ++c)
        {
          P()(r, c) = u()(r) + v()(c) - C()(r, c);
        }
    }

    template< class IdxsOutputIterator >
    bool find_loop( cells_maps_vector_type const &rows, cells_maps_vector_type const &cols,
                    size_t r, size_t c, size_t const goalR, size_t const goalC, 
                    IdxsOutputIterator idxsOut, bool horizontalSearch )
    {
      // TODO: Assert sizes.
      
      cell_type &curCell = *rows[r].find(c)->second;
      
      // Adding current point to building loop end on visit.
      curCell.mark = true;
      
      if (horizontalSearch)
      {
        // Searching for next cell to append to loop in the row of loop end.
        for (cells_map_type::const_iterator cellPtrIt = rows[r].begin(); cellPtrIt != rows[r].end(); ++cellPtrIt)
        {
          cell_type const &cell = *(cellPtrIt->second);
          
          if (r == cell.r && c == cell.c)
          {
            // Omitting current cell.
            continue;
          }
          
          if (cell.r == goalR && cell.c == goalC)
          {
            // Found loop, outputting it.
            *idxsOut++ = std::make_pair(r, c);
            return true;
          }
          
          if (!cell.mark)
          {
            // Found cell not in current loop, trying to append it to current loop.
            if (find_loop(rows, cols, cell.r, cell.c, goalR, goalC, idxsOut, !horizontalSearch))
            {
              // Loop was found, outputting it.
              *idxsOut++ = std::make_pair(r, c);
              return true;
            }
          }
        }
      }
      else
      {
        // Searching for next cell to append to loop in the row of loop end.
        for (cells_map_type::const_iterator cellPtrIt = cols[c].begin(); cellPtrIt != cols[c].end(); ++cellPtrIt)
        {
          cell_type const &cell = *(cellPtrIt->second);
          
          if (r == cell.r && c == cell.c)
          {
            // Omitting current cell.
            continue;
          }
          
          if (cell.r == goalR && cell.c == goalC)
          {
            // Found loop, outputting it.
            *idxsOut++ = std::make_pair(r, c);
            return true;
          }
          
          if (!cell.mark)
          {
            // Found cell not in current loop, trying to append it to current loop.
            if (find_loop(rows, cols, cell.r, cell.c, goalR, goalC, idxsOut, !horizontalSearch))
            {
              // Loop was found, outputting it.
              *idxsOut++ = std::make_pair(r, c);
              return true;
            }
          }
        }
      }
      
      // Nothing useful found, removing current point from loop end.
      curCell.mark = false;

      return false;
    }
    
    template< class IdxsOutputIterator >
    void find_loop( cells_maps_vector_type const &rows, cells_maps_vector_type const &cols,
                    size_t const goalR, size_t const goalC, 
                    IdxsOutputIterator idxsOut )
    {
      // TODO: Assert sizes.
      size_t const m = rows.size(), n = cols.size();
      
      // debug
      {
        // Asserting that rows and columns data are valid.
        matrix<bool> _isPlanPointByRow = scalar_matrix<bool>(m, n, false);
        for (size_t r = 0; r < m; ++r)
          for (cells_map_type::const_iterator cellPtrIt = rows[r].begin(); cellPtrIt != rows[r].end(); ++cellPtrIt)
          {
            cell_type const &cell = *(cellPtrIt->second);
            size_t const c = cell.c;
            ASSERT_LT(c, n);      // c <= n
            ASSERT_EQ(cell.r, r); // cell.r == r
            
            _isPlanPointByRow(r, c) = true;
          }
        
        matrix<bool> _isPlanPointByColumn = scalar_matrix<bool>(m, n, false);
        for (size_t c = 0; c < n; ++c)
          for (cells_map_type::const_iterator cellPtrIt = cols[c].begin(); cellPtrIt != cols[c].end(); ++cellPtrIt)
          {
            cell_type const &cell = *(cellPtrIt->second);
            size_t const r = cell.r;
            ASSERT_LT(r, m);      // r < m
            ASSERT_EQ(cell.c, c); // cell.c == c
            
            _isPlanPointByColumn(r, c) = true;
          }
        
        ASSERT_EQ(_isPlanPointByRow, _isPlanPointByColumn);
      }
      // end of debug
      
      // Resetting cells marks.
      for (size_t r = 0; r < m; ++r)
        for (cells_map_type::const_iterator cellPtrIt = rows[r].begin(); cellPtrIt != rows[r].end(); ++cellPtrIt)
        {
          cell_type &cell = *(cellPtrIt->second);
          size_t const c = cell.c;
          ASSERT_LT(c, n);      // c <= n
          ASSERT_EQ(cell.r, r); // cell.r == r
          
          cell.mark = false;
        }
      
      // Searching starting by row. Must be equivalent to starting from column.
      VERIFY(find_loop(rows, cols, goalR, goalC, goalR, goalC, idxsOut, true));
    }
  } // End of anonymous namespace.
  
  template< class V1, class V2, class M1, class M2 >
  void solve( vector_expression<V1> const &a,
              vector_expression<V2> const &b,
              matrix_expression<M1> const &C,
              matrix_expression<M2>       &X )
  {
    typedef typename V1::value_type scalar_type; // TODO
    typedef vector<scalar_type>      vector_type;
    typedef zero_vector<scalar_type> zero_vector_type;
    typedef matrix<scalar_type>      matrix_type;
    typedef zero_matrix<scalar_type> zero_matrix_type;
    
    ASSERT(assert_tp_valid(a, b, C));
    ASSERT(is_tp_closed(a, b, C));

    size_t const m = a().size(), n = b().size();

    // Building start plan.
    matrix_type x;
    all_cells_map_type planCells;
    
    build_start_plan(a, b, C, x, planCells);
    
    // Building structure for fast plan cells retrieving.
    cells_maps_vector_type rows(m), cols(n);
    for (all_cells_map_type::const_iterator it = planCells.begin(); it != planCells.end(); ++it)
    {
      size_t const r = it->first.first, c = it->first.second;
      cell_ptr_type cellPtr = it->second;
      
      ASSERT_EQ(r, cellPtr->r);
      ASSERT_EQ(c, cellPtr->c);
      
      VERIFY(rows[r].insert(std::make_pair(c, cellPtr)).second);
      VERIFY(cols[c].insert(std::make_pair(r, cellPtr)).second);
    }
    
    // Iterating through all plans.
    vector_type u, v;
    matrix_type P;
    size_t const nMaxIterations(1000); // debug
    size_t nIterations(0);
    while (true)
    {
      // debug
      std::cout << "\n=== iteration #" << nIterations << " ===\n";
      // end of debug
      
      // Recalculating potentials coefficients.
      calculate_potentials_coefs(rows, cols, C, u, v);
      
      // Recalculating potentials.
      calculate_potentials(u, v, C, P);
      
      // Searching cell with maximum potential.
      size_t maxPRow(0), maxPColumn(0);
      for (size_t r = 0; r < m; ++r)
        for (size_t c = 0; c < n; ++c)
          if (P(maxPRow, maxPColumn) < P(r, c))
          {
            maxPRow    = r;
            maxPColumn = c;
          }
      
      if (eq(x(maxPRow, maxPColumn), scalar_type(0.)))
      {
        // Found optimal plan. Interrupting.
        break;
      }
      ASSERT_EQ(x(maxPRow, maxPColumn), scalar_type(0.));
      
      // debug
      std::cout << "u: "; output_vector_console(std::cout, u);
      std::cout << "v: "; output_vector_console(std::cout, u);
      std::cout << "X: "; output_matrix_console(std::cout, x);
      std::cout << "P: "; output_matrix_console(std::cout, P);
      std::cout << "max P elem: (" << maxPRow << "," << maxPColumn << ")\n";
      // end of debug
      
      // Searching loop.
      std::vector<std::pair<size_t, size_t> > loopCellsIdxs;
      find_loop(rows, cols, maxPRow, maxPColumn, std::back_inserter(loopCellsIdxs));
      ASSERT_GE(loopCellsIdxs.size(), 4);
      ASSERT_LE(loopCellsIdxs.size(), m + n - 1);
      ASSERT_EQ(loopCellsIdxs.size() % 2, 0);
      
      // Locating cell from which shipment will be canceled on even subloop.
      std::pair<size_t, scalar_type> minShipment = std::make_pair(2, x(loopCellsIdxs[2].first, loopCellsIdxs[2].second));
      for (size_t i = 4; i < loopCellsIdxs.size(); i += 2)
      {
        scalar_type const curShipment = x(loopCellsIdxs[i].first, loopCellsIdxs[i].second);
        ASSERT_FUZZY_NEQ(curShipment, 0);
        if (curShipment < minShipment.second)
        {
          minShipment.first = i;
          minShipment.second = curShipment;
        }
      }
      
      // Minimum shipment on loop.
      size_t const minShipmentR = loopCellsIdxs[minShipment.first].first;
      size_t const minShipmentC = loopCellsIdxs[minShipment.first].second;
      
      scalar_type const shipmentValue = minShipment.second;
      
      // Removing on even subloop.
      for (size_t i = 0; i < loopCellsIdxs.size(); i += 2)
      {
        size_t const r = loopCellsIdxs[i].first;
        size_t const c = loopCellsIdxs[i].second;
        x(r, c) -= shipmentValue;
      }
      
      // Adding on odd subloop.
      for (size_t i = 1; i < loopCellsIdxs.size(); i += 2)
      {
        size_t const r = loopCellsIdxs[i].first;
        size_t const c = loopCellsIdxs[i].second;
        x(r, c) += shipmentValue;
      }
      
      // Asserting that new plan is a plan.
      ASSERT(is_plan(a, b, x));
      
      // Updating storage: removing minimum shipment variable and adding maximum potential variable.
      
      // Removing from storage.
      VERIFY_EQ(rows[minShipmentR].erase(minShipmentC), 1);
      VERIFY_EQ(cols[minShipmentC].erase(minShipmentR), 1);
      VERIFY_EQ(planCells.erase(std::make_pair(minShipmentR, minShipmentC)), 1);
      
      // Adding to storage.
      cell_ptr_type newCell(new cell_type(maxPRow, maxPColumn));
      VERIFY(planCells.insert(std::make_pair(std::make_pair(maxPRow, maxPColumn), newCell)).second);
      VERIFY(cols[maxPColumn].insert(std::make_pair(maxPRow, newCell)).second);
      VERIFY(rows[maxPRow].insert(std::make_pair(maxPColumn, newCell)).second);
      
      // debug
      ++nIterations;
      if (nIterations > nMaxIterations)
        std::cerr << "lp_potentials::solve(): too much iterations!" << std::endl; 
      // end of debug
    }
    
    // Copying result matrix.
    X() = x;
  }
} // End of namespace 'lp_potentials'.
} // End of namespace 'numeric'.

#endif // NUMERIC_POTENTIALS_ALG_HPP

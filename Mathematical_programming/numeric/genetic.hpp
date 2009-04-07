/*
 * genetic.hpp
 * Genetics algorithms.
 * Vladimir Rutsky <altsysrq@gmail.com>
 * 31.03.2009
 */

#ifndef NUMERIC_GENETIC_HPP
#define NUMERIC_GENETIC_HPP

#include "numeric_common.hpp"

#include <vector>
#include <deque>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/bind.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/optional.hpp>
#include <boost/next_prior.hpp>

namespace numeric
{
namespace genetic
{
  typedef boost::minstd_rand base_generator_type; // TODO
  
  template< class V >
  struct ParallelepipedonUniformGenerator
  {
  private:
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));
    
  public:
    typedef V vector_type;
    
  public:
    ParallelepipedonUniformGenerator( vector_type const &a, vector_type const &b )
      : a_(a)
      , b_(b)
      , rndGenerator_(42u)
    {
      BOOST_ASSERT(a_.size() == b_.size());
      BOOST_ASSERT(a_.size() > 0);
    }
    
    vector_type operator()() const
    {
      vector_type v(a_.size());
      
      for (size_t r = 0; r < v.size(); ++r)
      {
        BOOST_ASSERT(a_(r) <= b_(r));
      
        // TODO: Optimize.
        boost::uniform_real<> uni_dist(a_(r), b_(r));
        boost::variate_generator<base_generator_type &, boost::uniform_real<> > uni(rndGenerator_, uni_dist);

        v(r) = uni();
        
        BOOST_ASSERT(a_(r) <= v(r) && v(r) <= b_(r));
      }
      
      return v;
    }
    
  private:
    vector_type const a_, b_;
    
    mutable base_generator_type rndGenerator_;
  };
  
  struct LCCrossOver
  {
    LCCrossOver()
      : rndGenerator_(30u)
    {
    }
    
    template< class V >
    V operator()( V const &x, V const &y ) const
    {
      // TODO: Optimize.
      boost::uniform_real<> uni_dist(0.0, 1.0);
      boost::variate_generator<base_generator_type &, boost::uniform_real<> > uni(rndGenerator_, uni_dist);

      double const lambda = uni();
      
      return x * lambda + (1 - lambda) * y;
    }
    
  private:
    mutable base_generator_type rndGenerator_;
  };
  
  template< class Scalar >
  struct ParallelepipedonMutation
  {
    typedef Scalar scalar_type;
    
    template< class OffsetFwdIt >
    ParallelepipedonMutation( OffsetFwdIt first, OffsetFwdIt beyond )
      : rndGenerator_(30u)
    {
      deviations_.assign(first, beyond);
    }
    
    template< class V, class S >
    V operator()( V const &x, S const scale ) const
    {
      BOOST_ASSERT(deviations_.size() == x.size());
      
      V result(deviations_.size());
      
      // TODO: Optimize.
      for (size_t r = 0; r < deviations_.size(); ++r)
      {
        boost::uniform_real<> uni_dist(0.0, 1.0);
        boost::variate_generator<base_generator_type &, boost::uniform_real<> > uni(rndGenerator_, uni_dist);
  
        double const lambda = uni();
        
        result(r) = x(r) + deviations_[r] * lambda * scale;
      }
      
      return result;
    }
    
  private:
    std::vector<scalar_type>    deviations_;
    mutable base_generator_type rndGenerator_;
  };
  
  // TODO: Documentation.
  template< class Generator, class Crossover, class Mutation, class V, class Func, class FuncScalar, class PointsVecsOut >
  V vectorSpaceGeneticSearch( Generator generator, Crossover crossover, Mutation mutation, Func fitness,
                              size_t nIndividuals, double liveRate, 
                              typename V::value_type precision, size_t nPrecisionSelect,
                              PointsVecsOut selectedPointsVecsOut, PointsVecsOut notSelectedPointsVecsOut )
  {
    BOOST_CONCEPT_ASSERT((ublas::VectorExpressionConcept<V>));
    BOOST_CONCEPT_ASSERT((boost::UnaryFunction<Func, FuncScalar, V>));
    // TODO: Concept asserts for Generator and Crossover.
    
    typedef FuncScalar               function_scalar_type;
    typedef V                        vector_type;
    typedef typename V::value_type   value_type;
    typedef std::vector<vector_type> individuals_vector_type;
    
    BOOST_ASSERT(0 <= liveRate && liveRate <= 1);
    BOOST_ASSERT(nPrecisionSelect > 0);
    
    individuals_vector_type population;
    population.reserve(nIndividuals);
    individuals_vector_type nextPopulation;
    nextPopulation.reserve(nIndividuals);
    
    base_generator_type rndGenerator(57u);
    
    typedef std::deque<vector_type> fitted_individuals_deque_type;
    fitted_individuals_deque_type fittedIndividuals;
    
    // Spawning initial population.
    for (size_t i = 0; i < nIndividuals; ++i)
      population.push_back(generator());
    
    size_t iterations = 0;
    while (true)
    {
      // Sorting current population.
      std::sort(population.begin(), population.end(), 
                boost::bind(std::less<function_scalar_type>(), boost::bind(fitness, _1), boost::bind(fitness, _2)));
      size_t const nSelected = liveRate * nIndividuals;
      
      BOOST_ASSERT(nSelected != 0 && nSelected != nIndividuals);
      
      {
        // Outputting current population.
        individuals_vector_type selected;
        selected.reserve(nSelected);
        std::copy(population.begin(), boost::next(population.begin(), nSelected), std::back_inserter(selected));
        *selectedPointsVecsOut++ = selected;
        
        individuals_vector_type notSelected;
        notSelected.reserve(nIndividuals - nSelected);
        std::copy(boost::next(population.begin(), nSelected), boost::next(population.begin(), nIndividuals),
                  std::back_inserter(notSelected));
        *notSelectedPointsVecsOut++ = notSelected;
      }
      
      fittedIndividuals.push_front(population[0]);
      
      BOOST_ASSERT(nPrecisionSelect > 0);
      while (fittedIndividuals.size() > nPrecisionSelect)
        fittedIndividuals.pop_back();
      
      if (fittedIndividuals.size() == nPrecisionSelect)
      {
        // Checking is most fitted individual is changing in range of precision.
        
        vector_type const lastMostFittedIndividual = fittedIndividuals.front();
        bool satisfy(true);
        for (typename fitted_individuals_deque_type::const_iterator it = boost::next(fittedIndividuals.begin()); it != fittedIndividuals.end(); ++it)
        {
          value_type const dist = ublas::norm_2(lastMostFittedIndividual - *it);
          if (dist >= precision)
          {
            satisfy = false;
            break;
          }
        }
        
        
        if (satisfy)
        {
          // Evolved to population which meets precision requirements.
          return lastMostFittedIndividual;
        }
      }
      
      {
        // Generating next population.

        nextPopulation.resize(0);
        
        // Copying good individuals.
        std::copy(population.begin(), boost::next(population.begin(), nSelected), 
                  std::back_inserter(nextPopulation));
        BOOST_ASSERT(nextPopulation.size() == nSelected);
        
        // Crossover and mutation.
        for (size_t i = nSelected; i < nIndividuals; ++i)
        {
          // TODO: Optimize.
          boost::uniform_int<> uni_dist(0, nIndividuals - 1);
          boost::variate_generator<base_generator_type &, boost::uniform_int<> > uni(rndGenerator, uni_dist);
          
          size_t const xIdx = uni();
          size_t const yIdx = uni();
          BOOST_ASSERT(xIdx < population.size());
          BOOST_ASSERT(yIdx < population.size());

          // Crossover.
          vector_type const x = population[xIdx], y = population[yIdx];
          vector_type const child = crossover(x, y);
          
          // Mutation.
          vector_type const mutant = mutation(child, ublas::norm_2(x - y)); // TODO: Process may be unstable.
          
          nextPopulation.push_back(mutant);
        }
      }
      
      // Replacing old population.
      population.swap(nextPopulation);
      
      // debug, TODO
      ++iterations;
      if (iterations >= 1000)
      {
        std::cerr << "Too much iterations!\n";
        break;
      }
      // end of debug
    }
    
    return population[0];
  }
} // End of namespace 'genetic'.
} // End of namespace 'numeric'.

#endif // NUMERIC_GENETIC_HPP

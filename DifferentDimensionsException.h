/*! \file DifferentDimensionsException.h
 *  \brief The declaration and definition of the DifferentDimensionsException 
 *         exception class.
 *  \author Christos Nitsas
 *  \date 2012
 */


#ifndef DIFFERENT_DIMENSIONS_EXCEPTION_H
#define DIFFERENT_DIMENSIONS_EXCEPTION_H

#include <exception>


/*!
 *  \weakgroup ParetoApproximator Everything needed for the chord algorithm.
 *  @{
 */


//! The namespace containing everything needed for the chord algorithm.
namespace pareto_approximator {


/*! 
 *  \brief Exception thrown by Point::operator<(), Point::ratioDistance(),
 *         Hyperplane::ratioDistance() and Hyperplane::init().
 *
 *  An exception thrown when:
 *  - Two Point instances were supposed to be compared but they have 
 *    different dimensions. (they belong to spaces of different dimensions)
 *  - We were trying to compute the ratio distance between two Point 
 *    instances or a Point instance and a Hyperplane instance but they 
 *    had different dimensions. (they belong to spaces of different 
 *    dimensions)
 *  - We were trying to initialize a Hyperplane from a given set of 
 *    n Point instances but not all the Point instances had dimension n.
 *    (they belong to spaces of different dimensions)
 */
class DifferentDimensionsException : public std::exception
{
  public:
    //! Return a simple char* message.
    const char* what() const throw() 
    {
      return "The instances have different dimensions.";
    }
};


}  // namespace pareto_approximator


/*! @} */


#endif  // DIFFERENT_DIMENSIONS_EXCEPTION_H

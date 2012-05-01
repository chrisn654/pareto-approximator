/*! \file Hyperplane.h
 *  \brief The declaration of the Hyperplane class.
 *  \author Christos Nitsas
 *  \date 2012
 */


#ifndef HYPERPLANE_H
#define HYPERPLANE_H

#include <iostream>
#include <string>
#include <set>
#include <vector>

#include "Point.h"
#include "DifferentDimensionsException.h"
#include "SamePointsException.h"
#include "Not2DPointsException.h"
#include "Not2DHyperplanesException.h"
#include "NonExistentCoefficientException.h"
#include "ParallelHyperplanesException.h"


/*!
 *  \weakgroup ParetoApproximator Everything needed for the chord algorithm.
 *  @{
 */


//! The namespace containing everything needed for the chord algorithm.
namespace pareto_approximator {


//! A class representing a hyperplane on an n-dimensional space.
/*!
 *  A hyperplane on an n-dimensional space can be described by an equation 
 *  of the form:
 *  \f$ a_{1} x_{1} + a_{2} x_{2} + ... + a_{n} x_{n} = b \f$
 *  
 *  We use the a_{i} coefficients and b to represent the hyperplane.
 *  
 *  \sa Hyperplane() and ~Hyperplane()
 */
class Hyperplane
{
  public:
    //! Random access iterator (to the a_{i} coefficients).
    typedef std::vector<double>::iterator iterator;
    
    //! Constant random access iterator (to the a_{i} coefficients).
    typedef std::vector<double>::const_iterator const_iterator;

    //! Constructor for a hyperplane on a 2D space. (a simple line)
    /*!
     *  \param a1 The first dimension's (x_{1}) coefficient.
     *  \param a2 The second dimension's (x_{2}) coefficient.
     *  \param b The right hand side of the hyperplane equation.
     *  
     *  Constructs the hyperplane described by equation:
     *  \f$ a_{1} x_{1} + a_{2} x_{2} = b \f$
     *  
     *  \sa Hyperplane
     */
    Hyperplane(double a1, double a2, double b);

    //! Constructor for a hyperplane on a 3D space. (a simple plane)
    /*!
     *  \param a1 The first dimension's (x_{1}) coefficient.
     *  \param a2 The second dimension's (x_{2}) coefficient.
     *  \param a3 The third dimension's (x_{3}) coefficient.
     *  \param b The right hand side of the hyperplane equation.
     *  
     *  Constructs the hyperplane described by equation:
     *  \f$ a_{1} x_{1} + a_{2} x_{2} + a_{3} x_{3} = b \f$
     *  
     *  \sa Hyperplane
     */
    Hyperplane(double a1, double a2, double a3, double b);

    //! Constructor for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator to the initial position in a std::vector<int>.
     *  \param last Iterator to the final position in a std::vector<int>.
     *  \param b The right hand side of the hyperplane equation.
     *  
     *  Constructs the hyperplane described by equation:
     *  \f$ a_{1} x_{1} + a_{2} x_{2} + ... + a_{n} x_{n} = b \f$
     *  
     *  The underlying vector should contain the hyperplane's coefficients, 
     *  ordered starting from a_{1} to a_{n}. The range used will be 
     *  [first, last), which includes all the elements between first and 
     *  last, including the element pointed by first but not the one pointed 
     *  by last.
     *  
     *  The resulting hyperplane's coefficients will be doubles, not ints.
     *  
     *  \sa Hyperplane
     */
    Hyperplane(std::vector<int>::const_iterator first, 
               std::vector<int>::const_iterator last, int b);

    //! Constructor for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator to the initial position in a std::vector<double>.
     *  \param last Iterator to the final position in a std::vector<double>.
     *  \param b The right hand side of the hyperplane equation.
     *  
     *  Constructs the hyperplane described by equation:
     *  \f$ a_{1} x_{1} + a_{2} x_{2} + ... + a_{n} x_{n} = b \f$
     *  
     *  The underlying vector should contain the hyperplane's coefficients, 
     *  ordered starting from a_{1} to a_{n}. The range used will be 
     *  [first, last), which includes all the elements between first and 
     *  last, including the element pointed by first but not the one pointed 
     *  by last.
     *  
     *  \sa Hyperplane
     */
    Hyperplane(std::vector<double>::const_iterator first, 
               std::vector<double>::const_iterator last, double b);

    //! Constructor for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator (pointer) to the initial position in an array 
     *               of int.
     *  \param last Iterator (pointer) to the final position in an array 
     *              of int. (the position just beyond the last element we want)
     *  \param b The right hand side of the hyperplane equation.
     *  
     *  Constructs the hyperplane described by equation:
     *  \f$ a_{1} x_{1} + a_{2} x_{2} + ... + a_{n} x_{n} = b \f$
     *  
     *  The underlying array should contain the hyperplane's coefficients, 
     *  ordered starting from a_{1} to a_{n}. The range used will be 
     *  [first, last), which includes all the elements between first and 
     *  last, including the element pointed by first but not the one pointed 
     *  by last.
     *  
     *  The resulting hyperplane's coefficients will be doubles, not ints.
     *  
     *  \sa Hyperplane
     */
    Hyperplane(const int * first, const int * last, int b);

    //! Constructor for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator (pointer) to the initial position in an array 
     *               of double.
     *  \param last Iterator (pointer) to the final position in an array of
     *              double. (the position just beyond the last element we want)
     *  \param b The right hand side of the hyperplane equation.
     *  
     *  Constructs the hyperplane described by equation:
     *  \f$ a_{1} x_{1} + a_{2} x_{2} + ... + a_{n} x_{n} = b \f$
     *  
     *  The underlying array should contain the hyperplane's coefficients, 
     *  ordered starting from a_{1} to a_{n}. The range used will be 
     *  [first, last), which includes all the elements between first and 
     *  last, including the element pointed by first but not the one pointed 
     *  by last.
     *  
     *  The resulting hyperplane's coefficients will be doubles, not ints.
     *  
     *  \sa Hyperplane
     */
    Hyperplane(const double * first, const double * last, double b);

    //! Constructor for a hyperplane on a 2D space. (line)
    /*!
     *  \param p1 A 2D Point instance.
     *  \param p2 A 2D Point instance.
     *  
     *  Constructs a 2-hyperplane (line) that passes through points p1 and p2.
     *  
     *  Possible exceptions:
     *  - May throw a SamePointsException exception if p1 and p2 represent 
     *    the same point.
     *  - May throw a Not2DPointsException exception if either p1 or p2 is 
     *    not a 2D point.
     *  
     *  \sa Hyperplane, init() and Point
     */
    Hyperplane(const Point & p1, const Point & p2);

    //! Constructor for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator to the first element in a std::set<Point>.
     *  \param last Iterator to the past-the-end element in a std::set<Point>.
     *  
     *  Let n be the number of elements in the std::vector<Point> that first
     *  and last refer to.
     *  
     *  Constructs an n-hyperplane that passes through all the points in the 
     *  std::set<Point> that first and last refer to.
     *  
     *  \sa Hyperplane, init() and Point
     */
    Hyperplane(std::set<Point>::const_iterator first, 
               std::set<Point>::const_iterator last);

    //! Constructor for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator (pointer) to the first element in an array 
     *               of Point instances.
     *  \param last Iterator (pointer) to the last element in an array of 
     *              Point instances.
     *  
     *  Let n be the number of elements in the array of Point instances 
     *  that first and last refer to.
     *  
     *  Constructs an n-hyperplane that passes through all the points in 
     *  array of Point instances that first and last refer to.
     *  
     *  \sa Hyperplane, init() and Point
     */
    Hyperplane(const Point * first, const Point * last);

    //! A simple (and empty) destructor.
    ~Hyperplane();

    //! Access operator for the hyperplane's coefficients (except b).
    /*!
     *  \param pos The position (in the coefficients vector) to access.
     *  \return The hyperplane's a_{pos+1} coefficient. Remember coefficients 
     *          are labeled a_{1}, a_{2}, ..., a_{n}.
     *  
     *  - May throw a NonExistentCoefficientException if the requested 
     *    coefficient does not exist (pos is out of bounds).
     *  
     *  \sa Hyperplane
     */
    double operator[](unsigned int pos) const;

    /*!
     *  \brief Return the Hyperplane instance's b coefficient. (the hyperplane 
     *         equation's right hand side)
     */
    double b() const;

    //! Return the dimension of the space the hyperplane lives in.
    unsigned int space_dimension() const;

    //! Return iterator to beginning of the vector of a_{i} coefficients.
    /*!
     *  \return An iterator referring to the first of the a_{i} coefficients.
     *  
     *  \sa Hyperplane
     */
    iterator begin();
    
    //! Return const iterator to beginning of the vector of a_{i} coefficients.
    /*!
     *  \return An iterator referring to the first of the a_{i} coefficients.
     *  
     *  \sa Hyperplane
     */
    const_iterator begin() const;

    //! Return iterator to end of the vector of a_{i} coefficients.
    /*!
     *  \return An iterator pointing just after the last a_{i} coefficient.
     *  
     *  \sa Hyperplane
     */
    iterator end();

    //! Return const iterator to end of the vector of a_{i} coefficients.
    /*!
     *  \return An iterator pointing just after the last a_{i} coefficient.
     *  
     *  \sa Hyperplane
     */
    const_iterator end() const;

    //! Get the hyperplane's equation in a string.
    /*!
     *  \return A std::string with the hyperplane's equation in parentheses.
     *  
     *  Examples:
     *  - ( 2.2 x1 + 5.4 x2 - 1.7 x3 = 9.2 )
     *  - ( 1.3 x1 - 6.7 x2 + 0.0 x3 + 0.0 x4 - 1.0 x5 = 10.1 )
     *  
     *  \sa Hyperplane
     */
    std::string str() const;

    //! The Hyperplane equality operator.
    /*!
     *  \param hyperplane A Hyperplane instance we want to compare with 
     *                    the current instance.
     *  \return true if the two instances represent the same hyperplane, 
     *          false otherwise.
     *  
     *  \sa Hyperplane and operator!=()
     */
    bool operator== (const Hyperplane & hyperplane) const;

    //! The Hyperplane inequality operator.
    /*!
     *  \param hyperplane A Hyperplane instance we want to compare with 
     *                    the current instance.
     *  \return true if the two instances represent different hyperplanes, 
     *          false otherwise.
     *  
     *  \sa Hyperplane and operator==()
     */
    bool operator!= (const Hyperplane & hyperplane) const;

    //! The Hyperplane output stream operator.
    /*!
     *  Output the hyperplane's equation in parentheses.
     *  
     *  Examples:
     *  - ( 2.2 x1 + 5.4 x2 - 1.7 x3 = 9.2 )
     *  - ( 1.3 x1 - 6.7 x2 + 0.0 x3 + 0.0 x4 - 1.0 x5 = 10.1 )
     *  
     *  \sa Hyperplane and str()
     */
    friend std::ostream & operator<< (std::ostream & out, 
                                     const Hyperplane & hyperplane);

    //! Compute the ratio distance from the given Point to the hyperplane.
    /*!
     *  \param p A Point instance.
     *  \return The ratio distance from p to the hyperplane.
     *  
     *  The ratio distance from a point p to a hyperplane H is defined as:
     *  \f$ RD(p, H) = \min_{q \in H} RD(p, q) \f$, where q is a point on H.
     *  The ratio distance from a point p to a point q is defined as:
     *  \f$ RD(p, q) = \max\{ \max_{i}\{(q_{i} - p_{i}) / p_{i}\}, 0.0 \} \f$.
     *  
     *  Intuitively it is the minimum value of \f$ \epsilon \ge 0 \f$ such 
     *  that some point on H \f$ \epsilon -covers p \f$.
     *  
     *  Possible exceptions:
     *  - May throw a DifferentDimensionsException exception if the given point 
     *    and the hyperplane belong in spaces of different dimensions.
     *  
     *  \sa Hyperplane and Point
     */
    double ratioDistance(const Point & p) const;
    
    //! Create a new Hyperplane parallel to the current one (through a point).
    /*!
     *  \param p A Point instance through which the new Hyperplane instance 
     *           must pass.
     *  \return A new Hyperplane instance, parallel to the current one and 
     *          passing through p.
     *  
     *  The new hyperplane will have the same a_{i} coefficients but a different 
     *  b coefficient, one that satisfies the equation:
     *  \f$ a_{1} * p_{1} + a_{2} * p_{2} + ... + a_{n} * p_{n} = b \f$.
     *  
     *  \sa Hyperplane and Point
     */
    Hyperplane parallelThrough(const Point & p) const;

    //! Check if two hyperplanes are parallel.
    /*!
     *  \param hyperplane A Hyperplane instance.
     *  \return true if the given hyperplane instance is parallel to the current 
     *          one; false otherwise.
     *  
     *  We should check if the two instances have the same a_{i} coefficients. The 
     *  only problem is that the a_{i} and b coefficients of one instance might 
     *  be scaled:
     *  Let h1 and h2 be two parallel hyperplanes. Scale h2's a_{i}'s and b by a 
     *  constant c. Its slope doesn't change, that is h1 and h2 remain parallel.
     *  
     *  To overcome this problem, we scale each hyperplane's coefficients by the 
     *  opposite hyperplane's a_{1} and expect equal a_{i}'s.
     *  
     *  \sa Hyperplane
     */
    bool isParallel(const Hyperplane & hyperplane) const;

    //! Find the point where two 2-hyperplanes (lines) intersect.
    /*!
     *  \param hyperplane A Hyperplane instance.
     *  \return A Point instance that represents the point where the given  
     *          hyperplane instance intersects with the current one.
     *  
     *  Possible exceptions:
     *  - May throw a Not2DHyperplanesException exception if either of the 
     *    two Hyperplane instances is not a 2-hyperplane (line).
     *  - May throw a ParallelHyperplanesException exception if the two 
     *    hyperplanes are parallel (or the same).
     *  
     *  \sa Hyperplane and Point
     */
    Point intersection(const Hyperplane & hyperplane) const;

  private:
    //! Initializer for a hyperplane on an n-dimensional space.
    /*!
     *  \param first Iterator to the first element in a std::set<Point>.
     *  \param last Iterator to the past-the-end element in a std::set<Point>.
     *  
     *  Let n be the number of elements in the std::vector<Point> that first
     *  and last refer to.
     *  
     *  Initializes the current instance to an n-hyperplane that passes 
     *  through all the points in the std::set<Point> that first and last 
     *  refer to.
     *  
     *  \sa Hyperplane and Point
     */
    void init(std::set<Point>::const_iterator first, 
              std::set<Point>::const_iterator last);

    //! A std::vector of all the a_{i} coefficients.
    /*!
     *  \sa Hyperplane
     */
    std::vector<double> coefficients_;

    //! The right hand side of the hyperplane equation.
    /*!
     *  \sa Hyperplane
     */
    double b_;
};


}  // namespace pareto_approximator


/* @} */


#endif  // HYPERPLANE_H

/*! \file Facet.cpp
 *  \brief The implementation of the Facet class.
 *  \author Christos Nitsas
 *  \date 2012
 *  
 *  Won't `#include` "Facet.h". In fact "Facet.h" will `#include` 
 *  "Facet.cpp" because it describes a class template (which doesn't allow 
 *  us to split declaration from definition).
 */


#include <assert.h>
#include <iterator>
#include <algorithm>
#include <armadillo>


/*!
 *  \weakgroup ParetoApproximator Everything needed for the chord algorithm.
 *  @{
 */


//! The namespace containing everything needed for the chord algorithm.
namespace pareto_approximator {


//! A Facet constructor.
/*!
 *  \param firstVertex An iterator to the first of the facet vertices.
 *  \param lastVertex An iterator to the past-the-end element in the 
 *                    container of facet vertices.
 *  \param preferPositiveNormalVector While computing the facet's normal 
 *                    vector prefer the all-positive one (if it exists).
 *  
 *  Vertices cannot be null PointAndSolution<S> instances and the 
 *  contained points cannot be null Point instances. 
 *  
 *  Initializes:
 *  - Facet<S>::vertices_ to the sequence of vertices pointed to by 
 *    firstVertex and lastVertex. The range used is 
 *    [firstVertex, lastVertex).
 *  - Calculates the hyperplane passing through the facet's vertices 
 *    and uses its normal vector as the facet's normal vector 
 *    (Facet<S>::normal_). For each set of n vertices there are two 
 *    different n-hyperplanes passing through them with opposite normal 
 *    vectors. This constructor will prefer the all-positive normal 
 *    vector (if one exists) if preferPositiveNormalVector is set to 
 *    true; otherwise it will choose one depending on the order of the 
 *    vertices.
 *  - Facet<S>::approximationErrorUpperBound_ to the ratio distance 
 *    between the Facet and its Lower Distal Point (LDP). Calculates 
 *    both the LDP and the ratio distance.
 *  
 *  Possible exceptions:
 *  - May throw a DifferentDimensionsException exception if the given 
 *    vertices have different dimensions.
 *  - May throw a NullObjectException if some of the given vertices or 
 *    some of the points contained in them are null instances (null 
 *    PointAndSolution<S> and null Point instances respectively).
 *  
 *  \sa Facet
 */
template <class S> 
Facet<S>::Facet(
      typename std::vector< PointAndSolution<S> >::const_iterator firstVertex, 
      typename std::vector< PointAndSolution<S> >::const_iterator lastVertex, 
      bool preferPositiveNormalVector)
{
  spaceDimension_ = firstVertex->dimension();

  // Only accept simplicial facets for now. 
  // - A facet is a simplicial facet if it consists of exactly d vertices, 
  //   where d is the dimension of the space that the facet lives in.
  assert(std::distance(firstVertex, lastVertex) == spaceDimension_);

  // Make sure that all the given vertices are valid.
  // - They all have the correct dimension. (i.e. the dimension of the space 
  //   that the facet lives in)
  // - They (and the points they contain) are not null instances.
  ConstVertexIterator cvi;
  for (cvi = firstVertex; cvi != lastVertex; ++cvi) {
    if (cvi->isNull() or cvi->point.isNull())
      throw NullObjectException();
    if (cvi->dimension() != spaceDimension_)
      throw DifferentDimensionsException();
  }

  // First fill-in Facet<S>::vertices_.
  vertices_.assign(firstVertex, lastVertex);

  // Compute and set the facet's normal vector (Facet<S>::normal_).
  computeAndSetFacetNormal(preferPositiveNormalVector);

  // Compute and set the facet's localApproximationErrorUpperBound_ and 
  // isBoundaryFacet_ attributes.
  computeAndSetLocalApproximationErrorUpperBoundAndIsBoundaryFacet();
}


//! Facet constructor.
/*!
 *  \param firstVertex An iterator to the first of the facet vertices.
 *  \param lastVertex An iterator to the past-the-end element in the 
 *                    container of facet vertices.
 *  \param firstElemOfFacetNormal An iterator to the first element of 
 *                                the facet's normal.
 *  \param lastElemOfFacetNormal An iterator to the past-the-end 
 *                               element in the container of facet 
 *                               vertices.
 *  
 *  Vertices cannot be null PointAndSolution<S> instances and the 
 *  contained points cannot be null Point instances. 
 *  
 *  Initializes:
 *  - Facet<S>::vertices_ to the sequence of vertices pointed to by 
 *    firstVertex and lastVertex. The range used is 
 *    [firstVertex, lastVertex).
 *  - Facet<S>::normal_ to the sequence of elements pointed to by 
 *    firstElemOfFacetNormal and lastElemOfFacetNormal. The range 
 *    used is [firstElemOfFacetNormal, lastElemOfFacetNormal).
 *  - Facet<S>::approximationErrorUpperBound_ to the ratio distance between 
 *    the Facet and its Lower Distal Point (LDP). Calculates both the 
 *    LDP and the ratio distance.
 *  
 *  We do not verify that the given normal vector is indeed correct, 
 *  i.e. it agrees with the given vertices. The responsibility lies with 
 *  the caller.
 *  
 *  Possible exceptions:
 *  - May throw a DifferentDimensionsException exception if the given 
 *    vertices have different dimensions.
 *  - May throw a NullObjectException if some of the given vertices or 
 *    some of the points contained in them are null instances (null 
 *    PointAndSolution<S> and null Point instances respectively).
 *  
 *  \sa Facet
 */
template <class S> 
Facet<S>::Facet(
      typename std::vector< PointAndSolution<S> >::const_iterator firstVertex, 
      typename std::vector< PointAndSolution<S> >::const_iterator lastVertex, 
      std::vector<double>::const_iterator firstElemOfFacetNormal, 
      std::vector<double>::const_iterator lastElemOfFacetNormal)
{
  spaceDimension_ = std::distance(firstElemOfFacetNormal, 
                                  lastElemOfFacetNormal);

  // Only accept simplicial facets for now. 
  // - A facet is a simplicial facet if it consists of exactly d vertices, 
  //   where d is the dimension of the space that the facet lives in.
  assert(std::distance(firstVertex, lastVertex) == spaceDimension_);

  // Make sure that all the given vertices are valid.
  // - They all have the correct dimension. (i.e. the dimension of the space 
  //   that the facet lives in)
  // - They (and the points they contain) are not null instances.
  ConstVertexIterator cvi;
  for (cvi = firstVertex; cvi != lastVertex; ++cvi) {
    if (cvi->isNull() or cvi->point.isNull())
      throw NullObjectException();
    if (cvi->dimension() != spaceDimension_)
      throw DifferentDimensionsException();
  }

  // First fill-in Facet<S>::vertices_ and Facet<S>::normal_.
  vertices_.assign(firstVertex, lastVertex);
  normal_.assign(firstElemOfFacetNormal, lastElemOfFacetNormal);

  // Compute and set the facet's localApproximationErrorUpperBound_ and 
  // isBoundaryFacet_ attributes.
  computeAndSetLocalApproximationErrorUpperBoundAndIsBoundaryFacet();
}


//! A simple (and empty) destructor.
template <class S> 
Facet<S>::~Facet() { }


//! Return the dimension of the space that the facet lives in.
/*!
 *  \sa Facet
 */
template <class S> 
unsigned int 
Facet<S>::spaceDimension() const
{
  return spaceDimension_;
}


//! Is the facet a boundary facet?
/*!
 *  \return true if the facet is a boundary facet; false otherwise.
 *  
 *  We call a facet a boundary facet if it does not have a Lower Distal 
 *  Point. (The hyperplanes h_{i} associated with its vertices v_{i} 
 *  do not intersect in a unique point. Check the documentation of Facet 
 *  for more info.)
 *  
 *  \sa Facet and Facet<S>::computeLowerDistalPoint()
 */
template <class S> 
bool 
Facet<S>::isBoundaryFacet() const
{
  return isBoundaryFacet_;
}


//! Get an upper bound to the current facet's approximation error.
/*! 
 *  \return the localApproximationErrorUpperBound attribute
 *  
 *  We will use the ratio distance from the facet to it's Lower Distal 
 *  Point as an upper bound to the local approximation error.
 *  
 *  Check the documentation for Facet for a description of what a Lower 
 *  Distal Point is.
 *  
 *  Possible exceptions:
 *  - May throw a BoundaryFacetException if the facet is a 
 *    boundary facet.
 *  
 *  \sa Facet and Facet<S>::computeLowerDistalPoint()
 */
template <class S> 
double 
Facet<S>::getLocalApproximationErrorUpperBound() const
{
  if (isBoundaryFacet())
    throw BoundaryFacetException();
  // else

  return localApproximationErrorUpperBound_;
}


//! Return iterator to the beginning of the vector of facet vertices.
/*! 
 *  \return An iterator pointing to the first vertex in the vector 
 *          of vertices.
 *  
 *  \sa Facet
 */
template <class S> 
typename Facet<S>::ConstVertexIterator 
Facet<S>::beginVertex() const
{
  return vertices_.begin();
}


//! Return iterator to the end of the vector of facet vertices.
/*! 
 *  \return An iterator pointing just after the last vertex in the 
 *          vector of vertices.
 *  
 *  \sa Facet
 */
template <class S> 
typename Facet<S>::ConstVertexIterator 
Facet<S>::endVertex() const
{
  return vertices_.end();
}


//! Return iterator to the beginning of the facet's normal vector.
/*! 
 *  \return An iterator pointing to the first element in the facet's 
 *          normal vector.
 *  
 *  \sa Facet
 */
template <class S> 
typename Facet<S>::ConstFacetNormalIterator 
Facet<S>::beginFacetNormal() const
{
  return normal_.begin();
}


//! Return iterator to the end of the facet's normal vector.
/*! 
 *  \return An iterator pointing just after the last element in the 
 *          facet's normal vector.
 *  
 *  \sa Facet
 */
template <class S> 
typename Facet<S>::ConstFacetNormalIterator 
Facet<S>::endFacetNormal() const
{
  return normal_.end();
}


/*!
 * \brief Compute the mean of all the weight vectors of the facet's 
 *        vertices.
 *
 *  \return A weight vector W of size this->spaceDimension(). Each 
 *          element W_{j} is the mean of all w_{ij}'s, where w_{i} is 
 *          the weight vector inside the facet's i'th vertex's 
 *          PointAndSolution object (PointAndSolution::weightsUsed).
 *  
 *  \sa Facet
 */
template <class S> 
std::vector<double> 
Facet<S>::computeMeanVertexWeights() const
{
  ConstVertexIterator cvi;
  std::vector<double> meanWeights(spaceDimension(), 0.0);
  for (unsigned int i = 0; i != spaceDimension(); ++i) {
    for (cvi = beginVertex(); cvi != endVertex(); ++cvi)
      meanWeights[i] += cvi->weightsUsed[i];
    meanWeights[i] /= spaceDimension();
  }

  return meanWeights;
}


//! Compute the facet's Lower Distal Point (LDP).
/*! 
 *  \return The facet's Lower Distal Point (Point instance) if one 
 *          exists, a null Point instance (i.e. a Point instance whose 
 *          Point::isNull() method returns true) otherwise. 
 *  
 *  Solves (for x) the system of k equations of the form:
 *  \f$ w_{i1} * x_{1} + ... + w_{ik} * x_{k} = w_{i} \dot v_{i} \f$, 
 *  where w_{i} is the weight vector associated with the i'th vertex of 
 *  the facet (the normal of the associated lower-bound hyperplane) and 
 *  v_{i} is the vector of the i'th vectex's coordinates. 
 *  
 *  (\f$ w_{i} \dot v_{i} = b_{i} \f$ is the associated hyperplane's offset 
 *  from the origin)
 *  
 *  The solution, if one (and only one) exists, will be the LDP's coordinates.
 *  If a unique solution does not exist we return a null Point instance 
 *  (i.e. a Point instance whose Point::isNull() method returns true).
 *  
 *  What is the Lower Distal Point (LDP)? Recall that hyperplanes through 
 *  the Pareto Set points with normal vectors equal to the weight vector 
 *  yielding that point are lower bounds of the Pareto Set. 
 *  Let's call the facet's vertices v_{i} and the hyperplane assosiated 
 *  with each vertex h_{i}.
 *  The LDP is the point where the current facet's h_{i}'s intersect, 
 *  provided these N hyperplanes intersect in a unique point.
 *  Intuitively, the LDP is the most distant possible point we might 
 *  generate using the current facet's normal as weights.
 *  
 *  LDP Example:
 *  The LDP in 3 dimensions is the top of the pyramid whose base is the 
 *  current facet. Each of the pyramid's sides lies on a hyperplane 
 *  h_{i}, where h_{i} is the hyperplane used to find the facet's vertex 
 *  v_{i}. (We can recreate h_{i} using v_{i}'s weight vector.)
 *  
 *  The h_{i}'s might not intersect in a unique point. In that case, 
 *  this method returns a null Point instance (i.e. a Point instance 
 *  Point::isNull() method returns true) and the current facet is 
 *  treated as a boundary facet.
 *  
 *  \sa Facet
 */
template <class S> 
Point 
Facet<S>::computeLowerDistalPoint() const
{
  // open a stream to /dev/null (will redirect error messages there)
  // - will redirect error messages there
  std::ofstream f("/dev/null");
  // redirect armadillo error messages to /dev/null
  // - e.g. when arma::solve() finds no solutions
  // - we check arma::solve()'s return value for errors, 
  //   no need for error messages
  arma::set_stream_err2(f);

  arma::mat W;
  arma::vec b;

  ConstVertexIterator cvi;
  // fill in matrix W and vector b
  for (cvi = beginVertex(); cvi != endVertex(); ++cvi) {
    // make sure the weightsUsed field of the current vertex is not empty
    assert(cvi->weightsUsed.size() == spaceDimension());

    arma::rowvec wi(cvi->weightsUsed);
    // fill in row i of the weight (hyperplane-normal) matrix
    W.insert_rows(W.n_rows, wi);
    // fill in element i of the hyperplane-offsets vector
    b.insert_rows(b.n_rows, wi * cvi->point.toVec());
  }
  
  arma::vec x;
  bool hasSolution = arma::solve(x, W, b);
  if (hasSolution) 
    // unique solution
    // - return it as a Point instance
    return Point(x.begin(), x.end());
  else 
    // either no solution or an infinite number of solutions 
    // - return a null Point instance 
    return Point();
}


/*!
 *  \brief Compute a point's ratio distance from the hyperplane that 
 *         the facet lies on.
 *  
 *  \param p A Point instance. (stricty positive)
 *  \return The point's ratio distance from the hyperplane on which the 
 *          facet lies.
 *  
 *  The ratio distance from a point p to a hyperplane H is defined as:
 *  \f$ RD(p, H) = \min_{q \in H} RD(p, q) \f$, where q is a point on H.
 *  The ratio distance from a point p to a point q is defined as:
 *  \f$ RD(p, q) = \max\{ \max_{i}\{(q_{i} - p_{i}) / p_{i}\}, 0.0 \} \f$.
 *  
 *  Intuitively it is the minimum value of \f$ \epsilon \ge 0 \f$ such 
 *  that some point on H \f$ \epsilon -covers p \f$.
 *  
 *  In order for the ratio distance to make sense point p must be 
 *  strictly positive, i.e. \f$ p_{i} > 0.0 \f$ must hold for all i.
 *  
 *  Possible exceptions:
 *  - May throw a DifferentDimensionsException exception if the given point 
 *    and the hyperplane belong in spaces of different dimensions.
 *  - May throw an InfiniteRatioDistanceException exception if the given 
 *    point's coordinate vector is perpendicular to the facet's 
 *    normal vector. Multiplying the point by a constant moves it in 
 *    a direction parallel to the facet's supporting hyperplane.
 *  - May throw a NotStrictlyPositivePointException exception if the 
 *    given point is not strictly positive.
 *  - May throw a NullObjectException exception if the given Point 
 *    instance is a null Point instance.
 *  
 *  \sa Point and Facet
 */
template <class S> 
double 
Facet<S>::ratioDistance(const Point & p) const
{
  if (p.isNull())
    throw NullObjectException();
  if (spaceDimension() != p.dimension())
    throw DifferentDimensionsException();
  if (not p.isStrictlyPositive()) 
    throw NotStrictlyPositivePointException();
  // else

  assert(spaceDimension() > 0);

  Point aPointOnTheFacet = vertices_[0].point;
  double dotProduct  = 0.0;
  double facetOffset = 0.0;      // the facet's offset from the origin
  for (unsigned int i=0; i!=spaceDimension(); ++i) {
    dotProduct  += normal_[i] * p[i];
    facetOffset += normal_[i] * aPointOnTheFacet[i];
  }

  double result;
  if (dotProduct == facetOffset)
    // the point is on the facet
    // it's okay even if dotProduct == 0.0
    result = 0.0;
  else if (dotProduct == 0.0)
    // multiplying the point by a constant moves it in a direction 
    // parallel to the hyperplane
    throw InfiniteRatioDistanceException();
  else
    result = std::max( (facetOffset - dotProduct) / dotProduct, 0.0 );

  return result;
}


//! Check if every element of the facet's normal vector is non-positive.
/*!
 *  \return true if every element of the facet's normal vector 
 *          (Facet<S>::normal_) is non-positive.
 *  
 *  Each element must be non-positive.
 *  
 *  \sa Facet
 */
template <class S> 
bool 
Facet<S>::hasAllNormalVectorElementsNonPositive() const
{
  for (unsigned int i = 0; i != spaceDimension(); ++i)
    if (normal_[i] > 0.0)
      return false;

  return true;
}


//! Check if every element of the facet's normal vector is non-negative.
/*!
 *  \return true if every element of the facet's normal vector 
 *          (Facet<S>::normal_) is non-negative.
 *  
 *  Each element must be non-negative.
 *  
 *  \sa Facet
 */
template <class S> 
bool 
Facet<S>::hasAllNormalVectorElementsNonNegative() const
{
  for (unsigned int i = 0; i != spaceDimension(); ++i)
    if (normal_[i] < 0.0)
      return false;

  return true;
}


//! Normalizes the facet's normal vector.
/*!
 *  Normalizes the facet's normal vector so that its magnitude 
 *  (i.e. length or L2-norm) becomes 1. 
 *  
 *  First computes "l2Norm", which is the current L2-norm of the 
 *  normal vector. Then divides each normal vector element by "l2Norm".
 *  
 *  \sa Facet
 */
template <class S> 
void 
Facet<S>::normalizeNormalVector()
{
  // get the facet's normal vector inside an armadillo vec
  arma::vec normalVec(normal_);

  // compute its L2-norm
  double l2Norm = arma::norm(normalVec, 2);

  // divide each normal vector element by "l2Norm"
  for (unsigned int i = 0; i != spaceDimension(); ++i)
    normal_[i] /= l2Norm;
}


//! Get a copy of the facet's normal vector.
/*!
 *  \return A copy of the facet's normal vector.
 *
 *  \sa Facet
 */
template <class S> 
std::vector<double> 
Facet<S>::getNormalVector() const
{
  return normal_;
}


//! Compute (and set) the facet's normal vector using the facet's vertices.
/*!
 *  \param preferPositiveNormalVector Should we prefer the all-positive 
 *                                    normal vector (if it exists)?
 *  
 *  Calculates the hyperplane passing through the facet's vertices 
 *  and uses its normal vector as the facet's normal vector. 
 *  
 *  For each set of n vertices there are two different n-hyperplanes passing 
 *  through them with opposite normal vectors. This method will prefer the 
 *  all-positive normal vector (if one exists) if preferPositiveNormalVector 
 *  is set to true; otherwise it will choose one depending on the order of 
 *  the facet vertices.
 *  
 *  \sa Facet
 */
template <class S> 
void 
Facet<S>::computeAndSetFacetNormal(bool preferPositiveNormalVector) 
{
  // fill a matrix will each point's coordinates
  arma::mat M;
  ConstVertexIterator vi;
  for (vi = vertices_.begin(); vi != vertices_.end(); ++vi)
    M.insert_rows(M.n_rows, vi->point.toRowVec());
  // add a column of ones at the end (will make the following easier)
  M.insert_cols(M.n_cols, arma::ones<arma::vec>(spaceDimension()));
  
  // fill in the normal vector's elements
  for (unsigned int i = 0; i != spaceDimension(); ++i) {
    M.swap_cols(i, M.n_cols - 1);
    normal_.push_back(arma::det(M.cols(0, M.n_cols - 2)));
    M.swap_cols(i, M.n_cols - 1);
  }

  if (preferPositiveNormalVector && hasAllNormalVectorElementsNonPositive())
    reverseNormalVectorSign();
}


/*! \brief Compute (and set) the facet's isBoundaryFacet_ and 
 *         localApproximationErrorUpperBound_ attributes.
 *  
 *  Computes the facet's local approximation error upper bound (i.e. 
 *  distance from the facet's Lower Distal Point if (a unique) one 
 *  exists) and sets the facet's localApproximationErrorUpperBound_ 
 *  and isBoundaryFacet_ attributes accordingly.
 *  
 *  We have only created this function to erase duplicate code from 
 *  inside the constructors.
 *  
 *  \sa Facet
 */
template <class S> 
void 
Facet<S>::computeAndSetLocalApproximationErrorUpperBoundAndIsBoundaryFacet()
{
  // - First find the facet's Lower Distal Point (LDP).
  Point lowerDistalPoint = computeLowerDistalPoint();

  // - If an LDP exists use it to compute the facet's local approximation 
  //   error, else mark the facet as a boundary facet.
  if (not lowerDistalPoint.isNull()) {
    isBoundaryFacet_ = false;
    if (lowerDistalPoint.isStrictlyPositive()) 
      localApproximationErrorUpperBound_ = ratioDistance(lowerDistalPoint);
    else {
      // The LDP is not strictly positive.
      // - mark the facet as a boundary facet
      isBoundaryFacet_ = true;
      // localApproximationErrorUpperBound_ is not valid now:
      localApproximationErrorUpperBound_ = -1.0;
    }
  }
  else {
    isBoundaryFacet_ = true;
    // localApproximationErrorUpperBound_ is not valid now:
    localApproximationErrorUpperBound_ = -2.0;
  }
}


//! Reverse the sign of all elements of the facet's normal vector.
/*!
 *  Reverse the sign of all the elements of the facet's normal vector 
 *  (Facet<S>::normal_).
 *  
 *  \sa Facet
 */
template <class S> 
void 
Facet<S>::reverseNormalVectorSign()
{
  for (unsigned int i = 0; i != spaceDimension(); ++i)
    normal_[i] = -normal_[i];
}


}  // namespace pareto_approximator


/* @} */
/*! \file FloodVisitor.cpp
 *  \brief The implementation of the FloodVisitor class.
 *  \author Christos Nitsas
 *  \date 2012
 */


#include <limits>

#include "tripleobjective_shortest_path_example_common.h"
#include "FloodVisitor.h"


using pareto_approximator::Point;
using pareto_approximator::NonDominatedSet;


/*!
 *  \weakgroup TripleobjectiveShortestPathExample
 *  An example tripleobjective shortest path problem.
 *  
 *  @{
 */


namespace tripleobjective_shortest_path_example {


//! A simple constructor.
/*!
 *  \param source The source vertex of the graph.
 *  \param target The target vertex of the graph.
 *  \param numVertices The number of vertices in the graph.
 */
FloodVisitor::FloodVisitor(const Vertex & source, 
                           const Vertex & target, 
                           unsigned int numVertices) : source_(source), 
                                                       target_(target)
{
  vertexDistances_.assign(numVertices, NonDominatedSet<Point>());
}


//! Initialize a vertex's vertexDistances_.
/*!
 *  \param u A vertex of the graph.
 *  \param g The graph.
 *  
 *  \sa FloodVisitor and FloodVisitor::vertexDistances_.
 */
void 
FloodVisitor::initializeVertex(const Vertex & u, const Graph &)
{
  if (u == source_)
    vertexDistances_[u].insert(Point(0.0, 0.0, 0.0));
  else
    vertexDistances_[u].insert(Point(std::numeric_limits<double>::max(), 
                                     std::numeric_limits<double>::max(), 
                                     std::numeric_limits<double>::max()));
}


//! Broadcast distances over edge.
/*!
 *  \param e An edge of the graph.
 *  \param g The graph.
 *  \return true if some of v's distances actually needed an update;
 *          false if nothing changed.
 *  
 *  Let's call e's source u and e's target v. Update v's distances 
 *  using u's distances plus e's weights.
 *  
 *  \sa FloodVisitor, FloodVisitor::vertexDistances_ and EdgeProperty.
 */
bool 
FloodVisitor::broadcastDistances(const Edge & e, const Graph & g)
{
  Vertex u = boost::source(e, g);
  Vertex v = boost::target(e, g);
  Point edgeWeight(g[e].black, g[e].red, g[e].green);

  bool insertedNewDistance = false;
  NonDominatedSet<Point>::iterator udi;
  for (udi = vertexDistances_[u].begin(); 
       udi != vertexDistances_[u].end(); ++udi) {
    insertedNewDistance |= vertexDistances_[v].insert(*udi + edgeWeight);
  }

  return insertedNewDistance;
}


//! Get the exact Pareto set as recorded in the visitor.
/*!
 *  \return The exact Pareto set (as recorded in the visitor).
 *
 *  The Pareto set will be vertexDistances_[target_].
 */
NonDominatedSet<Point> 
FloodVisitor::getParetoPoints()
{
  return vertexDistances_[target_];
}


}  // namespace tripleobjective_shortest_path_example


/*!
 *  @}
 */

//! \file experiments/vs_namoa_star/experiments_vs_namoa_star_common.h
//! \brief Common classes and typedefs used throughout the (vs NAMOA\*) 
//!        experiments.
//! \author Christos Nitsas
//! \date 2013
//!


#ifndef EXPERIMENTS_VS_NAMOA_STAR_COMMON_H
#define EXPERIMENTS_VS_NAMOA_STAR_COMMON_H


#include <limits>
#include <vector>
#include <list>

#include <Structs/Graphs/dynamicGraph.h>
#include <Structs/Graphs/packedMemoryArrayImpl.h>
#include <Algorithms/ShortestPath/Multicriteria/multicriteriaDijkstra.h>

#include "../../Point.h"
#include "../../PointAndSolution.h"


namespace pa = pareto_approximator;


//! 
//! \defgroup ExperimentsVsNamoaStar Experiments vs the NAMOA Star algorithm.
//!
//! @{
//!


//! Everything needed for the experiments vs the NAMOA\* algorithm.
namespace experiments_vs_namoa_star {


//! \brief Used inside the Node class (by our A* implementation). 
//!
//! \sa Node, AStarDijkstra and AStarDijkstra::runQuery()
//!
enum InWhichList 
{ 
  //! The Node is inside no list.
  NO_LIST, 
  //! The Node is inside the list of CLOSED Nodes.
  OPEN_LIST, 
  //! The Node is inside the list of OPEN Nodes.
  CLOSED_LIST 
};


//! \brief A class representing the data that will be associated with each 
//!        graph node. 
//! 
//! Will use it with PGL's Packed-Memory graph.
//! 
//! \sa experiments_vs_namoa_star::Edge
//!
class Node : DefaultGraphItem
{
  public:
    //! Constructor. (initializes the node's attributes)
    Node() : inWhichList(NO_LIST), pred(NULL), 
             dist(std::numeric_limits<double>::infinity()), 
             fScore(std::numeric_limits<double>::infinity()), 
             succ(NULL), marked(false), heuristicList(2), // CHANGE-HERE
             secondary_pqitem(std::numeric_limits<unsigned int>::max()), 
             pqitem(std::numeric_limits<unsigned int>::max()), 
             timestamp(0), x(0), y(0), 
             heuristicValue(0.0) // temporary
    { }

    // Things needed for (the modified) PGL's A* implementation: 

    //! \brief Which list is the node in (open, closed or none). (for A\*)
    //!
    //! Used by our simple A* implementation for performance reasons. 
    //! Specifically:
    //! - so that we won't need to actually use a CLOSED list and
    //! - so that we won't have to exhaustively search the OPEN list's 
    //!   contents every time we want to know if it contains a specific node
    //! 
    //! This attribute's value is valid only if the node's timestamp 
    //! matches A*'s timestamp, i.e. A* has encountered this Node at 
    //! least once. This also happens for performance reasons (so that 
    //! A* does not need to initialize every node's attributes before 
    //! each query).
    //! 
    //! Note: We refer to OPEN and CLOSED as lists above, but CLOSED does 
    //!       not really exist and OPEN is actually a priority queue.
    //! 
    //! \sa InWhichList, AStarSearch and AStarSearch::runQuery()
    //!
    InWhichList inWhichList;

    //! \brief The node's predecessor in the shortest path tree. (for A\*)
    //!
    //! Only valid after our A\* implementation has been used.
    //!
    void * pred;

    //! \brief The node's distance from the source node. (for A\*)
    //!
    //! Only valid after our A\* implementation has been used.
    //!
    //! The edge weights (of the DIMACS graphs we will use) are all 
    //! unsigned ints but this is declared double because comb will 
    //! be changing edge weights to a weighted sum of edge costs which 
    //! could (and will, more often than not) NOT be an integer.
    //!
    double dist; 

    //! \brief The node's f score. (for A\*)
    //! 
    //! The sum of the node's distance from the source node (through 
    //! the path pred points to) plus the heuristic estimate to the target. 
    //!
    //! Only needed inside our A\* implementation. It is not meaningful 
    //! after the algorithm ends.
    //!
    //! The edge weights (of the DIMACS graphs we will use) are all 
    //! unsigned ints but this is declared double because comb will 
    //! be changing edge weights to a weighted sum of edge costs which 
    //! could (and will, more often than not) NOT be an integer.
    //!
    double fScore; 

    // Things needed for PGL's NAMOA\* implementation:

    //! \brief The node's successor. (for PGL's Bounded TC Heuristic, 
    //!        used in NAMOA\*)
    //!
    //! We will not need to use this in our experiment. NAMOA\* 
    //! (specifically the Bounded TC Heuristic we use with it) needs it 
    //! though.
    //!
    void * succ;
    
    //! \brief Is the node marked? (for PGL's Bounded TC Heuristic, 
    //!        used in NAMOA\*)
    //!
    //! We will not need to use this in our experiment. NAMOA\* 
    //! (specifically the Bounded TC Heuristic we use with it) needs it 
    //! though.
    //!
    bool marked;

    //! \brief The node's list of heuristic values - one for each objective.
    //!        (for PGL's NAMOA\*)
    //! 
    //! Only valid after PGL's NAMOA\* implementation has been used.
    //!
    CriteriaList heuristicList;

    //! \brief The node's list of labels. (for PGL's NAMOA\*)
    //!
    //! Only valid after PGL's NAMOA\* implementation has been used and then 
    //! we only need the target node's list of labels. (We will use it to 
    //! make the list of Pareto points.)
    //! 
    //! Each label is an object of type Label. The Label class is defined 
    //! inside 
    //! PGLROOT/Algorithms/ShortestPath/Multicriteria/multicriteriaDijkstra.h.
    //!
    std::vector<Label> labels;

    //! \brief Needed by the Bounded TC Heuristic.
    //!
    //! We will not need to use this in our experiment. NAMOA\* 
    //! (specifically the Bounded TC Heuristic we use with it) needs it 
    //! though.
    //!
    unsigned int secondary_pqitem;

    // Things needed for both PGL's single objective Dijkstra and PGL's 
    // NAMOA\* implementation:

    //! \brief Needed (by PGL's algorithms) in order to track the element 
    //!        in a priority queue.
    //!
    unsigned int pqitem;

    //! \brief A timestamp. 
    //!
    //! According to PGL's single objective Dijkstra and NAMOA\* implementations 
    //! it is needed in order to check if a node is visited or not.
    //!
    unsigned int timestamp;

    //! \brief The node's latitude.
    //!
    //! PGL's DIMACS9Reader initializes this. It reads it from the 
    //! coordinates file.
    //!
    unsigned int x;
    
    //! \brief The node's longitude.
    //!
    //! PGL's DIMACS9Reader initializes this. It reads it from the 
    //! coordinates file.
    //!
    unsigned int y;

    // temporary
    //! \brief The heuristic value of the node.
    //!
    //! Computed in MultiobjectiveSpOnPmgProblem::comb() for each specific 
    //! query and graph edge weights (they change as comb's weights change).
    //!
    double heuristicValue;
};


//! \brief A class representing the data that will be associated with each 
//!        graph edge. 
//!
//! Will use it with PGL's Packed-Memory graph.
//! 
//! \sa experiments_vs_namoa_star::Node
//!
class Edge : DefaultGraphItem
{
  public:
    //! Constructor. (initializes the Edge's attributes)
    Edge() : weight(0), criteriaList(2) // CHANGE-HERE
    { }

    // Things needed for PGL's single objective Dijkstra implementation:

    //! \brief The edge's weight/cost. (used with single objective Dijkstra)
    //!
    //! Single cost, used with PGL's single objective Dijkstra implementation.
    //! 
    //! experiments_vs_namoa_star::MultiobjectiveSpOnPmgProblem::comb() will 
    //! set this to a weighted sum of the edges multiple criteria costs 
    //! (those inside 
    //! experiments_vs_namoa_star::MultiobjectiveSpOnPmgProblem::criteriaList)
    //! before calling PGL's Dijkstra.
    //! 
    //! \sa experiments_vs_namoa_star::MultiobjectiveSpOnPmgProblem::comb()
    //!
    double weight;

    // Things needed for PGL's NAMOA\* implementation:

    //! \brief The edge's multiple criteria costs. (used by NAMOA\*)
    //!
    //! One cost for each criterion. 
    //! 
    //! This will be used by both PGL's NAMOA\* implementation, to find the 
    //! exact Pareto set, and 
    //! experiments_vs_namoa_star::MultiobjectiveSpOnPmgProblem::comb(), to 
    //! compute a weighted sum of the criteria costs before calling Dijkstra.
    //! 
    //! \sa experiments_vs_namoa_star::MultiobjectiveSpOnPmgProblem::comb()
    //!
    CriteriaList criteriaList;
};


//! The type of the graph we will use. (PGL's Packed Memory Array graph)
typedef DynamicGraph< PackedMemoryArrayImpl, Node, Edge> PmaGraph;

//! Type of the graph's node descriptors.
typedef PmaGraph::NodeDescriptor NodeDescriptor;

//! Iterator type to the graph's nodes.
typedef PmaGraph::NodeIterator NodeIterator;

//! Type of the graph's edge descriptors.
typedef PmaGraph::EdgeDescriptor EdgeDescriptor;

//! Iterator type to the graph's (outgoing) edges.
typedef PmaGraph::EdgeIterator EdgeIterator;

//! Iterator type to the graph's incoming edges.
typedef PmaGraph::InEdgeIterator InEdgeIterator;

//! \brief The type of a path in the graph. (a sequence of nodes)
//!
//! The first node is the source node and the last one the target.
//!
typedef std::list<NodeDescriptor> Path;


}  // namespace experiments_vs_namoa_star


//! 
//! @}
//!


#endif  // EXPERIMENTS_VS_NAMOA_STAR_COMMON_H
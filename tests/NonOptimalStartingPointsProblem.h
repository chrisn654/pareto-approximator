/* NonOptimalStartingPointsProblem.h */


#ifndef EXAMPLE_CLASS_NON_OPTIMAL_STARTING_POINTS_PROBLEM_H
#define EXAMPLE_CLASS_NON_OPTIMAL_STARTING_POINTS_PROBLEM_H


#include <string>

#include "../PointAndSolution.h"
#include "../BaseProblem.h"


using std::string;

using pareto_approximator::PointAndSolution;
using pareto_approximator::BaseProblem;


class NonOptimalStartingPointsProblem : public BaseProblem<string>
{
  public:
    NonOptimalStartingPointsProblem();
    ~NonOptimalStartingPointsProblem();

    PointAndSolution<string> comb(double xWeight, double yWeight);
};


#endif  // EXAMPLE_CLASS_NON_OPTIMAL_STARTING_POINTS_PROBLEM_H

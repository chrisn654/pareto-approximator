/*! \file HyperplaneTest.cpp
 *  \brief Unit test for the Hyperplane class.
 *  \author Christos Nitsas
 *  \date 2012
 */


#include <set>
#include <vector>
#include <algorithm>

#include "gtest/gtest.h"
#include "../Hyperplane.h"
#include "../Point.h"


using namespace pareto_approximator;


namespace {


// The fixture for testing class Hyperplane.
class HyperplaneTest : public ::testing::Test 
{
  protected:
    HyperplaneTest() { }

    ~HyperplaneTest() { }
};


// Test that Hyperplane's constructors and accessors work as expected.
TEST_F(HyperplaneTest, HyperplaneConstructorsAndAccessorsWork) 
{
  // test Hyperplane(double, double, double)
  Hyperplane h1(5, 10, 15);
  EXPECT_EQ(h1[0], 5.0);
  EXPECT_EQ(h1[1], 10.0);
  EXPECT_EQ(h1.b(), 15.0);
  EXPECT_EQ(h1.space_dimension(), 2);

  // test Hyperplane(double, double, double, double)
  Hyperplane h2(0.0, 2.0, 5.0, -4.5);
  EXPECT_EQ(h2[0], 0.0);
  EXPECT_EQ(h2[1], 2.0);
  EXPECT_EQ(h2[2], 5.0);
  EXPECT_EQ(h2.b(), -4.5);
  EXPECT_EQ(h2.space_dimension(), 3);

  // test Hyperplane(std::vector<int>::const_iterator, 
  //                 std::vector<int>::const_iterator)
  std::vector<int> coefficientsA(4);
  coefficientsA[0] = -2;
  coefficientsA[1] = 1;
  coefficientsA[2] = 0;
  coefficientsA[3] = 0;
  Hyperplane h3(coefficientsA.begin(), coefficientsA.end(), 12);
  EXPECT_EQ(h3[0], -2.0);
  EXPECT_EQ(h3[1], 1.0);
  EXPECT_EQ(h3[2], 0.0);
  EXPECT_EQ(h3[3], 0.0);
  EXPECT_EQ(h3.b(), 12.0);
  EXPECT_EQ(h3.space_dimension(), 4);

  // test Hyperplane(std::vector<double>::const_iterator, 
  //                 std::vector<double>::const_iterator)
  std::vector<double> coefficientsB(4);
  coefficientsB[0] = -2.0;
  coefficientsB[1] = 1.0;
  coefficientsB[2] = 0.0;
  coefficientsB[3] = 0.0;
  Hyperplane h4(coefficientsB.begin(), coefficientsB.end(), 12.0);
  EXPECT_EQ(h4[0], -2.0);
  EXPECT_EQ(h4[1], 1.0);
  EXPECT_EQ(h4[2], 0.0);
  EXPECT_EQ(h4[3], 0.0);
  EXPECT_EQ(h4.b(), 12.0);
  EXPECT_EQ(h4.space_dimension(), 4);

  // test Hyperplane(const int *, const int *, int)
  int coefficientsC[6] = {-1, 0, 1, 2, 3, 4};
  Hyperplane h5(coefficientsC, coefficientsC + 6, 5);
  EXPECT_EQ(h5[0], -1.0);
  EXPECT_EQ(h5[1], 0.0);
  EXPECT_EQ(h5[2], 1.0);
  EXPECT_EQ(h5[3], 2.0);
  EXPECT_EQ(h5[4], 3.0);
  EXPECT_EQ(h5[5], 4.0);
  EXPECT_EQ(h5.b(), 5.0);
  EXPECT_EQ(h5.space_dimension(), 6);

  // test Hyperplane(const double *, const double *, double)
  double coefficientsD[6] = {-1.0, 0.0, 1.0, 2.0, 3.0, 4.0};
  Hyperplane h6(coefficientsD, coefficientsD + 6, 5.0);
  EXPECT_EQ(h6[0], -1.0);
  EXPECT_EQ(h6[1], 0.0);
  EXPECT_EQ(h6[2], 1.0);
  EXPECT_EQ(h6[3], 2.0);
  EXPECT_EQ(h6[4], 3.0);
  EXPECT_EQ(h6[5], 4.0);
  EXPECT_EQ(h6.b(), 5.0);
  EXPECT_EQ(h6.space_dimension(), 6);

  // test Hyperplane(const Point &, const Point &)
  Hyperplane h7(Point(0, 1), Point(1, 2));
  EXPECT_EQ(h7.space_dimension(), 2);
  EXPECT_EQ(h7[0], -h7[1]);
  EXPECT_EQ(h7[1], h7.b());

  // test Hyperplane(std::set<Point>::const_iterator, 
  //                 std::set<Point>::const_iterator)
  std::vector<Point> pointsA;
  pointsA.push_back(Point(1, 0, 0));
  pointsA.push_back(Point(0, 1, 0));
  pointsA.push_back(Point(0, 0, 1));
  Hyperplane h8(pointsA.begin(), pointsA.end());
  EXPECT_EQ(h8.space_dimension(), 3);
  EXPECT_EQ(h8[0], h8[1]);
  EXPECT_EQ(h8[1], h8[2]);
  EXPECT_EQ(h8[2], h8.b());

  // test Hyperplane(const Point *, const Point *)
  Point pointsB[4];
  pointsB[0] = Point(1.0, 0.0, 0.0, 0.0);
  pointsB[1] = Point(0.0, 1.0, 0.0, 0.0);
  pointsB[2] = Point(0.0, 0.0, 1.0, 0.0);
  pointsB[3] = Point(0.0, 0.0, 0.0, 1.0);
  Hyperplane h9(pointsB, pointsB + 4);
  EXPECT_EQ(h9.space_dimension(), 4);
  EXPECT_EQ(h9[0], h9[1]);
  EXPECT_EQ(h9[1], h9[2]);
  EXPECT_EQ(h9[2], h9[3]);
  EXPECT_EQ(h9[3], h9.b());

  // test Hyperplane constructors (special cases)
  // 1) a_{1} * x_{1} = b
  std::vector<Point> pointsC;
  pointsC.push_back(Point(1.0, 0.0, 0.0));
  pointsC.push_back(Point(1.0, 1.0, 0.0));
  pointsC.push_back(Point(1.0, 1.0, 1.0));
  Hyperplane h10(pointsC.begin(), pointsC.end());
  EXPECT_EQ(h10.space_dimension(), 3);
  EXPECT_NE(h10[0], 0.0);
  EXPECT_EQ(h10[0], h10.b());
  EXPECT_EQ(h10[1], 0.0);
  EXPECT_EQ(h10[2], 0.0);

  // 2) 3 points on the same 3D-line. Line:
  // x = 2t
  // y = 3t
  // z = 4t
  // where t is a real number.
  // An infinite number of different hyperplanes pass through points on 
  // that line. We expect the all-zero 3D-hyperplane (0 = 0) to be returned.
  std::vector<Point> pointsD;
  pointsD.push_back(Point(0.0, 0.0, 0.0));
  pointsD.push_back(Point(2.0, 3.0, 4.0));
  pointsD.push_back(Point(4.0, 6.0, 8.0));
  Hyperplane h11(pointsD.begin(), pointsD.end());
  EXPECT_EQ(h11.space_dimension(), 3);
  EXPECT_EQ(h11[0], 0.0);
  EXPECT_EQ(h11[1], 0.0);
  EXPECT_EQ(h11[2], 0.0);
  EXPECT_EQ(h11.b(), 0.0);
}


TEST_F(HyperplaneTest, HyperplaneOperatorsAndStrWork) 
{
  Hyperplane h1(4.1, -2.2, 0.15, -2.1);
  EXPECT_EQ(h1.str(), "( 4.1 * x1 - 2.2 * x2 + 0.15 * x3 = -2.1 )");
  Hyperplane h2(-1.0, 0.0, 0.0);
  EXPECT_EQ(h2.str(), "( -1 * x1 + 0 * x2 = 0 )");

  EXPECT_EQ(h1 == h2, false);
  EXPECT_EQ(h1 != h2, true);

  Hyperplane h3(2 * 4.1, 2 * (-2.2), 2 * 0.15, 2 * (-2.1));
  EXPECT_EQ(h1 == h3, true);
  EXPECT_EQ(h1 != h3, false);
}


TEST_F(HyperplaneTest, HyperplaneIteratorsWork) 
{
  double coefficients[6] = {-1.0, 0.0, 1.0, 2.0, 3.0, 4.0};
  Hyperplane h1(coefficients, coefficients + 6, 5.0);
  EXPECT_EQ(std::equal(h1.begin(), h1.end(), coefficients), true);

  Hyperplane::iterator h1_iter = h1.begin();
  *h1_iter = 10.0;
  *(h1_iter + 1) = 20.0;
  EXPECT_EQ(h1[0], 10.0);
  EXPECT_EQ(h1[1], 20.0);
}


TEST_F(HyperplaneTest, HyperplaneParallelThroughAndIsParallelWork) 
{
  Hyperplane h1(4.0, -2.0, 0.0, -2.0);
  Hyperplane h2 = h1.parallelThrough(Point(1, 1, 1));
  EXPECT_EQ(h2[0], 4.0);
  EXPECT_EQ(h2[1], -2.0);
  EXPECT_EQ(h2[2], 0.0);
  EXPECT_EQ(h2.b(), 4.0 * 1 - 2.0 * 1 + 0.0 * 1);
  EXPECT_EQ(h1.isParallel(h2), true);

  Hyperplane h3 = h1.parallelThrough(Point(-1, -1, 1));
  EXPECT_EQ(h3, h1);
  EXPECT_EQ(h3.isParallel(h1), true);

  Hyperplane h4(1.0, 1.0, 1.0);
  EXPECT_EQ(h4.isParallel(h1), false);

  Hyperplane h5(2.0, 2.0, 5.0);
  EXPECT_EQ(h5.isParallel(h4), true);
}


TEST_F(HyperplaneTest, HyperplaneIntersectionWorksFor2Hyperplanes) 
{
  Hyperplane h1(1.0, -1.0, 0.0);
  Hyperplane h2(5.0, 2.0, 0.0);
  EXPECT_EQ(h1.intersection(h2), Point(0.0, 0.0));

  Hyperplane h3(-2.0, 1.0, -1.0);
  EXPECT_EQ(h3.intersection(h1), Point(1.0, 1.0));

  Hyperplane h4(0.0, 1.0, 3.3);
  EXPECT_EQ(h4.intersection(h1), Point(3.3, 3.3));
}


TEST_F(HyperplaneTest, HyperplaneReverseCoefficientSignsWorks) 
{
  Hyperplane h1(1.0, -1.0, 1.0);
  h1.reverseCoefficientSigns();
  EXPECT_EQ(h1[0], -1.0);
  EXPECT_EQ(h1[1], 1.0);
  EXPECT_EQ(h1.b(), -1.0);

  Hyperplane h2(0.0, 0.0, -4.0, -4.0);
  h2.reverseCoefficientSigns();
  EXPECT_EQ(h2[0], 0.0);
  EXPECT_EQ(h2[1], 0.0);
  EXPECT_EQ(h2[2], 4.0);
  EXPECT_EQ(h2.b(), 4.0);
}


}  // namespace


// Run all tests
int 
main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


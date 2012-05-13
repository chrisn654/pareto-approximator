/*! \file PointTest.cpp
 *  \brief Unit test for the Point class.
 *  \author Christos Nitsas
 *  \date 2012
 */


#include <vector>
#include <armadillo>

#include "gtest/gtest.h"
#include "../Point.h"


using namespace pareto_approximator;


namespace {


// The fixture for testing class Point.
class PointTest : public ::testing::Test 
{
  protected:
    PointTest() { }
    
    ~PointTest() { }

};


// Test that Point's constructors work for ints.
TEST_F(PointTest, PointConstructorsWorkForInts) 
{
  // test Point(int)
  Point p1(5);
  EXPECT_EQ(p1[0], 5);
  EXPECT_EQ(p1.dimension(), 1);

  // test Point(int, int)
  Point p2(4, -1);
  EXPECT_EQ(p2[0], 4);
  EXPECT_EQ(p2[1], -1);
  EXPECT_EQ(p2.dimension(), 2);

  // test Point(int, int, int)
  Point p3(-10, 3, 7);
  EXPECT_EQ(p3[0], -10);
  EXPECT_EQ(p3[1], 3);
  EXPECT_EQ(p3[2], 7);
  EXPECT_EQ(p3.dimension(), 3);

  // test Point(int, int, int, int)
  Point p4(-1, 0, 1, 2);
  EXPECT_EQ(p4[0], -1);
  EXPECT_EQ(p4[1], 0);
  EXPECT_EQ(p4[2], 1);
  EXPECT_EQ(p4[3], 2);
  EXPECT_EQ(p4.dimension(), 4);

  // test Point(std::vector<int>::const_iterator, 
  //            std::vector<int>::const_iterator)
  std::vector<int> coordinatesA(5);
  coordinatesA[0] = 3;
  coordinatesA[1] = -2;
  coordinatesA[2] = 7;
  coordinatesA[3] = 0;
  coordinatesA[4] = -8;
  Point p5(coordinatesA.begin(), coordinatesA.end());
  EXPECT_EQ(p5[0], 3);
  EXPECT_EQ(p5[1], -2);
  EXPECT_EQ(p5[2], 7);
  EXPECT_EQ(p5[3], 0);
  EXPECT_EQ(p5[4], -8);
  EXPECT_EQ(p5.dimension(), 5);

  // test Point(const int *, const int *)
  int coordinatesB[6] = {-2, -1, 0, 1, 2, 3};
  Point p6(coordinatesB, coordinatesB + 6);
  EXPECT_EQ(p6[0], -2);
  EXPECT_EQ(p6[1], -1);
  EXPECT_EQ(p6[2], 0);
  EXPECT_EQ(p6[3], 1);
  EXPECT_EQ(p6[4], 2);
  EXPECT_EQ(p6[5], 3);
}


// Test that Point's constructors work for doubles.
TEST_F(PointTest, PointConstructorsWorkForDoubles) 
{
  // test Point(double)
  Point p1(7.4);
  EXPECT_EQ(p1[0], 7.4);
  EXPECT_EQ(p1.dimension(), 1);

  // test Point(double, double)
  Point p2(-4.73, 12.497);
  EXPECT_EQ(p2[0], -4.73);
  EXPECT_EQ(p2[1], 12.497);
  EXPECT_EQ(p2.dimension(), 2);

  // test Point(double, double, double)
  Point p3(8.888802, -0.0001, 29.3);
  EXPECT_EQ(p3[0], 8.888802);
  EXPECT_EQ(p3[1], -0.0001);
  EXPECT_EQ(p3[2], 29.3);
  EXPECT_EQ(p3.dimension(), 3);

  // test Point(double, double, double, double)
  Point p4(-1.1, 0.0, 1.1, 2.2);
  EXPECT_EQ(p4[0], -1.1);
  EXPECT_EQ(p4[1], 0.0);
  EXPECT_EQ(p4[2], 1.1);
  EXPECT_EQ(p4[3], 2.2);
  EXPECT_EQ(p4.dimension(), 4);

  // test Point(std::vector<double>::const_iterator, 
  //            std::vector<double>::const_iterator)
  std::vector<double> coordinatesA(5);
  coordinatesA[0] = -2.1;
  coordinatesA[1] = 5.49;
  coordinatesA[2] = 0.0;
  coordinatesA[3] = -3.0;
  coordinatesA[4] = 8.5;
  Point p5(coordinatesA.begin(), coordinatesA.end());
  EXPECT_EQ(p5[0], -2.1);
  EXPECT_EQ(p5[1], 5.49);
  EXPECT_EQ(p5[2], 0.0);
  EXPECT_EQ(p5[3], -3.0);
  EXPECT_EQ(p5[4], 8.5);
  EXPECT_EQ(p5.dimension(), 5);

  // test Point(const double *, const double *)
  double coordinatesB[6] = {-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};
  Point p6(coordinatesB, coordinatesB + 6);
  EXPECT_EQ(p6[0], -2.5);
  EXPECT_EQ(p6[1], -1.5);
  EXPECT_EQ(p6[2], -0.5);
  EXPECT_EQ(p6[3], 0.5);
  EXPECT_EQ(p6[4], 1.5);
  EXPECT_EQ(p6[5], 2.5);
  EXPECT_EQ(p6.dimension(), 6);
}


// Test that Point::operator=() works as expected.
TEST_F(PointTest, PointAssignmentOperatorWorks) 
{
  Point p1(4.0, 3.5, -2.7);
  Point p2 = p1;

  EXPECT_EQ(p1[0], p2[0]);
  EXPECT_EQ(p1[1], p2[1]);
  EXPECT_EQ(p1[2], p2[2]);
  EXPECT_EQ(p1.dimension(), p2.dimension());

  double coordinates[5] = {10.0, 9.0, 8.0, 7.0, 6.0};
  Point p3(coordinates, coordinates + 5);
  Point p4 = p3;
  EXPECT_EQ(p3[0], p4[0]);
  EXPECT_EQ(p3[1], p4[1]);
  EXPECT_EQ(p3[2], p4[2]);
  EXPECT_EQ(p3[3], p4[3]);
  EXPECT_EQ(p3[4], p4[4]);
  EXPECT_EQ(p3.dimension(), p4.dimension());
}

// Test that Point::operator==() works as expected.
TEST_F(PointTest, PointEqualityOperatorWorks) 
{
  Point p1(4.0, 3.5, -2.7);
  Point p2(1.8, 2.1,  8.2);
  Point p3(4.0, 3.5, -2.7);

  EXPECT_EQ(p1 == p3, true);
  EXPECT_EQ(p1 == p2, false);

  double coordinatesA[5] = {10.0, 9.0, 8.0, 7.0, 6.0};
  double coordinatesB[5] = {-1.0, 0.0, 1.0, 2.0, 3.0};
  Point p4(coordinatesA, coordinatesA + 5);
  Point p5(coordinatesA, coordinatesA + 5);
  Point p6(coordinatesB, coordinatesB + 5);

  EXPECT_EQ(p4 == p5, true);
  EXPECT_EQ(p4 == p6, false);
}

// Test that Point::operator!=() works as expected.
TEST_F(PointTest, PointInequalityOperatorWorks) 
{
  Point p1(4.0, 3.5, -2.7);
  Point p2(1.8, 2.1,  8.2);
  Point p3(4.0, 3.5, -2.7);

  EXPECT_EQ(p1 != p3, false);
  EXPECT_EQ(p1 != p2, true);

  double coordinates[5] = {10.0, 9.0, 8.0, 7.0, 6.0};
  Point p4(coordinates, coordinates + 5);
  Point p5(coordinates, coordinates + 5);

  EXPECT_EQ(p4 != p5, false);
  EXPECT_EQ(p4 != p3, true);
}

// Test that Point::operator<() works as expected.
TEST_F(PointTest, PointLessThanOperatorWorks) 
{
  Point p1(4.0, 3.5, -2.7);
  Point p2(1.8, 2.1,  8.2);
  Point p3(4.0, 3.5, -2.8);
  Point p4(1.8, 2.0, 17.5);
  Point p5(17.1, 15.4);
  Point p6(17.1, 13.1);
  
  EXPECT_EQ(p2 < p1, true);
  EXPECT_EQ(p1 < p2, false);
  EXPECT_EQ(p1 < p1, false);
  EXPECT_EQ(p3 < p1, true);
  EXPECT_EQ(p4 < p2, true);
  EXPECT_EQ(p6 < p5, true);
  EXPECT_EQ(p5 < p6, false);

  double coordinatesA[5] = {10.0, 9.0, 8.0, 7.0, 6.0};
  double coordinatesB[5] = {-1.0, 0.0, 1.0, 2.0, 3.0};
  Point p7(coordinatesA, coordinatesA + 5);
  Point p8(coordinatesB, coordinatesB + 5);
  
  EXPECT_EQ(p7 < p8, false);
  EXPECT_EQ(p8 < p7, true);

  EXPECT_THROW(p1 < p5, DifferentDimensionsException);
}

// Test that Point::operator[]() works as expected.
TEST_F(PointTest, PointAccessOperatorWorks) 
{
  Point p1(4.0, 3.5, -2.7);

  EXPECT_EQ(p1[0], 4.0);
  EXPECT_EQ(p1[1], 3.5);
  EXPECT_EQ(p1[2], -2.7);

  double coordinates[5] = {10.0, 9.0, 8.0, 7.0, 6.0};
  Point p2(coordinates, coordinates + 5);
  EXPECT_EQ(p2[0], 10.0);
  EXPECT_EQ(p2[1], 9.0);
  EXPECT_EQ(p2[2], 8.0);
  EXPECT_EQ(p2[3], 7.0);
  EXPECT_EQ(p2[4], 6.0);

  EXPECT_THROW(p1[3], NonExistentCoordinateException);
}

// Test that Point::toVec() and Point::toRowVec() work.
TEST_F(PointTest, PointToArmadilloVectorMethodsWork) 
{
  Point p1(1.4);
  arma::vec p1v = p1.toVec();
  EXPECT_EQ(p1v.size(), 1);
  EXPECT_EQ(p1v(0), 1.4);
  arma::rowvec p1rv = p1.toRowVec();
  EXPECT_EQ(p1rv.size(), 1);
  EXPECT_EQ(p1rv(0), 1.4);

  double coords[4] = {-1.0, 0.0, 1.0, 2.0};
  Point p2(coords, coords + 4);
  arma::vec p2v = p2.toVec();
  EXPECT_EQ(p2v.size(), 4);
  EXPECT_EQ(p2v(0), -1.0);
  EXPECT_EQ(p2v(1), 0.0);
  EXPECT_EQ(p2v(2), 1.0);
  EXPECT_EQ(p2v(3), 2.0);
  arma::rowvec p2rv = p2.toRowVec();
  EXPECT_EQ(p2rv.size(), 4);
  EXPECT_EQ(p2v(0), -1.0);
  EXPECT_EQ(p2v(1), 0.0);
  EXPECT_EQ(p2v(2), 1.0);
  EXPECT_EQ(p2v(3), 2.0);
}

// Test that Point::isZero() works.
TEST_F(PointTest, PointIsZeroWorks) 
{
  Point p1(0.0);
  EXPECT_TRUE(p1.isZero());

  Point p2(0.0, 0.0);
  EXPECT_TRUE(p2.isZero());
  
  Point p3(0.0, 0.0, 0.0);
  EXPECT_TRUE(p3.isZero());

  Point p4(1.0, 0.0, 2.4);
  EXPECT_FALSE(p4.isZero());

  double coordinates[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  Point p5(coordinates, coordinates + 5);
  EXPECT_TRUE(p5.isZero());
}
  
// Test that Point::dimension() and Point::dimension(int) work as expected.
TEST_F(PointTest, PointGetDimensionAndSetDimensionWork) 
{
  Point p1(3.9);
  Point p2(0.3, 1.0);
  double coordinates[4] = {10.0, 9.0, 8.0, 7.0};
  Point p3(coordinates, coordinates + 4);
  EXPECT_EQ(p1.dimension(), 1);
  EXPECT_EQ(p2.dimension(), 2);
  EXPECT_EQ(p3.dimension(), 4);

  p3.dimension(2);
  EXPECT_EQ(p3.dimension(), 2);
  EXPECT_EQ(p3[0], 10.0);
  EXPECT_EQ(p3[1], 9.0);

  p3.dimension(1);
  EXPECT_EQ(p3.dimension(), 1);
  EXPECT_EQ(p3[0], 10.0);
}


// Test that Point::ratioDistance() works as expected.
TEST_F(PointTest, PointRatioDistanceWorks) 
{
  Point p1(2, 100);
  Point p2(4, 900);
  EXPECT_EQ(p1.ratioDistance(p2), 8);
  Point p3(4, 110);
  EXPECT_EQ(p1.ratioDistance(p3), 1);
  Point p4(1, 100);
  EXPECT_EQ(p1.ratioDistance(p4), 0);
  EXPECT_EQ(p1.ratioDistance(p1), 0);
  Point p5(1, 10, 100);
  Point p6(2, 30, 400);
  EXPECT_EQ(p5.ratioDistance(p6), 3);

  double coordinatesA[5] = {1.0, 10.0, 100.0, 1000.0, 10000.0};
  double coordinatesB[5] = {1.0, 20.0, 300.0, 4000.0, 50000.0};
  Point p7(coordinatesA, coordinatesA + 5);
  Point p8(coordinatesB, coordinatesB + 5);
  EXPECT_EQ(p7.ratioDistance(p8), 4);

  EXPECT_THROW(p1.ratioDistance(p5), DifferentDimensionsException);
}


// Test that Point::dominates() works as expected.
TEST_F(PointTest, PointDominatesWorks) 
{
  Point p1(1.0, 5.0);
  Point p2(1.5, 7.0);
  EXPECT_EQ(p1.dominates(p2), true);
  EXPECT_EQ(p2.dominates(p1), false);
  EXPECT_EQ(p2.dominates(p1, 0.5), true);
  Point p3(1.6, 6.0);
  EXPECT_EQ(p3.dominates(p1, 0.5), false);
  Point p4(1.0, 1.0, 1.0);
  Point p5(2.0, 2.0, 2.0);
  EXPECT_EQ(p4.dominates(p5), true);
  EXPECT_EQ(p5.dominates(p4), false);

  double coordinatesA[5] = {1.0, 10.0, 100.0, 1000.0, 10000.0};
  double coordinatesB[5] = {1.0, 20.0, 300.0, 4000.0, 50000.0};
  Point p7(coordinatesA, coordinatesA + 5);
  Point p8(coordinatesB, coordinatesB + 5);
  EXPECT_EQ(p7.dominates(p8), true);
  EXPECT_EQ(p8.dominates(p7), false);
  EXPECT_EQ(p8.dominates(p7, 4), true);

  EXPECT_THROW(p1.dominates(p2, -0.5), NegativeApproximationRatioException);
  Point p9(-1.3, 8.7);
  EXPECT_THROW(p1.dominates(p9), NotPositivePointException);
  Point p10(2.4, 8.97, 1.42);
  EXPECT_THROW(p1.dominates(p10), DifferentDimensionsException);
}


// Test that Point::str() works as expected.
TEST_F(PointTest, PointStrWorks) 
{
  Point p1(1, 1000);
  EXPECT_EQ(p1.str(), "(1, 1000)");
  Point p2(49.75, 5000000.2);
  EXPECT_EQ(p2.str(), "(49.75, 5e+06)");
  Point p3(-4.9, 0.0);
  EXPECT_EQ(p3.str(), "(-4.9, 0)");
  double coordinates[4] = {2.2, 4.2, 8.2, 16.2};
  Point p4(coordinates, coordinates + 4);
  EXPECT_EQ(p4.str(), "(2.2, 4.2, 8.2, 16.2)");
}


}  // namespace


// Run all tests
int 
main(int argc, char** argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


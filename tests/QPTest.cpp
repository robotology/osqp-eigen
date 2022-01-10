/**
 * @file QPTest.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2020
 */

// Catch2
#include <catch2/catch.hpp>

#include <OsqpEigen/OsqpEigen.h>

TEST_CASE("QPProblem - Unconstrained")
{
    constexpr double tolerance = 1e-4;

    Eigen::SparseMatrix<double> H_s(2,2);
    H_s.insert(0,0) = 3;
    H_s.insert(0,1) = 2;
    H_s.insert(1,0) = 2;
    H_s.insert(1,1) = 4;

    Eigen::Vector2d gradient;
    gradient << 3, 1;

    OsqpEigen::Solver solver;
    solver.settings()->setVerbosity(true);
    solver.settings()->setAlpha(1.0);

    solver.data()->setNumberOfVariables(2);
    solver.data()->setNumberOfConstraints(0);

    REQUIRE(solver.data()->setHessianMatrix(H_s));
    REQUIRE(solver.data()->setGradient(gradient));

    REQUIRE(solver.initSolver());
    REQUIRE(solver.solveProblem() == OsqpEigen::ErrorExitFlag::NoError);

    // expected solution
    Eigen::Vector2d expectedSolution;
    expectedSolution << -1.2500,  0.3750;

    REQUIRE(solver.getSolution().isApprox(expectedSolution, tolerance));
}


TEST_CASE("QPProblem")
{
    constexpr double tolerance = 1e-4;

    Eigen::SparseMatrix<double> H_s(2,2);
    H_s.insert(0,0) = 4;
    H_s.insert(0,1) = 1;
    H_s.insert(1,0) = 1;
    H_s.insert(1,1) = 2;

    Eigen::SparseMatrix<double> A_s(3,2);
    A_s.insert(0,0) = 1;
    A_s.insert(0,1) = 1;
    A_s.insert(1,0) = 1;
    A_s.insert(2,1) = 1;

    Eigen::Vector2d gradient;
    gradient << 1, 1;

    Eigen::Vector3d lowerBound;
    lowerBound << 1, 0, 0;

    Eigen::Vector3d upperBound;
    upperBound << 1, 0.7, 0.7;

    OsqpEigen::Solver solver;
    solver.settings()->setVerbosity(true);
    solver.settings()->setAlpha(1.0);

    REQUIRE_FALSE(solver.data()->setHessianMatrix(H_s));
    solver.data()->setNumberOfVariables(2);

    solver.data()->setNumberOfConstraints(3);
    REQUIRE(solver.data()->setHessianMatrix(H_s));
    REQUIRE(solver.data()->setGradient(gradient));
    REQUIRE(solver.data()->setLinearConstraintsMatrix(A_s));
    REQUIRE(solver.data()->setLowerBound(lowerBound));
    REQUIRE(solver.data()->setUpperBound(upperBound));

    REQUIRE(solver.initSolver());

    REQUIRE(solver.solveProblem() == OsqpEigen::ErrorExitFlag::NoError);
    Eigen::Vector2d expectedSolution;
    expectedSolution << 0.3,  0.7;

    REQUIRE(solver.getSolution().isApprox(expectedSolution, tolerance));
}

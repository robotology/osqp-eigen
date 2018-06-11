/**
 * @file QPTest.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// gtest
#include <gtest/gtest.h>

#include <OsqpEigen.h>

TEST(QPProblem, )
{
    Eigen::Matrix2d H;
    H << 4, 1,
        1, 2;
    Eigen::SparseMatrix<double> H_s;
    H_s = H.sparseView();

    Eigen::Matrix<double,3,2> A;
    A << 1, 1,
        1, 0,
        0, 1;
    Eigen::SparseMatrix<double> A_s;
    A_s = A.sparseView();

    Eigen::Vector2d gradient;
    gradient << 1, 1;

    Eigen::Vector3d lowerBound;
    lowerBound << 1, 0, 0;

    Eigen::Vector3d upperBound;
    upperBound << 1, 0.7, 0.7;

    OsqpEigen::Solver solver;
    solver.settings()->setVerbosity(false);

    ASSERT_FALSE(solver.data()->setHessianMatrix(H_s));
    solver.data()->setNumberOfVariables(2);

    solver.data()->setNumberOfConstraints(3);
    ASSERT_TRUE(solver.data()->setHessianMatrix(H_s));
    ASSERT_TRUE(solver.data()->setGradient(gradient));
    ASSERT_TRUE(solver.data()->setLinearConstraintsMatrix(A_s));
    ASSERT_TRUE(solver.data()->setLowerBound(lowerBound));
    ASSERT_TRUE(solver.data()->setUpperBound(upperBound));


    ASSERT_TRUE(solver.initSolver());

    ASSERT_TRUE(solver.solve());

    std::cerr << "The solution of the QP problem is" << std::endl;
    std::cerr << "[ " << solver.getSolution() << " ]"
              << std::endl;
};

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    clock_t startTime, endTime;

    startTime = clock();
    bool outcome = RUN_ALL_TESTS();
    endTime = clock();

    std::cerr << "Total time " << (static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC)
              << " seconds." << std::endl;

    return outcome;
}

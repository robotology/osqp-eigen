/**
 * @file QPTest.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// gtest
#include <gtest/gtest.h>

#include "SparseMatrix.hpp"
#include "OptimizatorData.hpp"
#include "OptimizatorSettings.hpp"
#include "OptimizatorWorkspace.hpp"


TEST(QPProblem, )
{
    Eigen::Matrix2d H;
    H << 4, 1,
        1, 2;
    Eigen::SparseMatrix<double> H_s;
    H_s = H.sparseView();
    OSQPWrapper::SparseMatrix hessian(H_s);

    Eigen::Matrix<double,3,2> A;
    A << 1, 1,
        1, 0,
        0, 1;
    Eigen::SparseMatrix<double> A_s;
    A_s = A.sparseView();
    OSQPWrapper::SparseMatrix linear(A_s);

    Eigen::Vector2d gradient;
    gradient << 1, 1;

    Eigen::Vector3d lowerBound;
    lowerBound << 1, 0, 0;

    Eigen::Vector3d upperBound;
    upperBound << 1, 0.7, 0.7;

    OSQPWrapper::OptimizatorSettings settings;
    settings.setVerbosity(false);

    OSQPWrapper::OptimizatorData data;
    data.setNumberOfVariables(2);
    data.setNumberOfConstraints(3);

    ASSERT_TRUE(data.setHessianMatrix(hessian));
    ASSERT_TRUE(data.setGradient(gradient));
    ASSERT_TRUE(data.setLinearConstraintsMatrix(linear));
    ASSERT_TRUE(data.setLowerBound(lowerBound));
    ASSERT_TRUE(data.setUpperBound(upperBound));

    OSQPWrapper::OptimizatorWorkspace solver;
    ASSERT_TRUE(solver.setWorkspace(data, settings));

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

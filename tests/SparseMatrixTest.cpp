/**
 * @file SparseMatrixTest.cpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// gtest
#include <gtest/gtest.h>

#include "SparseMatrixHelper.hpp"
#include "osqp.h"

template<typename T, int n, int m>
bool computeTest(const Eigen::Matrix<T, n, m> &mEigen)
{
    Eigen::SparseMatrix<T> matrix, newMatrix;
    matrix = mEigen.sparseView();

    csc* osqpSparseMatrix = nullptr;
    //NOTE: Dynamic memory allocation
    if(!OSQPWrapper::SparseMatrixHelper::createOsqpSparseMatrix(matrix, osqpSparseMatrix))
        return false;

    if(!OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToEigenSparseMatrix(osqpSparseMatrix, newMatrix))
        return false;

    bool outcome = matrix.isApprox(newMatrix);

    c_free(osqpSparseMatrix);

    return true;
}

TEST(SparseMatrix, Double)
{
    Eigen::Matrix3d m;
    m << 0, 1, 0,
        0, 0, 0,
        0, 1,0;

    ASSERT_TRUE(computeTest(m));
}

TEST(SparseMatrix, Float)
{
    Eigen::Matrix3f m;
    m << 0, 1, 0,
        0, 0, 0,
        0, 1,0;

    ASSERT_TRUE(computeTest(m));
}

TEST(SparseMatrix, Int)
{
    Eigen::Matrix3i m;
    m << 0, 1, 0,
        0, 0, 0,
        0, 1,0;

    ASSERT_TRUE(computeTest(m));
}

TEST(SparseMatrix, Double1)
{
    Eigen::Matrix<double,4,2> m;
    m << 0, 0, 0, 4,
        0, 0, 0, 0;

    ASSERT_TRUE(computeTest(m));
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/**
 * @file SparseMatrixTest.cpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// gtest
#include <gtest/gtest.h>

#include "SparseMatrix.hpp"

template<typename T, int n, int m>
bool computeTest(const Eigen::Matrix<T, n, m> &mEigen)
{
    Eigen::SparseMatrix<T> matrix;
    matrix = mEigen.sparseView();

    OSQPWrapper::SparseMatrix matrixOSQP;

    matrixOSQP << matrix;

    return matrix.isApprox(matrixOSQP.toEigen<T>());
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

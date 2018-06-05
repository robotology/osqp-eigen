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
    std::vector<Eigen::Triplet<T>> tripletListCsc;
    if(!OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToTriplets(osqpSparseMatrix, tripletListCsc))
        return false;

    for(auto a: tripletListCsc)
        std::cout<<a.row() << " " <<a.col() << " " <<a.value()<<"\n ";

    std::vector<Eigen::Triplet<T>> tripletListEigen;
    OSQPWrapper::SparseMatrixHelper::eigenSparseMatrixToTriplets(matrix, tripletListEigen);

    std::cout<<"***********************************************\n";
    for(auto a: tripletListEigen)
        std::cout<<a.row() << " " <<a.col() << " " <<a.value()<<"\n ";


    bool outcome = matrix.isApprox(newMatrix);

    csc_spfree(osqpSparseMatrix);

    return outcome;
}

TEST(SparseMatrix, Double)
{
    Eigen::Matrix3d m;
    m << 0, 1.002311, 0,
        0, 0, 0,
        0, 0.90835435,0;

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

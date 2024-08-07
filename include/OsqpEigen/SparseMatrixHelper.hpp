/**
 * @file SparseMatrixHelper.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2018
 */

#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

// std
#include <iostream>

// eigen
#include <Eigen/Sparse>

#include <OsqpEigen/Compat.hpp>

/**
 * OsqpEigen namespace.
 */
namespace OsqpEigen
{
/**
 * SparseMatrixHelper namespace is a namespace that contains helper function to handle osqp matrix.
 * Use it to create to update or manage an osqp sparse matrix.
 * osqp sparse matrix in [compressed-column](https://people.sc.fsu.edu/~jburkardt/data/cc/cc.html)
 * or triplet form.
 */
namespace SparseMatrixHelper
{

/**
 * Allocate an osqpSparseMatrix struct.
 * NOTE: <code>c_malloc</code> function is used to allocate memory please call
 * <code>c_free</code> to deallcate memory.
 * @return a const point to the csc struct.
 */
template <typename Derived>
bool createOsqpSparseMatrix(const Eigen::SparseCompressedBase<Derived>& eigenSparseMatrix,
                            csc*& osqpSparseMatrix);

/**
 * Convert an osqp sparse matrix into an eigen sparse matrix.
 * @param osqpSparseMatrix is  a constant pointer to a constant csc struct;
 * @param eigenSparseMatrix is the eigen sparse matrix object.
 * @return a const point to the csc struct.
 */
template <typename T>
bool osqpSparseMatrixToEigenSparseMatrix(const csc* const& osqpSparseMatrix,
                                         Eigen::SparseMatrix<T>& eigenSparseMatrix);

/**
 * Convert an osqp sparse matrix into a eigen triplet list.
 * @param osqpSparseMatrix is reference to a constant pointer to a constant csc struct;
 * @param tripletList is a std::vector containing the triplet.
 * @return a const point to the csc struct.
 */
template <typename T>
bool osqpSparseMatrixToTriplets(const csc* const& osqpSparseMatrix,
                                std::vector<Eigen::Triplet<T>>& tripletList);

/**
 * Convert an eigen sparse matrix into a eigen triplet list.
 * @param eigenSparseMatrix is the eigen sparse matrix object;
 * @param tripletList is a std::vector containing the triplet.
 * @return a const point to the csc struct.
 */
template <typename Derived, typename T>
bool eigenSparseMatrixToTriplets(const Eigen::SparseCompressedBase<Derived>& eigenSparseMatrix,
                                 std::vector<Eigen::Triplet<T>>& tripletList);
}; // namespace SparseMatrixHelper
} // namespace OsqpEigen

#include <OsqpEigen/SparseMatrixHelper.tpp>

#endif

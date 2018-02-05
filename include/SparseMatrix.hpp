/**
 * @file SparseMatrix.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

// std
#include <utility>

// eigen
#include <Eigen/Sparse>


// OSQP
#include "osqp.h"

/**
 * OSQPWrapper namespace.
 */
namespace OSQPWrapper
{
    /**
     * SparseMatrix class is a wrapper of the OSQP csc struct.
     */
    class SparseMatrix
    {
        /**
         * Matrix in compressed-column or triplet form.
         * <a href="https://people.sc.fsu.edu/~jburkardt/data/cc/cc.html">Here</a>
         * you can find further information.
         */
        csc *m_matrix;

        std::vector<c_int> m_innerIndex; /**< Row indices, size nzmax starting from 0. */
        std::vector<c_int> m_outerIndex; /**< Column pointers (size n+1). */
        std::vector<c_float> m_values; /**< Numerical values (size m_innerIndex.size()) */

        /**
         * Get the sparse matrix using the triplet form ( i.e. a small structure to hold
         * a non zero as a triplet (i,j,value)).
         * @return a std::vector containing the sparse matrix triplet.
         */
        template<typename T>
        std::vector<Eigen::Triplet<T>> toTriplets();

    public:

        /**
         * Constructor.
         */
        SparseMatrix();

        /**
         * Constructor.
         * @param Eigen::SparseMatrix is a standard Eigen sparse matrix object.
         */
        template<typename T>
        SparseMatrix(const Eigen::SparseMatrix<T> &matrix);

        /**
         * Deconstructor.
         */
        ~SparseMatrix();

        /**
         * Convenient operator to set the a sparse matrix from an Eigen::SparseMatrix object.
         */
        template<typename T>
        void operator<<(const Eigen::SparseMatrix<T> &matrix);

        /**
         * Get the sparse matrix in Eigen form.
         * @return a Eigen::SparseMatrix.
         */
        template<typename T>
        Eigen::SparseMatrix<T> toEigen();

        /**
         * Get the sparse matrix in csc form.
         * @return a const point to the csc struct.
         */
        csc* const & getSparseMatrix() const;

        /**
         * Get the size of the matrix.
         * @return an std::pair containing the size of the matrix.
         */
        std::pair<c_int, c_int> size() const;
    };
}

#include "SparseMatrix.tpp"

#endif

/**
 * @file SparseMatrix.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

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
         * you can find furthere information.
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
         * @param Eigen::SparseMatrix is a standard Eigen sparse matrix object
         */
        template<typename T>
        SparseMatrix(const Eigen::SparseMatrix<T> &matrix);

        /**
         * Deconstructor.
         */
        ~SparseMatrix();

        /**
         * Get the sparse matrix in Eigen form
         * @return a Eigen::SparseMatrix
         */
        template<typename T>
        Eigen::SparseMatrix<T> toEigen();
    };
}

#include "SparseMatrix.tpp"

#endif

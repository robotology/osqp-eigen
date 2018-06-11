/**
 * @file Data.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#ifndef OSQPEIGEN_DATA_HPP
#define OSQPEIGEN_DATA_HPP

// Eigen
#include <Eigen/Dense>

// OSQP
#include <osqp.h>

// OsqpEigen
#include <OsqpEigen/SparseMatrixHelper.hpp>

/**
 * OsqpEigen namespace.
 */
namespace OsqpEigen
{
    /**
     * Data class is a wrapper of the OSQP OSQPData struct.
     */
    class Data
    {
        OSQPData *m_data; /**< OSQPData struct. */
        bool m_isNumberOfVariablesSet; /**< Boolean true if the number of variables is set. */
        bool m_isNumberOfConstraintsSet; /**< Boolean true if the number of constraints is set. */
        bool m_isHessianMatrixSet;  /**< Boolean true if the hessian matrix is set. */
        bool m_isGradientSet; /**< Boolean true if the gradient vector is set. */
        bool m_isLinearConstraintsMatrixSet; /**< Boolean true if the linear constrain matrix is set. */
        bool m_isLowerBoundSet; /**< Boolean true if the lower bound vector is set. */
        bool m_isUpperBoundSet; /**< Boolean true if the upper bound vector is set. */

    public:
        /**
         * Constructor.
         */
        Data();

        /**
         * Constructor.
         * @param n is the number of variables;
         * @param m is the number of constraints.
         */
        Data(int n, int m);

        /**
         * Deconstructor.
         */
        ~Data();

        /**
         * Clear the hessian matrix.
         */
        void clearHessianMatrix();

        /**
         * Clear the linear constraints matrix.
         */
        void clearLinearConstraintsMatrix();

        /**
         * Set the number of variables.
         * @param n is the number of variables.
         */
        void setNumberOfVariables(int n);

        /**
         * Set the number of constraints.
         * @param m is the number of constraints.
         */
        void setNumberOfConstraints(int m);

        /**
         * Set the quadratic part of the cost function (Hessian).
         * It is assumed to be a simmetric matrix.
         * @param hessianMatrix is the Hessian matrix.
         * @return true/false in case of success/failure.
         */
        template<typename T>
        bool setHessianMatrix(const Eigen::SparseMatrix<T> &hessianMatrix);

        /**
         * Set the linear part of the cost function (Gradient).
         * @param gradientVector is the Gradient vector.
         * @return true/false in case of success/failure.
         */
        template<int n>
        bool setGradient(Eigen::Matrix<c_float, n, 1> &gradientVector);

        /**
         * Set the linear constraint matrix A (size m x n)
         * @param linearConstraintsMatrix is the linear constraints matrix A.
         * @return true/false in case of success/failure.
         */
        template<typename T>
        bool setLinearConstraintsMatrix(const Eigen::SparseMatrix<T> &linearConstraintsMatrix);

        /**
         * Set the array for lower bound (size m).
         * @param lowerBoundVector is the lower bound constraint.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool setLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBoundVector);

        /**
         * Set the array for upper bound (size m).
         * @param upperBoundVector is the upper bound constraint.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool setUpperBound(Eigen::Matrix<c_float, m, 1>& upperBoundVector);

        /**
         * Get the OSQPData struct.
         * @return a const point to the OSQPData struct.
         */
        OSQPData *const & getData() const;

        /**
         * Verify if all the matrix and vectors are already set.
         * @return true if all the OSQPData struct are set.
         */
        bool isSet() const;
    };
}

#include <OsqpEigen/Data.tpp>

#endif

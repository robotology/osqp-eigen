/**
 * @file OptimizatorWorkspace.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */
#ifndef OPTIMIZATOR_WORKSPACE_HPP
#define OPTIMIZATOR_WORKSPACE_HPP

// Eigen
#include <Eigen/Dense>

// OSQP
#include "osqp.h"

// OSQPWrapper
#include "OptimizatorData.hpp"
#include "OptimizatorSettings.hpp"

/**
 * OSQPWrapper namespace.
 */
namespace OSQPWrapper
{
    /**
     * OptimizatorWorkspace class is a wrapper of the OSQP OSQPWorkspace struct.
     */
    class OptimizatorWorkspace
    {
        OSQPWorkspace *m_workspace;  /**< OSQPWorkspace struct. */

    public:
        /**
         * Constructor.
         * @param data is an OSQPWrapper::OptimizatorData object;
         * @param settings is an OSQPWrapper::OptimizatorSettings object;
         */
        OptimizatorWorkspace(const OSQPWrapper::OptimizatorData& data,
                             OSQPWrapper::OptimizatorSettings& settings);

        /**
         * Deconstructor.
         */
        ~OptimizatorWorkspace();

        /**
         * Solve the QP optimization problem.
         * @return true/false in case of success/failure.
         */
        bool solve();

        /**
         * Get the optimization problem solution.
         * @return an Eigen::Vector contating the optimization result.
         */
        Eigen::VectorXd getSolution();

        /**
         * Update the linear part of the cost function (Gradient).
         * @param gradient is the Gradient vector.
         * @return true/false in case of success/failure.
         */
        template<int n>
        bool updateGradient(Eigen::Matrix<c_float, n, 1>& gradien);

        /**
         * Update the lower bound constraints (size m).
         * @param lowerBound is the lower bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound);

        /**
         * Update the upper bound constraints (size m).
         * @param upperBound is the upper bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateUpperBound(Eigen::Matrix<c_float, m, 1>& upperBound);

        /**
         * Update both upper and lower bounds (size m).
         * @param lowerBound is the lower bound constraint vector;
         * @param upperBound is the upper bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateBounds(Eigen::Matrix<c_float, m, 1>& lowerBound,
                          Eigen::Matrix<c_float, m, 1>& upperBound);

        /**
         * Update the quadratic part of the cost function (Hessian).
         * It is assumed to be a simmetric matrix.
         * @param hessian is the Hessian matrix.
         * @return true/false in case of success/failure.
         */
        bool updateHessianMatrix(const OSQPWrapper::SparseMatrix& hessian);

        /**
         * Update the linear constraints matrix A (size m x n)
         * @param A is the linear constraint matrix.
         * @return true/false in case of success/failure.
         */
        bool updateLinearConstraintsMatrix(const OSQPWrapper::SparseMatrix& A);
    };

    #include "OptimizatorWorkspace.tpp"
}

#endif

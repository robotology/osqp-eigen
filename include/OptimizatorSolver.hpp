/**
 * @file OptimizatorSolver.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */
#ifndef OPTIMIZATOR_WORKSPACE_HPP
#define OPTIMIZATOR_WORKSPACE_HPP

// Std
#include <memory>

// Eigen
#include <Eigen/Dense>

// OSQP
#include "osqp.h"

// OSQPWrapper
#include "OptimizatorData.hpp"
#include "OptimizatorSettings.hpp"
#include "OptimizatorConstants.hpp"

/**
 * OSQPWrapper namespace.
 */
namespace OSQPWrapper
{
    /**
     * OptimizatorSolver class is a wrapper of the OSQP OSQPWorkspace struct.
     */
    class OptimizatorSolver
    {
        OSQPWorkspace *m_workspace;  /**< OSQPWorkspace struct. */
        std::unique_ptr<OSQPWrapper::OptimizatorSettings> m_settings; /**< Pointer to OptimizatorSettings class. */
        std::unique_ptr<OSQPWrapper::OptimizatorData> m_initData; /**< Pointer to OptimizatorData class. */

    public:

        /**
         * Constructor.
         */
        OptimizatorSolver();

        /**
         * Deconstructor.
         */
        ~OptimizatorSolver();

        /**
         * Initialize the solver with the actual initial data and settings.
         * @return true/false in case of success/failure.
         */
        bool initSolver();

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
        bool updateGradient(Eigen::Matrix<c_float, n, 1>& gradient);

        /**
         * Update the lower bounds limit (size m).
         * @param lowerBound is the lower bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound);

        /**
         * Update the upper bounds limit (size m).
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
         * Get the solver settings pointer.
         * @return the pointer to OptimizatorSettings object.
         */
        const std::unique_ptr<OSQPWrapper::OptimizatorSettings>& settings() const;

        /**
         * Get the pointer to the solver initial data.
         * @return the pointer to OptimizatorData object.
         */
        const std::unique_ptr<OSQPWrapper::OptimizatorData>& initData() const;

    };

    #include "OptimizatorSolver.tpp"
}

#endif

/**
 * @file OptimizatorWorkspace.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#ifndef OPTIMIZATOR_WORKSPACE_HPP
#define OPTIMIZATOR_WORKSPACE_HPP

// OSQP
#include "osqp.h"

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
                             const OSQPWrapper::OptimizatorSettings& settings);

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
    };
}

#endif

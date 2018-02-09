/**
 * @file OptimizatorSolver.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// OSQPWrapper
#include "OptimizatorData.hpp"
#include "OptimizatorSettings.hpp"
#include "OptimizatorSolver.hpp"

OSQPWrapper::OptimizatorSolver::OptimizatorSolver()
{
    m_settings = std::make_unique<OSQPWrapper::OptimizatorSettings>();
    m_initData = std::make_unique<OSQPWrapper::OptimizatorData>();
    m_workspace = nullptr;
}

bool OSQPWrapper::OptimizatorSolver::initSolver()
{
    if(!m_initData->isSet()){
        std::cout << "[Optimizator Workspace] Some data are not set." << std::endl;
        return false;
    }

    m_workspace = osqp_setup(m_initData->getOptimizatorData(),
                             m_settings->getOptimizatorSettings());

    // settings and initData will not be used anymore
    m_settings.reset();
    m_initData.reset();
    return true;
}

OSQPWrapper::OptimizatorSolver::~OptimizatorSolver()
{
    if(m_workspace != nullptr)
        osqp_cleanup(m_workspace);
}

bool OSQPWrapper::OptimizatorSolver::solve()
{
    return (osqp_solve(m_workspace) == 0);
}

Eigen::VectorXd OSQPWrapper::OptimizatorSolver::getSolution()
{
    Eigen::VectorXd vector;

    // copy data from an array to Eigen vector
    c_float* solution = m_workspace->solution->x;
    vector = Eigen::Map<Eigen::VectorXd>(solution, m_workspace->data->n, 1);

    return vector;
}

const std::unique_ptr<OSQPWrapper::OptimizatorSettings>& OSQPWrapper::OptimizatorSolver::settings() const
{
    return m_settings;
}

const std::unique_ptr<OSQPWrapper::OptimizatorData>& OSQPWrapper::OptimizatorSolver::initData() const
{
    return m_initData;
}

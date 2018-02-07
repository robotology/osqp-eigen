/**
 * @file OptimizatorWorkspace.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// OSQPWrapper
#include "OptimizatorData.hpp"
#include "OptimizatorSettings.hpp"
#include "OptimizatorWorkspace.hpp"

OSQPWrapper::OptimizatorWorkspace::OptimizatorWorkspace()
{
    m_workspace = nullptr;
}

bool OSQPWrapper::OptimizatorWorkspace::setWorkspace(const OSQPWrapper::OptimizatorData& data,
                                                     const OSQPWrapper::OptimizatorSettings& settings)
{
    if(!data.isSet()){
        std::cout << "[Optimizator Workspace] Some data are not set." << std::endl;
        return false;
    }

    m_workspace = osqp_setup(data.getOptimizatorData(), settings.getOptimizatorSettings());
    return true;
}


OSQPWrapper::OptimizatorWorkspace::~OptimizatorWorkspace()
{
    if(m_workspace != nullptr)
        osqp_cleanup(m_workspace);
}

bool OSQPWrapper::OptimizatorWorkspace::solve()
{
    return !osqp_solve(m_workspace);
}

Eigen::VectorXd OSQPWrapper::OptimizatorWorkspace::getSolution()
{
    Eigen::VectorXd vector;

    // copy data from an array to Eigen vector
    c_float* solution = m_workspace->solution->x;
    vector = Eigen::Map<Eigen::VectorXd>(solution, m_workspace->data->n, 1);

    return vector;
}

bool OSQPWrapper::OptimizatorWorkspace::updateHessianMatrix(const OSQPWrapper::SparseMatrix& hessian)
{
    m_workspace->data->P = hessian.getSparseMatrix();
}

bool OSQPWrapper::OptimizatorWorkspace::updateLinearConstraintsMatrix(const OSQPWrapper::SparseMatrix& A)
{
    m_workspace->data->A = A.getSparseMatrix();
}

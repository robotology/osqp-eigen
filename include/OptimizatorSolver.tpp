/**
 * @file OptimizatorSolver.tpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include <iostream>

template<int n>
bool OSQPWrapper::OptimizatorSolver::updateGradient(Eigen::Matrix<c_float, n, 1>& gradient)
{
    // check if the dimension of the gradient is correct
    if(gradient.rows() != m_workspace->data->n){
        std::cerr << "[Optimizator Workspace] The size of the gradient must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the gradient vector
    if(!osqp_update_lin_cost(m_workspace, gradient.data())){
        std::cerr << "[Optimizator Workspace] Error when the update gradient is called."
                  << std::endl;
        return false;
    }
    return true;
}

template<int m>
bool OSQPWrapper::OptimizatorSolver::updateLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound)
{
    // check if the dimension of the lowerBound vector is correct
    if(lowerBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizator Workspace] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the lower bound vector
    if(osqp_update_lower_bound(m_workspace, lowerBound.data())){
        std::cerr << "[Optimizator Workspace] Error when the update lower bound is called."
                  << std::endl;
        return false;
    }

    return true;
}

template<int m>
bool OSQPWrapper::OptimizatorSolver::updateUpperBound(Eigen::Matrix<c_float, m, 1>& upperBound)
{
    // check if the dimension of the upperBound vector is correct
    if(upperBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizator Workspace] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the upper bound vector
    if(osqp_update_upper_bound(m_workspace, upperBound.data())){
        std::cerr << "[Optimizator Workspace] Error when the update upper bound is called."
                  << std::endl;
        return false;
    }
    return true;
}


template<int m>
bool OSQPWrapper::OptimizatorSolver::updateBounds(Eigen::Matrix<c_float, m, 1>& lowerBound,
                                                  Eigen::Matrix<c_float, m, 1>& upperBound)
{
    // check if the dimension of the upperBound vector is correct
    if(upperBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizator Workspace] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // check if the dimension of the lowerBound vector is correct
    if(lowerBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizator Workspace] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update lower and upper constraints
    if(osqp_update_bounds(m_workspace, lowerBound.data(), upperBound.data())){
        std::cerr << "[Optimizator Workspace] Error when the update bounds is called."
                  << std::endl;
        return false;
    }
    return true;
}

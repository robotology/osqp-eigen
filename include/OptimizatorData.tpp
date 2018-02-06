/**
 * @file OptimizatorData.tpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include <iostream>

template<int n>
bool OSQPWrapper::OptimizatorData::setGradient(Eigen::Matrix<c_float, n, 1>& gradient)
{
    if(gradient.rows() != m_data->n){
        std::cerr << "[Optimizator Data] The size of the gradient must be equal to the number of the variables."
                  << std::endl;
        return false;
    }
    m_data->q = gradient.data();
    return true;
}

template<int m>
bool OSQPWrapper::OptimizatorData::setLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound)
{
    if(lowerBound.rows() != m_data->m){
        std::cerr << "[Optimizator Data] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }
    m_data->l = lowerBound.data();
    return true;
}

template<int m>
bool OSQPWrapper::OptimizatorData::setUpperBound(Eigen::Matrix<c_float, m, 1>& upperBound)
{
    if(upperBound.rows() != m_data->m){
        std::cerr << "[Optimizator Data] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }
    m_data->u = upperBound.data();
    return true;
}

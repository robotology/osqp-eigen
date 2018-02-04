/**
 * @file OptimizatorData.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// std
#include <iostream>
#include <utility>

// OSQPWrapper
#include "SparseMatrix.hpp"
#include "OptimizatorData.hpp"

OSQPWrapper::OptimizatorData::OptimizatorData()
{
    m_data = (OSQPData *)c_malloc(sizeof(OSQPData));
}

OSQPWrapper::OptimizatorData::OptimizatorData(int n, int m)
{
    m_data = (OSQPData *)c_malloc(sizeof(OSQPData));
    setNumberOfVariables(n);
    setNumberOfConstraints(m);
}

OSQPWrapper::OptimizatorData::~OptimizatorData()
{
    c_free(m_data);
}

void OSQPWrapper::OptimizatorData::setNumberOfVariables(int n)
{
    m_data->n = n;
}

void OSQPWrapper::OptimizatorData::setNumberOfConstraints(int m)
{
    m_data->m = m;
}

bool OSQPWrapper::OptimizatorData::setHessianMatrix(const OSQPWrapper::SparseMatrix& hessian)
{
    std::pair<c_int, c_int> matrixSize = hessian.size();
    if ((std::get<0>(matrixSize) != m_data->n) || (std::get<1>(matrixSize) != m_data->n)){
        std::cerr << "[Optimizator Data] The Hessian matrix must have n x n size."
                  << std::endl;
        return false;
    }
    m_data->P = hessian.getSparseMatrix();
    return true;
}

bool OSQPWrapper::OptimizatorData::setLinearConstraintsMatrix(const OSQPWrapper::SparseMatrix& A)
{
    std::pair<c_int, c_int> matrixSize = A.size();
    if ((std::get<0>(matrixSize) != m_data->m) || (std::get<1>(matrixSize) != m_data->n)){
        std::cerr << "[Optimizator Data] The Linear constraints matrix matrix must have m x n size."
                  << std::endl;
        return false;
    }
    m_data->A = A.getSparseMatrix();
    return true;
}

OSQPData* const & OSQPWrapper::OptimizatorData::getOptimizatorData() const
{
    return m_data;
}

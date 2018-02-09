/**
 * @file OptimizatorData.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// std
#include <iostream>

// OSQPWrapper
#include "OptimizatorData.hpp"

OSQPWrapper::OptimizatorData::OptimizatorData()
    : m_isNumberOfVariablesSet(false),
      m_isNumberOfConstraintsSet(false),
      m_isHessianMatrixSet(false),
      m_isGradientSet(false),
      m_isLinearConstraintsMatrixSet(false),
      m_isLowerBoundSet(false),
      m_isUpperBoundSet(false)
{
    m_data = (OSQPData *)c_malloc(sizeof(OSQPData));
    m_data->P = nullptr;
    m_data->A = nullptr;
}

OSQPWrapper::OptimizatorData::OptimizatorData(int n, int m)
    : m_isNumberOfVariablesSet(true),
      m_isNumberOfConstraintsSet(true),
      m_isHessianMatrixSet(false),
      m_isGradientSet(false),
      m_isLinearConstraintsMatrixSet(false),
      m_isLowerBoundSet(false),
      m_isUpperBoundSet(false)
{
    m_data = (OSQPData *)c_malloc(sizeof(OSQPData));
    m_data->P = nullptr;
    m_data->A = nullptr;

    setNumberOfVariables(n);
    setNumberOfConstraints(m);
}

OSQPWrapper::OptimizatorData::~OptimizatorData()
{
    c_free(m_data);
    if(m_data->P != nullptr)
        csc_spfree(m_data->P);

    if(m_data->A != nullptr)
        csc_spfree(m_data->A);
}

void OSQPWrapper::OptimizatorData::setNumberOfVariables(int n)
{
    m_isNumberOfVariablesSet = true;
    m_data->n = n;
}

void OSQPWrapper::OptimizatorData::setNumberOfConstraints(int m)
{
    m_isNumberOfConstraintsSet = true;
    m_data->m = m;
}

OSQPData* const & OSQPWrapper::OptimizatorData::getOptimizatorData() const
{
    return m_data;
}

bool OSQPWrapper::OptimizatorData::isSet() const
{
    return m_isNumberOfVariablesSet &&
        m_isNumberOfConstraintsSet &&
        m_isHessianMatrixSet &&
        m_isGradientSet &&
        m_isLinearConstraintsMatrixSet &&
        m_isLowerBoundSet &&
        m_isUpperBoundSet;
}

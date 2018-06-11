/**
 * @file Data.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// std
#include <iostream>

// OsqpEigen
#include <OsqpEigen/Data.hpp>

OsqpEigen::Data::Data()
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

OsqpEigen::Data::Data(int n, int m)
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

void OsqpEigen::Data::clearHessianMatrix()
{
    if(m_isHessianMatrixSet){
        m_isHessianMatrixSet = false;
        csc_spfree(m_data->P);
        m_data->P = nullptr;
    }
}

void OsqpEigen::Data::clearLinearConstraintsMatrix()
{
    if(m_isLinearConstraintsMatrixSet){
        m_isLinearConstraintsMatrixSet = false;
        csc_spfree(m_data->A);
        m_data->A = nullptr;
    }
}

OsqpEigen::Data::~Data()
{
    clearHessianMatrix();
    clearLinearConstraintsMatrix();
    c_free(m_data);
}

void OsqpEigen::Data::setNumberOfVariables(int n)
{
    m_isNumberOfVariablesSet = true;
    m_data->n = n;
}

void OsqpEigen::Data::setNumberOfConstraints(int m)
{
    m_isNumberOfConstraintsSet = true;
    m_data->m = m;
}

OSQPData* const & OsqpEigen::Data::getData() const
{
    return m_data;
}

bool OsqpEigen::Data::isSet() const
{
    return m_isNumberOfVariablesSet &&
        m_isNumberOfConstraintsSet &&
        m_isHessianMatrixSet &&
        m_isGradientSet &&
        m_isLinearConstraintsMatrixSet &&
        m_isLowerBoundSet &&
        m_isUpperBoundSet;
}

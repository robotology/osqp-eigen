/**
 * @file SparseMatrix.cpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include "SparseMatrix.hpp"

OSQPWrapper::SparseMatrix::SparseMatrix()
    : m_matrix(nullptr)
{}

OSQPWrapper::SparseMatrix::~SparseMatrix()
{
    // dellocate memory
    if(!(m_matrix == nullptr))
        c_free(m_matrix);
}

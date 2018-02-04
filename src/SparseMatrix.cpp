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

csc* const & OSQPWrapper::SparseMatrix::getSparseMatrix() const
{
    return m_matrix;
}

std::pair<c_int, c_int> OSQPWrapper::SparseMatrix::size() const
{
    // get the matrix size
    std::pair<c_int, c_int> size = std::make_pair(m_matrix->m, m_matrix->n);
    return size;
}

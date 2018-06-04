/**
 * @file OptimizerData.tpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include <iostream>

template<typename T>
bool OSQPWrapper::OptimizerData::setHessianMatrix(const Eigen::SparseMatrix<T> &hessianMatrix)
{
    if(m_isHessianMatrixSet){
        std::cerr << "[setHessianMatrix] The hessian matrix was already set. "
                  << "Please use clearHessianMatrix() method to deallocate memory."
                  << std::endl;
        return false;
    }

    if(!m_isNumberOfVariablesSet){
        std::cerr << "[setHessianMatrix] Please set the number of variables before "
                  << "add the hessian matrix."
                  << std::endl;
        return false;
    }

    // check if the number of row and columns are equal to the number of the optimization variables
    if ((hessianMatrix.rows() != m_data->n) || (hessianMatrix.cols()!= m_data->n)){
        std::cerr << "[setHessianMatrix] The Hessian matrix has to be a n x n size matrix."
                  << std::endl;
        return false;
    }

    //set the hessian matrix
    if(!OSQPWrapper::SparseMatrixHelper::createOsqpSparseMatrix(hessianMatrix, m_data->P)){
        std::cerr << "[setHessianMatrix] Unable to instantiate the osqp sparse matrix."
                  << std::endl;
        return false;
    }
    m_isHessianMatrixSet = true;
    return true;
}

template<int n>
bool OSQPWrapper::OptimizerData::setGradient(Eigen::Matrix<c_float, n, 1>& gradient)
{
    if(gradient.rows() != m_data->n){
        std::cerr << "[Optimizer Data] The size of the gradient must be equal to the number of the variables."
                  << std::endl;
        return false;
    }
    m_isGradientSet = true;
    m_data->q = gradient.data();
    return true;
}

template<typename T>
bool OSQPWrapper::OptimizerData::setLinearConstraintsMatrix(const Eigen::SparseMatrix<T> &linearConstraintsMatrix)
{
    if(m_isLinearConstraintsMatrixSet){
        std::cerr << "[setLinearConstraintsMatrix] The linear constraint matrix was already set. "
                  << "Please use clearLinearConstraintsMatrix() method to deallocate memory."
                  << std::endl;
        return false;
    }

    if(!m_isNumberOfConstraintsSet){
        std::cerr << "[setLinearConstraintMatrix] Please set the number of constraints before add the constraint matrix."
                  << std::endl;
        return false;
    }

    if(!m_isNumberOfVariablesSet){
        std::cerr << "[setLinearConstraintMatrix] Please set the number of variables before add the constraint matrix."
                  << std::endl;
        return false;
    }

    if ((linearConstraintsMatrix.rows() != m_data->m) || (linearConstraintsMatrix.cols()!= m_data->n)){
        std::cerr << "[setLinearConstraintMatrix] The Linear constraints matrix has to be a m x n size matrix."
                  << std::endl;
        return false;
    }

    // set the hessian matrix
    if(!OSQPWrapper::SparseMatrixHelper::createOsqpSparseMatrix(linearConstraintsMatrix,
                                                                m_data->A)){
        std::cerr << "[setLinearConstraintMatrix] osqp sparse matrix not created."
                  << std::endl;
        return false;
    }

    m_isLinearConstraintsMatrixSet = true;

    return true;
}

template<int m>
bool OSQPWrapper::OptimizerData::setLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound)
{
    if(lowerBound.rows() != m_data->m){
        std::cerr << "[setLowerBound] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }
    m_isLowerBoundSet = true;
    m_data->l = lowerBound.data();
    return true;
}

template<int m>
bool OSQPWrapper::OptimizerData::setUpperBound(Eigen::Matrix<c_float, m, 1>& upperBound)
{
    if(upperBound.rows() != m_data->m){
        std::cerr << "[setUpperBound] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }
    m_isUpperBoundSet = true;
    m_data->u = upperBound.data();
    return true;
}

/**
 * @file OptimizerSolver.tpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include <iostream>
#include "auxil.h"
#include "scaling.h"
template<int n>
bool OSQPWrapper::OptimizerSolver::updateGradient(Eigen::Matrix<c_float, n, 1>& gradient)
{
    // check if the dimension of the gradient is correct
    if(gradient.rows() != m_workspace->data->n){
        std::cerr << "[Optimizer Workspace] The size of the gradient must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the gradient vector
    if(osqp_update_lin_cost(m_workspace, gradient.data())){
        std::cerr << "[Optimizer Workspace] Error when the update gradient is called."
                  << std::endl;
        return false;
    }
    return true;
}

template<int m>
bool OSQPWrapper::OptimizerSolver::updateLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound)
{
    // check if the dimension of the lowerBound vector is correct
    if(lowerBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizer Workspace] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the lower bound vector
    if(osqp_update_lower_bound(m_workspace, lowerBound.data())){
        std::cerr << "[Optimizer Workspace] Error when the update lower bound is called."
                  << std::endl;
        return false;
    }

    return true;
}

template<int m>
bool OSQPWrapper::OptimizerSolver::updateUpperBound(Eigen::Matrix<c_float, m, 1>& upperBound)
{
    // check if the dimension of the upperBound vector is correct
    if(upperBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizer Workspace] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the upper bound vector
    if(osqp_update_upper_bound(m_workspace, upperBound.data())){
        std::cerr << "[Optimizer Workspace] Error when the update upper bound is called."
                  << std::endl;
        return false;
    }
    return true;
}


template<int m>
bool OSQPWrapper::OptimizerSolver::updateBounds(Eigen::Matrix<c_float, m, 1>& lowerBound,
                                                  Eigen::Matrix<c_float, m, 1>& upperBound)
{
    // check if the dimension of the upperBound vector is correct
    if(upperBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizer Workspace] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // check if the dimension of the lowerBound vector is correct
    if(lowerBound.rows() != m_workspace->data->m){
        std::cerr << "[Optimizer Workspace] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update lower and upper constraints
    if(osqp_update_bounds(m_workspace, lowerBound.data(), upperBound.data())){
        std::cerr << "[Optimizer Workspace] Error when the update bounds is called."
                  << std::endl;
        return false;
    }
    return true;
}

template<typename T>
bool OSQPWrapper::OptimizerSolver::updateHessianMatrix(const Eigen::SparseMatrix<T> &hessianMatrix)
{
    if(!m_isSolverInitialized){
        std::cerr << "[updateHessianMatrix] The solver has not been initialized."
                  << std::endl;
        return false;
    }

    if(((c_int)hessianMatrix.rows() != m_workspace->data->n)||
       ((c_int)hessianMatrix.cols() != m_workspace->data->n)){
        std::cerr << "[updateHessianMatrix] The hessian matrix has to be a nxn matrix"
                  << std::endl;
        return false;
    }


    // evaluate the triplets from old and new hessian sparse matrices
    std::vector<Eigen::Triplet<T>> oldHessianTriplet, newHessianTriplet;
    if(!OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToTriplets(m_workspace->data->P,
                                                                    oldHessianTriplet)){
        std::cerr << "[updateHessianMatrix] Unable to evaluate triplets from the old hessian matrix."
                  << std::endl;
        return false;
    }
    if(!OSQPWrapper::SparseMatrixHelper::eigenSparseMatrixToTriplets(hessianMatrix,
                                                                     newHessianTriplet)){
        std::cerr << "[updateHessianMatrix] Unable to evaluate triplets from the old hessian matrix."
                  << std::endl;
        return false;
    }

    std::vector<Eigen::Triplet<T>> newUpperTriangularHessianTriplets;
    selectUpperTriangularTriplets(newHessianTriplet, newUpperTriangularHessianTriplets);

    // try to update the hessian matrix without reinitialize the solver
    // according to the osqp library it can be done only if the sparsity pattern of the hessian
    // matrix does not change.
    std::vector<c_int> newIndices;
    std::vector<c_float> newValues;

    if(evaluateNewValues(oldHessianTriplet,  newUpperTriangularHessianTriplets,
                         newIndices, newValues)){
        if(osqp_update_P(m_workspace, newValues.data(), newIndices.data(), newIndices.size()) != 0){
            std::cerr << "[updateHessianMatrix] Unable to update hessian matrix."
                      << std::endl;
            return false;
        }
    }
    else{
        // the sparsity pattern has changed
        // the optimizer solver has to be setup again

        // get the primal and the dual variables
        Eigen::Matrix<c_float, Eigen::Dynamic ,1> primalVariable;
        Eigen::Matrix<c_float, Eigen::Dynamic ,1> dualVariable;

        if(!getPrimalVariable(primalVariable)){
            std::cerr << "[updateHessianMatrix] Unable to get the primal variable."
                      << std::endl;
            return false;
        }

        if(!getDualVariable(dualVariable)){
            std::cerr << "[updateHessianMatrix] Unable to get the dual variable."
                      << std::endl;
            return false;
        }

        // clear old hessian matrix
        m_data->clearHessianMatrix();

        // set new hessian matrix
        if(!m_data->setHessianMatrix(hessianMatrix)){
            std::cerr << "[updateHessianMatrix] Unable to update the hessian matrix in "
                      << "OptimizaroData object."
                      << std::endl;
            return false;
        }

        // clear the old solver
        clearSolver();

        // initialize a new solver
        initSolver();

        // set the old primal and dual variables
        if(!setPrimalVariable(primalVariable)){
            std::cerr << "[updateHessianMatrix] Unable to set the primal variable."
                      << std::endl;
            return false;
        }

        if(!setDualVariable(dualVariable)){
            std::cerr << "[updateHessianMatrix] Unable to set the dual variable."
                      << std::endl;
            return false;
        }
    }
    return true;
}

template<typename T>
bool OSQPWrapper::OptimizerSolver::updateLinearConstraintsMatrix(const Eigen::SparseMatrix<T> &linearConstraintsMatrix)
{
    if(!m_isSolverInitialized){
        std::cerr << "[updateLinearConstraintMatrix] The solver has not been initialized."
                  << std::endl;
        return false;
    }

    if(((c_int)linearConstraintsMatrix.rows() != m_workspace->data->m)||
       ((c_int)linearConstraintsMatrix.cols() != m_workspace->data->n)){
        std::cerr << "[updateLinearConstraintMatrix] The constraints matrix has to be a mxn matrix"
                  << std::endl;
        return false;
    }

    // evaluate the triplets from old and new hessian sparse matrices
    std::vector<Eigen::Triplet<T>> oldLinearConstraintsTriplet, newLinearConstraintsTriplet;

    if(!OSQPWrapper::SparseMatrixHelper::osqpSparseMatrixToTriplets(m_workspace->data->A,
                                                                    oldLinearConstraintsTriplet)){
        std::cerr << "[updateLinearConstraintMatrix] Unable to evaluate triplets from the old hessian matrix."
                  << std::endl;
        return false;
    }
    if(!OSQPWrapper::SparseMatrixHelper::eigenSparseMatrixToTriplets(linearConstraintsMatrix,
                                                                     newLinearConstraintsTriplet)){
        std::cerr << "[updateLinearConstraintMatrix] Unable to evaluate triplets from the old hessian matrix."
                  << std::endl;
        return false;
    }

    // try to update the linear constraints matrix without reinitialize the solver
    // according to the osqp library it can be done only if the sparsity pattern of the
    // matrix does not change.
    std::vector<c_int> newIndices;
    std::vector<c_float> newValues;

    if(evaluateNewValues(oldLinearConstraintsTriplet, newLinearConstraintsTriplet,
                         newIndices, newValues)){
        if(osqp_update_A(m_workspace, newValues.data(), newIndices.data(), newIndices.size()) != 0){
            std::cerr << "[updateLinearConstraintMatrix] Unable to update linear constraints matrix."
                      << std::endl;
            return false;
        }
    }
    else{
        // the sparsity pattern has changed
        // the optimizer solver has to be setup again

        // get the primal and the dual variables
        Eigen::Matrix<c_float, Eigen::Dynamic ,1> primalVariable;
        Eigen::Matrix<c_float, Eigen::Dynamic ,1> dualVariable;

        if(!getPrimalVariable(primalVariable)){
            std::cerr << "[updateLinearConstraintMatrix] Unable to get the primal variable."
                      << std::endl;
            return false;
        }

        if(!getDualVariable(dualVariable)){
            std::cerr << "[updateLinearConstraintMatrix] Unable to get the dual variable."
                      << std::endl;
            return false;
        }

        // clear old linear constraints matrix
        m_data->clearLinearConstraintsMatrix();

        // set new linear constraints matrix
        if(!m_data->setLinearConstraintsMatrix(linearConstraintsMatrix)){
            std::cerr << "[updateLinearConstraintMatrix] Unable to update the hessian matrix in "
                      << "OptimizaroData object."
                      << std::endl;
            return false;
        }

        // clear the old solver
        clearSolver();

        // initialize a new solver
        initSolver();

        // set the old primal and dual variables
        if(!setPrimalVariable(primalVariable)){
            std::cerr << "[updateLinearConstraintMatrix] Unable to set the primal variable."
                      << std::endl;
            return false;
        }

        if(!setDualVariable(dualVariable)){
            std::cerr << "[updateLinearConstraintMatrix] Unable to set the dual variable."
                      << std::endl;
            return false;
        }
    }
    return true;
}

template<typename T, int n, int m>
bool OSQPWrapper::OptimizerSolver::setWarmStart(const Eigen::Matrix<T, n, 1> &primalVariable,
                                                  const Eigen::Matrix<T, m, 1> &dualVariable)
{
    if(primalVariable.rows() != m_workspace->data->n){
        std::cerr << "[setWarmStart] The size of the primal variable vector has to be equal to "
                  << " the number of variables."
                  << std::endl;
        return false;
    }

    if(dualVariable.rows() != m_workspace->data->m){
        std::cerr << "[setWarmStart] The size of the dual variable vector has to be equal to "
                  << " the number of constraints."
                  << std::endl;
        return false;
    }

    Eigen::Matrix<c_float, n, 1> primalVariableCast = primalVariable.template cast <c_float>();
    Eigen::Matrix<c_float, n, 1> dualVariableCast = dualVariable.template cast <c_float>();

    return (osqp_warm_start(m_workspace, primalVariableCast.data(), dualVariableCast.data()) == 0);

}

template<typename T, int n>
bool OSQPWrapper::OptimizerSolver::setPrimalVariable(const Eigen::Matrix<T, n, 1> &primalVariable)
{
    if(primalVariable.rows() != m_workspace->data->n){
        std::cerr << "[setPrimalVariable] The size of the primal variable vector has to be equal to "
                  << " the number of variables."
                  << std::endl;
        return false;
    }

    Eigen::Matrix<c_float, n, 1> primalVariableCast = primalVariable.template cast <c_float>();

    return (osqp_warm_start_x(m_workspace, primalVariableCast.data()) == 0);
}


template<typename T, int m>
bool OSQPWrapper::OptimizerSolver::setDualVariable(const Eigen::Matrix<T, m, 1> &dualVariable)
{
    if(dualVariable.rows() != m_workspace->data->m){
        std::cerr << "[setDualVariable] The size of the dual variable vector has to be equal to "
                  << " the number of constraints."
                  << std::endl;
        return false;
    }

    Eigen::Matrix<c_float, m, 1> dualVariableCast = dualVariable.template cast <c_float>();

    return (osqp_warm_start_y(m_workspace, dualVariableCast.data()) == 0);


}

template<typename T, int n>
bool OSQPWrapper::OptimizerSolver::getPrimalVariable(Eigen::Matrix<T, n, 1> &primalVariable)
{
    if(n == Eigen::Dynamic){
        primalVariable.resize(m_workspace->data->n, 1);
    }
    else{
        if (n != m_workspace->data->n){
            std::cerr << "[getPrimalVariable] The size of the vector has to be equal to the number of "
                      << "variables. (You can use an eigen dynamic vector)"
                      << std::endl;
            return false;
        }
    }
    for(int i = 0; i< m_workspace->data->n; i++){
        primalVariable(i,0) = (T)m_workspace->x[i];
    }
    return true;
}

template<typename T, int m>
bool OSQPWrapper::OptimizerSolver::getDualVariable(Eigen::Matrix<T, m, 1> &dualVariable)
{
    if(m == Eigen::Dynamic){
        dualVariable.resize(m_workspace->data->m, 1);
    }
    else{
        if (m != m_workspace->data->m){
            std::cerr << "[getdualVariable] The size of the vector has to be equal to the number of "
                      << "constraints. (You can use an eigen dynamic vector)"
                      << std::endl;
            return false;
        }
    }

    for(int i = 0; i< m_workspace->data->m; i++){
        dualVariable(i,0) =  (T)m_workspace->y[i];
    }
    return true;
}

template<typename T>
bool OSQPWrapper::OptimizerSolver::evaluateNewValues(const std::vector<Eigen::Triplet<T>> &oldMatrixTriplet,
                                                     const std::vector<Eigen::Triplet<T>> &newMatrixTriplet,
                                                     std::vector<c_int> &newIndices,
                                                     std::vector<c_float> &newValues) const
{
    // check if the sparsity pattern is changed
    if(newMatrixTriplet.size() == oldMatrixTriplet.size()){
        for(int i = 0; i < newMatrixTriplet.size(); i++){
            // check if the sparsity pattern is changed
            if((newMatrixTriplet[i].row() != oldMatrixTriplet[i].row()) ||
               (newMatrixTriplet[i].col() != oldMatrixTriplet[i].col()))
                return false;

            // check if an old value is changed
            if(newMatrixTriplet[i].value() != oldMatrixTriplet[i].value()){
                newValues.push_back((c_float) newMatrixTriplet[i].value());
                newIndices.push_back((c_int) i);
            }
        }
        return true;
    }
    return false;
}

template<typename T>
void OSQPWrapper::OptimizerSolver::selectUpperTriangularTriplets(const std::vector<Eigen::Triplet<T>> &fullMatrixTriplets,
                                                                 std::vector<Eigen::Triplet<T>> &upperTriangularMatrixTriplets) const {

    int upperTriangularTriplets = 0;
    for (int i = 0; i < fullMatrixTriplets.size(); ++i) {
        if (fullMatrixTriplets[i].row() >= fullMatrixTriplets[i].col()) {
            if (upperTriangularTriplets < upperTriangularMatrixTriplets.size()) {
                upperTriangularMatrixTriplets[upperTriangularTriplets] = fullMatrixTriplets[i];
            } else {
                upperTriangularMatrixTriplets.push_back(fullMatrixTriplets[i]);
            }
            upperTriangularTriplets++;
        }
    }

    upperTriangularMatrixTriplets.erase(upperTriangularMatrixTriplets.begin() + upperTriangularTriplets, upperTriangularMatrixTriplets.end());
}

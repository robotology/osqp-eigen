/**
 * @file OptimizerSolver.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */
#ifndef OPTIMIZER_WORKSPACE_HPP
#define OPTIMIZER_WORKSPACE_HPP

// Std
#include <memory>

// Eigen
#include <Eigen/Dense>

// OSQP
#include "osqp.h"

// OSQPWrapper
#include "OptimizerData.hpp"
#include "OptimizerSettings.hpp"
#include "OptimizerConstants.hpp"

/**
 * OSQPWrapper namespace.
 */
namespace OSQPWrapper
{
    /**
     * OptimizerSolver class is a wrapper of the OSQP OSQPWorkspace struct.
     */
    class OptimizerSolver
    {
        OSQPWorkspace *m_workspace;  /**< OSQPWorkspace struct. */
        std::unique_ptr<OSQPWrapper::OptimizerSettings> m_settings; /**< Pointer to OptimizerSettings class. */
        std::unique_ptr<OSQPWrapper::OptimizerData> m_data; /**< Pointer to OptimizerData class. */
        Eigen::Matrix<c_float, Eigen::Dynamic ,1> m_primalVariables;
        Eigen::Matrix<c_float, Eigen::Dynamic ,1> m_dualVariables;
        Eigen::VectorXd m_solution;

        std::vector<c_int> m_hessianNewIndices;
        std::vector<c_float> m_hessianNewValues;

        std::vector<c_int> m_constraintsNewIndices;
        std::vector<c_float> m_constraintsNewValues;

        std::vector<Eigen::Triplet<c_float>> m_oldHessianTriplet, m_newHessianTriplet, m_newUpperTriangularHessianTriplets;
        std::vector<Eigen::Triplet<c_float>> m_oldLinearConstraintsTriplet, m_newLinearConstraintsTriplet;



        bool m_isSolverInitialized; /**< Boolean true if solver is initialized. */

        /**
         * Evaluate the position and the values of the new elements of a sparse matrix.
         * @param oldMatrixTriplet vector containing the triplets of the old sparse matrix;
         * @param newMatrixTriplet vector containing the triplets of the mew sparse matrix;
         * @param newIndices vector of the index mapping new elements
         * to position in the sparse matrix;
         * @param newValues vector of new elements in the sparse matrix.
         * @return true if the sparsity patern is not changed false otherwise.
         */
        template<typename T>
        bool evaluateNewValues(const std::vector<Eigen::Triplet<T>> &oldMatrixTriplet,
                               const std::vector<Eigen::Triplet<T>> &newMatrixTriplet,
                               std::vector<c_int> &newIndices,
                               std::vector<c_float> &newValues) const;

        /**
         * Takes only the triplets which belongs to the upper triangular part of the matrix.
         * @param fullMatrixTriplets vector containing the triplets of the sparse matrix;
         * @param upperTriangularMatrixTriplets vector containing the triplets of the mew sparse matrix;
         */
        template<typename T>
        void selectUpperTriangularTriplets(const std::vector<Eigen::Triplet<T>> &fullMatrixTriplets,
                                           std::vector<Eigen::Triplet<T>> &upperTriangularMatrixTriplets) const;

    public:

        /**
         * Constructor.
         */
        OptimizerSolver();

        /**
         * Deconstructor.
         */
        ~OptimizerSolver();

        /**
         * Initialize the solver with the actual initial data and settings.
         * @return true/false in case of success/failure.
         */
        bool initSolver();

        /**
         * Check if the solver is initialized.
         * @return true if the solver is initialized.
         */
        bool isInitialized();

        /**
         * Dealocate memory.
         */
        void clearSolver();

        /**
         * Set to zero all the solver variables.
         * @return true/false in case of success/failure.
         */
        bool clearSolverVariables();

        /**
         * Solve the QP optimization problem.
         * @return true/false in case of success/failure.
         */
        bool solve();

        /**
         * Get the optimization problem solution.
         * @return an Eigen::Vector contating the optimization result.
         */
        const Eigen::VectorXd &getSolution();

        /**
         * Update the linear part of the cost function (Gradient).
         * @param gradient is the Gradient vector.
         * @return true/false in case of success/failure.
         */
        template<int n>
        bool updateGradient(Eigen::Matrix<c_float, n, 1>& gradient);

        /**
         * Update the lower bounds limit (size m).
         * @param lowerBound is the lower bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateLowerBound(Eigen::Matrix<c_float, m, 1>& lowerBound);

        /**
         * Update the upper bounds limit (size m).
         * @param upperBound is the upper bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateUpperBound(Eigen::Matrix<c_float, m, 1>& upperBound);

        /**
         * Update both upper and lower bounds (size m).
         * @param lowerBound is the lower bound constraint vector;
         * @param upperBound is the upper bound constraint vector.
         * @return true/false in case of success/failure.
         */
        template<int m>
        bool updateBounds(Eigen::Matrix<c_float, m, 1>& lowerBound,
                          Eigen::Matrix<c_float, m, 1>& upperBound);

        /**
         * Update the quadratic part of the cost function (Hessian).
         * It is assumed to be a simmetric matrix.
         * \note
         * If the sparsity pattern is preserved the matrix is simply update
         * otherwise the entire solver will be reinitialized. In this case
         * the primal and dual variable are copied in the new workspace.
         *
         * @param hessian is the Hessian matrix.
         * @return true/false in case of success/failure.
         */
        template<typename T>
        bool updateHessianMatrix(const Eigen::SparseMatrix<T> &hessianMatrix);

        /**
         * Update the linear constraints matrix (A)
         * \note
         * If the sparsity pattern is preserved the matrix is simply update
         * otherwise the entire solver will be reinitialized. In this case
         * the primal and dual variable are copied in the new workspace.
         *
         * @param linearConstraintsMatrix is the linear constraint matrix A
         * @return true/false in case of success/failure.
         */
        template<typename T>
        bool updateLinearConstraintsMatrix(const Eigen::SparseMatrix<T> &linearConstraintsMatrix);

        /**
         * Set the entire
         * @param linearConstraintsMatrix is the linear constraint matrix A
         * @return true/false in case of success/failure.
         */
        template<typename T, int n, int m>
        bool setWarmStart(const Eigen::Matrix<T, n, 1> &primalVariable,
                          const Eigen::Matrix<T, m, 1> &dualVariable);

        template<typename T, int n>
        bool setPrimalVariable(const Eigen::Matrix<T, n, 1> &primalVariable);

        template<typename T, int m>
        bool setDualVariable(const Eigen::Matrix<T, m, 1> &dualVariable);

        template<typename T, int n>
        bool getPrimalVariable(Eigen::Matrix<T, n, 1> &primalVariable);

        template<typename T, int m>
        bool getDualVariable(Eigen::Matrix<T, m, 1> &dualVariable);

        /**
         * Get the solver settings pointer.
         * @return the pointer to OptimizerSettings object.
         */
        const std::unique_ptr<OSQPWrapper::OptimizerSettings>& settings() const;

        /**
         * Get the pointer to the solver initial data.
         * @return the pointer to OptimizerData object.
         */
        const std::unique_ptr<OSQPWrapper::OptimizerData>& data() const;
    };

    #include "OptimizerSolver.tpp"
}

#endif

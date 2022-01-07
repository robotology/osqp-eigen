/**
 * @file Solver.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2018
 */

// OsqpEigen
#include <OsqpEigen/Data.hpp>
#include <OsqpEigen/Settings.hpp>
#include <OsqpEigen/Solver.hpp>
#include <OsqpEigen/Debug.hpp>

void OsqpEigen::Solver::OSQPWorkspaceDeleter(OSQPWorkspace* ptr) noexcept
{
    if(ptr != nullptr)
        osqp_cleanup(ptr);
}

OsqpEigen::Solver::Solver()
    : m_isSolverInitialized(false)
    , m_workspace{nullptr, Solver::OSQPWorkspaceDeleter}
{
    m_settings = std::make_unique<OsqpEigen::Settings>();
    m_data = std::make_unique<OsqpEigen::Data>();
}

bool OsqpEigen::Solver::clearSolverVariables()
{
    if(!m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::clearSolverVariables] Unable to clear the solver variables. "
                  << "Are you sure that the solver is initialized?"
                  << std::endl;
        return false;
    }

    for(int i = 0; i < m_workspace->data->n; i++){
        m_workspace->x[i] = 0;
        m_workspace->x_prev[i] = 0;

        m_workspace->Px[i] = 0;
        m_workspace->Aty[i] = 0;
        m_workspace->Atdelta_y[i] = 0;

        m_workspace->delta_x[i] = 0;
        m_workspace->Pdelta_x[i] = 0;
    }

    for(int i = 0; i < m_workspace->data->m; i++){
        m_workspace->z[i] = 0;
        m_workspace->z_prev[i] = 0;
        m_workspace->y[i] = 0;

        m_workspace->Ax[i] = 0;
        m_workspace->delta_y[i] = 0;

        m_workspace->Adelta_x[i] = 0;
    }

    for(int i = 0; i < m_workspace->data->n + m_workspace->data->m; i++){
        m_workspace->xz_tilde[i] = 0;
    }

    return true;
}

bool OsqpEigen::Solver::initSolver()
{
    if(m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::initSolver] The solver has been already initialized. "
                  << "Please use clearSolver() method to deallocate memory."
                  << std::endl;
        return false;
    }

    if(!m_data->isSet()){
        debugStream() << "[OsqpEigen::Solver::initSolver] Some data are not set."
                  << std::endl;
        return false;
    }

    // if the number of constraints is equal to zero the user may not
    // call setLinearConstraintsMatrix()
    if(m_data->getData()->m == 0)
    {
        if(m_data->getData()->A == nullptr)
        {
            // let's create the matrix manually. This is required by osqp. Please check
            // https://github.com/oxfordcontrol/osqp/issues/295
            Eigen::SparseMatrix<c_float> A(m_data->getData()->m, m_data->getData()->n);
            if(!m_data->setLinearConstraintsMatrix(A))
            {
                debugStream() << "[OsqpEigen::Solver::initSolver] Unable to set the empty linear constraint "
                              << "matrix in case of unconstrained optimization problem"
                              << std::endl;
                return false;
            }
        }
    }

    OSQPWorkspace* workspace;
    if(osqp_setup(&workspace, m_data->getData(),
                  m_settings->getSettings()) != 0 ){
        debugStream() << "[OsqpEigen::Solver::initSolver] Unable to setup the workspace."
                  << std::endl;
        return false;
    }

    m_workspace.reset(workspace);

    m_isSolverInitialized = true;
    return true;
}

bool OsqpEigen::Solver::isInitialized()
{
    return m_isSolverInitialized;
}

void OsqpEigen::Solver::clearSolver()
{
    if(m_isSolverInitialized){
        m_workspace.reset();
        m_isSolverInitialized = false;
    }
}

bool OsqpEigen::Solver::solve()
{
    if (this->solveProblem() != OsqpEigen::ErrorExitFlag::NoError) {
        debugStream() << "[OsqpEigen::Solver::solve] Unable to solve the problem."
                      << std::endl;
        return false;
    }

    // check if the solution is feasible
    if(this->getStatus() != OsqpEigen::Status::Solved) {
        debugStream() << "[OsqpEigen::Solver::solve] The solution is unfeasible."
                      << std::endl;
        return false;
    }

    return true;
}

OsqpEigen::Status OsqpEigen::Solver::getStatus() const
{
    return static_cast<OsqpEigen::Status>(m_workspace->info->status_val);
}

OsqpEigen::ErrorExitFlag OsqpEigen::Solver::solveProblem()
{
    if(!m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::solveProblem] The solve has not been initialized yet. "
                  << "Please call initSolver() method."
                  << std::endl;
        return OsqpEigen::ErrorExitFlag::WorkspaceNotInitError;
    }

    return static_cast<OsqpEigen::ErrorExitFlag>(osqp_solve(m_workspace.get()));
}

const Eigen::VectorXd &OsqpEigen::Solver::getSolution()
{
    // copy data from an array to Eigen vector
    c_float* solution = m_workspace->solution->x;
    m_solution = Eigen::Map<Eigen::VectorXd>(solution, m_workspace->data->n, 1);

    return m_solution;
}

const Eigen::VectorXd &OsqpEigen::Solver::getDualSolution()
{
    // copy data from an array to Eigen vector
    c_float* solution = m_workspace->solution->y;
    m_dualSolution = Eigen::Map<Eigen::VectorXd>(solution, m_workspace->data->m, 1);

    return m_dualSolution;
}

const std::unique_ptr<OsqpEigen::Settings>& OsqpEigen::Solver::settings() const
{
    return m_settings;
}

const std::unique_ptr<OsqpEigen::Data>& OsqpEigen::Solver::data() const
{
    return m_data;
}

const std::unique_ptr<OSQPWorkspace, std::function<void(OSQPWorkspace *)>>& OsqpEigen::Solver::workspace() const
{
    return m_workspace;
}

bool OsqpEigen::Solver::updateGradient(const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& gradient)
{
    if(!m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::updateGradient] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the gradient is correct
    if(gradient.rows() != m_workspace->data->n){
        debugStream() << "[OsqpEigen::Solver::updateGradient] The size of the gradient must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the gradient vector
    if(osqp_update_lin_cost(m_workspace.get(), gradient.data())){
        debugStream() << "[OsqpEigen::Solver::updateGradient] Error when the update gradient is called."
                  << std::endl;
        return false;
    }
    return true;
}

bool OsqpEigen::Solver::updateLowerBound(const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& lowerBound)
{
    if(!m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::updateLowerBound] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the lowerBound vector is correct
    if(lowerBound.rows() != m_workspace->data->m){
        debugStream() << "[OsqpEigen::Solver::updateLowerBound] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the lower bound vector
    if(osqp_update_lower_bound(m_workspace.get(), lowerBound.data())){
        debugStream() << "[OsqpEigen::Solver::updateLowerBound] Error when the update lower bound is called."
                  << std::endl;
        return false;
    }

    return true;
}

bool OsqpEigen::Solver::updateUpperBound(const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& upperBound)
{
    if(!m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::updateUpperBound] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the upperBound vector is correct
    if(upperBound.rows() != m_workspace->data->m){
        debugStream() << "[OsqpEigen::Solver::updateUpperBound] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update the upper bound vector
    if(osqp_update_upper_bound(m_workspace.get(), upperBound.data())){
        debugStream() << "[OsqpEigen::Solver::updateUpperBound] Error when the update upper bound is called."
                  << std::endl;
        return false;
    }
    return true;
}

bool OsqpEigen::Solver::updateBounds(const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& lowerBound,
                                     const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& upperBound)
{
    if(!m_isSolverInitialized){
        debugStream() << "[OsqpEigen::Solver::updateBounds] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the upperBound vector is correct
    if(upperBound.rows() != m_workspace->data->m){
        debugStream() << "[OsqpEigen::Solver::updateBounds] The size of the upper bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // check if the dimension of the lowerBound vector is correct
    if(lowerBound.rows() != m_workspace->data->m){
        debugStream() << "[OsqpEigen::Solver::updateBounds] The size of the lower bound must be equal to the number of the variables."
                  << std::endl;
        return false;
    }

    // update lower and upper constraints
    if(osqp_update_bounds(m_workspace.get(), lowerBound.data(), upperBound.data())){
        debugStream() << "[OsqpEigen::Solver::updateBounds] Error when the update bounds is called."
                  << std::endl;
        return false;
    }
    return true;
}

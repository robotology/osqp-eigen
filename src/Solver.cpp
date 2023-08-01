/**
 * @file Solver.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2018
 */

// OsqpEigen
#include <OsqpEigen/Data.hpp>
#include <OsqpEigen/Debug.hpp>
#include <OsqpEigen/Settings.hpp>
#include <OsqpEigen/Solver.hpp>

#ifdef OSQP_EIGEN_OSQP_IS_V1
void OsqpEigen::Solver::OSQPSolverDeleter(OSQPSolver* ptr) noexcept
#else
void OsqpEigen::Solver::OSQPWorkspaceDeleter(OSQPWorkspace* ptr) noexcept
#endif
{
    if (ptr != nullptr)
    {
        osqp_cleanup(ptr);
    }
}

OsqpEigen::Solver::Solver()
    : m_isSolverInitialized(false)
    ,
#ifdef OSQP_EIGEN_OSQP_IS_V1
    m_solver{nullptr, Solver::OSQPSolverDeleter}
#else
    m_workspace{nullptr, Solver::OSQPWorkspaceDeleter}
#endif
{
    m_settings = std::make_unique<OsqpEigen::Settings>();
    m_data = std::make_unique<OsqpEigen::Data>();
}

bool OsqpEigen::Solver::clearSolverVariables()
{
    if (!m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::clearSolverVariables] Unable to clear the solver "
                         "variables. "
                      << "Are you sure that the solver is initialized?" << std::endl;
        return false;
    }

#ifndef OSQP_EIGEN_OSQP_IS_V1
    for (int i = 0; i < getData()->n; i++)
    {
        m_workspace->x[i] = 0;
        m_workspace->x_prev[i] = 0;

        m_workspace->Px[i] = 0;
        m_workspace->Aty[i] = 0;
        m_workspace->Atdelta_y[i] = 0;

        m_workspace->delta_x[i] = 0;
        m_workspace->Pdelta_x[i] = 0;
    }

    for (int i = 0; i < getData()->m; i++)
    {
        m_workspace->z[i] = 0;
        m_workspace->z_prev[i] = 0;
        m_workspace->y[i] = 0;

        m_workspace->Ax[i] = 0;
        m_workspace->delta_y[i] = 0;

        m_workspace->Adelta_x[i] = 0;
    }

    for (int i = 0; i < getData()->n + getData()->m; i++)
    {
        m_workspace->xz_tilde[i] = 0;
    }
#endif

    return true;
}

bool OsqpEigen::Solver::initSolver()
{
    if (m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::initSolver] The solver has been already initialized. "
                      << "Please use clearSolver() method to deallocate memory." << std::endl;
        return false;
    }

    if (!m_data->isSet())
    {
        debugStream() << "[OsqpEigen::Solver::initSolver] Some data are not set." << std::endl;
        return false;
    }

    // if the number of constraints is equal to zero the user may not
    // call setLinearConstraintsMatrix()
    if (m_data->getData()->m == 0)
    {
        if (m_data->getData()->A == nullptr)
        {
            // let's create the matrix manually. This is required by osqp. Please check
            // https://github.com/oxfordcontrol/osqp/issues/295
            Eigen::SparseMatrix<c_float> A(m_data->getData()->m, m_data->getData()->n);
            if (!m_data->setLinearConstraintsMatrix(A))
            {
                debugStream() << "[OsqpEigen::Solver::initSolver] Unable to set the empty linear "
                                 "constraint "
                              << "matrix in case of unconstrained optimization problem"
                              << std::endl;
                return false;
            }
        }
    }

#ifdef OSQP_EIGEN_OSQP_IS_V1
    OSQPSolver* solver;
    auto data = m_data->getData();
    if (osqp_setup(&solver,
                   data->P,
                   data->q,
                   data->A,
                   data->l,
                   data->u,
                   data->m,
                   data->n,
                   m_settings->getSettings())
        != 0)
    {
        debugStream() << "[OsqpEigen::Solver::initSolver] Unable to setup the workspace."
                      << std::endl;
        return false;
    }

    m_solver.reset(solver);
#else
    OSQPWorkspace* workspace;
    if (osqp_setup(&workspace, m_data->getData(), m_settings->getSettings()) != 0)
    {
        debugStream() << "[OsqpEigen::Solver::initSolver] Unable to setup the workspace."
                      << std::endl;
        return false;
    }

    m_workspace.reset(workspace);
#endif

    m_isSolverInitialized = true;
    return true;
}

bool OsqpEigen::Solver::isInitialized()
{
    return m_isSolverInitialized;
}

void OsqpEigen::Solver::clearSolver()
{
    if (m_isSolverInitialized)
    {
#ifdef OSQP_EIGEN_OSQP_IS_V1
        m_solver.reset();
#else
        m_workspace.reset();
#endif
        m_isSolverInitialized = false;
    }
}

bool OsqpEigen::Solver::solve()
{
    if (this->solveProblem() != OsqpEigen::ErrorExitFlag::NoError)
    {
        debugStream() << "[OsqpEigen::Solver::solve] Unable to solve the problem." << std::endl;
        return false;
    }

    // check if the solution is feasible
    if (this->getStatus() != OsqpEigen::Status::Solved)
    {
        debugStream() << "[OsqpEigen::Solver::solve] The solution is unfeasible." << std::endl;
        return false;
    }

    return true;
}

OsqpEigen::Status OsqpEigen::Solver::getStatus() const
{
    return static_cast<OsqpEigen::Status>(getInfo()->status_val);
}

const c_float OsqpEigen::Solver::getObjValue() const
{
    return getInfo()->obj_val;
}

OsqpEigen::ErrorExitFlag OsqpEigen::Solver::solveProblem()
{
    if (!m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::solveProblem] The solve has not been initialized "
                         "yet. "
                      << "Please call initSolver() method." << std::endl;
        return OsqpEigen::ErrorExitFlag::WorkspaceNotInitError;
    }

#ifdef OSQP_EIGEN_OSQP_IS_V1
    return static_cast<OsqpEigen::ErrorExitFlag>(osqp_solve(m_solver.get()));
#else
    return static_cast<OsqpEigen::ErrorExitFlag>(osqp_solve(m_workspace.get()));
#endif
}

const Eigen::Matrix<c_float, -1, 1>& OsqpEigen::Solver::getSolution()
{
    // copy data from an array to Eigen vector
    c_float* solution = getOSQPSolution()->x;
    m_solution = Eigen::Map<Eigen::Matrix<c_float, -1, 1>>(solution, getData()->n, 1);

    return m_solution;
}

const Eigen::Matrix<c_float, -1, 1>& OsqpEigen::Solver::getDualSolution()
{
    // copy data from an array to Eigen vector
    c_float* solution = getOSQPSolution()->y;
    m_dualSolution = Eigen::Map<Eigen::Matrix<c_float, -1, 1>>(solution, getData()->m, 1);

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

#ifdef OSQP_EIGEN_OSQP_IS_V1
const std::unique_ptr<OSQPSolver, std::function<void(OSQPSolver*)>>&
OsqpEigen::Solver::solver() const
{
    return m_solver;
}
#else
const std::unique_ptr<OSQPWorkspace, std::function<void(OSQPWorkspace*)>>&
OsqpEigen::Solver::workspace() const
{
    return m_workspace;
}
#endif

bool OsqpEigen::Solver::updateGradient(
    const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& gradient)
{
    if (!m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::updateGradient] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the gradient is correct
    if (gradient.rows() != getData()->n)
    {
        debugStream() << "[OsqpEigen::Solver::updateGradient] The size of the gradient must be "
                         "equal to the number of the variables."
                      << std::endl;
        return false;
    }

    // update the gradient vector
#ifdef OSQP_EIGEN_OSQP_IS_V1
    if (osqp_update_data_vec(m_solver.get(), gradient.data(), nullptr, nullptr))
    {
#else
    if (osqp_update_lin_cost(m_workspace.get(), gradient.data()))
    {
#endif
        debugStream() << "[OsqpEigen::Solver::updateGradient] Error when the update gradient is "
                         "called."
                      << std::endl;
        return false;
    }
    return true;
}

bool OsqpEigen::Solver::updateLowerBound(
    const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& lowerBound)
{
    if (!m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::updateLowerBound] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the lowerBound vector is correct
    if (lowerBound.rows() != getData()->m)
    {
        debugStream() << "[OsqpEigen::Solver::updateLowerBound] The size of the lower bound must "
                         "be equal to the number of the variables."
                      << std::endl;
        return false;
    }

    // update the lower bound vector
#ifdef OSQP_EIGEN_OSQP_IS_V1
    if (osqp_update_data_vec(m_solver.get(), nullptr, lowerBound.data(), nullptr))
    {
#else
    if (osqp_update_lower_bound(m_workspace.get(), lowerBound.data()))
    {
#endif
        debugStream() << "[OsqpEigen::Solver::updateLowerBound] Error when the update lower bound "
                         "is called."
                      << std::endl;
        return false;
    }

    return true;
}

bool OsqpEigen::Solver::updateUpperBound(
    const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& upperBound)
{
    if (!m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::updateUpperBound] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the upperBound vector is correct
    if (upperBound.rows() != getData()->m)
    {
        debugStream() << "[OsqpEigen::Solver::updateUpperBound] The size of the upper bound must "
                         "be equal to the number of the variables."
                      << std::endl;
        return false;
    }

    // update the upper bound vector
#ifdef OSQP_EIGEN_OSQP_IS_V1
    if (osqp_update_data_vec(m_solver.get(), nullptr, nullptr, upperBound.data()))
    {
#else
    if (osqp_update_upper_bound(m_workspace.get(), upperBound.data()))
    {
#endif
        debugStream() << "[OsqpEigen::Solver::updateUpperBound] Error when the update upper bound "
                         "is called."
                      << std::endl;
        return false;
    }
    return true;
}

bool OsqpEigen::Solver::updateBounds(
    const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& lowerBound,
    const Eigen::Ref<const Eigen::Matrix<c_float, Eigen::Dynamic, 1>>& upperBound)
{
    if (!m_isSolverInitialized)
    {
        debugStream() << "[OsqpEigen::Solver::updateBounds] The solver is not initialized"
                      << std::endl;
        return false;
    }

    // check if the dimension of the upperBound vector is correct
    if (upperBound.rows() != getData()->m)
    {
        debugStream() << "[OsqpEigen::Solver::updateBounds] The size of the upper bound must be "
                         "equal to the number of the variables."
                      << std::endl;
        return false;
    }

    // check if the dimension of the lowerBound vector is correct
    if (lowerBound.rows() != getData()->m)
    {
        debugStream() << "[OsqpEigen::Solver::updateBounds] The size of the lower bound must be "
                         "equal to the number of the variables."
                      << std::endl;
        return false;
    }

    // update lower and upper constraints
#ifdef OSQP_EIGEN_OSQP_IS_V1
    if (osqp_update_data_vec(m_solver.get(), nullptr, lowerBound.data(), upperBound.data()))
    {
#else
    if (osqp_update_bounds(m_workspace.get(), lowerBound.data(), upperBound.data()))
    {
#endif
        debugStream() << "[OsqpEigen::Solver::updateBounds] Error when the update bounds is called."
                      << std::endl;
        return false;
    }
    return true;
}

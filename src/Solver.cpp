/**
 * @file Solver.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

// OsqpEigen
#include <OsqpEigen/Data.hpp>
#include <OsqpEigen/Settings.hpp>
#include <OsqpEigen/Solver.hpp>

OsqpEigen::Solver::Solver()
    : m_isSolverInitialized(false)
{
    m_settings = std::make_unique<OsqpEigen::Settings>();
    m_data = std::make_unique<OsqpEigen::Data>();
    m_workspace = nullptr;
}

OsqpEigen::Solver::~Solver()
{
    clearSolver();
}

bool OsqpEigen::Solver::clearSolverVariables()
{
    if(!m_isSolverInitialized){
        std::cerr << "[clearSolverVariables] Unable to clear the solver variables. "
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
        std::cerr << "[initSolver] The solver has been already initialized. "
                  << "Please use clearSolver() method to deallocate memory."
                  << std::endl;
        return false;
    }

    if(!m_data->isSet()){
        std::cout << "[Optimizer Workspace] Some data are not set."
                  << std::endl;
        return false;
    }

    m_workspace = osqp_setup(m_data->getData(),
                             m_settings->getSettings());

    if(m_workspace == OSQP_NULL){
        std::cout << "[Optimizer Workspace] Unable to setup the workspace."
                  << std::endl;
        return false;
    }


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
        osqp_cleanup(m_workspace);
        m_isSolverInitialized = false;
    }
}

bool OsqpEigen::Solver::solve()
{
    if(!m_isSolverInitialized){
        std::cout << "[solve] The solve has hot been initialized yet. "
                  << "Please call initSolver() method."
                  << std::endl;
        return false;
    }

    if(osqp_solve(m_workspace) != 0){
        std::cout << "[solve] Unable to solve the problem."
                  << std::endl;
        return false;
    }

    // check if the solution is feasible
    if(m_workspace->info->status_val != OSQP_SOLVED)
    {
        std::cout << "[solve] The solution is unfeasible."
                  << std::endl;
        return false;
    }

    return true;
}

const Eigen::VectorXd &OsqpEigen::Solver::getSolution()
{
    // copy data from an array to Eigen vector
    c_float* solution = m_workspace->solution->x;
    m_solution = Eigen::Map<Eigen::VectorXd>(solution, m_workspace->data->n, 1);

    return m_solution;
}

const std::unique_ptr<OsqpEigen::Settings>& OsqpEigen::Solver::settings() const
{
    return m_settings;
}

const std::unique_ptr<OsqpEigen::Data>& OsqpEigen::Solver::data() const
{
    return m_data;
}

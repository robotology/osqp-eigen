/**
 * @file Settings.cpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include <OsqpEigen/Settings.hpp>

OsqpEigen::Settings::Settings()
{
    m_settings = (OSQPSettings *)c_malloc(sizeof(OSQPSettings));
    osqp_set_default_settings(m_settings);
}

OsqpEigen::Settings::~Settings()
{
    c_free(m_settings);
}

void OsqpEigen::Settings::resetDefaultSettings()
{
    osqp_set_default_settings(m_settings);
}

void OsqpEigen::Settings::setRho(const double rho)
{
    m_settings->rho = rho;
}

void OsqpEigen::Settings::setSigma(const double sigma)
{
    m_settings->sigma = sigma;
}

void OsqpEigen::Settings::setScaling(const int scaling)
{
    m_settings->scaling = (c_int)scaling;
}

void OsqpEigen::Settings::setAdaptiveRho(const bool isRhoStepSizeAdactive)
{
    m_settings->adaptive_rho = (c_int)isRhoStepSizeAdactive;
}

void OsqpEigen::Settings::setAdaptiveRhoInterval(const int rhoInterval)
{
    m_settings->adaptive_rho_interval = (c_int)rhoInterval;
}

void OsqpEigen::Settings::setAdaptiveRhoTolerance(const double adaptiveRhoTolerance)
{
    m_settings->adaptive_rho_tolerance = (c_float)adaptiveRhoTolerance;
}

void OsqpEigen::Settings::setAdaptiveRhoFraction(const double adaptiveRhoFraction)
{
    m_settings->adaptive_rho_fraction = (c_float)adaptiveRhoFraction;
}
void OsqpEigen::Settings::setMaxIteraction(const int maxIteration)
{
    m_settings->max_iter = (c_int)maxIteration;
}

void OsqpEigen::Settings::setAbsoluteTolerance(const double absoluteTolerance)
{
    m_settings->eps_abs = (c_float)absoluteTolerance;
}

void OsqpEigen::Settings::setRelativeTolerance(const double relativeTolerance)
{
    m_settings->eps_rel = (c_float)relativeTolerance;
}

void OsqpEigen::Settings::setPrimalInfeasibilityTollerance(const double primalInfeasibilityTollerance)
{
    m_settings->eps_prim_inf = (c_float)primalInfeasibilityTollerance;
}

void OsqpEigen::Settings::setDualInfeasibilityTollerance(const double dualInfeasibilityTollerance)
{
    m_settings->eps_dual_inf = (c_float)dualInfeasibilityTollerance;
}

void OsqpEigen::Settings::setAlpha(const double alpha)
{
    m_settings->alpha = (c_float)alpha;
}

void OsqpEigen::Settings::setLinearSystemSolver(const int linsysSolver)
{
    m_settings->linsys_solver = (linsys_solver_type)linsysSolver;
}

void OsqpEigen::Settings::setDelta(const double delta)
{
    m_settings->delta = (c_float)delta;
}

void OsqpEigen::Settings::setPolish(const bool polish)
{
    m_settings->polish = (c_int)polish;
}

void OsqpEigen::Settings::setPolishRefineIter(const int polishRefineIter)
{
    m_settings->polish_refine_iter = (c_int)polishRefineIter;
}

void OsqpEigen::Settings::setVerbosity(const bool isVerbose)
{
    m_settings->verbose = (c_int)isVerbose;
}

void OsqpEigen::Settings::setScaledTerimination(const bool scaledTermination)
{
    m_settings->scaled_termination = (c_int)scaledTermination;
}

void OsqpEigen::Settings::setCheckTermination(const int checkTermination)
{
    m_settings->check_termination = (c_int)checkTermination;
}

void OsqpEigen::Settings::setWarmStart(const bool warmStart)
{
    m_settings->warm_start = (c_int)warmStart;
}

OSQPSettings* const & OsqpEigen::Settings::getSettings() const
{
    return m_settings;
}

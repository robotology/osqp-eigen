/**
 * @file OptimizerSettings.cpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include "OptimizerSettings.hpp"

OSQPWrapper::OptimizerSettings::OptimizerSettings()
{
    m_settings = (OSQPSettings *)c_malloc(sizeof(OSQPSettings));
    osqp_set_default_settings(m_settings);
}

OSQPWrapper::OptimizerSettings::~OptimizerSettings()
{
    c_free(m_settings);
}

void OSQPWrapper::OptimizerSettings::resetDefaultSettings()
{
    osqp_set_default_settings(m_settings);
}

void OSQPWrapper::OptimizerSettings::setRho(const double rho)
{
    m_settings->rho = rho;
}

void OSQPWrapper::OptimizerSettings::setSigma(const double sigma)
{
    m_settings->sigma = sigma;
}

void OSQPWrapper::OptimizerSettings::setScaling(const int scaling)
{
    m_settings->scaling = (c_int)scaling;
}

void OSQPWrapper::OptimizerSettings::setAdaptiveRho(const bool isRhoStepSizeAdactive)
{
    m_settings->adaptive_rho = (c_int)isRhoStepSizeAdactive;
}

void OSQPWrapper::OptimizerSettings::setAdaptiveRhoInterval(const int rhoInterval)
{
    m_settings->adaptive_rho_interval = (c_int)rhoInterval;
}

void OSQPWrapper::OptimizerSettings::setAdaptiveRhoTolerance(const double adaptiveRhoTolerance)
{
    m_settings->adaptive_rho_tolerance = (c_float)adaptiveRhoTolerance;
}

void OSQPWrapper::OptimizerSettings::setAdaptiveRhoFraction(const double adaptiveRhoFraction)
{
    m_settings->adaptive_rho_fraction = (c_float)adaptiveRhoFraction;
}
void OSQPWrapper::OptimizerSettings::setMaxIteraction(const int maxIteration)
{
    m_settings->max_iter = (c_int)maxIteration;
}

void OSQPWrapper::OptimizerSettings::setAbsoluteTolerance(const double absoluteTolerance)
{
    m_settings->eps_abs = (c_float)absoluteTolerance;
}

void OSQPWrapper::OptimizerSettings::setRelativeTolerance(const double relativeTolerance)
{
    m_settings->eps_rel = (c_float)relativeTolerance;
}

void OSQPWrapper::OptimizerSettings::setPrimalInfeasibilityTollerance(const double primalInfeasibilityTollerance)
{
    m_settings->eps_prim_inf = (c_float)primalInfeasibilityTollerance;
}

void OSQPWrapper::OptimizerSettings::setDualInfeasibilityTollerance(const double dualInfeasibilityTollerance)
{
    m_settings->eps_dual_inf = (c_float)dualInfeasibilityTollerance;
}

void OSQPWrapper::OptimizerSettings::setAlpha(const double alpha)
{
    m_settings->alpha = (c_float)alpha;
}

void OSQPWrapper::OptimizerSettings::setLinearSystemSolver(const int linsysSolver)
{
    m_settings->linsys_solver = (linsys_solver_type)linsysSolver;
}

void OSQPWrapper::OptimizerSettings::setDelta(const double delta)
{
    m_settings->delta = (c_float)delta;
}

void OSQPWrapper::OptimizerSettings::setPolish(const bool polish)
{
    m_settings->polish = (c_int)polish;
}

void OSQPWrapper::OptimizerSettings::setPolishRefineIter(const int polishRefineIter)
{
    m_settings->polish_refine_iter = (c_int)polishRefineIter;
}

void OSQPWrapper::OptimizerSettings::setVerbosity(const bool isVerbose)
{
    m_settings->verbose = (c_int)isVerbose;
}

void OSQPWrapper::OptimizerSettings::setScaledTerimination(const bool scaledTermination)
{
    m_settings->scaled_termination = (c_int)scaledTermination;
}

void OSQPWrapper::OptimizerSettings::setCheckTermination(const int checkTermination)
{
    m_settings->check_termination = (c_int)checkTermination;
}

void OSQPWrapper::OptimizerSettings::setWarmStart(const bool warmStart)
{
    m_settings->warm_start = (c_int)warmStart;
}

OSQPSettings* const & OSQPWrapper::OptimizerSettings::getOptimizerSettings() const
{
    return m_settings;
}

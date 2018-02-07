/**
 * @file OptimizatorSettings.cpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#include "OptimizatorSettings.hpp"

OSQPWrapper::OptimizatorSettings::OptimizatorSettings()
{
    m_settings = (OSQPSettings *)c_malloc(sizeof(OSQPSettings));
    set_default_settings(m_settings);
}

OSQPWrapper::OptimizatorSettings::~OptimizatorSettings()
{
    c_free(m_settings);
}

void OSQPWrapper::OptimizatorSettings::resetDefaultSettings()
{
    set_default_settings(m_settings);
}

void OSQPWrapper::OptimizatorSettings::setRho(const double rho)
{
    m_settings->rho = rho;
}

void OSQPWrapper::OptimizatorSettings::setSigma(const double sigma)
{
    m_settings->sigma = sigma;
}

void OSQPWrapper::OptimizatorSettings::setScaling(const int scaling)
{
    m_settings->scaling = (c_int)scaling;
}

void OSQPWrapper::OptimizatorSettings::setAdaptiveRho(const bool isRhoStepSizeAdactive)
{
    m_settings->adaptive_rho = (c_int)isRhoStepSizeAdactive;
}

void OSQPWrapper::OptimizatorSettings::setAdaptiveRhoInterval(const int rhoInterval)
{
    m_settings->adaptive_rho_interval = (c_int)rhoInterval;
}

void OSQPWrapper::OptimizatorSettings::setAdaptiveRhoTolerance(const double adaptiveRhoTolerance)
{
    m_settings->adaptive_rho_tolerance = (c_float)adaptiveRhoTolerance;
}

void OSQPWrapper::OptimizatorSettings::setAdaptiveRhoFraction(const double adaptiveRhoFraction)
{
    m_settings->adaptive_rho_fraction = (c_float)adaptiveRhoFraction;
}
void OSQPWrapper::OptimizatorSettings::setMaxIteraction(const int maxIteration)
{
    m_settings->max_iter = (c_int)maxIteration;
}

void OSQPWrapper::OptimizatorSettings::setAbsoluteTolerance(const double absoluteTolerance)
{
    m_settings->eps_abs = (c_float)absoluteTolerance;
}

void OSQPWrapper::OptimizatorSettings::setRelativeTolerance(const double relativeTolerance)
{
    m_settings->eps_rel = (c_float)relativeTolerance;
}

void OSQPWrapper::OptimizatorSettings::setPrimalInfeasibilityTollerance(const double primalInfeasibilityTollerance)
{
    m_settings->eps_prim_inf = (c_float)primalInfeasibilityTollerance;
}

void OSQPWrapper::OptimizatorSettings::setDualInfeasibilityTollerance(const double dualInfeasibilityTollerance)
{
    m_settings->eps_dual_inf = (c_float)dualInfeasibilityTollerance;
}

void OSQPWrapper::OptimizatorSettings::setAlpha(const double alpha)
{
    m_settings->alpha = (c_float)alpha;
}

//void OSQPWrapper::OptimizatorSettings::setLinearSystemSolver(const linsys_solver_type linsys_solver)

void OSQPWrapper::OptimizatorSettings::setDelta(const double delta)
{
    m_settings->delta = (c_float)delta;
}

void OSQPWrapper::OptimizatorSettings::setPolish(const bool polish)
{
    m_settings->polish = (c_int)polish;
}

void OSQPWrapper::OptimizatorSettings::setPolishRefineIter(const int polishRefineIter)
{
    m_settings->polish_refine_iter = (c_int)polishRefineIter;
}

void OSQPWrapper::OptimizatorSettings::setVerbosity(const bool isVerbose)
{
    m_settings->verbose = (c_int)isVerbose;
}

void OSQPWrapper::OptimizatorSettings::setScaledTerimination(const bool scaledTermination)
{
    m_settings->scaled_termination = (c_int)scaledTermination;
}

void OSQPWrapper::OptimizatorSettings::setCheckTermination(const int checkTermination)
{
    m_settings->check_termination = (c_int)checkTermination;
}

void OSQPWrapper::OptimizatorSettings::setWarmStart(const bool warmStart)
{
    m_settings->warm_start = (c_int)warmStart;
}

OSQPSettings* const & OSQPWrapper::OptimizatorSettings::getOptimizatorSettings() const
{
    return m_settings;
}

/**
 * @file Settings.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2018
 */

#include <OsqpEigen/Compat.hpp>
#include <OsqpEigen/Debug.hpp>
#include <OsqpEigen/Settings.hpp>
#include <iostream>

template <typename... Args>
inline void unused(Args &&...) {}

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
#if EMBEDDED != 1
    m_settings->adaptive_rho = (c_int)isRhoStepSizeAdactive;
#else
    debugStream() << "[OsqpEigen::Settings::setAdaptiveRho] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(isRhoStepSizeAdactive);
#endif
}

void OsqpEigen::Settings::setAdaptiveRhoInterval(const int rhoInterval)
{
#if EMBEDDED != 1
    m_settings->adaptive_rho_interval = (c_int)rhoInterval;
#else
    debugStream() << "[OsqpEigen::Settings::setAdaptiveRhoInterval] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(rhoInterval);
#endif
}

void OsqpEigen::Settings::setAdaptiveRhoTolerance(const double adaptiveRhoTolerance)
{
#if EMBEDDED != 1
    m_settings->adaptive_rho_tolerance = (c_float)adaptiveRhoTolerance;
#else
    debugStream() << "[OsqpEigen::Settings::setAdaptiveRhoTolerance] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(adaptiveRhoTolerance);
#endif
}

void OsqpEigen::Settings::setAdaptiveRhoFraction(const double adaptiveRhoFraction)
{
#if EMBEDDED != 1
#ifdef PROFILING
    m_settings->adaptive_rho_fraction = (c_float)adaptiveRhoFraction;
#else
    debugStream() << "[OsqpEigen::Settings::setAdaptiveRhoFraction] OSPQ has been set without PROFILING, hence this setting is disabled." << std::endl;
    unused(adaptiveRhoFraction);
#endif // ifdef PROFILING
#else  // # if EMBEDDED != 1
    debugStream() << "[OsqpEigen::Settings::setAdaptiveRhoFraction] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(adaptiveRhoFraction);
#endif // # if EMBEDDED != 1
}

void OsqpEigen::Settings::setMaxIteraction(const int maxIteration)
{
    setMaxIteration(maxIteration);
}

void OsqpEigen::Settings::setMaxIteration(const int maxIteration)
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

void OsqpEigen::Settings::setPrimalInfeasibilityTollerance(const double primalInfeasibilityTolerance)
{
    this->setPrimalInfeasibilityTolerance(primalInfeasibilityTolerance);
}

void OsqpEigen::Settings::setPrimalInfeasibilityTolerance(const double primalInfeasibilityTolerance)
{
    m_settings->eps_prim_inf = (c_float)primalInfeasibilityTolerance;
}

void OsqpEigen::Settings::setDualInfeasibilityTollerance(const double dualInfeasibilityTolerance)
{
    this->setDualInfeasibilityTolerance(dualInfeasibilityTolerance);
}

void OsqpEigen::Settings::setDualInfeasibilityTolerance(const double dualInfeasibilityTolerance)
{
    m_settings->eps_dual_inf = (c_float)dualInfeasibilityTolerance;
}

void OsqpEigen::Settings::setAlpha(const double alpha)
{
    m_settings->alpha = (c_float)alpha;
}

void OsqpEigen::Settings::setLinearSystemSolver(const int linsysSolver)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1
    m_settings->linsys_solver = (osqp_linsys_solver_type)linsysSolver;
#else
    m_settings->linsys_solver = (linsys_solver_type)linsysSolver;
#endif
}

void OsqpEigen::Settings::setDelta(const double delta)
{
#ifndef EMBEDDED
    m_settings->delta = (c_float)delta;
#else
    debugStream() << "[OsqpEigen::Settings::setDelta] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(delta);
#endif
}

void OsqpEigen::Settings::setPolish(const bool polish)
{
#ifndef EMBEDDED
#ifdef OSQP_EIGEN_OSQP_IS_V1
    m_settings->polishing = (c_int)polish;
#else
    m_settings->polish = (c_int)polish;
#endif
#else
    debugStream() << "[OsqpEigen::Settings::setPolish] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(polish);
#endif
}

void OsqpEigen::Settings::setPolishRefineIter(const int polishRefineIter)
{
#ifndef EMBEDDED
    m_settings->polish_refine_iter = (c_int)polishRefineIter;
#else
    debugStream() << "[OsqpEigen::Settings::setPolishRefineIter] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(polishRefineIter);
#endif
}

void OsqpEigen::Settings::setVerbosity(const bool isVerbose)
{
#ifndef EMBEDDED
    m_settings->verbose = (c_int)isVerbose;
#else
    debugStream() << "[OsqpEigen::Settings::setVerbosity] OSPQ has been set to EMBEDDED, hence this setting is disabled." << std::endl;
    unused(isVerbose);
#endif
}

void OsqpEigen::Settings::setScaledTerimination(const bool scaledTermination)
{
    m_settings->scaled_termination = (c_int)scaledTermination;
}

void OsqpEigen::Settings::setCheckTermination(const int checkTermination)
{
    m_settings->check_termination = (c_int)checkTermination;
}

void OsqpEigen::Settings::setCheckDualGap(const bool checkDualGap)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->check_dualgap = (c_int)checkDualGap;
#else
    debugStream() << "[OsqpEigen::Settings::setCheckDualGap] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(checkDualGap);
#endif
}

void OsqpEigen::Settings::setWarmStart(const bool warmStart)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1
    m_settings->warm_starting = (c_int)warmStart;
#else
    m_settings->warm_start = (c_int)warmStart;
#endif
}

void OsqpEigen::Settings::setTimeLimit(const double timeLimit)
{
#ifdef PROFILING
    m_settings->time_limit = (c_float)timeLimit;
#else
    debugStream() << "[OsqpEigen::Settings::setTimeLimit] OSPQ has been set without PROFILING, hence this setting is disabled." << std::endl;
    unused(timeLimit);
#endif
}

void OsqpEigen::Settings::setAllocateSolution(const bool allocateSolution)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->allocate_solution = static_cast<decltype(m_settings->allocate_solution)>(allocateSolution);
#else
    debugStream() << "[OsqpEigen::Settings::setAllocateSolution] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(allocateSolution);
#endif
}

void OsqpEigen::Settings::setCgMaxIter(const int cgMaxIter)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->cg_max_iter = static_cast<decltype(m_settings->cg_max_iter)>(cgMaxIter);
#else
    debugStream() << "[OsqpEigen::Settings::setCgMaxIter] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(cgMaxIter);
#endif
}

void OsqpEigen::Settings::setCgPrecond(const int cgPrecond)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->cg_precond = static_cast<decltype(m_settings->cg_precond)>(cgPrecond);
#else
    debugStream() << "[OsqpEigen::Settings::setCgPrecond] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(cgPrecond);
#endif
}

void OsqpEigen::Settings::setCgTolFraction(const double cgTolFraction)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->cg_tol_fraction = static_cast<decltype(m_settings->cg_tol_fraction)>(cgTolFraction);
#else
    debugStream() << "[OsqpEigen::Settings::setCgTolFraction] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(cgTolFraction);
#endif
}

void OsqpEigen::Settings::setCgTolReduction(const double cgTolReduction)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->cg_tol_reduction = static_cast<decltype(m_settings->cg_tol_reduction)>(cgTolReduction);
#else
    debugStream() << "[OsqpEigen::Settings::setCgTolReduction] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(cgTolReduction);
#endif
}

void OsqpEigen::Settings::setDevice(const int device)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->device = static_cast<decltype(m_settings->device)>(device);
#else
    debugStream() << "[OsqpEigen::Settings::setDevice] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(device);
#endif
}

void OsqpEigen::Settings::setProfilerLevel(const int level)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->profiler_level = static_cast<decltype(m_settings->profiler_level)>(level);
#else
    debugStream() << "[OsqpEigen::Settings::setProfilerLevel] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(level);
#endif
}

void OsqpEigen::Settings::setRhoIsVec(const bool rhoIsVec)
{
#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
    m_settings->rho_is_vec = static_cast<decltype(m_settings->rho_is_vec)>(rhoIsVec);
#else
    debugStream() << "[OsqpEigen::Settings::setRhoIsVec] OSQP version is lower than v1.0.0, this setting is not available." << std::endl;
    unused(rhoIsVec);
#endif
}

OSQPSettings *const &OsqpEigen::Settings::getSettings() const
{
    return m_settings;
}

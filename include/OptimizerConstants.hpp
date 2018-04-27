/**
 * @file OptimizerConstants.hpp
 * @author Giulio Romualdi
 * @copyright  Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 * @date 2018
 */

#ifndef OPTIMIZER_CONSTANTS_HPP
#define OPTIMIZER_CONSTANTS_HPP

// osqp
#include "osqp.h"

/**
 * OSQPWrapper namespace.
 */
namespace OSQPWrapper
{
    const c_float INFTY = OSQP_INFTY; /**< Infinity constant. */
}

#endif

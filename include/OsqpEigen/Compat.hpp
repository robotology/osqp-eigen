/**
 * @file SparseMatrixHelper.hpp
 * @author Pierre Gergondet
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2023
 */

#ifndef OSQP_EIGEN_COMPAT_HPP
#define OSQP_EIGEN_COMPAT_HPP

// OSQP
#include <osqp.h>

#ifdef OSQP_EIGEN_OSQP_IS_V1

// Re-use the same name in the global namespace as the old versions of OSQP
using c_int = OSQPInt;
using c_float = OSQPFloat;
using csc = OSQPCscMatrix;

// Those symbols are available in the library but hidden from the public API
extern "C" {
extern OSQPCscMatrix* csc_spalloc(c_int m, c_int n, c_int nzmax, c_int values, c_int triplet);
extern void csc_spfree(OSQPCscMatrix* A);
}

#ifndef OSQP_CUSTOM_MEMORY
#define c_malloc malloc
#define c_free free
#endif

namespace OsqpEigen
{

struct OSQPData
{
    OSQPInt n; ///< number of variables n
    OSQPInt m; ///< number of constraints m
    OSQPCscMatrix* P; ///< the upper triangular part of the quadratic objective matrix P (size n x n).
    OSQPCscMatrix* A; ///< linear constraints matrix A (size m x n)
    OSQPFloat* q; ///< dense array for linear part of objective function (size n)
    OSQPFloat* l; ///< dense array for lower bound (size m)
    OSQPFloat* u; ///< dense array for upper bound (size m)
};

} // namespace OsqpEigen

#endif // OSQP_EIGEN_OSQP_IS_V1
#endif // OSQP_EIGEN_COMPAT_HPP

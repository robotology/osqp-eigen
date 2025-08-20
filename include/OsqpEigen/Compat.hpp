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

#include <cstdlib>

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

inline OSQPCscMatrix* spalloc(OSQPInt m,
                              OSQPInt n,
                              OSQPInt nzmax)
{
    // NOLINTBEGIN(clang-analyzer-unix.Malloc): no leak, ownership is with the callee via M
    OSQPCscMatrix* M = static_cast<OSQPCscMatrix*>(calloc(1, sizeof(OSQPCscMatrix))); /* allocate the OSQPCscMatrix struct */
    if (!M)
    {
        return static_cast<OSQPCscMatrix*>(OSQP_NULL);
    }

    OSQPInt* M_p = static_cast<OSQPInt*>(calloc(n + 1, sizeof(OSQPInt)));
    if (!M_p)
    {
        free(M);
        return static_cast<OSQPCscMatrix*>(OSQP_NULL);
    }

    OSQPInt* M_i = static_cast<OSQPInt*>(calloc(nzmax,  sizeof(OSQPInt)));
    if (!M_i)
    {
        free(M);
        free(M_p);
        return static_cast<OSQPCscMatrix*>(OSQP_NULL);
    }

    OSQPFloat* M_x = static_cast<OSQPFloat*>(calloc(nzmax,  sizeof(OSQPFloat)));
    if (!M_x)
    {
        free(M);
        free(M_p);
        free(M_i);
        return static_cast<OSQPCscMatrix*>(OSQP_NULL);
    }

    OSQPInt M_nnz = 0;

    if (nzmax >= 0)
    {
        M_nnz = nzmax;
    }

#ifdef OSQP_EIGEN_OSQP_IS_V1_FINAL
#define OSQPEigen_OSQPCscMatrix_set_data OSQPCscMatrix_set_data
#else
#define OSQPEigen_OSQPCscMatrix_set_data csc_set_data
#endif

    OSQPEigen_OSQPCscMatrix_set_data(M, m, n, M_nnz, M_x, M_i, M_p);

    return M;
    // NOLINTEND(clang-analyzer-unix.Malloc)
}

inline void spfree(OSQPCscMatrix* M)
{
    if (M){
        if (M->p) free(M->p);
        if (M->i) free(M->i);
        if (M->x) free(M->x);
        free(M);
    }
}

} // namespace OsqpEigen

#else

namespace OsqpEigen
{

inline csc* spalloc(c_int m, c_int n, c_int nzmax)
{
    return csc_spalloc(m, n, nzmax, 1, 0);
}

inline void spfree(csc* M)
{
    return csc_spfree(M);
}

} // namespace OsqpEigen


#endif // OSQP_EIGEN_OSQP_IS_V1
#endif // OSQP_EIGEN_COMPAT_HPP

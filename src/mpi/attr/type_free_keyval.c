/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Type_free_keyval */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Type_free_keyval = PMPI_Type_free_keyval
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Type_free_keyval  MPI_Type_free_keyval
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Type_free_keyval as PMPI_Type_free_keyval
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Type_free_keyval(int *type_keyval) __attribute__ ((weak, alias("PMPI_Type_free_keyval")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Type_free_keyval
#define MPI_Type_free_keyval PMPI_Type_free_keyval

#endif

int MPI_Type_free_keyval(int *type_keyval)
{
    QMPI_Context context;
    QMPI_Type_free_keyval_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Type_free_keyval(context, 0, type_keyval);

    fn_ptr = (QMPI_Type_free_keyval_t *) MPIR_QMPI_first_fn_ptrs[MPI_TYPE_FREE_KEYVAL_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_TYPE_FREE_KEYVAL_T], type_keyval);
}

/*@
   MPI_Type_free_keyval - Frees an attribute key for datatypes

Input Parameters:
. type_keyval - Frees the integer key value (integer)

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER
.N MPI_ERR_KEYVAL
@*/
int QMPI_Type_free_keyval(QMPI_Context context, int tool_id, int *type_keyval)
{
    MPII_Keyval *keyval_ptr = NULL;
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_TYPE_FREE_KEYVAL);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_TYPE_FREE_KEYVAL);

    /* Validate parameters, especially handles needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(type_keyval, "type_keyval", mpi_errno);
            MPIR_ERRTEST_KEYVAL(*type_keyval, MPIR_DATATYPE, "datatype", mpi_errno);
            MPIR_ERRTEST_KEYVAL_PERM(*type_keyval, mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif

    /* Convert MPI object handles to object pointers */
    MPII_Keyval_get_ptr(*type_keyval, keyval_ptr);

    /* Validate parameters and objects (post conversion) */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPII_Keyval_valid_ptr(keyval_ptr, mpi_errno);
            if (mpi_errno)
                goto fn_fail;
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    MPIR_free_keyval(keyval_ptr);
    *type_keyval = MPI_KEYVAL_INVALID;

    /* ... end of body of routine ... */

#ifdef HAVE_ERROR_CHECKING
  fn_exit:
#endif
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_TYPE_FREE_KEYVAL);
    MPID_THREAD_CS_EXIT(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
  fn_fail:
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_type_free_keyval", "**mpi_type_free_keyval %p",
                                 type_keyval);
    }
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
#endif
    /* --END ERROR HANDLING-- */
}

/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Type_create_keyval */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Type_create_keyval = PMPI_Type_create_keyval
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Type_create_keyval  MPI_Type_create_keyval
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Type_create_keyval as PMPI_Type_create_keyval
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Type_create_keyval(MPI_Type_copy_attr_function * type_copy_attr_fn,
                           MPI_Type_delete_attr_function * type_delete_attr_fn,
                           int *type_keyval, void *extra_state)
    __attribute__ ((weak, alias("PMPI_Type_create_keyval")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Type_create_keyval
#define MPI_Type_create_keyval PMPI_Type_create_keyval

#endif

int MPI_Type_create_keyval(MPI_Type_copy_attr_function * type_copy_attr_fn,
                           MPI_Type_delete_attr_function * type_delete_attr_fn,
                           int *type_keyval, void *extra_state)
{
    QMPI_Context context;
    QMPI_Type_create_keyval_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Type_create_keyval(context, 0, type_copy_attr_fn, type_delete_attr_fn,
                                       type_keyval, extra_state);

    fn_ptr = (QMPI_Type_create_keyval_t *) MPIR_QMPI_first_fn_ptrs[MPI_TYPE_CREATE_KEYVAL_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_TYPE_CREATE_KEYVAL_T],
                      type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state);
}

/*@
   MPI_Type_create_keyval - Create an attribute keyval for MPI datatypes

Input Parameters:
+ type_copy_attr_fn - copy callback function for type_keyval (function)
. type_delete_attr_fn - delete callback function for type_keyval (function)
- extra_state - extra state for callback functions

Output Parameters:
. type_keyval - key value for future access (integer)

Notes:

   Default copy and delete functions are available.  These are
+ MPI_TYPE_NULL_COPY_FN   - empty copy function
. MPI_TYPE_NULL_DELETE_FN - empty delete function
- MPI_TYPE_DUP_FN         - simple dup function

.N AttrErrReturn

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER
@*/
int QMPI_Type_create_keyval(QMPI_Context context, int tool_id,
                            MPI_Type_copy_attr_function * type_copy_attr_fn,
                            MPI_Type_delete_attr_function * type_delete_attr_fn, int *type_keyval,
                            void *extra_state)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_TYPE_CREATE_KEYVAL);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_TYPE_CREATE_KEYVAL);

    /* Validate parameters and objects (post conversion) */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(type_keyval, "type_keyval", mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    mpi_errno = MPIR_Type_create_keyval_impl(type_copy_attr_fn, type_delete_attr_fn, type_keyval,
                                             extra_state);
    if (mpi_errno)
        goto fn_fail;
    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_TYPE_CREATE_KEYVAL);
    MPID_THREAD_CS_EXIT(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_type_create_keyval", "**mpi_type_create_keyval %p %p %p %p",
                                 type_copy_attr_fn, type_delete_attr_fn, type_keyval, extra_state);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}

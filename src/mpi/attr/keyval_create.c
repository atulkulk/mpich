/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Keyval_create */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Keyval_create = PMPI_Keyval_create
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Keyval_create  MPI_Keyval_create
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Keyval_create as PMPI_Keyval_create
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Keyval_create(MPI_Copy_function * copy_fn, MPI_Delete_function * delete_fn,
                      int *keyval, void *extra_state)
    __attribute__ ((weak, alias("PMPI_Keyval_create")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Keyval_create
#define MPI_Keyval_create PMPI_Keyval_create

#endif

int MPI_Keyval_create(MPI_Copy_function * copy_fn,
                      MPI_Delete_function * delete_fn, int *keyval, void *extra_state)
{
    QMPI_Context context;
    QMPI_Keyval_create_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Keyval_create(context, 0, copy_fn, delete_fn, keyval, extra_state);

    fn_ptr = (QMPI_Keyval_create_t *) MPIR_QMPI_first_fn_ptrs[MPI_KEYVAL_CREATE_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_KEYVAL_CREATE_T], copy_fn, delete_fn,
                      keyval, extra_state);
}

/*@

MPI_Keyval_create - Greates a new attribute key

Input Parameters:
+ copy_fn - Copy callback function for 'keyval'
. delete_fn - Delete callback function for 'keyval'
- extra_state - Extra state for callback functions

Output Parameters:
. keyval - key value for future access (integer)

Notes:
Key values are global (available for any and all communicators).

There are subtle differences between C and Fortran that require that the
copy_fn be written in the same language that 'MPI_Keyval_create'
is called from.
This should not be a problem for most users; only programmers using both
Fortran and C in the same program need to be sure that they follow this rule.

.N ThreadSafe

.N Deprecated
The replacement for this routine is 'MPI_Comm_create_keyval'.

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_EXHAUSTED
.N MPI_ERR_ARG

.seealso  MPI_Keyval_free, MPI_Comm_create_keyval
@*/
int QMPI_Keyval_create(QMPI_Context context, int tool_id, MPI_Copy_function * copy_fn,
                       MPI_Delete_function * delete_fn, int *keyval, void *extra_state)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_KEYVAL_CREATE);
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_KEYVAL_CREATE);

    mpi_errno = PMPI_Comm_create_keyval(copy_fn, delete_fn, keyval, extra_state);

    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_KEYVAL_CREATE);
    return mpi_errno;
}

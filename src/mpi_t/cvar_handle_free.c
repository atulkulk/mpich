/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_T_cvar_handle_free */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_T_cvar_handle_free = PMPI_T_cvar_handle_free
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_T_cvar_handle_free  MPI_T_cvar_handle_free
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_T_cvar_handle_free as PMPI_T_cvar_handle_free
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_T_cvar_handle_free(MPI_T_cvar_handle * handle)
    __attribute__ ((weak, alias("PMPI_T_cvar_handle_free")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_T_cvar_handle_free
#define MPI_T_cvar_handle_free PMPI_T_cvar_handle_free
#endif /* MPICH_MPI_FROM_PMPI */

int MPI_T_cvar_handle_free(MPI_T_cvar_handle * handle)
{
    QMPI_Context context;
    QMPI_T_cvar_handle_free_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_T_cvar_handle_free(context, 0, handle);

    fn_ptr = (QMPI_T_cvar_handle_free_t *) MPIR_QMPI_first_fn_ptrs[MPI_T_CVAR_HANDLE_FREE_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_T_CVAR_HANDLE_FREE_T], handle);
}

/*@
MPI_T_cvar_handle_free - Free an existing handle for a control variable

Input/Output Parameters:
. handle - handle to be freed (handle)

.N ThreadSafe

.N Errors
.N MPI_SUCCESS
.N MPI_T_ERR_NOT_INITIALIZED
.N MPI_T_ERR_INVALID_HANDLE
@*/
int QMPI_T_cvar_handle_free(QMPI_Context context, int tool_id, MPI_T_cvar_handle * handle)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_T_cvar_handle_t *hnd;

    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_T_CVAR_HANDLE_FREE);
    MPIR_ERRTEST_MPIT_INITIALIZED(mpi_errno);
    MPIR_T_THREAD_CS_ENTER();
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_T_CVAR_HANDLE_FREE);

    /* Validate parameters */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(handle, "handle", mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    hnd = *handle;
    MPL_free(hnd);
    *handle = MPI_T_CVAR_HANDLE_NULL;

    /* ... end of body of routine ... */

#ifdef HAVE_ERROR_CHECKING
  fn_exit:
#endif
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_T_CVAR_HANDLE_FREE);
    MPIR_T_THREAD_CS_EXIT();
    return mpi_errno;

#ifdef HAVE_ERROR_CHECKING
  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_t_cvar_handle_free", "**mpi_t_cvar_handle_free %p", handle);
    }
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
#endif
}

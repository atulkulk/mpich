/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Test_cancelled */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Test_cancelled = PMPI_Test_cancelled
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Test_cancelled  MPI_Test_cancelled
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Test_cancelled as PMPI_Test_cancelled
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Test_cancelled(const MPI_Status * status, int *flag)
    __attribute__ ((weak, alias("PMPI_Test_cancelled")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Test_cancelled
#define MPI_Test_cancelled PMPI_Test_cancelled

#endif

int MPI_Test_cancelled(const MPI_Status * status, int *flag)
{
    QMPI_Context context;
    QMPI_Test_cancelled_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Test_cancelled(context, 0, status, flag);

    fn_ptr = (QMPI_Test_cancelled_t *) MPIR_QMPI_first_fn_ptrs[MPI_TEST_CANCELLED_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_TEST_CANCELLED_T], status, flag);
}

/*@
  MPI_Test_cancelled - Tests to see if a request was cancelled

Input Parameters:
. status - status object (Status)

Output Parameters:
. flag - true if the request was cancelled, false otherwise (logical)

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_ARG
@*/
int QMPI_Test_cancelled(QMPI_Context context, int tool_id, const MPI_Status * status, int *flag)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_TEST_CANCELLED);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPIR_FUNC_TERSE_REQUEST_ENTER(MPID_STATE_MPI_TEST_CANCELLED);

    /* Validate parameters if error checking is enabled */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(status, "status", mpi_errno);
            MPIR_ERRTEST_ARGNULL(flag, "flag", mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */
    *flag = MPIR_STATUS_GET_CANCEL_BIT(*status);

    /* ... end of body of routine ... */

#ifdef HAVE_ERROR_CHECKING
  fn_exit:
#endif
    MPIR_FUNC_TERSE_REQUEST_EXIT(MPID_STATE_MPI_TEST_CANCELLED);
    return mpi_errno;

    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
  fn_fail:
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_test_cancelled", "**mpi_test_cancelled %p %p", status,
                                 flag);
    }
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
#endif
    /* --END ERROR HANDLING-- */
}

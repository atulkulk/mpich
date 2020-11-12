/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Close_port */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Close_port = PMPI_Close_port
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Close_port  MPI_Close_port
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Close_port as PMPI_Close_port
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Close_port(const char *port_name) __attribute__ ((weak, alias("PMPI_Close_port")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Close_port
#define MPI_Close_port PMPI_Close_port

int MPIR_Close_port_impl(const char *port_name)
{
    return MPID_Close_port(port_name);
}


#endif

int MPI_Close_port(const char *port_name)
{
    QMPI_Context context;
    QMPI_Close_port_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Close_port(context, 0, port_name);

    fn_ptr = (QMPI_Close_port_t *) MPIR_QMPI_first_fn_ptrs[MPI_CLOSE_PORT_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_CLOSE_PORT_T], port_name);
}

/*@
   MPI_Close_port - close port

Input Parameters:
.  port_name - a port name (string)

.N NotThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
@*/
int QMPI_Close_port(QMPI_Context context, int tool_id, const char *port_name)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_CLOSE_PORT);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPID_THREAD_CS_ENTER(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_CLOSE_PORT);

    /* ... body of routine ...  */

    mpi_errno = MPIR_Close_port_impl(port_name);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_CLOSE_PORT);
    MPID_THREAD_CS_EXIT(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_close_port", "**mpi_close_port %s", port_name);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}

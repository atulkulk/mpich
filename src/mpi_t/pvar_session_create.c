/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_T_pvar_session_create */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_T_pvar_session_create = PMPI_T_pvar_session_create
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_T_pvar_session_create  MPI_T_pvar_session_create
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_T_pvar_session_create as PMPI_T_pvar_session_create
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_T_pvar_session_create(MPI_T_pvar_session * session)
    __attribute__ ((weak, alias("PMPI_T_pvar_session_create")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_T_pvar_session_create
#define MPI_T_pvar_session_create PMPI_T_pvar_session_create

/* any non-MPI functions go here, especially non-static ones */

int MPIR_T_pvar_session_create_impl(MPI_T_pvar_session * session)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_CHKPMEM_DECL(1);

    *session = MPI_T_PVAR_SESSION_NULL;

    MPIR_CHKPMEM_MALLOC(*session, MPI_T_pvar_session, sizeof(**session), mpi_errno,
                        "performance var session", MPL_MEM_MPIT);

    /* essential for utlist to work */
    (*session)->hlist = NULL;

#ifdef HAVE_ERROR_CHECKING
    (*session)->kind = MPIR_T_PVAR_SESSION;
#endif

    MPIR_CHKPMEM_COMMIT();
  fn_exit:
    return mpi_errno;
  fn_fail:
    MPIR_CHKPMEM_REAP();
    goto fn_exit;
}

#endif /* MPICH_MPI_FROM_PMPI */

int MPI_T_pvar_session_create(MPI_T_pvar_session * session)
{
    QMPI_Context context;
    QMPI_T_pvar_session_create_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_T_pvar_session_create(context, 0, session);

    fn_ptr = (QMPI_T_pvar_session_create_t *) MPIR_QMPI_first_fn_ptrs[MPI_T_PVAR_SESSION_CREATE_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_T_PVAR_SESSION_CREATE_T], session);
}

/*@
MPI_T_pvar_session_create - Create a new session for accessing performance variables

Output Parameters:
. session - identifier of performance session (handle)

.N ThreadSafe

.N Errors
.N MPI_SUCCESS
.N MPI_T_ERR_NOT_INITIALIZED
.N MPI_T_ERR_OUT_OF_SESSIONS
@*/
int QMPI_T_pvar_session_create(QMPI_Context context, int tool_id, MPI_T_pvar_session * session)
{
    int mpi_errno = MPI_SUCCESS;

    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_T_PVAR_SESSION_CREATE);
    MPIR_ERRTEST_MPIT_INITIALIZED(mpi_errno);
    MPIR_T_THREAD_CS_ENTER();
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_T_PVAR_SESSION_CREATE);

    /* Validate parameters, especially handles needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ARGNULL(session, "session", mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    mpi_errno = MPIR_T_pvar_session_create_impl(session);
    if (mpi_errno != MPI_SUCCESS)
        goto fn_fail;

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_T_PVAR_SESSION_CREATE);
    MPIR_T_THREAD_CS_EXIT();
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_t_pvar_session_create", "**mpi_t_pvar_session_create %p",
                                 session);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}

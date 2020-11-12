/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Pack_size */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Pack_size = PMPI_Pack_size
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Pack_size  MPI_Pack_size
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Pack_size as PMPI_Pack_size
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size)
    __attribute__ ((weak, alias("PMPI_Pack_size")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Pack_size
#define MPI_Pack_size PMPI_Pack_size

void MPIR_Pack_size_impl(int incount, MPI_Datatype datatype, MPI_Aint * size)
{
    MPI_Aint typesize;
    MPIR_Datatype_get_size_macro(datatype, typesize);
    *size = incount * typesize;
}


#endif

int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size)
{
    QMPI_Context context;
    QMPI_Pack_size_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Pack_size(context, 0, incount, datatype, comm, size);

    fn_ptr = (QMPI_Pack_size_t *) MPIR_QMPI_first_fn_ptrs[MPI_PACK_SIZE_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_PACK_SIZE_T], incount, datatype, comm,
                      size);
}

/*@
   MPI_Pack_size - Returns the upper bound on the amount of space needed to
                    pack a message

Input Parameters:
+ incount - count argument to packing call (integer)
. datatype - datatype argument to packing call (handle)
- comm - communicator argument to packing call (handle)

Output Parameters:
. size - upper bound on size of packed message, in bytes (integer)

Notes:
The MPI standard document describes this in terms of 'MPI_Pack', but it
applies to both 'MPI_Pack' and 'MPI_Unpack'.  That is, the value 'size' is
the maximum that is needed by either 'MPI_Pack' or 'MPI_Unpack'.

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_COMM
.N MPI_ERR_TYPE
.N MPI_ERR_ARG

@*/
int QMPI_Pack_size(QMPI_Context context, int tool_id, int incount, MPI_Datatype datatype,
                   MPI_Comm comm, int *size)
{
#ifdef HAVE_ERROR_CHECKING
    MPIR_Comm *comm_ptr = NULL;
#endif
    int mpi_errno = MPI_SUCCESS;
    MPI_Aint size_x = MPI_UNDEFINED;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_PACK_SIZE);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_PACK_SIZE);

    /* Validate parameters, especially handles needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_COMM(comm, mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

#ifdef HAVE_ERROR_CHECKING
    {
        /* Convert MPI object handles to object pointers */
        MPIR_Comm_get_ptr(comm, comm_ptr);

        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_Datatype *datatype_ptr = NULL;

            MPIR_ERRTEST_COUNT(incount, mpi_errno);
            MPIR_ERRTEST_ARGNULL(size, "size", mpi_errno);

            MPIR_Comm_valid_ptr(comm_ptr, mpi_errno, FALSE);
            if (mpi_errno)
                goto fn_fail;

            MPIR_ERRTEST_DATATYPE(datatype, "datatype", mpi_errno);

            if (!HANDLE_IS_BUILTIN(datatype)) {
                MPIR_Datatype_get_ptr(datatype, datatype_ptr);
                MPIR_Datatype_valid_ptr(datatype_ptr, mpi_errno);
                MPIR_Datatype_committed_ptr(datatype_ptr, mpi_errno);
                if (mpi_errno)
                    goto fn_fail;
            }
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ... */

    MPIR_Pack_size_impl(incount, datatype, &size_x);
    MPIR_Assign_trunc(*size, size_x, int);

    /* ... end of body of routine ... */

#ifdef HAVE_ERROR_CHECKING
  fn_exit:
#endif
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_PACK_SIZE);
    return mpi_errno;

    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
  fn_fail:
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_pack_size", "**mpi_pack_size %d %D %C %p", incount,
                                 datatype, comm, size);
    }
    mpi_errno = MPIR_Err_return_comm(comm_ptr, __func__, mpi_errno);
    goto fn_exit;
#endif
    /* --END ERROR HANDLING-- */
}

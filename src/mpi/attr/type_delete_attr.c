/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_Type_delete_attr */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_Type_delete_attr = PMPI_Type_delete_attr
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_Type_delete_attr  MPI_Type_delete_attr
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_Type_delete_attr as PMPI_Type_delete_attr
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_Type_delete_attr(MPI_Datatype datatype, int type_keyval)
    __attribute__ ((weak, alias("PMPI_Type_delete_attr")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_Type_delete_attr
#define MPI_Type_delete_attr PMPI_Type_delete_attr

#endif

int MPI_Type_delete_attr(MPI_Datatype datatype, int type_keyval)
{
    QMPI_Context context;
    QMPI_Type_delete_attr_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_Type_delete_attr(context, 0, datatype, type_keyval);

    fn_ptr = (QMPI_Type_delete_attr_t *) MPIR_QMPI_first_fn_ptrs[MPI_TYPE_DELETE_ATTR_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_TYPE_DELETE_ATTR_T], datatype,
                      type_keyval);
}

/*@
   MPI_Type_delete_attr - Deletes an attribute value associated with a key on
   a datatype

Input Parameters:
+  datatype - MPI datatype to which attribute is attached (handle)
-  type_keyval - The key value of the deleted attribute (integer)

.N ThreadSafe

.N Fortran

.N Errors
.N MPI_SUCCESS
.N MPI_ERR_OTHER
.N MPI_ERR_KEYVAL
@*/
int QMPI_Type_delete_attr(QMPI_Context context, int tool_id, MPI_Datatype datatype, int type_keyval)
{
    int mpi_errno = MPI_SUCCESS;
    MPIR_Datatype *type_ptr = NULL;
    MPII_Keyval *keyval_ptr = 0;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_TYPE_DELETE_ATTR);

    MPIR_ERRTEST_INITIALIZED_ORDIE();

    /* The thread lock prevents a valid attr delete on the same datatype
     * but in a different thread from causing problems */
    MPID_THREAD_CS_ENTER(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_TYPE_DELETE_ATTR);

    /* Validate parameters, especially handles needing to be converted */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_DATATYPE(datatype, "datatype", mpi_errno);
            MPIR_ERRTEST_KEYVAL(type_keyval, MPIR_DATATYPE, "datatype", mpi_errno);
            MPIR_ERRTEST_KEYVAL_PERM(type_keyval, mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif

    /* Validate parameters and objects (post conversion) */
    MPIR_Datatype_get_ptr(datatype, type_ptr);
    MPII_Keyval_get_ptr(type_keyval, keyval_ptr);

    /* Validate parameters and objects (post conversion) */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            /* Validate type_ptr */
            MPIR_Datatype_valid_ptr(type_ptr, mpi_errno);
            /* If type_ptr is not valid, it will be reset to null */
            /* Validate keyval_ptr */
            if (mpi_errno)
                goto fn_fail;
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    mpi_errno = MPIR_Type_delete_attr_impl(type_ptr, keyval_ptr);
    if (mpi_errno != MPI_SUCCESS) {
        goto fn_fail;
    }

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_TYPE_DELETE_ATTR);
    MPID_THREAD_CS_EXIT(GLOBAL, MPIR_THREAD_GLOBAL_ALLFUNC_MUTEX);
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_type_delete_attr", "**mpi_type_delete_attr %D %d", datatype,
                                 type_keyval);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}

/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_T_enum_get_item */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_T_enum_get_item = PMPI_T_enum_get_item
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_T_enum_get_item  MPI_T_enum_get_item
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_T_enum_get_item as PMPI_T_enum_get_item
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_T_enum_get_item(MPI_T_enum enumtype, int indx, int *value, char *name, int *name_len)
    __attribute__ ((weak, alias("PMPI_T_enum_get_item")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_T_enum_get_item
#define MPI_T_enum_get_item PMPI_T_enum_get_item
#endif /* MPICH_MPI_FROM_PMPI */

int MPI_T_enum_get_item(MPI_T_enum enumtype, int indx, int *value, char *name, int *name_len)
{
    QMPI_Context context;
    QMPI_T_enum_get_item_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_T_enum_get_item(context, 0, enumtype, indx, value, name, name_len);

    fn_ptr = (QMPI_T_enum_get_item_t *) MPIR_QMPI_first_fn_ptrs[MPI_T_ENUM_GET_ITEM_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_T_ENUM_GET_ITEM_T], enumtype, indx,
                      value, name, name_len);
}

/*@
MPI_T_enum_get_item - Get the information about an item in an enumeration

Input/Output Parameters:
. name_len - length of the string and/or buffer for name (integer)

Input Parameters:
. enumtype - enumeration to be queried (handle)

Output Parameters:
+ indx - number of the value to be queried in this enumeration (integer)
. value - variable value (integer)
- name - buffer to return the string containing the name of the enumeration item (string)

.N ThreadSafe

.N Errors
.N MPI_SUCCESS
.N MPI_T_ERR_NOT_INITIALIZED
.N MPI_T_ERR_INVALID_HANDLE
.N MPI_T_ERR_INVALID_ITEM
@*/
int QMPI_T_enum_get_item(QMPI_Context context, int tool_id, MPI_T_enum enumtype, int indx,
                         int *value, char *name, int *name_len)
{
    int mpi_errno = MPI_SUCCESS;
    enum_item_t *item;

    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_T_ENUM_GET_ITEM);
    MPIR_ERRTEST_MPIT_INITIALIZED(mpi_errno);
    MPIR_T_THREAD_CS_ENTER();
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_T_ENUM_GET_ITEM);

    /* Validate parameters */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_ENUM_HANDLE(enumtype, mpi_errno);
            MPIR_ERRTEST_ENUM_ITEM(enumtype, indx, mpi_errno);
            MPIR_ERRTEST_ARGNULL(value, "value", mpi_errno);
            /* Do not do TEST_ARGNULL for name or name_len, since this is
             * permitted per MPI_T standard.
             */
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    item = (enum_item_t *) utarray_eltptr(enumtype->items, indx);
    *value = item->value;
    MPIR_T_strncpy(name, item->name, name_len);

    /* ... end of body of routine ... */

#ifdef HAVE_ERROR_CHECKING
  fn_exit:
#endif
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_T_ENUM_GET_ITEM);
    MPIR_T_THREAD_CS_EXIT();
    return mpi_errno;

#ifdef HAVE_ERROR_CHECKING
  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_t_enum_get_item", "**mpi_t_enum_get_item %p %d %p %p %p",
                                 enumtype, indx, value, name, name_len);
    }
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
#endif
}

/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpiimpl.h"

/* -- Begin Profiling Symbol Block for routine MPI_T_category_get_cvars */
#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_T_category_get_cvars = PMPI_T_category_get_cvars
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_T_category_get_cvars  MPI_T_category_get_cvars
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_T_category_get_cvars as PMPI_T_category_get_cvars
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_T_category_get_cvars(int cat_index, int len, int indices[])
    __attribute__ ((weak, alias("PMPI_T_category_get_cvars")));
#endif
/* -- End Profiling Symbol Block */

/* Define MPICH_MPI_FROM_PMPI if weak symbols are not supported to build
   the MPI routines */
#ifndef MPICH_MPI_FROM_PMPI
#undef MPI_T_category_get_cvars
#define MPI_T_category_get_cvars PMPI_T_category_get_cvars

/* any non-MPI functions go here, especially non-static ones */

int MPIR_T_category_get_cvars_impl(int cat_index, int len, int indices[])
{
    int mpi_errno = MPI_SUCCESS;
    cat_table_entry_t *cat;
    int i, num_cvars, count;

    cat = (cat_table_entry_t *) utarray_eltptr(cat_table, cat_index);
    num_cvars = utarray_len(cat->cvar_indices);
    count = len < num_cvars ? len : num_cvars;

    for (i = 0; i < count; i++) {
        indices[i] = *(int *) utarray_eltptr(cat->cvar_indices, i);
    }

    return mpi_errno;
}

#endif /* MPICH_MPI_FROM_PMPI */

int MPI_T_category_get_cvars(int cat_index, int len, int indices[])
{
    QMPI_Context context;
    QMPI_T_category_get_cvars_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_T_category_get_cvars(context, 0, cat_index, len, indices);

    fn_ptr = (QMPI_T_category_get_cvars_t *) MPIR_QMPI_first_fn_ptrs[MPI_T_CATEGORY_GET_CVARS_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_T_CATEGORY_GET_CVARS_T], cat_index, len,
                      indices);
}

/*@
MPI_T_category_get_cvars - Get control variables in a category

Input Parameters:
+ cat_index - index of the category to be queried, in the range [0,N-1] (integer)
- len - the length of the indices array (integer)

Output Parameters:
. indices - an integer array of size len, indicating control variable indices (array of integers)

.N ThreadSafe

.N Errors
.N MPI_SUCCESS
.N MPI_T_ERR_NOT_INITIALIZED
.N MPI_T_ERR_INVALID_INDEX
@*/
int QMPI_T_category_get_cvars(QMPI_Context context, int tool_id, int cat_index, int len,
                              int indices[])
{
    int mpi_errno = MPI_SUCCESS;

    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPI_T_CATEGORY_GET_CVARS);
    MPIR_ERRTEST_MPIT_INITIALIZED(mpi_errno);
    MPIR_T_THREAD_CS_ENTER();
    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPI_T_CATEGORY_GET_CVARS);

    /* Validate parameters */
#ifdef HAVE_ERROR_CHECKING
    {
        MPID_BEGIN_ERROR_CHECKS;
        {
            MPIR_ERRTEST_CAT_INDEX(cat_index, mpi_errno);
            if (len != 0)
                MPIR_ERRTEST_ARGNULL(indices, "indices", mpi_errno);
        }
        MPID_END_ERROR_CHECKS;
    }
#endif /* HAVE_ERROR_CHECKING */

    /* ... body of routine ...  */

    if (len == 0)
        goto fn_exit;

    mpi_errno = MPIR_T_category_get_cvars_impl(cat_index, len, indices);
    MPIR_ERR_CHECK(mpi_errno);

    /* ... end of body of routine ... */

  fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPI_T_CATEGORY_GET_CVARS);
    MPIR_T_THREAD_CS_EXIT();
    return mpi_errno;

  fn_fail:
    /* --BEGIN ERROR HANDLING-- */
#ifdef HAVE_ERROR_CHECKING
    {
        mpi_errno =
            MPIR_Err_create_code(mpi_errno, MPIR_ERR_RECOVERABLE, __func__, __LINE__, MPI_ERR_OTHER,
                                 "**mpi_t_category_get_cvars",
                                 "**mpi_t_category_get_cvars %d %d %p", cat_index, len, indices);
    }
#endif
    mpi_errno = MPIR_Err_return_comm(NULL, __func__, mpi_errno);
    goto fn_exit;
    /* --END ERROR HANDLING-- */
}

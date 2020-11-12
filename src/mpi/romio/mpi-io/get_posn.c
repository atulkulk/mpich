/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpioimpl.h"
#include "adioi.h"

/* -- Begin Profiling Symbol Block */
#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_File_get_position = PMPI_File_get_position
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_File_get_position MPI_File_get_position
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_File_get_position as PMPI_File_get_position
/* end of weak pragmas */
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_File_get_position(MPI_File fh, MPI_Offset * offset)
    __attribute__ ((weak, alias("PMPI_File_get_position")));
#endif
/* -- End Profiling Symbol Block */

/* Include mapping from MPI->PMPI */
#define MPIO_BUILD_PROFILING
#include "mpioprof.h"
#endif

int MPI_File_get_position(MPI_File fh, MPI_Offset * offset)
{
    QMPI_Context context;
    QMPI_File_get_position_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_File_get_position(context, 0, fh, offset);

    fn_ptr = (QMPI_File_get_position_t *) MPIR_QMPI_first_fn_ptrs[MPI_FILE_GET_POSITION_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_FILE_GET_POSITION_T], fh, offset);
}

/*@
    MPI_File_get_position - Returns the current position of the
                individual file pointer in etype units relative to
                the current view

Input Parameters:
. fh - file handle (handle)

Output Parameters:
. offset - offset of individual file pointer (nonnegative integer)

.N fortran
@*/
int QMPI_File_get_position(QMPI_Context context, int tool_id, MPI_File fh, MPI_Offset * offset)
{
    int error_code;
    ADIO_File adio_fh;
    static char myname[] = "MPI_FILE_GET_POSITION";

    adio_fh = MPIO_File_resolve(fh);

    /* --BEGIN ERROR HANDLING-- */
    MPIO_CHECK_FILE_HANDLE(adio_fh, myname, error_code);
    MPIO_CHECK_NOT_SEQUENTIAL_MODE(adio_fh, myname, error_code);
    /* --END ERROR HANDLING-- */

    ADIOI_Get_position(adio_fh, offset);

  fn_exit:
    return MPI_SUCCESS;
}

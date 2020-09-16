/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpioimpl.h"

/* -- Begin Profiling Symbol Block */
#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_File_read_at_all_end = PMPI_File_read_at_all_end
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_File_read_at_all_end MPI_File_read_at_all_end
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_File_read_at_all_end as PMPI_File_read_at_all_end
/* end of weak pragmas */
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_File_read_at_all_end(MPI_File fh, void *buf, MPI_Status * status)
    __attribute__ ((weak, alias("PMPI_File_read_at_all_end")));
#endif
/* -- End Profiling Symbol Block */

/* Include mapping from MPI->PMPI */
#define MPIO_BUILD_PROFILING
#include "mpioprof.h"
#endif

/*@
    MPI_File_read_at_all_end - Complete a split collective read using
    explicit offset

Input Parameters:
. fh - file handle (handle)

Output Parameters:
. buf - initial address of buffer (choice)
. status - status object (Status)

.N fortran
@*/
int MPI_File_read_at_all_end(MPI_File fh, void *buf, MPI_Status * status)
{
    int error_code;
    static char myname[] = "MPI_FILE_READ_AT_ALL_END";


    error_code = MPIOI_File_read_all_end(fh, buf, myname, status);

    return error_code;
}

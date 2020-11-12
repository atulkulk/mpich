/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpioimpl.h"
#include "adioi.h"

/* -- Begin Profiling Symbol Block */
#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_File_seek = PMPI_File_seek
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_File_seek MPI_File_seek
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_File_seek as PMPI_File_seek
/* end of weak pragmas */
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_File_seek(MPI_File fh, MPI_Offset offset, int whence)
    __attribute__ ((weak, alias("PMPI_File_seek")));
#endif
/* -- End Profiling Symbol Block */

/* Include mapping from MPI->PMPI */
#define MPIO_BUILD_PROFILING
#include "mpioprof.h"
#endif

int MPI_File_seek(MPI_File fh, MPI_Offset offset, int whence)
{
    QMPI_Context context;
    QMPI_File_seek_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_File_seek(context, 0, fh, offset, whence);

    fn_ptr = (QMPI_File_seek_t *) MPIR_QMPI_first_fn_ptrs[MPI_FILE_SEEK_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_FILE_SEEK_T], fh, offset, whence);
}

/*@
    MPI_File_seek - Updates the individual file pointer

Input Parameters:
. fh - file handle (handle)
. offset - file offset (integer)
. whence - update mode (state)

.N fortran
@*/
int QMPI_File_seek(QMPI_Context context, int tool_id, MPI_File fh, MPI_Offset offset, int whence)
{
    int error_code;
    ADIO_File adio_fh;
    static char myname[] = "MPI_FILE_SEEK";
    MPI_Offset curr_offset, eof_offset;

#ifdef MPI_hpux
    int fl_xmpi;

    HPMP_IO_START(fl_xmpi, BLKMPIFILESEEK, TRDTBLOCK, adio_fh, MPI_DATATYPE_NULL, -1);
#endif /* MPI_hpux */

    ROMIO_THREAD_CS_ENTER();

    adio_fh = MPIO_File_resolve(fh);

    /* --BEGIN ERROR HANDLING-- */
    MPIO_CHECK_FILE_HANDLE(adio_fh, myname, error_code);
    MPIO_CHECK_NOT_SEQUENTIAL_MODE(adio_fh, myname, error_code);
    /* --END ERROR HANDLING-- */

    switch (whence) {
        case MPI_SEEK_SET:
            /* --BEGIN ERROR HANDLING-- */
            if (offset < 0) {
                error_code = MPIO_Err_create_code(MPI_SUCCESS,
                                                  MPIR_ERR_RECOVERABLE, myname,
                                                  __LINE__, MPI_ERR_ARG, "**iobadoffset", 0);
                error_code = MPIO_Err_return_file(adio_fh, error_code);
                goto fn_exit;
            }
            /* --END ERROR HANDLING-- */
            break;
        case MPI_SEEK_CUR:
            /* find offset corr. to current location of file pointer */
            ADIOI_Get_position(adio_fh, &curr_offset);
            offset += curr_offset;

            /* --BEGIN ERROR HANDLING-- */
            if (offset < 0) {
                error_code = MPIO_Err_create_code(MPI_SUCCESS,
                                                  MPIR_ERR_RECOVERABLE, myname,
                                                  __LINE__, MPI_ERR_ARG, "**ionegoffset", 0);
                error_code = MPIO_Err_return_file(adio_fh, error_code);
                goto fn_exit;
            }
            /* --END ERROR HANDLING-- */

            break;
        case MPI_SEEK_END:
            /* we can in many cases do seeks w/o a file actually opened, but not in
             * the MPI_SEEK_END case */
            ADIOI_TEST_DEFERRED(adio_fh, "MPI_File_seek", &error_code);

            /* find offset corr. to end of file */
            ADIOI_Get_eof_offset(adio_fh, &eof_offset);
            offset += eof_offset;

            /* --BEGIN ERROR HANDLING-- */
            if (offset < 0) {
                error_code = MPIO_Err_create_code(MPI_SUCCESS,
                                                  MPIR_ERR_RECOVERABLE, myname,
                                                  __LINE__, MPI_ERR_ARG, "**ionegoffset", 0);
                error_code = MPIO_Err_return_file(adio_fh, error_code);
                goto fn_exit;
            }
            /* --END ERROR HANDLING-- */

            break;
        default:
            /* --BEGIN ERROR HANDLING-- */
            error_code = MPIO_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE,
                                              myname, __LINE__, MPI_ERR_ARG, "**iobadwhence", 0);
            error_code = MPIO_Err_return_file(adio_fh, error_code);
            goto fn_exit;
            /* --END ERROR HANDLING-- */
    }

    ADIO_SeekIndividual(adio_fh, offset, ADIO_SEEK_SET, &error_code);
    /* TODO: what do we do with this error? */

    /* --BEGIN ERROR HANDLING-- */
    if (error_code != MPI_SUCCESS)
        error_code = MPIO_Err_return_file(adio_fh, error_code);
    /* --END ERROR HANDLING-- */

#ifdef MPI_hpux
    HPMP_IO_END(fl_xmpi, adio_fh, MPI_DATATYPE_NULL, -1);
#endif /* MPI_hpux */

    error_code = MPI_SUCCESS;

  fn_exit:
    ROMIO_THREAD_CS_EXIT();
    return error_code;
}

/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpioimpl.h"

/* -- Begin Profiling Symbol Block */
#ifdef HAVE_WEAK_SYMBOLS

#if defined(HAVE_PRAGMA_WEAK)
#pragma weak MPI_File_iwrite_all = PMPI_File_iwrite_all
#elif defined(HAVE_PRAGMA_HP_SEC_DEF)
#pragma _HP_SECONDARY_DEF PMPI_File_iwrite_all MPI_File_iwrite_all
#elif defined(HAVE_PRAGMA_CRI_DUP)
#pragma _CRI duplicate MPI_File_iwrite_all as PMPI_File_iwrite_all
/* end of weak pragmas */
#elif defined(HAVE_WEAK_ATTRIBUTE)
int MPI_File_iwrite_all(MPI_File fh, const void *buf, int count, MPI_Datatype datatype,
                        MPI_Request * request)
    __attribute__ ((weak, alias("PMPI_File_iwrite_all")));
#endif
/* -- End Profiling Symbol Block */

/* Include mapping from MPI->PMPI */
#define MPIO_BUILD_PROFILING
#include "mpioprof.h"
#endif

#ifdef HAVE_MPI_GREQUEST
#include "mpiu_greq.h"
#endif

int MPI_File_iwrite_all(MPI_File fh, ROMIO_CONST void *buf, int count,
                        MPI_Datatype datatype, MPI_Request * request)
{
    QMPI_Context context;
    QMPI_File_iwrite_all_t *fn_ptr;

    context.storage_stack = NULL;

    if (MPIR_QMPI_num_tools == 0)
        return QMPI_File_iwrite_all(context, 0, fh, buf, count, datatype, request);

    fn_ptr = (QMPI_File_iwrite_all_t *) MPIR_QMPI_first_fn_ptrs[MPI_FILE_IWRITE_ALL_T];

    return (*fn_ptr) (context, MPIR_QMPI_first_tool_ids[MPI_FILE_IWRITE_ALL_T], fh, buf, count,
                      datatype, request);
}

/*@
    MPI_File_iwrite_all - Nonblocking collective write using individual file pointer

Input Parameters:
. fh - file handle (handle)
. buf - initial address of buffer (choice)
. count - number of elements in buffer (nonnegative integer)
. datatype - datatype of each buffer element (handle)

Output Parameters:
. request - request object (handle)

.N fortran
@*/
int QMPI_File_iwrite_all(QMPI_Context context, int tool_id, MPI_File fh, ROMIO_CONST void *buf,
                         int count, MPI_Datatype datatype, MPI_Request * request)
{
    int error_code;
    static char myname[] = "MPI_FILE_IWRITE_ALL";
#ifdef MPI_hpux
    int fl_xmpi;

    HPMP_IO_START(fl_xmpi, BLKMPIFILEIWRITEALL, TRDTBLOCK, fh, datatype, count);
#endif /* MPI_hpux */

    error_code = MPIOI_File_iwrite_all(fh, (MPI_Offset) 0,
                                       ADIO_INDIVIDUAL, buf, count, datatype, myname, request);

#ifdef MPI_hpux
    HPMP_IO_END(fl_xmpi, fh, datatype, count);
#endif /* MPI_hpux */

    return error_code;
}

/* Note: MPIOI_File_iwrite_all also used by MPI_File_iwrite_at_all */
/* prevent multiple definitions of this routine */
#ifdef MPIO_BUILD_PROFILING
int MPIOI_File_iwrite_all(MPI_File fh,
                          MPI_Offset offset,
                          int file_ptr_type,
                          const void *buf,
                          int count, MPI_Datatype datatype, char *myname, MPI_Request * request)
{
    int error_code;
    MPI_Count datatype_size;
    ADIO_File adio_fh;
    void *e32buf = NULL;
    const void *xbuf = NULL;

    ROMIO_THREAD_CS_ENTER();

    adio_fh = MPIO_File_resolve(fh);

    /* --BEGIN ERROR HANDLING-- */
    MPIO_CHECK_FILE_HANDLE(adio_fh, myname, error_code);
    MPIO_CHECK_COUNT(adio_fh, count, myname, error_code);
    MPIO_CHECK_DATATYPE(adio_fh, datatype, myname, error_code);

    if (file_ptr_type == ADIO_EXPLICIT_OFFSET && offset < 0) {
        error_code = MPIO_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE,
                                          myname, __LINE__, MPI_ERR_ARG, "**iobadoffset", 0);
        error_code = MPIO_Err_return_file(adio_fh, error_code);
        goto fn_exit;
    }
    /* --END ERROR HANDLING-- */

    MPI_Type_size_x(datatype, &datatype_size);

    /* --BEGIN ERROR HANDLING-- */
    MPIO_CHECK_INTEGRAL_ETYPE(adio_fh, count, datatype_size, myname, error_code);
    MPIO_CHECK_WRITABLE(adio_fh, myname, error_code);
    MPIO_CHECK_NOT_SEQUENTIAL_MODE(adio_fh, myname, error_code);
    MPIO_CHECK_COUNT_SIZE(adio_fh, count, datatype_size, myname, error_code);
    /* --END ERROR HANDLING-- */

    xbuf = buf;
    if (adio_fh->is_external32) {
        error_code = MPIU_external32_buffer_setup(buf, count, datatype, &e32buf);
        if (error_code != MPI_SUCCESS)
            goto fn_exit;

        xbuf = e32buf;
    }

    ADIO_IwriteStridedColl(adio_fh, xbuf, count, datatype, file_ptr_type,
                           offset, request, &error_code);

    /* --BEGIN ERROR HANDLING-- */
    if (error_code != MPI_SUCCESS)
        error_code = MPIO_Err_return_file(adio_fh, error_code);
    /* --END ERROR HANDLING-- */

  fn_exit:
    if (e32buf != NULL)
        ADIOI_Free(e32buf);
    ROMIO_THREAD_CS_EXIT();

    return error_code;
}
#endif

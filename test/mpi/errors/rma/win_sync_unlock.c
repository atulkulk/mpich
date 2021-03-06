/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "mpi.h"
#include <stdio.h>
#include "mpitest.h"
#include "win_sync.h"

int main(int argc, char *argv[])
{
    int rank;
    int errors = 0, errs = 0;
    int buf = 0;
    MPI_Win win;

    MTest_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Win_create(&buf, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_set_errhandler(win, MPI_ERRORS_RETURN);

    /* This should fail because the window is not locked. */
    CHECK_ERR(MPI_Win_unlock(0, win));

    MPI_Win_free(&win);

    MTest_Finalize(errors);

    return MTestReturnValue(errs);
}

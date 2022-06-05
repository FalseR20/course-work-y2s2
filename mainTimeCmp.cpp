#include <mpi.h>
#include "stdio.h"

#include "Image.h"
#include "ImageMPI.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double time;

    if (rank == 0) {
        time = MPI_Wtime();

        Image image("./jpg/space5k.jpg");
        image.average();
        image.increaseDecrease();
        image.save("./jpg/result1.jpg");

        printf("------------------------------\nTime without MPI: %2.2f sec\n------------------------------\n",
                MPI_Wtime() - time);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        time = MPI_Wtime();

    ImageMPI image("./jpg/space5k.jpg");
    image.average();
    image.increaseDecrease();
    image.save("./jpg/result5.jpg");

    if (rank == 0)
        printf("------------------------------\nTime with MPI: %2.2f sec\n------------------------------\n",
               MPI_Wtime() - time);

    MPI_Finalize();
}

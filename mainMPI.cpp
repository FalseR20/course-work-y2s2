#include <mpi.h>

#include "ImageMPI.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    ImageMPI image("./jpg/space5k.jpg");
    image.average();
    image.increaseDecrease();
    image.save("./jpg/result5.jpg");

    MPI_Finalize();
}

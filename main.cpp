#include "Image.h"

int main(int argc, char **argv) {
    Image image("..\\jpg\\space5k.jpg", argc, argv);
    image.average_MPI();
    image.save("..\\jpg\\new_image.jpg");
}

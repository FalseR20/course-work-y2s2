#include "Image.h"

int main(int argc, char **argv) {
    Image image("./jpg/space5k.jpg", argc, argv);
    image.average();
    image.save("./jpg/result1.jpg");
}

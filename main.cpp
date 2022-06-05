#include "Image.h"

int main() {
    Image image("./jpg/space5k.jpg");
    image.average();
    image.increase_decrease();
    image.save("./jpg/result1.jpg");
}

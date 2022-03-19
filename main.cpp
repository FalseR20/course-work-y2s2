#include "Image.h"

int main() {
    Image image("..\\jpg\\image.jpg");
    image.debug = true;
    image.roberts();
    image.save("..\\jpg\\new_image.jpg");
}
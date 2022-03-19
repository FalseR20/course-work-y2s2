#include "Image.h"

int main() {
    Image image("..\\jpg\\space5k.jpg");
    image.roberts();
    image.save("..\\jpg\\new_image.jpg", 91);
}
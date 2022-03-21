#include "Image.h"


int main() {
    Image image("..\\jpg\\city5k.jpg");
//    image.debug = true;
    image.average();
    image.save("..\\jpg\\new_image.jpg");
}
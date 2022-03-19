#include <turbojpeg.h>
#include <cstdio>
#include <cstdlib>

#include "Image.h"

Image::Image(const char *filename) {
    FILE *file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END); // set pointer to the end of file for getting file size
    jpegSize = ftell(file); // get file size
    fseek(file, 0, SEEK_SET);
    jpegBuf = new unsigned char[jpegSize];
    fread(jpegBuf, jpegSize, 1, file); //get a pointer to a buffer containing JPEG image in jpegBuf
    fclose(file);

    tjhandle handle = tjInitDecompress();
    int jpegSubsamp = TJSAMP_GRAY;
    tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    image = new unsigned char[width * height];
    tjDecompress2(handle, jpegBuf, jpegSize, image, width, width, height, TJPF_GRAY, 0);
}

void Image::roberts() {
    auto *new_image = new unsigned char[width * height];
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width; j++) {
            int new_pixel = abs((image[i * width + j] - image[(i + 1) * width + (j + 1)])) +
                            abs((image[i * width + (j + 1)] - image[(i + 1) * width + j]));
            new_image[i * width + j] = new_pixel;
        }
    }
    delete image;
    image = new_image;
}

void Image::save(const char *filename) {
    tjhandle handle = tjInitCompress();
    tjCompress2(handle, image, width, width, height, TJPF_GRAY, &jpegBuf, &jpegSize,
                TJSAMP_GRAY, 100, 0);
    FILE *file = fopen(filename, "wb");

    fwrite(jpegBuf, jpegSize, 1, file);
}

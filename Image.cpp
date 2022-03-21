#include <cstdio>
#include <iostream>
#include <turbojpeg.h>

#include "Image.h"

Image::Image(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == nullptr) {
        std::cout << "File haven't found\n";
        exit(0);
    }

    fseek(file, 0, SEEK_END); // set pointer to the end of file for getting file size
    jpegSize = ftell(file); // get file size
    fseek(file, 0, SEEK_SET);
    jpegBuf = new unsigned char[jpegSize];
    fread(jpegBuf, jpegSize, 1, file); //get a pointer to a buffer containing JPEG image in jpegBuf
    fclose(file);

    tjhandle handle = tjInitDecompress();
    int jpegSubsamp = TJSAMP_GRAY;
    tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    nPixels = width * height;
    image = new unsigned char[nPixels];
    tjDecompress2(handle, jpegBuf, jpegSize, image, width, width, height, TJPF_GRAY, 0);
}

void Image::average() {
    auto *new_image = new unsigned char[nPixels];
    int buff_pixel;
    int counter;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            buff_pixel = 0;
            counter = 0;
            for (int y = std::max(i - 1, 0); y < std::min(i + 1, height - 1); y++) {
                for (int x = std::max(j - 1, 0); x < std::min(j + 1, width - 1); x++) {
                    buff_pixel += image[y * height + x];
                    counter++;
                }
            }
            new_image[i * width + j] = buff_pixel / counter;
        }
    }
    delete image;
    image = new_image;
}

void Image::save(const char *filename, int quality) {
    tjhandle handle = tjInitCompress();
    tjCompress2(handle, image, width, width, height, TJPF_GRAY, &jpegBuf, &jpegSize,
                TJSAMP_GRAY, quality, 0);
    FILE *file = fopen(filename, "wb");

    fwrite(jpegBuf, jpegSize, 1, file);
}

#include <iostream>
#include <turbojpeg.h>
#include <valarray>

#include "Image.h"

Image::Image(const char *filename) {
    load(filename);
}

Image::~Image() {
    delete[] image;
}

void Image::load(const char *filename) {
    printf("Loading image\n");
    FILE *file = fopen(filename, "rb");
    if (file == nullptr) {
        printf("File hasn't found\n");
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

void Image::save(const char *filename, int quality) {
    printf("Saving image\n");
    tjhandle handle = tjInitCompress();
    tjCompress2(handle, image, width, width, height, TJPF_GRAY, &jpegBuf, &jpegSize,
                TJSAMP_GRAY, quality, 0);
    FILE *file = fopen(filename, "wb");
    fwrite(jpegBuf, jpegSize, 1, file);
}

void Image::average() {
    printf("Averaging image\n");
    auto *newImage = new unsigned char[nPixels];
    int pixel, counter;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixel = counter = 0;
            for (int ii = std::max(i - 1, 0); ii <= std::min(i + 1, height - 1); ii++) {
                for (int jj = std::max(j - 1, 0); jj <= std::min(j + 1, width - 1); jj++) {
                    pixel += image[ii * width + jj];
                    counter++;
                }
            }
            newImage[i * width + j] = pixel / counter;
        }
    }
    delete image;
    image = newImage;
}

void Image::increaseDecrease() {
    printf("Changing image with increase-decrease method\n");
    increaseDecreaseFunc(std::max);
    increaseDecreaseFunc(std::min);
    increaseDecreaseFunc(std::min);
    increaseDecreaseFunc(std::max);
}

void Image::increaseDecreaseFunc(const unsigned char &maxOrMin(const unsigned char &, const unsigned char &)) {
    auto *newImage = new unsigned char[nPixels];
    unsigned char pixel;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixel = 255 - maxOrMin(0, 255);
            for (int ii = std::max(i - 1, 0); ii <= std::min(i + 1, height - 1); ii++) {
                for (int jj = std::max(j - 1, 0); jj <= std::min(j + 1, width - 1); jj++) {
                    pixel = maxOrMin(pixel, image[ii * width + jj]);
                }
            }
            newImage[i * width + j] = pixel;
        }
    }
    delete image;
    image = newImage;
}
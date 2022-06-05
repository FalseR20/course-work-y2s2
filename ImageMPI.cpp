#include <iostream>
#include <turbojpeg.h>
#include <mpi.h>
#include <cstring>

#include "ImageMPI.h"

Image::Image(const char *filename, int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    sprintf(threadName, "thread %d/%d", rank, size);
    load(filename);
}

Image::~Image() {
    if (rank == 0) {
        delete[] image;
    }
    MPI_Finalize();
}

void Image::load(const char *filename) {
    if (rank == 0) {
        printf("Loading image with %s\n", threadName);
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
    } else {
        width = 5000;
        height = 5000;
        nPixels = 25000000;
    }
}

void Image::save(const char *filename, int quality) {
    if (rank == 0) {
        printf("Saving image with %s\n", threadName);
        tjhandle handle = tjInitCompress();
        tjCompress2(handle, image, width, width, height, TJPF_GRAY, &jpegBuf, &jpegSize,
                    TJSAMP_GRAY, quality, 0);
        FILE *file = fopen(filename, "wb");
        fwrite(jpegBuf, jpegSize, 1, file);
    }
}

void Image::average() {
    printf("Averaging with %s\n", threadName);
    if (size == 1) {
        auto *newImage = new unsigned char[nPixels];
        int buffPixel;
        int counter;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                buffPixel = 0;
                counter = 0;
                for (int y = std::max(i - 1, 0); y <= std::min(i + 1, height - 1); y++) {
                    for (int x = std::max(j - 1, 0); x <= std::min(j + 1, width - 1); x++) {
                        buffPixel += image[y * width + x];
                        counter++;
                    }
                }
                newImage[i * width + j] = buffPixel / counter;
            }
        }
        delete image;
        image = newImage;
    } else {
        unsigned nSharedPixels = nPixels / size;
        unsigned char *sharedImage = new unsigned char[nSharedPixels];
        unsigned char *newSharedImage;
        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                MPI_Send(image + i * nSharedPixels, nSharedPixels, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
            }
            strncpy((char *) sharedImage, (const char *) image, nSharedPixels);
            newSharedImage = image;
        } else {
            newSharedImage = new unsigned char[nSharedPixels];
            MPI_Recv(sharedImage, nSharedPixels, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        }
//        printf("%s: %d %d\n", threadName, sharedImage, nSharedPixels);
        unsigned pixel;
        unsigned counter;
        int sharedHeigth = height / size;
//        if (rank == 0)
        for (int i = 0; i < sharedHeigth; i++) {
            for (int j = 0; j < width; j++) {
                pixel = 0;
                counter = 0;
                for (int y = std::max(i - 1, 0); y <= std::min(i + 1, sharedHeigth - 1); y++) {
                    for (int x = std::max(j - 1, 0); x <= std::min(j + 1, sharedHeigth - 1); x++) {
                        pixel += sharedImage[y * width + x];
                        counter++;
                    }
                }
                newSharedImage[i * width + j] = pixel / counter;
            }
        }
//        if (rank > 0) {
//            MPI_Send(newSharedImage, nSharedPixels, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
//        }
//        else {
//            strncpy((char *) image, (const char *) newSharedImage, nSharedPixels);
//            for (int i = 1; i < size; i++) {
//                MPI_Recv(newSharedImage, nSharedPixels, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &status);
//            }
//            delete[] newSharedImage;
//        }
//        delete[] sharedImage;
    }
}

int Image::getRank() {
    return rank;
}

int Image::getSize() {
    return size;
}

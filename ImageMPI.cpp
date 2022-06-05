#include <iostream>
#include <turbojpeg.h>
#include <mpi.h>
#include <cstring>

#include "ImageMPI.h"

ImageMPI::ImageMPI(const char *filename) {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    workers_count = size - 1;
    isLead = (rank == 0);
    load(filename);
}

ImageMPI::~ImageMPI() {
    if (isLead) {
        delete[] image;
        delete[] jpegBuf;
    }
}

void ImageMPI::load(const char *filename) {
    if (isLead) {
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

    int bufPos = 0;
    size_t tempBufSize = sizeof(width) + sizeof(height);
    char tempBuf[tempBufSize];
    if (isLead) {
        MPI_Pack(&width, 1, MPI_INT, tempBuf, tempBufSize, &bufPos, MPI_COMM_WORLD);
        MPI_Pack(&height, 1, MPI_INT, tempBuf, tempBufSize, &bufPos, MPI_COMM_WORLD);
    }
    MPI_Bcast(tempBuf, tempBufSize, MPI_BYTE, 0, MPI_COMM_WORLD);
    if (!isLead) {
        MPI_Unpack(tempBuf, tempBufSize, &bufPos, &width, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(tempBuf, tempBufSize, &bufPos, &height, 1, MPI_INT, MPI_COMM_WORLD);
        nPixels = width * height;
    }
}

void ImageMPI::save(const char *filename, int quality) {
    if (isLead) {
        printf("Saving image\n");
        tjhandle handle = tjInitCompress();
        tjCompress2(handle, image, width, width, height, TJPF_GRAY, &jpegBuf, &jpegSize,
                    TJSAMP_GRAY, quality, 0);
        FILE *file = fopen(filename, "wb");
        fwrite(jpegBuf, jpegSize, 1, file);
    }
}

void ImageMPI::average() {
    int nSharedPixels = nPixels / workers_count;
    if (isLead) {
        printf("Averaging image\n");
        for (int i = 1; i < size; i++)
            MPI_Send(&image[(i - 1) * nSharedPixels], nSharedPixels, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
        for (int i = 1; i < size; i++)
            MPI_Recv(&image[(i - 1) * nSharedPixels], nSharedPixels, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &status);
    } else {
        image = new unsigned char[nSharedPixels];
        MPI_Recv(image, nSharedPixels, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        auto newImage = new unsigned char[nSharedPixels];
        int sharedHeigth = height / workers_count;
        int pixel, counter;
        for (int i = 0; i < sharedHeigth; i++) {
            for (int j = 0; j < width; j++) {
                pixel = counter = 0;
                for (int ii = std::max(i - 1, 0); ii <= std::min(i + 1, sharedHeigth - 1); ii++) {
                    for (int jj = std::max(j - 1, 0); jj <= std::min(j + 1, width - 1); jj++) {
                        pixel += image[ii * width + jj];
                        counter++;
                    }
                }
                newImage[i * width + j] = pixel / counter;
            }
        }
        MPI_Send(newImage, nSharedPixels, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
        delete[] image;
        delete[] newImage;
    }
}

void ImageMPI::increaseDecrease() {
    if (isLead) printf("Changing image with increase-decrease method\n");
    increaseDecreaseFunc(std::max);
    increaseDecreaseFunc(std::min);
    increaseDecreaseFunc(std::min);
    increaseDecreaseFunc(std::max);
}

void ImageMPI::increaseDecreaseFunc(const unsigned char &maxOrMin(const unsigned char &, const unsigned char &)) {
    int nSharedPixels = nPixels / workers_count;
    if (isLead) {
        for (int i = 1; i < size; i++)
            MPI_Send(&image[(i - 1) * nSharedPixels], nSharedPixels, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
        for (int i = 1; i < size; i++)
            MPI_Recv(&image[(i - 1) * nSharedPixels], nSharedPixels, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &status);
    } else {
        image = new unsigned char[nSharedPixels];
        MPI_Recv(image, nSharedPixels, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        auto newImage = new unsigned char[nSharedPixels];
        int sharedHeigth = height / workers_count;
        int pixel;
        for (int i = 0; i < sharedHeigth; i++) {
            for (int j = 0; j < width; j++) {
                pixel = 255 - maxOrMin(0, 255);
                for (int ii = std::max(i - 1, 0); ii <= std::min(i + 1, sharedHeigth - 1); ii++) {
                    for (int jj = std::max(j - 1, 0); jj <= std::min(j + 1, width - 1); jj++) {
                        pixel = maxOrMin(pixel, image[ii * width + jj]);
                    }
                }
                newImage[i * width + j] = pixel;
            }
        }
        MPI_Send(newImage, nSharedPixels, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
        delete[] image;
        delete[] newImage;
    }
}

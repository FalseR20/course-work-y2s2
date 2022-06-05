#ifndef COURSE_WORK_Y2S2_IMAGEMPI_H
#define COURSE_WORK_Y2S2_IMAGEMPI_H

#include <mpi.h>

class ImageMPI {
public:
    explicit ImageMPI(const char *filename);

    ~ImageMPI();

    void load(const char *filename);

    void save(const char *filename, int quality = 100);

    void average();

    void increaseDecrease();

private:
    void increaseDecreaseFunc(const unsigned char &maxOrMin(const unsigned char &, const unsigned char &));

    int rank, size, workers_count;
    bool isLead;
    MPI_Status status;

    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;
    unsigned int nPixels;
};


#endif //COURSE_WORK_Y2S2_IMAGEMPI_H

#ifndef COURSE_WORK_Y2S2_IMAGEMPI_H
#define COURSE_WORK_Y2S2_IMAGEMPI_H

#include <mpi.h>

class Image {
public:
    explicit Image(const char *filename);

    ~Image();

    void load(const char *filename);

    void save(const char *filename, int quality = 100);

    void average();

    int getRank();

    int getSize();

//    bool debug;
private:
    int rank, size, workers_count;
    bool isLead;
    MPI_Status status;

    char threadName[11];
    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;
    unsigned int nPixels;
};


#endif //COURSE_WORK_Y2S2_IMAGEMPI_H

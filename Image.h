#ifndef COURSE_WORK_Y2S2_IMAGE_H
#define COURSE_WORK_Y2S2_IMAGE_H


class Image {
public:
    explicit Image(const char *filename, int argc, char **argv);

    void save(const char *filename, int quality = 100);

    void average();

    void average_MPI();

//    bool debug;
private:
    int argc;
    char **argv;

    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;
    unsigned int nPixels;
};


#endif //COURSE_WORK_Y2S2_IMAGE_H

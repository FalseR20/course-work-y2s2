#ifndef COURSE_WORK_Y2S2_IMAGE_H
#define COURSE_WORK_Y2S2_IMAGE_H


class Image {
public:
    explicit Image(const char *filename);

    void average();

    void save(const char *filename, int quality = 100);

    bool debug;
private:
    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;
    unsigned int nPixels;

};


#endif //COURSE_WORK_Y2S2_IMAGE_H

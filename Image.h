#ifndef COURSE_WORK_Y2S2_IMAGE_H
#define COURSE_WORK_Y2S2_IMAGE_H

class Image {
public:
    Image(const char *filename);

    ~Image();

    void load(const char *filename);

    void save(const char *filename, int quality = 100);

    void average();

    void increase_decrease();

private:
    void increase();

    void decrease();

    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;
    unsigned int nPixels;
};


#endif //COURSE_WORK_Y2S2_IMAGE_H

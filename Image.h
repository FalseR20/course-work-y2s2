#ifndef COURSE_WORK_Y2S2_IMAGE_H
#define COURSE_WORK_Y2S2_IMAGE_H

class Image {
public:
    Image(const char *filename);

    ~Image();

    void load(const char *filename);

    void save(const char *filename, int quality = 100);

    void average();

    void increaseDecrease();

private:
    void increaseDecreaseFunc(const unsigned char &maxOrMin(const unsigned char &, const unsigned char &));

    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;
    unsigned int nPixels;
};


#endif //COURSE_WORK_Y2S2_IMAGE_H

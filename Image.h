#ifndef COURSE_WORK_Y2S2_IMAGE_H
#define COURSE_WORK_Y2S2_IMAGE_H


class Image {
public:
    explicit Image(const char *filename);

    void roberts();

    void save(const char *filename, int quality = 100);

//    bool debug = false;
private:
    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    unsigned char *image;

};


#endif //COURSE_WORK_Y2S2_IMAGE_H

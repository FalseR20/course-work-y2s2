#include <iostream>
#include "turbojpeg.h"

const char* FILENAME = "..\\jpg\\image3.jpg";
const char* FILENAME_NEW = "..\\jpg\\new_image3.jpg";


unsigned char* get_image_solution(int &height, int &width, unsigned long& jpegSize, unsigned char* jpegBuf){
    FILE *file = fopen(FILENAME, "rb");
    int jpegSubsamp = TJSAMP_GRAY; //

    fseek(file, 0, SEEK_END); // set pointer to the end of file for getting file size
    jpegSize = ftell(file); // get file size
    fseek(file, 0, SEEK_SET);
    jpegBuf = new unsigned char[jpegSize];
    fread( jpegBuf, jpegSize, 1, file ); //get a pointer to a buffer containing JPEG image in jpegBuf

    tjhandle handle = tjInitDecompress();
    tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    fclose(file);
    return jpegBuf;
}

unsigned char* get_grayscale(int height, int width, unsigned long jpegSize,unsigned char* jpegBuf, unsigned char* image){
    image = new unsigned char[width * height];
    tjhandle handle = tjInitDecompress();
    int pixelFormat = TJPF_GRAY;
    int nbands = 1;
    int flags = 0;
    int pitch = width * nbands;
    tjDecompress2(handle, jpegBuf, jpegSize, image, width, pitch, height, pixelFormat, flags);
    return image;
}

unsigned char* robberts(int height, int width, unsigned char* image){

    unsigned char* new_image = new unsigned char[width * height];
    for (int i = 0; i < height-1; i++) {
        for (int j = 0; j < width; j++) {
            int new_pixel = abs((image[i*width+j]-image[(i+1)*width+(j+1)]))+
                            abs((image[i*width+(j+1)]-image[(i+1)*width+j]));
            new_image[i * width + j] = new_pixel;
        }
    }
    return new_image;
}
int main(){
    int height, width;
    unsigned long jpegSize;
    unsigned char *jpegBuf;
    jpegBuf = get_image_solution(height, width, jpegSize, jpegBuf);//getting file
    unsigned char *image;
    image = get_grayscale(height, width, jpegSize, jpegBuf, image);




    tjhandle handle_2 = tjInitCompress();

    unsigned char * jpegBuf_2 = NULL;
    unsigned long jpegSize_2 = 0;


    tjCompress2(handle_2, robberts(height,width,image), width, width, height, TJPF_GRAY, &(jpegBuf_2), &jpegSize_2,
                TJSAMP_GRAY , 91, 0);
    FILE *file_2 = fopen(FILENAME_NEW, "wb");

    fwrite(jpegBuf_2, jpegSize_2, 1, file_2);




}
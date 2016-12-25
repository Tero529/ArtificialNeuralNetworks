/*! \file */
#ifndef FACE_IMAGE_H
#define FACE_IMAGE_H

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<list>
#include<vector>

using namespace std;

/*! \brief Representation of an Image
*
*   This structure is used to
*   represent the contents of an image
*   that is to be trained on or tested.
*/
typedef struct{
    float **data; //!< A Matrix of size nRows x nCols to store pixel intensities
    int nRows;    //!< The image height in pixels
    int nCols;    //!< The image width in pixels
    int maxGray;  //!< The maxmimum intensity value of the image, from 0 to maxGray
}IMAGE;


int allocateExamples(list<IMAGE *> *trainingExamples,list<vector<float> > *labels, int labelIndex,char *imageList);

IMAGE *readFile(char *filename);

float **allocateData(int row,int col);

IMAGE *imageAllocate();

int givePoseLabel(char label[]);

int giveFaceLabel(char token[]);

int giveSunglassLabel(char label[]);

//! Possible Labels for the Pose classifier
enum POSE{
    STRAIGHT,
    LEFT,
    RIGHT,
    UP};

//! Possible Labels for the Face classifier
enum FACE{
    AN2I,
    AT33,
    BOLAND,
    BPM,
    CH4F,
    CHEYER,
    CHOON,
    DANIELN,
    GLICKMAN,
    KARYADI,
    KAWAMURA,
    KK49,
    MEGAK,
    MITCHELL,
    NIGHT,
    PHOEBE,
    SAAVIK,
    STEFFI,
    SZ24,
    TAMMO};

//! Possible Labels for the Sunglass classifier
enum SUNGLASS{
    OPEN,
    SUNGLASSES};
#endif

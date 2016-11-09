#ifndef FACE_IMAGE_H
#define FACE_IMAGE_H

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<list>
#include<vector>

using namespace std;
typedef struct{
    float **data;
    int nRows;
    int nCols;
    int maxGray;
}IMAGE;

float **allocateData(int row,int col);

IMAGE *imageAllocate();

IMAGE *readFile(char *filename);

int allocateExamples(list<IMAGE *> *trainingExamples,list<vector<float> > *labels, int labelIndex,char *imageList);

int givePoseLabel(char label[]);

int giveFaceLabel(char token[]);

int giveSunglassLabel(char label[]);


enum POSE{
    STRAIGHT,
    LEFT,
    RIGHT,
    UP};

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

enum SUNGLASS{
    OPEN,
    SUNGLASSES};
#endif
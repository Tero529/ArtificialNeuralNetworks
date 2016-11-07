#ifndef NETWORK_HPP
#define NETWORK_HPP

#include<iostream>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<list>
#include<vector>
#include "../ImageParse/FaceImage.hpp"

using namespace std;
typedef struct{
    vector<float> *weights;
    vector<float> *inputs;
    float error;
    float output;
    int layerIndex;//Index Within the Layer;
}node;

node *allocateNode(int index,int inputs);

float sigmoid(float input);

void generateOutput(node *current,list<int> inputs);

void feedForward( list<list <node *> > *NetworkNodes,list<int> inp);

void generateError(node *current,list<node*> downstream);

void generateError(node *current,float label);

void CreateNetwork(list<list <node *> > *NetworkNodes,list<int> layerNodes,int inputs);

list<int> BackpropogationDriver(list<IMAGE *> *trainingExamples,list<int> *labels,int inputs,int iterations,list<int> layerNodes,int NumOfExamples,float LearningRate);





#endif
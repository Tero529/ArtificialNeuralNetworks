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

list<list<node *> > BackpropogationDriver(list<IMAGE *> *trainingExamples,list<vector<float> > *labels,int inputs,int iterations,list<int> layerNodes,float LearningRate,int outputs);

float predict(list<list<node *> > LearnedNetwork, list<IMAGE *> instances, list<vector<float> > labels,int outputs);


void feedForward( list<list <node *> > *NetworkNodes,list<float> inp);
void generateOutput(node *current,list<int> inputs);

void backProp(list<list <node *> > *NetworkNodes, vector<float> label);
void generateError(node *current,list<node*> downstream);
void generateError(node *current,float label);

void update(list<list<node *> > *NetworkNodes,float LearningRate);


node *allocateNode(int index,int inputs);

float sigmoid(float input);

void CreateNetwork(list<list <node *> > *NetworkNodes,list<int> layerNodes,int inputs,int outputs);


int max(vector<float> outs);



#endif
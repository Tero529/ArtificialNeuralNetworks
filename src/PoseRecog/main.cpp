#include "../Network/network.hpp"
#include "../ImageParse/FaceImage.hpp"

//Straight is 0
//Left is 1
//Right is 2
//Up is 3

#define INPUTS 961


int main(int argc,char *argv[]){
    
    list<IMAGE *> trainingExamples;
    list<IMAGE *> test1Examples;
    list<IMAGE *> test2Examples;

    
    
    list<vector<float> > trainLabels,test1Labels,test2Labels;
    
    list<list<node*> > LearnedNetwork;
    
    list<int> hiddenNodes; // From innermost to outermost
    
    hiddenNodes.push_back(8);
    

    char ListFile[]= "../../Data/PoseData/all_train.listActual.txt";
    char TestFile1[]="../../Data/PoseData/all_test1.listActual.txt";
    char TestFile2[]="../../Data/PoseData/all_test1.listActual.txt";

    int NumOfExamples=allocateExamples(&trainingExamples,&trainLabels,1,ListFile);
    
    LearnedNetwork=BackpropogationDriver(&trainingExamples,&trainLabels,INPUTS,100,hiddenNodes,NumOfExamples,0.3f,4);
   
    
    allocateExamples(&test1Examples,&test1Labels,1,TestFile1);
    
    cout<< "Accuracy First Set "<<predict(LearnedNetwork,test1Examples,test1Labels)<<endl;
    
    allocateExamples(&test2Examples,&test2Labels,1,TestFile2);
    
    cout<< "Accuracy Second Set "<<predict(LearnedNetwork,test2Examples,test2Labels)<<endl;

    
    return 0;
}

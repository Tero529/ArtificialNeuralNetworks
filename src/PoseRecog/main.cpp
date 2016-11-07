#include "../Network/network.hpp"
#include "../ImageParse/FaceImage.hpp"

//Straight is 0
//Left is 1
//Right is 2
//Up is 3


int main(int argc,char *argv[]){
    
    list<IMAGE *> trainingExamples;
    
    list<int> labels;
    
    list<int> predictions;
    
    list<int> hiddenNodes; // From innermost to outermost
    
    hiddenNodes.push_back(6);
    

    char ListFile[]= "../../Data/PoseData/all_train.listActual.txt";
    int NumOfExamples=allocateExamples(&trainingExamples,&labels,1,ListFile);
    
    predictions=BackpropogationDriver(&trainingExamples,&labels,32*30,50,hiddenNodes,NumOfExamples,0.3f);
    
    
    return 0;
}
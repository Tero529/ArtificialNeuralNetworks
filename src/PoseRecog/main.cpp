#include "../Network/network.hpp"
#include "../ImageParse/FaceImage.hpp"

//Straight is 0
//Left is 1
//Right is 2
//Up is 3

#define INPUTS 961


int main(int argc,char *argv[]){

    char ListFile[]= "../../Data/PoseData/all_train.listActual.txt";
    char TestFile1[]="../../Data/PoseData/all_test1.listActual.txt";
    char TestFile2[]="../../Data/PoseData/all_test2.listActual.txt";

    list<IMAGE *> trainingExamples;
    list<IMAGE *> test1Examples;
    list<IMAGE *> test2Examples;
    
    list<vector<float> > trainLabels,test1Labels,test2Labels;
    
    list<list<node*> > LearnedNetwork;
    
    list<int> hiddenNodes; // From innermost to outermost
    

    
    allocateExamples(&trainingExamples,&trainLabels,1,ListFile);
    //allocateExamples(&test1Examples,&test1Labels,1,TestFile1);
    //allocateExamples(&test2Examples,&test2Labels,1,TestFile2);
    
   // allocateExamples(&trainingExamples,&trainLabels,3,ListFile);
    // allocateExamples(&test1Examples,&test1Labels,3,TestFile1);
    //allocateExamples(&test2Examples,&test2Labels,3,TestFile2);
    
    float maxL1,maxL2,maxLAv;
    int maxN1,maxN2,maxNAv;
    int maxI1,maxI2,maxIAv;
    float max1=0.0f,max2=0.0f,maxAv=0.0f;
    float ac1,ac2,acAv;
    int iterations=200;
    int outputs=4;
    int nodes=7;
    
    
    for(nodes=14;nodes<=21;nodes+=7){
        
        hiddenNodes.clear();
        hiddenNodes.push_back(nodes);
        
        for(float LearningRate = 0.4f ; LearningRate <= 0.8f ; LearningRate = LearningRate + 0.15f){
            
            for(iterations=100;iterations<=300;iterations+=200){
                
                LearnedNetwork.clear();
                
                LearnedNetwork=BackpropogationDriver(&trainingExamples,&trainLabels,INPUTS,iterations,hiddenNodes,LearningRate,outputs);
                
                cout<<"\n 1 Layer, each layer with Nodes "<<nodes<<" Learning Rate "<<LearningRate<<" Iterations "<<iterations<<endl;
                
                ac1=predict(LearnedNetwork,trainingExamples,trainLabels,outputs);
                cout<< "Accuracy Training Set "<< ac1<<endl;
                
                // ac2=predict(LearnedNetwork,test2Examples,test2Labels,outputs);
                //cout<< "Accuracy Second Set "<<ac2<<endl;
                
                //  acAv=(ac1 +ac2)/2;
                
                //  cout<< "Accuracy Average Set "<<acAv<<endl;
                
                if(ac1>max1){
                    max1=ac1;
                    maxL1=LearningRate;
                    maxI1=iterations;
                    maxN1=nodes;
                    
                }
                /*if(ac2>max2){
                 max2=ac2;
                 maxL2=LearningRate;
                 maxI2=iterations;
                 maxN2=nodes;
                 
                 }
                 if(acAv>maxAv){
                 maxAv=acAv;
                 maxLAv=LearningRate;
                 maxIAv=iterations;
                 maxNAv=nodes;
                 }*/
                
                
            }
        }
        
    }
    
    cout<<"Maximum Accuracy on Traning Set is "<<max1<<" with Nodes "<<maxN1<<" Learning Rate "<<maxL1<<" Iterations "<<maxI1<<endl;
    //  cout<<"Maximum Accuracy on Set 2 is "<<max2<<" with Nodes "<<maxN2<<" Learning Rate "<<maxL2<<" Iterations "<<maxI2<<endl;
    //cout<<"Maximum Average Accuracy is "<<maxAv<<" with Nodes "<<maxNAv<<" Learning Rate "<<maxLAv<<" Iterations "<<maxIAv<<endl;
    return 0;
}

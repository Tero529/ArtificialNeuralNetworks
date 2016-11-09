#include "../Network/network.hpp"
#include "../ImageParse/FaceImage.hpp"

#define INPUTS 961


int main(int argc,char *argv[]){
    
    char ListFile[]= "../../Data/FaceData/straighteven_train.listActual.txt";
    char TestFile1[]="../../Data/FaceData/straighteven_test1.listActual.txt";
    char TestFile2[]="../../Data/FaceData/straighteven_test2.listActual.txt";
    
    list<IMAGE *> trainingExamples;
    list<IMAGE *> test1Examples;
    list<IMAGE *> test2Examples;
    
    list<vector<float> > trainLabels,test1Labels,test2Labels;
    
    list<list<node*> > LearnedNetwork;
    
    list<int> hiddenNodes; // From innermost to outermost
    
    hiddenNodes.push_back(20);
    
    
    
    allocateExamples(&trainingExamples,&trainLabels,0,ListFile);

    allocateExamples(&test1Examples,&test1Labels,0,TestFile1);
    allocateExamples(&test2Examples,&test2Labels,0,TestFile2);
    
    float maxL1,maxL2,maxLAv;
    float max1=0.0f,max2=0.0f,maxAv=0.0f;
    float ac1,ac2,acAv;
    int outputs=20;
    // for(float LearningRate = 0.1f ; LearningRate < 0.8f ; LearningRate = LearningRate + 0.1f){
    
    LearnedNetwork=BackpropogationDriver(&trainingExamples,&trainLabels,INPUTS,150,hiddenNodes,0.3f,outputs);
    
    //cout<<"\nLearning Rate "<<LearningRate<<endl;
    
    ac1=predict(LearnedNetwork,test1Examples,test1Labels,outputs);
    cout<< "Accuracy First Set "<< ac1<<endl;
    
    ac2=predict(LearnedNetwork,test2Examples,test2Labels,outputs);
    cout<< "Accuracy Second Set "<<ac2<<endl;
    
    acAv=(ac1 +ac2)/2;
    
    cout<< "Accuracy Average Set "<<acAv<<endl;
    
    /*
     if(ac1>max1){
     max1=ac1;
     maxL1=LearningRate;
     }
     if(ac2>max2){
     max2=ac2;
     maxL2=LearningRate;
     }
     if(acAv>maxAv){
     maxAv=acAv;
     maxLAv=LearningRate;
     }
     
     }
     
     cout<<"Maximum Accuracy on Set 1 is "<<max1<<" with Learning Rate "<<maxL1<<endl;
     cout<<"Maximum Accuracy on Set 2 is "<<max2<<" with Learning Rate "<<maxL2<<endl;
     cout<<"Maximum Average Accuracy is "<<maxAv<<" with Learning Rate "<<maxLAv<<endl;
     
     */
    return 0;
}

#include "network.hpp"
float sigmoid(float x)
{
    float exp_value;
    float return_value;
    
    exp_value = exp((double) -1*x);
    
    return_value = 1 / (1 + exp_value);
    return return_value;
}


void generateOutput(node *current,list<float> inputs){
    float net=0;
    int index=0;
    float wt;
    current->inputs->clear();
    
    for(float input:inputs){
        //mcout<<input<<" ";
        wt=(current->weights)->at(index++);
        //cout<<wt<<" "<<input<<endl;
        net= net + (wt)*(input);
        (current->inputs)->push_back(input);
    }
    //cout<<endl<<endl<<endl;
    current->output=sigmoid(net);
   
}


void generateError(node *current,float label){
    float out=current->output;
    /*if((current->layerIndex)==3)
        cout<<current->output<<" "<<label<<endl;
    else
        cout<<"\nLayer Index: "<<current->layerIndex<<endl;*/
    current->error = (label-out)*(1-out)*out;
   // cout<<label<<" "<<out<<" "<<current->error<<endl;

}

void generateError(node *current, list<node*> downstream){
    int index=current->layerIndex;
    float out=current->output;
    float multiplier=0;
    //cout<<"Current index "<<current->layerIndex<<endl;

    for(node* ds:downstream)
        multiplier= multiplier + (ds->error) * ((ds->weights)->at(index));
    
    current->error=out * (1-out) *multiplier;
    
}
node *allocateNode(int index,int inputs){
    node *temp;
    float randomWeight;

    temp=(node*)malloc(sizeof(node));
    temp->error=0.0f;
    temp->output=0.0f;
    temp->layerIndex=index;
    temp->weights= new vector<float>;
    temp->inputs=new vector<float>;
    for(int i=0;i<inputs;i++){
        randomWeight= -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.0)));
        (temp->weights)->push_back(randomWeight);
        }
    return temp;
}

void CreateNetwork(list<list <node *> > *NetworkNodes,list<int> layerNodes,int inputs,int outputs){
    int counter=0,prev=inputs;
    
    list<node *> temp;
    
    for(int layer:layerNodes){
        
        temp.clear();
        
        for(int i=0;i<layer;i++)
            temp.push_back(allocateNode( i,prev));
        (NetworkNodes)->push_back(temp);
        counter++;
        prev=layer+1;
    }
    temp.clear();
    for(int i=0;i<outputs;i++)
        temp.push_back(allocateNode(i,prev));
    
    NetworkNodes->push_back(temp);
}

void feedForward( list<list <node *> > *NetworkNodes,list<float> inp){
    list<float> inp2;
    
    
    int counter=0;
    int number;
    for(list<node*> layer: *NetworkNodes){
        inp2.clear();
        inp2.push_back(1);
        number=0;
        for(node *NODE : layer){
            generateOutput(NODE,inp);
            inp2.push_back(NODE->output);
        }
        inp=inp2;
    }

}

void backProp(list<list <node *> > *NetworkNodes, vector<float> label){
    
    int counter=0;
    int labelIndex=0;
    
    list<node *> downstream,dstemp,layer;
    
    for (list<list<node *> >::reverse_iterator i = NetworkNodes->rbegin(); i != NetworkNodes->rend(); ++i){
        
        layer= *(i);
        downstream=dstemp;
        dstemp.clear();
        //cin>>counter;

//        cout<<"\nSize: "<<layer.size()<<endl;
        for(node *NODE: layer){
            dstemp.push_back(NODE);
            
            if(i==NetworkNodes->rbegin()){
                //cout<<"Label: "<<label.at(labelIndex);
                generateError(NODE,label.at(labelIndex++));
               //cout<<"\nOutput Error "<< NODE->error<<"\n";
            }
            else{
                generateError(NODE,downstream);
               // cout<<"\nHidden Error "<< NODE->error<<" "<<downstream.size();
            }
          //  cout<<NODE->error<<endl;
        }
    }
}

void update(list<list<node *> > *NetworkNodes,float LearningRate){
    int in;
    for(list<node*> layer: *NetworkNodes){
        for(node *NODE : layer){
            int limit=(NODE->inputs)->size();
            for(int i=0;i<limit;i++){
                //cin>>in;
                //cout<<"index "<<i<<endl;
                float save= NODE->weights->at(i);
                (*(NODE->weights))[i]=save +(NODE->error * (NODE->inputs)->at(i) *LearningRate);
                //cout<<" "<<save-NODE->weights->at(i)<<endl;

            }
        }
    }

}


int max(vector<float> outs){
    float max=0;
    int ret=-1;
    for(int i=0;i<outs.size();i++){
        if(outs[i] > max){
            ret=i;
            max=outs[i];
        }
    }
    return ret;
}

float predict(list<list<node *> > LearnedNetwork,list<IMAGE *> instances, list<vector<float> > labels,int outputs){
    
    float correct=0.0f,wrong=0.0f,accuracy;
    float total=0.0f;

    list<vector<float> >::iterator it=labels.begin();
    list<float> inp;
    vector<float> outs;
    
    int learn,lab;
    
    for(IMAGE *current : instances){
        
        inp.clear();
        inp.push_back(1);
        outs.clear();
        
        for(int i=0;i<30;i++){
            for(int j=0;j<32;j++){
                inp.push_back(current->data[i][j]);
            }
        }
        
        feedForward(&LearnedNetwork,inp);
        
        for(node *N : LearnedNetwork.back())
            outs.push_back(N->output);
        
        learn=max(outs);
        
        for(int i=0;i<outputs;i++){
            if((*it).at(i) == 0.9f)
                lab=i;
        }
        total=total+1.0f;
        if(learn == lab)
            correct=correct+1.0f;
        else
            wrong=wrong+1.0f;
        
        it++;
    }
    return (correct)/(total);
    

    
}


list< list<node *> >  BackpropogationDriver(list<IMAGE *> *trainingExamples,list<vector<float> > *labels,int inputs,int iterations,list<int> layerNodes,float LearningRate,int outputs){
    
    list< list<node *> > NetworkNodes;
    IMAGE *current;
    CreateNetwork(&NetworkNodes,layerNodes,inputs,outputs);
    list<float> inp;
    list<vector<float> >::iterator it;
    //cout<<"Begun ...\n";

    for( int iter=0;iter<iterations;iter++){
        cout<<"Iteration "<<iter<<endl;
        it=labels->begin();
        
        for(IMAGE *current : *trainingExamples){
            inp.clear();
            inp.push_back(1);
            for(int i=0;i<30;i++){
                for(int j=0;j<32;j++){
                    inp.push_back(current->data[i][j]);
                }
            }
            feedForward(&NetworkNodes,inp);
            
            backProp(&NetworkNodes,*it);
            
            update(&NetworkNodes,LearningRate);

            it++;
        }
        
    }
    //cout<<"Complete\n";

    return NetworkNodes;
}
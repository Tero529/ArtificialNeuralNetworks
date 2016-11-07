#include "network.hpp"
float sigmoid(float x)
{
    float exp_value;
    float return_value;
    
    exp_value = exp((double) -x);
    
    return_value = 1 / (1 + exp_value);
    
    return return_value;
}

void generateOutput(node *current,list<node*> inputs){
    float net=0;
    int index=0;
    float wt;
    (current->inputs)->clear();
    for(node *input:inputs){
        wt=(current->weights)->at(index++);
        net= net + (wt)*(input->output);
        (current->inputs)->push_back(input->output);
    }
    current->output=sigmoid(net);
}

void generateOutput(node *current,list<int> inputs){
    float net=0;
    int index=0;
    float wt;
    current->inputs->clear();
    for(int input:inputs){
        wt=(current->weights)->at(index++);
        //cout<<wt<<" "<<input<<endl;
        net= net + (wt)*((float)input);
        (current->inputs)->push_back((float)input);
       // cout<<net<< " ";
    }
    current->output=sigmoid(net);
    //cout<<sigmoid(net)<<endl;
}


void generateError(node *current,float label){
    float out=current->output;
    current->error = (label-out)*(1-out)*out;
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

void CreateNetwork(list<list <node *> > *NetworkNodes,list<int> layerNodes,int inputs){
    int counter=0,prev=inputs;
    list<node *> temp;
    for(int layer:layerNodes){
        temp.clear();
        for(int i=0;i<layer;i++)
            temp.push_back(allocateNode( i,prev));
        (NetworkNodes)->push_back(temp);
        counter++;
        prev=layer;
    }
    temp.clear();
    temp.push_back(allocateNode(0,prev));
    NetworkNodes->push_back(temp);
    temp.clear();
}

void feedForward( list<list <node *> > *NetworkNodes,list<int> inp){
    list<int> inp2;
    
    
    int counter=0;
    int number;
    for(list<node*> layer: *NetworkNodes){
        //cout<<"\nLayer "<<counter++;
        inp2.clear();
        number=0;
        for(node *NODE : layer){
         //   cout<<"\n\tNode "<<number++;
            generateOutput(NODE,inp);
            inp2.push_back(NODE->output);
        }
        inp.clear();
        inp=inp2;
    }

}

void backProp(list<list <node *> > *NetworkNodes, int label){
    
    int counter=0;
    list<node *> downstream;
    for (list<list<node *> >::reverse_iterator i = NetworkNodes->rbegin(); i != NetworkNodes->rend(); ++i){
        list<node *> layer= *(i);
        for(node *NODE: layer){
            if(counter==0){
                counter++;
                generateError(NODE,label);
            }
            else{
                generateError(NODE,downstream);
            }
            downstream.clear();
            downstream=layer;
          //  cout<<NODE->error<<endl;
            
        }
    }
}

void update(list<list<node *> > *NetworkNodes,float LearningRate){

    for(list<node*> layer: *NetworkNodes){
        for(node *NODE : layer){
            int limit=(NODE->inputs)->size();
            for(int i=0;i<limit;i++)
                (NODE->weights)->at(i)=((NODE->weights)->at(i)) +(NODE->error * (NODE->inputs)->at(i) *LearningRate);
        }
    }

}

list<int> BackpropogationDriver(list<IMAGE *> *trainingExamples,list<int> *labels,int inputs,int iterations,list<int> layerNodes,int NumOfExamples,float LearningRate){
    
    list< list<node *> > NetworkNodes;
    IMAGE *current;
    CreateNetwork(&NetworkNodes,layerNodes,inputs);
    list<int> predictions;
    
    for( int iter=0;iter<iterations;iter++){
        predictions.clear();
        list<int>::iterator it=labels->begin();
        cout<<"Iteration "<<iter<<endl;
        for(IMAGE *current : *trainingExamples){
            
            list<int> inp(current->data[0],(current->data[0])+inputs);
            feedForward(&NetworkNodes,inp);
            
            backProp(&NetworkNodes,*it);
            
            update(&NetworkNodes,LearningRate);

            it++;
        }
        
    }
    return predictions;
}
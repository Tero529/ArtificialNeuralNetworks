#include "network.hpp"



/*! \brief Driver Function for learning the Neural Network
*
*   Performs the backpropogation algorithm for a given number of iterations to learn a Neural network of given specifications over
*   given training data.
*   \param trainingExamples The list of IMAGE training examples which are to be learned from
*   \param labels The labels corresponding to the IMAGE training examples in the same order.
*   \param inputs The Number of Inputs to the network
*   \param iterations The iterations of the backpropogation algorithm to stop after
*   \param layerNodes A list containing the number of nodes in each hidden layer from innermost to outermost. \n The last entry of the list corresponds to the layer right before the output layer
*   \param LearningRate The learning Rate for updation
    \param outputs The number of output nodes.
*   \return A list of layers of the learned network from innermost to outermost, the end of the list specifies the output layer. \n
*           Each layer consists of a list of network nodes in the order of their layer Index.
*/
list< list<node *> >  BackpropogationDriver(list<IMAGE *> *trainingExamples,list<vector<float> > *labels,int inputs,int iterations,list<int> layerNodes,float LearningRate,int outputs){

    list< list<node *> > NetworkNodes;
    IMAGE *current;
    CreateNetwork(&NetworkNodes,layerNodes,inputs,outputs);// Create Network with random weights
    list<float> inp;
    list<vector<float> >::iterator it; // Iterator for output label vectors

    for( int iter=0;iter<iterations;iter++){
      //  cout<<"Iteration "<<iter<<endl;
        
        it=labels->begin();

        for(IMAGE *current : *trainingExamples){// Iterate through training examples one by one.
            inp.clear();// Refresh Inputs for new training example.
            
            inp.push_back(1);// Add first 1 input which exists for ever node.
            
            for(int i=0;i<30;i++){
                for(int j=0;j<32;j++){
                    inp.push_back(current->data[i][j]);// Set inputs
                }
            }
            feedForward(&NetworkNodes,inp);// Genearte node outputs

            backProp(&NetworkNodes,*it); // Back propograte Node errors
 
            update(&NetworkNodes,LearningRate); // Update weights according to gradient descent rule with required learning rate

            it++;
        }

    }
    return NetworkNodes;
}

/*! \brief Creates an Empty Network of nodes with given specifications
*   \param NetworkNodes The network that is to be created. Represented as a list of layers, each of which is a list of nodes.
*   \param layerNodes A list of integers with each value corresponding to the number of nodes in that layer. From innermost to outermost
*   \param inputs The number of inputs to the Neural Network
*   \param outputs The number of output nodes.
*/
void CreateNetwork(list<list <node *> > *NetworkNodes,list<int> layerNodes,int inputs,int outputs){
    int prev=inputs; // the number of inputs to the current layer

    list<node *> temp; // Empty Network layer to be put at te end of network list.

    for(int layer:layerNodes){ //Get number of nodes in each layer from layerNodes list

        temp.clear();

        for(int i=0;i<layer;i++)
            temp.push_back(allocateNode( i,prev)); // Create specified number random weighted nodes and add them to current layer.
        
        (NetworkNodes)->push_back(temp); //Add constructed layer to the network
        
        prev=layer+1; // Update Number of inputs for the next layer
    }
    
    temp.clear();
    //Add specified number of output nodes to the network
    for(int i=0;i<outputs;i++)
        temp.push_back(allocateNode(i,prev));

    NetworkNodes->push_back(temp);
}

/*! \brief Returns the sigmoid of the given float value */
float sigmoid(float x)
{
    float exp_value;
    float return_value;

    exp_value = exp((double) -1*x);

    return_value = 1 / (1 + exp_value);
    return return_value;
}



/*! \brief Sets the error of an output node
*   \param current The node whose error is to be calculated
*   \param label The output of the node that is to be learned
*/
void generateError(node *current,float label){
    float out=current->output;
    current->error = (label-out)*(1-out)*out; // Generate error using backpropogation update rule for an output node
}

/*! \brief Sets the error of a non output network node
*   \param current The ndoe whose error is to be calculated
*   \param downstream The list of downstream nodes, whose errors are need
*/
void generateError(node *current, list<node*> downstream){
    int index=current->layerIndex;
    float out=current->output;
    float multiplier=0;

    for(node* ds:downstream)
        multiplier= multiplier + (ds->error) * ((ds->weights)->at(index)); // Sum up weighted errors of all downstream nodes.

    current->error=out * (1-out) *multiplier;
}

/*! \brief Sets the output of a network node
* \param current The node whose output is to be determined
* \param inputs A list of inputs ot the network node
*/
void generateOutput(node *current,list<float> inputs){
    float net=0;
    int index=0;
    float wt;
    current->inputs->clear();

    // To obtain netj
    for(float input:inputs){
        wt=(current->weights)->at(index++); //Iterate through the weight vector
        net= net + (wt)*(input);
        (current->inputs)->push_back(input);
    }
    
    current->output=sigmoid(net);

}


/*! \brief Intiliases a network node with random weights
*   \param index The index of the ndoe within the layer
*   \param inputs The number of inputs to the node (required to initialise that many weights)
*   \return node An empty node with random weights between -0.5 and 0.5
*/
node *allocateNode(int index,int inputs){
    node *temp;
    float randomWeight;

    temp=(node*)malloc(sizeof(node));
    temp->error=0.0f;
    temp->output=0.0f;
    temp->layerIndex=index;
    temp->weights= new vector<float>;
    temp->inputs=new vector<float>;
    // Generate and assign random weight value between -0.5 and 0.5 to the node.
    for(int i=0;i<inputs;i++){
        randomWeight= -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.0)));
        (temp->weights)->push_back(randomWeight); // Add weight to the end of the list of node weights
        }
    return temp;
}

/*! \brief Feeds an instance through an arbitrary network to generate an output
*   \param NetworkNodes The network to be used to generate the output for the training instance
*   \param inp A list of inputs to the Neural Network Nodes
*/
void feedForward( list<list <node *> > *NetworkNodes,list<float> inp){
    list<float> inp2; // To store the inputs to a given node in the next layer..


    int counter=0;
    int number;
    for(list<node*> layer: *NetworkNodes){
        inp2.clear();
        inp2.push_back(1);
        number=0;
        for(node *NODE : layer){
            generateOutput(NODE,inp);
            inp2.push_back(NODE->output);// Outputs will act as inputs to the next layer.
        }
        inp=inp2;
    }

}

/*! \brief Propogates the errors backwards through an arbitrary given Network
*   \param NetworkNodes The Network whose errors are to be set and back propogated
*   \param label A vector of the labels of the traiing example which has been fed forward in the current iteration
*/
void backProp(list<list <node *> > *NetworkNodes, vector<float> label){

    int labelIndex=0;
    list<node *> downstream, //Downstream nodes.
                dstemp, //Downstream nodes for next layer
                layer; // The current layer being operated on

    for (list<list<node *> >::reverse_iterator i = NetworkNodes->rbegin(); i != NetworkNodes->rend(); ++i){

        layer= *(i);
        downstream=dstemp;
        dstemp.clear();

        for(node *NODE: layer){
            dstemp.push_back(NODE);// Current node will be a part of the next layer's downstream

            if(i==NetworkNodes->rbegin()){
                generateError(NODE,label.at(labelIndex++));
            }
            else{
                generateError(NODE,downstream);
            }
        }
    }
}

/*! \brief Updates the Network according to the gradient descent rule
*
*   Using the outputs generated in the feed forwards stage, and the errors generated in the back propogation stage.
*   This function augments it with the learning rate to update the weights of each node in the Network in the falling direction
*   of the gradient
*   \param NetworkNodes The Network who's weights are to be updated
*   \param LearningRate The learning rate to be applied to the Network
*/
void update(list<list<node *> > *NetworkNodes,float LearningRate){
    int in;
    for(list<node*> layer: *NetworkNodes){
        for(node *NODE : layer){
            int limit=(NODE->inputs)->size();
            for(int i=0;i<limit;i++){
                float save= NODE->weights->at(i);
                (*(NODE->weights))[i]=save +(NODE->error * (NODE->inputs)->at(i) *LearningRate); // Use update rule for back propogation algorthm to update node
            }
        }
    }
}

/*! \brief Gives the classified integer from vector outputs
*  \param outs The output vector generated by the Network
*  \return The integer specifying the classification given by te Network
*/
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

/*! \brief Gives the prediction accuracy of a Network on a Test Set
*
* Using a learned network, this function applies it to a test set and returns the
* classification accuracy obtained on the set.
* \param LearnedNetwork The Network that is to be tested
* \param instances The list of image instances that make up the test set
* \param labels The labels corresponding the instances of the test set
* \param outputs The Number of outputs nodes in the network
* \return The Classification Accuracy obtained
*/
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
                inp.push_back(current->data[i][j]); //Initiliase Image structure of testing instance
            }
        }

        feedForward(&LearnedNetwork,inp);// use learned network to generate outptu for test instance.

        for(node *N : LearnedNetwork.back())
            outs.push_back(N->output); // Create a list consisting of outpus of the final layer in the network

        learn=max(outs); // Get classification as maximum of ouputs.

        for(int i=0;i<outputs;i++){
            if((*it).at(i) == 0.9f)
                lab=i; // The label with 0.9 is the classified output
        }
        total=total+1.0f;
        if(learn == lab)
            correct=correct+1.0f;
        else
            wrong=wrong+1.0f;

        it++;
    }
    return (correct)/(total); // Accuracy is returned



}

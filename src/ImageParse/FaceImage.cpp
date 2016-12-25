#include"FaceImage.hpp"

using namespace std;


/*! \brief Function that returns emtpy allocated space for iamge data
 *
 *   \param row The image height
 *   \param col The image width
 *   \return Float matrix of size row*col with each entry initialised to 0
 */
float **allocateData(int row,int col){
    
    float **returnData;
    returnData=(float **)malloc(row*sizeof(float*)); //Allocate heap Space
    
    if(returnData==NULL){
        perror("Could Not Allocate Memory");
        exit(EXIT_FAILURE);
    }
    
    for(int i=0;i<col;i++){
        returnData[i]=(float *)malloc(sizeof(float)*col);
        if(returnData[i]==NULL){
            perror("Could Not allocate Memory");
            exit(EXIT_FAILURE);
        }
    }
    
    return returnData;
}


/*! \brief Allocates and returns  a new IMAGE structure
 *
 *   Function creates an empty IMAGE structure
 *   with it's space allocated on the heap and returns it's
 *   location
 *   \return IMAGE sturcture with data initialised to NULL and other attributes to 0
 */
IMAGE *imageAllocate(){
    IMAGE *retVal;
    retVal=(IMAGE *)malloc(sizeof(IMAGE)); //Allocate Heap Space
    retVal->data=NULL;
    retVal->nRows=0;
    retVal->nCols=0;
    retVal->maxGray=0;
    return retVal;
}

/*! \brief Parses a .pgm Image File
 *
 *  The function parses a .pgm file to return the pixel intensities and
 *  dimensions of the image as an IMAGE structure
 * \param filename The directory address of the file that is to be parsed
 * \return IMAGE structure containing appropriate pixel intensity matrix and dimension data specifeid by file
 */
IMAGE *readFile(char *filename){
    
    FILE *imageFile;
    char line[512]; //To store line parsed from file
    float pixel;
    int type,rows,cols,grays;
    IMAGE *newImage;

    imageFile=fopen(filename,"r"); //Open Input File in read mode

    if(imageFile==NULL){ //Handle File Errors
        printf("%s\n",filename);
        perror("Could Not Open File");
        exit(EXIT_FAILURE);
    }
    
    /*Reads and scans first line of PGM File to extract type
    * We handle only P5 type PGM files.
    */
    fgets(line,sizeof(line),imageFile);
    sscanf(line,"P%d",&type);

    if(type!=5){
        perror("Incorrect File ");
        fclose(imageFile);
        exit(EXIT_FAILURE);
    }

    newImage=imageAllocate();

    //Scan Next Line for the image dimensions written as "width height"
    fgets(line,sizeof(line),imageFile);
    sscanf(line,"%d %d",&cols,&rows);


    newImage->nRows=rows;
    newImage->nCols=cols;

    //Scan Next Line to obtain maximum pixel intensity
    fgets(line,sizeof(line),imageFile);
    sscanf(line,"%d",&grays);
    
    //Handle error caused by larger than 8 bit intensity value
    if(grays>255){
        perror("Image too vast can only handle 8 bits\n");
        fclose(imageFile);
        exit(EXIT_FAILURE);
    }
    newImage->maxGray=grays;

    newImage->data=allocateData(rows,cols);

    // Set data field of image with pixel intensities.
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            pixel=fgetc(imageFile);
            newImage->data[i][j]=(pixel)/255.0f; // Scale down by 255 so that pixel intensity value lies between 0 and 1 as needed by network.
        }
    }

    fclose(imageFile);
    return newImage;

}

//! \brief Assigns integer value to the label according to the corresponding enum POSE
int givePoseLabel(char label[]){
    if(strcmp(label,"straight")==0)
        return POSE(STRAIGHT);
    else if(strcmp(label,"left")==0)
        return POSE(LEFT);
    else if(strcmp(label,"right")==0)
        return POSE(RIGHT);
    else if(strcmp(label,"up")==0)
        return POSE(UP);
    else{
        perror("Incorrect label, clean data first");
        exit(EXIT_FAILURE);
    }
}

//! \brief Assigns integer value to the label according to the corresponding enum SUNGLASS
int giveSunglassLabel(char label[]){
    if(strcmp(label,"open")==0)
        return SUNGLASS(OPEN);
    else if(strcmp(label,"sunglasses")==0)
        return SUNGLASS(SUNGLASSES);
    else{
        perror("Incorrect label, clean data first");
        exit(EXIT_FAILURE);
    }

}

//! \brief Assigns integer value to the label according to the corresponding enum FACE
int giveFaceLabel(char token[]){

    char labels[] ="an2i,at33,boland,bpm,ch4f,cheyer,choon,danieln,glickman,karyadi,kawamura,kk49,megak,mitchell,night,phoebe,saavik,steffi,sz24,tammo";
    char *label;
    int counter=0;
    label=strtok(labels,",");
    while(label){

        if(strcmp(token,label)==0){
            return counter;
        }

        label=strtok(NULL,",");
        counter++;
    }
    return -1;

}


/*! \brief Stores IMAGE representations of every image location listed in given file
 *
 *  \param trainingExamples A list of IMAGE constructs, one for each file specified in the imageList
 *  \param labels A List of vectors where each vector represents the label of the corresponding image instance in the trainingExamples lis
 *  \param labelIndex Specifies which field of file name is to be used as label \n Face is 0 \n Pose is 1 \n Sunglasses is 3
 *  \param imagelist File name of file which contains the list of image files to be parsed on each line.
 *  \returns The total number of images parsed.
 */
int allocateExamples(list<IMAGE *> *trainingExamples,list<vector<float> > *labels,int labelIndex, char *ListFile){
    
    char line[512]; //Line To store scanned line from file
    FILE *imageList;
    char file[512]="../../Data/"; // Relative address to be prepended to file location
    const int index=strlen(file);
    char *label;
    int NumOfExamples=0;
    
    /*Output Vectors for each of the 3 classification possiblities.
    *   Outputs have been encoded as individual nodes for each possibility  and classified label is 0.9 while
    *   the remaining ones are 0.1
    */
    static const float Pose[][4] = { {0.9,0.1,0.1,0.1},{0.1,0.9,0.1,0.1},{0.1,0.1,0.9,0.1},{0.1,0.1,0.1,0.9} };
    static const float Sunglass[][2] = { {0.9,0.1},{0.1,0.9} };
    float FaceL[20]={0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};

    if(!(imageList=fopen(ListFile,"r"))){
        perror("Could Not Open List File");
        exit(EXIT_FAILURE);
    }

    if(labelIndex!=0 && labelIndex!=1 && labelIndex!=3){//Will only classify pose, sunglasses and faces
        perror("Incorrect Label Index Specified");
        exit(EXIT_FAILURE);
    }


    while(fgets(line,sizeof(line),imageList)){
        NumOfExamples++;
        file[index-1]='\0'; //Reboot from previous iteration

        strcat(file,line); //Prepend relative address
        file[strlen(file)-1]='\0';
        
        (trainingExamples)->push_back(readFile(file)); // Add generated IMAGE structure to list of training examples
        
        
        //Use a series of strtoks to extract the label from the file name
        label=strtok(line,"_");

        if(labelIndex==0){ // 0 Corresponds to Face classification
            int j=strlen(label)-1;
            while(label[j--]!='/');
            label=&label[j+2];
        }

        for(int i=0;i<labelIndex;i++){
            label=strtok(NULL,"_");
        }
        
        
        // Add appropriate output vector on to list of outputs depending on the label obtained from file name of training example.
        if(labelIndex==0){
            int l=giveFaceLabel(label);
            FaceL[l]=0.9f;
            labels->push_back(vector<float>(FaceL,FaceL + 20));
            FaceL[l]=0.1f;
        }
        else if(labelIndex==1){
            int l=givePoseLabel(label);
            labels->push_back(vector<float>(Pose[l],Pose[l] + 4));
        }
        else if(labelIndex==3) {
            int l=giveSunglassLabel(label);
            labels->push_back(vector<float>(Sunglass[l],Sunglass[l] +2));
        }
    }

    fclose(imageList);
    return NumOfExamples;
}

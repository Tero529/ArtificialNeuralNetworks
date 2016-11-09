#include"FaceImage.hpp"

using namespace std;


float **allocateData(int row,int col){
    float **returnData;
    returnData=(float **)malloc(row*sizeof(float*));
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


IMAGE *imageAllocate(){
    IMAGE *retVal;
    retVal=(IMAGE *)malloc(sizeof(IMAGE));
    retVal->data=NULL;
    retVal->nRows=0;
    retVal->nCols=0;
    retVal->maxGray=0;
    return retVal;
}

IMAGE *readFile(char *filename){
    FILE *imageFile;
    char line[512];
    float pixel;
    int type,rows,cols,grays;
    IMAGE *newImage;

    imageFile=fopen(filename,"r");
    
    if(imageFile==NULL){
        printf("%s\n",filename);
        perror("Could Not Open File");
        exit(EXIT_FAILURE);
    }
    fgets(line,sizeof(line),imageFile);
    sscanf(line,"P%d",&type);
    
    if(type!=5){
        perror("Incorrect File ");
        fclose(imageFile);
        exit(EXIT_FAILURE);
    }
    
    newImage=imageAllocate();
    
    fgets(line,sizeof(line),imageFile);
    sscanf(line,"%d %d",&cols,&rows);


    newImage->nRows=rows;
    newImage->nCols=cols;
    
    fgets(line,sizeof(line),imageFile);
    sscanf(line,"%d",&grays);
    if(grays>255){
        perror("Image too vast can only handle 8 bits\n");
        fclose(imageFile);
        exit(EXIT_FAILURE);
    }
    newImage->maxGray=grays;
    
    newImage->data=allocateData(rows,cols);
    
    
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            pixel=fgetc(imageFile);
            newImage->data[i][j]=(pixel)/255.0f;
        }
    }
    
    fclose(imageFile);
    return newImage;

}


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

int allocateExamples(list<IMAGE *> *trainingExamples,list<vector<float> > *labels,int labelIndex, char *ListFile){
    //!Label Index (indexed at 0) Specifies which field of file name is label
    //! Face is 0
    //! Pose is 1
    //! Sunglasses is 3
    char line[512];
    FILE *imageList;
    char file[512]="../../Data/";
    const int index=strlen(file);
    char *label;
    int NumOfExamples=0;
    static const float Pose[][4] = { {0.9,0.1,0.1,0.1},{0.1,0.9,0.1,0.1},{0.1,0.1,0.9,0.1},{0.1,0.1,0.1,0.9} };
    static const float Sunglass[][2] = { {0.9,0.1},{0.1,0.9} };
    float FaceL[20]={0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
    
    if(!(imageList=fopen(ListFile,"r"))){
        perror("Could Not Open List File");
        exit(EXIT_FAILURE);
    }
    
    if(labelIndex!=0 && labelIndex!=1 && labelIndex!=3){
        perror("Incorrect Label Index Specified");
        exit(EXIT_FAILURE);
    }
    
    
    while(fgets(line,sizeof(line),imageList)){
        NumOfExamples++;
        file[index-1]='\0';
        
        strcat(file,line);
        file[strlen(file)-1]='\0';
        (trainingExamples)->push_back(readFile(file));
        label=strtok(line,"_");

        if(labelIndex==0){
            int j=strlen(label)-1;
            while(label[j--]!='/');
            label=&label[j+2];
        }
        
        //cout<<label<<endl;
        for(int i=0;i<labelIndex;i++){
            label=strtok(NULL,"_");
        }
        
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
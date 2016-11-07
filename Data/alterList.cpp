#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
using namespace std;

char *files[]={ "./FaceData/straighteven_test1.list.txt",
"./FaceData/straighteven_train.list.txt",
"./FaceData/straighteven_test2.list.txt",
"./PoseData/all_test1.list.txt",
"./SunglassData/straightrnd_test1.list.txt",
"./PoseData/all_test2.list.txt",
"./SunglassData/straightrnd_test2.list.txt",
"./PoseData/all_train.list.txt",
"./SunglassData/straightrnd_train.list.txt"
};

char *alter(char line[]);

int main(int argc,char *argv[]){
    FILE *in,*out;
    int len;
    char outFileName[512],line[512],*outline;
    char *filename;

    for(int i=0;i<9;i++){

    filename=files[i];
        in=fopen(filename,"r");
    strcpy(outFileName,filename);
    len=strlen(outFileName);
    outFileName[len-4]=0;
    strcat(outFileName,"Actual.txt");
    
    if((out=fopen(outFileName,"w"))==NULL){
        perror("Could Not Open File\n");
        exit(EXIT_FAILURE);
    }
    
    while(fgets(line,sizeof(line),in)){
        outline=alter(line);
        fprintf(out,"%s",outline);
    }
        fclose(in);fclose(out);
    }

}

char *alter(char *line){
    
    int len=strlen(line)-1;
    int count=3;
    
    while(count!=0 && len>=0){
        if( line[len]=='/')
           count--;
        len--;
        
    }
           line = &line[len+1];
    return line;
    
}
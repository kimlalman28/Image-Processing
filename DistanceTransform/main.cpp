//
//  main.cpp
//  Distance Transform
//
//  Created by Kimberly Lalmansingh on 2/19/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

int numRows = 0, numCols = 0, minVal = 0, maxVal = 0, newMin = 0, newMax = 0; //data members for header
int** zeroFramedAry;
int neighborAry[5];

//load neighbors of a specific pixel in the zero framed array
void loadNeighbors(int i, int j, string pass){
    //neighbors for pass 1
    if(pass == "pass1"){
        neighborAry[0] = zeroFramedAry[i-1][j-1]; //a
        neighborAry[1] = zeroFramedAry[i-1][j]; //b
        neighborAry[2] = zeroFramedAry[i-1][j+1]; //c
        neighborAry[3] = zeroFramedAry[i][j-1]; //d
    }
    //neighbors for pass 2
    else{
        neighborAry[0] = zeroFramedAry[i][j+1]; //m
        neighborAry[1] = zeroFramedAry[i+1][j-1]; //n
        neighborAry[2] = zeroFramedAry[i+1][j]; //o
        neighborAry[3] = zeroFramedAry[i+1][j+1]; //p
    }
    neighborAry[4] = zeroFramedAry[i][j]; //p(i,j)
}

//zero frame array
void zeroFramed(){
    //top and bottom borders of mirror frame
    for(int i=1; i<=numCols; i++){
        zeroFramedAry[0][i] = 0;
        zeroFramedAry[numRows+1][i] = 0;
    }
    //left and right borders of mirror frame and corners
    for(int i=0; i<=numRows+1; i++){
        zeroFramedAry[i][0] = 0;
        zeroFramedAry[i][numCols+1] = 0;
    }
}

//read in header and pixel values from input file and store in zeroFrameAry
void loadImage(string file){
    ifstream inFile; //input file
    inFile.open(file); //open input file
    //check if input file could not be opened
    if (!inFile) {
        cout << "Unable to open input file"<<endl;
        exit(1);
    }
    int header; //header values
    //read in header values, assign them to their respected variables
    for(int i=0; i<4; i++){
        inFile >> header;
        if(i==0) numRows = header;
        else if(i==1) numCols = header;
        else if (i==2) {
            minVal = header;
            newMin = minVal;
        }
        else {
            maxVal = header;
            newMax = maxVal;
        }
    }
    //set up zeroFramedAry
    zeroFramedAry = new int*[numRows+2];
    for(int i = 0; i < numRows+2; ++i){
        zeroFramedAry[i] = new int[numCols+2];
    }
    int pixel_values;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            inFile >> pixel_values;
            zeroFramedAry[i][j] = pixel_values;
        }
    }
    //close input file
    inFile.close();
}

//pretty print method
void prettyPrint(string pass, ofstream &output){
    output << "Below is the result of " << pass << endl;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            if(zeroFramedAry[i][j] > 0) output << zeroFramedAry[i][j] << " ";
            else output << "  ";
        }
        output << endl;
    }
    output << endl;
}

//pass 1
void firstPassDistance(){
    int min;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            if(zeroFramedAry[i][j] > 0){
                loadNeighbors(i, j, "pass1"); //load neighbors given pass 1
                min = neighborAry[0]; //set min to neighbor 'a'
                //check if other neighbors and smaller than current min
                for(int k=1; k<4; k++){
                    //if another neighbor of p(i,j) is smaller than current min
                    //set that neighbor as the min
                    if(neighborAry[k]<min) min = neighborAry[k];
                }
                //after checking all neighbors, set p(i,j) = to min+1
                zeroFramedAry[i][j] = min+1;
            }
        }//end col loop
    }//end row loop
}

//pass 2
void secondPassDistance(){
    int min;
    for(int i=numRows; i>0; i--){
        for(int j=numCols; j>0; j--){
            if(zeroFramedAry[i][j] > 0){
                loadNeighbors(i, j, "pass2"); //load neighbors given pass 2
                min = neighborAry[4]; //set min to p(i,j)
                //check all other neighbors if it is smaller than current min
                for(int k=0; k<4; k++){
                    //if smaller than current min, set min equal to that neighbor
                    // +1 directly checks if p(i,j)+1 is smaller than current p(i,j)
                    if((neighborAry[k]+1)<min) min = neighborAry[k]+1;
                }
                zeroFramedAry[i][j] = min; //set p(i,j) to min
                if(min>newMax){ //check for new max
                    newMax=min;
                }
                if(min<newMin){ //check for new min
                    newMin = min;
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    loadImage(argv[1]); //read in image and store in zeroframAry
    //change outfile name based on input file name
    string outputFile = argv[1];
    outputFile.erase(outputFile.find_last_of("."), string::npos);

    zeroFramed(); //zero frame
    ofstream output2;
    output2.open(outputFile + "_OutFile2.txt"); //open output file2
    firstPassDistance(); //pass 1
    prettyPrint("Pass 1", output2); //PP pass 1
    secondPassDistance(); //pass 2
    prettyPrint("Pass 2", output2); //PP pass 2
    output2.close(); //close output file 2
    
    ofstream output1;
    output1.open(outputFile + "_OutFile1.txt"); //open output file1
    //output image of pass 2 to outfile 2 for future processing
    output1 << numRows <<" "<< numCols <<" "<< newMin <<" "<< newMax << endl;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            output1 << zeroFramedAry[i][j] << " ";
        }
        output1 << endl;
    }
    output1.close(); //close output file 1
    return 0;
}

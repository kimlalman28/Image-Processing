//
//  main.cpp
//  Median3x3filtering
//
//  Created by Kimberly Lalmansingh on 2/10/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

int numRows = 0, numCols = 0, minVal = 0, maxVal = 0, newMin = 0, newMax = 0; //data members for header
int** mirrorFramedAry;
int**  tempAry;
int neighborAry[9];

//selection sort to sort neighbor array
int selectionSort(int nAry[]){
    int minIndex, temp;
    for (int i=0; i < 5; i++){
        minIndex = i;
        for (int j=i+1; j < 9; j++){
            if (nAry[j] < nAry[minIndex])
                minIndex = j;
        }
        if (minIndex != i)
        {
            temp = nAry[i];
            nAry[i] = nAry[minIndex];
            nAry[minIndex] = temp;
        }
    }
    return nAry[4];
}

//load neighbors of a specific pixel in the mirror framed array
void loadNeighbors(int i, int j){
    neighborAry[0] = mirrorFramedAry[i-1][j-1];
    neighborAry[1] = mirrorFramedAry[i-1][j];
    neighborAry[2] = mirrorFramedAry[i-1][j+1];
    neighborAry[3] = mirrorFramedAry[i][j-1];
    neighborAry[4] = mirrorFramedAry[i][j];
    neighborAry[5] = mirrorFramedAry[i][j+1];
    neighborAry[6] = mirrorFramedAry[i+1][j-1];
    neighborAry[7] = mirrorFramedAry[i+1][j];
    neighborAry[8] = mirrorFramedAry[i+1][j+1];
}

//mirror frame array as learned in class
void mirrorFramed(){
    //top and bottom borders of mirror frame
    for(int i=1; i<=numCols; i++){
        mirrorFramedAry[0][i] = mirrorFramedAry[1][i];
        mirrorFramedAry[numRows+1][i] = mirrorFramedAry[numRows][i];
    }
    //left and right borders of mirror frame and corners
    for(int i=0; i<=numRows+1; i++){
        mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
        mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];
    }
}

//read in header and pixel values from input file and store in mirrorFrameAry to be mirror framed
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
    //set up mirrorFramedAry and tempAry
    mirrorFramedAry = new int*[numRows+2];
    for(int i = 0; i < numRows+2; ++i){
        mirrorFramedAry[i] = new int[numCols+2];
    }
    tempAry = new int*[numRows+2];
    for(int i = 0; i < numRows+2; ++i){
        tempAry[i] = new int[numCols+2];
    }
    int pixel_values;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            inFile >> pixel_values;
            mirrorFramedAry[i][j] = pixel_values;
        }
    }
    //close input file
    inFile.close();
}

//Median 3X3 algorithm
void Median3X3(string file){
    loadImage(file);
   
    ofstream outFile; //output file
    //removes input file extention for use in naming the output file correcly
    string outfileEnding = file.substr(file.length()-5);
    //ending will be N.txt where N is the Median3X3OutN.txt n = 0, 1, ..., 9
    outFile.open("Median3X3Out" + outfileEnding);
    
    mirrorFramed();
    //for each pixel of the image in the mirror framed array, load neighbors
    //find the median value and store in temp array and check if the median value can be a new min/max
    int median;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            loadNeighbors(i, j);
            median = selectionSort(neighborAry);
            tempAry[i][j] = median;
            if(median<newMin){
                newMin=median;
            }
            if(median>newMax){
                newMax=median;
            }
        }
    }
    //output temp array pixel values to output file
    outFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            outFile << tempAry[i][j] << " ";
        }
        outFile << endl;
    }
    //close output files
    outFile.close();
}

int main(int argc, const char * argv[]) {
    Median3X3(argv[1]);
    return 0;
}

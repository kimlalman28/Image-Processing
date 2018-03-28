//
//  main.cpp
//  ThinningSkeleton
//
//  Created by Kimberly Lalmansingh on 3/2/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class ThinningSkeleton{
public:
    v
    //data members
    bool changeFlag;
    int cycleCount;
    int numRows, numCols, minVal, maxVal; //data members for header
    int** firstAry;
    int** secondAry;
    
    //constructor
    ThinningSkeleton(int rows, int cols, int min, int max){
        numRows = rows;
        numCols = cols;
        minVal = min;
        maxVal = max;
        //dynamically allocate arrays
        firstAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; ++i){
            firstAry[i] = new int[numCols+2];
        }
        secondAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; ++i){
            secondAry[i] = new int[numCols+2];
        }
        cycleCount = 0;
        changeFlag = true;
    };
    
    //methods
    void zeroFramed();
    void loadImage(ifstream &inFile);
    void copyAry();
    void doThinning(int i, int j);
    void NorthThinning();
    void SouthThinning();
    void EastThinning();
    void WestThinning();
    void prettyPrint(ofstream &outfile);

};

void ThinningSkeleton::zeroFramed(){
    //top and bottom borders of zero frame
    for(int i=1; i<=numCols; i++){
        firstAry[0][i] = 0;
        firstAry[numRows+1][i] = 0;
        secondAry[0][i] = 0;
        secondAry[numRows+1][i] = 0;
    }
    //left and right borders of zero frame
    for(int i=0; i<=numRows+1; i++){
        firstAry[i][0] = 0;
        firstAry[i][numCols+1] = 0;
        secondAry[i][0] = 0;
        secondAry[i][numCols+1] = 0;
    }
}

void ThinningSkeleton::loadImage(ifstream &inFile){
    //load file onto firstAry
    int pixel_values=0;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            inFile >> pixel_values;
            firstAry[i][j] = pixel_values;
        }
    }
    //close input file
    inFile.close();
}

void ThinningSkeleton::copyAry(){
    //copy secondAry to firstAry
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            firstAry[i][j] = secondAry[i][j];
        }
    }
}

void ThinningSkeleton::doThinning(int i, int j){
    bool condition1 = true;
    bool condition2 = true;
    int greaterThanZero = 0;
    
    //condition 1
    //check if there are four or more neighbors of p(i,j) that are equal to 1
    for(int r=i-1; r<=i+1; r++){
        for(int c=j-1; c<=j+1; c++){
            if(r==i && c == j) continue;
            if(firstAry[r][c]>0) greaterThanZero++;
        }
    }
    if(greaterThanZero < 4) condition1 = false;
    
    //condition 2
    //Below are the cases that will create multiple components if p(i,j) is set to 0
    if(firstAry[i-1][j]==0 && firstAry[i+1][j]==0) condition2 = false; //verical
    else if(firstAry[i][j-1]==0 && firstAry[i][j+1]==0) condition2 = false; //horizontal
    else if(firstAry[i][j+1]==0 && firstAry[i+1][j]==0 && firstAry[i+1][j+1]==1) condition2 = false;
    else if(firstAry[i][j-1]==0 && firstAry[i+1][j]==0 && firstAry[i+1][j-1]==1) condition2 = false;
    else if(firstAry[i-1][j]==0 && firstAry[i][j+1]==0 && firstAry[i-1][j+1]==1) condition2 = false;
    else if(firstAry[i][j-1]==0 && firstAry[i-1][j]==0 && firstAry[i-1][j-1]==1) condition2 = false;

    //if both conditions are passed, change p(i,j) to 0
    if((condition1 == true) && (condition2==true)){
        secondAry[i][j] = 0;
        changeFlag = true;
    }
    //else p(i,j) stays as 1
    else secondAry[i][j] = 1;
}

void ThinningSkeleton::NorthThinning(){
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            //check north neighbor
            if(firstAry[i][j]>0 && firstAry[i-1][j]==0)
                doThinning(i, j);
            else {
                //if firstAry == 1 but its north neighbor != 0,
                //set secondAry[i][j] = to firstAry[i][j] or else
                //secondAry will be filled in as 0 and would copy over 0s
                //when calling copy()
                secondAry[i][j] = firstAry[i][j];
            }
        }
    }
}

void ThinningSkeleton::SouthThinning(){
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            //check south neighbor
            if(firstAry[i][j]>0 && firstAry[i+1][j]==0)
                doThinning(i, j);
            else {
                secondAry[i][j] = firstAry[i][j];
            }
        }
    }
}

void ThinningSkeleton::WestThinning(){
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            //check west neighbor
            if(firstAry[i][j]>0 && firstAry[i][j-1]==0)
                doThinning(i, j);
            else {
                secondAry[i][j] = firstAry[i][j];
            }
        }
    }
}

void ThinningSkeleton::EastThinning(){
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            //check east neighbor
            if(firstAry[i][j]>0 && firstAry[i][j+1]==0)
                doThinning(i, j);
            else {
                secondAry[i][j] = firstAry[i][j];
            }
        }
    }
}

void ThinningSkeleton::prettyPrint(ofstream &outfile){
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            if(firstAry[i][j] > 0) outfile << firstAry[i][j] << " ";
            else outfile << "  ";
        }
        outfile << endl;
    }
    outfile << endl;
}



int main(int argc, const char * argv[]) {
    ifstream inFile; //input file
    ofstream outfile1, outfile2;
    inFile.open(argv[1]); //open input file
    outfile1.open(argv[2]);
    outfile2.open(argv[3]);
    //check if input file could not be opened
    if (!inFile) {
        cout << "Unable to open input file"<<endl;
        exit(1);
    }
    int header=0, rows=0, cols=0, min=0, max=0; //header values
    //read in header values, assign them to their respected variables
    for(int i=0; i<4; i++){
        inFile >> header;
        if(i==0) rows = header;
        else if(i==1) cols = header;
        else if (i==2) min = header;
        else max = header;
    }
    //thinning object
    ThinningSkeleton thinning(rows, cols, min, max);
    thinning.zeroFramed(); //zeroFrame
    thinning.loadImage(inFile); //loadImage
    outfile2 << "Initial Image" << endl;
    thinning.prettyPrint(outfile2); //prettyPrint input

    //thinning loop
    while(thinning.changeFlag == true){
        thinning.changeFlag = false;
        thinning.cycleCount++;

        thinning.NorthThinning();
        thinning.copyAry();
        if(thinning.cycleCount==2 || thinning.cycleCount==4){
            outfile2 << "Result of cycle #" << thinning.cycleCount << " of North Thinning"<< endl;
            thinning.prettyPrint(outfile2);
        }

        thinning.SouthThinning();
        thinning.copyAry();
        if(thinning.cycleCount==2 || thinning.cycleCount==4){
            outfile2 << "Result of cycle #" << thinning.cycleCount << " of South Thinning"<< endl;
            thinning.prettyPrint(outfile2);
        }

        thinning.WestThinning();
        thinning.copyAry();
        if(thinning.cycleCount==2 || thinning.cycleCount==4){
            outfile2 << "Result of cycle #" << thinning.cycleCount << " of West Thinning"<< endl;
            thinning.prettyPrint(outfile2);
        }

        thinning.EastThinning();
        thinning.copyAry();
        if(thinning.cycleCount==2 || thinning.cycleCount==4){
            outfile2 << "Result of cycle #" << thinning.cycleCount << " of East Thinning"<< endl;
            thinning.prettyPrint(outfile2);
        }
    }
    outfile2 << "End of Thinning Result" << endl;
    thinning.prettyPrint(outfile2); //prettyPrint after breaking out of loop
    
    //output image to outfile 1 for future processing
    outfile1 << thinning.numRows <<" "<< thinning.numCols <<" "<< thinning.minVal <<" "<< thinning.maxVal << endl;
    for(int i=1; i<=thinning.numRows; i++){
        for(int j=1; j<=thinning.numCols; j++){
            outfile1 << thinning.firstAry[i][j] << " ";
        }
        outfile1 << endl;
    }
    outfile1.close(); //close output file 1
    outfile2.close();
    return 0;
}

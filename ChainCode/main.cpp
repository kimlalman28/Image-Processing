//
//  main.cpp
//  ChainCode
//
//  Created by Kimberly Lalmansingh on 3/14/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class Image{ //image class
public:
    //data members
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int** imgAry;
    
    //methods
    //constructor
    Image(int rows, int cols, int min, int max){
        numRows = rows;
        numCols = cols;
        minVal = min;
        maxVal = max;
        //dynamically allocate array
        imgAry = new int*[numRows+2];
        for(int i = 0; i < numRows+2; ++i){
            imgAry[i] = new int[numCols+2];
        }
    };
    
    void loadImage(ifstream &inFile);
    void zeroFramed();
};


class chainCode { //chain code class
public:
    class Point { //point class
    public:
        int row;
        int col;
        
        //getters and setters for point class
        void setPoint(int r, int c){
            row = r;
            col = c;
        };
        int getRow(){return row;};
        int getCol(){return col;};
    };
    
    Point neighborCoord[8];
    Point startP;
    Point currentP;
    Point nextP;
    int lastQ;
    int nextQ;
    int nextDirTable[8] = {6,6,0,0,2,2,4,4}; //direction table for last zero
    int nextDir;
    int PchainDir;
    
    //methods
    //constructor  for chain code class
    chainCode(){
        startP.setPoint(0, 0);
        currentP.setPoint(0, 0);
        nextP.setPoint(0, 0);
        lastQ = 0;
        nextQ = 0;
        nextDir = 0;
        PchainDir = 0;
    };
    
    void getChainCode(int rows, int cols, int** imgAry, ofstream &outfile1, ofstream &outfile2);
    void loadNeighborsCoord();
    int findNextP(int** imgAry);
    int getChainDir(int** imgAry);
    void prettyPrint(int** imgAry, ofstream &outfile, int rows, int cols);
};

void Image::loadImage(ifstream &inFile){
    //load file onto firstAry
    int pixel_values=0;
    for(int i=1; i<=numRows; i++){
        for(int j=1; j<=numCols; j++){
            inFile >> pixel_values;
            imgAry[i][j] = pixel_values;
        }
    }
    //close input file
    inFile.close();
}

void Image::zeroFramed(){
    //top and bottom borders of zero frame
    for(int i=1; i<=numCols; i++){
        imgAry[0][i] = 0;
        imgAry[numRows+1][i] = 0;
    }
    //left and right borders of zero frame
    for(int i=0; i<=numRows+1; i++){
        imgAry[i][0] = 0;
        imgAry[i][numCols+1] = 0;
    }
}

void chainCode::getChainCode(int rows, int cols, int** imgAry, ofstream &outfile1, ofstream &outfile2){
    bool foundNonZero = false; //when a nonzero pixel is found, break out of for loops to find the start pixel of object
    for(int i=1; i<=rows; i++){
        for(int j=1; j<=cols; j++){
            if(imgAry[i][j]>0){
                foundNonZero = true;
                outfile1 << imgAry[i][j] <<  " " <<  i << " " << j << " "; //label, startRow, startCol for outfile 1
                outfile2 << i << " " << j << " " << imgAry[i][j] << endl; //startRow, startCol, label for outfile 2
                startP.setPoint(i, j);
                currentP.setPoint(i, j);
                lastQ = 4;
            }
            if(foundNonZero) break;
        }
        if(foundNonZero) break;
    }
    if(!foundNonZero) return; //no objects in image, dont execute next set of code
    
    //continue loop until the current pixel gets back to the start pixel
    bool finished = false;
    int outFileChainCode = 0;
    while(!finished){
        nextQ = (lastQ+1)%8; //next 0
        PchainDir = findNextP(imgAry); //nextP will be determined inside this method
        outfile1 << PchainDir << " ";
        outFileChainCode++;
        outfile2 << PchainDir << " ";
        if(outFileChainCode == 15){
            outfile2 << endl;
            outFileChainCode = 0;
        }
        
        lastQ = nextDirTable[PchainDir]; //get last zero using next direction array
        currentP = nextP; //set current pixel equal to next pixel
        if((currentP.row == startP.row) && (currentP.col == startP.col)) finished = true;
    }
}

void chainCode::loadNeighborsCoord(){
    //neighbors of p(i,j), an array of the points of p(i,j)s neighbors
    neighborCoord[0].setPoint(currentP.getRow(), currentP.getCol()+1);
    neighborCoord[1].setPoint(currentP.getRow()-1, currentP.getCol()+1);
    neighborCoord[2].setPoint(currentP.getRow()-1, currentP.getCol());
    neighborCoord[3].setPoint(currentP.getRow()-1, currentP.getCol()-1);
    neighborCoord[4].setPoint(currentP.getRow(), currentP.getCol()-1);
    neighborCoord[5].setPoint(currentP.getRow()+1, currentP.getCol()-1);
    neighborCoord[6].setPoint(currentP.getRow()+1, currentP.getCol());
    neighborCoord[7].setPoint(currentP.getRow()+1, currentP.getCol()+1);
}

int chainCode::findNextP(int** imgAry){
    loadNeighborsCoord(); //load neighbors
    int chainDir = getChainDir(imgAry); //get direction of next non zero pixel
    //set row and col position of next non zero pixel
    nextP.setPoint(neighborCoord[chainDir].getRow(), neighborCoord[chainDir].getCol());
    return chainDir; //return the direction of next pixel
}

int chainCode::getChainDir(int** imgAry){
    int nonZero = nextQ;
    //check if next zero position of p(i,j)s neighbor value is non zero
    if(imgAry[neighborCoord[nonZero].getRow()][neighborCoord[nonZero].getCol()] > 0) return nonZero;
    else {
        //check all other remaining neighbors of p(i,j)
        nonZero = (nonZero+1)%8;
        while (nonZero != nextQ) {
            if(imgAry[neighborCoord[nonZero].getRow()][neighborCoord[nonZero].getCol()] > 0) return nonZero;
            else nonZero = (nonZero+1)%8;
        }
    }
    return -1; //no non-zero neighbors, return statement here just for sake of method return type
}

void chainCode::prettyPrint(int** imgAry, ofstream &outfile, int rows, int cols){
    for(int i=1; i<=rows; i++){
        for(int j=1; j<=cols; j++){
            if(imgAry[i][j] > 0) outfile << imgAry[i][j] << " ";
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
    //output headers to outFiles
    outfile1 << rows <<" "<< cols <<" "<< min <<" "<< max << endl;
    outfile2 << rows <<" "<< cols <<" "<< min <<" "<< max << endl;
    
    Image img(rows, cols, min, max); //call constructor
    img.loadImage(inFile); //load image onto array
    img.zeroFramed(); //zero frame
    
    chainCode cc; //new chain code
    cc.getChainCode(img.numRows, img.numCols, img.imgAry, outfile1, outfile2);
    
    outfile1.close();
    outfile2.close();
    return 0;
}

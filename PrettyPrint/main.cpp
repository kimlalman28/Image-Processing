//
//  main.cpp
//  IP_31 Project 1.2 PrettyPrint
//
//  Created by Kimberly Lalmansingh on 2/4/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

void prettyPrint(string file){
    int numRows = 0, numCols = 0, minVal = 0, maxVal = 0; //data members for header
    ifstream inFile; //input file
    ofstream outFile; //output file
    
    inFile.open(file); //open input file
    //check if input file could not be opened
    if (!inFile) {
        cout << "Unable to open input file";
        exit(1);
    }
    //removes input file extention for use in naming the output file correcly
    string outputFile = file;
    outputFile.erase(outputFile.find_last_of("."), string::npos);

    outFile.open(outputFile + "_PP.txt"); //open output file
    
    int header; //header values
    int pixel_val; //pixel values
    
    //read in header values, assign them to their
    //variables and output to ouput file
    for(int i=0; i<4; i++){
        inFile >> header;
        if(i==0) numRows = header;
        else if(i==1) numCols = header;
        else if (i==2) minVal = header;
        else maxVal = header;
    }
    
    //write header to output file
    outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
    
    //read in each pixel value and check if it passes the conditions
    //of pretty print
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            inFile >> pixel_val;
            if(pixel_val > 0) outFile << pixel_val << " ";
            else outFile << "  ";
        }
        outFile << endl;
    }
    //close input and output files
    inFile.close();
    outFile.close();
} //end pretty print method

int main(int argc, const char * argv[]) {
    prettyPrint(argv[1]);
    return 0;
}

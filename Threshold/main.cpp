//
//  main.cpp
//  IP_31 Project 1.3 Threshold
//
//  Created by Kimberly Lalmansingh on 2/5/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;


int main(int argc, const char * argv[]) {
    //data memebers for header
    int numRows = 0, numCols = 0, minVal = 0, maxVal = 0; //data members for header
    ifstream inFile; //input file
    ofstream outFile; //output file
    
    inFile.open(argv[1]); //open input file
    //check if input file could not be opened
    if (!inFile) {
        cout << "Unable to open input file";
        exit(1);
    }
    int header;
    //read in header and assign to respected variable
    for(int i=0; i<4; i++){
        inFile >> header;
        if(i==0) numRows = header;
        else if(i==1) numCols = header;
        else if (i==2) minVal = header;
        else maxVal = header;
    }
    //ask user for threshod value
    int thrValue;
    cout << "Please enter a threshold value: ";
    cin >> thrValue;
    
    //generate output file name
    string outFileName = argv[1];
    outFileName.erase(outFileName.find_last_of("."), string::npos);
    outFileName = outFileName + "_thr_" + to_string(thrValue) + ".txt";
    
    outFile.open(outFileName); //open output file
    //write header to file
    outFile << numRows << " " << numCols << " 0" << " 1" << endl;
    
    //read in each pixel value and check if it passes the conditions
    //of the threshold
    int pixel_val;
    for(int i=0; i<numRows; i++){
        for(int j=0; j<numCols; j++){
            inFile >> pixel_val;
            if(pixel_val >= thrValue) outFile << "1 ";
            else outFile << "0 ";
        }
        outFile << endl;
    }
    //close input and output files
    inFile.close();
    outFile.close();
    
    return 0;
}

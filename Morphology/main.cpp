//
//  main.cpp
//  Morphology
//
//  Created by Kimberly Lalmansingh on 5/5/18.
//  Copyright © 2018 Kimberly Lalmansingh. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

class Morphology {
    private:
    int numRowsImg;
    int numColsImg;
    int minImg;
    int maxImg;
    int numRowsStructElem;
    int numColsStructElem;
    int minStrctElem;
    int maxStrctElem;
    int rowOrigin;
    int colOrigin;
    int rowFrameSize;
    int colFrameSize;
    int** imgAry;
    int** morphAry;
    int** structElemAry;
    int** tempAry;
    
    public:
    Morphology(int rI, int cI, int minVI, int maxVI, int rS, int cS, int minVS, int maxVS, int rO, int cO);
    void computeFrameSize();
    void loadImage(ifstream &inFile1);
    void loadStruct(ifstream &inFile2);
    void zeroFrameImg();
    void initMorphAry();
    void dilation();
    void erosion();
    void closing();
    void opening();
    void prettyPrint(string type);
    void outputResult(ofstream &outfile, string type);
    void allocateImgAry();
    void allocateMorphAry();
    void allocateSEAry();
    void prettyPrintImg(string type);
    void prettyPrintSE(string type);
    void initTemp();
    
};

Morphology::Morphology(int rI, int cI, int minVI, int maxVI, int rS, int cS, int minVS, int maxVS, int rO, int cO) {
    numRowsImg = rI;
    numColsImg = cI;
    minImg = minVI;
    maxImg = maxVI;
    numRowsStructElem = rS;
    numColsStructElem = cS;
    minStrctElem = minVS;
    maxStrctElem = maxVS;
    rowOrigin = rO;
    colOrigin = cO;
}

void Morphology::computeFrameSize(){
    rowFrameSize = numRowsStructElem*2;
    colFrameSize = numColsStructElem*2;
}

void Morphology::loadImage(ifstream &inFile1){
    for(int i=rowFrameSize/2; i<numRowsImg+(rowFrameSize/2); i++){
        for(int j=colFrameSize/2; j<numColsImg+(colFrameSize/2); j++){
            inFile1 >> imgAry[i][j];
        }
    }
}

void Morphology::loadStruct(ifstream &inFile2){
    for(int i=0; i<numRowsStructElem; i++){
        for(int j=0 ; j<numColsStructElem; j++){
            inFile2 >> structElemAry[i][j];
        }
    }
}

void Morphology::zeroFrameImg(){
    for(int i=0; i<numRowsImg+(rowFrameSize/2); i++){
        imgAry[i][0] = 0;
        imgAry[i][numColsImg+colFrameSize-1] = 0;
    }
    
    for(int j=colFrameSize; j<numColsImg+(colFrameSize/2); j++){
        imgAry[0][j] = 0;
        imgAry[numRowsImg+rowFrameSize-1][j] = 0;
    }
}

void Morphology::initMorphAry(){
    for(int i=(rowFrameSize/2)/2; i<=numRowsImg+(rowFrameSize/2); i++){
        for(int j=(colFrameSize/2)/2; j<=numColsImg+(colFrameSize/2); j++){
            morphAry[i][j] = 0;
        }
    }
}

void Morphology::initTemp(){
    for(int i=(rowFrameSize/2)/2; i<=numRowsImg+(rowFrameSize/2); i++){
        for(int j=(colFrameSize/2)/2; j<=numColsImg+(colFrameSize/2); j++){
            tempAry[i][j] = 0;
        }
    }
}

void Morphology::dilation(){
    for(int i=rowFrameSize/2; i<numRowsImg+(rowFrameSize/2); i++){
        for(int j=colFrameSize/2; j<numColsImg+(colFrameSize/2); j++){
            if(imgAry[i][j] == structElemAry[rowOrigin][colOrigin]){
                for(int m=0; m<numRowsStructElem; m++){
                    for(int n=0; n<numColsStructElem; n++){
                        if(structElemAry[m][n]>0){
                            morphAry[i+m-(rowOrigin)][j+n-(colOrigin)] = structElemAry[m][n];
                        }
                    }
                }
            }
        }
    }
}

void Morphology::erosion(){
    for(int i=rowFrameSize/2; i<numRowsImg+(rowFrameSize/2); i++){
        for(int j=colFrameSize/2; j<numColsImg+(colFrameSize/2); j++){
            if(imgAry[i][j] > 0){
                bool equal = true;
                for(int m=0; m<numRowsStructElem; m++){
                    for(int n=0; n<numColsStructElem; n++){
                        if(structElemAry[m][n] == 0) continue;
                        else if(!(structElemAry[m][n]==imgAry[i+m-(rowOrigin)][j+n-(colOrigin)])){
                            equal = false;
                            break;
                        }
                        if(!equal) break;
                    }
                }
                if(equal) morphAry[i][j] = 1;
            }
        }
    }
}

void Morphology::closing(){
    initTemp();
    //dilation
    for(int i=rowFrameSize/2; i<numRowsImg+(rowFrameSize/2); i++){
        for(int j=colFrameSize/2; j<numColsImg+(colFrameSize/2); j++){
            if(imgAry[i][j] == structElemAry[rowOrigin][colOrigin]){
                for(int m=0; m<numRowsStructElem; m++){
                    for(int n=0; n<numColsStructElem; n++){
                        if(structElemAry[m][n]>0){
                            tempAry[i+m-(rowOrigin)][j+n-(colOrigin)] = structElemAry[m][n];
                        }
                    }
                }
            }
        }
    }
    
    //erosion
    for(int i=(rowFrameSize/2)/2; i<=numRowsImg+(rowFrameSize/2); i++){
        for(int j=(colFrameSize/2)/2; j<=numColsImg+(colFrameSize/2); j++){
            if(tempAry[i][j] > 0){
                bool equal = true;
                for(int m=0; m<numRowsStructElem; m++){
                    for(int n=0; n<numColsStructElem; n++){
                        if(structElemAry[m][n] == 0) continue;
                        else if(!(structElemAry[m][n]==tempAry[i+m-(rowOrigin)][j+n-(colOrigin)])){
                            equal = false;
                            break;
                        }
                        if(!equal) break;
                    }
                }
                if(equal) morphAry[i][j] = 1;
            }
        }
    }
}

void Morphology::opening(){
    initTemp();
    //erosion
    for(int i=rowFrameSize/2; i<numRowsImg+(rowFrameSize/2); i++){
        for(int j=colFrameSize/2; j<numColsImg+(colFrameSize/2); j++){
            if(imgAry[i][j] > 0){
                bool equal = true;
                for(int m=0; m<numRowsStructElem; m++){
                    for(int n=0; n<numColsStructElem; n++){
                        if(structElemAry[m][n] == 0) continue;
                        else if(!(structElemAry[m][n]==imgAry[i+m-(rowOrigin)][j+n-(colOrigin)])){
                            equal = false;
                            break;
                        }
                        if(!equal) break;
                    }
                }
                if(equal) tempAry[i][j] = 1;
            }
        }
    }
    
    //dilation
    for(int i=(rowFrameSize/2)/2; i<=numRowsImg+(rowFrameSize/2); i++){
        for(int j=(colFrameSize/2)/2; j<=numColsImg+(colFrameSize/2); j++){
            if(tempAry[i][j] == structElemAry[rowOrigin][colOrigin]){
                for(int m=0; m<numRowsStructElem; m++){
                    for(int n=0; n<numColsStructElem; n++){
                        if(structElemAry[m][n]>0){
                            morphAry[i+m-(rowOrigin)][j+n-(colOrigin)] = structElemAry[m][n];
                        }
                    }
                }
            }
        }
    }
}

void Morphology::prettyPrint(string type){
    cout << type <<endl;
    for(int i=(rowFrameSize/2)/2; i<=numRowsImg+((rowFrameSize/2)); i++){
        for(int j=(colFrameSize/2)/2; j<=numColsImg+((colFrameSize/2)); j++){
            if(morphAry[i][j]==0) cout << "  ";
            else cout << morphAry[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Morphology::prettyPrintImg(string type){
    cout << type <<endl;
    for(int i=rowFrameSize/2; i<numRowsImg+(rowFrameSize/2); i++){
        for(int j=colFrameSize/2; j<numColsImg+(colFrameSize/2); j++){
            if(imgAry[i][j]==0) cout << "  ";
            else cout << imgAry[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Morphology::prettyPrintSE(string type){
    cout << type <<endl;
    for(int i=0; i<numRowsStructElem; i++){
        for(int j=0; j<numColsStructElem; j++){
            if(structElemAry[i][j]==0) cout << "  ";
            else cout << structElemAry[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Morphology::outputResult(ofstream &outfile, string type){
    outfile << type << endl;
    for(int i=(rowFrameSize/2)/2; i<=numRowsImg+((rowFrameSize/2)); i++){
        for(int j=(colFrameSize/2)/2; j<=numColsImg+((colFrameSize/2)); j++){
            if(morphAry[i][j]==0) outfile << "  ";
            else outfile << morphAry[i][j] << " ";
        }
        outfile << endl;
    }
}



void Morphology::allocateImgAry(){
    imgAry = new int*[numRowsImg+(rowFrameSize)];
    for(int i=0; i<(numRowsImg+(rowFrameSize)); i++){
        imgAry[i] = new int[(numColsImg+(colFrameSize))];
    }
}

void Morphology::allocateMorphAry(){
    morphAry = new int*[numRowsImg+(rowFrameSize)];
    for(int i=0; i<(numRowsImg+(rowFrameSize)); i++){
        morphAry[i] = new int[(numColsImg+(colFrameSize))];
    }
    
    tempAry = new int*[numRowsImg+(rowFrameSize)];
    for(int i=0; i<(numRowsImg+(rowFrameSize)); i++){
        tempAry[i] = new int[(numColsImg+(colFrameSize))];
    }
}

void Morphology::allocateSEAry(){
    structElemAry = new int*[numRowsStructElem];
    for(int i=0; i<(numRowsStructElem); i++){
        structElemAry[i] = new int[(numColsStructElem)];
    }
}


int main(int argc, const char * argv[]) {
    ifstream inFile1; //input file image
    ifstream inFile2; //input file SE
    ofstream outfile1, outfile2, outfile3, outfile4;
    inFile1.open(argv[1]); //open input file
    inFile2.open(argv[2]);
    outfile1.open(argv[3]);
    outfile2.open(argv[4]);
    outfile3.open(argv[5]);
    outfile4.open(argv[6]);
    //check if input file could not be opened
    if (!inFile1 || !inFile2) {
        cout << "Unable to open input file"<<endl;
        exit(1);
    }
    int rowsI=0, colsI=0, minI=0, maxI=0; //header values
    int rowsS=0, colsS=0, minS=0, maxS=0, rowO=0, colO=0;
    //read in header values, assign them to their respected variables
    inFile1 >> rowsI;
    inFile1 >> colsI;
    inFile1 >> minI;
    inFile1 >> maxI;
    inFile2 >> rowsS;
    inFile2 >> colsS;
    inFile2 >> minS;
    inFile2 >> maxS;
    inFile2 >> rowO;
    inFile2 >> colO;
    Morphology morph(rowsI, colsI, minI, maxI, rowsS, colsS, minS, maxS, rowO, colO);
    morph.computeFrameSize();
    morph.allocateImgAry();
    morph.loadImage(inFile1);
    morph.zeroFrameImg();
    morph.prettyPrintImg("Input Image"); // image array to console
    morph.allocateMorphAry();
    morph.allocateSEAry();
    morph.loadStruct(inFile2);
    morph.prettyPrintSE("Structuring Element"); // struct array to console

    morph.initMorphAry();
    morph.dilation();
    morph.prettyPrint("Result of Dilation"); //result of dilation to console
    morph.outputResult(outfile1, "Result of Dilation");

    morph.initMorphAry();
    morph.erosion();
    morph.prettyPrint("Result of Erosion"); //result of erosion to console
    morph.outputResult(outfile2, "Result of Erosion");

    morph.initMorphAry();
    morph.closing();
    morph.prettyPrint("Result of Closing"); //result of closing to console
    morph.outputResult(outfile3, "Result of Closing");

    morph.initMorphAry();
    morph.opening();
    morph.prettyPrint("Result of Opening"); //result of opening to console
    morph.outputResult(outfile4, "Result of Opening");
    
    inFile1.close();
    inFile2.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    outfile4.close();
    
    return 0;
}

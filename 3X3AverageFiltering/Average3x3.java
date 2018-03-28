import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/*
 * Kimberly Lalmansingh
 * IP_31 Image Processing
 */

public class Average3x3 {
	//initialize data members for header
	static int numRows=0, numCols=0, minVal=0, maxVal=0, newMin=0, newMax=0;
	static int[][] mirrorFramedAry;
	static int[][] tempAry;
	static int[] neighborAry;
	
	public static void main(String args[]){
		AVG3X3(args[0], args[1]); //call algorithm
	}
	
	public static void AVG3X3(String inputFile, String outputFile){
		File inFile = new File(inputFile);
		try {
			BufferedWriter outputLine = new BufferedWriter(new FileWriter(outputFile));
			loadImage(inFile); 
			mirrorFramed();
			//calculate averages, gets neighbors and stores average in temp array
			int sum, average;
			for(int i=1; i<=numRows; i++){
				for(int j=1; j<=numCols; j++){
					sum=0;
					loadNeighbors(i, j);
					for(int k=0; k<9; k++){
						sum = sum + neighborAry[k];
					}
					average = sum/9;
					tempAry[i][j] = average; //store average
					//check if newMin and newMax values are to be updated
					if(average<newMin){
						newMin=average;
					}
					if(average>newMax){
						newMax=average;
					}
				}
			}
			//output temp array to output file
			outputLine.write(numRows + " " + numCols + " " + newMin + " " + newMax); //header information
			outputLine.newLine();
			for(int i=1; i<=numRows; i++){
				for(int j=1; j<=numCols; j++){
					outputLine.write(tempAry[i][j] + " ");
				}
				outputLine.newLine();
			}
			outputLine.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	//read in image from input file
	public static void loadImage(File inFile){
		try {
			Scanner line = new Scanner(inFile);
			int input;
			//information about image header
			for(int i=0; i<4; i++){
				input = line.nextInt();
				if(i==0) numRows = input;
				else if(i==1) numCols = input;
				else if (i==2){
					minVal = input;
					newMin=minVal;
				}
				else{
					maxVal = input;
					newMax=maxVal;
				}
			}	
			//Allocate arrays 
			mirrorFramedAry = new int[numRows+2][numCols+2];
			tempAry = new int[numRows+2][numCols+2];
			//read pixel values from input file
			for(int i=1; i<=numRows; i++){
				for(int j=1; j<=numCols; j++){
					input = line.nextInt();
					mirrorFramedAry[i][j] = input;
				}
			}
			line.close();
		} 
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	//mirror frame array as done in class
	public static void mirrorFramed(){
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
	
	//sets the neighbors of a specific pixel
	public static void loadNeighbors(int i, int j){
		neighborAry = new int[9];
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
}

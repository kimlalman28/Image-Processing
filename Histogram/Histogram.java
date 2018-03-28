import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/*
 * Kimberly Lalmansingh
 * Image Processing
 * Project 1.1
 */

public class Histogram {
	//initialize data members for header
	static int numRows=0, numCols=0, minVal=0, maxVal=0;
	static int[] hist;
	
	public static void main(String args[]){
		//runs histogram algorithm passing in input and output files as arguments
		computeHistogram(args[0], args[1]); 
	}
	
	public static void computeHistogram(String inputFile, String outputFile){
		File file = new File(inputFile);
		try {
			Scanner line = new Scanner(file);
			int input;
			//information about image header
			for(int i=0; i<4; i++){
				input = line.nextInt();
				if(i==0) numRows = input;
				else if(i==1) numCols = input;
				else if (i==2) minVal = input;
				else maxVal = input;
			}
			//Instantiate array 
			hist = new int[maxVal+1];
			
			//read pixel values from input file
			while(line.hasNext()){
				input = line.nextInt();
				hist[input]++;	
			}
			line.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		//write to output file
		try {
			BufferedWriter outputLine = new BufferedWriter(new FileWriter(outputFile));
			outputLine.write(numRows + " " + numCols + " " + minVal + " "+ maxVal); //header information
			outputLine.newLine();
			for(int i=0; i<=maxVal; i++){
				outputLine.write(i + " " + hist[i]); //histogram pixel data
				outputLine.newLine();
			}
			outputLine.close();	
		} catch (IOException e) {
			e.printStackTrace();
		}
	}//end computeHistogram method
} //end class

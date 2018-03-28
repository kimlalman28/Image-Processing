import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class ConnectedComponent {
	
	/*
	 * Kimberly Lalmansingh
	 * IP_31 
	 * 8 Connected Component
	 */

	private static int numRows, numCols, minVal, maxVal, newMin, newMax, newLabel=0;
	private static int[][] zeroFramedAry;
	private static int[] NeighborAry;
	private static int[] EQAry;
	private static Property[] Prop;
	
	//Property class to store information about property file
	private static class Property {
		private int label;
		private int numPixels;
		private int minRow, minCol, maxRow, maxCol;
		
		public Property(){ //constructor
			label = 0;
			numPixels = 0;
			minRow = 99999; //large min and max allow to check if min is 0
			minCol = 99999;
			maxRow = 0;
			maxCol = 0;
		}
		//getters and setters
		public int getLabel() { return label; }
		public void setLabel(int l){label = l; }
		
		public int getNumPixels() { return numPixels; }
		public void setNumPixels(int nP){numPixels = nP; }
		
		public int getMinRow() { return minRow; }
		public void setMinRow(int mR){minRow = mR; }
		
		public int getMinCol() { return minCol; }
		public void setMinCol(int mC){minCol = mC; }
		
		public int getMaxRow() { return maxRow; }
		public void setMaxRow(int mR){maxRow = mR; }
		
		public int getMaxCol() { return maxCol; }
		public void setMaxCol(int mC){maxCol = mC; }
		
	}
	
	public static void main(String args[]){
		File inFile = new File(args[0]); //input file
		try{
			BufferedWriter output1 = new BufferedWriter(new FileWriter(args[1])); //outfile1
			loadImage(inFile); //read image
			zeroFramed(); //zero frame image
			//allocate EQAry & initialize 
			EQAry = new int[(numRows*numCols)/2];
			for(int i=0; i<EQAry.length; i++){
				EQAry[i] = i;
			}
			//process each pixel > 0 for pass 1
			for(int i=1; i<=numRows; i++){
				for(int j=1; j<=numCols; j++){
					if(zeroFramedAry[i][j]>0){
						loadNeighbors(i, j, "pass1");
						ConnectCC_Pass1(i, j);
					}
				}
			}
			prettyPrint(output1, "Pass 1");
			
			//process each pixel > 0 for pass 2
			for(int i=numRows; i>0; i--){
				for(int j=numCols; j>0; j--){
					if(zeroFramedAry[i][j]>0){
						loadNeighbors(i, j, "pass2");
						ConnectCC_Pass2(i, j);
					}
				}
			}
			prettyPrint(output1, "Pass 2");
			
			manageEQAry(); //find out true number of connected components
		    output1.write("Below is the EQary after ManageEQAry" + "\n");
		    for(int i=0; i<=newLabel; i++){
		    	output1.write(EQAry[i] + " ");
		    }
		    output1.write("\n\n");
			
		  //process each pixel > 0 for pass 3
			for(int i=1; i<=numRows; i++){
				for(int j=1; j<=numCols; j++){
					if(zeroFramedAry[i][j]>0){
						ConnectCC_Pass3(i, j);
					}
				}
			}
			//set values of bounding box with respect to image file
			for(int i=0; i<Prop.length; i++){
				Prop[i].setMinRow(Prop[i].getMinRow()-1);
				Prop[i].setMinCol(Prop[i].getMinCol()-1);
				Prop[i].setMaxRow(Prop[i].getMaxRow()-1);
				Prop[i].setMaxCol(Prop[i].getMaxCol()-1);	
			}
			prettyPrint(output1, "Pass 3");
			
			//outfile2 image creation using zeroFramedAry from pass 3
			BufferedWriter output2 = new BufferedWriter(new FileWriter(args[2]));
			output2.write(numRows + " " + numCols + " " + newMin + " " + newMax + "\n");
			for(int i=1; i <=numRows; i++){
				for(int j=1; j<=numCols; j++){
					output2.write(zeroFramedAry[i][j] + " ");
				}
				output2.newLine();
			}

			//outfile3 Property file
			BufferedWriter output3 = new BufferedWriter(new FileWriter(args[3]));
			printCCProperty(output3);
			//close output files
		    output1.close();
		    output2.close();
		    output3.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}
	
	public static void zeroFramed(){
		//top and bottom borders of zero frame
		for(int i=1; i<=numCols; i++){
			zeroFramedAry[0][i] = 0;
			zeroFramedAry[numRows+1][i] = 0;
			}
		//left and right borders of zero frame and corners
		for(int i=0; i<=numRows+1; i++){
			zeroFramedAry[i][0] = 0;
			zeroFramedAry[i][numCols+1] = 0;
			}
	}
	
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
			//Allocate array 
			zeroFramedAry = new int[numRows+2][numCols+2];
			//read pixel values from input file
			for(int i=1; i<=numRows; i++){
				for(int j=1; j<=numCols; j++){
					input = line.nextInt();
					zeroFramedAry[i][j] = input;
				}
			}
			line.close();
		} 
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	
	public static void loadNeighbors(int i, int j, String pass){
		NeighborAry = new int[5];
		//for pass 1 neighbors
		if(pass.equals("pass1")){
			NeighborAry[0] = zeroFramedAry[i-1][j-1]; //a
			NeighborAry[1] = zeroFramedAry[i-1][j]; //b
			NeighborAry[2] = zeroFramedAry[i-1][j+1]; //c
			NeighborAry[3] = zeroFramedAry[i][j-1]; //d
		}
		//for pass 2 neighbors
		else{
			NeighborAry[0] = zeroFramedAry[i][j+1]; //m
			NeighborAry[1] = zeroFramedAry[i+1][j-1]; //n
			NeighborAry[2] = zeroFramedAry[i+1][j]; //o
			NeighborAry[3] = zeroFramedAry[i+1][j+1]; //p
		}
		NeighborAry[4] = zeroFramedAry[i][j]; //p(i, j)
	}
	
	public static void ConnectCC_Pass1(int i, int j){
		//case 1 - if all neighbors = 0
		if((NeighborAry[0]==0) && (NeighborAry[1]==0) && (NeighborAry[2]==0) && (NeighborAry[3]==0)){
			newLabel++;
			zeroFramedAry[i][j] = newLabel;
		}
		
		//case 2 - some of p(i,j) neighbors are labeled and are the same
		else if((NeighborAry[0]>0) || (NeighborAry[1]>0) || (NeighborAry[2]>0) || (NeighborAry[3]>0)){
			int neighborLabel = 0;
			boolean case2 = true;
			//check if labels of neighbor are the same
			for(int m=0; m<NeighborAry.length-1; m++){
				if(NeighborAry[m]==0) continue;
				neighborLabel = NeighborAry[m];
				for(int n=m+1; n<NeighborAry.length-1; n++){
					if(NeighborAry[n]==0) continue;
					//if they are neighbors are not labeled the same, this is not case 2
					else if(NeighborAry[n] != neighborLabel){
						case2=false;
					}
				}
				break;
			}
			if(case2) {
				zeroFramedAry[i][j] = neighborLabel;
			}
			//case 3 - some of p(i,j) neighbors are labeled but are not the same
			else{
				int min = 0;
				//find min of the labels
				for(int m=0; m<NeighborAry.length-1; m++){
					if(NeighborAry[m]==0) continue;
					min = NeighborAry[m];
					for(int n=m+1; n<NeighborAry.length-1; n++){
						if(NeighborAry[n]==0) continue;
						else if(NeighborAry[n] < min) min = NeighborAry[n];
					}
					break;
				}
				updateEQAry(min); //any label in the neighborAry that is >min needs to be updated in EQAry
				zeroFramedAry[i][j] = min;
			}
		}
	}
	
	public static void ConnectCC_Pass2(int i, int j){
		//case 1 - neighbors = 0, do nothing
		if((NeighborAry[0]==0) && (NeighborAry[1]==0) && (NeighborAry[2]==0) && (NeighborAry[3]==0)) return;
		
		//case 2 - neighbors AND p(i,j) are same label, do nothing
		else if((NeighborAry[0]>0) || (NeighborAry[1]>0) || (NeighborAry[2]>0) || (NeighborAry[3]>0)){
			int neighborLabel = 0;
			boolean case2 = true;
			for(int m=0; m<NeighborAry.length-1; m++){
				if(NeighborAry[m]==0) continue;
				neighborLabel = NeighborAry[m];
				for(int n=m+1; n<NeighborAry.length; n++){
					if(NeighborAry[n]==0) continue;
					//if labels of p(i,j) and/or neighbors are different, it is case 3
					else if(NeighborAry[n] != neighborLabel) case2=false;
				}
				break;
			}
			if(case2) return;
			//case 3 - neighbors and/or p(i,j) have different labels
			else{
				int min = 0;
				//find min of labels
				for(int m=0; m<NeighborAry.length-1; m++){
					if(NeighborAry[m]==0) continue;
					min = NeighborAry[m];
					for(int n=m+1; n<NeighborAry.length-1; n++){
						if(NeighborAry[n]==0) continue;
						else if(NeighborAry[n] < min) min = NeighborAry[n];
					}
					break;
				}
				updateEQAry(min); //any label in the neighborAry that is >min needs to be updated in EQAry
				zeroFramedAry[i][j] = min;
			}
		}
	}
	
	public static void ConnectCC_Pass3(int i, int j){
		//use EQAry to update zeroFramedAry 
		zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
		int label = zeroFramedAry[i][j];
		//update header values
		if(label < minVal) newMin = label;
		if(label > maxVal) newMax = label;
		//set property information
		if(Prop[label-1]==null){
			//if new component
			Prop[label-1] = new Property();
			Prop[label-1].setLabel(label);
		}
		Prop[label-1].setNumPixels(Prop[label-1].getNumPixels()+1); //set numPixels
		//set bounding box pixel row and col
		if(i<Prop[label-1].getMinRow()){
			Prop[label-1].setMinRow(i);
		}
		if(j<Prop[label-1].getMinCol()){
			Prop[label-1].setMinCol(j);
		}
		if(i>Prop[label-1].getMaxRow()){
			Prop[label-1].setMaxRow(i);
		}
		if(j>Prop[label-1].getMaxCol()){
			Prop[label-1].setMaxCol(j);
		}
	}
	
	public static void updateEQAry(int min){
		//updating EQAry labeling
		for(int i=0; i<NeighborAry.length-1; i++){
			if(NeighborAry[i]==0) continue;
			else if(NeighborAry[i] > min){
				EQAry[NeighborAry[i]] = min;
			}
		}
	}
	
	public static void manageEQAry(){
		//find out true number of connected components
		int c=0;
		for(int i=1; i<=newLabel; i++){
			if(EQAry[i]==i){
				EQAry[i] = ++c;
			}
			else{
				EQAry[i] = EQAry[EQAry[i]];
			}
		}
		//set size of property Ary = # of components
		Prop = new Property[c];
	}
	
	public static void printCCProperty(BufferedWriter output3){
		//output property file
		try {
			output3.write(numRows + " " + numCols + " " + newMin + " " + newMax + "_______CC image header");
			output3.newLine();
			output3.write(Prop.length + "_______Number of Components \n");
			for(int i=0; i<Prop.length; i++){
				output3.newLine();
				output3.write(Prop[i].getLabel() + "______CC label #" + (i+1));
				output3.newLine();
				output3.write(Prop[i].getNumPixels() + "_______Number of Pixels in CC #" + (i+1));
				output3.newLine();
				output3.write(Prop[i].getMinRow() + " " + Prop[i].getMinCol() + "_____upper left most pixel of the bounding box of CC #" + (i+1));
				output3.newLine();
				output3.write(Prop[i].getMaxRow() + " " + Prop[i].getMaxCol() + "_____lower right most pixel of the bounding box of CC #" + (i+1) + "\n");
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
	
	public static void prettyPrint(BufferedWriter output1, String pass){
		//output pretty print of image
		try{
		    output1.write("Below is the result after " + pass + "\n");
		    for(int i=1; i<=numRows; i++){
		        for(int j=1; j<=numCols; j++){
		            if(zeroFramedAry[i][j] > 0)
							output1.write(zeroFramedAry[i][j] + " ");
					else output1.write("  ");
		        }
		        output1.newLine();
		    }
		    output1.newLine();
		    
		    //output EQAry after each pass
		    output1.write("Below is the EQary after " + pass + "\n");
		    for(int i=0; i<=newLabel; i++){
		    	output1.write(EQAry[i] + " ");
		    }
		    output1.write("\n\n");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
}

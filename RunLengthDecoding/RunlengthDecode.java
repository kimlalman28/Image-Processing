import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class RunlengthDecode {

	private static int numRows, numCols, minVal, maxVal;
	private static int[][] imgAry;
	private static RunlengthDecode RLD;
	private static BufferedWriter output;
	private static Scanner line;
	
	public RunlengthDecode(int nR, int nC, int minV, int maxV){
		//constructor
		numRows = nR;
		numCols = nC;
		minVal = minV;
		maxVal = maxV;
		imgAry = new int[numRows][numCols];
		for(int i=0; i<numRows; i++){
			for(int j=0; j<numCols; j++){
				imgAry[i][j] = 0;
			}
		}
	}
	
	//getters and setters
	public int getRows(){ return numRows;}
	public void setRows(int r){ numRows = r;}
	
	public int getCols(){ return numCols;}
	public void setCols(int c){ numCols = c;}
	
	public int getMin(){ return minVal;}
	public void setMin(int min){ minVal = min;}
	
	public int getMax(){ return maxVal;}
	public void setMax(int max){ maxVal = max;}
	
	
	public static void main(String[] args) {
		File inFile = new File(args[0]); //input file
		try {
			line = new Scanner(inFile);
			try {
				output = new BufferedWriter(new FileWriter(args[1])); //outputfile
				int input=0, rows=0, cols=0, min=0, max=0;
				//information about image header
				for(int i=0; i<4; i++){
					input = line.nextInt();
					if(i==0) rows = input;
					else if(i==1) cols = input;
					else if (i==2) min = input;
					else max = input;
				}	
				
				RLD = new RunlengthDecode(rows, cols, min, max); //runlength decode object
				output.write(RLD.getRows() +" " + RLD.getCols() + " " + RLD.getMin() + " " + RLD.getMax() + "\n"); //header to outfile
				int chosenMethod = line.nextInt(); //get chosen method from line 2 of file
				if(chosenMethod>4 || chosenMethod<1){ 
					System.out.println("Your input was not one of the options. Exiting...");
					return;
				}
				else {
					if(chosenMethod==1) RLD.method1(inFile, line);
					else if(chosenMethod==2) RLD.method2(inFile, line);
					else if(chosenMethod==3) RLD.method3(inFile, line);
					else if(chosenMethod==4) RLD.method4(inFile, line);
					line.close();
					RLD.outputImage(output);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} //buffered writer try/catch
		} 
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}//scanner writer try/catch
	}
	
	public void method1(File inFile, Scanner value){
		//decode only non-zeros and no wrapped around
		int row=0, col=0, greyscale=0, num=0;
		while(value.hasNext()){
			//get the 4 numbers of run representation
			row = value.nextInt();
			col= value.nextInt();
			greyscale = value.nextInt();
			num = value.nextInt();
			while(num != 0){ //while number of those pixels is not = 0
				imgAry[row][col] = greyscale; //given start row and col of array
				num--; 
				col++; //continue placing pixel in array
			}
		}
	}
	
	public void method2(File inFile, Scanner value){
		//decode only non-zeros with wrapped around
		int row=0, col=0, greyscale=0, num=0;
		while(value.hasNext()){
			//get the 4 numbers of run representation
			row = value.nextInt();
			col= value.nextInt();
			greyscale = value.nextInt();
			num = value.nextInt();
			while(num != 0){
				imgAry[row][col] = greyscale;
				num--;
				col++;
				//because of wrapped around, we must check if we have
				//reached the end of a row and need to go to the next row
				if(col == RLD.getCols()){
					row++;
					col = 0;
				}
			}
		}
	}
	
	public void method3(File inFile, Scanner value){
		//decode zeros and non-zeros without wrapped around
		int row=0, col=0, greyscale=0, num=0;
		while(value.hasNext()){
			//get the 4 numbers of run representation
			row = value.nextInt();
			col= value.nextInt();
			greyscale = value.nextInt();
			num = value.nextInt();
			while(num != 0){
				imgAry[row][col] = greyscale;
				num--;
				col++;
			}
		}
	}
	
	public void method4(File inFile, Scanner value){
		int row=0, col=0, greyscale=0, num=0;
		while(value.hasNext()){
			//get the 4 numbers of run representation
			row = value.nextInt();
			col= value.nextInt();
			greyscale = value.nextInt();
			num = value.nextInt();
			while(num != 0){
				imgAry[row][col] = greyscale;
				num--;
				col++;
				//because of wrapped around, we must check if we have
				//reached the end of a row and need to go to the next row
				if(col == RLD.getCols()){
					row++;
					col = 0;
				}
			}
		}
	}
	
	public void outputImage(BufferedWriter image){
		//output the final array to output file
		try {
			for(int i=0; i<RLD.getRows(); i++){
				for(int j=0; j<RLD.getCols(); j++){
					image.write(imgAry[i][j] + " ");
				}
				image.write("\n");
			}
			image.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class RunlengthEncode {

	private static int numRows, numCols, minVal, maxVal;
	private static RunlengthEncode RLE;
	private static BufferedWriter output;
	private static Scanner line;
	
	public RunlengthEncode(int nR, int nC, int minV, int maxV){
		numRows = nR;
		numCols = nC;
		minVal = minV;
		maxVal = maxV;
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
				
				RLE = new RunlengthEncode(rows, cols, min, max); //runlength encode object
				output.write(RLE.getRows() +" " + RLE.getCols() + " " + RLE.minVal + " " + RLE.maxVal + "\n"); //header to outfile
				Scanner method = new Scanner(System.in); //ask user for a runlength encode method to run
				int chosenMethod = 0;
				System.out.println("Please choose a RunlengthEncoding Method by entering a number: ");
				System.out.println("(1) Method 1 Encode without zero and no wrap-around.");
				System.out.println("(2) Method 2 Encode without zero and wrap-around.");
				System.out.println("(3) Method 3 Encode with zero and no wrap-around. ");
				System.out.println("(4) Method 4 Encode with zero and wrap-around.");
				
				chosenMethod = method.nextInt();
				if(chosenMethod>4 || chosenMethod<1){ 
					System.out.println("Your input was not one of the options. Exiting...");
					return;
				}
				else {
					output.write(chosenMethod + "\n");
					if(chosenMethod==1) RLE.method1(inFile, output, line);
					else if(chosenMethod==2) RLE.method2(inFile, output, line);
					else if(chosenMethod==3) RLE.method3(inFile, output, line);
					else if(chosenMethod==4) RLE.method4(inFile, output, line);
					line.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			} //buffered writer try/catch
		} 
		catch (FileNotFoundException e) {
			e.printStackTrace();
		}//scanner writer try/catch
	}
	
	public void method1(File inFile, BufferedWriter output, Scanner value){
		//encode without zero and no wrap around
		int r=0, c=0, count=0, currVal=0, nextVal=0;
		try {
			while(r<RLE.getRows()){ //while we have not gone through every row of the file
				c=0;
				count=0; 
				currVal = value.nextInt(); //get next value
				//if there is a long chain of 0s, handle skipping them here
				while(currVal == 0){
					c++;
					currVal = value.nextInt();
					if(c==RLE.getCols()-1){ //in case string of 0s reaches the end of the row
						r++; //next row
						c=-1; //set to -1 b/c entering while loops will increment to 0
					}
				}
			output.write(r +" "+ c +" "+ currVal+" "); //found non zero value, output to outfile
			count++;
			
			while(c<RLE.getCols()-1){ //loop to go through each column in row of non-zero
				c++;
				nextVal = value.nextInt(); //get next value
				//if next value is 0, handle skipping it here
				if(nextVal == 0){
					currVal = 0;
					continue;
				}
				//if current value and next value are equal
				else if(nextVal == currVal) count++;
				//if the current value and next value are different and the next value is a nonzero
				else{
					output.write(count + "\n"); //output previous pixel values count
					currVal = nextVal; //set current value to value recently read in
					count = 1;
					output.write(r +" "+ c +" "+ currVal+" ");
				}
			}
			//if run of values reaches the end of the row, output the count and go to next row
			output.write(count + "\n");
			r++;
		}
		output.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	
	public void method2(File inFile, BufferedWriter output, Scanner value) {
		//encode without zeros and with wrap around
		int r=0, c=0, count=0, currVal=0, nextVal=0;
		try {
			currVal = value.nextInt(); //get value from file
			while(currVal == 0){ //if file starts with a run of 0's, handle it here
				c++;
				currVal = value.nextInt(); //check next value 
				//if run of 0's reaches the end of the row, go to next row
				if(c==RLE.getCols()-1){
					r++;
					c=-1;
				}
			}
			output.write(r +" "+ c +" "+ currVal+" ");
			count++;
			
			while(c!=RLE.getCols()-1){ //while we haven't reached the end of the row
				c++;
				nextVal = value.nextInt(); //get the next value in the row

				if(nextVal == 0){ // if the next value is 0
					currVal = 0; // set current value = 0
					if(c==RLE.getCols()-1){ // we will check if we have reached the end of the row
						r++; //go to next row
						c=-1; //beginning col
					}
					if(r==RLE.getRows()){ // if we have reached the end of the last row
						output.write(count + "\n"); //output the count of the previous non-zero value
						break; //end the loop
					}
					continue; //so, if we read in a zero, just continue the loop after running previous tests
				}
				else if(nextVal == currVal) count++; // if previous value and next value are the same, increment count
				else{ //new run begins
					output.write(count + "\n");
					currVal = nextVal;
					count = 1;
					output.write(r +" "+ c +" "+ currVal+" ");
				}
				if(c==RLE.getCols()-1){ //check if end of row, since wrapped around 
					r++; //go to next row
					c=-1; //go to beginning of row
				}
				if(r==RLE.getRows()){ //check if we've reached the end of the file
					output.write(count + "\n");
					break;
				}
			}
			output.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void method3(File inFile, BufferedWriter output, Scanner value) {
		//encode with zero and no wrap around
		int r=0, c=0, count=0, currVal=0, nextVal=0;
		try {
			while(r<RLE.getRows()){ //while we haven't processed all pixels
				c=0;
				count=0; 
				currVal = value.nextInt(); //get next value
			output.write(r +" "+ c +" "+ currVal+" ");
			count++;
			
			while(c<RLE.getCols()-1){ //while we haven't reached the end of a row
				c++; 
				nextVal = value.nextInt();
				if(nextVal == currVal) count++; //check if previous value and current value are the same
				else{ // if not, a new run begins
					output.write(count + "\n");
					currVal = nextVal;
					count = 1;
					output.write(r +" "+ c +" "+ currVal+" ");
				}
			}
			output.write(count + "\n"); // if we reached the end of the column, output the count for the run
			r++; //go to next row
		}
		output.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void method4(File inFile, BufferedWriter output, Scanner value){
		//encode with zeros and wrap around
		int r=0, c=0, count=0, currVal=0, nextVal=0; 
		try {
			currVal = value.nextInt(); //get next value
			output.write(r +" "+ c +" "+ currVal+" ");
			count++;
			while(c!=RLE.getCols()-1){ //while we haven't reached the end of the row
				c++;
				nextVal = value.nextInt(); //get next value
				if(nextVal == currVal) count++; //check if the current value and next value are the same
				else{ //if not, new run begins
					output.write(count + "\n");
					currVal = nextVal;
					count = 1;
					output.write(r +" "+ c +" "+ currVal+" ");
				} 
				if(c==RLE.getCols()-1){ // check if the end of the column, since wrapped around
					r++; //go to next row
					c=-1; //beginning of row
				}
				if(r==RLE.getRows()){ //check if we have processed all rows
					output.write(count + "\n");
					break;
				}
			}
			output.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}

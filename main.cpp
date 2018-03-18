#include "mbed.h"
#include "math.h"

DigitalOut shiftBitA(p30); //pin 30 on board to pin 1 on chip
DigitalOut clk(p27);	//pin 27 on board to clocks of chips
DigitalOut reset(p28);	//pin 28 on board to clears of chips
DigitalIn  one(p5);
DigitalIn  two(p6);
DigitalIn  three(p7);
DigitalIn  four(p8);
DigitalOut  five(p23);
DigitalOut  six(p24);
DigitalOut  seven(p25);
DigitalOut  eight(p26);

I2C tempSense(p32,p31);


void outputNumber(int num,bool pm);//function to send bits corresponding to the number num
void CLK();//function to set clock low then high with 2 nanoseconds between
int tempDecode(int Data);
void tempDisplay(int Number);
int padScan();
int rowOne();
int rowTwo();
int rowThree();
int rowFour();
void Add();
void Sub();
void AND();
void ORR();
void Clear();
void Compute(int OP, int onesA1, int tensA1, int onesA2, int tensA2);
void calculator();
void inNum();
#define T true;
#define F false;
int keyPressed;
float setupTime;
bool firstArg, secArg, Operator = F;
int onesA1, tensA1, onesA2, tensA2, OP, tmp = NULL;
int seconds, minutes, hours, day, date, month, year;
int Temp;
int main() {
	setupTime = .05;
	one.mode(PullNone);
	two.mode(PullNone);
	three.mode(PullNone);
	four.mode(PullNone);
	reset = 0;//reset the chips
	wait(.001);
	reset = 1;
	outputNumber(0,0);
	outputNumber(0,0);
	outputNumber(0,0);
	//configure real time clock
	tempSense.start();
	tempSense.write(0xD0);
	tempSense.write(0x00);
	tempSense.write(0x00);
	tempSense.stop();
	
	tempSense.start();
	tempSense.write(0xD0);
	tempSense.write(0x00);
	tempSense.write(0x00);//sec
	tempSense.write(0x30);//min
	tempSense.write(0x61);//hr
	tempSense.write(0x04);//day
	tempSense.write(0x10);//date
	tempSense.write(0x11);//month
	tempSense.write(0x16);//year
	tempSense.start();
	tempSense.write(0xD0);
	tempSense.write(0x0e);
	tempSense.write(0x20);
	tempSense.write(0x00);
	tempSense.stop();

	
	//configure temperature sensor
	tempSense.start();
	tempSense.write(0x9E);
	tempSense.write(0xAC);
	tempSense.write(2);
	tempSense.stop();
	
	tempSense.start();
	tempSense.write(0x9E);
	tempSense.write(0x51);
	tempSense.stop();
	tempSense.start();
	tempSense.write(0x9E);
	tempSense.write(0xAA);
	
	while(1){
		Temp = 0;
		tempSense.start();
		tempSense.write(0x9E);
		tempSense.write(0xAA);
		tempSense.start();
		tempSense.write(0x9F);
		Temp = tempDecode(tempSense.read(0));
		
		tempSense.stop();
		tempDisplay(Temp);
		wait(2);
			
	//read time
	tempSense.start();
	tempSense.write(0xD0);
	tempSense.write(0x00);
	tempSense.start();
	tempSense.write(0xD1);
	seconds = tempSense.read(1);
	minutes = tempSense.read(1);
	hours = tempSense.read(1);
	day = tempSense.read(1);
	date = tempSense.read(1);
	month = tempSense.read(1);
	year = tempSense.read(0);
	if((hours&0x20) == 0x20){
		outputNumber(minutes&0x0F,1);
	}
	else{
		outputNumber(minutes&0x0F,0);
	}
	
	outputNumber(minutes>>4,0);
	outputNumber(hours&0x0F,0);
	wait(2);
	tempSense.stop();
		
		
		/*calculator();*/
	
		
		
	/*
	for(int i = 0; i < 10; i++){ // loop for most significant digit
		for(int x = 0; x < 10; x++){// loop for middle digit
			for(int z = 0; z < 10; z++){// loop for least significant digit
				outputNumber(z);//output the least significant digit
				outputNumber(x);//output the middle digit
				outputNumber(i);//output the most significant digit
				wait(0.05);//wait 5/100ths of a second
			}//for3
		}//for2
	}//for
	outputNumber(77);//output dot three times
	outputNumber(77);
	outputNumber(77);
	*/
	//wait(0.5);//wait 1/2 second
	
	
	
	
	}//while
}//main
void tempDisplay(int Number){
	int huns, tens, ones, temp;
	huns = Number/100;
	Number = Number - huns*100;
	tens = Number/10;
	Number = Number - tens*10;
	ones = Number/1;
	outputNumber(ones,0);
	outputNumber(tens,0);
	outputNumber(huns,0);
}//tempDisplay
int tempDecode(int Data){
	Data = Data & 0x00FF;
	//Data = Data >> 8;
	/*int bit;
	int temp = 0;
	bit = 0x4000;
	bit = Data & bit;
	if(bit > 0){
		temp = temp + 2^6;}
	bit = 0x2000;
	bit = Data & bit;
	if(bit > 0){
		temp = temp + 2^5;}
	bit = 0x1000;
	bit = Data & bit;
	if(bit > 0){
		temp = temp + 2^4;}
	bit = 0x0800;
	bit = Data & bit;
	if(bit > 0){
		temp = temp + 2^3;}
	bit = 0x0400;
	bit = Data & bit;
	if(bit > 0){
		temp = temp +2^2;}
	bit = 0x0200;
	bit = Data & bit;
	if(bit > 0){
		temp = temp + 2^1;}
	bit = 0x0100;
	bit = Data & bit;
	if(bit > 0)
		temp = temp + 2^0;*/
	return(Data);
}//Data
void Add(){
}//Add
void Sub(){
}//Sub
void AND(){
}//AND
void ORR(){
}//ORR
void Clear(){
}
void Compute(int OP, int onesA1, int tensA1, int onesA2, int tensA2){
	int arg1,arg2,result,resHun,resTen;
	arg1 = (tensA1 * 10) + onesA1;
	arg2 = (tensA2 * 10) + onesA2;
	switch(OP){
		case 10:
			result = arg1 + arg2;
			resHun = result/100;
			result = result - resHun*100;
			resTen = result/10;
			result = result - resTen*10;
			outputNumber(result,0);
			outputNumber(resTen,0);
			outputNumber(resHun,0);
			break;
		case 11:
			result = arg1 - arg2;
			resHun = result/100;
			result = result - resHun*100;
			resTen = result/10;
			result = result - resTen*10;
			outputNumber(result,0);
			outputNumber(resTen,0);
			outputNumber(resHun,0);
			break;
		case 12:
			result = arg1 & arg2;
			resHun = result/100;
			result = result - resHun*100;
			resTen = result/10;
			result = result - resTen*10;
			outputNumber(result,0);
			outputNumber(resTen,0);
			outputNumber(resHun,0);
			break;
		case 13:
			result = arg1 | arg2;
			resHun = result/100;
			result = result - resHun*100;
			resTen = result/10;
			result = result - resTen*10;
			outputNumber(result,0);
			outputNumber(resTen,0);
			outputNumber(resHun,0);
			break;
	}
}

void inNum(){
	if(firstArg == 0 ){
				if(tensA1 == NULL){
						tensA1 = tmp;
						outputNumber(tensA1,0);
						outputNumber(0,0);
						outputNumber(0,0);
				}//if
				else if(onesA1 == NULL){
						onesA1 = tmp;
						outputNumber(onesA1,0);
						outputNumber(tensA1,0);
						outputNumber(0,0);
						firstArg = 1;
				}
	}
	else if(secArg == 0){
						if(tensA2 == NULL){
							tensA2 = tmp;
							outputNumber(tensA2,0);
							outputNumber(0,0);
							outputNumber(0,0);
						}//if
						else if(onesA2 == NULL){
							onesA2 = tmp;
							outputNumber(onesA2,0);
							outputNumber(tensA2,0);
							outputNumber(0,0);
							secArg = 1;
						}
	}
}
void inOP(){
	if(tmp == 15){
		firstArg = 0;
		secArg = 0;
		Operator = 0;
		onesA1 = NULL;
		tensA1 = NULL;
		onesA2 = NULL;
		tensA2 = NULL;
		OP = NULL;
		outputNumber(0,0);
		outputNumber(0,0);
		outputNumber(0,0);
		}//if
	if((OP == NULL) && (tmp != 15)){
		OP = tmp;
		Operator = 1;
		outputNumber(0,0);
		outputNumber(0,0);
		outputNumber(0,0);
	}
	if((firstArg == 0) && (tmp != 15)){
		onesA1 = tensA1;
		tensA1 = 0;
		firstArg = 1;
	}
	if((Operator == 1) && (secArg == 0) && (tmp == 14)){
		onesA2 = tensA2;
		tensA2 = 0;
		secArg = 1;
	}
	if((firstArg == 1) && (secArg == 1) && (Operator == 1) && (tmp == 14)){
			Compute(OP, onesA1, tensA1, onesA2, tensA2);
		}//if
}
void calculator(){
	while(1){
		tmp = NULL;
		tmp = padScan();
		if(tmp != NULL){
			switch(tmp){
				case 99:
					tmp = 0;
					inNum();
					break;
				case 1:
					inNum();
					break;
				case 2:
					inNum();
					break;
				case 3:
					inNum();
					break;
				case 4:
					inNum();
					break;
				case 5:
					inNum();
					break;
				case 6:
					inNum();
					break;
				case 7:
					inNum();
					break;
				case 8:
					inNum();
					break;
				case 9:
					inNum();
					break;
				case 10:
					inOP();
					break;
				case 11:
					inOP();
					break;
				case 12:
					inOP();
					break;
				case 13:
					inOP();
					break;
				case 14:
					inOP();
					break;
				case 15:
					inOP();
					break;
			}//switch
		}
	}

}//calculator


int padScan(){
	int retVal;
	retVal = rowOne();
	if(retVal != NULL){
		//outputNumber(77);
		return(retVal);
	}
	retVal = rowTwo();
	if(retVal != NULL){
		return(retVal);
	}
	retVal = rowThree();
	if(retVal != NULL){
		return(retVal);
	}
	retVal = rowFour();
	if(retVal != NULL){
		return(retVal);
	}
	return(NULL);
}//padMon

int rowOne(){
		int One,Two,Three,Four;
	five.write(0);
	six.write(1);
	seven.write(1);
	eight.write(1);
	wait(setupTime);
	One = one.read();
	Two = two.read();
	Three = three.read();
	Four = four.read();
	if(One || Two || Three || Four == 0){
		if(One == 0){
			return(1);
			//key 1
		}
		if(Two == 0){
			return(2);
			//key 2
		}
		if(Three == 0){
			return(3);
			//key 3
		}
		if(Four == 0){
			return(10);
			//key A
		}
	}
	return(NULL);
}
int rowTwo(){
		int One,Two,Three,Four;
	five.write(1);
	six.write(0);
	seven.write(1);
	eight.write(1);
	wait(setupTime);
	One = one.read();
	Two = two.read();
	Three = three.read();
	Four = four.read();
	if(One || Two || Three || Four == 0){
		if(One == 0){
			//key 1
			return(4);
		}
		if(Two == 0){
			//key 2
			return(5);
		}
		if(Three == 0){
			//key 3
			return(6);
		}
		if(Four == 0){
			//key B
			return(11);
		}
	}
	return(NULL);
}
int rowThree(){
		int One,Two,Three,Four;
	five.write(1);
	six.write(1);
	seven.write(0);
	eight.write(1);
	wait(setupTime);
	One = one.read();
	Two = two.read();
	Three = three.read();
	Four = four.read();
	if(One || Two || Three || Four == 0){
		if(One == 0){
			return(7);
			//key 4
		}
		if(Two == 0){
			return(8);
			//key 5
		}
		if(Three == 0){
			return(9);
			//key 6
		}
		if(Four == 0){
			return(12);
			//key B
		}
	}
	return(NULL);
}
int rowFour(){
		int One,Two,Three,Four;
	five.write(1);
	six.write(1);
	seven.write(1);
	eight.write(0);
	wait(setupTime);
	One = one.read();
	Two = two.read();
	Three = three.read();
	Four = four.read();
	if(One || Two || Three || Four == 0){
		if(One == 0){
			return(99);
			//key 7
		}
		if(Two == 0){
			return(15);
			//key 8
		}
		if(Three == 0){
			return(14);
			//key 9
		}
		if(Four == 0){
			return(13);
			//key C
		}
	}
	return(NULL);
}

void CLK(){
			clk =F;// clock low
			wait(.00000002);
			clk =T;//clock high
}//CLK
/* function outputNumber
*parameter: int num 
*return: void
*abstract: sending the 0 through 9 as num causes this function to output the bits with clocks between to display that number, 
*					sending 77 displays the dot.  The bits are output, last bit of the display goes out first, first bit of the display
*					goes out last.  This results in the bits needed being in the right order when they reach the display since this is
*					implemented with shift registers. 
*					EXAMPLE:  to get the dot to display you call outputNumber(77);  The function will then output F T T T T T T T with 
*										a clock cycle between, and since we have common anode displays a low(F) corresponds to turning the LED on
*										this results in a F at bin H of the SSD(seven segment display) and T at pins A through G.
*/				
void outputNumber(int num,bool pm){
	if(pm ==true){
	switch(num){
		case 0:
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 1:
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			break;
		case 2:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 3:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 4:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			break;
		case 5:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 6:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 7:
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 8:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 9:
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 77:
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			break;
		default:
			break;
	}//switch
}//if
	else{
		switch(num){
		case 0:
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 1:
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			break;
		case 2:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 3:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 4:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			break;
		case 5:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 6:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 7:
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 8:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 9:
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			shiftBitA = F;
			CLK();
			break;
		case 77:
			shiftBitA = F;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			shiftBitA = T;
			CLK();
			break;
		default:
			break;
	}//switch
	}//else
}//outputNumber
#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <unistd.h>
#define MODFREQ 38000
#define TICK (1/50)
void transmit(unsigned int data[],int size);
void append(unsigned int data[], int s, int e,int v);
void setarray(unsigned int data[],int s, int e,int input[]);
int duration(char str[]);
int volume(char str[]);
void binaryconverter(int sec, int temp[]);
void printtest(unsigned int data[],int s,int e);
int main(void){

	while (1){	
		char s = 'A';
		char dur[256];
		char vol[] = "55";
		printf("Enter a Note(A - G) in all caps:");
		//scanf(" %c", &s);
		printf("%c \n", s);
		printf("Enter a duration(x:xx):");
		scanf("%s", &dur);
		printf("%s \n", dur);
		printf("Enter Volume:");
		//scanf("%s",&vol);
		printf("%s \n",vol);
		unsigned int data[428] = {0};
		int size = sizeof(data)/sizeof(int);
		append(data,0,80,1);
		append(data,116,196,1);
		append(data,232,312,1);
		append(data,348,428,1);
	
		if(s == 'A'){
			
			int noteA[] = {0,1,1,0,0,1,0,1};
			setarray(data,80,112,noteA);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);
		}
		else if (s == 'B'){
			int noteB[] = {0,0,0,1,1,0,1,0};
			setarray(data,80,112,noteB);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);
		}
		else if (s == 'C'){
			int noteC[] = {0,1,0,1,1,0,1,1};
			setarray(data,80,112,noteC);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);
		}
		else if (s == 'D'){
			int noteD[] = {0,0,1,0,0,1,0,0};
			setarray(data,80,112,noteD);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);
		}
		else if (s == 'E'){
			
			int noteE[] = {0,1,1,0,1,1,0,1};
			setarray(data,80,112,noteE);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);
		}
		else if (s == 'F'){
			
			int noteF[] = {0,1,0,0,1,1,0,1};
			setarray(data,80,112,noteF);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);;
		}
		else if (s == 'G'){
			
			int noteG[] = {0,0,1,1,0,0,1,0};
			setarray(data,80,112,noteG);
			printtest(data,80,112);
			printf("\n");
			int seconds = duration(dur);
			int secBinary[8] = {0};
			binaryconverter(seconds, secBinary);
			setarray(data,196,228,secBinary);
			printtest(data,196,228);
			printf("\n");
			int level = volume(vol);
			int levelBinary[8] = {0};		
			binaryconverter(level, levelBinary);
			setarray(data,312,344,levelBinary);
			printtest(data,312,344);
			printf("\n");
			transmit(data,size);
		}

	}
}

void printtest(unsigned int data[],int s,int e){
	
	for(int i = s; i < e; i= i+4){
		printf("%d",data[i]);
		}
	}
void transmit(unsigned int data[],int size){
	gpioTerminate();
	gpioInitialise();	
	gpioSetMode(18, PI_OUTPUT);
	int duty = 500000;
	unsigned count = 0;

	while(count<size){
		if(data[count]){
			gpioHardwarePWM(18,MODFREQ,duty);
	
		}
		else{
			gpioWrite(18,0);
		}
		count+=1;
		sleep(TICK);
	}
	gpioSetMode(18,PI_OUTPUT);
	gpioWrite(18,0);
	gpioTerminate();
	
	}
void setarray(unsigned int data[],int s, int e,int input[]){
	int i = 0;
	for(int g = s; g < e; g = g+4){
		if(input[i]==0){data[g] =0;data[g+1] =0;data[g+2] =0;data[g+3] =0;}
		else if(input[i]==1){data[g] =1;data[g+1] =1;data[g+2] =1;data[g+3] =1;}
		i = i+1;
		}
	
	}
int duration(char str[]){
	
	int m = str[0] - '0';
	int s = str[2] - '0';
	int s1 = str[3] - '0';
	m = m*60;
	s = (s *10)+s1+m;
	return s;
	}
int volume(char str[]){
	
	int p1 = str[0] - '0';
	int p2 = str[1] - '0';
	p1 = p1*10;
	p1 = p1 + p2;
	return p1;
	}
void binaryconverter(int sec, int temp[]){

	int check = sec;
	int i = 7;
	if(check == 64){
		temp[1] = 1;
		
		}
	else if(check == 32){
		temp[2] = 1;
		
		}
	else if(check == 16){
		temp[3] = 1;
		
		}
	else if(check == 8){
		temp[4] = 1;
		
		}
	else if(check == 4){
		temp[5] = 1;
		
		}
	else if(check == 2){
		temp[6] = 1;
		
		}
	else if(check == 1){
		temp[7] = 1;
		
		}
	else{
		while(i >= 0 ){
			
			int bit = check%2;
			temp[i] = bit;
			i = i - 1;
			check = check/2;
			}	
		}
	}
void append(unsigned int data[],int s,int e,int v){
	for(int i = s; i < e; i++){
		data[i] = v;
		}
	}
	

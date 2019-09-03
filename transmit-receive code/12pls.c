#include <stdio.h>
#include <pigpio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "audio.h"

// notes frequency chart: "http://www.phy.mtu.edu/~suits/notefreqs.html"
#define freqA4 440
#define freqB4 494
#define freqC5 523
#define freqD5 587
#define freqE5 659
#define freqF5 698
#define freqG5 784

static double Start;

void slice_str(const char * str, char * buffer, size_t start, size_t end)
{
    size_t j = 0;
    for ( size_t i = start; i <= end; i++ ) {
        buffer[j] = str[i];
        j++;
    }
    buffer[j] = '\0';
}

void playNote(int freq, int duration, int volume){
	buzzer_start();
	buzzer_set_volume(volume/10);
	buzzer_set_freq(freq);
	msleep(duration*1000);
	buzzer_stop();
}

void playM(int freq, int duration){
	buzzer_set_freq(freq);
	msleep(duration);
}

int Bin2Dec(char* binVal){
	long longVal = atol(binVal);
	int decNum = 0, i=0, rem;
	while(longVal!=0)
	{
		rem = longVal%10;
		longVal /= 10;
		decNum += rem*pow(2,i);
		i++;
	}
	return decNum;
}

void tic(){
	// This is an alternative, using <sys/time.h>.  It is not as accurate though.
	// timeval Time;
	// gettimeofday(&Time, 0);
	// Start = (double)(Time.tv_sec) + (double)(Time.tv_usec)*1e-6;
	static int First = 1;
	struct timespec Time;
	if(First==1){
		clock_getres(CLOCK_MONOTONIC, &Time);
		Start = (double)(Time.tv_sec) + (double)(Time.tv_nsec)*1e-9;
		printf("Clock resolution: %lg ns\n", Start*1e9);
		First = 0;
	}
	clock_gettime(CLOCK_MONOTONIC, &Time);
	Start = (double)(Time.tv_sec) + (double)(Time.tv_nsec)*1e-9;
}

double toc(){
	double End;
	//timeval Time;
	//gettimeofday(&Time, 0);
	//End = (double)(Time.tv_sec) + (double)(Time.tv_usec)*1e-6;

	struct timespec Time;
	clock_gettime(CLOCK_MONOTONIC, &Time);
	End = (double)(Time.tv_sec) + (double)(Time.tv_nsec)*1e-9;
	return End - Start;
}

char* concat(const char *s1, const char *s2){
	const size_t len1 = strlen(s1);
	const size_t len2 = strlen(s2);
	char *result = malloc(len1 + len2 +1); //+1 for the null-terminator
	// in real code you would check for errors in malloc here
	memcpy(result,s1,len1);
	memcpy(result + len1, s2, len2 + 1); //+1 to copy the null-terminator
	return result;
}

void compare(char* sLetter,int duration){
	if(strcmp(sLetter,"011001010")==0){
		printf("A\n");
		playM(freqA4,duration);
	}else if(strcmp(sLetter,"000110100")==0){
		printf("B\n");
		playM(freqB4,duration);
	}else if(strcmp(sLetter,"010110110")==0){
		printf("C\n");
		playM(freqC5,duration);
	}else if(strcmp(sLetter,"001001000")==0){
		printf("D\n");
		playM(freqD5,duration);
	}else if(strcmp(sLetter,"011011010")==0){
		printf("E\n");
		playM(freqE5,duration);
	}else if(strcmp(sLetter,"010011010")==0){
		printf("F\n");
		playM(freqF5,duration);
	}else if(strcmp(sLetter,"001100100")==0){
		printf("G\n");
		playM(freqG5,duration);
	}else{
		
		char* newLetter = malloc(9);
		if((strlen(sLetter)-9==1) || (strlen(sLetter)-9==-1)){
			printf("Entered the pain\n");
			if(strlen(sLetter)>9){
			printf("Bigger\n");
			slice_str(sLetter, newLetter,0,strlen(sLetter)-2);
			printf("new letter: %s \n",newLetter);
			if(strcmp(newLetter,"011001010")==0){
				printf("A\n");
				playM(freqA4,duration);
			}else if(strcmp(newLetter,"000110100")==0){
				printf("B\n");
				playM(freqB4,duration);
			}else if(strcmp(newLetter,"010110110")==0){
				printf("C\n");
				playM(freqC5,duration);
			}else if(strcmp(newLetter,"001001000")==0){
				printf("D\n");
				playM(freqD5,duration);
			}else if(strcmp(newLetter,"011011010")==0){
				printf("E\n");
				playM(freqE5,duration);
			}else if(strcmp(newLetter,"010011010")==0){
				printf("F\n");
				playM(freqF5,duration);
			}else if(strcmp(newLetter,"001100100")==0){
				printf("G\n");
				playM(freqG5,duration);
			}else{
				free(newLetter);
				char* newLetter2 = malloc(9);
				slice_str(sLetter, newLetter2,1,strlen(sLetter)-2);
				printf("new letter 2: %s \n",newLetter2);
				if(strcmp(newLetter2,"011001010")==0){
					printf("A\n");
					playM(freqA4,duration);
				}else if(strcmp(newLetter2,"000110100")==0){
					printf("B\n");
					playM(freqB4,duration);
				}else if(strcmp(newLetter2,"010110110")==0){
					printf("C\n");
					playM(freqC5,duration);
				}else if(strcmp(newLetter2,"001001000")==0){
					printf("D\n");
					playM(freqD5,duration);
				}else if(strcmp(newLetter2,"011011010")==0){
					printf("E\n");
					playM(freqE5,duration);
				}else if(strcmp(newLetter2,"010011010")==0){
					printf("F\n");
					playM(freqF5,duration);
				}else if(strcmp(newLetter2,"001100100")==0){
					printf("G\n");
					playM(freqG5,duration);
				}else{
				printf("Did not get 9 bits for a letter. Bit Error.\n");
				}
				
				free(newLetter2);
			}
				
		}else if(strlen(sLetter)<9){
			printf("Smaller\n");
			strcpy(newLetter,sLetter);
			concat(sLetter, "0");
			printf("sLetter: %s \n",sLetter);
			if(strcmp(sLetter,"011001010")==0){
				printf("A\n");
				playM(freqA4,duration);
			}else if(strcmp(sLetter,"000110100")==0){
				printf("B\n");
				playM(freqB4,duration);
			}else if(strcmp(sLetter,"010110110")==0){
				printf("C\n");
				playM(freqC5,duration);
			}else if(strcmp(sLetter,"001001000")==0){
				printf("D\n");
				playM(freqD5,duration);
			}else if(strcmp(sLetter,"011011010")==0){
				printf("E\n");
				playM(freqE5,duration);
			}else if(strcmp(sLetter,"010011010")==0){
				printf("F\n");
				playM(freqF5,duration);
			}else if(strcmp(sLetter,"001100100")==0){
				printf("G\n");
				playM(freqG5,duration);
			}else{
				concat("0", newLetter);
				printf("new letter: %s \n",newLetter);
				if(strcmp(newLetter,"011001010")==0){
					printf("A\n");
					playM(freqA4,duration);
				}else if(strcmp(newLetter,"000110100")==0){
					printf("B\n");
					playM(freqB4,duration);
				}else if(strcmp(newLetter,"010110110")==0){
					printf("C\n");
					playM(freqC5,duration);
				}else if(strcmp(newLetter,"001001000")==0){
					printf("D\n");
					playM(freqD5,duration);
				}else if(strcmp(newLetter,"011011010")==0){
					printf("E\n");
					playM(freqE5,duration);
				}else if(strcmp(newLetter,"010011010")==0){
					printf("F\n");
					playM(freqF5,duration);
				}else if(strcmp(newLetter,"001100100")==0){
					printf("G\n");
					playM(freqG5,duration);
				}else{
				printf("Did not get 9 bits for a letter. Bit Error.\n");
			}
			free(newLetter);
		}}
		}else{
			printf("Did not get 9 bits for a letter. Bit Error.\n");
		}
		
	}
}

int main(void){
	//tic();
	gpioInitialise();
	gpioSetMode(17, PI_INPUT);
	
	/*int operation = 1;*/
	
	volatile int iSignal = 0;
	//printf("Hello\n");
	//double toMessageTime = toc();
	//printf("toMessageTime: %f\n",toMessageTime);
	while(1){
		volatile int count = 0;
		unsigned int signals[10200];
		uint64_t times[10200];
		double times2[10200];
		uint64_t totalDuration = 0;
		
		volatile int value = 1;
		/* Loop until we read a 0 */
		while(value==1){
			value=gpioRead(17);
		}
		
		/* Grab the start time of the signal */
		struct timespec start, end;
		tic();
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		/*time_t startTime = time(NULL);
		printf("startTime: %s\n",ctime(&startTime));*/
		/* The end of the "signal" happens when we read more than a certain number of ones (1 is off
		 * for my IR receiver)*/
		volatile unsigned int numOnes = 0;
		
		// Used to keep track of transitions from 1 to 0 
		int previousVal = 0;
		
		while(1){
			if (value != previousVal){
				//The value has changed, so calculate the length of this run
				double timerecord = toc();
				clock_gettime(CLOCK_MONOTONIC_RAW, &end);
				/*time_t endTime = time(NULL);
				printf("endTime: %s\n",ctime(&endTime));
				double diff = difftime(startTime, endTime);*/
				uint64_t diff = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec)/1000;
				//startTime = endTime;
				totalDuration += diff;
				start = end;

				signals[count]=previousVal;
				times[count]=diff;
				times2[count]=timerecord;
				//timerecord=0;
				count+=1;
				//printf("count: %d",count);
			}
			
			if(value){
				numOnes = numOnes+1;
			}else{
				numOnes=0;
			}
			
			if (numOnes > 100000){
				break;
			}
			
			previousVal = value;
			value = gpioRead(17);
		}
		FILE* fp;
		fp = fopen("./Data.txt","w");
		//detecting whether mode 1 or 2
		volatile int counter2 = 0;
		for(int a = 0; a<count; a+=1){
				printf("%d\t%" PRIu64 "\t%f\n",signals[a],times[a],times2[a]);
				fprintf(fp,"%d: %d\t%" PRIu64 "\t%f\n",a,signals[a],times[a],times2[a]);
				if(times[a]>6000){
					counter2 += 1;
				}
		}	
		printf("Counter2 number: %d\n",counter2);
		printf("Size of arrary is %d\n", count);
		printf("Total Duration: %" PRIu64 "\n",totalDuration);
		fclose(fp);
		if(counter2>4){
			printf("Melody Note Mode\n");
			if(times[0]>6000){
			char* sLetter1 = "";
			char* sLetter2 = "";
			char* sLetter3 = "";
			char* sLetter4 = "";
			char* sLetter5 = "";
			char* sLetter6 = "";
			char* sLetter7 = "";
			char* sLetter8 = "";
			char* sLetter9 = "";
			char* sLetter10 = "";
			char* sLetter11 = "";
			char* sLetter12 = "";
			
			char* sDuration = "";
			char* sVolume = "";
			iSignal = iSignal + 1;
			printf("---------Start----------\n");
			printf("Signal Number: %d\n",iSignal);
			printf("Total Duration: %" PRIu64 "\n",totalDuration);
			
			volatile int counter = 0;
			for(int a = 1; a<count; a+=1){
				if(times[a]>6000){
					counter += 1;
					continue;
				}
				if(counter==0){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/400);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sDuration = concat(sDuration,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sDuration = concat(sDuration,"0");
							printf("0");
						}
					}
				}
				if(counter==1){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/370);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sVolume = concat(sVolume,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/250);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sVolume = concat(sVolume,"0");
							printf("0");
						}
					}
				}
				if(counter==2){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter1 = concat(sLetter1,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter1 = concat(sLetter1,"0");
							printf("0");
						}
					}
				}
				if(counter==3){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter2 = concat(sLetter2,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter2 = concat(sLetter2,"0");
							printf("0");
						}
					}
				}
				if(counter==4){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter3 = concat(sLetter3,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter3 = concat(sLetter3,"0");
							printf("0");
						}
					}
				}
				if(counter==5){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter4 = concat(sLetter4,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter4 = concat(sLetter4,"0");
							printf("0");
						}
					}
				}
				if(counter==6){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter5 = concat(sLetter5,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter5 = concat(sLetter5,"0");
							printf("0");
						}
					}
				}
				if(counter==7){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter6 = concat(sLetter6,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter6 = concat(sLetter6,"0");
							printf("0");
						}
					}
				}
				if(counter==8){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter7 = concat(sLetter7,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter7 = concat(sLetter7,"0");
							printf("0");
						}
					}
				}
				if(counter==9){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter8 = concat(sLetter8,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter8 = concat(sLetter8,"0");
							printf("0");
						}
					}
				}
				if(counter==10){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter9 = concat(sLetter9,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter9 = concat(sLetter9,"0");
							printf("0");
						}
					}
				}
				if(counter==11){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter10 = concat(sLetter10,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter10 = concat(sLetter10,"0");
							printf("0");
						}
					}
				}
				if(counter==12){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter11 = concat(sLetter11,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter11 = concat(sLetter11,"0");
							printf("0");
						}
					}
				}
				if(counter==13){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter12 = concat(sLetter12,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter12 = concat(sLetter12,"0");
							printf("0");
						}
					}
				}
			}
			printf("\n---------End----------\n");
			//printf("String letter\n%s\n",sLetter);
			printf("String duration\n%s\n",sDuration);
			printf("String volume\n%s\n",sVolume);
			printf("Size of arrary is %d\n", count);
			
			totalDuration = 0;
			
			//if((strlen(sLetter)==9) && (strlen(sDuration)==9) && (strlen(sVolume)==9)){
				char* sDuration2=malloc(9);
				char* sVolume2=malloc(9);
				slice_str(sDuration, sDuration2, 0, strlen(sDuration)-2);
				slice_str(sVolume, sVolume2,0,strlen(sVolume)-2);
				printf("String duration cut\n%s\n",sDuration2);
				printf("String volume cut\n%s\n",sVolume2);
				int iDuration = Bin2Dec(sDuration2);
				int iVolume = Bin2Dec(sVolume2);
				printf("Duration: %d\nVolume: %d\n",iDuration,iVolume);
				iDuration=iDuration*1000;
				iVolume=iVolume/10;
				buzzer_start();
				buzzer_set_volume(iVolume);
				free(sDuration2);
				free(sVolume2);
				
				printf("String letter 1\n%s\n",sLetter1);
				compare(sLetter1, iDuration);
				
				printf("String letter 2\n%s\n",sLetter2);
				compare(sLetter2, iDuration);
				
				printf("String letter 3\n%s\n",sLetter3);
				compare(sLetter3, iDuration);
				
				printf("String letter 4\n%s\n",sLetter4);
				compare(sLetter4, iDuration);
				
				printf("String letter 5\n%s\n",sLetter5);
				compare(sLetter5, iDuration);
				
				printf("String letter 6\n%s\n",sLetter6);
				compare(sLetter6, iDuration);
				
				printf("String letter 7\n%s\n",sLetter7);
				compare(sLetter7, iDuration);
				
				printf("String letter 8\n%s\n",sLetter8);
				compare(sLetter8, iDuration);
				
				printf("String letter 9\n%s\n",sLetter9);
				compare(sLetter9, iDuration);
				
				printf("String letter 10\n%s\n",sLetter10);
				compare(sLetter10, iDuration);
				
				printf("String letter 11\n%s\n",sLetter11);
				compare(sLetter11, iDuration);
				
				printf("String letter 12\n%s\n",sLetter12);
				compare(sLetter12, iDuration);
				
				buzzer_stop();
				
				
				/*if(strcmp(sLetter,"011001010")==0){
					printf("A\n");
					playNote(freqA4,iDuration,iVolume);
				}else if(strcmp(sLetter,"000110100")==0){
					printf("B\n");
					buzzer_start();
					buzzer_set_freq(freqB4);
					buzzer_set_volume(8);
					msleep(2500);
					buzzer_stop();
					playNote(freqB4,iDuration,iVolume);
				}else if(strcmp(sLetter,"010110110")==0){
					printf("C\n");
					playNote(freqC5,iDuration,iVolume);
				}else if(strcmp(sLetter,"001001000")==0){
					printf("D\n");
					playNote(freqD5,iDuration,iVolume);
				}else if(strcmp(sLetter,"011011010")==0){
					printf("E\n");
					playNote(freqE5,iDuration,iVolume);
				}else if(strcmp(sLetter,"010011010")==0){
					printf("F\n");
					playNote(freqF5,iDuration,iVolume);
				}else if(strcmp(sLetter,"001100100")==0){
					printf("G\n");
					playNote(freqG5,iDuration,iVolume);
				}else{
					printf("Did not get 9 bits for a letter. Bit Error.\n");
				}*/
			
			free(sLetter1);
			free(sLetter2);
			free(sLetter3);
			free(sLetter4);
			free(sLetter5);
			free(sLetter6);
			free(sLetter7);
			free(sLetter8);
			free(sLetter9);
			free(sLetter10);
			free(sLetter11);
			free(sLetter12);
			free(sDuration);
			free(sVolume);
			
		}
		}else if(counter2==4){
		printf("Single Note Mode\n");
		if(times[0]>6000){
			char* sLetter = "";
			char* sDuration = "";
			char* sVolume = "";
			iSignal = iSignal + 1;
			printf("---------Start----------\n");
			printf("Signal Number: %d\n",iSignal);
			printf("Total Duration: %" PRIu64 "\n",totalDuration);
			
			volatile int counter = 0;
			for(int a = 1; a<count; a+=1){
				if(times[a]>6000){
					counter += 1;
					continue;
				}
				if(counter==0){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/400);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sLetter = concat(sLetter,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sLetter = concat(sLetter,"0");
							printf("0");
						}
					}
				}
				if(counter==1){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/370);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sDuration = concat(sDuration,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/250);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sDuration = concat(sDuration,"0");
							printf("0");
						}
					}
				}
				if(counter==2){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
						int repeats = round((double)(times[a])/740);
						for(int b=0; b<repeats;b+=1){
							sVolume = concat(sVolume,"1");
							printf("1");
						}
					}
					if(signals[a]==1){
						//int repeats = round((double)(times[a])/275);
						int repeats = round((double)(times[a])/500);
						for(int b=0; b<repeats;b+=1){
							sVolume = concat(sVolume,"0");
							printf("0");
						}
					}
				}
			}
			printf("\n---------End----------\n");
			printf("String letter\n%s\n",sLetter);
			printf("String duration\n%s\n",sDuration);
			printf("String volume\n%s\n",sVolume);
			printf("Size of arrary is %d\n", count);
			
			totalDuration = 0;
			
			//if((strlen(sLetter)==9) && (strlen(sDuration)==9) && (strlen(sVolume)==9)){
			if(strlen(sDuration)!=9){
				printf("Invalid Duration in Single Note Mode\n");
			}
			if(strlen(sVolume)!=9){
				printf("Invalid Volume in Single Note Mode\n");
			}
				char* sDuration2=malloc(9);
				char* sVolume2=malloc(9);
				slice_str(sDuration, sDuration2, 0, strlen(sDuration)-2);
				slice_str(sVolume, sVolume2,0,strlen(sVolume)-2);
				printf("String duration cut\n%s\n",sDuration2);
				printf("String volume cut\n%s\n",sVolume2);
				int iDuration = Bin2Dec(sDuration2);
				int iVolume = Bin2Dec(sVolume2);
				printf("Duration: %d\nVolume: %d\n",iDuration,iVolume);
				
				free(sDuration2);
				free(sVolume2);
				if(strcmp(sLetter,"011001010")==0){
					printf("A\n");
					playNote(freqA4,iDuration,iVolume);
				}else if(strcmp(sLetter,"000110100")==0){
					printf("B\n");
					/*buzzer_start();
					buzzer_set_freq(freqB4);
					buzzer_set_volume(8);
					msleep(2500);
					buzzer_stop();*/
					playNote(freqB4,iDuration,iVolume);
				}else if(strcmp(sLetter,"010110110")==0){
					printf("C\n");
					playNote(freqC5,iDuration,iVolume);
				}else if(strcmp(sLetter,"001001000")==0){
					printf("D\n");
					playNote(freqD5,iDuration,iVolume);
				}else if(strcmp(sLetter,"011011010")==0){
					printf("E\n");
					playNote(freqE5,iDuration,iVolume);
				}else if(strcmp(sLetter,"010011010")==0){
					printf("F\n");
					playNote(freqF5,iDuration,iVolume);
				}else if(strcmp(sLetter,"001100100")==0){
					printf("G\n");
					playNote(freqG5,iDuration,iVolume);
				}else{
					printf("Invalid Letter in Single Note Mode.\n");
				}
			
			free(sLetter);
			free(sDuration);
			free(sVolume);
			
		}
	}
	}
	gpioTerminate();
	printf("Exitting Program & GPIO terminate is complete!\n");
}
	

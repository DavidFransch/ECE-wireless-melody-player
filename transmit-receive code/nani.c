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
        buffer[i] = str[i];
        j++;
    }
    buffer[j] = '\0';
}

void playNote(int freq, int duration, int volume){
	buzzer_start();
	buzzer_set_freq(freq);
	buzzer_set_volume(volume/10);
	msleep(duration*1000);
	buzzer_stop();
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
				if(counter>=2){
					if(signals[a]==0){
						//int repeats = round((double)(times[a])/350);
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
			}
			printf("\n---------End----------\n");
			printf("String letter\n%s\n",sLetter);
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
				}				
			//}else{
			//	printf("Did not get 9 bits for a variable. Bit Error.\n");
			//}
			
			free(sLetter);
			free(sDuration);
			free(sVolume);
			
		}
	}
	gpioTerminate();
	printf("Exitting Program & GPIO terminate is complete!\n");
}
	

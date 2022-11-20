#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define NUM_THREADS      64 // Number of threads
int size = 10000000;		
int array[10000000];
long count = 0;

/*
*This function initializes an array of predifiened size.
* Requires: nothing. 
* Effects: fills the array with random numbers [0-5] inclusive.
*/
void CreatArray() {
	int max = 6;// 0 - 5
	for (int i = 0; i < size; i++) {
		array[i] = (rand() % max) ; 
	}
}


/*
*This function is used for multithread counting.
* Requires: a starting index. 
* Effects: returns nothing. Counts the occurences of one in the give partition of the array.
*/
void* countOnes(int* index) {
	count = 0;
	int dx = (int)*index; 
	long sect = size/	NUM_THREADS;
	long long myFirstI = dx*sect;
	long long myLastI = myFirstI + sect;

	for (int i = myFirstI; i < myLastI; i++) {
		if (array[i] == 1) {
			count++; 
		}
	}
	return NULL; 
}


/*
*This function is used for sequential counting.
* Requires: nothing
* Effects: returns an integer count of one's in the Array
*/
int Ones () {
    int num =0; 
    for (int i = 0; i < size; i++) {
        if (array[i] == 1){
            num++;
        }
    }
    return num; 
}



int main() {
	srand(0); 
	clock_t start, end;
	CreatArray();

	pthread_t th[NUM_THREADS];

	//For loop to run the code multimple times for accuracy. 
    for (int k = 0; k < 100; k++) {

		start = clock();

		//Create given numbers of threads
		for (int i = 0; i < NUM_THREADS; i++) {
			int* a = (int*)malloc(sizeof(int)); 
			*a = i;
			if (pthread_create(&th[i], NULL, (void*)&countOnes, a)) {
				perror("failed to creat thread"); 
			}
		}
		int *res;// result is just a dummy var here. 

		//Waits for the threads to join
		for (int i = 0; i < NUM_THREADS; i++) {
			if (pthread_join(th[i],(void**) &res ) != 0) {
				perror("failed to join thread");
			}
		}
		end = clock();

		//Calculating the time taken by threads. 
		double time_In_Seconds = ((double)end - start) / CLOCKS_PER_SEC;

		start = clock();
		int Actual_Number_of_Ones = Ones(); 
		end = clock();
		double time_In_Seconds1 = ((double)end - start) / CLOCKS_PER_SEC;
		
		//Printing the results
		printf("\n\n\nSize: %40d", size);
		printf("\nActual Frequency of Ones: %20d", Actual_Number_of_Ones);
		printf("\nSequential time Elapsed: %20f", time_In_Seconds1);
		printf("\nNumber of Threads: %23d", NUM_THREADS);
		printf("\nThread's One count: %25d", count);
		printf("\nTotal Threads Time Elapsed(seconds): %f", time_In_Seconds);
		printf("\nIs threads Result true: %18s", (Actual_Number_of_Ones == count)? "Yes": "No");
		count = 0;
	}
	return 0; 
}
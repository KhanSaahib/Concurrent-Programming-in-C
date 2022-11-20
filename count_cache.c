#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define NUM_THREADS     64// Number of threads
int size = 10000000;
int array[10000000];
int L1 = 255996; // size of L1 chache - 4 bytes. 

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
* Effects: returns an integer count of one's in the give partition of the array.
*/
void* countOnes(int* index) {
	int dx = (int)*index; 
	long sect = size/	NUM_THREADS;
	long long myFirstI = dx*sect;
	long long myLastI = myFirstI + sect;
    int* mycount = (int *) malloc(sizeof(int)); 
    *mycount = 0;
    char arr [255996];

	for (int i = myFirstI; i < myLastI; i++) {
		if (array[i] == 1) {
			*mycount = *mycount + 1; 
		}
	}
    
	return mycount; 
}

/*
*This function is used for sequential counting.
* Requires: nothing
* Effects: returns an integer count of one's in the Array
*/
int count;
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
	int* res; 
	clock_t start, end;
	CreatArray();
	pthread_t th[NUM_THREADS];
    int total_ones = 0;

    //For loop to run the code 100 times. for accuracy
    for (int k = 0; k < 100; k++) {
        start = clock();

        //Create given number of threads
        for (int i = 0; i < NUM_THREADS; i++) {
            int* a = (int*)malloc(sizeof(int)); 
            int section = (size / NUM_THREADS); 
            *a = i;
            if (pthread_create(&th[i], NULL, (void*)&countOnes, a)) {
                perror("failed to creat thread"); 
            }
        }

        //Wait for threads to cimplete and add thre returned values.
        for (int i = 0; i < NUM_THREADS; i++) {
            if (pthread_join(th[i], (void**)&res) != 0) {
                perror("failed to join thread");
            }
            total_ones += *res;
            free(res);
        }
        end = clock();

        //Calculating the total number of time taken by all threads.
        double time_In_Seconds = ((double)end - start) / CLOCKS_PER_SEC;



        start = clock();
        int Actual_Number_of_Ones = Ones(); 
        end = clock();

        //Calculating the total number of time taken by squential function. 
        double time_In_Seconds1 = ((double)end - start) / CLOCKS_PER_SEC;


        //Printing the results
        printf("\n\n\nSize: %40d", size);
        printf("\nActual Frequency of Ones: %20d", Actual_Number_of_Ones);
        printf("\nSequential time Elapsed: %20f", time_In_Seconds1);
        printf("\nNumber of Threads: %23d", NUM_THREADS);
        printf("\nThread's One count: %25d", total_ones);
        printf("\nTotal Threads Time Elapsed(seconds): %f", time_In_Seconds);
        printf("\nIs threads Result true: %18s", (Actual_Number_of_Ones == total_ones)? "Yes": "No");
        count = 0;
        total_ones = 0;
	}


	return 0; 
}
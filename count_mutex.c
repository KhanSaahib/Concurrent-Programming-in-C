#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define NUM_THREADS     2
int size = 10000000;
int array[10000000];
int count;
long i;
pthread_mutex_t mutex;


void CreatArray() {
	int max = 6;// 0 - 5
	for (int i = 0; i < size; i++) {
		array[i] = (rand() % max) ; 
	}
}

void* countOnes(int* index) {
	int dx = (int)*index; 
	long sect = size/	NUM_THREADS;
	long long myFirstI = dx*sect;
	long long myLastI = myFirstI + sect;

	for (int i = myFirstI; i < myLastI; i++) {
		if (array[i] == 1) {
			pthread_mutex_lock(&mutex);
			count++; 
			pthread_mutex_unlock(&mutex);
		}
	}
	return NULL; 
}

int Ones () {
    int num =0; 
    for (int i = 0; i < size; i++) {
        if (array[i] == 1){
            num++;
        }
    }
    return num; 
}

void print () {
	printf("\n");
	for (int i = 0; i < size; i++) {
		printf("%d ", array[i]);
	}
	
	printf("\n");
}

int main() {
	srand(0);
	int* res; 
	clock_t start, end;
	CreatArray();
	pthread_t th[NUM_THREADS];
	pthread_mutex_init(&mutex,NULL);

    for (int k = 0; k < 100; k++) {
	start = clock();
	for (int i = 0; i < NUM_THREADS; i++) {
		int* a = (int*)malloc(sizeof(int)); 
		int section = (size / NUM_THREADS); 
		*a = i;
		if (pthread_create(&th[i], NULL, (void*)&countOnes, a)) {
			perror("failed to creat thread"); 
		}
	}
	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(th[i], (void**)&res) != 0) {
			perror("failed to join thread");
		}
	}
    end = clock();
	double time_In_Seconds = ((double)end - start) / CLOCKS_PER_SEC;



    start = clock();
    int Actual_Number_of_Ones = Ones(); 
    end = clock();
	double time_In_Seconds1 = ((double)end - start) / CLOCKS_PER_SEC;

	printf("\n\n\nSize: %40d", size);
    printf("\nActual Frequency of Ones: %20d", Actual_Number_of_Ones);
    printf("\nSequential time Elapsed: %20f", time_In_Seconds1);
    printf("\nNumber of Threads: %23d", NUM_THREADS);
    printf("\nThread's One count: %25d", count);
    printf("\nTotal Threads Time Elapsed(seconds): %f", time_In_Seconds);
    printf("\nIs threads Result true: %18s", (Actual_Number_of_Ones == count)? "Yes": "No");
    count = 0;
	}


	
	pthread_mutex_destroy(&mutex);
	return 0; 
}
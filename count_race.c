#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define NUM_THREADS      64
int size = 100000000;
int array[100000000];
int count;
long i;
//pthread_mutex_t m;
//pthread_t threads[NUM_THREADS];


void CreatArray() {
	int max = 6;// 0 - 5
	for (int i = 0; i < size; i++) {
		array[i] = (rand() % max) ; 
	}
}

void* countOnes(int* index) {
	count = 0; 
	int dx = (int)*index; 
	for (int i = dx; i < size/NUM_THREADS; i++) {
		if (array[i] == 1) {
			count++; 
		}
	}
	int* countptr = &count; 
	return countptr; 
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



int main() {
	srand(0);
	int* res; 
	clock_t start, end;
	CreatArray();
	pthread_t th[NUM_THREADS];
    for (int k = 0; k < 100; k++) {
	start = clock();
	for (int i = 0; i < NUM_THREADS; i++) {
		int* a = (int*)malloc(sizeof(int)); 
		//int a;
		int section = size / NUM_THREADS; 
		*a = i * section;
		if (pthread_create(&th[i], NULL, (void*)&countOnes, a)) {
			perror("failed to creat thread"); 
		}
	}
    int sum = 0; 
	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(th[i], (void**)&res) != 0) {
			perror("failed to join thread");
		}
        sum += *res; 
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
    printf("\nThread's One count: %25d", sum);
    printf("\nTotal Threads Time Elapsed(seconds): %f", time_In_Seconds);
    printf("\nIs threads Result true: %18s", (Actual_Number_of_Ones == sum)? "Yes": "No");
    }

	return 0; 
}
/*
	Name    : Pradeep Suthar
	Reg no. : 11815262
	email id: sutharp777@gmail.com
	subject : Operating System
	Mentor  : Dr. Baljit Singh Saini
	Topic   : Reader Writer Problem and synchronization using pthread (mutex, condition variable)
*/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"

struct var
{
    char str[10];
    int num;
};

const int x = 3;   // x belong to { 1,2,3,4 ...} input [test case]

pthread_mutex_t mutex ;
pthread_mutex_t writer_mutex ;
pthread_cond_t writer_cond ;

int reader_counter = 0;
int writer_counter = 0;

struct var global_var = {"default", 50};

int xyz = 10;    // another global variable
int a[5] = {1, 2, 3, 4, 5};
char string[][10] = {"", "one", "two", "three", "four", "five"};

void *read_func(void *args)
{

    int thread_no = *(int*)args;
    pthread_mutex_lock( &mutex);
        reader_counter++;
        if (reader_counter==1)
	{
            pthread_mutex_lock( &writer_mutex);
            // pthread_cond_wait( &writer_cond, &mutex);
        }
        
    pthread_mutex_unlock( &mutex);
    sleep( rand()%5);
        printf("Reader Thread Number : %d\n", thread_no);
        printf("Number of Thread in Critical Section (includeing self) : %d \n", reader_counter + writer_counter);
        for (int i=0; i<x; i++)    // reading global variable x times 
            printf("Reader Thread %d Readed xyz (global variable) is %d\n", thread_no, xyz);  
            printf("global variable struct var readed { %s, %d }\n", global_var.str, global_var.num);
        printf("-----------------------------------------\n");

    pthread_mutex_lock( &mutex);
        reader_counter--;
        if (reader_counter==0){
            pthread_mutex_unlock( &writer_mutex);
            // pthread_cond_signal(&writer_cond);
        }

    pthread_mutex_unlock( &mutex);
}

void *write_func(void *args)
{
    int thread_no = *(int*)args;
    sleep(rand()%5);
       pthread_mutex_lock(&writer_mutex);
        // pthread_cond_wait(&writer_cond, &mutex);
        writer_counter++;
        for(int i=0;i<x;i++)
            xyz++;     // updateing global variable (shered var) x time ;

        strcpy(global_var.str, string[ thread_no ]);
        global_var.num = thread_no;
        printf("Writer Thread Number : %d\n", thread_no);
        printf("Number of Thread in Critical Section (includeing self) : %d \n", reader_counter + writer_counter);
        printf("Writer xyz (incrementing global variable \"x\" %d times) is %d\n", x, xyz);
        printf("global variable struct var readed { %s, %d }\n", global_var.str, global_var.num);

        printf("-----------------------------------------\n");
        
        writer_counter--;
    pthread_mutex_unlock(&writer_mutex);
    // pthread_cond_signal(&writer_cond);
}

int main(){
    
    pthread_t reader[5], writer[5];
 
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&writer_mutex, NULL);
    pthread_cond_init(&writer_cond, NULL);
    
    for(int i = 0; i < 5; i++) {
        pthread_create(&reader[i], NULL,read_func,  (void *)&a[i]);
        pthread_setschedprio( reader[i], 1);
        
        /**
         * after the creation I set priority of respective ith reader thread is to 0;
         * using pthread_setschedprio(pthread_t thread, int priority_no);
         * all reader thread has priority 0;
         */
         
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&writer[i], NULL, write_func, (void *)&a[i]);
        pthread_setschedprio( writer[i], 2);
        /**
         * here I set priority of each writer to '5'
         * by default grater the priority_no lesser the priority of that thread
         * if we invert these priority than "writer must execute first".
         */

    }

    for(int i = 0; i < 5; i++) {
        pthread_join(reader[i], NULL);
    }


    for(int i = 0; i < 5; i++) {
        pthread_join(writer[i], NULL);
    }
	return 0;
}

/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <pthread.h>

#define BEGIN_THREADS 0

int count_thread,
    number_thread,
    visble_scope,
    dependent_variable,
    i,j,k,
    set_number_of_threads;

typedef struct
{
    long setID;
    long iteracion;
}threadNode;

void *run_my_thread(void *p);

int main(void)
{
    int independent_variable,
        solve_for_threads;
    printf("Threads No. = ");
    scanf("%d",&set_number_of_threads);
    
   // threadNode arrayThreads[0];
   
   count_thread=BEGIN_THREADS;

    //Number of Threads given by the user through console
    pthread_t c[set_number_of_threads];
    threadNode arrayThreads[set_number_of_threads];      /*Declare array of Threads*/
    pthread_attr_t a;
    pthread_attr_init(&a);
    
    if(pthread_attr_setscope(&a, PTHREAD_SCOPE_SYSTEM)!=0)
        printf("NOT SET SCHEDULLLING");
    
    if(pthread_attr_getscope(&a,&visble_scope)==0)
    {
        count_thread++;
        if(visble_scope==PTHREAD_SCOPE_SYSTEM)
            printf("PTHREAD_SCOPEE_SYSTEM\n");
        else if(visble_scope==PTHREAD_SCOPE_PROCESS)
            printf("PTHREAD_SCOPEE_PROCESS\n");
        else
            fprintf(stderr," SCOPE NOT VALID\n");
    }
    else{
        fprintf(stderr,"NOT SCHEDULLLING SCOPE\n");
    }
    
    for(i=0; i<set_number_of_threads;i++){
        
        arrayThreads[i].setID=rand()%(set_number_of_threads*10);
        arrayThreads[i].iteracion=i;
        
        pthread_create(&c[i],&a,run_my_thread,(void*)&arrayThreads[i]);
        printf("\nI am thread %d\t\t Created new thread %d\t\t  in iteration%d",i+1,arrayThreads[i].setID,arrayThreads[i].iteracion);
    
        if((i+1)%5==0) sleep(1);
    }
for (i = 0; i < set_number_of_threads; i++){
    pthread_join(c[i],NULL);
}
}

void *run_my_thread(void *p){
    threadNode *c=p;
    printf("\nHello from thread %d \t\t I was created in iteration %d",c->setID,c->iteracion);
    pthread_exit(0);
}

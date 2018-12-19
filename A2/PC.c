#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int queue[200]; 
int box = 0; 
int r_size  = 5000; 
int q_size  = 100; 
int request = 0;
int produce = 0;
int consume = 0;
int produce_num[10]; 
int consume_num[10]; 

sem_t full;
sem_t empty;
sem_t mutex;
int set(int what, int id){
    int i = 0;
    if(what == 1){ 
        box++;
        sem_wait(&empty);
        sem_wait(&mutex);
        if(request>=r_size){ 
            sem_post(&mutex);
            sem_post(&full);
            return -1;
        }
        queue[produce%q_size] = 1;
        produce++;
        request++;
        produce_num[id] += 1;
        printf("%6d ", request); 
        for(i = 0; i<q_size; i++)
            printf("%d",queue[i]);
        printf("\n");
        sem_post(&mutex);
        sem_post(&full);
        return 0;
    }      
    else{ 
        sem_wait(&full);
        sem_wait(&mutex);
        if(request>= r_size){
            sem_post(&mutex);
            sem_post(&empty);
            return -1;
        }
        queue[consume%q_size] = 0;
        consume++;
        request++;
        consume_num[id] += 1;
        printf("%6d ", request); 
        for(i = 0; i<q_size; i++)
            printf("%d",queue[i]);
        printf("\n");
        sem_post(&mutex);
        sem_post(&empty);
        box--;
        return 0;
    }
}
void *produce_thread(void *tid){ 
    int id = *((int *)tid);
    int i;
    while(1){
        if(set(1,id)==-1)
            pthread_exit(NULL);
    }
}
 
void *consume_thread(void *tid){
    int id = *((int *)tid); 
    int i;
    while(1){
        if(set(0,id)==-1)
            pthread_exit(NULL);
    }
}
 
int main(int argc, char* argv[]){
    int i;
    int state;
    char* q_str = "-q";
    char* r_str = "-r";
    char* p_str = "-p"; int p_size = 3;
    char* c_str = "-c"; int c_size = 3;
    int status  = 0;
    int p_num[10]; 
    int c_num[10];
    pthread_t thread_p[10];
    pthread_t thread_c[10];

    for(i = 0; i<argc; i++){
        if(strcmp(argv[i],q_str)==0)
            q_size = atoi(argv[i+1]);
        if(strcmp(argv[i],r_str)==0)
            r_size = atoi(argv[i+1]);
        if(strcmp(argv[i],p_str)==0)
            p_size = atoi(argv[i+1]);
        if(strcmp(argv[i],c_str)==0)
            c_size = atoi(argv[i+1]);
    }
    if(q_size<1 || q_size>200){
        printf("q_size is not correct\n");
        return -1;
    }
    if(r_size<1 || r_size>50000){
        printf("r_size is not correct\n");
        return -1;
    }
    if(p_size<1 || p_size>10){
        printf("p_size is not correct\n");
        return -1;
    }
    if(c_size<1 || c_size>10){
        printf("c_size is not correct\n");
        return -1;
    }
    for(i = 0; i<p_size; i++)
        p_num[i] = i;
    for(i = 0; i<c_size; i++)
        c_num[i] = i;
    if(sem_init(&full,0,0)!=0){
        printf("sem_init Error\n");
        return 0;
    }
    if(sem_init(&empty,0,q_size)!=0){
        printf("sem_init Error\n");
        return 0;
    }
    if(sem_init(&mutex,0,1)!=0){
        printf("sem_init Error\n");
        return 0;
    }
    
    for(i = 0; i<p_size; i++){
        status = pthread_create(&thread_p[i], NULL,  produce_thread,(void*) (p_num+i));
        if(status != 0)
            printf("produce : pthread_creat returned error code : %d\n",status);
    }
    
    for(i = 0; i<c_size; i++){
        status = pthread_create(&thread_c[i], NULL, consume_thread, (void*) (c_num+i));
        if(status != 0)
            printf("consume : pthread_creat returned error code : %d\n",status);
    }
   
     
    for(i = 0; i<p_size; i++){
        pthread_join(thread_p[i], (void**)&status);
    }
     
    for(i = 0; i<c_size; i++){
        pthread_join(thread_c[i], (void**)&status);
    }
   
    for(i = 0; i<p_size; i++){
        printf("producer %d:%d\n",i,produce_num[i]);
    }
    printf("\n");
    
    for(i = 0; i<c_size; i++){
        printf("consumer %d:%d\n",i,consume_num[i]);
    }
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&mutex);
     
    return 0;
}

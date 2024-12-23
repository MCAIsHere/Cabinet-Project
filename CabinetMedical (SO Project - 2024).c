#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

// - Vor fi create un numar dat de thread-uri pacienti care vor astepta pentru eliberarea unor resurse reprezentand doctorii 
// - Clientii vor ocupa resursa doctor pentru o perioada random care sa nu depaseasca TIMP_CONSULT_MAX
// - Fiecare pacient va fi generat la un interval random pentru o perioada data de timp
// - Dupa consultatie, pacientul va afisa timpul de asteptare si timpul consultatiei

#define NR_PACIENTI 20
#define NR_DOCTORI 2
#define TIMP_GENERAT_MAX 15
#define TIMP_CONSULT_MAX 5
int doctori_liberi = NR_DOCTORI;
pthread_mutex_t mtx;
sem_t sem;

void room(int* id, double timp_asteptare){
    int timp_consultare = (rand() % TIMP_CONSULT_MAX) + 1;      // get random number
    
    sleep(timp_consultare);

    pthread_mutex_lock(&mtx);
    printf("Pacient %d a asterptat %.2f sec si a fost consultat %d sec. \n", *id, timp_asteptare, timp_consultare);   // fixing the printf order
    pthread_mutex_unlock(&mtx);
    
    sem_post(&sem);
    
    free(id);
}

void* lobby(void* arg){
    int* id = (int*) arg;                             
    int timp = (rand() % TIMP_GENERAT_MAX) + 1;       // get random number
    
    sleep(timp);                          
    
    pthread_mutex_lock(&mtx);
    printf("Pacientul %d a fost generat. \n", *id);     // fixing the printf order
    pthread_mutex_unlock(&mtx);
    
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    sem_wait(&sem);
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double timp_asteptare = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    
    room(id,timp_asteptare);
}

int main(){
        pthread_t pacienti[NR_PACIENTI];
        int n_rand;
        
        if (pthread_mutex_init(&mtx,NULL)) {                // mutex
          perror(NULL);
          return errno;
        }
        
        if (sem_init(&sem,0,NR_DOCTORI)){      // semafor
          perror(NULL);
          return errno;
        }
        
        for (int i = 0; i < NR_PACIENTI; i++){              // pthread create
          int* id = malloc(sizeof(int));
          *id = i;
          if (pthread_create(&pacienti[i], NULL, lobby, (void*)id)){     
            perror("Erroare la pthread_create");
            return errno;
          }
        }
        
        for (int i = 0; i < NR_PACIENTI; i++){              // pthread join
          if (pthread_join(pacienti[i], NULL)) {      
            perror("Eroare la pthread_join");
            return errno;
          }
        }
        
        pthread_mutex_destroy(&mtx);
        sem_destroy(&sem);
	return 0;
}
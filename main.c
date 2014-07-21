#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "globalVariables.h"
#include "home.h"
#include "trash.h"
#include "collectManager.h"

/*
 * Distribution of the trash sizes for individual trashes
 * We have a certain number of size available, and each size is linked to a proportion : some size and more common that others
 * percentageTrashSize contains the thresholds compared witht the random number generated at the creation of the trash
 */
int numberTrashSizes = 4;
float percentageTrashSize[][2] = {{80, 0.3}, {120, 0.5}, {180, 0.8},{240,1}};

/*
 * Free the memory, the mutex and the conditions before the program ends
 */
void freeMemory() {
	int i;
	
	for(i = 0; i < NB_TRASHES; i++) {
		if(trashes[i] != NULL) {
			free(trashes[i]);
			trashes[i] = NULL;
		}
		pthread_mutex_destroy(&(mutexTrashes[i]));
		free(wait_throw[i]);
		wait_throw[i] = NULL;
		
	}
	pthread_cond_destroy(&addTrashListToEmpty);
	pthread_cond_destroy(&firstWait);
	free(wait_throw);
	wait_throw = NULL;
	free(arrayTrashInList);
	arrayTrashInList = NULL;
	free(users_thread);
	users_thread = NULL;
	free(mutexTrashes);
	mutexTrashes = NULL;
	free(trashes_thread);
	trashes_thread = NULL;
}

/*
 * Signal handler used for catching Ctrl+C
 */
void sig_handler(int signo)
{
	if (signo == SIGINT) {
		int i;
		for(i = 0; i < nHomes; i++) {
			pthread_cancel(users_thread[i]);
		}
		for(i = 0; i < NB_TRASHES; i++) {
			pthread_cancel(trashes_thread[i]);
		}
		printf("\nSoftware will shut down\n");
		freeMemory();
		kill(getpid(), SIGINT);
	}
}


int main(int argc, char **argv) 
{
	int i, j;
	
	/* Default values for parameters */
	nHomes = 4;
	homesWithIndiv = 0.6;
	homesWithKey = 0.4;
	nSortingCenter = 3;
	
	
	if(argc <= 1) {
		printf("%s nHomes nSortingCenters [homesWithIndiv] [homesWithKey]\n", argv[0]);
		printf("If you choose to continue, the default values (%d %f %f %d) will be used.\n", nHomes, homesWithIndiv, homesWithKey, nSortingCenter);
		getchar();
	}
	if(argc >= 3) {
		nHomes = atoi(argv[1]);
		nSortingCenter = atoi(argv[2]);
		if(argc >= 4)
			homesWithIndiv = atof(argv[3]);
		if(argc >= 5)
			homesWithKey = atof(argv[4]);
	}
	
	// Initialize the random number generator
	srand(time(0));

	// Size of the sorting center trashes
	sizeSortingCenters[ORDINARY] = 400;
	sizeSortingCenters[OTHER] = 300;
	
	// Average number of homes per sorting center (idea of neighbourhood)
	homePerSortingCenter = nHomes/nSortingCenter;	

	/*
	 * Memory allocation
	 */
	mutexTrashes = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t)*NB_TRASHES);
	for(i = 0; i < NB_TRASHES; i++) {
		pthread_mutex_init(mutexTrashes+i, 0);
	}
	trashes = (Trash**) malloc(sizeof(Trash*)*NB_TRASHES);
	users_thread = (pthread_t*) malloc(nHomes * sizeof(pthread_t));
	trashes_thread = (pthread_t*) malloc(sizeof(pthread_t)*NB_TRASHES);
	wait_throw = (pthread_cond_t**)  malloc(sizeof(pthread_cond_t*)*NB_TRASHES);
	arrayTrashInList = (int*) malloc(sizeof(int)*NB_TRASHES);					
	
	pthread_mutex_init(&mutexListToEmpty,0);
	pthread_cond_init(&addTrashListToEmpty,0);
	pthread_cond_init(&firstWait,0);
	
	pthread_create(&collect_thread, 0, (void *(*)())collectManager, NULL);
	
	/*
	 * A sorting center is a series of trashes available to multiple homes
	 */
	for(i = 0; i < nSortingCenter; i++) {
		createTrash(nHomes+i*NB_WASTE_TYPE);
		initTrash(nHomes+i*NB_WASTE_TYPE, sizeSortingCenters[0], true, 0);
		
		for(j = 1; j < NB_WASTE_TYPE; j++) {
			createTrash(nHomes+i*NB_WASTE_TYPE+j);
			initTrash(nHomes+i*NB_WASTE_TYPE+j, sizeSortingCenters[j], false, j);
		}
	}
	
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		printf("\nCan't catch SIGINT\n");  
	
	pthread_mutex_init(&firstWaitMutex, 0);
	
	for(i = 0; i < nHomes; i++) {
		pthread_create(&(users_thread[i]),0,(void *(*)())home,(void*)i);
	}	
	
	// Wait for every thread to initialize its variables
	sleep(1);
	printf("\nPress any key to continue...\n");
	getchar();
	
	// Unlock every threads
	pthread_mutex_lock(&firstWaitMutex);
	pthread_cond_broadcast(&firstWait);
	pthread_mutex_unlock(&firstWaitMutex);
	
	pthread_mutex_destroy(&firstWaitMutex);	
	
	for(i = 0; i < nHomes; i++) {
		pthread_join(users_thread[i],NULL);
	}
	
	freeMemory();
	return EXIT_SUCCESS;
}

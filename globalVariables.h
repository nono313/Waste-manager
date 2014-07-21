#ifndef __GLOBAL_VARIABLES__
#define __GLOBAL_VARIABLES__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <pthread.h>
#include <unistd.h>

#include "list.h"
// Types of waste managed by the program
#define NB_WASTE_TYPE 2
// Total number of trashes in the city (personal and sorting centers)
#define NB_TRASHES (nHomes+NB_WASTE_TYPE*nSortingCenter)

// Duration of a day (in milliseconds)
#define DAY_DURATION 1500	/* 1000 ms = 1s */

// A possible enhancement would be to add other types of waste (like paper or glass)
typedef enum {
	ORDINARY = 0,
	OTHER = 1
} WasteType;

// A garbage bag contains only one type of waste.
typedef struct{
	WasteType type;
	int wasteQuantity;	// Litre of waste in the bag (<= capacity)
	int capacity;
} GarbageBag;

// A trash is like a garbage bag with a bigger capacity and a boolean value saying if a key is needed to access the trash (it is the case of the ordinary trashes in the sorting centers)
typedef struct {
	int wasteQuantity;
	int capacity;
	bool needKey;
	WasteType type;
} Trash;

/*
 * Array of pointers to trashes
 * The first nHomes represents the individual trashes of every home.
 * 	These pointers can be null if the home doesn't have an individual bin
 */
Trash **trashes;

/*
 * A home can have either an individual trash, or an access to the sorting centers (with the key), or both
 * These variables represents the proportion of homes with individual trashes and with key (they are indipendant and therefore some homes will have access to both
 */
float homesWithIndiv;
float homesWithKey;

/*
 * Number of sorting centers and number of homes attached to these sorting centers
 * It works as quarter with an equal number of homes attached to each sorting center
 */
int nSortingCenter;
int homePerSortingCenter;

/*
 * Distribution of the trash sizes for individual trashes
 * (see main.c)
 * We have a certain number of size available, and each size is linked to a proportion : some size and more common that others
 */
extern int numberTrashSizes;
extern float percentageTrashSize[][2];

// Arrays of threads
pthread_t *users_thread;
pthread_t *trashes_thread;

/*
 * Conditions used by the homes to wake up trashes once something has been put in the trash
 * This is an array of pointers to pthread_cond_t
 */
pthread_cond_t **wait_throw;

// Number of homes in the city
int nHomes;

// Size of the trashes in the sorting centers
int sizeSortingCenters[NB_WASTE_TYPE];

// List of trashes that need to be picked up
AbstractList trashesToEmpty;
int *arrayTrashInList;	// Counters used to know if the trash is already in the list

/*
 * Each trash has a mutex that needs to be locked in order to interact with it (either filling or empty it)
 */
pthread_mutex_t *mutexTrashes;

/*
 * Thread, mutex and condition used for the collect manager
 */
pthread_t collect_thread;
pthread_mutex_t mutexListToEmpty;
pthread_cond_t addTrashListToEmpty;

/*
 * Conditions and mutex used to sync the launch of the program 
 * and the initialization of the home threads
 */
pthread_cond_t firstWait;
pthread_mutex_t firstWaitMutex;

#endif
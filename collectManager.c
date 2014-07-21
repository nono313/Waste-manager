#include "collectManager.h"
#include "trash.h"

/*
 * CollectManager : This thread empty the trashes when needed
 */
void * collectManager() {
	int a;
	// List of trashes that have reached critical filling
	trashesToEmpty = initList(sizeof(int));	
	
	pthread_mutex_lock(&mutexListToEmpty);
	for(int i = 0; i < NB_TRASHES; i++) {
		arrayTrashInList[i] = 0;
	}
	
	while(1) {
// 		printf("Collect : wait on addTrashListToEmpty and release mutexList\n");
		pthread_cond_wait(&addTrashListToEmpty, &mutexListToEmpty);
// 		printf("Collect : out of wait on addTrashListToEmpty and get mutexList\n");
		
		// If the list contains at least 4 trashes, we send the garbage truck
		if(isEmpty(trashesToEmpty) == false && trashesToEmpty.count >= 4)
		{
// 			
			// We empty the list from the head (FIFO) untill it is empty
			// Between 2 trashes, the truck sleeps. Another trash can be added to the queue of the list during that sleep
			while(isEmpty(trashesToEmpty) == false) {
				a = *((int*)(trashesToEmpty.head->value));
				arrayTrashInList[a] = 0;
				trashesToEmpty = removeHead(trashesToEmpty);
// 				printf("Collect : remove head : %d\n", a);
				
				pthread_mutex_unlock(&mutexListToEmpty);
				
				
// 				printf("Collect : call emptyTrash on %d\n", a);
				emptyTrash(a);
// 				printf("Collect : end of call emptyTrash on %d\n", a);
 				
// 				printf("\nCollect : free mutexList to sleep\n");
				
				// Sleep between trashes
				usleep(200);
				
				pthread_mutex_lock(&mutexListToEmpty);
// 				printf("\nCollect : got mutexList\n");
			}
		}
	}
	
	pthread_mutex_unlock(&mutexListToEmpty);
	pthread_exit(0);
}

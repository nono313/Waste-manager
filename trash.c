#include "trash.h"

void createTrash(int id) {
	pthread_mutex_lock(mutexTrashes+id);
	trashes[id] = (Trash*) malloc(sizeof(Trash));
	int r = rand()%100;
	int i = 0;
	// Select randomly a size of trash for the individual trash
	while(i < numberTrashSizes && r > percentageTrashSize[i][1]*100) {
		i++;
	}
	if(i < numberTrashSizes && r <= percentageTrashSize[i][1]*100) {
		trashes[id]->capacity = percentageTrashSize[i][0];
	}
	else {
		trashes[id]->capacity = percentageTrashSize[numberTrashSizes-1][0];
	}
	trashes[id]->type = ORDINARY;

	// Cond on which the thread trash waits on a movement in the trash (anywhere when a client throw a bag in the trash
	wait_throw[id] = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));

	pthread_cond_init(wait_throw[id],0);
	
	pthread_mutex_unlock(mutexTrashes+id);
	
	//Launch the thread trash
	pthread_create(trashes_thread+id, 0, (void*(*)())trash, (void*)id);
}

void initTrash(int id, int capa, bool key, WasteType type) {
	pthread_mutex_lock(mutexTrashes+id);
	
	trashes[id]->capacity = capa;
	trashes[id]->needKey = key;
	trashes[id]->wasteQuantity = 0;
	trashes[id]->type = type;
	
	pthread_mutex_unlock(mutexTrashes+id);
}

void initTrashNull(int id) {
	pthread_mutex_lock(mutexTrashes+id);
	trashes[id] = NULL;
	pthread_mutex_unlock(mutexTrashes+id);
}

bool throwWaste(GarbageBag *gb, int id, int idHome) {
// 	printf("\tThrowWaste : wait on lock on mutex %d\n", id);
	pthread_mutex_lock(mutexTrashes+id);
// 	printf("\tThrowWaste : locked on mutex %d\n", id);
	bool res = false;

	//printf("THROW ==> ID : %d | Pointeur : %p\n", id, trashes[id]);
	
	// If there is enough free space to throw the bag in the trash
	if((trashes[id]->wasteQuantity + gb->wasteQuantity) <= trashes[id]->capacity) 
	{
		printf("\nClient %d ajoute %d dans la poubelle %d. (%d / %d)", idHome, gb->wasteQuantity, id, gb->wasteQuantity+trashes[id]->wasteQuantity, trashes[id]->capacity);
		trashes[id]->wasteQuantity += gb->wasteQuantity;
		gb->wasteQuantity = 0;
		res = true;
// 		printf("\tThrowWaste : signal on wait_throw %d\n", id);
		pthread_cond_signal(wait_throw[id]);
// 		printf("\tThrowWaste : signalED on wait_throw %d\n", id);
	}	
	pthread_mutex_unlock(mutexTrashes+id);
// 	printf("\tThrowWaste : UNlocked on mutex %d\n", id);

	return res;
}

void emptyTrash(int id) {
// 	printf("\t\tEmpty : ask for mutex %d\n", id);
	pthread_mutex_lock(mutexTrashes+id);
// 	printf("\t\tEmpty : locked mutex %d\n", id);
	
	trashes[id]->wasteQuantity = 0;
	printf("\n\t\tLa poubelle %d a été vidée", id);
	pthread_mutex_unlock(mutexTrashes+id);
// 	printf("\t\tEmpty : UNlocked mutex %d\n", id);
}

void addTrashToList(int t) {
// 	printf("\t\t\taddTrash %d : ask for mutexList\n",t);
	pthread_mutex_lock(&mutexListToEmpty);
// 	printf("\t\t\taddTrash %d : locked on mutexList\n",t);
	arrayTrashInList[t] = arrayTrashInList[t] + 1;		// Compteur permettant de savoir si la poubelle est déja dans la liste
	
	if(arrayTrashInList[t] == 1) {
		trashesToEmpty = insertTail(trashesToEmpty, &t, true);
// 		pthread_cond_signal(&addTrashListToEmpty);
		printf("\nListe des poubelles à vider : \t");
// 		displayListInt(trashesToEmpty);
	}
	else {	
		/*
		 * Possible improvement : create a second list of trashes for the most prioritary trashes (those who asks to be empty when they are already in the trashesToEmpty list
		 */
		printf("\nLa poubelle %d est déjà dans la liste de poubelles à vider.", t);
	}
	
	displayListInt(trashesToEmpty);
// 	printf("\t\t\taddTrash : will send signal addTrashListToEmpty\n");
	pthread_cond_signal(&addTrashListToEmpty);
// 	printf("\t\t\taddTrash : signal send on addTrashListToEmpty\n");
	pthread_mutex_unlock(&mutexListToEmpty);	
// 	printf("\t\t\taddTrash : UNlocked mutexList\n");
}

void * trash(void* arg) {
	int id = (int)arg;
// 	printf("\t\t\t\tTrash : ask for mutex %d\n", id);
	pthread_mutex_lock(mutexTrashes+id);
// 	printf("\t\t\t\tTrash : locked mutex %d\n", id);
	while(1) {
// 		printf("\t\t\t\tTrash : wait on wait_throw %d\n", id);
		pthread_cond_wait(wait_throw[id], mutexTrashes+id);		// Wait that a customer put something in the trash
// 		printf("\t\t\t\tTrash : end of wait on wait_throw %d\n", id);
		float percentage = (1.*trashes[id]->wasteQuantity)/(trashes[id]->capacity);
		if(percentage > 0.70) {			// If the trash is filled at more than 70%, we add it to the trashesToEmpty list.
			
			printf("\n\t\tPoubelle %d presque pleine (%.2f) !", id, percentage);
// 			printf("\t\t\taddTrash : call addTrash %d\n", id);
			addTrashToList(id);
			
// 			printf("\t\t\taddTrash : end of call addTrash %d\n", id);
		}

	}
	pthread_mutex_unlock(mutexTrashes+id);
// 	printf("\t\t\taddTrash : UNlocked mutex %d\n", id);
	pthread_exit(0);
}


#include "home.h"
#include "globalVariables.h"
#include "trash.h"

#include <math.h>


/*
 * Thread home
 */
void *home(void *arg) {
	
	// Initialization
	
	int id = (int)arg;	// Unique id of the home
	int idPersonalTrash = -1;	// Id of the individual trash
	int idSortingCenter = -1;	// Id of the first trash of the associated sorting center
	
	int i;
	
	bool hasKey = false;
	bool hasIndivTrash = false;	
	bool hasThrown;
	
	GarbageBag garbageBag[NB_WASTE_TYPE];	// Each home has NB_WASTE_TYPE garbage bags
	int tmpGarbage[NB_WASTE_TYPE];	// Used to allow a daily waste production greater than the size of one garbage bag.
	int garbageProductionRate[NB_WASTE_TYPE];	// Waste production rate
	
	int bagFreePlace;
	
	// Compare random number to the proportion of homes with individual trashes
	if(rand()%100 < homesWithIndiv*100)
	{
		hasIndivTrash = true;
		createTrash(id);
		idPersonalTrash = id;
	}
	else {
		initTrashNull(id);
	}
	
	idSortingCenter = (id/homePerSortingCenter%nSortingCenter)*NB_WASTE_TYPE + nHomes;
	if(rand()%100 < homesWithKey*100 || hasIndivTrash == false) {
		hasKey = true;
	}	
	
	/* Array initialisation*/
	for(i=0; i<NB_WASTE_TYPE; i++)
	{
		/* Random from 20 to 30 */
		garbageProductionRate[i] = rand()%10 + 20;		
		garbageBag[i].capacity = 30;	
		tmpGarbage[i] = 0;	
	}
		
	printf("Client %d\tPoubelle privée = %d\tCentre de tri = %d\n", id, idPersonalTrash, idSortingCenter);	
	
	// Wait for the initialization of all threads
	pthread_mutex_lock(&firstWaitMutex);
	pthread_cond_wait(&firstWait, &firstWaitMutex);
	pthread_mutex_unlock(&firstWaitMutex);

	
	while(1)
	{	
		/* Sleep for a day and then add waste in the garbage bag */
		usleep(1000 * DAY_DURATION);
		
		for(i=0; i<NB_WASTE_TYPE; i++)
		{
			hasThrown = true;
			tmpGarbage[i] += garbageProductionRate[i];
			
			while(tmpGarbage[i] > 0)
			{
				bagFreePlace = garbageBag[i].capacity - garbageBag[i].wasteQuantity;
				
				/* If the free place is lesser than the waste quantity */
				if(tmpGarbage[i] < bagFreePlace)
				{
					printf("\n\t\t\t\tClient %d dépose %d L dans son sac (type %d).", id, tmpGarbage[i], i);
					garbageBag[i].wasteQuantity += tmpGarbage[i];
					tmpGarbage[i] = 0;
				}
				/* If the free place is bigger than the waste quantity */
				else
				{
					garbageBag[i].wasteQuantity += bagFreePlace;
					tmpGarbage[i] -= bagFreePlace;
					
					/* If it is ordinary waste */
					if(i == 0)
					{
						/* If he has a personal trash */
						if(idPersonalTrash != -1)
						{
							hasThrown = throwWaste(&(garbageBag[i]), idPersonalTrash, id);						
							
							if(!hasThrown)
							{								
								/* If he has a key, he try to go in his sortingCenter */
								if(hasKey)
								{
									hasThrown = throwWaste(&(garbageBag[i]), idSortingCenter, id);
									
									/*if(!hasThrown)
										printf("PROBLEM : The user %d cant put his garbageBag in his trash nor in his sortingCenter !\n", id);
									else
										printf("INFO : The user %d threw his garbageBag.\n", id); */
								}
								/* Else he is stuck with his garbageBag */
								else
									printf("\n\t\t\t\t\t\t\tPROBLEM : Le client %d ne peut pas jeter son sac et ne possède pas de clé !", id);
								usleep(300);
							}
							else {
// 								printf("\n\t\t\t\t\t\t\tINFO : The user %d threw his garbageBag.\n", id);
							}
						}
						/* Else he goes to his sortingCenter */
						else
						{
							hasThrown = throwWaste(&(garbageBag[i]), idSortingCenter, id);
							
							/*if(!hasThrown)
								printf("PROBLEM : The user %d cant put his garbageBag in his sortingCenter !\n", id);
							else
								printf("INFO : The user %d threw his garbageBag.\n", id);*/
						}
					}
					/* Else, he goes to the sorting center */
					else
					{
						hasThrown = throwWaste(&(garbageBag[i]), idSortingCenter+i, id);						
						
						/*if(!hasThrown)
							printf("PROBLEM : The user %d cant put his garbageBag in his sortingCenter!\n", id);
						else
							printf("INFO : The user %d threw his garbageBag.\n", id);
						*/
					}
				}
				
				if(!hasThrown)
				{
					//printf("PROBLEM : The user %d have to wait !\n", id);
					usleep(500);
				}
			}
		}
	}	
	
	pthread_exit(0);		//pthread_exit create a memory leak
}

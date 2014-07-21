#ifndef __TRASH__
#define __TRASH__

#include "globalVariables.h"

// Create and initialize trashes
void * trash(void* arg);
void createTrash(int id);
void initTrash(int id, int capa, bool key, WasteType type);
void initTrashNull(int id);
bool throwWaste(GarbageBag *gb, int id, int idHome);

// Manipulate trashes
void emptyTrash(int id);
void addTrashToList(int t);

#endif

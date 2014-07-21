#ifndef __TYPES__
#define __TYPES__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* == Structures == */
/* AbstractList : may also be used as Library */
typedef struct elemAbstract {
	void* value;					
	struct elemAbstract* next;
	struct elemAbstract* prev;
} AbstractElement;

typedef struct {
	AbstractElement* head;
	AbstractElement* tail;
	int count;
	size_t size;
} AbstractList;


/* == Functions' prototypes == */
/*
* isEmpty : indicates if the list is empty or not
* 
* list : a generic list
* 
* return a boolean : TRUE if the list is empty, FALSE if the list isnt
*/
bool isEmpty(AbstractList list);

/*
* initList : initialize a list (which is returned) setting his head/tail equal to NULL, his count equal to 0 and his size equal to the given size (function's argument)
* 
* size : since the list is a abstract list (including void*) we must have a list.size which contains the size of the list's element,
         for Example : if it's a list of entryField then we'll have size=sizeof(entryField), if it's a list of publications for then we'll have size=sizeof(Entry)
* 
* return an AbstractList initialized
*/
AbstractList initList(int size);

/*
* insertHead : insert an element or a pointer at the beginning of the list
* 
* list : the list in which we have to put the element/pointer
* element : a pointer on a given element we want to put in the beginning of list
* allocMemory : if allocMemory is equal to TRUE (1) then we create a new element at the beginning of the list and we will copy the value of element into the new created element
                else then we just copy the pointer at the end of the list (which means element is not a temporary element so we can just copy his adress)
*
* return the list with the new element
*/
AbstractList insertHead(AbstractList list, void* element, bool allocMemory);
/*
* removeHead : remove the first element of the list
* 
* list : the list in which we have to remove the first element
* 
* return the list without the first element
*/
AbstractList removeHead(AbstractList list);
/*
* insertTail : insert an element or a pointer at the end of the list
* 
* list : the list in which we have to put the element/pointer
* element : a pointer on a given element we want to put in the end of list
* allocMemory : if allocMemory is equal to TRUE (1) then we create a new element at the beginning of the list and we will copy the value of element into the new created element
                else then we just copy the pointer at the end of the list (which means element is not a temporary element so we can just copy his adress)
*
* return the list with the new element
*/
AbstractList insertTail(AbstractList list, void* element, bool allocMemory);
/*
* removeTail : remove the first element of the list
* 
* list : the list in which we have to remove the first element
* 
* return the list without the last element
*/
AbstractList removeTail(AbstractList list);

AbstractList removeElementFrom(AbstractList list, int val);

void displayListInt(AbstractList list);

#endif

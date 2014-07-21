#include "list.h"



/*
* isEmpty : indicates if the list is empty or not
* 
* list : a generic list
* 
* return a boolean : TRUE if the list is empty, false if the list isnt
*/
bool isEmpty(AbstractList list)
{
	if(list.head == NULL)
		return true;
	else
		return false;
}


/*
* initList : initialize a list (which is returned) setting his head/tail equal to NULL, his count equal to 0 and his size equal to the given size (function's argument)
* 
* size : since the list is a abstract list (including void*) we must have a list.size which contains the size of the list's element,
         for Example : if it's a list of entryField then we'll have size=sizeof(entryField), if it's a list of publications for then we'll have size=sizeof(Entry)
* 
* return an AbstractList initialized
*/
AbstractList initList(int size)
{
	AbstractList l;
	
	l.head = NULL;
	l.tail = NULL;
	l.count = 0;
	l.size = size;
	
	return l;
}



/*
* insertHead : insert an element or a pointer at the beginning of the list
* 
* list : the list in which we have to put the element/pointer
* element : a pointer on a given element we want to put in the beginning of list
* allocMemory : if allocMemory is equal to true (1) then we create a new element at the beginning of the list and we will copy the value of element into the new created element
                else then we just copy the pointer at the end of the list (which means element is not a temporary element so we can just copy his adress)
*
* return the list with the new element
*/
AbstractList insertHead(AbstractList list, void* element, bool allocMemory)
{
	AbstractElement* newEl;
	newEl = (AbstractElement*) malloc(sizeof(AbstractElement));
	
	if(allocMemory == true)
	{
		newEl->value = (void*) malloc(list.size);
		memcpy(newEl->value, element, list.size);
	}
	else		newEl->value = element;
	
	newEl->prev = NULL;
	list.count++;
	
	if(isEmpty(list))
	{
		newEl->next = NULL;
		list.head = newEl;
		list.tail = newEl;
	}
	else
	{
		list.head->prev = newEl;
		newEl->next = list.head;
		list.head = newEl;
	}
	
	return list;
}
/*
* removeHead : remove the first element of the list
* 
* list : the list in which we have to remove the first element
* 
* return the list without the first element
*/
AbstractList removeHead(AbstractList list)
{
	if(!isEmpty(list))	/* If the list is empty, do nothing and return the list */
	{
		if(list.head->next == NULL)	/* The list has only one element */
		{
			free(list.head);
			list.head = NULL;
			list.tail = NULL;
			list.count--;
		}
		else
		{
			list.head = list.head->next;
			free(list.head->prev);
			list.head->prev = NULL;
			list.count--;
		}
	}
	
	return list;
}
/*
* insertTail : insert an element or a pointer at the end of the list
* 
* list : the list in which we have to put the element/pointer
* element : a pointer on a given element we want to put in the end of list
* allocMemory : if allocMemory is equal to true (1) then we create a new element at the beginning of the list and we will copy the value of element into the new created element
                else then we just copy the pointer at the end of the list (which means element is not a temporary element so we can just copy his adress)
*
* return the list with the new element
*/
AbstractList insertTail(AbstractList list, void* element, bool allocMemory)
{
	
	AbstractElement* newEl;
	if(element != NULL)
	{
		newEl = (AbstractElement*) malloc(sizeof(AbstractElement));
		
		if(allocMemory == true)
		{
			newEl->value = (void*) malloc(list.size);
			memcpy(newEl->value, element, list.size);
			
		}
		else		
			newEl->value = element;
		
		newEl->next = NULL;
		list.count++;
		
		if(isEmpty(list))
		{
			newEl->prev = NULL;
			list.head = newEl;
			list.tail = newEl;
		}
		else
		{
			list.tail->next = newEl;
			newEl->prev = list.tail;
			list.tail = newEl;
		}
	}
	return list;
}
/*
* removeTail : remove the first element of the list
* 
* list : the list in which we have to remove the first element
* 
* return the list without the last element
*/
AbstractList removeTail(AbstractList list)
{
	if(!isEmpty(list))	/* If the list is empty, do nothing and return the list */
	{
		if(list.tail->prev == NULL)	/* If the list has only one element */
		{
			free(list.tail);
			list.tail = NULL;
			list.head = NULL;
			list.count--;
		}
		else
		{
			list.tail = list.tail->prev;
			free(list.tail->next);
			list.tail->next = NULL;
			list.count--;
		}
	}
	
	return list;
}

AbstractList removeElementFrom(AbstractList list, int val) {
	AbstractElement* it = list.head;
	if(!isEmpty(list)) {
		while(it != NULL && *((int*)it->value) != val) {
			it = it->next;
		}
		if(it != NULL) {
			if(it == list.head) {
				removeHead(list);
			}
			else if(it == list.tail) {
				removeTail(list);
			}
			else {
				it->prev->next = it->next;
				it->next->prev = it->prev;
			}
		}
	}
	return list;
}



void displayListInt(AbstractList list) {
	AbstractElement* it = list.head;
	while(it != NULL) {
		printf("%d\t", *((int*)it->value));
		it = it->next;
	}
// 	printf("\n");
}
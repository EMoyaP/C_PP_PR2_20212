#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "center.h"

// Initialize a center
void center_init(tHealthCenter* center, const char* cp) {	
	// PR2 Ex 2a
	// Check input data
	assert(center != NULL);

	center->stock.first = NULL;
	center->stock.last = NULL;
	center->stock.count = 0;
	center->cp = (char*) malloc((strlen(cp) + 1) * sizeof(char));
	strcpy(center->cp, cp);
}

// Release a center's data
void center_free(tHealthCenter* center) {	
	// PR2 Ex 2b
	// Check input data
	assert(center != NULL);

	stockList_free(&center->stock);
	if (center->cp != NULL){
		free(center->cp);
		center->cp = NULL;
	}
}

// Initialize a list of centers
void centerList_init(tHealthCenterList* list) {	
	// PR2 Ex 2c
	// Check input data
	assert(list!=NULL);
	
	list->first = NULL;
	list->count = 0;
	

}

// Release a list of centers
void centerList_free(tHealthCenterList* list) {	
	// PR2 Ex 2d
	// Check input data
	assert(list!=NULL);

	center_free(&list->first->elem);
	list->first = NULL;
	free(list->first);
	list->count = 0;
}

// Insert a new center
void centerList_insert(tHealthCenterList* list, const char* cp) {	
	// PR2 Ex 2e	
	// Check input data
	assert(list != NULL);
	
	tHealthCenterNode *pNode = NULL;
	tHealthCenterNode *pPrev = NULL;
	 
    if (list->count == 0) {		
		list->first = (tHealthCenterNode*) malloc(sizeof(tHealthCenterNode));
		center_init(&(list->first->elem), cp);
	
    } else {  
		pNode = list->first;
		pPrev = pNode;                

		while(pNode != NULL && strcmp(pNode->elem.cp, cp) < 0) {             
			pPrev = pNode;
			pNode = pNode->next;
		}

		if (pNode == pPrev) {
            
			list->first = (tHealthCenterNode*) malloc(sizeof(tHealthCenterNode));
			list->first->next = pNode;
			center_init(&(list->first->elem), cp);   

		} else {                        
			pPrev->next = (tHealthCenterNode*) malloc(sizeof(tHealthCenterNode));        
			center_init(&(pPrev->next->elem), cp);
			pPrev->next->next = pNode;           
		}
	}
    list->count ++;
}


// Find a center
tHealthCenter* centerList_find(tHealthCenterList* list, const char* cp) {
	// PR2 Ex 2f
	// Check input data
	assert(list != NULL);

	tHealthCenterNode *pNode = NULL;
    tHealthCenterNode *pCenter = NULL;
	tHealthCenter* aux = NULL;

	pNode = list->first;
    
	while(pNode != NULL && pCenter == NULL) {

		if (strcmp(pNode->elem.cp, cp) == 0) {
			pCenter = pNode;
			aux = &(pCenter->elem);
		}
		pNode = pNode->next;
	}
	return aux;
}


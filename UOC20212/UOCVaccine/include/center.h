#ifndef __CENTER_H__
#define __CENTER_H__

#include "stock.h"

// Health center
typedef struct _tHealthCenter {    
    char* cp;
    tVaccineStockData stock;    
} tHealthCenter;

// Health center list node
typedef struct _tHealthCenterNode {    
    tHealthCenter elem;
    struct _tHealthCenterNode *next;
} tHealthCenterNode;

// Health center list node
typedef struct _tHealthCenterList {    
    tHealthCenterNode* first;
    int count;
} tHealthCenterList;


// Initialize a center
void center_init(tHealthCenter* center, const char* cp);

// Release a center's data
void center_free(tHealthCenter* center);

// Initialize a list of centers
void centerList_init(tHealthCenterList* list);

// Release a list of centers
void centerList_free(tHealthCenterList* list);

// Insert a new center
void centerList_insert(tHealthCenterList* list, const char* cp);

// Find a center
tHealthCenter* centerList_find(tHealthCenterList* list, const char* cp);


#endif // __CENTER_H__
#ifndef __STOCK__H
#define __STOCK__H

#include "vaccine.h"
#include "date.h"

// Vaccine stock
typedef struct _tVaccineStock {
    tVaccine* vaccine;
    int doses;    
} tVaccineStock;

// Node of a list of vaccine stocks
typedef struct _tVaccineStockNode {    
    tVaccineStock elem;
    struct _tVaccineStockNode* next;
} tVaccineStockNode;

// Daily vaccine stock
typedef struct _tVaccineDailyStock {    
    tDate day;
    tVaccineStockNode* first;
    int count;
    struct _tVaccineDailyStock* next;
} tVaccineDailyStock;

// Stock data
typedef struct _tVaccineStockData {        
    tVaccineDailyStock* first;
    tVaccineDailyStock* last;
    int count;
} tVaccineStockData;


// Initialize a stock list
void stockList_init(tVaccineStockData* list);

// Modify the doses of a certain vaccine
void stockList_update(tVaccineStockData* list, tDate date, tVaccine* vaccine, int doses);

// Get the number of doses for a certain vaccine and date
int stockList_getDoses(tVaccineStockData* list, tDate date, tVaccine* vaccine);

// Release a list of stocks
void stockList_free(tVaccineStockData* list);


///// AUX Methods: Top-down design //////

// Initialize a stock element
void stock_init(tVaccineStock* stock, tVaccine* vaccine, int doses);

// Get the number of doses for a given vaccine
int stockNode_getDoses(tVaccineStockNode* stock, tVaccine* vaccine);

// Initialize a stock node element
void stockNode_init(tVaccineStockNode* node, tVaccine* vaccine, int doses);

// Initialize a daily stock element
void dailyStock_init(tVaccineDailyStock* stock, tDate date);

// Remove a daily stock element data
void dailyStock_free(tVaccineDailyStock* stock);

// Find a vaccine node for a given daily stock
tVaccineStockNode* dailyStock_find(tVaccineDailyStock* stock, tVaccine* vaccine);

// Remove all vaccine entries with no doses
void dailyStock_purge(tVaccineDailyStock* stock);

// Update the number of doses for a given vaccine
void dailyStock_update(tVaccineDailyStock* stock, tVaccine* vaccine, int doses);

// Copy the contents from source to destination
void dailyStock_copy(tVaccineDailyStock* src, tVaccineDailyStock* dst);

// Find the stock for a given date
tVaccineDailyStock* stockList_find(tVaccineStockData* list, tDate date);

// Extend the list adding empty day cells on left
void stockList_expandLeft(tVaccineStockData* list, tDate date);

// Extend the list to the right with the data of the last position
void stockList_expandRight(tVaccineStockData* list, tDate date);

// Remove entries with no data on the start and end of the list
void stockList_purge(tVaccineStockData* list);



#endif // __STOCK__H
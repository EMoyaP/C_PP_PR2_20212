#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "stock.h"

// Initialize a stock list
void stockList_init(tVaccineStockData* list) {
	// PR2 Ex 1a
	// Check input data
	assert(list != NULL);

	// Initialize list
	list->first = NULL;
	list->last = NULL;
	// Initialize count
	list->count = 0;
}

// Modify the doses of a certain vaccine
void stockList_update(tVaccineStockData* list, tDate date, tVaccine* vaccine, int doses) {
    // PR2 Ex 1b
	// Check input data
	assert(list != NULL);
	assert(vaccine != NULL);

	tVaccineDailyStock* dStock = NULL;
	tVaccineStockNode* sNode = NULL;
	tVaccineStockNode* sNodeAdd = NULL;
	tVaccineStockNode* loop = NULL;

	if(list->count == 0) {
		dStock = (tVaccineDailyStock*) malloc(sizeof(tVaccineDailyStock));
		dailyStock_init(dStock, date);
		sNode = (tVaccineStockNode*) malloc(sizeof(tVaccineStockNode));
		stockNode_init(sNode, vaccine, doses);
		dStock->first = sNode;
		dStock->count++;
		list->first = dStock;
		list->last = dStock;
		list->count++;

	} else if(date_cmp(list->last->day, date) < 0) {
		stockList_expandRight(list, date);
		dailyStock_update(list->last, vaccine, doses);

	} else if(date_cmp(list->first->day, date) > 0) {
		stockList_expandLeft(list, date);
		dStock = list->first;

		while(dStock != NULL && (date_cmp(dStock->day, date) >= 0)) {
			dailyStock_update(dStock, vaccine, doses);
			dStock = dStock->next;
		}
		free(dStock);

	} else {	
		while(date_cmp(date, list->last->day) <= 0) {
			 dStock = stockList_find(list, date);
			 if((dStock != NULL) && (dStock->count > 0)) {
				 tVaccineStockNode* sNode = dailyStock_find(dStock, vaccine);
				 if(sNode == NULL) {
					sNodeAdd = (tVaccineStockNode*) malloc(sizeof(tVaccineStockNode));
					stock_init(&sNodeAdd->elem, vaccine, doses);
					sNodeAdd->next = NULL;
					loop = dStock->first;
					if (strcmp(loop->elem.vaccine->name, vaccine->name) > 0) {
						dStock->first = sNodeAdd;
						sNodeAdd->next = loop;
					} else {
						bool added = false;
						while(loop->next != NULL) {
							if (strcmp(loop->next->elem.vaccine->name, vaccine->name) > 0) {
								loop->next = sNodeAdd;
								sNodeAdd->next = loop->next;
								added = true;
								break;
							}
						}
						if(!added) {
							loop->next = sNodeAdd;
						}
					}
					dStock->count++;
				} else {
					dailyStock_update(dStock, vaccine, doses);
				 }
			 }
			 date_addDay(&date, 1);
		}
	}

}

// Get the number of doses for a certain vaccine and date
int stockList_getDoses(tVaccineStockData* list, tDate date, tVaccine* vaccine) {
	// PR2 Ex 1c 
	// Check input data
	assert(list != NULL);
	assert(vaccine != NULL);
	tVaccineDailyStock* dStock = NULL;
	tVaccineStockNode* sNode = NULL;

    if(date_cmp(date, list->first->day) < 0) {
		return 0;
	}

	if(date_cmp(date, list->last->day) > 0) {
		dStock = list->last;
	} else {
		dStock = stockList_find(list, date);
	}
		sNode = dailyStock_find(dStock, vaccine);
	if (sNode == NULL) {
		return 0;
	} else {
		return stockNode_getDoses(sNode, vaccine);
	}
}

// Release a list of stocks
void stockList_free(tVaccineStockData* list) {
	// PR2 Ex 2b
	// Check input data
	assert(list != NULL);

    list->first = NULL;
	list->last = NULL;
	list->count = 0;
}

///// AUX Methods: Top-down design //////

// Initialize a stock element
void stock_init(tVaccineStock* stock, tVaccine* vaccine, int doses) {
	// Check input data
	assert(stock != NULL);
	assert(vaccine != NULL);

	// Initialize stock
	stock->vaccine = vaccine;
	stock->doses = doses;
}

// Get the number of doses for a given vaccine
int stockNode_getDoses(tVaccineStockNode* stock, tVaccine* vaccine) {
	// Check input data
	assert(stock != NULL);
    assert(vaccine != NULL);

	int doses = 0;
	tVaccineStockNode* aux = NULL;
	aux = stock;

	while(aux != NULL) {
		if(strcmp(aux->elem.vaccine->name, vaccine->name) == 0) {
			doses += aux->elem.doses;
		}
		aux = aux->next;
	}
	return doses;
}

// Initialize a stock node element
void stockNode_init(tVaccineStockNode* node, tVaccine* vaccine, int doses) {
	// Check input data
	assert(node != NULL);
    assert(vaccine != NULL);

	tVaccineStock* vStock = NULL;

	vStock = (tVaccineStock*) malloc(sizeof(tVaccineStock));

	stock_init(vStock, vaccine, doses);
	node->elem = *vStock;
	node->next = NULL;
	free(vStock);
}

// Initialize a daily stock element
void dailyStock_init(tVaccineDailyStock* stock, tDate date) {
	// Check input data
	assert(stock != NULL);
    // Check input data
	assert(date.day >=1 && date.day <=31);
	assert(date.month >=1 && date.month <=12);   
	// Initialize date
	stock->day.day = date.day;
	stock->day.month = date.month;
	stock->day.year = date.year;
	// Initialize first node
	stock->first = NULL;
	// Initialize next node
	stock->next = NULL;
	// Initialize count
	stock->count = 0;
}

// Remove a daily stock element data
void dailyStock_free(tVaccineDailyStock* stock) {
	// Check input data
	assert(stock != NULL);

	stock->first = NULL;
	stock->next = NULL;
	stock->count = 0;
}

// Find a vaccine node for a given daily stock
tVaccineStockNode* dailyStock_find(tVaccineDailyStock* stock, tVaccine* vaccine) {
	// Check input data
	assert(stock != NULL);
	assert(vaccine != NULL);

	tVaccineStockNode* aux = NULL;
	aux = stock->first;

	while(aux != NULL) {
		if(strcmp(aux->elem.vaccine->name, vaccine->name) == 0)  {
			return aux;
		}
		aux = aux->next;
	}
	return NULL;
}

// Remove all vaccine entries with no doses
void dailyStock_purge(tVaccineDailyStock* stock) {
	// Check input data
	assert(stock != NULL);

	tVaccineStockNode* node = NULL;
	tVaccineStockNode* prev = NULL;

	while((stock->first != NULL) && (stock->first->elem.doses == 0)) {
		stock->first = stock->first->next;
		stock->count--;
	}

	prev = stock->first;
	if(prev != NULL ) {
		node = prev->next;
	}
	while(node != NULL) {
		if(node->elem.doses == 0) {
			prev->next = node->next;
			stock->count--;
		} else {
			prev = node;
			node = node->next;
		}
	}

}

// Update the number of doses for a given vaccine
void dailyStock_update(tVaccineDailyStock* stock, tVaccine* vaccine, int doses) {
	// Check input data
	assert(stock != NULL);
	assert(vaccine != NULL);

	tVaccineStockNode* node = NULL;
	tVaccineStockNode* prev = NULL;
	
	node = stock->first;
	bool exist;

	exist = false;

	while(node != NULL) {
		if(strcmp(node->elem.vaccine->name, vaccine->name) == 0)  {
			node->elem.doses += doses;
			if(node->elem.doses == 0) {
				if(!exist) {
					stock->first = node->next;
				} else {
					prev->next = node->next;
				}
				stock->count--;
			}
		}
		exist = true;
		prev = node;
		node = node->next;
	}
}

// Copy the contents from node source to node destination
void vaccineStockNode_copy(tVaccineStockNode* src, tVaccineStockNode* dst) {
    // Check input data
	assert(src != NULL);
	assert(dst != NULL);

	stock_init(&dst->elem, src->elem.vaccine, src->elem.doses);
	if(src->next != NULL) {
		vaccineStockNode_copy(src->next, dst);
		dst->next = dst;
	} else {
		dst->next = NULL;
	}
}
// Copy the contents from node source to node destination
void dailyStock_copy(tVaccineDailyStock* src, tVaccineDailyStock* dst) {
    // Check input data
	assert(src != NULL);
	assert(dst != NULL);

	tVaccineStockNode* nodeDst = NULL;

	nodeDst= (tVaccineStockNode*) malloc(sizeof(tVaccineStockNode));

	vaccineStockNode_copy(src->first, nodeDst);
	dst->first = nodeDst;
	dst->count = src->count;
	dst->day = src->day;

}

// Find the stock for a given date
tVaccineDailyStock* stockList_find(tVaccineStockData* list, tDate date) {
    // Check input data
	assert(list != NULL);

	tVaccineDailyStock* aux = NULL;

	aux = list->first;

	while(aux != NULL) {
		if(date_cmp(aux->day, date) == 0) {
			return aux;
		}
		aux = aux->next;
	}
	return NULL;
}

// Extend the list adding empty day cells on left
void stockList_expandLeft(tVaccineStockData* list, tDate date) {
    // Check input data
	assert(list != NULL);

	tVaccineDailyStock* dStock = NULL;
	tVaccineStockNode* loop = NULL;

	while(date_cmp(list->first->day, date) > 0) {
		dStock = (tVaccineDailyStock*) malloc(sizeof(tVaccineDailyStock));
		dailyStock_copy(list->first, dStock);
		dStock->next = NULL;
		date_addDay(&dStock->day, -1);
		loop = dStock->first;
		while(loop != NULL) {
			loop->elem.doses = 0;
			loop = loop->next;
		}
		dStock->next = list->first;
		list->first = dStock;
		list->count++;
	}

}

// Extend the list to the right with the data of the last position
void stockList_expandRight(tVaccineStockData* list, tDate date) {
    // Check input data
	assert(list != NULL);

	tVaccineDailyStock* dStock = NULL;

	while(date_cmp(list->last->day, date) < 0) {
		dStock = (tVaccineDailyStock*) malloc(sizeof(tVaccineDailyStock));
		dailyStock_copy(list->last, dStock);
		dStock->next = NULL;
		date_addDay(&dStock->day, 1);
		list->last->next = dStock;
		list->last = dStock;
		list->count++;
	}

}

// Remove all vaccine entries with no doses
void stockList_purge(tVaccineStockData* list) {
    // Check input data
	assert(list != NULL);
	tVaccineDailyStock* loop = NULL;

	dailyStock_purge(list->first);
	loop = (tVaccineDailyStock*) malloc(sizeof(tVaccineDailyStock));
	loop = list->first->next;
	while(loop != NULL) {
		dailyStock_purge(loop);
		loop = loop->next;
	}
	free(loop);
}
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test_pr2.h"
#include "api.h"

// Run all tests for PR1
bool run_pr2(tTestSuite* test_suite, const char* input) {
    bool ok = true;
    tTestSection* section = NULL;

    assert(test_suite != NULL);

    testSuite_addSection(test_suite, "PR2", "Tests for PR2 exercices");

    section = testSuite_getSection(test_suite, "PR2");
    assert(section != NULL);

    ok = run_pr2_ex1(section, input);
    ok = run_pr2_ex2(section, input) && ok;
    ok = run_pr2_ex3(section, input) && ok;

    return ok;
}

// Run all tests for Exercice 1 of PR1
bool run_pr2_ex1(tTestSection* test_section, const char* input) {    
    tDate date;
    int i;
    tVaccineStockData stock;
    tVaccineDailyStock* pDailyStockNode;    
    tVaccine vModerna, vPfizer, vAAAA;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;
    
    // Initialize sample vaccines
    vPfizer.name = "PFIZER";
    vPfizer.days = 21;
    vPfizer.required = 2;
    vModerna.name = "MODERNA";
    vModerna.days = 0;
    vModerna.required = 1;
    vAAAA.name = "AAAAA";
    vAAAA.days = 27;
    vAAAA.required = 3;
    
    /////////////////////////////
    /////  PR2 EX1 TEST 1  //////
    /////////////////////////////    
    start_test(test_section, "PR2_EX1_1", "Initialize Stock data structure");
    
    // Set incorrect value
    stock.count = -34;
    
    // Initialize the data    
    stockList_init(&stock);
    if (stock.count != 0 || stock.first != NULL || stock.last != NULL) {
        failed = true;
        passed = false;
        fail_all = true;
    }
    end_test(test_section, "PR2_EX1_1", !failed);
    
    /////////////////////////////
    /////  PR2 EX1 TEST 2  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_2", "Add vaccine lot to empty stock");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "02/01/2022");
        stockList_update(&stock, date, &vModerna, 50);
        
        // Check result
        if (stock.count != 1) {
            failed = true;
        } else if (stock.first->count != 1 || stock.first == NULL || stock.last == NULL) {
            failed = true;
        } else if (date_cmp(stock.first->day, date) != 0) {
            failed = true;
        } else if (stock.first->count != 1 || stock.first->first == NULL || stock.last->first == NULL) {
            failed = true;
        } else if (stock.first->first->elem.vaccine != &vModerna || stock.first->first->elem.doses != 50 || stock.first->first->next != NULL) {
            failed = true;
        } else if (stock.last->first->elem.vaccine != &vModerna || stock.last->first->elem.doses != 50 || stock.last->first->next != NULL) {
            failed = true;
        }
        
        if (failed) {
            fail_all = true;
            passed = false;
        }        
    }
    end_test(test_section, "PR2_EX1_2", !failed);
    
    
    /////////////////////////////
    /////  PR2 EX1 TEST 3  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_3", "Add vaccine lot with days step");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "05/01/2022");
        stockList_update(&stock, date, &vModerna, 10);
        
        // Check result
        if (stock.count != 4) {
            failed = true;
        } else {
            date_parse(&date, "02/01/2022");            
            pDailyStockNode = stock.first;
            for (i=0; i<4 && !failed; i++) {
                date.day = i + 2;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->count != 1 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<3 && pDailyStockNode->first->elem.doses != 50) {
                        failed = true;                        
                    } else if (i==3 && pDailyStockNode->first->elem.doses != 60) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_3", !failed);
    
    /////////////////////////////
    /////  PR2 EX1 TEST 4  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_4", "Add vaccine before previous stocks");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "01/01/2022");
        stockList_update(&stock, date, &vModerna, 2);
        
        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i=0; i<5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->count != 1 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2) {
                        failed = true;                        
                    } else if (i>0 && i<4 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;                        
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 62) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_4", !failed);
   
    /////////////////////////////
    /////  PR2 EX1 TEST 5  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_5", "Add other vaccine");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "03/01/2022");
        stockList_update(&stock, date, &vPfizer, 30);
        
        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i=0; i<5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2) {
                        failed = true;                        
                    } else if (i>0 && i<4 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;            
                    } else if (i>=2 && pDailyStockNode->count != 2) {
                        failed = true;
                    } else if (i<2 && pDailyStockNode->count != 1) {
                        failed = true;                    
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 62) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_5", !failed);
    
    /////////////////////////////
    /////  PR2 EX1 TEST 6  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_6", "Decrease the number of vaccines reverting last test");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "03/01/2022");
        stockList_update(&stock, date, &vPfizer, -30);
        
        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i=0; i<5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->count != 1 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2) {
                        failed = true;                        
                    } else if (i>0 && i<4 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;                        
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 62) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_6", !failed);
   
    /////////////////////////////
    /////  PR2 EX1 TEST 7  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_7", "Decrease the number of vaccines removing stocks in the middle");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "03/01/2022");
        stockList_update(&stock, date, &vModerna, -52);
        
        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i=0; i<5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0) {
                        failed = true;
                    } else if((i<2 || i >3) && (pDailyStockNode->count != 1 || pDailyStockNode->first == NULL)) {
                        failed = true;                    
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2 ) {
                        failed = true;                        
                    } else if (i>0 && i<2 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;                        
                    } else if (i>=2 && i<4 && pDailyStockNode->count != 0) {
                        failed = true;                        
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 10) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_7", !failed);
   
    /////////////////////////////
    /////  PR2 EX1 TEST 8  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_8", "Request the number of doses");
    
    if (!fail_all) {
        // Parse first date                
        date_parse(&date, "01/01/2022");
        if (stockList_getDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getDoses(&stock, date, &vModerna) != 2) {
            failed = true;
        }
        // Parse other date                
        date_parse(&date, "02/01/2022");
        if (stockList_getDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getDoses(&stock, date, &vModerna) != 52) {
            failed = true;
        }
        // Parse other date                
        date_parse(&date, "03/01/2022");
        if (stockList_getDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getDoses(&stock, date, &vModerna) != 0) {
            failed = true;
        }
        // Parse non existing date from before the stock range
        date_parse(&date, "30/12/2021");
        if (stockList_getDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getDoses(&stock, date, &vModerna) != 0) {
            failed = true;
        }
        
        // Parse non existing date from after the stock range
        date_parse(&date, "30/01/2022");
        if (stockList_getDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getDoses(&stock, date, &vModerna) != 10) {
            failed = true;
        }
        

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_8", !failed);
    
    /////////////////////////////
    /////  PR2 EX1 TEST 9  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_9", "Add a new vaccine to check stock order");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "01/01/2022");
        stockList_update(&stock, date, &vAAAA, 12);
        
        // Check result
        if (stock.count != 5) {
            failed = true;
        } else if (stock.first->count != 2) {
            failed = true;
        } else if (stock.first->first->elem.vaccine != &vAAAA) {
            failed = true;
        } else if (stock.first->first->next->elem.vaccine != &vModerna) {
            failed = true;
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX1_9", !failed);

    // Release used memory
    stockList_free(&stock);
        
    return passed;
}
    
// Run all tests for Exercice 2 of PR2
bool run_pr2_ex2(tTestSection* test_section, const char* input) {    
    tDate date;    
    tVaccine vModerna;
    tHealthCenterList list;    
    tHealthCenter center;
    tHealthCenter *pCenter;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;
    
    // Initialize sample vaccines    
    vModerna.name = "MODERNA";
    vModerna.days = 0;
    vModerna.required = 1;
        
    /////////////////////////////
    /////  PR2 EX2 TEST 1  //////
    /////////////////////////////    
    start_test(test_section, "PR2_EX2_1", "Initialize a center");
    
    // Set incorrect value
    center.stock.count = -34;
    
    // Initialize the data    
    center_init(&center, "08016");
    if (center.stock.count != 0 || strcmp(center.cp, "08016") != 0) {
        failed = true;
        passed = false;
        fail_all = true;
    }
    end_test(test_section, "PR2_EX2_1", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 2  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_2", "Remove center data");
    
    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "01/01/2022");
        stockList_update(&(center.stock), date, &vModerna, 12);
        
        // Clean center data
        center_free(&center);
        if (center.cp != NULL) {
            failed = true;
        } else if (center.stock.count != 0) {
            failed = true;
        } else if (center.stock.first != NULL) {
            failed = true;
        } else if (center.stock.last != NULL) {
            failed = true;
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }        
    } 
    end_test(test_section, "PR2_EX2_2", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 3  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_3", "Initialize list of health centers");
    
    if (!fail_all) {
        // Set incorrect value
        list.count = -34;
    
        // Initialize the data    
        centerList_init(&list);
        if (list.count != 0 || list.first != NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    } 
    end_test(test_section, "PR2_EX2_3", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 4  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_4", "Insert a health center to an empty list");
    
    if (!fail_all) {       
    
        // Initialize the data    
        centerList_init(&list);
        
        // Insert a new center
        centerList_insert(&list, "08004");        
        
        if (list.count != 1 || list.first == NULL || strcmp(list.first->elem.cp, "08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    } 
    end_test(test_section, "PR2_EX2_4", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 5  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_5", "Test CP sorting");
    
    if (!fail_all) {       
        // Insert a new center
        centerList_insert(&list, "08005");        
        
        if (list.count != 2 || list.first == NULL || strcmp(list.first->elem.cp, "08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        } else if (list.first->next == NULL || strcmp(list.first->next->elem.cp, "08005") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }       
        
        // Insert a new center
        centerList_insert(&list, "08001");        
        
        if (list.count != 3 || list.first == NULL || strcmp(list.first->elem.cp, "08001") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        } else if (list.first->next == NULL || strcmp(list.first->next->elem.cp, "08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        } else if (list.first->next->next == NULL || strcmp(list.first->next->next->elem.cp, "08005") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    } 
    end_test(test_section, "PR2_EX2_5", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 6  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_6", "Find an existing health center by CP");
    
    if (!fail_all) {
        
        // Initialize the data    
        pCenter = centerList_find(&list, "08001");
        if (pCenter == NULL || strcmp(pCenter->cp, "08001") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    } 
    end_test(test_section, "PR2_EX2_6", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 7  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_7", "Find a non existing health center by CP");
    
    if (!fail_all) {
        
        // Initialize the data    
        pCenter = centerList_find(&list, "08500");
        if (pCenter != NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    } 
    end_test(test_section, "PR2_EX2_7", !failed);
    
    /////////////////////////////
    /////  PR2 EX2 TEST 8  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_8", "Release a list of health centers");
    
    if (!fail_all) {
        
        // Initialize the data    
        centerList_free(&list);
        if (list.count != 0 || list.first != NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }        
    } 
    end_test(test_section, "PR2_EX2_8", !failed);
       
    
    return passed;
}

// Run all tests for Exercice 3 of PR2
bool run_pr2_ex3(tTestSection* test_section, const char* input) {    
    tApiData data;
    tApiError error;    
    int nLots;
    int nVaccines;
    int nPeople;
    int nCenters;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;
    
    /////////////////////////////
    /////  PR2 EX3 TEST 1  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR2_EX3_1", "Initialize the API data");
    // Initialize the data    
    error = api_initData(&data);
    if (error != E_SUCCESS) {        
        passed = false; 
        failed = true;
        fail_all = true;
    } else if (api_centersCount(data) != 0) {
        passed = false; 
        failed = true;
        fail_all = true;
    }
        
    end_test(test_section, "PR2_EX3_1", !failed);
    
    
    /////////////////////////////
    /////  PR2 EX3 TEST 2  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX3_2", "Load API data");
    if (!fail_all) {
        error = api_loadData(&data, input, true);
        nVaccines = api_vaccineCount(data);
        nLots = api_vaccineLotsCount(data);
        nPeople = api_populationCount(data);
        nCenters = api_centersCount(data);
        if (error != E_SUCCESS || nVaccines != 2 || nLots != 5 || nPeople != 2) {            
            passed = false; 
            failed = true;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX3_2", !failed);
    
    /////////////////////////////
    /////  PR2 EX3 TEST 3  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX3_3", "Obtain number of centers in the API");
    if (!fail_all) {        
        if (nCenters != 2) {            
            passed = false; 
            failed = true;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX3_3", !failed);
    
    // Release all data
    api_freeData(&data);
    
    return passed;
}

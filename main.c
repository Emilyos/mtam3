/*
 * ramiz_list_test.c
 *
 *  Created on: 26 ���� 2017
 *      Author: danazoabi
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list_mtm.h"
#include "test_utilities.h"

typedef struct city *City;
struct city{
    char* name;
    int people_population;
};

static City cityCreate(char* name,int people_population){
    if(!name){
        return NULL;
    }
    City new_city = malloc(sizeof(*new_city));
    if(!new_city){
        return NULL;
    }
    new_city->name = malloc(strlen(name)+1);
    if(!new_city->name){
        free(new_city);
        return NULL;
    }
    strcpy(new_city->name,name);
    new_city->people_population=people_population;
    return new_city;
}

static void cityDestroy(ListElement element) {
    City city=(City)element;
    free(city->name);
    free(city);
}

static ListElement cityCopy(ListElement element) {
    if (!element) {
        return NULL;
    }
    City city = (City)element;
    return cityCreate( city->name, city->people_population);
}

static bool citycompare(ListElement element1, ListElement element2) {
    City city1 = (City)element1;
    City city2 = (City)element2;
    if(strcmp(city1->name, city2->name)){
        return false;
    }
    return !(city1->people_population - city2->people_population);
}

static int compareElement(ListElement element1, ListElement element2,ListSortKey key){
    City city1 = (City)element1;
    City city2 = (City)element2;
    int key_casted =* (int*) key;
    return city1->people_population - city2->people_population - key_casted;
}

static bool isHighter(ListElement element, ListFilterKey people_population) {
    City city = (City)element;
    if( city->people_population > *(int*)people_population){
        return true;
    }
    else {
        return false;
    }
}

static List createFilteredList(List list) {
    int key = 500;
    return listFilter(list, isHighter, &key);
}

static bool testListCreate() {
    List list=listCreate(cityCopy,cityDestroy);
    ASSERT_TEST(list!=NULL);
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(cityCopy,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,cityDestroy) == NULL);
    listDestroy(list);
    return true;
}

static bool testListCopy() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==4);
    List copy1=listCopy(list);
    ASSERT_TEST(listGetSize(copy1)==4);
    City city_copy = listGetFirst(copy1);
    ASSERT_TEST(citycompare(city3, city_copy)==true);
    city_copy = listGetNext(copy1);
    ASSERT_TEST(citycompare(city1, city_copy)==true);
    city_copy = listGetNext(copy1);
    ASSERT_TEST(citycompare(city2, city_copy)==true);
    city_copy = listGetNext(copy1);
    ASSERT_TEST(citycompare(city4, city_copy)==true);
    List copy2=listCopy(NULL);
    ASSERT_TEST(copy2==NULL);
    listDestroy(list);
    listDestroy(copy1);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListGetSize() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,city1)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==1);
    ASSERT_TEST(listInsertLast(list,city2)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==2);
    ASSERT_TEST(listInsertFirst(list,city3)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==3);
    ASSERT_TEST(listInsertLast(list,city4)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==4);
    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==3);
    listGetFirst(list);
    ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==2);
    ASSERT_TEST(listInsertLast(list,city4)==LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list)==3);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListGetFirst() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    City first = listGetFirst(list);
    ASSERT_TEST(citycompare(city3, first)==true);
    listRemoveCurrent(list);
    first = listGetFirst(list);
    ASSERT_TEST(citycompare(city1, first)==true);
    listRemoveCurrent(list);
    first = listGetFirst(list);
    ASSERT_TEST(citycompare(city2, first)==true);
    listRemoveCurrent(list);
    first = listGetFirst(list);
    ASSERT_TEST(citycompare(city4, first)==true);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListGetNext() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    City current = listGetFirst(list);
    ASSERT_TEST(citycompare(city3, current)==true);
    current = listGetNext(list);
    ASSERT_TEST(citycompare(city1, current)==true);
    current = listGetNext(list);
    ASSERT_TEST(citycompare(city2, current)==true);
    current = listGetNext(list);
    ASSERT_TEST(citycompare(city4, current)==true);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListInsertFirst() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(NULL,(ListElement)city1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city4) == LIST_SUCCESS);
    City first = listGetFirst(list);
    ASSERT_TEST(citycompare(city4, first)==true);
    first = listGetNext(list);
    ASSERT_TEST(citycompare(city3, first)==true);
    first = listGetNext(list);
    ASSERT_TEST(citycompare(city2, first)==true);
    first = listGetNext(list);
    ASSERT_TEST(citycompare(city1, first)==true);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListInsertLast() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertLast(NULL,(ListElement)city1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertLast(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    City first = listGetFirst(list);
    ASSERT_TEST(citycompare(city1, first)==true);
    first = listGetNext(list);
    ASSERT_TEST(citycompare(city2, first)==true);
    first = listGetNext(list);
    ASSERT_TEST(citycompare(city3, first)==true);
    first = listGetNext(list);
    ASSERT_TEST(citycompare(city4, first)==true);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListInsertBeforeCurrent() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertBeforeCurrent(NULL, (ListElement)city1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(list, (ListElement)city1) == LIST_SUCCESS);
    ListElement current = listGetFirst(list);
    City city = (City)current;
    ASSERT_TEST(city->people_population == 588 );
    ASSERT_TEST(listInsertBeforeCurrent(list, (ListElement)city2) == LIST_SUCCESS);
    current = listGetFirst(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 546 );
    ASSERT_TEST(listInsertBeforeCurrent(list,(ListElement)city3) == LIST_SUCCESS);
    current = listGetFirst(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 417 );
    ASSERT_TEST(listInsertBeforeCurrent(list,(ListElement)city4) == LIST_SUCCESS);
    current = listGetFirst(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 835 );
    current = listGetNext(list);
    current = listGetNext(list);
    current = listGetNext(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 588 );
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}


static bool testListInsertAfterCurrent() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertAfterCurrent(NULL, (ListElement)city1) == LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertFirst(list, (ListElement)city1) == LIST_SUCCESS);
    ListElement current = listGetFirst(list);
    City city = (City)current;
    ASSERT_TEST(city->people_population == 588 );
    ASSERT_TEST(listInsertAfterCurrent(list, (ListElement)city2) == LIST_SUCCESS);
    current = listGetNext(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 546 );
    ASSERT_TEST(listInsertAfterCurrent(list,(ListElement)city3) == LIST_SUCCESS);
    current = listGetNext(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 417 );
    ASSERT_TEST(listInsertAfterCurrent(list,(ListElement)city4) == LIST_SUCCESS);
    current = listGetNext(list);
    city = (City)current;
    ASSERT_TEST(city->people_population == 835 );
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListRemoveCurrent(){
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    ASSERT_TEST(listRemoveCurrent(list)==LIST_INVALID_CURRENT);
    ASSERT_TEST(listRemoveCurrent(NULL)==LIST_NULL_ARGUMENT);
    City current = listGetFirst(list);
    ASSERT_TEST(citycompare(city1, current) == true);
    ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
    current = listGetFirst(list);
    ASSERT_TEST(citycompare(city2, current) == true);
    current = listGetNext(list);
    ASSERT_TEST(citycompare(city3, current) == true);
    ASSERT_TEST(listRemoveCurrent(list)==LIST_SUCCESS);
    current = listGetFirst(list);
    current = listGetNext(list);
    ASSERT_TEST(citycompare(city4, current) == true);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListSort() {
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    ASSERT_TEST(listSort(NULL,compareElement,0) == LIST_NULL_ARGUMENT);
    int key = 0;
    ASSERT_TEST(listSort(list,compareElement,&key) == LIST_SUCCESS);
    City current = (City)listGetFirst(list);
    ASSERT_TEST(current->people_population == 417);
    current = (City)listGetNext(list);
    ASSERT_TEST(current->people_population == 546);
    current = (City)listGetNext(list);
    ASSERT_TEST(current->people_population == 588);
    current = (City)listGetNext(list);
    ASSERT_TEST(current->people_population == 835);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListFilter(){
    List list1 = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    listInsertFirst(list1,(ListElement)city1);
    listInsertLast(list1,(ListElement)city2);
    listInsertLast(list1,(ListElement)city3);
    listInsertLast(list1,(ListElement)city4);
    ASSERT_TEST(listGetSize(list1) == 4);
    List list2 = createFilteredList(list1);
    ASSERT_TEST(listGetSize(list2) == 3);
    listDestroy(list1);
    listDestroy(list2);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListClear(){
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazareth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 4);
    ASSERT_TEST(listClear(list) == LIST_SUCCESS);
    ASSERT_TEST(listGetSize(list) == 0);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

static bool testListDestroy(){
    List list = listCreate(cityCopy,cityDestroy);
    City city1 = cityCreate("Tel Aviv", 588);
    City city2= cityCreate("Haifa", 546);
    City city3 = cityCreate("Nazarth", 417);
    City city4 = cityCreate("Jeruesalem", 835);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city1) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city2) == LIST_SUCCESS);
    ASSERT_TEST(listInsertFirst(list,(ListElement)city3) == LIST_SUCCESS);
    ASSERT_TEST(listInsertLast(list,(ListElement)city4) == LIST_SUCCESS);
    listDestroy(list);
    cityDestroy(city1);
    cityDestroy(city2);
    cityDestroy(city3);
    cityDestroy(city4);
    return true;
}

int main() {
    RUN_TEST(testListCreate);
    RUN_TEST(testListCopy);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListGetFirst);
    RUN_TEST(testListGetNext);
    RUN_TEST(testListInsertFirst);
    RUN_TEST(testListInsertLast);
    RUN_TEST(testListInsertBeforeCurrent);
    RUN_TEST(testListInsertAfterCurrent);
    RUN_TEST(testListRemoveCurrent);
    RUN_TEST(testListSort);
    RUN_TEST(testListFilter);
    RUN_TEST(testListClear);
    RUN_TEST(testListDestroy);
    return 0;
}



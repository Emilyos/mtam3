#include <stdlib.h>
#include <assert.h>
#include "list_mtm.h"

typedef struct node_t *Node;

static bool isEmpty(List);

static Node createNode(ListElement, Node, Node);

static ListResult addFirstElement(List, ListElement);

static int bubble(ListElement *, int, CompareListElements, ListSortKey);

static void swapElements(ListElement *, ListElement *);

static void deallocateNode(Node node, FreeListElement freeElement);
/**Type for defining the node**/


/**Struct for Node, containing 3 fields.
 * data, this field holds the data of the Node
 * next points to the next Node in the list
 * previous points to the previous Node in the list.
 * */
struct node_t {
    ListElement data;
    Node next;
    Node previous;
};

struct List_t {
    int len;
    CopyListElement copyElement;
    FreeListElement freeElement;
    Node iterator;
    Node head;
    Node tail;
};

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    if (!copyElement || !freeElement) {
        return NULL;
    }
    List newList = malloc(sizeof(*newList));
    if (!newList) {
        return NULL;
    }
    newList->copyElement = copyElement;
    newList->freeElement = freeElement;
    newList->len = 0;
    newList->iterator = NULL;
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

List listCopy(List list) {
    if (!list) {
        return NULL;
    }

    List newList = listCreate(list->copyElement, list->freeElement);
    if (!newList) {
        return NULL;
    }
    Node current = list->head;
    while (current) {
        listInsertLast(newList, current->data);
        if (current == list->iterator) {
            newList->iterator = current;
        }
        current = current->next;
    }
    return newList;
}

int listGetSize(List list) {
    if (!list) {
        return -1;
    }
    return list->len;
}

ListElement listGetFirst(List list) {
    if (!list || isEmpty(list)) {
        return NULL;
    }
    list->iterator = list->head;
    return list->iterator->data;
}

ListElement listGetNext(List list) {
    if (!list || !list->iterator) {
        return NULL;
    }
    list->iterator = list->iterator->next;
    return list->iterator ? list->iterator->data : NULL;
}

ListElement listGetCurrent(List list) {
    if (!list || !list->iterator) {
        return NULL;
    }
    return list->iterator->data;
}


ListResult listInsertFirst(List list, ListElement element) {
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    if (isEmpty(list)) {
        return addFirstElement(list, element);
    }
    Node nodeElement = createNode(list->copyElement(element), list->head, NULL);
    if (!nodeElement) {
        return LIST_OUT_OF_MEMORY;
    }
    list->len++;
    list->head->previous = nodeElement;
    list->head = nodeElement;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    if (isEmpty(list)) {
        return addFirstElement(list, element);
    }
    Node nodeElement = createNode(list->copyElement(element), NULL, list->tail);
    if (!nodeElement) {
        return LIST_OUT_OF_MEMORY;
    }
    list->len++;
    list->tail->next = nodeElement;
    list->tail = nodeElement;
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    if (!list->iterator) {
        return LIST_INVALID_CURRENT;
    }
    if (list->iterator == list->head) {
        return listInsertFirst(list, element);
    }
    Node nodeElement = createNode(list->copyElement(element), list->iterator, list->iterator->previous);
    if (!nodeElement) {
        return LIST_OUT_OF_MEMORY;
    }
    list->iterator->previous->next = nodeElement;
    list->iterator->previous = nodeElement;
    list->len++;
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element) {

    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    if (!list->iterator) {
        return LIST_INVALID_CURRENT;
    }
    if (list->iterator == list->tail) {
        return listInsertLast(list, element);
    }
    Node nodeElement = createNode(list->copyElement(element), list->iterator->next, list->iterator);
    if (!nodeElement) {
        return LIST_OUT_OF_MEMORY;
    }
    list->iterator->next->previous = nodeElement;
    list->iterator->next = nodeElement;
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
    if (!list) {
        return LIST_NULL_ARGUMENT;
    }
    if (!list->iterator) {
        return LIST_INVALID_CURRENT;
    }
    if (listGetSize(list) > 1) {
        if (list->iterator == list->head) {
            list->head = list->iterator->next;
            list->head->previous = NULL;
        } else if (list->iterator == list->tail) {
            list->tail = list->iterator->previous;
            list->tail->next = NULL;
        } else {
            list->iterator->previous->next = list->iterator->next;
            list->iterator->next->previous = list->iterator->previous;
        }
    }
    deallocateNode(list->iterator, list->freeElement);
    list->iterator = NULL;
    list->len--;
    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement, ListSortKey key) {
    if (!list || !compareElement) {
        return LIST_NULL_ARGUMENT;
    }
    int size = listGetSize(list);
    ListElement *helper_array = malloc(sizeof(*helper_array) * size);
    if (!helper_array) {
        return LIST_OUT_OF_MEMORY;
    }
    int i = 0;
    Node backup = list->iterator;
    LIST_FOREACH(ListElement, iterator, list) {
        helper_array[i++] = iterator;
    }
    int not_sorted = 1;
    while (size > 1 && not_sorted) {
        not_sorted = bubble(helper_array, size--, compareElement, key);
    }
    size = listGetSize(list);
    Node current = list->head;
    for (i = 0; i < size && current; i++) {
        current->data = helper_array[i];
        current = current->next;
    }
    list->iterator = backup;
    free(helper_array);

    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    if (!list || !filterElement) {
        return NULL;
    }
    List newList = listCreate(list->copyElement, list->freeElement);
    if (!newList) {
        return NULL;
    }
    LIST_FOREACH(ListElement, iterator, list) {
        if (filterElement(iterator, key)) {
            listInsertLast(newList, iterator);
        }
    }
    newList->iterator = NULL;
    return newList;
}

ListResult listClear(List list) {
    if (!list) {
        return LIST_NULL_ARGUMENT;
    }
    Node current = list->head;
    while (current) {
        Node tmp = current->next;
        deallocateNode(current, list->freeElement);
        current = tmp;
    }
    list->iterator = NULL;
    list->len = 0;
    return LIST_SUCCESS;
}

void listDestroy(List list) {
    if (list) {
        if (!isEmpty(list)) {
            listClear(list);
        }
        free(list);
    }
}

/**
 * This function is used for swapping ListElements.
 *
 * for swapping between the elements we use a helper (tmo), in order not to lose one of the elements.
 *
 * @param element1 : this is the first element that we want to replace it by the second element.
 * @param element2 : this is the second element that we want to replace it by the first element.
 * */
static void swapElements(ListElement *element1, ListElement *element2) {
    ListElement tmp = *element1;
    *element1 = *element2;
    *element2 = tmp;
}
/**
 * This function is for sorting the list, using the bubble sort algorithm
 * It starts with the first two indexes in the array and checks which one is bigger according to CompareElement,
 * if the bigger one is the in the first index then it swappes the first and the second element. and it continues
 * in the same algorithm until we have a sorted array.
 *
 * @param array : this is the array that we want to sort.
 * @param compareElement : this function indicates which element is bigger than the other one
 * according to the sortKey.
 * @param size : this is the size of the array that we want to sort.
 * @param sortKey : this is the key that according to it the function CompareElement compares between two elements.
 *
 * @return this function returns 0 if the array is sorted ( no swaps are done ) otherwise
 * it returns 1.
 *
 * */
static int bubble(ListElement *array, int size, CompareListElements compareElement, ListSortKey sortKey) {
    int swapped = 0;
    for (int i = 1; i < size; i++) {
        if (compareElement(array[i - 1], array[i], sortKey) > 0) {
            swapElements(&array[i - 1], &array[i]);
            swapped = 1;
        }
    }
    return swapped;
}


/**
 * returns true if the list is empty, checked by the function listGetSize otherwise false
 * */
static bool isEmpty(List list) {
    return listGetSize(list) == 0;
}

/**
 * THis function is used for making a new node.
 *
 *
 * @param data which the new node will hold ( it should not be NULL )
 * @param next this field is a pointer to the next node in the list, this parameter is allowed to be NULL.
 * @param previous this field is a pointer to the previous node in the list ,this parameter is allowed to be NULL
 *
 *
 * @return NULL in case of receiving NULL data or in case of failing in allocating memory. Otherwise,
 * it returns the new node.
 * **/
static Node createNode(ListElement data, Node next, Node previous) {
    if (!data) {
        return NULL;
    }
    Node newNode = malloc(sizeof(*newNode));
    if (!newNode) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = next;
    newNode->previous = previous;
    return newNode;
}

/**
 * This function adds an element to an empty list. ( the first one )
 *
 * @param list: the empty list that we want to add to it the first element.
 * @param listElement : the element that we want to add to the empty list.
 *
 * @return LIST_OUT_OF_MEMORY if we have an error in allocating memory.
 * LIST_SUCCESS if the element is added successfully.
 * */
static ListResult addFirstElement(List list, ListElement listElement) {
    assert(list && listElement && isEmpty(list));
    Node node = createNode(list->copyElement(listElement), NULL, NULL);
    if (!node) {
        return LIST_OUT_OF_MEMORY;
    }
    list->head = node;
    list->tail = node;
    list->len++;
    return LIST_SUCCESS;

}

/**
 * This function is used for deallocating a node.
 * we use the FreeListElement supplied by the user to clear the data.
 *
 * @param freeElement : this is the function that is supplied by the user for freeing the data.
 * @param node : the node that we want to deallocate.
 *
 *
 * */
static void deallocateNode(Node node, FreeListElement freeElement) {
    if (!freeElement || !node) {
        return;
    }
    node->next = NULL;
    node->previous = NULL;
    freeElement(node->data);
    free(node);

}
#include "skiplist.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

//跳表最大层次
#define SKIPLIST_MAX_LEVEL  16
//跳表层次概率P,百分比
#define SKIPLIST_LEVEL_PROBABILITY  25

#define MAX_KEY  (int64_t)(0X7FFFFFFFFFFFFFFF)

#define MIN_KEY  (int64_t)(0X8000000000000001)

static skiplist_node_t NIL_ = { MAX_KEY, NULL, NULL, NULL };
static skiplist_node_t* NIL = &NIL_;

static int randomLevel() {
	srand(time(NULL));
	int r = rand() % 100;
	int level = 1;
	while (r < SKIPLIST_LEVEL_PROBABILITY && level <= SKIPLIST_MAX_LEVEL) {
		level++;
		r = rand() % 100;
	}
	return level;
}

int skiplist_size(skiplist_t* list) {
	return list->size;
}

skiplist_t* skiplist_create() {
	skiplist_t* list = (skiplist_t*)malloc(sizeof(skiplist_t));
	if (list == NULL) {
		return NULL;
	}
	list->max_level = 0;
	list->size = 0;
	list->head = (skiplist_node_t*)malloc(sizeof(skiplist_node_t));
	list->head->key = MIN_KEY;
	list->head->next = NULL;
	list->head->forward = (skiplist_node_t**)malloc(sizeof(skiplist_node_t*)*(SKIPLIST_MAX_LEVEL + 1));
	for (int i = 0; i <= SKIPLIST_MAX_LEVEL; i++) {
		list->head->forward[i] = NIL;
	}
	return list;
}

int skiplist_add_elem(skiplist_t* list, int64_t key, void* newelem) {
	//search first
	if (list == NULL || key == MAX_KEY || key == MIN_KEY) {
		return -1;
	}
	skiplist_node_t* rightmost[SKIPLIST_MAX_LEVEL];
	skiplist_node_t* current = list->head;
	for (int i = list->max_level; i > 0; i--) {
		for (; current->forward[i]->key < key; current = current->forward[i]);
		rightmost[i] = current;
	}
	current = current->forward[1];
	if (current->key == key) { //find existed,add to bucket
		skiplist_node_t* next = current->next;
		current->next = (skiplist_node_t*)malloc(sizeof(skiplist_node_t));
		if (current->next == NULL) {
			return -1;
		}
		current->next->key = key;
		current->next->data = newelem;
		current->next->forward = NULL; //be careful when add
		current->next->next = next;
		list->size++;
		return 0;
	}//else not exist,add new node
	skiplist_node_t* node = (skiplist_node_t*)malloc(sizeof(skiplist_node_t));
	if (node == NULL) {
		return -1;
	}

	node->data = newelem;
	node->key = key;
	node->next = NULL;
	int level = randomLevel();
	node->forward = (skiplist_node_t**)malloc(sizeof(skiplist_node_t*)*(level+1));
	if (node->forward == NULL) {
		free(node);
		return -1;
	}
	if (level > list->max_level) {
		for (int i = level; i > list->max_level; i--) {
			rightmost[i] = list->head;
		}
		list->max_level = level;
	}
	for (int i = level; i > 0; i--) {
		node->forward[i] = rightmost[i]->forward[i];
		rightmost[i]->forward[i] = node;
	}
	list->size++;
	return 0;
}

int skiplist_del_elem(skiplist_t* list, int64_t key) {
	if (list == NULL || key == MAX_KEY || key == MIN_KEY) {
		return -1;
	}
	skiplist_node_t* rightmost[SKIPLIST_MAX_LEVEL];
	skiplist_node_t* current = list->head;
	for (int i = list->max_level; i > 0; i--) {
		for (; current->forward[i]->key < key; current = current->forward[i]);
		rightmost[i] = current;
	}
	current = current->forward[1];
	if (current->key == key) { //found,delete the whole shit
		for (int i = list->max_level; i > 0; i--) {
			if (rightmost[i]->forward[i] == current) {
				rightmost[i]->forward[i] = current->forward[i];
			}
		}
		while (current != NULL) { //free bucket
			skiplist_node_t* next = current->next;
			free(current);
			current = next;
			list->size--;
		}
	}
	while(list->max_level>0 && rightmost[list->max_level]->forward[list->max_level] == NIL) {
		list->max_level--;
	}
	return 0;
}

skiplist_node_t* skiplist_search(skiplist_t* list, int64_t key) {
	if (list == NULL || key == MAX_KEY || key == MIN_KEY) {
		return NULL;
	}
	skiplist_node_t* current = list->head;
	for (int i = list->max_level; i > 0; i--) {
		for (; current->forward[i]->key < key; current = current->forward[i]);
	}
	current = current->forward[1];
	if (current->key == key) {
		return current;
	}
	else {
		return NULL;
	}
}

void skiplist_clear(skiplist_t* list) {
	if (list != NULL) {
		skiplist_node_t* current = list->head->forward[1];
		while (current != NIL) {
			skiplist_node_t* forward = current->forward[1];
			if (current->next != NULL) { //clear bucket
				skiplist_node_t* next = current->next;
				if (current->forward != NULL) {
					free(current->forward);
					current->forward = NULL;
				}
				free(current);
				current = next;
			}
			if (current->forward != NULL) {
				free(current->forward);
				current->forward = NULL;
			}
			free(current);
			current = forward;
		}
		for (int i = list->max_level; i > 0; i--) {
			list->head->forward[i] = NIL;
		}
		list->max_level = 0;
		list->size = 0;
	}
}

void skiplist_destroy(skiplist_t* list) {
	if (list != NULL) {
		skiplist_clear(list);
		free(list->head);
		free(list);
	}
}

void skiplist_traverse(skiplist_t* list, skipList_traverse_func_t traverse_func) {
	if (list == NULL) {
		return;
	}
	skiplist_node_t* current = list->head->forward[1];
	int rank = 0;
	while (current != NIL) {
		rank++;
		skiplist_node_t* forward = current->forward[1];
		if (current->next != NULL) {
			skiplist_node_t* next = current->next;
			traverse_func(rank, current->key, current->data);
			current = next;
		}
		traverse_func(rank, current->key, current->data);
		current = forward;
	}
}
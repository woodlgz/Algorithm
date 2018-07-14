#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_
#include <stdint.h>

typedef void(*SkipListTraverseFunc)(int,int64_t,void*);

typedef struct SkipList_Node_t {
	int64_t key;
	void* data;
	struct SkipList_Node_t** forward;
	struct SkipList_Node_t* next;
} SkipListNode,*PSkipListNode;

typedef struct SkipList_t {
	int max_level;
	PSkipListNode head;
} SkipList,*PSkipList;


PSkipList skiplist_create();

void skiplist_destroy(PSkipList list);

int skiplist_add_elem(PSkipList list, int64_t key, void* newelem);

int skiplist_del_elem(PSkipList list, int64_t key);

PSkipListNode skiplist_search(PSkipList list, int64_t key);

void skiplist_clear(PSkipList list);

void skiplist_traverse(PSkipList list, SkipListTraverseFunc traverse_func);

#endif
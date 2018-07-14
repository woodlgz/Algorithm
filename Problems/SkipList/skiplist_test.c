#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"

void dump(int rank, int64_t key, void* data) {
	printf("%ld,", key);
}

void dumpSkipList(PSkipList skiplist,const char* message) {
	printf("%s[",message);
	skiplist_traverse(skiplist, &dump);
	printf("]\n");
}

int main() {
	int64_t keys[] = { 13,25,324,-124,333,2345,7889,11,-1002,98,10000,1234 };
	PSkipList skiplist = skiplist_create();
	for (int i = 0; i < sizeof(keys) / sizeof(int64_t); i++) {
		skiplist_add_elem(skiplist, keys[i], NULL);
	}
	dumpSkipList(skiplist,"after add operation:");
	skiplist_del_elem(skiplist, 100);
	skiplist_del_elem(skiplist, 333);
	skiplist_del_elem(skiplist, 11);
	dumpSkipList(skiplist, "after delete operation:");
	skiplist_add_elem(skiplist, 100, NULL);
	skiplist_add_elem(skiplist, 324, NULL);
	dumpSkipList(skiplist, "after add operation again:");
	int64_t tosearchs[] = { 1000,10000 };
	for (int i = 0; i < sizeof(tosearchs) / sizeof(int64_t); i++) {
		PSkipListNode node = skiplist_search(skiplist, tosearchs[i]);
		if (node == NULL) {
			printf("node of key %ld not found\n", tosearchs[i]);
		}
		else {
			printf("node of key %ld found\n", tosearchs[i]);
		}
	}
	skiplist_clear(skiplist);
	dumpSkipList(skiplist, "after clear operation:");
	for (int i = 0; i < sizeof(keys) / sizeof(int64_t); i++) {
		skiplist_add_elem(skiplist, keys[i], NULL);
	}
	dumpSkipList(skiplist, "after clear and add operation:");
	skiplist_destroy(skiplist);
}
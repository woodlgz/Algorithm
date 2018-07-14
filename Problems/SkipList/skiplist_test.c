#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "skiplist.h"

void dump(int rank, int64_t key, void* data) {
	printf("%ld,", key);
}

void dumpSkipList(PSkipList skiplist,const char* message) {
	printf("%s[",message);
	skiplist_traverse(skiplist, &dump);
	printf("]\n");
}

int64_t now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)(tv.tv_sec) * 1000 + (int64_t)(tv.tv_usec) / 1000;
}

void pressureTest() {
	srand(time(NULL));
	int count = 100000;
	PSkipList skiplist = skiplist_create();
	int64_t beginTime = now();
	for (int i = 0; i < count; i++) {
		skiplist_add_elem(skiplist, rand(), NULL);
	}
	int64_t endTime = now();
	printf("time collasped %ld ms for %d add operation\n", endTime - beginTime, count);
	beginTime = now();
	for (int i = 0; i < count; i++) {
		skiplist_del_elem(skiplist, rand());
	}
	endTime = now();
	printf("time collasped %ld ms for %d delete operation\n", endTime - beginTime, count);
	beginTime = now();
	for (int i = 0; i < count; i++) {
		skiplist_search(skiplist, rand());
	}
	endTime = now();
	printf("time collapsed %ld ms for %d search operation\n", endTime - beginTime, count);
	int size = skiplist_size(skiplist);
	beginTime = now();
	skiplist_destroy(skiplist);
	endTime = now();
	printf("time collapsed %ld ms for destroy %d size skiplist\n", endTime - beginTime,size);
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
	pressureTest();
}
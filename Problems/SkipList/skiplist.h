#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(*skipList_traverse_func_t)(int, int64_t, void*);

	typedef struct skiplist_node_t {
		int64_t key;
		void* data;
		struct skiplist_node_t** forward;
		struct skiplist_node_t* next;
	} skiplist_node_t;

	typedef struct skiplist_t {
		int max_level;
		skiplist_node_t* head;
		int size;
	} skiplist_t;


	skiplist_t* skiplist_create();

	void skiplist_destroy(skiplist_t* list);

	int skiplist_add_elem(skiplist_t* list, int64_t key, void* newelem);

	int skiplist_del_elem(skiplist_t* list, int64_t key);

	skiplist_node_t* skiplist_search(skiplist_t* list, int64_t key);

	void skiplist_clear(skiplist_t* list);

	void skiplist_traverse(skiplist_t* list, skipList_traverse_func_t traverse_func);

	int skiplist_size(skiplist_t* list);

#ifdef __cplusplus
}
#endif

#endif
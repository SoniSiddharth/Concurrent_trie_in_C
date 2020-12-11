#ifndef TRIE_H
#define TRIE_H
#include <stdbool.h>
#include <pthread.h>

typedef struct node { 
    int value;
    bool is_end;
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_t node_lock;
    #endif
    struct node* children[26];   // Array mapping character to the next node
} _trie_node_t;

typedef _trie_node_t* trie_node_t;

typedef struct {
    trie_node_t head; // The head/start node of trie
    #ifdef _S_LOCK_TRIE
    pthread_mutex_t trie_lock;
    #endif
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_t lock_rw;
    #endif
} _trie_t;

typedef _trie_t* trie_t;

trie_t init_trie(void); 

void insert(trie_t trie, char* key, int value); 

int find(trie_t trie, char* key, int* val_ptr); 

void delete_kv(trie_t trie, char* key);

char** keys_with_prefix(trie_t trie, char* prefix);

void delete_trie(trie_t trie);


#endif
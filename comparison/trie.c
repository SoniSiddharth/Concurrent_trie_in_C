#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include <string.h>

// each alphabet can be represented as a number from 0 to 25
#define CHARACHTER_INDEX(x) ((int)x - (int)'a')
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 
char ar[26] = "abcdefghijklmnopqrstuvwxyz";
// #define ALPHABETS ar[26]

// intializing a new trie
trie_t init_trie(void){

    // creating a new trie
    trie_t new_trie = malloc(sizeof(_trie_node_t));
    
    // creating the head node of the trie
    trie_node_t head_node = NULL;
    head_node = (trie_node_t)malloc(sizeof(_trie_node_t));
    
    // initializing the hoh node lock for head node
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_init(&head_node->node_lock, NULL);
    pthread_mutex_lock(&head_node->node_lock);      // locking the head node
    #endif

    head_node->value = 0;
    head_node->is_end = false;
    
    int i = 0;
    // intitalizing children
    for(i=0; i<26; i++){
        head_node->children[i] = NULL;
    }

    // unlocking the hoh lock over head node
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_unlock(&head_node->node_lock);
    #endif
    
    // initializing single mutex lock (s_lock)
    #ifdef _S_LOCK_TRIE
    if (pthread_mutex_init(&new_trie->trie_lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
    } 
    #endif

    // initializing readers-writer lock (rw_lock) 
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    // pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
    pthread_rwlock_init(&new_trie->lock_rw, NULL);
    #endif
    #endif

    new_trie->head = head_node;
    return new_trie;
} 

// insert a new word into the trie
void insert(trie_t trie, char* key, int value){

    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_wrlock(&trie->lock_rw);
    #endif
    #endif

    int height;     // height of the trie (number of alphabets in a word)
    int key_length = strlen(key);
    // printf("Length of the current string -> %d\n", key_length);
    trie_node_t current_node;
    current_node = trie->head;

    int ptr = 0;
    for(height=0; height<key_length; height++){
        ptr  = CHARACHTER_INDEX(key[height]);       // get the index of the current alphabet
        // printf("Index of the character is -> %d\n", ptr);

        // locking the current node with hoh lock
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->node_lock);
        #endif

        // if the alphabet do not have a node already created, create a new one
        if(current_node->children[ptr]==NULL){

            // initialization of new node 
            trie_node_t next_node = NULL;
            next_node = (trie_node_t)malloc(sizeof(_trie_node_t));
            next_node->value = 0;
            next_node->is_end = false;

            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_init(&next_node->node_lock, NULL);
            #endif
            
            int i = 0;
            for(i=0; i<26; i++){
                next_node->children[i] = NULL;
            }
            current_node->children[ptr] = next_node;
        }

        // unlock the current node
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
        current_node = current_node->children[ptr];
        // move to the next node
    }

    // hoh locking on the current node
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_lock(&current_node->node_lock);
    #endif
    current_node->is_end = true;    // make the is_end to TRUE
    current_node->value = value;    // assign value to the end node of a word

    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_unlock(&current_node->node_lock);
    #endif

    #ifdef _S_LOCK_TRIE
    pthread_mutex_unlock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_unlock(&trie->lock_rw );
    #endif
    #endif
    // unlock all the locks before exiting the function
}

// finding a key in trie
int find(trie_t trie,char* key, int* val_ptr){

    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_rdlock(&trie->lock_rw );
    #endif
    #endif

    int height;     // height of the trie (number of alphabets in a word)
    int key_length = strlen(key);
    // printf("Length of the current string -> %d\n", key_length);
    trie_node_t current_node;
    current_node = trie->head;

    int ans;    // to store the value of the word
    int ptr;    // int variable to store the index of the current alphabet
    for(height=0; height<key_length; height++){
        ptr  = CHARACHTER_INDEX(key[height]);
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->node_lock);
        #endif
        if(!current_node->children[ptr]){
            #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&trie->trie_lock);
            #endif
            #ifndef _S_LOCK_TRIE
            #ifdef _NO_HOH_LOCK_TRIE
            pthread_rwlock_unlock(&trie->lock_rw );
            #endif
            #endif
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->node_lock);
            #endif
            return -1;
        }
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
        // unlock the trie just before the current node changes
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->children[ptr]->node_lock);
        #endif
        current_node = current_node->children[ptr];

        // the children becomes the current node so unlock it
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
    }
    if (current_node!=NULL && current_node->is_end){
        ans = current_node->value;
        *val_ptr = ans;     // storing the address of the value in the assigned pointer 
        #ifdef _S_LOCK_TRIE
        pthread_mutex_unlock(&trie->trie_lock);
        #endif
        #ifndef _S_LOCK_TRIE
        #ifdef _NO_HOH_LOCK_TRIE
        pthread_rwlock_unlock(&trie->lock_rw );
        #endif
        #endif
        return 0;
    }
    #ifdef _S_LOCK_TRIE
    pthread_mutex_unlock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_unlock(&trie->lock_rw );
    #endif
    #endif
    return -1;
}

// delete a key value 
void delete_kv(trie_t trie, char* key){
    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_wrlock(&trie->lock_rw );
    #endif
    #endif

    trie_node_t current_node;
    current_node = trie->head;
    int key_len = strlen(key);
    int i = 0;
    int ptr;

    // iterate over the key to reach the last node 
    for(i=0;i<key_len;i++){
        ptr = CHARACHTER_INDEX(key[i]);
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->node_lock);
        #endif
        if(current_node->children[ptr]){
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->node_lock);
            #endif
            current_node = current_node->children[ptr];
        }
        // if there does not exist any such key
        else{      
            #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&trie->trie_lock);
            #endif
            #ifndef _S_LOCK_TRIE
            #ifdef _NO_HOH_LOCK_TRIE
            pthread_rwlock_unlock(&trie->lock_rw );
            #endif
            #endif
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->node_lock);
            #endif 
            return;
        }
    }
    if(current_node){
        current_node->is_end = false;
    }

    #ifdef _S_LOCK_TRIE
    pthread_mutex_unlock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_unlock(&trie->lock_rw );
    #endif
    #endif
}

// recursive function for getting the strings with given prefix
void getting_strings(trie_t trie, trie_node_t current_node, char* prefix, char** list, int *ct, int prefix_len){
    int i = 0;
    // iterate through all the children of the current nofde
    for(i=0;i<26;i++){
        if (current_node->children[i]!=NULL){
            prefix[prefix_len] = ar[i];     // if not NULL add it to the prefix
            prefix_len+=1;                  // increase the length of the prefix
            prefix[prefix_len] = '\0';      // to end the string add '/0'

            // if it is the end of the a word then append it to the list
            if(current_node->children[i]->is_end){
                char* vari = malloc(sizeof(char)*100);
                strcpy(vari, prefix);
                list[(*ct)++] = vari;   
            }
            // call the same function for each children
            getting_strings(trie, current_node->children[i], prefix, list, ct, prefix_len);
            prefix[prefix_len-1] = '\0';    // come back to the same prefix
            prefix_len-=1;      // decrease the length
        }
    }
}

// amin function to get all the strings
char** keys_with_prefix(trie_t trie, char* prefix){
    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_rdlock(&trie->lock_rw );
    #endif
    #endif

    // intialize the list containg the strings
    char** list = malloc(1000*sizeof(char*));
    list[0] = NULL;

    int ptr;
    int j = 0;
    int prefix_len = strlen(prefix);
    trie_node_t current_node;
    current_node = trie->head;

    // iterate over the prefix only to reach the diverging node
    for(j=0;j<prefix_len;j++){
        if(current_node==NULL){
            #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&trie->trie_lock);
            #endif
            #ifndef _S_LOCK_TRIE
            #ifdef _NO_HOH_LOCK_TRIE
            pthread_rwlock_unlock(&trie->lock_rw );
            #endif
            #endif
            return list;
        }
        // get the aplhabet index
        ptr = CHARACHTER_INDEX(prefix[j]);
        if(current_node->children[ptr]!=NULL){
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&current_node->children[ptr]->node_lock);
            #endif
            current_node = current_node->children[ptr];
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->node_lock);
            #endif
        }
        else{
            #ifdef _S_LOCK_TRIE
            pthread_mutex_unlock(&trie->trie_lock);
            #endif
            #ifndef _S_LOCK_TRIE
            #ifdef _NO_HOH_LOCK_TRIE
            pthread_rwlock_unlock(&trie->lock_rw );
            #endif
            #endif
            return list;
        }
    }
    // intitial string creation
    int ct = 0;
    if(current_node->is_end){
        char* first = (char*)malloc(sizeof(char)*100);
        strcpy(first, prefix);
        list[ct++] = first;
    }
    // calling the recursive function
    getting_strings(trie, current_node, prefix, list, &ct, prefix_len);
    list[ct] = NULL;    // last element as NULL

    #ifdef _S_LOCK_TRIE
    pthread_mutex_unlock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_unlock(&trie->lock_rw );
    #endif
    #endif
    return list;
}

// recursive function to delete all the nodes of the trie
void delete_nodes(trie_node_t node_data){
    int j;
    // iterate over all the children
    for (j=0;j<26;j++){
        if (node_data->children[j]){
            // call the recurive function for each child
            delete_nodes(node_data->children[j]);
        }
    }
    free(node_data);    // free the memory allocated
    node_data = NULL;
}

// main delete function
void delete_trie(trie_t trie){
    delete_nodes(trie->head);   // call the recursive function
    free(trie);                 // free the trie
    return;
}
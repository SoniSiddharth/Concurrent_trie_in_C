#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include <string.h>

#define CHARACHTER_INDEX(x) ((int)x - (int)'a')
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 
char ar[26] = "abcdefghijklmnopqrstuvwxyz";
// #define ALPHABETS ar[26]

trie_t init_trie(void){
    // Write your code here
    printf("intializing new trie\n");
    trie_t new_trie = malloc(sizeof(_trie_node_t));
    
    trie_node_t head_node = NULL;
    head_node = (trie_node_t)malloc(sizeof(_trie_node_t));
    
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_init(&head_node->node_lock, NULL);
    pthread_mutex_lock(&head_node->node_lock);
    #endif

    printf("initilaizing parts of trie \n");
    head_node->value = 0;
    printf("is end initialization \n");
    head_node->is_end = false;
    int i = 0;
    printf("intializing children \n");
    for(i=0; i<26; i++){
        head_node->children[i] = NULL;
    }
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_unlock(&head_node->node_lock);
    #endif
    
    #ifdef _S_LOCK_TRIE
    if (pthread_mutex_init(&new_trie->trie_lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        // return 1; 
    } 
    #endif

    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    // pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
    pthread_rwlock_init(&new_trie->lock_rw, NULL);
    #endif
    #endif

    new_trie->head = head_node;
    printf("return the empty trie\n");    
    return new_trie;
} 

void insert(trie_t trie, char* key, int value){
    // Write your code here
    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_wrlock(&trie->lock_rw);
    #endif
    #endif
    int height;
    // printf("%s\n", key);
    int key_length = strlen(key);
    // printf("Length of the current string -> %d\n", key_length);
    trie_node_t current_node;
    current_node = trie->head;

    int ptr = 0;
    for(height=0; height<key_length; height++){
        ptr  = CHARACHTER_INDEX(key[height]);
        // printf("Index of the character is -> %d\n", ptr);
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->node_lock);
        #endif
        if(current_node->children[ptr]==NULL){
            // printf("new creation\n");
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
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->children[ptr]->node_lock);
        #endif
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
        current_node = current_node->children[ptr];
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
    }
    current_node->is_end = true;
    current_node->value = value;
    // #ifndef _NO_HOH_LOCK_TRIE
    // pthread_mutex_unlock(&current_node->node_lock);
    // #endif

    #ifdef _S_LOCK_TRIE
    pthread_mutex_unlock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_unlock(&trie->lock_rw );
    #endif
    #endif
}

int find(trie_t trie,char* key, int* val_ptr){
    // Write your code here
    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_rdlock(&trie->lock_rw );
    #endif
    #endif
    int height;
    int key_length = strlen(key);
    // printf("Length of the current string -> %d\n", key_length);
    trie_node_t current_node;
    current_node = trie->head;

    int ans;
    int ptr;
    for(height=0; height<key_length; height++){
        ptr  = CHARACHTER_INDEX(key[height]);
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->node_lock);
        #endif
        if(!current_node->children[ptr]){
            // printf("Not Found\n");
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
        pthread_mutex_lock(&current_node->children[ptr]->node_lock);
        #endif
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
        current_node = current_node->children[ptr];
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_unlock(&current_node->node_lock);
        #endif
    }
    if (current_node!=NULL && current_node->is_end){
        ans = current_node->value;
        *val_ptr = ans;
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

int checkChildren(trie_node_t head){
    int i;
    for(i=0;i<26;i++){
        if(head->children[i]){
            return 0;
        }
    }
    return 1;
}

trie_node_t deleteRecursively(trie_node_t head, char* key, int depth){
    if (!head){ 
        return NULL; 
    }
    if (depth == strlen(key)) { 
        if (head->is_end) 
            head->is_end = false;  
        if (checkChildren(head)) { 
            free(head);
            head = NULL;
            return head;
        }
        return head;
    } 
    int index = key[depth] - 'a'; 
    head->children[index] =  deleteRecursively(head->children[index], key, depth + 1); 
  
    if (checkChildren(head) && head->is_end == false) { 
        free(head);
        head = NULL;
        return head; 
    }
    return head;
}


void delete_kv(trie_t trie, char* key){
    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_wrlock(&trie->lock_rw );
    #endif
    #endif
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_lock(&trie->head->node_lock);
    #endif
    deleteRecursively(trie->head, key, 0);
    #ifdef _S_LOCK_TRIE
    pthread_mutex_unlock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_unlock(&trie->lock_rw );
    #endif
    #endif
    #ifndef _NO_HOH_LOCK_TRIE
    pthread_mutex_unlock(&trie->head->node_lock);
    #endif
}

void getting_strings(trie_t trie, trie_node_t current_node, char* prefix, char** list, int *ct, int prefix_len){
    int i = 0;
    for(i=0;i<26;i++){
        if (current_node->children[i]!=NULL){
            prefix[prefix_len] = ar[i];
            prefix_len+=1;
            prefix[prefix_len] = '\0';
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&current_node->children[i]->node_lock);
            #endif
            if(current_node->children[i]->is_end){
                char* vari = malloc(sizeof(char)*100);
                strcpy(vari, prefix);
                list[(*ct)++] = vari;   
            }
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->children[i]->node_lock);
            #endif
            getting_strings(trie, current_node->children[i], prefix, list, ct, prefix_len);
            prefix[prefix_len-1] = '\0';
            prefix_len-=1;
        }
    }
}

char** keys_with_prefix(trie_t trie, char* prefix){
    // Write your code here
    #ifdef _S_LOCK_TRIE
    pthread_mutex_lock(&trie->trie_lock);
    #endif
    #ifndef _S_LOCK_TRIE
    #ifdef _NO_HOH_LOCK_TRIE
    pthread_rwlock_rdlock(&trie->lock_rw );
    #endif
    #endif
    char** list = malloc(1000*sizeof(char*));
    list[0] = NULL;
    int prefix_len = strlen(prefix);
    int j = 0;
    trie_node_t current_node;
    current_node = trie->head;

    int ptr;
    for(j=0;j<prefix_len;j++){
        #ifndef _NO_HOH_LOCK_TRIE
        pthread_mutex_lock(&current_node->node_lock);
        #endif
        if(current_node==NULL){
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
            return list;
        }
        // printf("hello\n");
        ptr = CHARACHTER_INDEX(prefix[j]);
        if(current_node->children[ptr]!=NULL){
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_lock(&current_node->children[ptr]->node_lock);
            #endif
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->node_lock);
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
            #ifndef _NO_HOH_LOCK_TRIE
            pthread_mutex_unlock(&current_node->node_lock);
            #endif
            return list;
        }
    }
    // printf("done\n");
    // string creation
    int ct = 0;
    if(current_node->is_end){
        char* first = (char*)malloc(sizeof(char)*100);
        strcpy(first, prefix);
        list[ct++] = first;
    }
        // printf("prefix is %s\n", prefix);
    getting_strings(trie, current_node, prefix, list, &ct, prefix_len);
    list[ct] = NULL;
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

void delete_nodes(trie_node_t node_data){
    int j;
    for (j=0;j<26;j++){
        if (node_data->children[j]){
            delete_nodes(node_data->children[j]);
        }
    }
    free(node_data);
    node_data = NULL;
}

void delete_trie(trie_t trie){
    delete_nodes(trie->head);
    free(trie);
    return;
}

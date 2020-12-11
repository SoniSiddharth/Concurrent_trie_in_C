#define _S_LOCK_TRIE
#define _NO_HOH_LOCK_TRIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "trie.c"
#include <time.h>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define RED     "\033[31m"      /* Red */
#define ff fflush(stdout)

trie_t trie;

void* insert_task(void *tnum){
    char key[15];
    int val;
    char ins_file_name[100];
    sprintf(ins_file_name, "test_init.txt");
    FILE* ins_file = fopen(ins_file_name,"r");
    while (1)
    {
        fscanf(ins_file,"%s",key);
        if (key[0]=='-') break;
        fscanf(ins_file,"%d",&val);
        insert(trie,key,val);
    }
    fclose(ins_file);
    return NULL;
}

void* find_task(void *tnum){
    char key[15];
    char find_test_name[100];
    sprintf(find_test_name,"test_find.txt");
    FILE* find_test = fopen(find_test_name,"r");
    while (1)
    {
        fscanf(find_test,"%s",key);
        if (key[0]=='-') break;
    }
    fclose(find_test);
    return NULL;
}

void *rem_task(void *tnum){
    char key[15];
    char rem_file_name[100];
    sprintf(rem_file_name,"test_rm.txt");
    FILE* rem_file = fopen(rem_file_name,"r");
    while (1)
    {
        fscanf(rem_file,"%s",key);
        if (key[0]=='-') break;
        delete_kv(trie,key);
    }
    fclose(rem_file);
    return NULL;
}

void* pref_task(void *tnum){
    char key[15];
    char pref_test_name[100];
    sprintf(pref_test_name,"test_pref.txt");
    FILE* pref_test = fopen(pref_test_name,"r");
    while (1)
    {
        char** list;
        fscanf(pref_test,"%s",key);
        if (key[0]=='-') break;
        list = keys_with_prefix(trie, key);
        int i=0;
        while(list[i]!=NULL){
            free(list[i]);
            i++;
        }
        free(list);
    }
    fclose(pref_test);
    return NULL;
}

int main(int argc, char *argv[]){
    char* news = malloc(sizeof(char)*10);
    strcpy(news, argv[1]);
    int type = atoi(news);
    char f_name[100] = "s_lock";

    // creating new csv file to store time and number of threads
	char *file_name = f_name;
	FILE *fd;
	file_name = strcat(file_name, ".csv");
	fd = fopen(file_name,"w+");
	fprintf(fd,"num_of_threads, time");

    trie = init_trie();
    // printf("trie created\n");
    int narr[5] = {0,1,2,3,4};
    int num_of_threads;
    int k;

    for(k=3;k<100;k++){
        num_of_threads = k;
        //read write 50-50

        if(type==0){
            num_of_threads = num_of_threads/2;
            pthread_t ins_threads[num_of_threads];
            pthread_t find_threads[num_of_threads];
            int i;

            clock_t time_taken;
            time_taken = clock();
            for (i=0;i<num_of_threads;i++){
                pthread_create(&ins_threads[i], NULL, insert_task,(void *)&narr[1]);
                pthread_create(&find_threads[i], NULL, find_task,(void *)&narr[1]);
            }
            for(i=0;i<num_of_threads;i++){
                pthread_join(ins_threads[i],NULL);
                pthread_join(find_threads[i],NULL);
            }
            time_taken = clock() - time_taken;
            double time_total = ((double)time_taken)/CLOCKS_PER_SEC;
            // printf("%f\n", time_total);
            fprintf(fd, "\n%d, %f", num_of_threads, time_total);
        }
        else if(type==1){
            // read intensive
            int readt = 0.8*num_of_threads;
            int writet = num_of_threads - readt;
            pthread_t ins_threads[writet];
            pthread_t find_threads[readt];
            int i;
            clock_t time_taken;
            time_taken = clock();
            for (i=0;i<readt;i++){
                pthread_create(&find_threads[i], NULL, find_task,(void *)&narr[1]);
                if (i<writet){
                    pthread_create(&ins_threads[i], NULL, insert_task,(void *)&narr[1]);
                }
            }
            for(i=0;i<readt;i++){
                pthread_join(find_threads[i],NULL);
                if(i<writet){
                    pthread_join(ins_threads[i],NULL);

                }   
            }
            time_taken = clock() - time_taken;
            double time_total = ((double)time_taken)/CLOCKS_PER_SEC;
            fprintf(fd, "\n%d, %f", num_of_threads, time_total);
            // printf("done\n");
        }
        else if (type==2){
            // write intensive
            int writet = 0.8*num_of_threads;
            int readt = num_of_threads - writet;
            pthread_t ins_threads[writet];
            pthread_t find_threads[readt];
            int i;
            clock_t time_taken;
            time_taken = clock();
            for (i=0;i<writet;i++){
                pthread_create(&ins_threads[i], NULL, insert_task,(void *)&narr[1]);
                if (i<readt){
                    pthread_create(&find_threads[i], NULL, find_task,(void *)&narr[1]);    
                }
            }
            for(i=0;i<writet;i++){
                pthread_join(ins_threads[i],NULL);
                if(i<readt){
                    pthread_join(find_threads[i],NULL);
                }   
            }
            time_taken = clock() - time_taken;
            double time_total = ((double)time_taken)/CLOCKS_PER_SEC;
            fprintf(fd, "\n%d, %f", num_of_threads, time_total);
            // printf("done\n");
        }
    }
    fclose(fd);
    return 0;
}
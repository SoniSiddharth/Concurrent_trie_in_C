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
    // int t = *((int *) tnum);
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
    // int t = *((int *) tnum);
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
    // fclose(find_test_exp);
    return NULL;
}

void *rem_task(void *tnum){
    // int t = *((int *) tnum);
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
    // int t = *((int *) tnum);
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
        if (list[i]==NULL){
            // fprintf(pref_test_exp,"\n");
        }
        while(list[i]!=NULL){
            // fprintf(pref_test_exp,"%s\n",list[i]);
            free(list[i]);
            i++;
        }
        free(list);
    }
    fclose(pref_test);
    // fclose(pref_test_exp);
    return NULL;
}

int main(int argc, char *argv[]){
    char* news = malloc(sizeof(char)*10);
    strcpy(news, argv[1]);
    int type = atoi(news);
    char f_name[100] = "hoh_lock";
	char *file_name = f_name;
	FILE *fd;
	file_name = strcat(file_name, ".csv");
	fd = fopen(file_name,"w+");
	fprintf(fd,"num_of_threads, time");

    trie = init_trie();
    int narr[5] = {0,1,2,3,4};
    int num_of_threads;
    int k;
    // printf("here1\n");
    for(k=3;k<100;k++){
        num_of_threads = k;
        //read write 50-50
        // printf("here2\n");
        if(type==0){
            num_of_threads = num_of_threads/2;
            pthread_t ins_threads[num_of_threads];
            pthread_t find_threads[num_of_threads];
            int i;
            // printf("here3\n");
            clock_t time_taken;
            // printf("here4\n");
            time_taken = clock();
            // printf("here5\n");
            for (i=0;i<num_of_threads;i++){
                // printf("here6\n");
                pthread_create(&ins_threads[i], NULL, insert_task,(void *)&narr[1]);
                pthread_create(&find_threads[i], NULL, find_task,(void *)&narr[1]);
            }
            // printf("here7\n");
            for(i=0;i<num_of_threads;i++){
                pthread_join(ins_threads[i],NULL);
                pthread_join(find_threads[i],NULL);
            }
            time_taken = clock() - time_taken;
            double time_total = ((double)time_taken)/CLOCKS_PER_SEC;
            printf("%f\n", time_total);
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
                pthread_create(&ins_threads[i], NULL, insert_task,(void *)&narr[1]);
                if (i<writet){
                    pthread_create(&find_threads[i], NULL, find_task,(void *)&narr[1]);
                }
            }
            for(i=0;i<readt;i++){
                pthread_join(ins_threads[i],NULL);
                if(i<writet){
                    pthread_join(find_threads[i],NULL);
                }   
            }
            time_taken = clock() - time_taken;
            double time_total = ((double)time_taken)/CLOCKS_PER_SEC;
            fprintf(fd, "\n%d, %f", num_of_threads, time_total);
            // printf("done\n");
        }
        else if (type==2){
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

    // Removing
//     printf(YELLOW "Testing Delete..." RESET);ff;

//     pthread_t rem_threads[num_of_threads];
    
//     for(i=0;i<num_of_threads;i++){
//         pthread_create(&rem_threads[i], NULL, rem_task,(void *)&narr[1]);
//     }

//     for(i=0;i<num_of_threads;i++){
//         pthread_join(rem_threads[i],NULL);
//     }

//     printf("delete done\n");
//     // // Test trie state after remove
//     // char * rem_exp_name = "./tests/multi_thread/rem/exp.txt";
//     // FILE* rem_exp = fopen(rem_exp_name,"r");
//     // err = 0;
//     // while (1)
//     // {
//     //     int f_ret = -101;
//     //     fscanf(rem_exp,"%s",key);
//     //     if (key[0]=='-') break;
//     //     fscanf(rem_exp,"%d",&val);
//     //     find(trie,key,&f_ret);
//     //     if (f_ret!=val){
//     //         err = 1;
//     //         break;
//     //     }
//     // }
//     // fclose(rem_exp);
//     // if(err==1) {printf(RED "Failure\n" RESET);ff;}
//     // else {printf(GREEN "Passed\n" RESET);ff;}

//     // Prefix searching
//     printf(YELLOW "Testing Prefix...\n" RESET);ff;
//     fflush(stdout);
//     pthread_t pref_threads[num_of_threads];
    
//     for(i=0;i<num_of_threads;i++){
//         pthread_create(&pref_threads[i], NULL, pref_task,(void *)&narr[1]);
//     }

//     for(i=0;i<num_of_threads;i++){
//         pthread_join(pref_threads[i],NULL);
//     }

//     printf("prefic done\n");
//     // printf(YELLOW "Thread 1: " RESET);ff;
//     // test_file_diff("./tests/multi_thread/pref/exp_1.txt","./.testout/pref_text_exp_1.txt");
//     // printf(YELLOW "Thread 2: " RESET);ff;
//     // test_file_diff("./tests/multi_thread/pref/exp_2.txt","./.testout/pref_text_exp_2.txt");
//     // printf(YELLOW "Thread 3: " RESET);ff;
//     // test_file_diff("./tests/multi_thread/pref/exp_3.txt","./.testout/pref_text_exp_3.txt");

//     // Remove + Find Test
//     printf(YELLOW "Testing Find + Delete..." RESET);ff;


//     for(i=0;i<num_of_threads;i++){
//         insert_task(&narr[1]);
//     }

//     pthread_t frem_threads[num_of_threads+1];
//     for(i=0;i<num_of_threads;i++){
//         pthread_create(&frem_threads[i], NULL, rem_task,(void *)&narr[1]);
//     }
//     pthread_create(&frem_threads[num_of_threads], NULL, find_task,(void *)&narr[1]);

//     for(i=0;i<num_of_threads;i++){
//         pthread_join(frem_threads[i],NULL);
//     }
//     pthread_join(frem_threads[num_of_threads],NULL);

//     // Test trie state after remove
//     // rem_exp = fopen(rem_exp_name,"r");
//     // err = 0;
//     // while (1)
//     // {
//     //     int f_ret = -101;
//     //     fscanf(rem_exp,"%s",key);
//     //     if (key[0]=='-') break;
//     //     fscanf(rem_exp,"%d",&val);
//     //     find(trie,key,&f_ret);
//     //     if (f_ret!=val){
//     //         err = 1;
//     //         break;
//     //     }
//     // }
//     // fclose(rem_exp);
//     // if(err==1) {printf(RED "Failure\n" RESET);ff;}
//     // else {printf(GREEN "Passed\n" RESET);ff;}

//     // Delete the trie
//     delete_trie(trie);
//     printf(GREEN "Multi threaded trie implementation test completed.\n" RESET);
//     return 0;
// }
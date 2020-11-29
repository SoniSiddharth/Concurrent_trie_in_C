/*
INSTRUCTIONS:

This file will contain all the functions related to the generation of workload

*/

#include "definitions.h"
#include <stdlib.h>
#include <stdio.h>

// Note: The work must be generated for the returned pointer
struct workload * generate_workload(WorkloadsT type, int pages, int size)
{   
    struct workload new_workload;
    struct workload *ptr_workload = (struct workload *)malloc(sizeof(struct workload));
    ptr_workload->type = type;
    ptr_workload->pages = pages;
    ptr_workload->size = size;
    if(type==LOCAL){
        printf("generating local\n");
        ptr_workload = generate_local(ptr_workload);
        printf("local generated\n");
        return ptr_workload;
    }
    else if(type==RANDOM){
        printf("generating random\n");
        ptr_workload = generate_random(ptr_workload);
        printf("random generated\n");
        return ptr_workload;
    }
    else if(type==LOOP){
        printf("generating loop\n");
        ptr_workload = generate_loop(ptr_workload);
        printf("loop generated\n");
        return ptr_workload;
    }
    else{
        printf("Specify correct type\n");
        return ptr_workload;
    }
    
}

struct workload * generate_random(struct workload * w)
{   
    int load[w->size];
    int j = 0;
    int num;
    for(j=0;j<w->size;j++){
        num = (rand() % (w->pages - 1 + 1)) + 1;
        load[j] = num;
    }
    w->work = load;
    return w;

}

struct workload * generate_loop(struct workload * w)
{
    int load[w->size];
    int j = 0;
    int num;
    for(j=0;j<w->size;j++){
        num = (j+1)%w->pages;
        if (num==0){
            load[j] = w->pages;
        }
        else{
            load[j] = num;
        }
    }
    w->work = load;
    return w;
}

struct workload * generate_local(struct workload * w)
{
    int load[w->size];
    int j = 0;
    float num20;

    num20 = 0.2*w->pages;
    int n20 = 0.2*w->pages;
    if(num20<n20){
        // printf("num20 modified\n");
        num20 = n20;
    }
    else{
        num20 = n20 + 5;
    }
    int num80 = w->pages - num20;
    int num20_final = num20;

    int taken[num20_final];
    int length = 0;
    int number;
    while(length<num20_final){
        int flag = 0;
        number = (rand() % (w->pages - 1 + 1)) + 1;
        for(int k=0; k<length;k++){
            if (taken[k]==number){
                flag = 1;
                break;
            }
        }
        if (flag==0){
            taken[length] = number;
            length+=1;
        }
    }
    int flag;
    int nottaken[num80];
    int index = 0;
    while(j<w->pages){
        flag = 0;
        for(int k=1;k<num20_final;k++){
            if(j==taken[k]){
                flag = 1;
                break;
            }
        }
        if (flag==0){
            nottaken[index] = j;
            index+=1;
        }
        j+=1;
    }

    float work20 = 0.2*w->size;
    int work_int20 = 0.2*w->size;
    if(work20<work_int20){
        // printf("work20 modified\n");
        work20 = work_int20;
    }
    else{
        work20 = work_int20 + 5;
    }

    int work80 = w->size - work20;
    int work20_f = work20;
    for(j=0;j<w->size;j++){
        if(j%work20_f==0){
            index = (rand() % (num80 - 0 + 1)) + 0;
            load[j] = nottaken[index];
        }
        else{
            index = (rand() % (num20_final - 0 + 1)) + 0;
            load[j] = nottaken[index];
        }
    }
    w->work = load;
    return w;
}
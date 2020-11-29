/* INSTRUCTIONS:

This file will contain all functions related with various policies.
Each policy must return the hit rate

*/

#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>

int hit(int page_no, int arr[], int n, int* index){
	int j = 0;
	// printf("%d\n", page_no);
	for(j=0;j<n;j++){
		// printf("%d\n",arr[j]);
		if(arr[j]==page_no){
			*index = j;
			return 1;
		}
	}
	return 0;
}

float policy_FIFO(struct workload * w, int cache_size)
{	
	float hit_rate;
	/* fill this */
	int i;
	int cache[cache_size];
	for(i=0;i<cache_size;i++){
		cache[i] = -1;
	}
	int *p = w->work;
	float wsize = w->size;
	i = 0;
	float faults = 0;
	int m;
	int available;
	int index;
	while(i<wsize){
		// printf("%d\n", *p);
		available = hit(*p, cache, cache_size, &index);
		// printf("%d\n", available);
		if(available==0){
			for(m=0; m<cache_size-1; m++){
				cache[m] = cache[m+1];
			}
			cache[m] = *p;
			// printf("%d\n",m);
			// printf("%d\n",cache[m]);
			faults++;
		}
		// printf("%d\n", cache[cache_size-1]);
		i++;
		p++;
	}
	// printf("%f\n", faults);
	// printf("%f\n", wsize);
	hit_rate = (wsize - faults)/wsize;
	// printf("%f\n", hit_rate);
	// printf("FIFO ended\n");
	return hit_rate;
}

float policy_LRU(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	/* fill this */
	int cache[cache_size];
	int i;
	for(i=0;i<cache_size;i++){
		cache[i] = -1;
	}
	int *p = w->work;
	float wsize = w->size;
	i = 0;
	float faults = 0;
	int n;
	n = cache_size;
	int j = 0;
	int index = 0;
	int available;
	int unique_pages = w->pages;
	while(i<wsize){
		// printf("%d\n", *p);
		available = hit(*p, cache, n, &index);
		// printf("%d\n", available);
		if (available==1){
			for(j=index;j<n-1;j++){
				cache[j] = cache[j+1];
			}
			cache[j] = *p;
		}
		else{
			for(j=0;j<n-1;j++){
				cache[j] = cache[j+1];
			}
			cache[j] = *p;
			// printf("%d\n", cache[j]);
			faults++;
		}
		i++;
		p++;
	}
	// printf("%f\n", faults);
	// printf("%f\n", wsize);
	hit_rate = (wsize - faults)/wsize;
	return hit_rate;
}

float policy_LRUapprox(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	/* fill this */
	int cache[cache_size];
	int i = 0;
	for(i=0;i<cache_size;i++){
		cache[i] = -1;
	}
	int *p = w->work;
	float wsize = w->size;
	float faults = 0;
	int n;
	int unique_pages = w->pages;
	n = cache_size;
	int use_bits[w->pages];
	for(i=0; i<w->pages; i++){
        use_bits[i]=0;
	}
	i = 0;
	while(i<wsize){
		int j = 0;
		int index = 0;
		int available;
		int page_replc = 0;
		available = hit(*p, cache, n, &index);
		if (available==0){
			faults+=1;
			if(use_bits[index]==1){
				do{
					use_bits[index] = 0;
					index+=1;
					if(index==n){
						index = 0;
					}
				}
				while(use_bits[index]!=0);
			}
			if(use_bits[index]==0){
				cache[index] = *p;
				use_bits[index] = 1;
				index+=1;
			}
		}
		else{
			// printf("hello4\n");
			if(use_bits[index]==0){
				use_bits[index] = 1;
			}
		}
		if(index==n){
			index = 0;
		}
		p++;
		i++;
	}
	// printf("faults in approx %d\n",faults);
	// printf("faults in approx %d\n",wsize);
	hit_rate = (wsize - faults)/wsize;
	return hit_rate;
}

float policy_RANDOM(struct workload * w, int cache_size)
{
	float hit_rate = 0;
	/* fill this */
	int cache[cache_size];
	int *p = w->work;
	float wsize = w->size;
	int i = 0;
	float faults = 0;
	int n;
	for(i=0;i<wsize;i++){
		n = cache_size;
		int j = 0;
		int index;
		int available;
		available = hit(*p, cache, n, &index);
		if(available==0){
			int index;
			index = (rand() % (n-1 - 0 + 1)) + 0;
			for(j=index;j<n-1;j++){
				cache[j] = cache[j+1];
			}
			cache[j] = *p;
			faults++;
		}
		p++;
	}
	hit_rate = (wsize - faults)/wsize;
	return hit_rate;
}

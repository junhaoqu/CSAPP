#include "cachelab.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

char *path;
char str[1024];


int Hits = 0, Misses = 0, Evicts = 0;
//init the cache
typedef struct cacheStruct{
	int valid;
	int tag;
	int stamp;

}cacheLine;
cacheLine **cache = NULL;

void initCache(int S, int E){
	cache = (cacheLine**)malloc(sizeof(cacheLine*) * S);
	for(int i = 0; i < S; i++){
		cache[i] = (cacheLine*)malloc(sizeof(cacheLine)*E);
		for(int j = 0; j < E; j++){
			cache[i][j].tag = -1;
			cache[i][j].stamp = -1;
			cache[i][j].valid = 0;
		}
	}

}

void freeCache(int S){
	for(int i = 0; i < S; i++){
		free(cache[i]);
	}
	free(cache);
}

int char2Int(char c){
	return c - '0';
}

void updateStamp(int s, int E){
	for(int i = 0; i < 1<<s; i++){
		for(int j = 0; j < E; j++){
			if(cache[i][j].valid == 1)
				++cache[i][j].stamp;
		}
	}
}

void update(uint64_t address, int b, int s, int E){
	int index = (address >> b) & ((-1U) >> (64 - s));
	int tag = address >> (b + s);
	int MaxStamp = INT_MIN;
	int MaxStamp_index = -1;
	for(int i = 0; i < E; i++){
		if(cache[index][i].tag == tag){
			cache[index][i].stamp = 0;
			++Hits;
			return;
		}

	}
	for(int i = 0; i < E; i++){
		if(cache[index][i].valid == 0){
			cache[index][i].stamp = 0;
			cache[index][i].valid = 1;
			cache[index][i].tag = tag;
			++Misses;
			return;
		}
	}
	++Evicts;
	++Misses;
	for(int i = 0; i < E; ++i){
		if(cache[index][i].stamp > MaxStamp){
			MaxStamp = cache[index][i].stamp;
			MaxStamp_index = i;
		}
	}
	cache[index][MaxStamp_index].tag = tag;
	cache[index][MaxStamp_index].stamp = 0;

	return;
}




int main(int argc, char* argv[])
{
    int s, E, b, size;
    char op;
    uint64_t address;
    FILE *fp;
    for(int i = 1; i < argc; i++){
    	if(argv[i][0] == '-'){
		char tag = argv[i][1];
		switch(tag){
			case 's':
				s = char2Int(argv[++i][0]);
				break;
			case 'E':
				E = char2Int(argv[++i][0]);
				break;
			case 'b':
				b = char2Int(argv[++i][0]);
				break;
			case 't':
				path = argv[++i];
				break;	
		
		}
	}
	if(i > argc) break;
    }
    initCache(1<<s, E);
    fp = fopen(path, "r");
    if(fp == NULL){
    	printf("open error");
	exit(-1);
    }
    while(fgets(str, 100, fp) != NULL){
    	sscanf(str, " %c %lx,%d", &op, &address, &size);
	switch(op){
		case 'I':
			continue;
		case 'L': case 'S':
			update(address, b, s, E);
			break;
		case 'M':
			update(address, b, s, E);
			update(address, b, s, E);
			break;
	}
	updateStamp(s,E);
    	
    }
    fclose(fp);
    freeCache(1>>s);
    printSummary(Hits, Misses, Evicts);
    return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
    char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if(map == NULL) return;
    if(key == NULL) return;
    if(value == NULL) return;
    if(map->size >= map->capacity) enlarge(map);

    long index = hash(key, map->capacity);
    Pair * nuevoPar = createPair(key, value);
    
    if(map->buckets[index] != NULL){
        free(map->buckets[index]);
        map->buckets[index] = nuevoPar;
        return;
    }

    map->buckets[index] = nuevoPar;
    map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap *nuevoMapa = (HashMap*)malloc(sizeof(HashMap));
    if(nuevoMapa == NULL) return NULL;
    nuevoMapa->current = -1;
    nuevoMapa->size = 0;
    nuevoMapa->capacity = capacity;
        
    nuevoMapa->buckets = (Pair**)malloc(sizeof(Pair*)*capacity);
    if(nuevoMapa->buckets == NULL){
        free(nuevoMapa);
        return NULL;
    }

    for(int i=0; i<capacity; i++){
        nuevoMapa->buckets[i] = NULL;
    }

    return nuevoMapa;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   
    if(map == NULL) return NULL;
    if(key == NULL) return NULL;
    long index = hash(key, map->capacity);
    if(map->buckets[index] == NULL) return NULL;
    if(is_equal(map->buckets[index]->key, key) == 1) return map->buckets[index];
    for(int i=0; i<map->capacity; i++){
        if(map->buckets[i] != NULL && is_equal(map->buckets[i]->key, key) == 1){
            return map->buckets[i];
        }
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}

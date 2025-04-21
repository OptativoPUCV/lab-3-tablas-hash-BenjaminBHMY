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
    // Validaciones iniciales
    if(map == NULL) return;
    if(value == NULL) return;
    if(map->size / map->capacity >= 0.7) enlarge(map); // Aumentar capacidad si esta sobre el 70%

    long index = hash(key, map->capacity); // Calcular el índice del key
    Pair * nuevoPar = createPair(key, value);

    while (map->buckets[index] != NULL) 
    {
        // Si la clave ya existe, reemplazar el valor
        if (is_equal(map->buckets[index]->key, key) == 1 ) {
            free(map->buckets[index]);
            map->buckets[index] = nuevoPar;
            return;
        }
        // Si la casilla es invalida, insertar el nuevo par
        if (map->buckets[index]->key == NULL) {
            free(map->buckets[index]);
            map->buckets[index] = nuevoPar;
            map->size++;
            return;
        }

        index = (index + 1) % map->capacity; //probar con el siguiente bucket
    }

    // Si la casilla esta disponible, insertar el nuevo par
    map->buckets[index] = nuevoPar;
    map->size++;
    return;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    if(map == NULL) return; //validar el mapa

    Pair ** old_buckets = map->buckets; //guardar la tabla vieja
    long old_capacity = map->capacity; //guardar la capacidad vieja

    map->capacity *= 2;
    map->size = 0;
    // Crear una nueva tabla con el doble de capacidad
    Pair **new_buckets = (Pair**)malloc(sizeof(Pair*)*map->capacity); 
    if(new_buckets == NULL) return;

    // Inicializar la nueva tabla
    for(int i=0; i < map->capacity; i++)
        new_buckets[i] = NULL;
    
    map->buckets = new_buckets; //asignar la nueva tabla al mapa

    // Reinsertar los pares de la tabla vieja en la nueva tabla
    for(int i=0; i < old_capacity; i++) {
        if (old_buckets[i] != NULL) {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value); //reinsertar el par en la nueva tabla
            free(old_buckets[i]); //liberar el par viejo
        }
    }

    free(old_buckets); //liberar la tabla vieja
}


HashMap * createMap(long capacity) {
     // Asignar espacio para el mapa
    HashMap *nuevoMapa = (HashMap*)malloc(sizeof(HashMap));
    if(nuevoMapa == NULL) return NULL;
    // Inicializar valores
    nuevoMapa->current = -1;
    nuevoMapa->size = 0;
    nuevoMapa->capacity = capacity;
        
    // Asignar espacio para los buckets
    nuevoMapa->buckets = (Pair**)malloc(sizeof(Pair*)*capacity);
    if(nuevoMapa->buckets == NULL){
        free(nuevoMapa);
        return NULL;
    }

    // Inicializar los buckets
    for(int i=0; i<capacity; i++){
        nuevoMapa->buckets[i] = NULL;
    }

    return nuevoMapa;
}

void eraseMap(HashMap * map,  char * key) {   
    // Validaciones iniciales 
    if (map == NULL) return;
    if (key == NULL) return;
    
    long index = hash(key, map->capacity); // Calcular el índice del key
    while (map->buckets[index] != NULL) 
    {
        // Si la clave existe, eliminar el par
        if (is_equal(map->buckets[index]->key, key) == 1) {
            map->buckets[index]->key = NULL;
            map->size--;
            return;
        }
        index = (index + 1) % map->capacity; //probar con el siguiente bucket
    }

    return;
}

Pair * searchMap(HashMap * map,  char * key) {   
    if(map == NULL) return NULL;
    if(key == NULL) return NULL;

    long index = hash(key, map->capacity);    
    while (map->buckets[index] != NULL) 
    {
        if (is_equal(map->buckets[index]->key, key) == 1) {
            map->current = index; //actualiza el indice del ultimo elemento accedido
            return map->buckets[index];
        }
        index = (index + 1) % map->capacity; //probar con el siguiente bucket
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {
    if (map == NULL) return NULL;
    if (map->size == 0) return NULL;
    for (long i = 0; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL) return NULL;
    if (map->size == 0) return NULL;
    for (long i = map->current + 1; i < map->capacity; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;

    /*if (map == NULL || map->size == 0 || map->current == -1) return NULL; // Validaciones iniciales

    long start = map->current; // Guardar el índice inicial
    long i = (map->current + 1) % map->capacity; // Comenzar en el siguiente índice

    while (i != start) { // Recorrer la tabla hasta volver al índice inicial
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i; // Actualizar el índice actual
            return map->buckets[i];
        }
        i = (i + 1) % map->capacity; // Avanzar al siguiente índice
    }

    map->current = -1; // No se encontraron más elementos válidos
    return NULL; // Retornar NULL si no hay más elementos*/
}

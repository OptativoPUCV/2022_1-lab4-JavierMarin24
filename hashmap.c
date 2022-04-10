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


void insertMap(HashMap * map, char * key, void * value) 
{
  int aux_posicion;
  aux_posicion = hash(key, map->capacity);
  while(map->buckets[aux_posicion] != NULL && map->buckets[aux_posicion]->key != NULL)
  {
    
      if(is_equal(map->buckets[aux_posicion]->key, key) == 1){
        break;
       }
      aux_posicion++;
      if(aux_posicion==map->capacity)
      {
        aux_posicion=0;
      }
    }
  
  if(map->buckets[aux_posicion] != NULL)
  {
    map->buckets[aux_posicion]->key=key;
    map->buckets[aux_posicion]->value=value;
    map->current=aux_posicion;
    map->size++;
    
  }
  else
  {
    map->buckets[aux_posicion] = createPair(key, value);
    map->size++;
  }
}

void enlarge(HashMap * map) {
  enlarge_called = 1; 
  map->size = 0;
  Pair** bucket_anterior = (Pair**) calloc (map->capacity, sizeof(Pair*));
  for (int k = 0 ; k < map->capacity ; k++)
  {
    if (map->buckets[k] != NULL && map->buckets[k]->key != NULL)
    {
      bucket_anterior[k] = createPair(map->buckets[k]->key, map->buckets[k]->value); 
    }
  }
  map->capacity =  map->capacity * 2 ;
  Pair** bucket_doble = (Pair**) calloc (map->capacity, sizeof(Pair*));
  
  
  map->buckets = bucket_doble;
  
   
  int i = 0;
  while(i<(map->capacity/2))
    {
      if (bucket_anterior[i] != NULL && bucket_anterior[i]->key != NULL)
      {
        insertMap(map,bucket_anterior[i]->key, bucket_anterior[i]->value); 
        map->size++;
      }
      i++;
    }
 
}


HashMap * createMap(long capacity) {
  HashMap *mapa=(HashMap*) malloc(sizeof(HashMap));
   mapa->buckets=(Pair**)calloc(capacity,sizeof(Pair*));
  mapa->size=0;
  mapa->capacity=capacity;
  mapa->current=-1;
 
    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
int aux;
aux=hash(key, map->capacity);
while(map->buckets[aux] != NULL && map->buckets[aux]->key != NULL)
  {
    if(is_equal(map->buckets[aux]->key, key) == 1)
    {
      map->buckets[aux]->key = NULL;
      map->current=aux;
      map->size--;
      break;
    }
    aux++;
    if(aux==map->capacity)
    {
      aux=0;
    }
    
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
int aux;
  aux=hash(key, map->capacity);
  while(map->buckets[aux] != NULL)
    {
       if(is_equal(map->buckets[aux]->key, key) == 1)
       {
             map->current = aux;
             return map->buckets[aux];
       }
      aux++;
      if(aux == map->capacity)
      {
        aux=0;
      }
      

    }
  return NULL;

    return NULL;
}

Pair * firstMap(HashMap * map) {
int i=0;
  while(i<map->capacity)
    {
      if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current=i;
        return map->buckets[i];
      }
      i++;
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
  int aux = map->current;
   while(aux < map->capacity)
  {
    if (map->buckets[aux] != NULL && map->buckets[aux]->key != NULL && aux != map->current)
      {
        map->current = aux;
        return map->buckets[aux];
      }
    aux++;
    
  }
  return NULL; 
}

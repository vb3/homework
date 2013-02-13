#include <stdlib.h>
#include "hashMap.h"
#include <assert.h>
#include <string.h>

int stringHash1(char * str)
{
    int i;
    int r = 0;
    for (i = 0; str[i] != '\0'; i++)
        r += str[i];
    return r;
}

int stringHash2(char * str)
{
    int i;
    int r = 0;
    for (i = 0; str[i] != '\0'; i++)
        r += (i+1) * str[i]; /*the difference between 1 and 2 is on this line*/
    return r;
}

void initMap (struct hashMap * ht, int tableSize)
{
    int index;
    
    if(ht == NULL)
        return;
    ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
    ht->tableSize = tableSize;
    ht->count = 0;
    for(index = 0; index < tableSize; index++)
        ht->table[index] = NULL;
}

void freeMap (struct hashMap * ht)
{
    hashLink * temp;
    for(int i=0;i<ht->tableSize;i++){
        temp = ht->table[i];
        while(temp != 0){
            free(temp->key);
            free(temp);
            temp = temp->next;
        }
    }
    free(ht->table);
    ht->tableSize = 0;
}

void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
    int hashIndex = (HASHING_FUNCTION == 2 ? stringHash2(k) : stringHash1(k)) % ht->tableSize;
    if (hashIndex < 0) hashIndex += ht->tableSize;
    
    hashLink * newLink = (hashLink*) malloc(sizeof(hashLink));
    assert(newLink);
    newLink->key = k;
    newLink->value = v;
    newLink->next = ht->table[hashIndex];
    ht->table[hashIndex] = newLink;
    
    ht->count++;
    
}

ValueType* atMap (struct hashMap * ht, KeyType k)
{
    int hashIndex = (HASHING_FUNCTION == 2 ? stringHash2(k) : stringHash1(k)) % ht->tableSize;
    hashLink * cur = ht->table[hashIndex];
    while(cur != 0){
        if(strcmp(cur->key, k) == 0) return &cur->value;
        else cur = cur->next;
    }
    return 0;
}

int containsKey (struct hashMap * ht, KeyType k)
{
    int *result = atMap(ht, k);
    if(result != 0)
        return *result;
    return 0;
}

void removeKey (struct hashMap * ht, KeyType k)
{
    int hashIndex = (HASHING_FUNCTION == 2 ? stringHash2(k) : stringHash1(k)) % ht->tableSize;
    hashLink * cur = ht->table[hashIndex];
    hashLink * prev = ht->table[hashIndex];
    while(cur != 0){
        if(cur->key == k){
            prev->next = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    
}

int size (struct hashMap *ht)
{
    return ht->count;
}

int capacity(struct hashMap *ht)
{
    return ht->tableSize;
}

int emptyBuckets(struct hashMap *ht)
{
    int count=0;
    for(int i=0;i<ht->tableSize;i++){
        if(ht->table[i] == 0) count++;
    }
    return count;
}

float tableLoad(struct hashMap *ht)
{
    return (float)size(ht)/capacity(ht);
}

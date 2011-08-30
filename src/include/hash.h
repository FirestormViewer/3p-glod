/* Copyright (c) 2001, Stanford University
 * All rights reserved.
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */
/* Stolen from CR at the moment */

#ifndef HASH_H
#define HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#define HASH_DEFAULT_NUM_BUCKETS 1047


typedef struct HashData
{
	union
	{
		void *pData;
		int nValue;
	} uData;

	enum EValType
	{
		ePtr, eInt
	};

	EValType mType;

} HashData;

typedef struct HashNode {
        unsigned int key;
        HashData data;
        struct HashNode *next;
} HashNode;

typedef struct HashTable {
        unsigned int num_buckets;
        unsigned int num_elements;
        HashNode **buckets;
} HashTable;

HashTable *AllocHashtable( void );
HashTable *AllocHashtableBySize( int num_buckets );
void FreeHashtable( HashTable *hash );
void FreeHashtableCautious ( HashTable *hash ); // does not free data pointer
void HashtableAddData( HashTable *h, unsigned int key, HashData *data );
void HashtableAddPtr( HashTable *h, unsigned int key, void *data );
void HashtableAddInt( HashTable *h, unsigned int key, int data );
void HashtableDelete( HashTable *h, unsigned int key );
void HashtableDeleteCautious( HashTable *h, unsigned int key);
void *HashtableSearchPtr( HashTable *h, unsigned int key );
int HashtableSearchInt(HashTable* h, unsigned int key);

bool HashtableKeyExists( HashTable *h, unsigned int key );

unsigned int HashtableNumElements( HashTable *h) ;

#define HASHTABLE_WALK( h, t ) {         \
  HashNode *t;                            \
  int _;                                        \
  for (_ = 0 ; _ < h->num_buckets ; _++) {  \
    for (t = h->buckets[_] ; t; t = t->next)   {


#define HASHTABLE_WALK_END( h )          \
  }                                             \
 }                                              \
}

#define HASHTABLE_FIRST_NODE( h, t ) \
   HashNode *t = NULL; \
   int _; \
   for (_ = 0 ; _ < h->num_buckets ; _++) {  \
		if(h->buckets[_] != NULL)	    \
			break; \
   } \
   t = h->buckets[_];


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* HASH_H */

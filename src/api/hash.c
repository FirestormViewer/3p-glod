/* Taken from chromium.
 * Original copyright notice follows.
 ***************************************************************************
 * Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

HashTable *AllocHashtable( void )
{
	unsigned int i;
	HashTable *hash = (HashTable *) malloc( sizeof( HashTable )) ;
        hash->num_buckets = HASH_DEFAULT_NUM_BUCKETS;
        hash->buckets = (HashNode**) malloc(sizeof(HashNode*) * hash->num_buckets);
	hash->num_elements = 0;
	for (i = 0 ; i < hash->num_buckets ; i++)
	{
		hash->buckets[i] = NULL;
	}
	return hash;
}

HashTable *AllocHashtableBySize( int numB )
{
	unsigned int i;
	HashTable *hash = (HashTable *) malloc( sizeof( HashTable )) ;
        hash->num_buckets = numB;
        hash->buckets = (HashNode**) malloc(sizeof(HashNode*) * hash->num_buckets);
	hash->num_elements = 0;
	for (i = 0 ; i < hash->num_buckets ; i++)
	{
		hash->buckets[i] = NULL;
	}
	return hash;
}

void FreeHashtable( HashTable *hash )
{
	unsigned int i;

	for ( i = 0; i < hash->num_buckets; i++ )
	{
		if ( hash->buckets[i] ) 
		{
			if( hash->buckets[i]->data.mType == HashData::ePtr )
				free( hash->buckets[i]->data.uData.pData );
			free( hash->buckets[i] );
		}
	}
        free( hash->buckets );
	free( hash );
}

void FreeHashtableCautious( HashTable *hash )
{
	unsigned int i;

	for ( i = 0; i < hash->num_buckets; i++ )
	{
		if ( hash->buckets[i] ) 
		{
		    HashNode *next = hash->buckets[i];
		    while (next !=NULL){
			next = hash->buckets[i]->next;
			free (hash->buckets[i]);
			hash->buckets[i] = next;
		    }
			free( hash->buckets[i] );
		}
	}
        free( hash->buckets );
	free( hash );
}


static unsigned int doHash( HashTable* hash, unsigned int key )
{
	return key % hash->num_buckets;
}

void HashtableAddPtr( HashTable *h, unsigned int key, void *data )
{
	HashNode *node = (HashNode *) malloc( sizeof( HashNode ) );
	node->key = key;

	node->data.mType = HashData::ePtr;
	node->data.uData.pData = data;
	node->next = h->buckets[doHash( h, key )];
	h->buckets[ doHash( h, key ) ] = node;
	h->num_elements++;
}

void HashtableAddInt( HashTable *h, unsigned int key, int data )
{
	HashNode *node = (HashNode *) malloc( sizeof( HashNode ) );
	node->key = key;

	node->data.mType = HashData::eInt;
	node->data.uData.nValue = data;
	node->next = h->buckets[doHash( h, key )];
	h->buckets[ doHash( h, key ) ] = node;
	h->num_elements++;
}

void HashtableAddData( HashTable *h, unsigned int key, HashData *data )
{
	HashNode *node = (HashNode *) malloc( sizeof( HashNode ) );
	node->key = key;

	node->data.mType = data->mType;

	if( data->mType == HashData::eInt )
		node->data.uData.nValue = data->uData.nValue;
	else
		node->data.uData.pData = data->uData.pData;

	node->next = h->buckets[doHash( h, key )];
	h->buckets[ doHash( h, key ) ] = node;
	h->num_elements++;
}


void HashtableDelete( HashTable *h, unsigned int key )
{
	unsigned int index = doHash( h, key );
	HashNode *temp, *beftemp = NULL;
	for ( temp = h->buckets[index]; temp; temp = temp->next )
	{
		if ( temp->key == key )
			break;
		beftemp = temp;
	}
	if ( !temp )
		return; /* not an error */
	if ( beftemp )
		beftemp->next = temp->next;
	else
		h->buckets[index] = temp->next;
	h->num_elements--;

	if( temp->data.mType == HashData::ePtr )
		free( temp->data.uData.pData );
	free( temp );
}

void HashtableDeleteCautious( HashTable *h, unsigned int key )
{
	unsigned int index = doHash( h, key );
	HashNode *temp, *beftemp = NULL;
	for ( temp = h->buckets[index]; temp; temp = temp->next )
	{
		if ( temp->key == key )
			break;
		beftemp = temp;
	}
	if ( !temp )
		return; /* not an error */
	if ( beftemp )
		beftemp->next = temp->next;
	else
		h->buckets[index] = temp->next;
	h->num_elements--;
	free( temp );
}

bool HashtableKeyExists( HashTable *h, unsigned int key )
{
	unsigned int index = doHash( h, key );
	HashNode *temp;
	for ( temp = h->buckets[index]; temp; temp = temp->next )
	{
		if ( temp->key == key )
			break;
	}

	return temp != 0;
}


void *HashtableSearchPtr( HashTable *h, unsigned int key )
{
	unsigned int index = doHash( h, key );
	HashNode *temp;
	for ( temp = h->buckets[index]; temp; temp = temp->next )
	{
		if ( temp->key == key )
			break;
	}
	if ( !temp )
		return NULL;

	if( temp->data.mType == HashData::eInt )
	{
		printf( "Warning, HashTableSearchPtr found int value\n" );
		return 0;
	}

	return temp->data.uData.pData;
}

int HashtableSearchInt(HashTable* h, unsigned int key)
{
	unsigned int index = doHash( h, key );
	HashNode *temp;
	for ( temp = h->buckets[index]; temp; temp = temp->next )
	{
		if ( temp->key == key )
			break;
	}

	if ( !temp )
		return 0;

	if( temp->data.mType != HashData::eInt )
	{
		printf( "Warning, HashTableSearchInt found non-int value\n" );
		return 0;
	}

	return temp->data.uData.nValue;
}

unsigned int HashtableNumElements( HashTable *h) 
{
	return h->num_elements;
}

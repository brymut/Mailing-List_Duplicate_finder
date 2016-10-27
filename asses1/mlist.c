/**
@author Bryan Mutai 
AP3 Exercise 1, Student ID: 2131791m
This is my own work as defined in the Academic Ethics agreement I have signed.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mlist.h"

#define TABLESIZE 100;

/* Definitions for the mlist & mlist node structs */

typedef struct mlistnode {
	MEntry *entry;
	int initiated;
	struct mlistnode *next;
} bucket;

struct mlist {
	int size;
	bucket **hash;
};


/** for the verbose flag **/
int ml_verbose=0;


int size = TABLESIZE;

/** pointer to the mailing list */
MList **mlpoint;



/** Create a new mailing list to work with */
MList *ml_create(void) {

	if(ml_verbose){
		fprintf(stderr,"mlist: creating mailing list\n");
	}

	/** Declare the new mailing list */
	MList *ml;

	

	/** temp hashtable initialisation variable */
	bucket *hashtab;

	

	/** create and allocate some space for the new mailing list */
	ml = (MList *) malloc(sizeof(MList));

	if(ml == NULL)
		return ml;
	


	/** Set initial mailing list size */
	ml->size = size;


	/** looping variable to be used next up */
	int i;

	/** allocate and initialise every bucket pointer */
	if( (ml->hash = (bucket **) calloc(ml->size,sizeof(bucket *)))!=NULL){
		for(i=0;i<size;i++){
			/** initialise every bucket */
		
			ml->hash[i] = (bucket *) malloc(sizeof(bucket));
			ml->hash[i]->initiated = 0;
		
			ml->hash[i]->next = NULL;
		}
	}
	
	

	return ml;

}
/* resize the mailing list */
void *resize(MList *ml){

	if(ml_verbose){
		fprintf(stderr,"mlist: resizing hash table\n");
	}

	/** loop counters */
	int i,j;
	

	/** create a new mailing double the size of the previous one */	
	MList *new_ml;
	size = (ml->size) * 2;
	new_ml = ml_create();

	/** cursor to loop through old data */
	bucket *cursor;
	bucket *new_cursor;
	bucket *add_cursor;


	
	unsigned long hashvalue;

	for(i=0;i<ml->size;i++){
		cursor = ml->hash[i];
		while(cursor->next!=NULL){
			


			new_cursor = cursor->next;
			
			
			cursor->next = NULL;

			hashvalue = me_hash(cursor->entry,size);

			add_cursor = new_ml->hash[hashvalue];

			
			
			while(add_cursor->next!=NULL){
				add_cursor = add_cursor->next;
			}
			
			if( new_ml->hash[hashvalue]->initiated == 0 ){

				new_ml->hash[hashvalue] = cursor;

				new_ml->hash[hashvalue]->initiated = 1;
			} 
		
			else if( new_ml->hash[hashvalue]->initiated == 1 ){
				new_ml->hash[hashvalue]->next = cursor;
				new_ml->hash[hashvalue]->initiated = 2;
			} else{
				add_cursor->next = cursor;
			}

			
			cursor = new_cursor;
		}
		
	}
	free(ml->hash);
	free(ml);

	return new_ml;
}


/* ml_add - adds a new MEntry to the list;
 * returns 1 if successful, 0 if error (malloc)*/
int ml_add(MList **ml, MEntry *me) {

	MList *m = *ml;
	unsigned long hash_value = 0;
	int i;
	int bucketsize = 0;
	bucket *buck,*bucket_new;

	
	/* check for any duplicates */
	if (ml_lookup(m, me) != NULL){
		return 1;
	}

	/* allocate space for the new entry */
	if((bucket_new = (bucket *) malloc(sizeof(bucket)))==NULL){
		return 0;
	}
	bucket_new->next = NULL;


	/* Compute hash value of the new entry */
	hash_value = me_hash(me,m->size);


	/* choose correct bucket from the mailing list */
	buck = m->hash[hash_value];


	/* cycle through until the next free bucket is found */
	while(buck->next!=NULL){
		bucketsize++;

		buck = buck->next;
		
	}

	/* set next to an empty bucket, and the entry to mentry */
	buck->next = bucket_new;
	buck->entry = me;
	
	/* if the bucketsize is too small resize it*/
	if(bucketsize > m->size)
		*ml = resize(m);

	return 1;	

}

/* ml_lookup - looks for MEntry in the list, returns matching entry or NULL */
MEntry *ml_lookup(MList *ml, MEntry *me) {

	if(ml_verbose){
		fprintf(stderr," Message from mlist : Looking up for similar entry\n");
	}

	bucket *buck_cursor;

	unsigned long hash_value = 0;

	
	

	/** find the hash_value of the new mail entry */
	hash_value = me_hash(me,ml->size);


	/** go through the mailing list, to see if there is a similar entry to the new one */
	buck_cursor = ml->hash[hash_value];

	while( buck_cursor->next!=NULL ){


		int found = me_compare(buck_cursor->entry,me);

		if( found == 0 ){
			/**found one, return its pointer */
			return buck_cursor->entry;

		} else {
			/** not a match, keep on searching*/
			buck_cursor = buck_cursor->next;
		}
	}
	

	/** a similar entry was not found, return NULL */
	return NULL;
}

/** destroy the mailing list */
void ml_destroy(MList *ml) {

	if(ml_verbose){
		fprintf(stderr,"mlist: ml_destroy() entered\n");
	}

	/** define pointers for nodes to delete
	1st for the node to delete currently, then the next one after that */

	bucket *delete; 
	bucket *next_delete;	


	/** looping variable */
	int i;


	/** loop through each hash entry, then loop through buckets, free'ing */
	for(i=0;i<size;i++){
		/** current one to delete */
		delete = ml->hash[i];


		while(delete->next!=NULL){

			/** define the next one to delete */
			next_delete = delete->next;

			/** destroy the current one */
			me_destroy(delete->entry);
			free(delete);

			/** move up the next node to delete up to the next one */
			delete = next_delete;
		}

		free(delete);
	}
	

	free(ml->hash);
	free(ml);
}
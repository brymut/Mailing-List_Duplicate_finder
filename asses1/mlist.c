/**
@author Bryan Mutai
*/

#include <stdio.h>
#include <stdlib.h>
#include "mlist.h"

#define HASHSIZE 20;

typedef struct mlistnode {
	struct mlistnode *next;
	MEntry *entry;
	int initiated;
} bucket;

struct mlist {
	int size;
	bucket **hash;
};

int ml_verbose=0;
int size = HASHSIZE;


MList **mlpoint;


MList *ml_create(void) {
	/** Declare mailing list */
	MList *ml;

	/** loop variable */
	int i;

	/** temp hashtable initialisation variable */
	bucket *hashtab;

	if(ml_verbose)
		fprintf(stderr,"mlist: creating mailing list\n");


	/** allocate space for mlist */
	if( (ml = (MList *) malloc(sizeof(MList))) == NULL){
		return ml;
	}



	/** Set mlist size */
	ml->size = size;

	/** allocate and initialise every bucket pointer */
	if( (ml->hash = (bucket **) calloc(ml->size,sizeof(bucket *)))!=NULL){
		for(i=0;i<size;i++){
			/** initialise every bucket */
			ml->hash[i] = (bucket *) malloc(sizeof(bucket));
			ml->hash[i]->next = NULL;
			ml->hash[i]->initiated = 0;
		}
	}
	
	

	return ml;

}

void *reallocate(MList *ml){

	if(ml_verbose)
		fprintf(stderr,"mlist: resizing hash table\n");

	/** loop counters */
	int i,j;
	int bucketcount;

	/** create a new mailing list with x2 size */	
	MList *new_ml;
	size = (ml->size) * 2;
	new_ml = ml_create();

	/** cursor to loop through old data */
	bucket *cursor;
	bucket *new_cursor;
	bucket *add_cursor;


	/** rehash and link old data into new list */
	unsigned long hashval;

	for(i=0;i<ml->size;i++){
		cursor = ml->hash[i];
		while(cursor->next!=NULL){
			/** loop through every single bucket of every single hashtab entry */

			/** stores location of next node to assign space to */
			new_cursor = cursor->next;
			
			/** set next value of cursor to NULL to indicate last node */
			cursor->next = NULL;

			hashval = me_hash(cursor->entry,size);
			add_cursor = new_ml->hash[hashval];

			/** loop through new mlist buckets to find where to put new entry */
			bucketcount=0;
			while(add_cursor->next!=NULL){
				add_cursor = add_cursor->next;
			}
			
			if( new_ml->hash[hashval]->initiated == 0 ){
				new_ml->hash[hashval] = cursor;
				new_ml->hash[hashval]->initiated = 1;
			} 
		
			else if( new_ml->hash[hashval]->initiated == 1 ){
				new_ml->hash[hashval]->next = cursor;
				new_ml->hash[hashval]->initiated = 2;
			} else
				add_cursor->next = cursor;

			/** update cursor to the next node */
			cursor = new_cursor;
		}
		
	}
	free(ml->hash);
	free(ml);

	return new_ml;
}


/** adds MEntry to list,
Returns 1 if successful
Returns 0 if not successful
*/
int ml_add(MList **ml, MEntry *me) {
	/** counter to determine current bucket size */
	int bucketsize = 0;


	MList *m = *ml;
	unsigned long hashval = 0;
	int i;
	bucket *buck,*bucket_new;

	//printf("%d\n",m->size);

	/** check duplicates */
	if (ml_lookup(m, me) != NULL){
		return 1;
	}

	/** allocate space for next entry */
	if((bucket_new = (bucket *) malloc(sizeof(bucket)))==NULL)
		return 0;

	bucket_new->next = NULL;

	/** Compute hash value of item */
	hashval = me_hash(me,m->size);


	/** choose appropriate bucket array from hash table */
	buck = m->hash[hashval];


	/** loop until free bucket */
	while(buck->next!=NULL){
		buck = buck->next;
		bucketsize++;
	}

	/** set next to an empty bucket, and the entry to mentry */
	buck->next = bucket_new;
	buck->entry = me;
	
	if(bucketsize > m->size)
		*ml = reallocate(m);

	return 1;	

}

/** looks for entry in ml matching me
if found, return pointer, if not return NULL */
MEntry *ml_lookup(MList *ml, MEntry *me) {
	unsigned long hashval = 0;
	bucket *buck_cursor;

	/** print statement if verbose */
	if(ml_verbose)
		fprintf(stderr," Message from mlist : Looking up from *ml_lookup(MList *ml, MEntry *me)\n");

	/** calculate hashval of me */
	hashval = me_hash(me,ml->size);


	/** loop through cursor values checking entries */
	buck_cursor = ml->hash[hashval];

	while(buck_cursor->next!=NULL){
		if(me_compare(buck_cursor->entry,me)==0){
			/**found match, return pointer */
			return buck_cursor->entry;
		} else {
			/** not found, continue searching*/
			buck_cursor = buck_cursor->next;
		}
	}
	

	/** entry was not found, return NULL */
	return NULL;
}

void ml_destroy(MList *ml) {
	int i;
	bucket *to_delete; /** pointer to node to delete */
	bucket *next_node;	/** pointer to the next node to delete */

	if(ml_verbose)
		fprintf(stderr,"mlist: ml_destroy() entered\n");
	
	/** loop through each hash entry, then loop through buckets, free'ing */
	for(i=0;i<size;i++){
		to_delete = ml->hash[i];
		while(to_delete->next!=NULL){
			next_node = to_delete->next;
			me_destroy(to_delete->entry);
			free(to_delete);
			to_delete = next_node;
		}
		free(to_delete);
	}
	/** free structures */
	free(ml->hash);
	free(ml);
}
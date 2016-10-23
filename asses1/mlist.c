/**
@author Bryan Mutai Student ID : 2131791
*/

#include <stdio.h>
#include "mlist.h"

#define HASHSIZE 20;

typedef struct mlistnode {
	struct mlistnode *next;
	MEntry *entry;
} bucket;

struct mlist {
	int size;
	bucket **hash;
};

int ml_verbose=0;
int size = HASHSIZE;


MList *ml_create(void) {
	/** Declare mailing list */
	MList *ml;

	/** loop variable */
	int i;

	/** temp hashtable initialisation variable */
	bucket *hashtab;

	if(ml_verbose)
		fprintf(stderr,"mlist: creating mailing list\n");



	if( (ml = (MList *) malloc(sizeof(MList))) == NULL){
		return ml;
	}

	if( (ml->hash = (bucket **) calloc(size,sizeof(bucket *)))!=NULL){
		for(i=0;i<size;i++){
			ml->hash[i] = (bucket *) malloc(sizeof(bucket));
			ml->hash[i]->next = NULL;
		}
	}

	/** Set hash pointer to bucket */
	ml->size=size;	

	/** Set mlist size */
	ml->size = size;

	return ml;

}

/** adds MEntry to list,
Returns 1 if successful
Returns 0 if not successful
*/
int ml_add(MList **ml, MEntry *me) {
	MList *m = *ml;
	unsigned long hashval;
	int i;
	bucket *buck,*bucket_new;

	/** check duplicates */
	if (ml_lookup(m, me) != NULL)
		return 1;

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
	}

	/** set next to an empty bucket, and the entry to mentry */
	buck->next = bucket_new;
	buck->entry = me;

	return 1;	

}

/** looks for entry in ml matching me
if found, return pointer, if not return NULL */
MEntry *ml_lookup(MList *ml, MEntry *me) {
	unsigned long hashval;
	bucket *buck_cursor;

	int tempsize = ml->size;

	/** print statement if verbose */
	if(ml_verbose)
		fprintf(stderr,"mlist: ml_lookup() entered\n");

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

	ml->size = tempsize;
	return NULL;
}


//  ml_destroy - destroy the mailing list 
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
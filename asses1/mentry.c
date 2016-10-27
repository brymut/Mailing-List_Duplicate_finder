/**
@author Bryan Mutai 
AP3 Exercise 1, Student ID: 2131791m
This is my own work as defined in the Academic Ethics agreement I have signed.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "mentry.h"

#define MAXLINE 500
#define LONGESTPOSTCODE 10
#define SURNAMELIMIT 100


/* Global variable pointing to the mail entry */
MEntry *mail_entry;


/* me_get returns the next file entry, or NULL if end of file*/
MEntry *me_get(FILE *fd){

	/* loop variables */
	int i,j,k;

	/* These will be used to store the mail address lines while we work. */
	char surname_line[MAXLINE+1] = "";
	char house_number_line[MAXLINE+1] = "";    /* we're using the +1 for the final '\0' */
	char postcode_line[MAXLINE+1] = "";

	

	if( ( mail_entry = ((MEntry *)malloc(sizeof(MEntry)))  ) != NULL ){

		/* allocate some storage */
		mail_entry->surname = malloc(sizeof(char)*SURNAMELIMIT);

		mail_entry->postcode = malloc(sizeof(char)*LONGESTPOSTCODE);
		
		/* three times as much as the maxline, because we'll be combining the three lines into a single string */
		mail_entry->full_address = malloc(sizeof(char)*MAXLINE*3);

		/* entry point for the data from the text file */
		fgets(surname_line,MAXLINE,fd);
		fgets(house_number_line,MAXLINE,fd);
		fgets(postcode_line,MAXLINE,fd);

		/* done with input */
		if(surname_line[0]=='\0'){
			return NULL;
		}

		/*  The entire 3-line address is stored as a single string at full_address */
		snprintf(mail_entry->full_address,sizeof(char)*MAXLINE*3,"%s%s%s",surname_line,house_number_line,postcode_line);

		/* insert surname into the mail entry in lower case, stopping at the comma. */
		for( i=0; surname_line[i]!=',';i++ ){
			mail_entry->surname[i] = (char)tolower(surname_line[i]);	
		}
		/* close up the surname entry */
		mail_entry->surname[i] = '\0';

		/* get the postcode into the mail_entry, getting rid of spaces and non-alphanumerics */
		for(j=0,k=0;postcode_line[j]!='\n';j++){

			if(isalnum(postcode_line[j]) &&   (strcmp(&postcode_line[j], " ") != 0)  ){

				mail_entry->postcode[k]=postcode_line[k];
				k++;
			}

		}
		/* close up the postcode entry */
		mail_entry->postcode[j] = '\0';

		/* insert the house number string as an int into the mail_entry */
		mail_entry->house_number = atoi(house_number_line);

	}



	return mail_entry;
}




/* me_hash computes a hash of the MEntry, mod size */
unsigned long me_hash(MEntry *mail_entry, unsigned long size){

	// looping variables
	int i,j;
	unsigned hash_value = 0;
	unsigned temp_hash;
	


	/* get the int value of the surname to use for hashing */
	for(i=0 ,temp_hash=0; mail_entry->surname[temp_hash]!='\0'; i++){
		temp_hash += (int) mail_entry->surname[i];
	}

	hash_value += temp_hash;

	/* get the int value of the Postcode to use for hashing */
	for(j=0,temp_hash=0;mail_entry->postcode[temp_hash]!='\0';j++)
		temp_hash += (int) mail_entry->postcode[j];

	hash_value += temp_hash;
	hash_value += mail_entry->house_number;


	return ( hash_value%size );

}







/* me_print prints the full address on fd */
void me_print(MEntry *mail_entry, FILE *fd){

	fprintf(fd,"%s",mail_entry->full_address);
}








/* me_compare compares two mail entries, returning 0 if they are equal, -1 if they are not equal
*/
int me_compare(MEntry *me1, MEntry *me2){

	/** comparison of the surname and postcode entries */
	int surname_comp = strcmp(me1->surname,me2->surname);
	int postcode_comp = strcmp(me1->postcode,me2->postcode);


	/** if the surname, postcode and house number are the same return 0 */
	if(( postcode_comp ==0 ) && ( surname_comp ==0 ) ) 
		return 0;
	else
		return -1;


}




/* me_destroy destroys the mail entry
*/
void me_destroy(MEntry *mail_entry){
	free(mail_entry->surname);
	free(mail_entry->full_address);
	free(mail_entry->postcode);
	free(mail_entry);
}


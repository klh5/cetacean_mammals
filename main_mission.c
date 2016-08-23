/*This file contains functions used to perform calculations or do more complex processing 
 of the cetacean mammal data.*/

#include <stdio.h>
#include <stdlib.h>
#include "navigation.h"
#include "records.h"

/*Returns the number of sightings in a given linked list of sightings*/
int num_in_list(struct sighting *list) {
    
    struct sighting *ptr = list;
    int i=0;
    
    while(ptr!= NULL) {
        i++;
        ptr = ptr->next;
    }
    return i;
}

/*Main mission*/
/*This function sets the true locations of any cetacean mammals that are not
 close enough to any others to be the same mammal to that of their original location*/
struct sighting* set_locations(struct sighting *sight_head) {
    
    struct sighting *s_ptr = sight_head;

    while(s_ptr != NULL)
    {
        if(s_ptr->true_loc.lat==0) {
            s_ptr->true_loc=s_ptr->cm_loc;   
        }
        s_ptr = s_ptr->next;
}
    return sight_head;
}

/*Main mission*/
/*This function works out whether any two cetacean mammals are close enough together
 to be the same animal.*/
struct sighting* get_same_location(struct sighting* sight_head) {
    
    struct sighting *s_ptr = sight_head;
    struct sighting *comp_ptr=s_ptr->next;
    struct sighting *a_ptr;
    double difference=0;
    double avg_lat=0;
    double avg_lng=0;
    double d=0;
    int no_in_list=num_in_list(sight_head); /*get the number of records*/
    int i;
    
    while(s_ptr->next!= NULL) { /*while there is a next pointer*/
        comp_ptr=s_ptr->next;   /*reset the comparison pointer, which is one ahead*/
        avg_lat=0;              /*reset the averages*/
        avg_lng=0;
        i=0;                    /*reset i to 0, so that the loop can iterate again*/
        
        if(s_ptr->true_loc.lat==0 && s_ptr->true_loc.lng==0) { /*If there is not already an average*/
            while(i<no_in_list) {   /*while there are still records to go through*/
                if(comp_ptr==NULL) { /*check that the comparison pointer isn't null*/
                break;
                }
                
                if(s_ptr->type==comp_ptr->type) {     /*compare the two types - P and D*/
                    difference=great_circle(s_ptr->cm_loc,comp_ptr->cm_loc); /*if they are the same, work out how far apart they are*/
                    if(difference<0.02) {     /*if they are close, average them*/                                           
                        if (avg_lat==0 && avg_lng==0) { /*if nothing else has been averaged on this iteration*/
                            avg_lat=(s_ptr->cm_loc.lat+comp_ptr->cm_loc.lat)/2;
                            avg_lng=(s_ptr->cm_loc.lng+comp_ptr->cm_loc.lng)/2;
                        }
                        else {
                            avg_lat=(avg_lat+comp_ptr->cm_loc.lat)/2; /*keep a rolling average*/
                            avg_lng=(avg_lng+comp_ptr->cm_loc.lng)/2;
                        } 
                    a_ptr=sight_head; 
                    while(a_ptr != NULL) { /*the averages for all of the mammals in this "group" need to be updated*/
                        if(s_ptr->type==a_ptr->type) {
                            d=great_circle(s_ptr->cm_loc,a_ptr->cm_loc);
                                if(d<=0.02) {
                                    a_ptr->true_loc.lat=avg_lat;
                                    a_ptr->true_loc.lng=avg_lng;
                                }
                        }
                        a_ptr=a_ptr->next;
                    }
                    }    
                }
            comp_ptr=comp_ptr->next;
            i++;
            }
        }                
    s_ptr = s_ptr->next;      
    }
    sight_head=set_locations(sight_head);
    return sight_head; /*when finished, return the pointer to the head node*/
}
 
   
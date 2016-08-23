/*This file contains functions which read in the data and do the initial processing.*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "navigation.h"
#include "records.h"

/*Recursive function to remove a sighting from the linked list. This is essentially
 * the same as the function to remove a pod from a linked list.
This code was adapted from https://www.cs.bu.edu/teaching/c/linked-list/delete/*/
struct sighting *rmv_sighting(struct sighting* sight_head,struct sighting* to_remove) {
    
    struct sighting *temp;
    
    if (sight_head == NULL) {
      return sight_head;
    }
  
    if (sight_head == to_remove) {
        temp = sight_head->next;
        free(sight_head);
        return temp;
    }

    sight_head->next = rmv_sighting(sight_head->next, to_remove);
    
    return sight_head;
}

/*Initial mission*/
/*Reads in the file of observers using the file name provided by the user. If the file can't
 be opened, returns null*/
struct observer* read_obs(char*obs_file) {
    
    struct observer *current, *previous, *head=NULL;
    FILE * file;
    int status;
    int i;
    char buffer[21];
    int ch_in,count_lines=0;
    int no_recs;
    
    file=fopen(obs_file,"r");
    
    if (file!=NULL) { 
        do {
            ch_in = fgetc(file);
            if(ch_in == '\n') {
                count_lines++;
                }
        } while (ch_in != EOF);
    
        fclose(file);
    
        file=fopen(obs_file,"r");
        fgets(buffer, 21, file);
    
        for(i=0;i<count_lines-1;i++) {
            current = malloc(sizeof(struct observer));
            status = fscanf(file, "%[A-Z0-9] %lf %lf\n",current->obs_id,&current->obs_loc.lat,&current->obs_loc.lng);
            no_recs++; 

            if(head==NULL) {
                head=current;
            }
            else {
                previous->next=current;
            }
            current->next=NULL;
            previous=current;
        }
        fclose(file);
        return head;       
        }
    else {
    printf("\nNo file found for observers");
    return NULL;
    }
}

/*Initial mission*/
/*Reads in the file of sightings using the file name provided by the user. If the file can't be
 opened, returns null.*/
struct sighting* read_sight(char *sight_file) {
    
    struct sighting *current, *previous, *head=NULL;
    FILE *file;
    int status;
    int i,j;
    int ch_in,count_lines=0;
    int no_recs;
    
    file=fopen(sight_file,"r");
    
    if (file!=NULL) {
        do {
            ch_in = fgetc(file);
            if(ch_in == '\n') {
                count_lines++;
                }
        } while (ch_in != EOF);
    
        fclose(file);
        file=fopen(sight_file,"r");
        
        for(i=0;i<count_lines;i++) {
            current = malloc(sizeof(struct sighting));
            status=fscanf(file, "%[A-Z0-9] %c %lf %lf\n",current->observer,&current->type,&current->bearing,
                            &current->range);
            current->true_loc.lat=0;
            current->true_loc.lng=0;
            no_recs++;
                       
            if(head==NULL) {
                head=current;
            }
            else {
                previous->next=current;
            }
            current->next=NULL;
            previous=current;
            }
        fclose(file);       
        return head;
        }
    else {
    printf("\nNo file found for sightings");
    return NULL;
    }
}

/*Initial mission*/
/*Checks if two strings are equal - used to determine which observer saw which
 cetacean mammal.*/
int check_if_equal(char *obs_1, char *obs_2) {
    return strcmp(obs_1,obs_2);
}

/*Initial mission*/
/*Checks if any sightings are two far north, south, east or west to be considered
 relevant and removes sightings if necessary. */
struct sighting* check_sightings(struct sighting* sight_head) {
    
    struct sighting *temp;
    struct sighting *s_ptr = sight_head;
    
    while(s_ptr!=NULL) {
            if(s_ptr->cm_loc.lat<52.00 || s_ptr->cm_loc.lat>52.833) {
                temp=s_ptr;
                s_ptr=s_ptr->next;
                sight_head=rmv_sighting(sight_head,temp);
                }
            else if(s_ptr->cm_loc.lng>-4.000 || s_ptr->cm_loc.lng<-5.500) {
                temp=s_ptr;
                s_ptr=s_ptr->next;
                sight_head=rmv_sighting(sight_head,temp);
            }
            else {
            s_ptr=s_ptr->next;
            }
    }
    return sight_head;
    }

/*Initial mission*/
/*This function calculates the location of a cetacean based on the location of
 its observer. It takes as parameters an observer location, a cetacean bearing and 
 a cetacean range.*/
location mammal_pos(location obs_loc,double bg,double range) {
    double olat=obs_loc.lat;
    double olatr;
    double olong=obs_loc.lng;
    double bgr;
    double cmlat;
    double cmlong;
    location cm_loc;
    
    olatr=olat * M_PI/180.0;
    bgr=bg * M_PI/180.0;
    
    cmlat=olat+(range * cos(bgr))/60;
    cmlong=olong+(range*sin(bgr)/cos(olatr))/60.0;
    
    cm_loc.lat=cmlat;
    cm_loc.lng=cmlong;
    
    return cm_loc;
}

/*Initial mission*/
/*Compares the list of observers with the list of sightings. If there is a match
 then the location of the observer is used to calculate the location of the mammal, which 
 can then be recorded as the mammal's location.*/
struct sighting* get_cm_locations(struct sighting* sight_head,struct observer* obs_head ) {
    
    struct observer *obs_ptr = obs_head;
    struct sighting *s_ptr = sight_head;
    location new_cm_loc;
    int if_equal;
    
    while(s_ptr!=NULL) {
        if(obs_ptr==NULL) {
            printf("\nAn observer could not be found for one of your sightings");
            return NULL;
        }
                if_equal=check_if_equal(s_ptr->observer,obs_ptr->obs_id);
                if(if_equal==0) {
                    new_cm_loc=mammal_pos(obs_ptr->obs_loc,s_ptr->bearing,s_ptr->range);
                    s_ptr->cm_loc.lat=new_cm_loc.lat;
                    s_ptr->cm_loc.lng=new_cm_loc.lng;
                    s_ptr = s_ptr->next;
                    obs_ptr=obs_head;
                }
                else {
                obs_ptr = obs_ptr->next;
                }       
    }
    return sight_head;
}
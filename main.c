/* 
 * File:   main.c
 * Author: klh5
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "navigation.h"
#include "records.h"

/*All missions*/
/*The main method gets input from the user and passes it to other functions for
  processing.*/
int main(int argc, char** argv) {
    
    char obs_file[201]; /*Variable to hold the path to the file of observers*/
    char sight_file[201];   /*Variable to hold the path to the file of sightings*/
    int ret_val;            /*Used to check the return value of functions*/
    int user_choice;        /*Holds the option from the menu that the user picks*/
    
    /*Initialise the linked lists.*/
    struct observer *obs_head = NULL;
    struct sighting *sight_head = NULL;
    struct pod *pod_head = NULL;
    
    printf("Welcome");
    
    /*Print the menu after executing whichever option the user chooses*/
    do {
        printf("\nWhat would you like to do?");
        printf("\n%d=Initial Mission, %d=Main Mission, %d=Pod Mission, %d=Clear data, %d=Quit\n",
        INITIAL_MISSION, MAIN_MISSION, POD_MISSION, CLEAR_DATA, QUIT);
    
        if(scanf("%d",&user_choice)!=1) {
            printf("\nInput was not an integer");
            break;
        }
        
        switch (user_choice) {  
            
            /*User chooses to clear any data that has been read in or calculated.
             Checks to see if the data is there, deletes it if it is*/
            case CLEAR_DATA:
                if(obs_head!=NULL) {    /*Check if there are any observers*/
                obs_head=clear_observers(obs_head);
                printf("Cleared observer data\n");
                }
                if(sight_head!=NULL) {  /*Check if there are any sightings*/
                sight_head=clear_sightings(sight_head);
                printf("Cleared sighting data\n");
                }
                if(pod_head!=NULL) {    /*Check if there are any pods*/
                    pod_head=clear_pods(pod_head);
                    printf("Cleared pod data\n");
                }
                break;
                
            /*User chooses to complete the initial mission*/
            case INITIAL_MISSION:
                printf("Please enter name of file to use for observers: ");
                scanf("%s",obs_file);
                printf("Reading in data for observers...");
                obs_head=read_obs(obs_file); /*Read in the file provided*/
    
                if(obs_head==NULL) {   
                    printf("\nNo observers could be recorded");
                    break;
                }
    
                printf("\nPlease enter name of file to use for sightings: ");
                scanf("%s",sight_file);
                printf("Reading in data for sightings...");
                sight_head=read_sight(sight_file); /*Read in the file of sightings*/
                
                sight_head=get_cm_locations(sight_head,obs_head); /*Give the sightings locations based
                                                                   on the locations of the observers*/
    
                if(sight_head==NULL) {
                    printf("\nNo sightings could be recorded");
                    break;
                }
    
                sight_head=check_sightings(sight_head); /*Check for any sightings that are too
                                                         far north, south, east or west*/
                print_sightings(sight_head); 
                break;
            
            /*User chooses to complete the main mission. The main mission cannot be
             completed unless the initial mission has been completed*/
            case MAIN_MISSION:
                if(sight_head!=NULL) {  /*Checks that sightings have been read in*/
                sight_head=get_same_location(sight_head); /*Finds any mammals which could be the same*/
                print_alt_sightings(sight_head);    /*The revised table, with true locations, is printed*/
                }
                else {
                    printf("Cannot complete main mission before initial mission\n");
                    break;
                }
            break;
            
            /*User chooses to complete the pod mission. The pod mission cannot be
             completed unless the initial mission has been completed. If the initial
             mission has been completed but not the main mission, no pods will be returned
             because none of the cetacean mammals will have true locations*/
            case POD_MISSION:
                pod_head=get_pods(sight_head); /*Attempts to create pods from the sightings*/
        
                if(pod_head==NULL) {    /*Checks that some pods were created - if pod_head is null, there were 
                                         no sightings to create pods from*/
                    printf("Cannot complete pod mission before initial mission\n");
                    break;
                }
        
                pod_head=rmv_duplicates(pod_head); /*Remove "duplicate" pods - pods that could both refer
                                                    to the same mammal*/
                pod_head=find_pods(pod_head); /*Now find the actual pods*/
 
                pod_head=rmv_singles(pod_head); /*Remove any "pods" which still only have one mammal*/
                pod_head=get_nums(pod_head); /*Count the number of mammals in each pod, and save that number 
                                              as num_in_pod for each pod*/
    
                if(pod_head==NULL) {    /*If after deleting any remaining singles pod_head is null
                                         then no pods could be found*/
                    printf("\nNo pods were detected\n");
                    break;
                }   
    
                ret_val=print_pods(pod_head);   /*If there are pods, they are printed*/
    
                if(ret_val==1) {
                    printf("Could not print pod data\n");
                    break;
                }
            break;
            
            default:
                break;
            }
    } while (user_choice != QUIT);
    
    return (EXIT_SUCCESS);
}

/*This function is used to remove all of the recorded observers and free their
 associated memory*/
struct observer *clear_observers(struct observer *obs_head) {
    
    struct observer *obs_ptr = obs_head;
    struct observer *temp;
    
    while(obs_ptr!=NULL) {
        temp=obs_ptr;
        obs_ptr=obs_ptr->next;
        free(temp);
    }
    obs_head=NULL;
    return obs_head;
}


/*This function is used to remove all of the recorded sightings and free their
 associated memory*/
struct sighting *clear_sightings(struct sighting *sight_head) {
    
    struct sighting *s_ptr = sight_head;
    struct sighting *temp;
    
    while(s_ptr!=NULL) {
        temp=s_ptr;
        s_ptr=s_ptr->next;
        free(temp);
    }
    sight_head=NULL;
    return sight_head;
}


/*This function is used to remove all of the recorded pods and free their
 associated memory*/
struct pod *clear_pods(struct pod *pod_head) {
    
    struct pod *pod_ptr = pod_head;
    struct pod *p_temp;
    struct sighting *s_ptr;
    struct sighting *s_temp;
    
    while(pod_ptr != NULL) /*Go through each pod*/
    {   
        s_ptr=pod_ptr->c_mammals;
        while(s_ptr!=NULL) { /*Go through each sighting for each pod*/
            s_temp=s_ptr;
            s_ptr=s_ptr->next;
            free(s_temp);
        }
        pod_ptr->c_mammals=NULL;
        p_temp=pod_ptr;
        pod_ptr=pod_ptr->next;
        free(p_temp);    
    }
    pod_head=NULL;
    return pod_head;
}


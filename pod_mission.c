/*This file contains methods related to the creation of pods.*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "navigation.h"
#include "records.h"

/*Recursive function to remove a pod from the linked list
 This code was adapted from https://www.cs.bu.edu/teaching/c/linked-list/delete/*/
struct pod *rmv_pod(struct pod* pod_head, struct pod* to_remove) {
    
    struct pod *temp;
    
    if (pod_head == NULL) { /*Check that the pod is not empty*/
      return pod_head;
    }
  
    if (pod_head == to_remove) { /*If the first pod in the list is the one to remove*/
        temp = pod_head->next;  /*Store the second pod in the list*/
        free(pod_head);         /*Free the head*/
        return temp;            /*Return the second pod in the list, which is now the head*/
    }

    pod_head->next = rmv_pod(pod_head->next, to_remove); /*Recursively go through the list*/
    
    return pod_head;
}

/*Returns the size of a given linked list of pods*/
int pod_list_size(struct pod *list) {
    
    struct pod *ptr = list;
    int i=0;
    
    while(ptr!= NULL) {
        i++;
        ptr = ptr->next;
    }
    return i;
}

/*Pod mission*/
/*This function places every cetacean mammal into its own pod.*/
struct pod* get_pods(struct sighting* sight_head) {
    
    struct sighting *s_ptr = sight_head;
    struct pod *current, *previous, *head=NULL;
    struct sighting *new;
    
    if(sight_head==NULL) {
        return NULL;
    }
    
    while(s_ptr != NULL)
    {
        current = malloc(sizeof(struct pod));
        new = malloc(sizeof(struct sighting));
        
        new->bearing=s_ptr->bearing;
        new->cm_loc=s_ptr->cm_loc;
        strcpy(new->observer,s_ptr->observer);
        new->range=s_ptr->range;
        new->true_loc=s_ptr->true_loc;
        new->type=s_ptr->type;
        new->next=NULL;
        current->c_mammals=new;
        
        if(head==NULL) {
                head=current;
            }
            else {
                previous->next=current;
            }
            current->next=NULL;
            previous=current;
        
        s_ptr = s_ptr->next;
    }
    return head;
}

/*Pod mission*/
/*This function removes duplicate pods - if two mammals have the same true location,
 we can assume they are the same mammal for the purpose of counting pods*/
struct pod* rmv_duplicates(struct pod* pod_head) {
    
  struct pod *first_ptr, *sec_ptr, *duplicate;
  first_ptr = pod_head;
 
  while(first_ptr != NULL && first_ptr->next != NULL) {
     sec_ptr = first_ptr;
 
     while(sec_ptr->next != NULL) {
       if(first_ptr->c_mammals->true_loc.lat == sec_ptr->next->c_mammals->true_loc.lat && first_ptr->
                c_mammals->true_loc.lng == sec_ptr->next->c_mammals->true_loc.lng) {
          duplicate = sec_ptr->next;
          sec_ptr->next = sec_ptr->next->next;
          free(duplicate);
       }
       else {
          sec_ptr = sec_ptr->next;
       }
     }
     first_ptr = first_ptr->next;
  }
  return pod_head;
}

/*Pod mission*/
/*This function merges two sets of sightings, allowing two pods to become one (awwwww).
 The second pod can then be deleted, so that the sightings gradually cluster together*/
struct sighting *merge_sightings(struct sighting *one, struct sighting *two) {
    
    struct sighting *a_ptr = one;
    struct sighting *b_ptr = one;
    
    one = one->next;
    
    while (two !=NULL && one !=NULL) { /*Loop through all of the sightings in both pods*/
        b_ptr->next = two;             
        two = two->next;
        b_ptr = b_ptr->next;
        b_ptr->next = one;
        one = one->next;
        b_ptr = b_ptr->next;
    }
    
    /*For the end of the list*/
    
    if (two !=NULL) {
        b_ptr->next = two;
    }
    else if (one !=NULL) { 
        b_ptr->next = one;
    }
    else {
        b_ptr->next = NULL;
    }
    return a_ptr;
} 

/*Pod mission*/
/*This function decides which cetacean mammals may be swimming in a pod.*/
struct pod* find_pods(struct pod* pod_head) {
    
    struct pod *pod_ptr=pod_head;
    struct pod *comp_ptr=pod_head->next; /*The next pod in the list*/
    double list_length;
    double g_circle;
    int i;
    
    while(pod_ptr!= NULL) { /*while there are pods to process*/
        list_length=pod_list_size(pod_head); /*Fetch the length of the list*/
        comp_ptr=pod_ptr->next;     /*Reset the comparison pointer*/
        i=0;                        /*Reset i*/
        while(i<list_length) { /*For every mammal in the list*/
            if(comp_ptr==NULL) { /*check that the comparison pointer isn't null*/
                break;
                }
            if(pod_ptr->c_mammals->type==comp_ptr->c_mammals->type) { /*if types are the same*/
                g_circle=great_circle(pod_ptr->c_mammals->true_loc,comp_ptr->c_mammals->true_loc);
                if(g_circle<0.1) { /*if the distance is small enough*/
                    pod_ptr->c_mammals=merge_sightings(pod_ptr->c_mammals,comp_ptr->c_mammals);
                    rmv_pod(pod_head,comp_ptr); /*Remove the comparison pod*/
                }
            }           
        comp_ptr=pod_ptr->next;
        i++;
        }
        pod_ptr=pod_ptr->next; 
    }
    return pod_head;
}

/*Pod Mission*/
/*This function finds any 'pods' that only have one cetacean mammal remaining and removes
 them*/
struct pod *rmv_singles(struct pod *pod_head) {
    
    struct pod *pod_ptr=pod_head;
    struct pod *temp;
    
    while(pod_ptr!=NULL) {
        if(pod_ptr->c_mammals->next==NULL) { /*If there is only one cetacean mammal*/
            if(pod_ptr->next==NULL) { /*If there is only one pod*/
                pod_head=NULL;         /*There are no pods left, so the pod list is null*/
                return pod_head;
            }                           /*If there is more than one cetacean mammal*/
            else {
                temp=pod_ptr;
                pod_ptr=pod_ptr->next;
                pod_head=rmv_pod(pod_head,temp);
            } 
        }
        else {
            pod_ptr=pod_ptr->next;
        }
    }
    return pod_head;
}

/*This function records for each pod the number of cetacean mammals in that pod*/
struct pod *get_nums(struct pod *pod_head) {
    
    int num;
    struct pod *pod_ptr=pod_head;
    
    while(pod_ptr!=NULL) {
        num=num_in_list(pod_ptr->c_mammals);
        pod_ptr->num_in_pod=num;
        pod_ptr=pod_ptr->next;      
    }
    return pod_head;
}
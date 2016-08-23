/*This file contains functions which are used to print tables which neatly present 
 * the cetacean data.*/

#include <stdio.h>
#include <stdlib.h>
#include "navigation.h"
#include "records.h"

/*This function prints the first list of sightings. A table is printed which includes
 the latitude, longitude, type and observer of each cetacean mammal*/
int print_sightings(struct sighting* head) {
    
    struct sighting *ptr = head;
    
    printf("\nLatitude\tLongitude\tType\tObserver\n");
    printf("==============================================");
    
    while(ptr != NULL)
    {
        printf("\n%f\t%f\t%c\t%s",ptr->cm_loc.lat,ptr->cm_loc.lng,
                    ptr->type,ptr->observer);
        ptr = ptr->next;
    }
    printf("\n==============================================");
    return 0;
}

/*This function prints the second list of sightings. A table is printed which includes
 the latitude, longitude, type, observer, and true latitude and longitude of each cetacean
 mammal*/
int print_alt_sightings(struct sighting* head) {
    
    struct sighting *ptr = head;
    
    printf("\nLatitude\tLongitude\tType\tObserver\tTrue Latitude\tTrue Longitude\n");
    printf("===================================================================================");
    
    while(ptr != NULL)
    {
        printf("\n%f\t%f\t%c\t%s\t\t%f\t%f",ptr->cm_loc.lat,ptr->cm_loc.lng,
                    ptr->type,ptr->observer,ptr->true_loc.lat,ptr->true_loc.lng);
        ptr = ptr->next;
    }
    printf("\n===================================================================================");
    return 0;
}

/*This function prints of the pods. For each pod, the number of mammals in the pod is printed,
 followed by a list of the types and locations of each mammal*/
int print_pods(struct pod* pod_head) {
    
    struct pod *pod_ptr = pod_head;
    struct sighting *s_ptr;
    
    if (pod_head==NULL) {
        return 1;
    }
    
    while(pod_ptr != NULL)
    {
        printf("\nNumber in Pod: %d\n",pod_ptr->num_in_pod);
        printf("================================\n");
        printf("Type\tLatitude\tLongitude\n");
        s_ptr=pod_ptr->c_mammals;
        while(s_ptr!=NULL) { /*A second loop is needed to print out the list of sightings for
                              each pod*/
            printf("\n%c\t%f\t%f",s_ptr->type,s_ptr->true_loc.lat,s_ptr->true_loc.lng);
            s_ptr=s_ptr->next;
        }
        printf("\n================================");
        pod_ptr=pod_ptr->next;
    }
    return 0;
}

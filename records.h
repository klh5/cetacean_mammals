/* 
 * File:   records.h
 * Author: klh5
 *
 * Created on 13 November 2014, 09:46
 */

#ifndef RECORDS_H

/*Options for the user menu*/
#define INITIAL_MISSION 1
#define MAIN_MISSION 2
#define POD_MISSION 3
#define CLEAR_DATA 4
#define QUIT 5

/*Structure for observer data*/
struct observer{
    char obs_id[5];
    location obs_loc;
    struct observer *next;
};

/*Structure for sightings data*/
struct sighting{
    char observer[5];
    char type;
    double bearing;
    double range;  
    location cm_loc;
    location true_loc;
    struct sighting *next;
};

/*Structure for pods*/
struct pod{
    int num_in_pod;
    struct sighting *c_mammals;
    struct pod *next;
};

/*List of functions*/
struct observer *clear_observers(struct observer *obs_head);
struct sighting *clear_sightings(struct sighting *sight_head);
struct pod *clear_pods(struct pod *pod_head);

/*Initial Mission*/
struct observer* read_obs(char*obs_file);
struct sighting* read_sight(char*sight_file);
struct sighting* get_cm_locations(struct sighting* s_head,struct observer* obs_head );
struct sighting* check_sightings(struct sighting* sight_head);
int print_sightings(struct sighting* head);

/*Main Mission*/
struct sighting* get_same_location(struct sighting* sight_head);
int num_in_list(struct sighting *list);

/*Pod Mission*/
struct pod* get_pods(struct sighting* sight_head);
struct pod* rmv_duplicates(struct pod* pod_head);
struct pod* find_pods(struct pod* pod_head);
struct pod *rmv_singles(struct pod *pod_head);
struct pod *get_nums(struct pod *pod_head);

#define	RECORDS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* RECORDS_H */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    if (pthread_cond_init(&(station->train_in_station), NULL) != 0) {
        perror("pthread_cond_init() error");
        exit(1);
    }

    if (pthread_cond_init(&(station->train_is_ready_to_go), NULL) != 0) {
        perror("pthread_cond_init() error");
        exit(1);
    }

    if(pthread_mutex_init(&(station->lock), NULL) != 0) {
        perror("pthread_mutex_init() error");
        exit(1);
    }
    station->waiting_passengers = 0;
    station->enough_seats_count = 0;
    station->available_seats_count = 0;

}

void
station_load_train(struct station *station, int count)
{


    pthread_mutex_lock(&(station->lock));
    station->available_seats_count = count;
    station->enough_seats_count = count;

    if(station->waiting_passengers != 0 && station->available_seats_count != 0){
//      notify all waited passengers
        pthread_cond_broadcast(&(station->train_in_station));
//      let the train wait until it was full or there's no passengers left in the station
        pthread_cond_wait(&(station->train_is_ready_to_go), &(station->lock));

    }
    pthread_mutex_unlock(&(station->lock));

}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&(station->lock));

    station->waiting_passengers++;
    do {
//      let the passenger wait till there's a train in the station
        pthread_cond_wait(&(station->train_in_station), &(station->lock));
//      update the enough seats based on the awoken passengers
        station->enough_seats_count--;
    }
//  let the passenger wait to another train if there's no enough seats on the current train
    while (station->enough_seats_count < 0);

    pthread_mutex_unlock(&(station->lock));


}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&(station->lock));
//  put passengers on the train and update the counters
    if(station->waiting_passengers != 0 && station->available_seats_count != 0){
        station->waiting_passengers--;
        station->available_seats_count--;

    }
    pthread_mutex_unlock(&(station->lock));

//  notify the train to leave the station when the train is full or there 's no waiting passengers
    if(station->available_seats_count == 0 || station->waiting_passengers == 0){
        pthread_cond_signal(&(station->train_is_ready_to_go));
    }


}

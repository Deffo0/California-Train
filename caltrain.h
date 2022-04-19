#include <pthread.h>

struct station {
	pthread_cond_t train_is_ready_to_go;
    pthread_cond_t train_in_station;
    pthread_mutex_t lock;
    int waiting_passengers;
    int enough_seats_count;
    int available_seats_count;



};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
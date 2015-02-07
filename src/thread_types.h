#ifndef THREAD_TYPES_H
#define THREAD_TYPES_H

#include <stdint.h>
#include <stdio.h>

/* thread states */
#define THREAD_RUNNING                0
#define THREAD_SHUTDOWN               1

#define THREAD_DATA_INVALID           0
#define THREAD_DATA_VALID             1
#define THREAD_DATA_LEN_INVALID      -1

#define BUF_SIZE 1024
#define PATH_SIZE 512
#define MODE_SIZE 3
#define NUM_MAX_THREADS 32

/* typedefs and forward declarations */
typedef int16_t thread_id_t;
typedef struct _thread_context thread_context;

/* prototypes */
void thread_mutex_lock(thread_id_t thread_id);
void thread_mutex_unlock(thread_id_t thread_id);
void thread_set_shutdown_flag(thread_id_t thread_id);
char* get_read_buffer(thread_id_t thread_id);
int16_t get_read_buffer_size(thread_id_t thread_id);
int16_t get_read_buffer_len(thread_id_t thread_id);
uint8_t is_valid_thread_id(thread_id_t thread_id);
uint8_t is_thread_data_valid(thread_id_t thread_id);
void thread_join(thread_id_t thread_id);

#endif /* THREAD_TYPES_H */

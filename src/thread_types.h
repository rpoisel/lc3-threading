#ifndef THREAD_TYPES_H
#define THREAD_TYPES_H

#include <stdint.h>
#include <stdio.h>

/* thread states */
#define THREAD_RUNNING         0
#define THREAD_SHUTDOWN        1

#define BUF_SIZE 1024
#define PATH_SIZE 512
#define MODE_SIZE 3

/* forward declarations */
typedef struct _thread_context thread_context;

/* prototypes */
void thread_mutex_lock(void);
void thread_mutex_unlock(void);
void thread_set_shutdown_flag(void);
char* get_read_buffer(void);
int get_read_buffer_len(void);

#endif /* THREAD_TYPES_H */

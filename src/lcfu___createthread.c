#ifndef LC_PROT_LCFU___CREATETHREAD__C
#define LC_PROT_LCFU___CREATETHREAD__C

#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <lcfu___createthread.h>

#include <thread_types.h>

#define THREAD_CYCLE_TIME 100000000L /* nanoseconds */

typedef struct _thread_data thread_data;

static void* runner(void* params);
static void worker(thread_data* data);
static void shutdown_worker(thread_data* data);

struct _thread_data
{
	thread_id_t thread_id;
	char path[PATH_SIZE];
	char mode[MODE_SIZE];
	FILE* fh;
	char buffers[2][BUF_SIZE];
	uint8_t work_buf_idx;
	pthread_mutex_t mutex;
};

struct _thread_context
{
	pthread_t pthread_id;
	uint8_t used;
	uint8_t shutdown;
	uint32_t period;
	thread_data data;
};

static thread_context _thread_contexts[NUM_MAX_THREADS];
static uint8_t _initialized = 0;

/*                            Functions                        */
void  lcfu___CREATETHREAD(LC_TD_Function_CREATETHREAD* LC_this, LcCgChar LC_VD_PATH[512], LcCgChar LC_VD_MODE[3], LC_TD_UDINT LC_VD_PERIOD, struct _lcoplck_epdb_1_impl* pEPDB)
{
	thread_id_t thread_id = 0;
	if (!_initialized)
	{
		for (thread_id = 0; thread_id < NUM_MAX_THREADS; thread_id++)
		{
			memset(&_thread_contexts[thread_id], 0, sizeof(_thread_contexts[0]));
			pthread_mutex_init(&(_thread_contexts[thread_id].data.mutex), NULL);
		}
		_initialized = 1;
	}

	for (thread_id = 0; thread_id < NUM_MAX_THREADS; thread_id++)
	{
		if (!_thread_contexts[thread_id].used)
		{
			break;
		}
	}
	if (thread_id == NUM_MAX_THREADS)
	{
		LC_this->LC_VD_CREATETHREAD = (LC_TD_DINT)-1;
		LC_this->LC_VD_ENO = LC_EL_false;
		return;
	}

	LC_this->LC_VD_CREATETHREAD = thread_id;
	_thread_contexts[thread_id].data.thread_id = thread_id;
	_thread_contexts[thread_id].used = 1;
	_thread_contexts[thread_id].shutdown = THREAD_RUNNING;
	if (LC_VD_PERIOD > 0)
	{
		_thread_contexts[thread_id].period = LC_VD_PERIOD;
	}
	strncpy(_thread_contexts[thread_id].data.path, LC_VD_PATH, PATH_SIZE);
	strncpy(_thread_contexts[thread_id].data.mode, LC_VD_MODE, MODE_SIZE);
	pthread_create(&(_thread_contexts[thread_id].pthread_id), NULL, runner,
			_thread_contexts + thread_id);
}

void thread_set_shutdown_flag(thread_id_t thread_id)
{
	_thread_contexts[thread_id].shutdown = THREAD_SHUTDOWN;
}

void thread_mutex_lock(thread_id_t thread_id)
{
	pthread_mutex_lock(&(_thread_contexts[thread_id].data.mutex));
}

void thread_mutex_unlock(thread_id_t thread_id)
{
	pthread_mutex_unlock(&(_thread_contexts[thread_id].data.mutex));
}

char* get_read_buffer(thread_id_t thread_id)
{
	return _thread_contexts[thread_id].data.buffers[!_thread_contexts[thread_id].data.work_buf_idx];
}

int get_read_buffer_len(thread_id_t thread_id)
{
	(void)thread_id;
	return BUF_SIZE;
}

uint8_t is_valid_thread_id(thread_id_t thread_id)
{
	return thread_id >= 0 && thread_id < NUM_MAX_THREADS;
}

void thread_join(thread_id_t thread_id)
{
	pthread_join(_thread_contexts[thread_id].pthread_id, NULL);
	_thread_contexts[thread_id].used = 0;
}

static void* runner(void* params)
{
	thread_context* context = (thread_context*)params;
	struct timespec sleep_period = { 0, 0 };
	sleep_period.tv_nsec = context->period;

	while(context->shutdown != THREAD_SHUTDOWN)
	{
		worker(&context->data);
		nanosleep(&sleep_period, NULL);
	}

	shutdown_worker(&context->data);
	return NULL;
}

static void worker(thread_data* data)
{
	size_t num_read = 0;

	/* file has not been opened yet */
	if (!data->fh)
	{
		data->fh = fopen(data->path, data->mode);
	}

	if (!data->fh)
	{
		/* file could not be opened */
		return;
	}
	fseek(data->fh, 0, SEEK_SET);
	num_read = fread(data->buffers[data->work_buf_idx], 1, BUF_SIZE, data->fh); /* may block */

	/* after reading the file's contents into the work buffer,
	 * do some clean up of the work buffer's contents */

	if (num_read <= 0)
	{
		return;
	}

	if (num_read < BUF_SIZE)
	{
		data->buffers[data->work_buf_idx][num_read] = '\0';
	}

	/* and then flip the buffers */
	thread_mutex_lock(data->thread_id);
	data->work_buf_idx = !data->work_buf_idx;
	thread_mutex_unlock(data->thread_id);
}

static void shutdown_worker(thread_data* data)
{
	if (data->fh)
	{
		fclose(data->fh);
		data->fh = NULL;
	}
}

#endif

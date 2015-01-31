#ifndef LC_PROT_LCFU___CREATETHREAD__C
#define LC_PROT_LCFU___CREATETHREAD__C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <lcfu___createthread.h>

#include <thread_types.h>

#define THREAD_CYCLE_TIME 100000000L /* nanoseconds */

typedef struct _thread_data thread_data;
typedef struct _thread_context thread_context;

static void* runner(void* params);
static void worker(thread_data* data);
static void shutdown_worker(thread_data* data);

struct _thread_data
{
	char path[PATH_SIZE];
	char mode[MODE_SIZE];
	FILE* fh;
	char buffers[2][BUF_SIZE];
	uint8_t work_buf_idx;
	pthread_mutex_t mutex;
};

struct _thread_context
{
	uint8_t shutdown;
	uint32_t period;
	thread_data data;
};

#define THREAD_CONTEXT_INITIALIZER \
	{ \
		0,                                      /* shutdown */ \
		THREAD_CYCLE_TIME,                      /* period */ \
		{ \
			{ '\0' },                           /* path */ \
			{ '\0' },                           /* mode */ \
			NULL,                               /* fh */ \
			{ { '\0' }, { '\0' } },             /* buffers */ \
			0,                                  /* work_buf_idx */ \
			PTHREAD_MUTEX_INITIALIZER           /* mutex */ \
		} \
	}

static thread_context _thread_context = THREAD_CONTEXT_INITIALIZER;

/*                            Functions                        */
void  lcfu___CREATETHREAD(LC_TD_Function_CREATETHREAD* LC_this, LcCgChar LC_VD_PATH[512], LcCgChar LC_VD_MODE[3], LC_TD_UDINT LC_VD_PERIOD, struct _lcoplck_epdb_1_impl* pEPDB)
{
	pthread_t thread_id = 0;
	_thread_context.shutdown = THREAD_RUNNING;
	if (LC_VD_PERIOD > 0)
	{
		_thread_context.period = LC_VD_PERIOD;
	}
	strncpy(_thread_context.data.path, LC_VD_PATH, PATH_SIZE);
	strncpy(_thread_context.data.mode, LC_VD_MODE, MODE_SIZE);
	pthread_create(&thread_id, NULL, runner, &_thread_context);
}

void thread_set_shutdown_flag(void)
{
	_thread_context.shutdown = THREAD_SHUTDOWN;
}

void thread_mutex_lock(void)
{
	pthread_mutex_lock(&(_thread_context.data.mutex));
}

void thread_mutex_unlock(void)
{
	pthread_mutex_unlock(&(_thread_context.data.mutex));
}

char* get_read_buffer(void)
{
	return _thread_context.data.buffers[!_thread_context.data.work_buf_idx];
}

int get_read_buffer_len(void)
{
	return BUF_SIZE;
}

static void* runner(void* params)
{
	thread_context* context = (thread_context*)params;
	struct timespec sleep_period = { 0, context->period };

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
	thread_mutex_lock();
	data->work_buf_idx = !data->work_buf_idx;
	thread_mutex_unlock();
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

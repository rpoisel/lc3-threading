#ifndef LC_PROT_LCFU___JOINTHREAD__C
#define LC_PROT_LCFU___JOINTHREAD__C

#include <stdlib.h>
#include <pthread.h>
#include <lcfu___jointhread.h>

#include <thread_types.h>

/*                            Functions                        */
void  lcfu___JOINTHREAD(LC_TD_Function_JOINTHREAD* LC_this, LC_TD_LINT LC_VD_THREAD_ID, struct _lcoplck_epdb_1_impl* pEPDB)
{
	thread_set_shutdown_flag();
	pthread_join(LC_VD_THREAD_ID, NULL);
}

#endif

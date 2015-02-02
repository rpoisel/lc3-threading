#ifndef LC_PROT_LCFU___JOINTHREAD__C
#define LC_PROT_LCFU___JOINTHREAD__C

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <lcfu___jointhread.h>

#include <thread_types.h>

/*                            Functions                        */
void  lcfu___JOINTHREAD(LC_TD_Function_JOINTHREAD* LC_this, LC_TD_DINT LC_VD_THREAD_ID, struct _lcoplck_epdb_1_impl* pEPDB)
{
	if (!is_valid_thread_id(LC_VD_THREAD_ID))
	{
		LC_this->LC_VD_ENO = LC_EL_false;
	}
	thread_set_shutdown_flag(LC_VD_THREAD_ID);
	thread_join(LC_VD_THREAD_ID);
	printf("Quitting thread with id %d\n", LC_VD_THREAD_ID);
}

#endif

#ifndef LC_PROT_LCFU___ACCESSDATA__C
#define LC_PROT_LCFU___ACCESSDATA__C

#include <string.h>

#include <lcfu___accessdata.h>

#include <thread_types.h>

/*                            Functions                        */
void  lcfu___ACCESSDATA(LC_TD_Function_ACCESSDATA* LC_this, struct _lcoplck_epdb_1_impl* pEPDB)
{
	thread_mutex_lock();
	strncpy(LC_this->LC_VD_BUFFER, get_read_buffer(), BUF_SIZE);
	thread_mutex_unlock();

	/* modify LC_this->LC_VD_BUFFER to your convenience (e. g. parse numeric values
	 * or strip unnecessary data out of it), in case it is necessary */
}

#endif

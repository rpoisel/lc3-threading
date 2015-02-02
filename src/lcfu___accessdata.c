#ifndef LC_PROT_LCFU___ACCESSDATA__C
#define LC_PROT_LCFU___ACCESSDATA__C

#include <string.h>

#include <lcfu___accessdata.h>

#include <thread_types.h>

/*                            Functions                        */
void  lcfu___ACCESSDATA(LC_TD_Function_ACCESSDATA* LC_this, LC_TD_DINT LC_VD_THREAD_ID, struct _lcoplck_epdb_1_impl* pEPDB)
{
	if (!is_valid_thread_id(LC_VD_THREAD_ID))
	{
		LC_this->LC_VD_ENO = LC_EL_false;
		return;
	}
	thread_mutex_lock(LC_VD_THREAD_ID);
	strncpy(LC_this->LC_VD_BUFFER, get_read_buffer(LC_VD_THREAD_ID), BUF_SIZE);
	thread_mutex_unlock(LC_VD_THREAD_ID);

	/* modify LC_this->LC_VD_BUFFER to your convenience (e. g. parse numeric values
	 * or strip unnecessary data out of it), in case it is necessary */
}

#endif

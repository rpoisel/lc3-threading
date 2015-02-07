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
	if (is_thread_data_valid(LC_VD_THREAD_ID) == THREAD_DATA_VALID)
	{
		LC_this->LC_VD_BUF_LEN = get_read_buffer_len(LC_VD_THREAD_ID);
		memcpy(LC_this->LC_VD_BUFFER, get_read_buffer(LC_VD_THREAD_ID), LC_this->LC_VD_BUF_LEN);
		/* as this is a string, zero terminate it */
		LC_this->LC_VD_BUFFER[
			LC_this->LC_VD_BUF_LEN > get_read_buffer_size(LC_VD_THREAD_ID) ? (get_read_buffer_size(LC_VD_THREAD_ID) - 1) : LC_this->LC_VD_BUF_LEN
			] = '\0';
	}
	else
	{
		LC_this->LC_VD_ENO = LC_EL_false;
	}
	thread_mutex_unlock(LC_VD_THREAD_ID);

	/* modify LC_this->LC_VD_BUFFER to your convenience (e. g. parse numeric values
	 * or strip unnecessary data out of it), in case it is necessary */
}

#endif

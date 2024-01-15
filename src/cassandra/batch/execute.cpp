#include "execute.hpp"
#include "cassandra/cassandra.hpp"
#include <cassandra.h>

void Cassandra_batch_execute(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassFuture* batch_future = cass_session_execute_batch(ptr->profile->session, ptr->batch);

	if (cass_future_error_code(batch_future) != CASS_OK)
	{
		const char* message;
		size_t message_length;
		cass_future_error_message(batch_future, &message, &message_length);
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, message, message_length);
		cass_future_free(batch_future);
		return;
	}

	cass_future_free(batch_future);
}

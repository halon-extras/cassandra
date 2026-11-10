#include "execute.hpp"
#include "cassandra/cassandra.hpp"
#include <cassandra.h>

void Cassandra_batch_execute(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassFuture* batch_future = cass_session_execute_batch(ptr->profile->session, ptr->batch);

	if (cass_future_error_code(batch_future) != CASS_OK)
		throw_cass_future_error(hhc, batch_future);
	cass_future_free(batch_future);
}

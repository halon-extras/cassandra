#include "batch.hpp"
#include "execute.hpp"
#include "cassandra/statement/statement.hpp"
#include "cassandra/cassandra.hpp"
#include "argument.hpp"

void Cassandra_batch(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	CassBatchType type;
	std::string _type;
	if (!parse_hsl_argument_as_string(hhc, args, 0, true, _type))
		return;

	if (_type == "LOGGED")
		type = CASS_BATCH_TYPE_LOGGED;
	else if (_type == "UNLOGGED")
		type = CASS_BATCH_TYPE_UNLOGGED;
	else if (_type == "COUNTER")
		type = CASS_BATCH_TYPE_COUNTER;
	else
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, "invalid argument", 0);
		return;
	}

	CassBatch* batch = cass_batch_new(type);

	Cassandra* ptr1 = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	Cassandra* ptr2 = new Cassandra();
	ptr2->profile = ptr1->profile;
	ptr2->batch = batch;

	HalonHSLObject* object = HalonMTA_hsl_object_new();
	HalonMTA_hsl_object_type_set(object, "CassandraBatch");
	HalonMTA_hsl_object_register_function(object, "statement", Cassandra_statement);
	HalonMTA_hsl_object_register_function(object, "execute", Cassandra_batch_execute);
	HalonMTA_hsl_object_ptr_set(object, ptr2, Cassandra_batch_free);
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_OBJECT, object, 0);
	HalonMTA_hsl_object_delete(object);
}

void Cassandra_batch_free(void* ptr)
{
	Cassandra* x = (Cassandra*)ptr;
	cass_batch_free(x->batch);
	delete x;
}

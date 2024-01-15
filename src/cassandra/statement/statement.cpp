#include "statement.hpp"
#include "bind.hpp"
#include "execute.hpp"
#include "paging.hpp"
#include "cassandra/cassandra.hpp"
#include "argument.hpp"

void Cassandra_statement(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	std::string query;
	if (!parse_hsl_argument_as_string(hhc, args, 0, true, query))
		return;
	double parameter_count = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, false, parameter_count))
		return;

	CassStatement* statement = cass_statement_new(query.c_str(), (size_t)parameter_count);

	Cassandra* ptr1 = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	Cassandra* ptr2 = new Cassandra();
	ptr2->profile = ptr1->profile;
	ptr2->statement = statement;

	if (ptr1->batch)
		cass_batch_add_statement(ptr1->batch, statement);

	HalonHSLObject* object = HalonMTA_hsl_object_new();
	HalonMTA_hsl_object_type_set(object, "CassandraStatement");
	HalonMTA_hsl_object_register_function(object, "paging_size", Cassandra_statement_paging_size);
	HalonMTA_hsl_object_register_function(object, "has_more_pages", Cassandra_statement_has_more_pages);
	HalonMTA_hsl_object_register_function(object, "execute", Cassandra_statement_execute);
	HalonMTA_hsl_object_register_function(object, "bind_bool", Cassandra_statement_bind_bool);
	HalonMTA_hsl_object_register_function(object, "bind_bytes", Cassandra_statement_bind_bytes);
	HalonMTA_hsl_object_register_function(object, "bind_double", Cassandra_statement_bind_double);
	HalonMTA_hsl_object_register_function(object, "bind_float", Cassandra_statement_bind_float);
	HalonMTA_hsl_object_register_function(object, "bind_int8", Cassandra_statement_bind_int8);
	HalonMTA_hsl_object_register_function(object, "bind_int16", Cassandra_statement_bind_int16);
	HalonMTA_hsl_object_register_function(object, "bind_int32", Cassandra_statement_bind_int32);
	HalonMTA_hsl_object_register_function(object, "bind_int64", Cassandra_statement_bind_int64);
	HalonMTA_hsl_object_register_function(object, "bind_uint32", Cassandra_statement_bind_uint32);
	HalonMTA_hsl_object_register_function(object, "bind_decimal", Cassandra_statement_bind_decimal);
	HalonMTA_hsl_object_register_function(object, "bind_inet", Cassandra_statement_bind_inet);
	HalonMTA_hsl_object_register_function(object, "bind_null", Cassandra_statement_bind_null);
	HalonMTA_hsl_object_register_function(object, "bind_string", Cassandra_statement_bind_string);
	HalonMTA_hsl_object_register_function(object, "bind_uuid", Cassandra_statement_bind_uuid);
	HalonMTA_hsl_object_register_function(object, "bind_duration", Cassandra_statement_bind_duration);
	HalonMTA_hsl_object_ptr_set(object, ptr2, Cassandra_statement_free);
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_OBJECT, object, 0);
	HalonMTA_hsl_object_delete(object);
}

void Cassandra_statement_free(void* ptr)
{
	Cassandra* x = (Cassandra*)ptr;
	cass_statement_free(x->statement);
	delete x;
}

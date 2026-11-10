#include "bind.hpp"
#include "statement.hpp"
#include "argument.hpp"
#include "cassandra/cassandra.hpp"
#include <cassandra.h>
#include <stdlib.h>
#include <string>

static CassStatement* get_statement(HalonHSLContext* hhc)
{
	return ((Cassandra*)HalonMTA_hsl_object_ptr_get(hhc))->statement;
}

static void bind_result(HalonHSLContext* hhc, HalonHSLValue* ret, CassError error)
{
	if (error != CASS_OK)
	{
		throw_cass_error(hhc, error);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

// bind_*(number $index, number $value)
template <typename T>
static void bind_number(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret, CassError (*bind)(CassStatement*, size_t, T))
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	bind_result(hhc, ret, bind(get_statement(hhc), (size_t)index, (T)value));
}

// bind_*(number $index, string $value)
template <typename F>
static void bind_string_value(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret, F bind)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string value;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, value))
		return;
	bind_result(hhc, ret, bind(get_statement(hhc), (size_t)index, value));
}

void Cassandra_statement_bind_bool(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	bool value;
	if (!parse_hsl_argument_as_bool(hhc, args, 1, true, value))
		return;
	bind_result(hhc, ret, cass_statement_bind_bool(get_statement(hhc), (size_t)index, value ? cass_bool_t::cass_true : cass_bool_t::cass_false));
}

void Cassandra_statement_bind_bytes(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_string_value(hhc, args, ret, [](CassStatement* statement, size_t index, const std::string& value) {
		return cass_statement_bind_bytes(statement, index, (const cass_byte_t*)value.data(), value.size());
	});
}

void Cassandra_statement_bind_double(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_number(hhc, args, ret, cass_statement_bind_double);
}

void Cassandra_statement_bind_float(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_number(hhc, args, ret, cass_statement_bind_float);
}

void Cassandra_statement_bind_int8(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_number(hhc, args, ret, cass_statement_bind_int8);
}

void Cassandra_statement_bind_int16(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_number(hhc, args, ret, cass_statement_bind_int16);
}

void Cassandra_statement_bind_int32(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_number(hhc, args, ret, cass_statement_bind_int32);
}

void Cassandra_statement_bind_int64(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_string_value(hhc, args, ret, [](CassStatement* statement, size_t index, const std::string& value) {
		return cass_statement_bind_int64(statement, index, strtoll(value.c_str(), nullptr, 10));
	});
}

void Cassandra_statement_bind_uint32(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_number(hhc, args, ret, cass_statement_bind_uint32);
}

void Cassandra_statement_bind_decimal(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string varint;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, varint))
		return;
	double scale = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 2, true, scale))
		return;
	bind_result(hhc, ret, cass_statement_bind_decimal(get_statement(hhc), (size_t)index, (const cass_byte_t*)varint.data(), varint.size(), (cass_int32_t)scale));
}

void Cassandra_statement_bind_inet(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_string_value(hhc, args, ret, [](CassStatement* statement, size_t index, const std::string& value) {
		CassInet inet;
		CassError error = cass_inet_from_string_n(value.data(), value.size(), &inet);
		if (error != CASS_OK)
			return error;
		return cass_statement_bind_inet(statement, index, inet);
	});
}

void Cassandra_statement_bind_null(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	bind_result(hhc, ret, cass_statement_bind_null(get_statement(hhc), (size_t)index));
}

void Cassandra_statement_bind_string(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_string_value(hhc, args, ret, [](CassStatement* statement, size_t index, const std::string& value) {
		return cass_statement_bind_string_n(statement, index, value.data(), value.size());
	});
}

void Cassandra_statement_bind_uuid(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	bind_string_value(hhc, args, ret, [](CassStatement* statement, size_t index, const std::string& value) {
		CassUuid uuid;
		CassError error = cass_uuid_from_string_n(value.data(), value.size(), &uuid);
		if (error != CASS_OK)
			return error;
		return cass_statement_bind_uuid(statement, index, uuid);
	});
}

void Cassandra_statement_bind_duration(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double months = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, months))
		return;
	double days = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 2, true, days))
		return;
	std::string nanos;
	if (!parse_hsl_argument_as_string(hhc, args, 3, true, nanos))
		return;
	bind_result(hhc, ret, cass_statement_bind_duration(get_statement(hhc), (size_t)index, (cass_int32_t)months, (cass_int32_t)days, (cass_int64_t)strtoll(nanos.c_str(), nullptr, 10)));
}

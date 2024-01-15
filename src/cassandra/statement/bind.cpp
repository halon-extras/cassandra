#include "bind.hpp"
#include "statement.hpp"
#include "argument.hpp"
#include "cassandra/cassandra.hpp"
#include <cassandra.h>
#include <string.h>

void Cassandra_statement_bind_bool(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	bool value;
	if (!parse_hsl_argument_as_bool(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_bool(ptr->statement, (size_t)index, value ? cass_bool_t::cass_true : cass_bool_t::cass_false);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_bytes(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string value;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, value))
		return;
	const char* bytes = value.c_str();
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_bytes(ptr->statement, (size_t)index, (uint8_t*)bytes, strlen(bytes));
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_double(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_double(ptr->statement, (size_t)index, value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_float(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_float(ptr->statement, (size_t)index, (cass_float_t)value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_int8(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_int8(ptr->statement, (size_t)index, (cass_int8_t)value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_int16(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_int16(ptr->statement, (size_t)index, (cass_int16_t)value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_int32(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_int32(ptr->statement, (size_t)index, (cass_int32_t)value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_int64(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string value;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_int64(ptr->statement, (size_t)index, strtoll(value.c_str(), nullptr, 10));
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_uint32(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	double value = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_uint32(ptr->statement, (size_t)index, (cass_uint32_t)value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
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
	const char* bytes = varint.c_str();
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_decimal(ptr->statement, (size_t)index, (uint8_t*)bytes, strlen(bytes), (cass_int32_t)scale);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_inet(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string value;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, value))
		return;

	CassError error;
	CassInet inet;
	error = cass_inet_from_string(value.c_str(), &inet);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	error = cass_statement_bind_inet(ptr->statement, (size_t)index, inet);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_null(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_null(ptr->statement, (size_t)index);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_string(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string value;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, value))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_string(ptr->statement, (size_t)index, value.c_str());
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_bind_uuid(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double index = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, index))
		return;
	std::string _value;
	if (!parse_hsl_argument_as_string(hhc, args, 1, true, _value))
		return;
	CassError error;
	CassUuid value;
	error = cass_uuid_from_string(_value.c_str(), &value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	error = cass_statement_bind_uuid(ptr->statement, (size_t)index, value);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
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
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_bind_duration(ptr->statement, (size_t)index, (cass_int32_t)months, (cass_int32_t)days, (cass_int64_t)strtoll(nanos.c_str(), nullptr, 10));
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

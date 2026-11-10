#include "execute.hpp"
#include <string>
#include "cassandra/cassandra.hpp"
#include <cassandra.h>

static CassError cass_value_to_hsl_value(HalonHSLValue* hsl_value, const CassValue* cass_value);

// Add an element with a numeric key to an HSL array, and return the element value
static HalonHSLValue* hsl_array_add(HalonHSLValue* array, double index)
{
	HalonHSLValue *k, *v;
	HalonMTA_hsl_value_array_add(array, &k, &v);
	HalonMTA_hsl_value_set(k, HALONMTA_HSL_TYPE_NUMBER, &index, 0);
	return v;
}

static void hsl_set_number(HalonHSLValue* hsl_value, double value)
{
	HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &value, 0);
}

static void hsl_set_string(HalonHSLValue* hsl_value, const std::string& value)
{
	HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, value.data(), value.size());
}

template <typename T>
static CassError get_number(HalonHSLValue* hsl_value, const CassValue* cass_value, CassError (*get)(const CassValue*, T*))
{
	T value;
	CassError error = get(cass_value, &value);
	if (error == CASS_OK)
		hsl_set_number(hsl_value, (double)value);
	return error;
}

// Lists, sets and tuples are arrays with numeric keys
static CassError iterator_to_hsl_value(HalonHSLValue* hsl_value, CassIterator* iterator)
{
	if (!iterator)
		return CASS_ERROR_LIB_INVALID_VALUE_TYPE;
	HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
	CassError error = CASS_OK;
	for (size_t index = 0; cass_iterator_next(iterator); ++index)
	{
		error = cass_value_to_hsl_value(hsl_array_add(hsl_value, (double)index), cass_iterator_get_value(iterator));
		if (error != CASS_OK)
			break;
	}
	cass_iterator_free(iterator);
	return error;
}

static CassError cass_value_to_hsl_value(HalonHSLValue* hsl_value, const CassValue* cass_value)
{
	// Must be checked first, the collection and tuple iterators are NULL for NULL values
	if (cass_value_is_null(cass_value))
	{
		HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
		return CASS_OK;
	}

	CassError error = CASS_OK;
	switch (cass_data_type_type(cass_value_data_type(cass_value)))
	{
		case CASS_VALUE_TYPE_ASCII:
		case CASS_VALUE_TYPE_TEXT:
		case CASS_VALUE_TYPE_VARCHAR:
		{
			const char* value;
			size_t value_length;
			error = cass_value_get_string(cass_value, &value, &value_length);
			if (error == CASS_OK)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, value, value_length);
			break;
		}
		case CASS_VALUE_TYPE_BIGINT:
		case CASS_VALUE_TYPE_COUNTER:
		case CASS_VALUE_TYPE_TIMESTAMP:
		case CASS_VALUE_TYPE_TIME:
		{
			cass_int64_t value;
			error = cass_value_get_int64(cass_value, &value);
			if (error == CASS_OK)
				hsl_set_string(hsl_value, std::to_string(value));
			break;
		}
		case CASS_VALUE_TYPE_INT:
			error = get_number(hsl_value, cass_value, cass_value_get_int32);
			break;
		case CASS_VALUE_TYPE_SMALL_INT:
			error = get_number(hsl_value, cass_value, cass_value_get_int16);
			break;
		case CASS_VALUE_TYPE_TINY_INT:
			error = get_number(hsl_value, cass_value, cass_value_get_int8);
			break;
		case CASS_VALUE_TYPE_FLOAT:
			error = get_number(hsl_value, cass_value, cass_value_get_float);
			break;
		case CASS_VALUE_TYPE_DOUBLE:
			error = get_number(hsl_value, cass_value, cass_value_get_double);
			break;
		case CASS_VALUE_TYPE_DATE:
			error = get_number(hsl_value, cass_value, cass_value_get_uint32);
			break;
		case CASS_VALUE_TYPE_INET:
		{
			CassInet value;
			error = cass_value_get_inet(cass_value, &value);
			if (error == CASS_OK)
			{
				char inet_string[CASS_INET_STRING_LENGTH];
				cass_inet_string(value, inet_string);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, inet_string, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_UUID:
		case CASS_VALUE_TYPE_TIMEUUID:
		{
			CassUuid value;
			error = cass_value_get_uuid(cass_value, &value);
			if (error == CASS_OK)
			{
				char uuid_string[CASS_UUID_STRING_LENGTH];
				cass_uuid_string(value, uuid_string);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, uuid_string, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_BOOLEAN:
		{
			cass_bool_t value;
			error = cass_value_get_bool(cass_value, &value);
			if (error == CASS_OK)
			{
				bool value_hsl = (bool)value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_BOOLEAN, &value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_DURATION:
		{
			cass_int32_t months, days;
			cass_int64_t nanos;
			error = cass_value_get_duration(cass_value, &months, &days, &nanos);
			if (error == CASS_OK)
			{
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
				hsl_set_number(hsl_array_add(hsl_value, 0), (double)months);
				hsl_set_number(hsl_array_add(hsl_value, 1), (double)days);
				hsl_set_string(hsl_array_add(hsl_value, 2), std::to_string((long long int)nanos));
			}
			break;
		}
		case CASS_VALUE_TYPE_BLOB:
		case CASS_VALUE_TYPE_VARINT:
		{
			const cass_byte_t* value;
			size_t value_size;
			error = cass_value_get_bytes(cass_value, &value, &value_size);
			if (error == CASS_OK)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, value, value_size);
			break;
		}
		case CASS_VALUE_TYPE_DECIMAL:
		{
			const cass_byte_t* varint;
			size_t varint_size;
			cass_int32_t scale;
			error = cass_value_get_decimal(cass_value, &varint, &varint_size, &scale);
			if (error == CASS_OK)
			{
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
				HalonMTA_hsl_value_set(hsl_array_add(hsl_value, 0), HALONMTA_HSL_TYPE_STRING, varint, varint_size);
				hsl_set_number(hsl_array_add(hsl_value, 1), (double)scale);
			}
			break;
		}
		case CASS_VALUE_TYPE_MAP:
		{
			CassIterator* iterator = cass_iterator_from_map(cass_value);
			if (!iterator)
				return CASS_ERROR_LIB_INVALID_VALUE_TYPE;
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
			while (cass_iterator_next(iterator))
			{
				HalonHSLValue *k, *v;
				HalonMTA_hsl_value_array_add(hsl_value, &k, &v);
				error = cass_value_to_hsl_value(k, cass_iterator_get_map_key(iterator));
				if (error == CASS_OK)
					error = cass_value_to_hsl_value(v, cass_iterator_get_map_value(iterator));
				if (error != CASS_OK)
					break;
			}
			cass_iterator_free(iterator);
			break;
		}
		case CASS_VALUE_TYPE_LIST:
		case CASS_VALUE_TYPE_SET:
			error = iterator_to_hsl_value(hsl_value, cass_iterator_from_collection(cass_value));
			break;
		case CASS_VALUE_TYPE_TUPLE:
			error = iterator_to_hsl_value(hsl_value, cass_iterator_from_tuple(cass_value));
			break;
		case CASS_VALUE_TYPE_CUSTOM:
		case CASS_VALUE_TYPE_UDT:
		case CASS_VALUE_TYPE_LAST_ENTRY:
		case CASS_VALUE_TYPE_UNKNOWN:
			// Not supported
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			break;
	}
	return error;
}

// Rows are arrays with numeric keys, columns are arrays with the column names as keys
static CassError result_to_hsl_value(HalonHSLValue* hsl_value, const CassResult* result)
{
	HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
	CassError error = CASS_OK;
	CassIterator* row_iterator = cass_iterator_from_result(result);
	for (size_t row_index = 0; cass_iterator_next(row_iterator); ++row_index)
	{
		HalonHSLValue* row = hsl_array_add(hsl_value, (double)row_index);
		HalonMTA_hsl_value_set(row, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
		CassIterator* column_iterator = cass_iterator_from_row(cass_iterator_get_row(row_iterator));
		for (size_t column_index = 0; cass_iterator_next(column_iterator); ++column_index)
		{
			HalonHSLValue *k, *v;
			HalonMTA_hsl_value_array_add(row, &k, &v);
			const char* column_name;
			size_t column_name_length;
			error = cass_result_column_name(result, column_index, &column_name, &column_name_length);
			if (error != CASS_OK)
				break;
			HalonMTA_hsl_value_set(k, HALONMTA_HSL_TYPE_STRING, column_name, column_name_length);
			error = cass_value_to_hsl_value(v, cass_iterator_get_column(column_iterator));
			if (error != CASS_OK)
				break;
		}
		cass_iterator_free(column_iterator);
		if (error != CASS_OK)
			break;
	}
	cass_iterator_free(row_iterator);
	return error;
}

void Cassandra_statement_execute(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassFuture* result_future = cass_session_execute(ptr->profile->session, ptr->statement);
	if (cass_future_error_code(result_future) != CASS_OK)
	{
		throw_cass_future_error(hhc, result_future);
		cass_future_free(result_future);
		return;
	}

	const CassResult* result = cass_future_get_result(result_future);
	CassError error = result_to_hsl_value(ret, result);
	if (error == CASS_OK)
	{
		cass_bool_t has_more_pages = cass_result_has_more_pages(result);
		if (has_more_pages)
			error = cass_statement_set_paging_state(ptr->statement, result);
		if (error == CASS_OK)
			ptr->has_more_pages = (bool)has_more_pages;
	}
	if (error != CASS_OK)
		throw_cass_error(hhc, error);

	cass_result_free(result);
	cass_future_free(result_future);
}

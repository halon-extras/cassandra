#include "execute.hpp"
#include <string>
#include "cassandra/cassandra.hpp"
#include <cassandra.h>

CassError cass_value_to_hsl_value(HalonHSLValue* hsl_value, const CassValue* cass_value)
{
	CassValueType cass_type = cass_data_type_type(cass_value_data_type(cass_value));

	switch (cass_type)
	{
		case CASS_VALUE_TYPE_ASCII:
		{
			const char* column_value;
			size_t column_value_length;
			CassError error = cass_value_get_string(cass_value, &column_value, &column_value_length);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value, column_value_length);
			break;
		}
		case CASS_VALUE_TYPE_TEXT:
		{
			const char* column_value;
			size_t column_value_length;
			CassError error = cass_value_get_string(cass_value, &column_value, &column_value_length);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value, column_value_length);
			break;
		}
		case CASS_VALUE_TYPE_VARCHAR:
		{
			const char* column_value;
			size_t column_value_length;
			CassError error = cass_value_get_string(cass_value, &column_value, &column_value_length);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value, column_value_length);
			break;
		}
		case CASS_VALUE_TYPE_BIGINT:
		{
			cass_int64_t column_value;
			CassError error = cass_value_get_int64(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				std::string column_value_hsl = std::to_string(column_value);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value_hsl.c_str(), 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_COUNTER:
		{
			cass_int64_t column_value;
			CassError error = cass_value_get_int64(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				std::string column_value_hsl = std::to_string(column_value);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value_hsl.c_str(), 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_INT:
		{
			cass_int32_t column_value;
			CassError error = cass_value_get_int32(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				double column_value_hsl = (double)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_SMALL_INT:
		{
			cass_int16_t column_value;
			CassError error = cass_value_get_int16(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				double column_value_hsl = (double)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_TINY_INT:
		{
			cass_int8_t column_value;
			CassError error = cass_value_get_int8(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				double column_value_hsl = (double)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_FLOAT:
		{
			cass_float_t column_value;
			CassError error = cass_value_get_float(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				double column_value_hsl = (double)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_DOUBLE:
		{
			cass_double_t column_value;
			CassError error = cass_value_get_double(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				double column_value_hsl = (double)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_INET:
		{
			CassInet column_value;
			CassError error = cass_value_get_inet(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				char column_value_hsl[CASS_INET_STRING_LENGTH];
				cass_inet_string(column_value, column_value_hsl);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_UUID:
		{
			CassUuid column_value;
			CassError error = cass_value_get_uuid(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				char column_value_hsl[CASS_UUID_STRING_LENGTH];
				cass_uuid_string(column_value, column_value_hsl);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_TIMEUUID:
		{
			CassUuid column_value;
			CassError error = cass_value_get_uuid(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				char column_value_hsl[CASS_UUID_STRING_LENGTH];
				cass_uuid_string(column_value, column_value_hsl);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_BOOLEAN:
		{
			cass_bool_t column_value;
			CassError error = cass_value_get_bool(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				bool column_value_hsl = (bool)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_BOOLEAN, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_TIMESTAMP:
		{
			cass_int64_t column_value;
			CassError error = cass_value_get_int64(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				std::string column_value_hsl = std::to_string(column_value);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value_hsl.c_str(), 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_DATE:
		{
			cass_uint32_t column_value;
			CassError error = cass_value_get_uint32(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				double column_value_hsl = (double)column_value;
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NUMBER, &column_value_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_TIME:
		{
			cass_int64_t column_value;
			CassError error = cass_value_get_int64(cass_value, &column_value);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				std::string column_value_hsl = std::to_string(column_value);
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value_hsl.c_str(), 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_DURATION:
		{
			cass_int32_t months, days;
			cass_int64_t nanos;
			CassError error = cass_value_get_duration(cass_value, &months, &days, &nanos);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
				HalonHSLValue *k1, *v1;
				HalonMTA_hsl_value_array_add(hsl_value, &k1, &v1);
				double months_index = 0;
				HalonMTA_hsl_value_set(k1, HALONMTA_HSL_TYPE_NUMBER, &months_index, 0);
				double months_hsl = (double)months;
				HalonMTA_hsl_value_set(v1, HALONMTA_HSL_TYPE_NUMBER, &months_hsl, 0);
				HalonHSLValue *k2, *v2;
				HalonMTA_hsl_value_array_add(hsl_value, &k2, &v2);
				double days_index = 1;
				HalonMTA_hsl_value_set(k2, HALONMTA_HSL_TYPE_NUMBER, &days_index, 0);
				double days_hsl = (double)days;
				HalonMTA_hsl_value_set(v2, HALONMTA_HSL_TYPE_NUMBER, &days_hsl, 0);
				HalonHSLValue *k3, *v3;
				HalonMTA_hsl_value_array_add(hsl_value, &k3, &v3);
				double nanos_index = 2;
				HalonMTA_hsl_value_set(k3, HALONMTA_HSL_TYPE_NUMBER, &nanos_index, 0);
				std::string nanos_hsl = std::to_string((long long int)nanos);
				HalonMTA_hsl_value_set(v3, HALONMTA_HSL_TYPE_STRING, nanos_hsl.c_str(), 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_BLOB:
		{
			const cass_byte_t* column_value;
			size_t column_value_length;
			CassError error = cass_value_get_bytes(cass_value, &column_value, &column_value_length);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value, column_value_length);
			break;
		}
		case CASS_VALUE_TYPE_VARINT:
		{
			const cass_byte_t* column_value;
			size_t column_value_length;
			CassError error = cass_value_get_bytes(cass_value, &column_value, &column_value_length);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_STRING, column_value, column_value_length);
			break;
		}
		case CASS_VALUE_TYPE_DECIMAL:
		{
			const cass_byte_t* varint;
			size_t varint_size;
			cass_int32_t scale;
			CassError error = cass_value_get_decimal(cass_value, &varint, &varint_size, &scale);
			if (error == CASS_ERROR_LIB_NULL_VALUE)
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			else if (error != CASS_OK)
				return error;
			else
			{
				HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
				HalonHSLValue *k1, *v1;
				HalonMTA_hsl_value_array_add(hsl_value, &k1, &v1);
				double varint_index = 0;
				HalonMTA_hsl_value_set(k1, HALONMTA_HSL_TYPE_NUMBER, &varint_index, 0);
				HalonMTA_hsl_value_set(v1, HALONMTA_HSL_TYPE_STRING, varint, varint_size);
				HalonHSLValue *k2, *v2;
				HalonMTA_hsl_value_array_add(hsl_value, &k2, &v2);
				double scale_index = 1;
				double scale_hsl = (double)scale;
				HalonMTA_hsl_value_set(k2, HALONMTA_HSL_TYPE_NUMBER, &scale_index, 0);
				HalonMTA_hsl_value_set(v2, HALONMTA_HSL_TYPE_NUMBER, &scale_hsl, 0);
			}
			break;
		}
		case CASS_VALUE_TYPE_MAP:
		{
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
			CassIterator* map_iterator = cass_iterator_from_map(cass_value);
			while (cass_iterator_next(map_iterator))
			{
				HalonHSLValue *k, *v;
				HalonMTA_hsl_value_array_add(hsl_value, &k, &v);
				const CassValue* key = cass_iterator_get_map_key(map_iterator);
				const CassValue* value = cass_iterator_get_map_value(map_iterator);
				CassError error;
				error = cass_value_to_hsl_value(k, key);
				if (error != CASS_OK)
				{
					cass_iterator_free(map_iterator);
					return error;
				}
				error = cass_value_to_hsl_value(v, value);
				if (error != CASS_OK)
				{
					cass_iterator_free(map_iterator);
					return error;
				}
			}
			cass_iterator_free(map_iterator);
			break;
		}
		case CASS_VALUE_TYPE_LIST:
		{
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
			CassIterator* list_iterator = cass_iterator_from_collection(cass_value);
			size_t list_index = 0;
			while (cass_iterator_next(list_iterator))
			{
				double di = (double)list_index;
				HalonHSLValue *k, *v;
				HalonMTA_hsl_value_array_add(hsl_value, &k, &v);
				HalonMTA_hsl_value_set(k, HALONMTA_HSL_TYPE_NUMBER, &di, 0);
				const CassValue* value = cass_iterator_get_value(list_iterator);
				CassError error = cass_value_to_hsl_value(v, value);
				if (error != CASS_OK)
				{
					cass_iterator_free(list_iterator);
					return error;
				}
				list_index++;
			}
			cass_iterator_free(list_iterator);
			break;
		}
		case CASS_VALUE_TYPE_SET:
		{
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
			CassIterator* list_iterator = cass_iterator_from_collection(cass_value);
			size_t list_index = 0;
			while (cass_iterator_next(list_iterator))
			{
				double di = (double)list_index;
				HalonHSLValue *k, *v;
				HalonMTA_hsl_value_array_add(hsl_value, &k, &v);
				HalonMTA_hsl_value_set(k, HALONMTA_HSL_TYPE_NUMBER, &di, 0);
				const CassValue* value = cass_iterator_get_value(list_iterator);
				CassError error = cass_value_to_hsl_value(v, value);
				if (error != CASS_OK)
				{
					cass_iterator_free(list_iterator);
					return error;
				}
				list_index++;
			}
			cass_iterator_free(list_iterator);
			break;
		}
		case CASS_VALUE_TYPE_TUPLE:
		{
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
			CassIterator* tuple_iterator = cass_iterator_from_tuple(cass_value);
			size_t tuple_index = 0;
			while (cass_iterator_next(tuple_iterator))
			{
				double di = (double)tuple_index;
				HalonHSLValue *k, *v;
				HalonMTA_hsl_value_array_add(hsl_value, &k, &v);
				HalonMTA_hsl_value_set(k, HALONMTA_HSL_TYPE_NUMBER, &di, 0);
				const CassValue* value = cass_iterator_get_value(tuple_iterator);
				CassError error = cass_value_to_hsl_value(v, value);
				if (error != CASS_OK)
				{
					cass_iterator_free(tuple_iterator);
					return error;
				}
				tuple_index++;
			}
			cass_iterator_free(tuple_iterator);
			break;
		}
		case CASS_VALUE_TYPE_CUSTOM:
		case CASS_VALUE_TYPE_UDT:
		case CASS_VALUE_TYPE_LAST_ENTRY:
		case CASS_VALUE_TYPE_UNKNOWN:
		{
			// Not supported
			HalonMTA_hsl_value_set(hsl_value, HALONMTA_HSL_TYPE_NONE, nullptr, 0);
			break;
		}
	}

	return CASS_OK;
}

void Cassandra_statement_execute(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassFuture* result_future = cass_session_execute(ptr->profile->session, ptr->statement);

	if (cass_future_error_code(result_future) == CASS_OK)
	{
		HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);

		const CassResult* result = cass_future_get_result(result_future);
		CassIterator* row_iterator = cass_iterator_from_result(result);
		size_t row_index = 0;
		while (cass_iterator_next(row_iterator))
		{
			double di = (double)row_index;
			HalonHSLValue *k1, *v1;
			HalonMTA_hsl_value_array_add(ret, &k1, &v1);
			HalonMTA_hsl_value_set(k1, HALONMTA_HSL_TYPE_NUMBER, &di, 0);
			HalonMTA_hsl_value_set(v1, HALONMTA_HSL_TYPE_ARRAY, nullptr, 0);
			const CassRow* row = cass_iterator_get_row(row_iterator);
			CassIterator* column_iterator = cass_iterator_from_row(row);
			size_t column_index = 0;
			while (cass_iterator_next(column_iterator))
			{
				CassError error;

				HalonHSLValue *k2, *v2;
				HalonMTA_hsl_value_array_add(v1, &k2, &v2);

				const char* column_name;
				size_t column_name_length;
				error = cass_result_column_name(result, column_index, &column_name, &column_name_length);
				if (error != CASS_OK)
				{
					HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
					HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
					cass_iterator_free(column_iterator);
					cass_iterator_free(row_iterator);
					cass_result_free(result);
					cass_future_free(result_future);
					return;
				}

				HalonMTA_hsl_value_set(k2, HALONMTA_HSL_TYPE_STRING, column_name, column_name_length);

				const CassValue* value = cass_iterator_get_column(column_iterator);
				error = cass_value_to_hsl_value(v2, value);
				if (error != CASS_OK)
				{
					HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
					HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
					cass_iterator_free(column_iterator);
					cass_iterator_free(row_iterator);
					cass_result_free(result);
					cass_future_free(result_future);
					return;
				}
				column_index++;
			}
			cass_iterator_free(column_iterator);
			row_index++;
		}
		cass_iterator_free(row_iterator);

		cass_bool_t has_more_pages = cass_result_has_more_pages(result);
		if (has_more_pages)
		{
			CassError error = cass_statement_set_paging_state(ptr->statement, result);
			if (error != CASS_OK)
			{
				HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
				HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
				cass_result_free(result);
				cass_future_free(result_future);
				return;
			}
		}
		ptr->has_more_pages = (bool)has_more_pages;

		cass_result_free(result);
	}
	else
	{
		const char* message;
		size_t message_length;
		cass_future_error_message(result_future, &message, &message_length);
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, message, message_length);
		cass_future_free(result_future);
		return;
	}

	cass_future_free(result_future);
}

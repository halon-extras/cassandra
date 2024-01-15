#ifndef CASSANDRA_STATEMENT_BIND_HPP_
#define CASSANDRA_STATEMENT_BIND_HPP_

#include <HalonMTA.h>

void Cassandra_statement_bind_bool(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_bytes(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_double(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_float(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_int8(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_int16(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_int32(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_int64(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_uint32(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_decimal(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_inet(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_null(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_string(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_uuid(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_bind_duration(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
// void Cassandra_statement_bind_tuple(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
// void Cassandra_statement_bind_collection(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
// void Cassandra_statement_bind_user_type(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
// void Cassandra_statement_bind_custom(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);

#endif

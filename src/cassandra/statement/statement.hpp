#ifndef CASSANDRA_STATEMENT_HPP_
#define CASSANDRA_STATEMENT_HPP_

#include <HalonMTA.h>

void Cassandra_statement(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_free(void* ptr);

#endif

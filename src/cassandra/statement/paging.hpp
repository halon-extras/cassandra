#ifndef CASSANDRA_STATEMENT_PAGING_HPP_
#define CASSANDRA_STATEMENT_PAGING_HPP_

#include <HalonMTA.h>

void Cassandra_statement_paging_size(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_statement_has_more_pages(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);

#endif

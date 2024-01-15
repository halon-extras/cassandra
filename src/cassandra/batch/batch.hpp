#ifndef CASSANDRA_BATCH_HPP_
#define CASSANDRA_BATCH_HPP_

#include <HalonMTA.h>

void Cassandra_batch(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret);
void Cassandra_batch_free(void* ptr);

#endif

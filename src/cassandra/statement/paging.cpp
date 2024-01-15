#include "paging.hpp"
#include <string>
#include "cassandra/cassandra.hpp"
#include "argument.hpp"
#include <cassandra.h>

void Cassandra_statement_paging_size(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	double paging_size = 0;
	if (!parse_hsl_argument_as_double(hhc, args, 0, true, paging_size))
		return;
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	CassError error = cass_statement_set_paging_size(ptr->statement, (int)paging_size);
	if (error != CASS_OK)
	{
		HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
		HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, cass_error_desc(error), 0);
		return;
	}
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_THIS, hhc, 0);
}

void Cassandra_statement_has_more_pages(HalonHSLContext* hhc, HalonHSLArguments* args, HalonHSLValue* ret)
{
	Cassandra* ptr = (Cassandra*)HalonMTA_hsl_object_ptr_get(hhc);
	bool has_more_pages = ptr->has_more_pages;
	HalonMTA_hsl_value_set(ret, HALONMTA_HSL_TYPE_BOOLEAN, &has_more_pages, 0);
}

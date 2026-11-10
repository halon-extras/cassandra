#include "argument.hpp"

void throw_hsl_exception(HalonHSLContext* hhc, const char* message, size_t message_length)
{
	HalonHSLValue* e = HalonMTA_hsl_throw(hhc);
	HalonMTA_hsl_value_set(e, HALONMTA_HSL_TYPE_EXCEPTION, message, message_length);
}

// On success arg is the argument, or nullptr if an optional argument is missing
static bool get_hsl_argument(HalonHSLContext* hhc, HalonHSLArguments* args, size_t index, bool required, int type, const char* type_error, HalonHSLValue*& arg)
{
	arg = HalonMTA_hsl_argument_get(args, index);
	if (!arg)
	{
		if (required)
		{
			throw_hsl_exception(hhc, "missing required argument");
			return false;
		}
		return true;
	}
	if (HalonMTA_hsl_value_type(arg) != type)
	{
		throw_hsl_exception(hhc, type_error);
		return false;
	}
	return true;
}

bool parse_hsl_argument_as_bool(HalonHSLContext* hhc, HalonHSLArguments* args, size_t index, bool required, bool& value)
{
	HalonHSLValue* arg;
	if (!get_hsl_argument(hhc, args, index, required, HALONMTA_HSL_TYPE_BOOLEAN, "argument is not a boolean", arg))
		return false;
	if (arg)
		HalonMTA_hsl_value_get(arg, HALONMTA_HSL_TYPE_BOOLEAN, &value, nullptr);
	return true;
}

bool parse_hsl_argument_as_double(HalonHSLContext* hhc, HalonHSLArguments* args, size_t index, bool required, double& value)
{
	HalonHSLValue* arg;
	if (!get_hsl_argument(hhc, args, index, required, HALONMTA_HSL_TYPE_NUMBER, "argument is not a number", arg))
		return false;
	if (arg)
		HalonMTA_hsl_value_get(arg, HALONMTA_HSL_TYPE_NUMBER, &value, nullptr);
	return true;
}

bool parse_hsl_argument_as_string(HalonHSLContext* hhc, HalonHSLArguments* args, size_t index, bool required, std::string& value)
{
	HalonHSLValue* arg;
	if (!get_hsl_argument(hhc, args, index, required, HALONMTA_HSL_TYPE_STRING, "argument is not a string", arg))
		return false;
	if (arg)
	{
		const char* _value;
		size_t _value_length;
		HalonMTA_hsl_value_get(arg, HALONMTA_HSL_TYPE_STRING, &_value, &_value_length);
		value.assign(_value, _value_length);
	}
	return true;
}

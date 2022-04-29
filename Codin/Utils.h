#pragma once

/// Mark that given parameter is not used at all in the function.
#define UNUSED(x) (void)(x)
/// Mark that given parameter is not used in given code path.
#define TOUCH(x) (void)(x)

// https://stackoverflow.com/questions/1082192/how-to-generate-random-variable-names-in-c-using-macros
// One level of macro indirection is required in order to resolve __COUNTER__,
// and get varname1 instead of varname__COUNTER__.
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)

#pragma once

#ifdef _MSC_VER
#    define GRACE_COMPAT_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else // _MSC_VER
#    define GRACE_COMPAT_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif // _MSC_VER

#ifndef WARNINGS_HPP
#define WARNINGS_HPP

#if defined(_MSC_VER)
#define DISABLE_WARNING_PUSH __pragma(warning(push))
#define DISABLE_WARNING_POP __pragma(warning(pop))
#define DISABLE_WARNING(warningNumber) __pragma(warning(disable : warningNumber))

#define DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE(4266)

#elif defined(__GNUC__) || defined(__clang__)
#define DO_PRAGMA(X) _Pragma(#X)
#define DISABLE_WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
#define DISABLE_WARNING_POP DO_PRAGMA(GCC diagnostic pop)
#define DISABLE_WARNING(warningName) DO_PRAGMA(GCC diagnostic ignored #warningName)

#define DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE DISABLE_WARNING(-Winconsistent-missing-override)
#else
#define DISABLE_WARNING_PUSH
#define DISABLE_WARNING_POP
#define DISABLE_WARNING_INCONSISTENT_MISSING_OVERRIDE
#endif

#endif  // WARNINGS_HPP

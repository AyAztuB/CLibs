#ifndef __AYAZTUB__CORE_UTILS__UTIL_ATTRIBUTES__H__
#define __AYAZTUB__CORE_UTILS__UTIL_ATTRIBUTES__H__

#ifndef __GNUC__
#define __attribute__(x)
#endif // __GNUC__

#define ACCESS(mode, ref_idx) __attribute__((access(mode, ref_idx)))

#define ALIAS(target) __attribute__((alias(#target)))

#define ALIGNED __attribute__((aligned))

#define ALWAYS_INLINE __attribute__((always_inline))

#define CONST __attribute__((const))

#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR __attribute__((destructor))

#define DEPRECATED __attribute__((deprecated))
#define DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))

#define ERROR(msg) __attribute__((error(msg)))
#define WARNING(msg) __attribute__((warning(msg)))

#define FD_ARG(n) __attribute__((fd_arg(n)))
#define FD_ARG_READ(n) __attribute__((fd_arg_read(n)))
#define FD_ARG_WRITE(n) __attribute__((fd_arg_write(n)))

#define FORMAT(archetype, string_index, first_to_check) __attribute__((format(archetype, string_index, first_to_check)))
#define FORMAT_ARG(string_index) __attribute__((format_arg(string_index)))

#define MALLOC(deallocator) __attribute__((malloc(deallocator)))

#define NONNULL __attribute__((nonnull))
#define NONNULL_POSITIONS(...) __attribute__((nonnull(__VA_ARGS__)))

#define NORETURN __attribute__((noreturn))
#define NOTHROW __attribute__((nothrow))

#define NULL_TERMINATED_STRING_ARG(n) __attribute__((null_terminated_string_arg(n)))

#define PURE __attribute__((pure))

#define RETURN_NONNULL __attribute__((return_nonnull))

#define SENTINEL __attribute__((sentinel))

#define UNAVAILABLE __attribute__((unavailable))
#define UNAVAILABLE_MSG(msg) __attribute__((unavailable(msg)))

#define UNUSED __attribute__((unused))
#define USED __attribute__((used))

#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#ifdef __GNUC__
#define UNREACHABLE __builtin_unreachable()
#else // __GNUC__
#define UNREACHABLE (void)0
#endif // __GNUC__

#endif // __AYAZTUB__CORE_UTILS__UTIL_ATTRIBUTES__H__

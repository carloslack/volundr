#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

// TODO: add __PRETTY_FUNCTION__ to output

#define log_info(a...)       ({ _log_info(__FILE__, __LINE__, a); })
#define log_debug(a...)      ({ _log_debug(__FILE__, __LINE__, a); })
#define log_error(a...)      ({ _log_error(__FILE__, __LINE__, a); })
#define log_warning(a...)    ({ _log_warning(__FILE__, __LINE__, a); })
#define log_fatal(a...)      ({ _log_fatal(__FILE__, __LINE__, a); })
#define log_info_buff(a...)  ({ _log_info_buff(__FILE__, __LINE__, a); })
#define log_debug_buff(a...) ({ _log_debug_buff(__FILE__, __LINE__, a); })

#define log_fatal(a...) ({ _log_fatal(__FILE__, __LINE__, a); })

i32 _log_info           (const char*, int, const sbyte *, ...);
i32 _log_debug          (const char*, int, const sbyte *, ...);
i32 _log_error          (const char*, int, const sbyte *, ...);
i32 _log_warning        (const char*, int, const sbyte *, ...);
i32 _log_info_buff      (const char*, int, const sbyte *, ...);
i32 _log_debug_buff     (const char*, int, const sbyte *, ...);

i32 logf_info           (FILE *f, const sbyte *fmt, ...);
i32 logf_debug          (FILE *f, const sbyte *fmt, ...);


// TODO: add __attribute__ noreturn with macros
i32 _log_fatal          (const char*, int, const sbyte *, ...);

#endif

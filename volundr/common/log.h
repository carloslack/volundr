#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <stdlib.h>


// TODO: add __PRETTY_FUNCTION__ to output

#define log_it(a...)         ({ _log_it(__FILE__, __LINE__, a); })
#define log_info(a...)       ({ _log_info(__FILE__, __LINE__, a); })
#define log_debug(a...)      ({ _log_debug(__FILE__, __LINE__, a); })
#define log_error(a...)      ({ _log_error(__FILE__, __LINE__, a); })
#define log_warning(a...)    ({ _log_warning(__FILE__, __LINE__, a); })
#define log_fatal(a...)      ({ _log_fatal(__FILE__, __LINE__, a); })
#define log_info_buff(a...)  ({ _log_info_buff(__FILE__, __LINE__, a); })
#define log_debug_buff(a...) ({ _log_debug_buff(__FILE__, __LINE__, a); })

#define log_fatal(a...) ({ _log_fatal(__FILE__, __LINE__, a); })

int32_t _log_it             (const char*, int, const char *, ...);
int32_t _log_info           (const char*, int, const char *, ...);
int32_t _log_debug          (const char*, int, const char *, ...);
int32_t _log_error          (const char*, int, const char *, ...);
int32_t _log_warning        (const char*, int, const char *, ...);
int32_t _log_info_buff      (const char*, int, const char *, ...);
int32_t _log_debug_buff     (const char*, int, const char *, ...);

int32_t logf_it             (FILE *f, const char *, ...);
int32_t logf_info           (FILE *f, const char *fmt, ...);
int32_t logf_debug          (FILE *f, const char *fmt, ...);


// TODO: add __attribute__ noreturn with macros
void _log_fatal          (const char*, int, const char *, ...);

#endif

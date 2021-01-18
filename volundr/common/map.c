/**
 *    @file  map.c
 *   @brief  Responsible for file memory mapping.
 *
 *   TODO: improve it all
 *   TODO: testings
 *
 */
#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"
#include "map.h"
#include "asm.h"
/**< x = offset to be aligned for mmap pages */


/**
 * @defgroup map Map
 * @ingroup common
 * This set of functions provides mechanisms to create an ELF image on memory
 */

/************ INTERFACES ************/
/**
 * @defgroup interfaces
 * @ingroup map
 * @addtogroup interfaces Interfaces
 * @{
 */


/**
 * @brief Copies a file from given file descritor to
 * memory from start address (usually (void*)0) to size.
 *
 * @param start The start address for the mapping
 * @param size The size in bytes being requested for mapping
 * @param fd File descriptor
 * @param rv Memory address of mapped area
 * @see file_open
 * @see asm.h
 *
 * @return A valid mapping address.
 */
bool map_filemap(void* start, size_t size, int fd, void **rv, open_mode_t m) {
    void* ret = NULL;
    if((ret = asm_mmap(NULL
                    , size
                    /** Play smart and mmap accordingly */
                    , m == F_RW ? PROT_WRITE : PROT_READ
                    , m == F_RW ? MAP_SHARED : MAP_PRIVATE  // MAP_SHARED to sync with fd
                    , fd
                    , (off_t)ALIGNOFFSET(fd))) == MAP_FAILED)
    {
        asm_close(fd);
        log_fatal("mmap %s", strerror(errno));
    }

    *rv = ret;
    log_debug("file mapped to %p", *rv);

    return true;
}

/**
 * @brief Copies src of size n to memory mapped segment.
 * @param mapaddr Address of mapped memory
 * @param src Source address
 * @param len Size in bytes to be written
 * @param rv Memory address of mapped area
 *
 * @return Whether operation was successful
 */
bool map_write(void *mapaddr, const void *src, size_t len, void **rv) {
    bool status = false;
    if(mapaddr != NULL) {
        *rv = memmove(mapaddr, src, len);
        status = true;
    }
    else
        log_error("invalid mapping address");

    return status;
}

/**
 * @brief Changes mapping protection from given mapping
 * address of size len.
 * @param mapaddr Address of mapped memory
 * @param len Size in bytes of memory area
 * @param prot Protection to set
 * @see asm.h
 *
 * @return Whether operation was successful
 */
i32 map_mprotect(void* mapaddr, size_t len, i32 prot) {
    i32 ret = -1;

    if(mapaddr != NULL) {
        if((ret = asm_mprotect(mapaddr, len, prot)) == -1) {
            log_debug("mprotect %s", strerror(errno));
        }
    } else {
        log_error("invalid mapping address");
    }

    return ret;
}

/**
 * @brief Synchronizes memory mapped region with its file on disk.
 * @param mapaddr Address of mapped memory
 * @param len Size in bytes of memory area
 *
 * @return Whether operation was successful
 */
i32 map_sync(void* mapaddr, off_t len) {
    i32 ret = -1;

    if(mapaddr != NULL) {
        if((ret = msync(mapaddr
                        , len
                        , MS_SYNC) == -1))
        {
            log_error("msync: %s", strerror(errno));
            return ret;
        }
    }else{
        log_error("invalid mapping address");
    }

    return ret;
}


/**
 * @brief Unmaps memory segment from given address to given size
 * @param mapaddr Address of mapped memory
 * @param len Size in bytes of memory area
 * @see asm.h
 *
 * @return Whether operation was successful
 */
i32 map_fileunmap(void *mapaddr, off_t len) {
    i32 ret = -1;

    if(mapaddr != NULL) {
        if((ret = asm_munmap(mapaddr, len)) == -1) {
            log_error("munmap: %s", strerror(errno));
        } else {
            log_debug("file unmaped from %p", mapaddr);
        }
    }else{
        log_error("invalid mapping address");
    }

    return ret;
}
/** @} */

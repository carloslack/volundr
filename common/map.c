/**
 *    @file  map.c
 *   @brief  Responsible for file memory mapping.
 *
 *   TODO: improve it all
 *   TODO: testings
 *
 */
#include "map.h"
#include "asm.h"

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
 * This function copies a file to memory
 *
 * @brief Copies a file from given file descritor to
 * memory from start address (usually (void*)0) to size.
 *
 * @return A valid mapping address.
 */
void* map_filemap(void* start, off_t size, i32 fd) {
    void* ret = NULL;

    if((ret = mmap(start
                    , size
                    , PROT_READ
                    , MAP_SHARED
                    , fd
                    , (off_t)ALIGNOFFSET(fd))) == MAP_FAILED)
    {
        asm_close(fd);
        log_fatal("mmap %s", strerror(errno));
    }

    log_debug("file mapped to %p", ret); 

    return ret;
}

/**
 * This function write data on memory mapped segment
 *
 * @brief Copies src of size n to memory mapped segment.
 *
 * @return A valid mapping address.
 */
void* map_write(void* mapaddr, const void* src, size_t n) {
    void* ret = NULL;

    if(mapaddr != NULL) 
        ret = memmove(mapaddr, src, n);	    
    else
        log_error("invalid mapping address");

    return ret;
}

/**
 * This function chages mapping protection
 *
 * @brief Changes mapping protection from given mapping
 * address os size len.
 *
 * @return success or error
 */
i32 map_mprotect(void* mapaddr, size_t len, i32 prot) {
    i32 ret = -1;

    if(mapaddr != NULL) {
        if((ret = mprotect(mapaddr, len, prot)) == -1) {
            log_debug("mprotect %s", strerror(errno));
        }
    } else {
        log_error("invalid mapping address");
    }

    return ret;
}

/**
 * This function synchronizes memory to its file reference on disk.
 *
 * @brief Synchronizes memory mapped region with its file on disk.
 *
 * @return success or error
 */
i32 map_sync(void* mapaddr, off_t size) {
    i32 ret = -1;

    if(mapaddr != NULL) {
        if((ret = msync(mapaddr
                        , size
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
 * This function unmaps memory segment
 *
 * @brief Unmaps memory segment from given address to given size
 *
 * @return success or error
 */
i32 map_fileunmap(void* mapaddr, off_t size) {
    i32 ret = -1;

    if(mapaddr != NULL) {
        if((ret = munmap(mapaddr, size)) == -1) {
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

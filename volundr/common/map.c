/**
 *  MIT License
 *
 *  Copyright (c) 2021 Carlos Carvalho
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
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
    void *ret = asm_mmap(start
            , PALIGN_UP(size)
            /** Play smart and mmap accordingly */
            , m == F_RW ? PROT_WRITE : PROT_READ
            , m == F_RW ? MAP_SHARED : MAP_PRIVATE  // MAP_SHARED to sync with fd
            , fd
            , 0);

    if (MAP_FAILED == ret) {
        asm_close(fd);
        log_error("map_filemap failed\n");
        return false;
    }

    *rv = ret;
    log_debug("file mapped to %p\n", ret);

    return true;
}

/**
 * @brief make sure changes are synced to disk
 *
 * @param start The start address for the mapping
 * @see asm.h
 *
 * @return true for success, false for error
 */
bool map_filesync(void* start, size_t size) {
    int ret = asm_msync(start, PALIGN_UP(size), MS_SYNC);
    if (0 != ret) {
        log_error("map_filesync failed\n");
        return false;
    }

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
int32_t map_mprotect(void* mapaddr, size_t len, int32_t prot) {
    int32_t ret = -1;

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
int32_t map_sync(void* mapaddr, off_t len) {
    int32_t ret = -1;

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
int32_t map_fileunmap(void *mapaddr, off_t len) {
    int32_t ret = -1;

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

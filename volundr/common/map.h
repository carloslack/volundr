#ifndef MAP_H
#define MAP_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/**
 *    @file  map.h
 *   @brief  provides mechanisms to create an ELF image on memory (definition module).
 */

/** Copies a file from given file descritor to memory
 * from start address (usually (void*)0) to size
 */
bool map_filemap   (void* start, size_t size, int fd, void **);

/** Copies src of size n to memory mapped segment */
bool map_write     (void* mapaddr, const void* src, size_t n, void **);

/** Changes mapping protection from given mapping
 * address os size len
 */
i32 map_mprotect    (void* mapaddr, size_t len, i32 prot);

/** Synchronizes memory mapped region with its file on disk */
i32 map_sync     	(void* mapaddr, off_t size);

/** Unmaps memory segment from given address to given size */
i32 map_fileunmap   (void* mapaddr, off_t size);

#endif

#ifndef MAP_H
#define MAP_H

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define PALIGN(x) (x & ~(PAGE_SIZE - 1))
#define PALIGN_UP(x) (PALIGN(x) + PAGE_SIZE)
/**
 *    @file  map.h
 *   @brief  provides mechanisms to create an ELF image on memory (definition module).
 */

/** Copies a file from given file descritor to memory
 * from start address (usually (void*)0) to size
 */
bool map_filemap   (void* start, size_t size, int fd, void **, open_mode_t);

/** Copies src of size n to memory mapped segment */
bool map_write     (void* mapaddr, const void* src, size_t n, void **);

/** Changes mapping protection from given mapping
 * address os size len
 */
int32_t map_mprotect    (void* mapaddr, size_t len, int32_t prot);

/** Synchronizes memory mapped region with its file on disk */
int32_t map_sync     	(void* mapaddr, off_t size);

/** Unmaps memory segment from given address to given size */
int32_t map_fileunmap   (void* mapaddr, off_t size);

bool map_filesync(void* start, size_t size);
#endif

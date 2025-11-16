/**
 * @file memory_ops.h
 * @brief High-level memory operation interface for the memory simulator.
 *
 * This module exposes the main functions used to allocate, reallocate,
 * and free named memory blocks within the simulated memory environment.
 * 
 * Each operation interacts with the internal allocator, block manager,
 * and variable table to ensure consistent state tracking and algorithm
 * selection (First-Fit, Best-Fit, Worst-Fit).
 */

#ifndef MEMORY_OPS_H
#define MEMORY_OPS_H

#include <stddef.h>

/**
 * @brief Allocates a memory block in the simulated memory and associates it with a variable name.
 *
 * The allocation strategy used (First-fit, Best-fit, or Worst-fit) is determined
 * by the global configuration of the simulator. The allocated block is filled 
 * with the variable name for visualization and debugging purposes.
 *
 * @param name Name of the variable to associate with the allocated block.
 * @param size Size in bytes to allocate.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int mem_alloc(const char *name, size_t size);

/**
 * @brief Frees a previously allocated block associated with the given variable name.
 *
 * The block is marked as free in the simulated memory, and all internal
 * tracking structures are updated accordingly. Attempting to free an
 * unknown or already freed variable results in an error.
 *
 * @param name Name of the variable whose memory block should be freed.
 * @return int Returns 0 on success, or a negative error code if the variable was not found.
 */
int mem_free(const char *name);

/**
 * @brief Changes the size of a previously allocated block in the simulated memory.
 *
 * The memory block bound to the given variable name is reallocated
 * to the new size. Depending on available space and algorithm selection,
 * the block may move to a new location. In such cases, the old block is
 * freed and the new block is filled with the variable name.
 *
 * @param name Name of the variable to reallocate.
 * @param new_size New size in bytes for the block.
 * @return int Returns 0 on success, or a negative error code on failure.
 */
int mem_realloc(const char *name, size_t new_size); 

#endif /* MEMORY_OPS_H */

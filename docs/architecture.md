# Architecture

This project demonstrates a minimal kernel-to-user memory mapping flow using a character device and the `mmap()` system call.

## Components

### 1. Kernel module
The driver in `driver/mmap.c` performs the following tasks:

- allocates a character device number
- creates a `struct cdev` object
- allocates a kernel buffer using `kmalloc()`
- registers file operations for `open`, `release`, and `mmap`
- creates a `/dev/mmap_device` node

The most important callback is `mmap_mmap()`, which converts the kernel buffer to a user-visible memory mapping by using `remap_pfn_range()`.

### 2. User-space application
The application in `userspace/mmap_app.c` opens `/dev/mmap_device`, calls `mmap()`, and then reads/writes the mapped memory.

This demonstrates the common pattern:

- kernel owns the backing memory
- user process sees a mapped virtual address
- reads and writes are performed on the shared memory region

## Data flow

1. The kernel module initializes and allocates a buffer.
2. The user app opens the device.
3. The app calls `mmap()` on the file descriptor.
4. The driver maps the kernel memory into user space.
5. The app reads or writes the mapping and can observe the data from the shared memory.

## Why this is useful

This setup forms the basis for many Linux subsystems where memory is shared between kernel and user space without copying data for every read or write.

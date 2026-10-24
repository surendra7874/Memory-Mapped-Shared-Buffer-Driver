# Memory-Mapped-Shared-Buffer-Driver
A Linux character device driver demonstrating efficient user-space and kernel-space communication using memory mapping (mmap()). The project implements a shared memory buffer that can be accessed by both a kernel module and a user-space application, reducing the need for repeated data copying through conventional read() and write() system calls.

**Key Features**

Custom Linux Character Driver: Implements a character device interface for user-space communication.

Shared Memory Mapping: Uses the driver's mmap() file operation to map kernel-allocated memory into user space.
Kernel Memory Management: Demonstrates kernel memory allocation and mapping using APIs such as vmalloc_user() and remap_vmalloc_range().
User-Space Application: Interacts with the driver through a memory-mapped buffer.
IOCTL Interface: Supports control operations between the user-space application and the kernel module
Memory Access and Synchronization: Explores buffer access, mapping permissions, memory lifetime, and safe communication between user space and kernel space.
Out-of-Tree Module Build: Supports building the driver independently of the Linux kernel source tree.

**Technologies Used**

Linux Kernel Programming
C Programming
Linux Character Device Drivers
Memory Management
mmap() System Call
ioctl() Interface
Kernel Module Development
Linux System Calls
BeagleBone Black

**Learning Objectives**

Understand the relationship between virtual memory, physical memory, and page tables.
Learn how kernel memory can be mapped into user space.
Explore the implementation of the mmap() file operation in a Linux character driver.
Understand the differences between memory-mapped communication and conventional read()/write() operations.
Develop practical experience with kernel module compilation, deployment, loading, and debugging on embedded Linux hardware.

**Target Platform**

Developed for embedded Linux systems, with testing intended for the BeagleBone Black running a Linux 4.19-based kernel.

**Project Goal**

Demonstrate practical knowledge of Linux kernel memory management, character device drivers, and efficient user-space/kernel-space communication through shared memory.

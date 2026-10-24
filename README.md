# Memory-Mapped-Shared-Buffer-Driver
A Linux character device driver demonstrating efficient user-space and kernel-space communication using memory mapping (mmap()). The project implements a shared memory buffer that can be accessed by both a kernel module and a user-space application, reducing the need for repeated data copying through conventional read() and write() system calls.

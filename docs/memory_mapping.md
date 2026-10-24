# Memory Mapping Design

The driver exposes a kernel buffer to user space using a page-mapped virtual memory region.

## Kernel-side allocation

The driver creates a buffer at module initialization time:

```c
mmap_dev->buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
```

This buffer lives in kernel memory, so the user process cannot access it directly.

## Mapping to user space

When the user app calls `mmap()`, the driver handles the callback:

```c
static int mmap_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmap_device *dev = file->private_data;
    unsigned long pfn = virt_to_phys(dev->buffer) >> PAGE_SHIFT;
    unsigned long size = vma->vm_end - vma->vm_start;

    return remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
}
```

This maps the physical pages backing the kernel buffer into the process address space.

## Important concept

`virt_to_phys()` converts the kernel virtual address to a physical address. The page frame number is then passed to `remap_pfn_range()`, which creates the user-visible mapping.

## Memory characteristics

- the backing memory is allocated by the kernel
- the mapping is shared between the process and the device driver
- page protections are inherited from the VMA
- the application must use `mmap()` with an appropriate access mode like `PROT_READ | PROT_WRITE`

This is a minimal but realistic example of how kernel drivers expose shared memory to user space.

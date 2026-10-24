#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>

/*
 * mmap.c - Simple character device driver with mmap support.
 *
 * This module creates a character device and exposes a kernel buffer to user
 * space through the mmap() file operation. It serves as a minimal example of
 * how a Linux driver can map memory for user access.
 */

#define DEVICE_NAME "mmap_device"
#define BUFFER_SIZE 4096

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
#define MMAP_CLASS_CREATE(name) class_create(name)
#else
#define MMAP_CLASS_CREATE(name) class_create(THIS_MODULE, name)
#endif

static int mmap_open(struct inode *inode, struct file *file);
static int mmap_release(struct inode *inode, struct file *file);
static int mmap_mmap(struct file *file, struct vm_area_struct *vma);

struct mmap_device {
    char *buffer;
    struct cdev cdev;
    dev_t devt;
};

struct file_operations mmap_fops = {
    .owner = THIS_MODULE,
    .open = mmap_open,
    .release = mmap_release,
    .mmap = mmap_mmap,
};

struct mmap_device *mmap_dev;
struct class *mmap_class;
struct device *mmap_device;

/*
 * mmap_open - Open callback for the device.
 * @inode: inode for the device.
 * @file: file object being opened.
 *
 * Return: 0 on success.
 */
static int mmap_open(struct inode *inode, struct file *file)
{
    file->private_data = mmap_dev;
    return 0;
}

/*
 * mmap_release - Release callback for the device.
 * @inode: inode for the device.
 * @file: file object being released.
 *
 * Return: 0 on success.
 */
static int mmap_release(struct inode *inode, struct file *file)
{
    return 0;
}

/*
 * mmap_mmap - mmap callback for the device.
 * @file: file object being mapped.
 * @vma: virtual memory area structure.
 *
 * Maps the kernel buffer to user space using remap_pfn_range().
 *
 * Return: 0 on success, or a negative error code on failure.
 */
static int mmap_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmap_device *dev = file->private_data;
    unsigned long pfn = virt_to_phys(dev->buffer) >> PAGE_SHIFT;
    unsigned long size = vma->vm_end - vma->vm_start;

    return remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
}

/*
 * mmap_init - Module initialization function.
 *
 * Allocates a device number, initializes the character device, creates the
 * sysfs class and device node, and prepares the driver for user access.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
static int __init mmap_driver_init(void)
{
    int ret;
    dev_t devt;

    // Allocate device number
    ret = alloc_chrdev_region(&devt, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate device number\n");
        return ret;
    }

    // Allocate and initialize the mmap_device structure
    mmap_dev = kzalloc(sizeof(struct mmap_device), GFP_KERNEL);
    if (!mmap_dev) {
        unregister_chrdev_region(devt, 1);
        return -ENOMEM;
    }
    mmap_dev->devt = devt;

    // Allocate buffer for mmap
    mmap_dev->buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!mmap_dev->buffer) {
        kfree(mmap_dev);
        unregister_chrdev_region(devt, 1);
        return -ENOMEM;
    }

    // Initialize the character device
    cdev_init(&mmap_dev->cdev, &mmap_fops);
    mmap_dev->cdev.owner = THIS_MODULE;

    // Add the character device to the system
    ret = cdev_add(&mmap_dev->cdev, devt, 1);
    if (ret < 0) {
        kfree(mmap_dev->buffer);
        kfree(mmap_dev);
        unregister_chrdev_region(devt, 1);
        return ret;
    }

    // Create a device class for sysfs
    mmap_class = MMAP_CLASS_CREATE(DEVICE_NAME);
    if (IS_ERR(mmap_class)) {
        cdev_del(&mmap_dev->cdev);
        kfree(mmap_dev->buffer);
        kfree(mmap_dev);
        unregister_chrdev_region(devt, 1);
        return PTR_ERR(mmap_class);
    }

    // Create the device node in /dev
    mmap_device = device_create(mmap_class, NULL, devt, NULL, DEVICE_NAME);
    if (IS_ERR(mmap_device)) {
        class_destroy(mmap_class);
        cdev_del(&mmap_dev->cdev);
        kfree(mmap_dev->buffer);
        kfree(mmap_dev);
        unregister_chrdev_region(devt, 1);
        return PTR_ERR(mmap_device);
    }

    pr_info("mmap driver initialized\n");
    return 0;
}

/*
 * mmap_exit - Module cleanup function.
 *
 * Releases the device node, class, character device state, and all dynamically
 * allocated memory used by the driver.
 */
static void __exit mmap_driver_exit(void)
{
    device_destroy(mmap_class, mmap_dev->devt);
    class_destroy(mmap_class);
    cdev_del(&mmap_dev->cdev);
    kfree(mmap_dev->buffer);
    kfree(mmap_dev);
    unregister_chrdev_region(mmap_dev->devt, 1);
    pr_info("mmap driver exited\n");
}

module_init(mmap_driver_init);
module_exit(mmap_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Surendra Peddapolu");
MODULE_DESCRIPTION("A simple mmap driver");
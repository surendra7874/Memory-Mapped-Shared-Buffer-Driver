# Testing and Validation

This project can be validated in a few simple steps.

## 1. Build the kernel module

```bash
cd driver
make
```

This compiles the module as `mmap.ko`.

## 2. Load the driver

```bash
cd scripts
./load_driver.sh
```

The script builds the module, inserts it with `insmod`, and creates the device node if needed.

## 3. Build the user-space app

```bash
cd userspace
make -f MakeFile
```

## 4. Run the application

```bash
./mmap_app
```

Expected behavior:

- the program opens `/dev/mmap_device`
- it maps the device memory
- it writes a test string into the mapped region
- it prints the mapped memory contents
- it unmaps the region and exits cleanly

## 5. Unload the driver

```bash
cd scripts
./unload_driver.sh
```

It removes the device node and unloads the module with `rmmod`.

## Notes

- Make sure you run the build scripts with sufficient privileges when loading or unloading the kernel module.
- This project is intended for Linux environments with a kernel build toolchain available.

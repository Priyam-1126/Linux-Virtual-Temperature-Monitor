# Stage 4 – Character Driver

The driver is a small Linux character device module.

It uses:

- dynamic device number allocation (`alloc_chrdev_region`),
- `cdev` and a `file_operations` table,
- device class and device node creation (`/dev/virtual_temperature`),
- `open`, `read`, `write`, `release`,
- a mutex around the shared temperature value,
- `copy_to_user` and `copy_from_user`,
- end-of-file handling in `read`, so `cat` stops after one value (added in Stage 5),
- a `temperature` module parameter for the start value.

The driver exposes `/dev/virtual_temperature` on a suitable Linux system.

# Stage 5– Character Driver

The driver is a small Linux character device module.

It uses:

- dynamic device number allocation,
- `cdev`,
- device class and device node creation,
- `open`, `read`, `write`, `release`,
- a mutex around shared temperature data,
- `copy_to_user` and `copy_from_user`.

The driver can expose `/dev/virtual_temperature` on a suitable Linux system.

# Linux Driver Build and Use

The driver is an external Linux kernel module named `virtual_temperature`.
It creates the device file `/dev/virtual_temperature`.

- **Read** gives the temperature as text, for example `28`.
- **Write** accepts a whole number from -50 to 150, for example `65`.

## Requirements

- Linux machine or VM (not WSL1, not macOS)
- Kernel headers that match the running kernel
  (`sudo apt install linux-headers-$(uname -r)`)
- `gcc`, `make`, and `sudo` access
- Kernel 6.4 or newer is preferred. 

Check that the headers match:

```bash
uname -r
ls /lib/modules/$(uname -r)/build
```

## Build

```bash
cd driver
make
```

Creates `virtual_temperature.ko`.

## Load

```bash
sudo insmod virtual_temperature.ko            # default 28 C
# or:  sudo insmod virtual_temperature.ko temperature=40
ls -l /dev/virtual_temperature
```

## Read and update

```bash
cat /dev/virtual_temperature                  # 28
echo 65 | sudo tee /dev/virtual_temperature   # set 65
cat /dev/virtual_temperature                  # 65
```

## Kernel log

```bash
dmesg | tail -n 20
```

## Use with the C++ application

The device file is owned by root, so a normal user cannot open it.
Either give permission for the demo:

```bash
sudo chmod 666 /dev/virtual_temperature
./build/temperature_monitor
```

or run the program with `sudo ./build/temperature_monitor`.

If the program cannot open the device, it prints the reason and uses the simulated sensor.
The source is chosen at start-up, so load the driver *before* starting the program.

## Unload

```bash
sudo rmmod virtual_temperature
```

## Common problems

| Problem | Reason | Fix |
|---|---|---|
| `make`: "No such file ... build" | kernel headers not installed | install `linux-headers-$(uname -r)` |
| `insmod: Invalid module format` | module built for another kernel | rebuild after `make clean` |
| `insmod: Operation not permitted` | not root, or Secure Boot blocks unsigned modules | use `sudo`; test in a VM with Secure Boot off |
| App says "Permission denied" | `/dev` file is root-only | `sudo chmod 666 /dev/virtual_temperature` |
| `echo 300 > ...` fails | value outside -50..150 | use a value in range |

## One-command test

```bash
bash tools/live_driver_test.sh      # run from the project folder
```

Saves the result to `tests/results/driver-live-test.txt`.

## Status

This driver was compiled successfully on Debian kernel headers 6.12.96
(see `tests/results/driver-build-output.txt`).
 

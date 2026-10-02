# Linux Driver Build 

The driver is an external Linux kernel module named `virtual_temperature`.

## Build

Use a Linux machine or VM with kernel headers matching `uname -r`:

```bash
cd driver
make
```

## Load

```bash
sudo insmod virtual_temperature.ko
ls -l /dev/virtual_temperature
```

## Read

```bash
cat /dev/virtual_temperature
```

## Update

```bash
echo 65 | sudo tee /dev/virtual_temperature
cat /dev/virtual_temperature
```

## Logs

```bash
dmesg | tail -n 20
```

## Unload

```bash
sudo rmmod virtual_temperature
```


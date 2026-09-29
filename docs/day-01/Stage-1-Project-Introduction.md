# Day 1 - Stage 1: Project Introduction

**Date:** 28 September 2026

## Project Title

**Linux-Based Virtual Temperature Monitoring System**

## Introduction

This project is a small Linux-based system for checking temperature from a virtual sensor.

A C++ application will read the temperature through a Linux device interface and show the current status.

## Problem Statement

A temperature sensor needs a way to provide its data to software. In this project, a virtual sensor is used instead of real hardware so the hardware-software interaction can be studied in a simple setup.

## Objective

- Read a temperature value from a virtual device.
- Use a Linux character device as the interface.
- Display the value using a C++ application.
- Give a warning when the temperature crosses a limit.
- Keep a simple record of important events.

## Scope

The project will focus on one virtual temperature sensor.

It will include the Linux device interface, C++ application, alert handling and basic logging.

Real sensor hardware and advanced interfaces such as GPIO, I2C and SPI are not part of the current scope.

## Expected Outcome

The final prototype should be able to read a temperature value, show the device status, detect a high-temperature condition and record the event.

## Application / Use Case

The project is a simple example of how a Linux application can get data from a device. It can be used as a learning model for basic embedded monitoring systems.

## Planned Technologies

- Ubuntu / Linux
- C++
- Linux System Programming
- Linux Character Device Driver
- g++ and Make
- Git / GitHub

## Day 1 Note

Today the project idea, scope and main objective were finalized. No implementation was done.

## Next Step

Prepare the project requirements and development plan.

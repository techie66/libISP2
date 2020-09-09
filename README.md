<!-- vim: syntax=Markdown -->
# libISP2

A library designed to provide easy communication with a device sending data via the Innovate Serial Protocol version 2. This library is currently read-only.

## About

The main project is the library, `libISP2`, which attempts to decode a stream that uses the Innovate Serial Protocol and provides a simple API to access any valid data within that stream.

There is an example project that shows basic usage with a file descriptor:

`/libiSP2_example [-v] [-d] [-s] file`

Where:

* `file` - A file or device node. If a file, should be raw binary output from a supported Innovate device.
* `-s` - `file` is a serial port (eg. `/dev/ttyS0`, `/dev/ttyUSB0`). Sets up correct baud rate and whatnot to work.
* `-d` - Add a ~40ms delay to file input to simulate realtime. Don't use this with `-s`.
* `-v` - Not implemented

## Details

libISP2 provides the function ISP2::isp2_read. The first argument is a file descriptor, the second is an isp2_t struct. It will attempt to parse one packet from the given file descriptor into the given struct. If successful, ISP2::isp2_read will return 0. If ISP2::isp2_read cannot find a header, it will return -1.

Until I write more better documentation, please have a look at isp2.h for information about the isp2_t struct.

## Installation

```bash
git clone https://github.com/nearwood/libISP2
cd libISP2
cmake --build build
make
#make install #TODO Incomplete
```

Binaries would be then located in `build/bin` and `build/lib`.

## TODO

 - [x] Fix serial port access
 - [x] Move serial port to library, as optional at compile time
 - [x] CLI example
 - [ ] GUI gauge example
 - [ ] Convert ISP2 library to C, not really using C++ features
 - [ ] Export NO_SERIAL_SUPPORT to cmake
 - [ ] Use non-blocking IO, and show some kind of RX indicator
 - [ ] Add more emojis

## Credits
* [techie66](https://github.com/techie66) for the [original implementation](https://github.com/techie66/libISP2).

[Innovate Motorsports](https://www.innovatemotorsports.com/) provides documentation detailing the protocol:
* PDF: [Innovate Serial Protocol v2](https://www.innovatemotorsports.com/support/downloads/Seriallog-2.pdf)
* PDF: [ISPv2 Supplement](https://www.innovatemotorsports.com/support/downloads/Serial2%20Protocol%20Supplement.pdf)

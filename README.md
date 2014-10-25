About
=====

PulseAudio emulation for ALSA.

Project is in stale state since its proclamation. The main objective,
working Skype test call, is reached. I don't have any plans for further
development.

Install
=======

```
$ mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
$ make
# make install
```

If you want 32-bit binaries on 64-bit machine (for example, for Skype), use:
```
$ mkdir build && cd build
$ CFLAGS=-m32 cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
$ make
# make install
```

There is a way to configure where apulse libraries will be installed, via
`APULSEPATH` cmake variable. For example, if you want to install libraries
into default path, `/usr/lib`, use
```
cmake -DAPULSEPATH=/usr/lib -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
```

If libraries are installed to a regular library path, you don't need run applications
through `apulse` wrapper.

Note you need to select build type to be `Release`, otherwise it will output enourmous
amount of debug text to the stdout.

Usage
=====

```
$ apulse <program-name> [parameters]
```

Environment variables `APULSE_CAPTURE_DEVICE` and `APULSE_PLAYBACK_DEVICE` can be used
to configure capture and playback devices. Try `hw:0,0`, `plughw:0,0` and the like.
Refer to the ALSA user guide for a full list of device names.

License
=======

Source code is distributed under the terms of the MIT License. See LICENSE.MIT for full text.

Third party code
================

`/3rdparty/pulseaudio-headers` contains part of PulseAudio project and is distributed
under LGPLv2.1+ terms. See content of the files for details.

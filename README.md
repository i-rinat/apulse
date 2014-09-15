About
=====

PulseAudio emulation for ALSA.

Install
=======

```
$ mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
$ make
# make install
```

If you want 32-bit binaries on 64-bit machine, use:
```
$ CFLAGS=-m32 cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
```
as configure step.

Usage
=====

```
$ apulse <programmname> [parameters]
```

License
=======

The MIT License. See LICENSE.MIT for full text.

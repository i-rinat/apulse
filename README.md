About
=====

PulseAudio emulation for ALSA.

The program provides an alternative partial implementation of the PulseAudio
API. It consists of a loader script and a number of shared libraries with the
same names as from original PulseAudio, so applications could dynamically load
them and think they are talking to PulseAudio. Internally, no separate sound
mixing daemon is used. Instead, apulse relies on ALSA's `dmix`, `dsnoop`, and
`plug` plugins to handle multiple sound sources and capture streams running at
the same time. `dmix` plugin muxes multiple playback streams; `dsnoop` plugin
allow multiple applications to capture from a single microphone; and `plug`
plugin transparently converts audio between various sample formats, sample rates
and channel numbers. For more than a decade now, ALSA comes with these plugins
enabled and configured by default.

`apulse` wasn't designed to be a drop-in replacement of PulseAudio. It's
pointless, since that will be just reimplementation of original PulseAudio, with
the same client-daemon architecture, required by the complete feature
set. Instead, only parts of the API that are crucial to specific applications
are implemented. That's why there is a loader script, named `apulse`. It updates
value of `LD_LIBRARY_PATH` environment variable to point also to the directory
where apulse's libraries are installed, making them available to the
application.

Name comes from names of both ALSA and PulseAudio. As `aoss` was a compatibility
layer between OSS programs and ALSA, `apulse` was designed to be compatibility
layer between PulseAudio applications and ALSA.


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


Usage
=====

```
$ apulse <program-name> [program-parameters]
```

Environment variables `APULSE_CAPTURE_DEVICE` and `APULSE_PLAYBACK_DEVICE` can be used
to configure capture and playback devices. Try `hw:0,0`, `plughw:0,0` and the like.
Refer to the ALSA user guide for a full list of device names.

System directory versus separate directory installations
--------------------------------------------------------

By default, libraries from `apulse` are installed into a separate directory, in
order to hide them from all applications.

Most applications in the wild, that support both PulseAudio and ALSA, try to
autodetect which sound system is used. First, applications try to start with
PulseAudio. Original client libraries fail early if no PulseAudio daemon is
running or can be started. Then they switch to ALSA. Decision is made once, at
the beginning. It works fine with PulseAudio, but doesn't work with
`apulse`. Latter has no daemons, it happily says that everything is fine, and
it's capable of playing audio. Applications then try to call more functions, and
eventually touch unimplemented parts, often with crashes. So, libraries are
hidden, and become visible only when a program is called through `apulse`
wrapper script.

It's possible to install apulse libraries to `/usr/lib`. Wrapper script won't
be required, but then all applications will try to use PulseAudio API, despite
they can use ALSA.


License
=======

Source code is distributed under the terms of the MIT License. See LICENSE.MIT for full text.

Third party code
================

`/3rdparty/pulseaudio-headers` contains part of PulseAudio project and is distributed
under LGPLv2.1+ terms. See content of the files for details.

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

You need ALSA libraries and GLib installed. On Debian-based distributions, they
are in packages `libasound2-dev` and `libglib2.0-dev`.

To build and install, run in source directory:

```
$ mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
$ make
# make install
```

That will create directory named `build`, and build there. It's possible to
install just by running `make install` as `root`, as shown above. But you won't
be able to uninstall installed files. That's why it's recommended to wrap files
into a package. Use `checkinstall`, or some alternative.

If you want 32-bit binaries on 64-bit machine (for example, for Skype), use:
```
$ mkdir build && cd build
$ CFLAGS=-m32 cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
$ make
# make install
```

Recent GLib versions use different `.pc` files for `i386` and `amd64`. To help
`pkg-config` find 32-bit versions, use `PKG_CONFIG_PATH` variable.  So, on
Debian it will be something like:

```
$ PKG_CONFIG_PATH=/usr/lib/i386-linux-gnu/pkgconfig CFLAGS=-m32 cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release ..
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

Per-app RPATH trick
-------------------

There is the RPATH property of ELF executable format, which is used to specify
paths to search for dynamic libraries. It's like LD_LIBRARY_PATH variable, but
per-executable. Since all that `apulse` launcher script does is setting
LD_LIBRARY_PATH value before launching an application, it's possible to bake
paths to apulse libraries into target executable itself. And so to launch it
as usual, without helper script.

For example, for Firefox it would be:

```
# patchelf --set-rpath /usr/lib/apulse /usr/lib/firefox/libxul.so
```

For some reason, it doesn't work if RPATH is set for `/usr/lib/firefox/firefox`
itself, so some experiments are required to make it work.

Known issues
============

Not implemented functions, application crashes
----------------------------------------------

Large portion of PulseAudio API is not implemented. There are functions that do
nothing and return some arbitraty values. Often, if application tries to call
something not implemented, it crashes while trying to dereference a NULL
pointer.  By default, tracing level is set to `0`, which means no messages are
printed to standard output. It's possible to increase that value to `1`, which
shows unimplemented function calls, or to `2`, which shows all function calls.

To change level, use `WITH_TRACE` parameter when calling `cmake`. Something like
`cmake -DWITH_TRACE=1 ..`

Building apulse with trace level 1 won't fix issues, but will at least help to
identify if crashes are caused by not implemented functions.

Generic errors in do_connect_pcm
--------------------------------

Apulse acts as a generic ALSA client. It tries to open audio device, and
sometimes fails.  At its core, apulse does neither audio mixing nor
resampling. Instead, it relies on `plug`, `dmix`, and `dsnoop` ALSA plugins,
which are usually enabled by default. These plugins handle multiple audio
sources, performing resampling and mixing transparently. For years now ALSA
comes with those plugins enabled. Audio just works without configuring
anything. But not everybody use default settings.

On custom configurations apulse may fail to output and/or capture audio. There
could be no sound at all, or just a single audio stream playing at a time.  It's
also possible that adapters with hardware mixers, which capable of playing
multiple streams, may still be unable to handle multiple capture
streams. Depending on hardware, you may still need either `dmix` or `dsnoop`
plugins. Or both.

In other words, for apulse to work, your setup should be capable of playing and
capturing multiple streams simultaneously.

Access errors in do_connect_pcm
-------------------------------

If other applications output sound just fine, it's possible that application you
are using restricts itself.

For example, Firefox now have a sandbox, that blocks file access. It has
predefined list of allowed paths, but ALSA devices are not included by
default. Fortunately, it's possible to add those path by hand. Add "/dev/snd/"
to "security.sandbox.content.write_path_whitelist" parameter in
`about:config`. Note that trailing slash in "/dev/snd/" is required.

Firefox 58 tabs crashing when trying to play audio
--------------------------------------------------

Firefox 58 (Nightly) tightened its sandbox a bit more. Now `ioctl()` calls are
forbidden too, but are used by ALSA libraries. That causes sandbox violation
with subsequent process termination. Exception can be added by setting parameter
`security.sandbox.content.syscall_whitelist` in `about:config`. That field
accepts a comma separated list of system call numbers. Add there `16` for
x86-64, or `54` for x86 or ARM.

License
=======

Source code is distributed under the terms of the MIT License. See LICENSE.MIT for full text.

Third party code
================

`/3rdparty/pulseaudio-headers` contains part of PulseAudio project and is distributed
under LGPLv2.1+ terms. See content of the files for details.

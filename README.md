# oboe

Toy operating system targeting <del>ARM9</del> ARM Cortex-A8.

Long-term goal is an Oberon-style system with a GUI + hot code reloading. 

Initial plan is to get a simple UART-based test running in qemu then transfer to a [BeagleBone Black](http://beagleboard.org/Products/BeagleBone+Black).

## Current Status

  - Rudimentary blocking UART IO
  - 16-bit framebuffer with ASCII text output
  - Keyboard character echo

![Oboe](https://raw.github.com/jaz303/oboe/master/screenshot.png)

## Hacking

  1. Install an ARM toolchain. I'm using [Yagarto](http://www.yagarto.org/).
  2. Install qemu; make sure it's in your `$PATH`.
  3. `$ export TOOLCHAIN=/path/to/arm/toolchain`
  4. `$ make emu`
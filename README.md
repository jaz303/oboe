# oboe

Toy operating system targeting ARM9.

Long-term goal is an Oberon-style system with a GUI + hot code reloading. 

Initial plan is to get a simple UART-based test running in qemu then transfer to a beefy dev board [like this](http://www.atmel.com/microsite/sam9gx/).

## Hacking

  1. Install an ARM toolchain. I'm using [Yagarto](http://www.yagarto.org/).
  2. Install qemu; make sure it's in your `$PATH`.
  3. `$ export TOOLCHAIN=/path/to/arm/toolchain`
  4. `$ make emu`
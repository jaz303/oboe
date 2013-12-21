TRM is here:
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0198e/DDI0198E_arm926ejs_r0p5_trm.pdf

ARM926EJ-S is targeted at multi-tasking applications with full memory management. Phew, that's a relief.

Supports 32-bit ARM and 16-bit Thumb modes. ARM debug architecture. Harvard cache architecture (separate instruction cache + data cache). MMU/TLB.

Coprocessor interface for floating point etc. Think the chip I'm targeting has one of these.

TLAs I'm not familiar with:

  * ETM
  * TCM - tightly-coupled memories
  * AHB

CP15 - system control coprocessor.
CP15 registers can only be accessed with the MRC and MCR instructions in a privileged mode. Instruction format on TRM page 35.

CP15 has 16 registers (TRM page 33).

With some exceptions all CP15 registers are initialised to zero on reset. (TRM page 34)

3 types of addresses:
 - virtual address (in processor) (VA)
 - modified virtual address (caches/MMU) (MVA)
 - physical address (TCM/AMBA bus) (PA)
 
- core issues instruction VA
- VA translated using FCSE PID to MVA
- protection check carried out on MVA by MMU, may abort
- cache check, if fails, convert MVA to PA

Control register c1
 - cache replacement strategy
 - location of exception vectors
 - icache enable/disable
 - protection bits
 - endianness
 - dcache enable/disable
 - alignment fault enable/disable
 - mmu enable/disable

Translation Table Base Register
Domain access control register (16 domains, D15-D0)
Fault status register
Cache operations register - invalidate all/entry, clean, test, prefetch, drain write buffer, wait for interrupt
TLB operations register - invalidate all/entry, invalidate instruction/data
Cache lockdown register
TCM region register
TLB lockdown register
Process ID register - fast context switching

MMU
---

Caches and Write Buffer
-----------------------

Tightly-Coupled Memory Interface
--------------------------------
Physically addressed address space. 2 regions - instruction + data. TCM region register controls their locations. Physical size defined by external inputs.

Bus Interface Unit
------------------

Noncacheable Instruction Fetches
--------------------------------

Coprocessor Interface
---------------------

Instruction Memory Barrier
--------------------------
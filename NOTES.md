ARM9E uses the ARMv5 architecture.

This RPi tutorial looks like a good starting point:
http://wiki.osdev.org/ARM_RaspberryPi_Tutorial_C

Integrator Board
----------------

http://infocenter.arm.com/help/topic/com.arm.doc.dui0159b/DUI0159B_integratorcp_1_0_ug.pdf

LCD controller
--------------

Base address: 0xC0000000

http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0159b/index.html
http://infocenter.arm.com/help/topic/com.arm.doc.dui0159b/DUI0159B_integratorcp_1_0_ug.pdf
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0161e/I904755.html
http://wiki.osdev.org/ARM_Integrator-CP_PL110_Dirty

Having trouble tracking down the TRM for this one...

KMI interface
-------------

http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0143c/index.html

Interrupt Controller
--------------------

The Cortex A8 does not have a built-in interrupt controller. Instead it seems there's an external controller that tweaks the core's IRQ and FIQ lines. Here's the documentation for that which is found on the IntegratorCP:

http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0138e/Chdfjiga.html
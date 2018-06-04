# ex2_eps

Ex-Alta 2 EPS module firmware EPS firmware Prototype


## Warning: This project is in a prototyping phase and includes placeholder design decisions that do not represent the final design.


eps.c implements the designed functionality of the ExAlta2 EPS controller firmware.
The various hw*.c files implement various hardware configurations.

## Configurations:

eps
- Deployable build intended for flatsat and launch hardware.
- Not yet implemented.

eps-sim
- Simulated hardware meant for development and unit testing.
- Build using "make eps-sim"

eps-arduino
- For prototyping hardware and hacks implemented using an Arduino board.
- Not yet implemented. Compiles hw_sim.c until hw_arduino.c is created.




### Design guidelines
- Any constants etc. that are used outside of the controller should go in datasheet.h
- eps.c should not assume anything about the hardware, which is abstracted in hw.h.
- Variable names of physical quantities should include the units (not complete).
- Avoid dynamic memory allocation or data/pointers that are uninitialized at boot.
- Do things that increase safety first, eg. if turning some power drains on and others off, turn off first, unless the order is given in the spec.

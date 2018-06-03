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
- Not yet implemented.

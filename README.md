# Trigo y Sol
Trigo y Sol is a 40" tall piece that hangs on a wall and features five illuminated arcade buttons that allow the viewer to interact with it. The painted plywood structure is in the shape of a life-size shield with a sun-shaped inset that features a blue LED backlight. Five buttons are mounted to an enclosure that also houses the power supply and control board, an Arduino Nano. 112 LEDs are arranged to resemble five stalks of wheat, similar to what is seen in national flags or other agriculture icons. 

Each LED on the surface of the "sun" is softened by 3D printed diffuser: PLA mixed with wood to give the appearance of an organic material. Stringy strands of plastic which were an unexpected by-product of the 3D printing process were left attached; they reinforce the appearance of the grains of wheat.
When all 112 LEDs are at full brightness, the LEDs, buttons, and controller require 3.8 Amps at 5 Volts. The blue backlight requires 1 Amp at 5 Volts.

Seven game modes and one non-interactive "standby" mode are selectable by a hidden mode change button just above the rightmost arcade button. The current mode is indicated by one or more of the wheat stalks lighting up for one second before the mode activates. Game modes include:

**Shooter:** a colored dot travels from right to left. The player must press the same color button to launch a mortar to explode the advancing ordinance.

**Fireworks:** The "night sky" is dark until the player presses an arcade button. A firework emitting a white light is then seen traveling into the sky. If the same button is pressed before the firework hits the ground, it will explode into a beautiful display of the chosen color. Multiple simultaneous fireworks can be launched and exploded.

**Pulse:** Press a button and a pulse of that color will advance from left to right across the wheat stalks. Press more than one button to see what combinations can be produced.
##Beam: Press and hold a button to launch a beam of corresponding color. Press and hold more than one button to see what combinations can be produced.

**Color Invaders:** Similar to the classic "Space Invaders" arcade game, aliens of different colors descend from space to attack earth. Match the color of the first infantry alien to blow him up. Blow up as many invaders as you can, but watch out because their advance gets faster and faster the closer to earth they get!

**Simon:** This is the pattern matching "Simon says" game. The Arduino creates a random sequence of lights and requires the player to repeat the sequence. If the player succeeds, one more light is added to the sequence, which becomes progressively longer and more complex. Once the user fails, the Arduino displays the correct color (what the player should have selected at that point in the sequence), and the game is over.

**Juggle:** The player is challenged to "juggle" as many "balls" as possible by  catching and throwing different colored balls back into the air. A successful catch/throw is made by hitting the button that  matches the color of the ball that is about to hit the ground.

A hidden power switch just above the leftmost arcade button interrupts current to the power supply. A braided steel wire is provided on the rear of the piece so it can be hung from a single anchor point. Foam covered bumpers on the rear protect the wall from scratches and help to stabilize the piece when buttons are pressed.

[How the LEDs are laid out](https://docs.google.com/spreadsheets/d/1bdw3SPKmITZUk9DHkq45C_wzfda27RjEgO8ZBFDRKSQ/).

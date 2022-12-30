# Worms
by There can only be two.

Link of the Github Repository: 

## Developers
- Xavi Alcañiz Carrillo (@StarvinXarvin)
- Héctor Báscones Zamora (@Hekbas)
- Jan Neto Llorens (@JanNetoLlorens)
- Júlia Serra Trujillo (@softdrawss)

# Grading Criteria
It follows the submission rules stated above.
- The game did not crash while testing.
- The physics engine implements at least four of the following forces: impulsive, gravity, lift, drag, buoyancy, elastic (springs). It allows for controllable player movement. It resolves collisions (regardless of complexity).
- The physics engine allows for full configuration and debug in real-time: the player must be able to switch between different integration/delta-time schemes, player control schemes, collision solvers, enable/disable specific forces, etc.
- Technical/Debug information is displayed in real-time on the screen.
- The game has a clear WIN and LOSE condition with an optional set of intermediate bonus objectives (if any).
- The game has a clear win/lose condition.
- The game is fun and comfortable to play with well defined rules.
- Original gameplay and graphics are well represented as well as animations, audio FX and score. Creativity will be rewarded.

## Keys
- A and D: Player movement on x
- W: Player jump
- LEFT AND RIGHT ARROWS: Change shooting angle
- DOWN AND UP ARROW: Change shooting force
- SPACE: shoot
- ESC: Quit

### Debug
- TAB: Enables and displays the following debug functionality
  - F1: Draws colliders
  - F2: Turn the lights of the worm ON/OFF
  - O: Turn the sfx of the worm ON/OFF
  - Frame Control: (I-/U+)
  - Gravity: (J-/K+)
  - Wind: on X (R-/T+) / on Y (3-/4+)
  - B: Switch between player and grenade to enable variable setting
    - Bounce coef: (N-/M+)
    - Aerodynamic Drag coef: (Z-/X+)
    - Hydrodynamic Drag coef: (G-/H+)
  - 5: enables/disables GRAVITY
  - P: change between players 1 and 2
  - U: Hide UI
  - V: Displays the following variables - Player 1 and 2: position Missile and Grenade: position, velocity, acceleration, grenade cooldown, show weopon equiped- 
  - 1: Grenade selected
  - 2: Missile selected
  
## Information
- 2 types of weopons: grenades and missiles
- Forces applied: aerodynamic drag, hydrodynamic drag, gravity, impulse forces, bouyancy.
- When the player collides with water we add 200kg to its mass to simulate the death as in the worms game.
  
## Feautures
- Explosions: grenades and missiles explode and damage players in a radius.
- Multiplayer game

## Development


-There can only be two

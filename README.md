# Worms
by There can only be two.

Link of the Github Repository: https://github.com/There-can-only-be-two/Worms

## Developers
- Xavi Alcañiz Carrillo (@StarvinXarvin)
- Héctor Báscones Zamora (@Hekbas)
- Jan Neto Llorens (@JanNetoLlorens)
- Júlia Serra Trujillo (@softdrawss)

## Keys
- A and D: Player movement on x
- W: Player jump
- LEFT AND RIGHT ARROWS: Change shooting angle
- DOWN AND UP ARROW: Change shooting force
- SPACE: shoot
- ESC: Quit
- ENTER: Restart once the game finishes

### Debug
- TAB: Enables and displays the following debug functionality
  - F1: Draws colliders
  - F2: Turn the lights of the worm ON/OFF
  - O: Turn the sfx of the worm ON/OFF
  - Frame Control: (I-/U+)
  - Gravity: (J-/K+)
  - Wind: on X (R-/T+) / on Y (3-/4+)
  - B: Switch between player and grenade to enable variable settings
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
- 2 types of wepons: grenades and missiles
- Forces applied: aerodynamic drag, hydrodynamic drag, gravity, impulse forces, bouyancy.
- When the player collides with water we add 200 kg to its mass to simulate the death as in the worms game.
  
## Features
- Explosions: grenades and missiles explode and damage players in a radius.
- Multiplayer game
- GOD MODE: While Player 1 is always in GOD MODE, which results in no taking any damage nor reacting to explosions, Player 2 is the opposite. (The aim for this was to show how players react to explosions without them dying so easily and show the Game loop once the other Player died)
- Game loop: Once one of the player dies, the game will show it is over and players can restart to play again.

## Development

- The thing that has given as most problems for sure are the collisions, we have passed a long time trying to figure out how to make collision at least believable.

-There can only be two

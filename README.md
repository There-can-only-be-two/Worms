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
- 
- LEFT AND RIGHT ARROWS: Change shooting direction
- DOWN AND UP ARROW: Change shooting force
- SPACE: shoot
- ESC: Quit

### Debug
- TAB: Enables and displays the following debug functionality
  - F1: Draws all colliders and allow to use a mouse joint
  - F2: Turn the lights of the pinball ON/OFF
  - F3: Turn the music of the pinball ON/OFF
  - Frame Control: (Q-/E+)
  - Gravity: (S-/W+)
  - Bounce coef: (A-/D+)
  - V: Displays the following variables - Ball(x, y), Spring force, Gravity and Bounce coef
  - F: Finishes the game and displays rankings
  - U: Change display of the UI
  - R: Resets ranking to default (Only after the game has finished and the ranking is displayed)

## Information
- AMOUNT OF BALLS PER GAME: 3 balls
  - Number of balls remaining is displayed at the bottom of the pinball.
- TIME: Even though game finishes when the player is out of balls, it can also finish when the player is out of time.
  - Time remaining is displayed at the bottom left of the pinball.
  
## Combos and Feautures
- EXTRA KIKCERS: Automatically shoots the ball when touched. Once activated, the kicker used turns off and will not activate again.
- EXTRA TIME: Passing through the Time feauture rewards the player extra time. However, Extra Time cancels the X2 multiplier and equals it to 1.
- MULTIPLIER: 
  - X2: Activating the three parts in the upper right zone of the map will sum 2 to the current value of the multiplier.
  - X10: Activating the three parts in the upper right zone of the map will temporally sum 10 to the current value of the multiplier.
- LIGHTS: Lights will turn on and off when ball bounces, parts of the pinball are activated, etc.
- RANKING: When the game finishes, the ranking displays the TOP 5 scores of the game. Scores by player are displayed in yellow. Try to get to TOP 1!
- UI: Highscore, Current Score, Multiplier, Previous Score, Time remaining, Balls remaining

## Development


-There can only be two

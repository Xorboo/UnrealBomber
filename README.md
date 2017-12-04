# UnrealBomber
Bomberman demo on Unreal Engine 4

# To the reviewer
### Completed Tasks
* Basic bomberman-like gameplay. Fair explosions, allowing bombs chaining, everything is exactly like in the original, except for player movement.
* Basic pickups: speed, bombs, bombs range
* Death with auto-respawn
* Random level generation
* Sounds
* Blasts effects

### Not Completed Tasks
* Local Co-Op
* Remote-Controlled Bombs
* No timer, no draw conditions
* No UI
* No score
* No AI

### Reasons and stuff
Not that it matters, but still... As I mentioned previously - I don't really have a lot of experience in UE4 because I was working on Unity for the past few years. Basically I was learning the engine while developing the demo, and a lot of time were spent on reading documentation and fixing dumb mistakes. There are no actual technical difficulties for me to complete the tasks, but I've already spent ~10-15 hours on the assignment, and it said "You should not spend more than 15 hours on the test", so I didn't. Well, I don't yet know how to make 2 player controls, because most of the solutions I found on the web look way too hack'ish to be the right ones.

Naming conventions are kinda weird in the assignment. I was jumping between conventions from the official tutorials and from the code I used to write on Unity.

### Ownership
* All the code and blueprints were made by me (player movement controller were inspired by standart demo).
* All other assets (textures, models, sounds) were either included with the initial project or downloaded from the web.

# TODO
### Things to fix
* Add a proper Player Controller so it would be separate form the Pawn class
* Separate GameMode from Match logic into different scripts
* No out of bounds check for bombs and pickups - it shouldn't ever happen right now, but it is one simple function that will save form the crashes
* I little bit of hardcode here and there, sorry

### Things to add
* Local multiplayer
* Extra pickups
* Core round logic - timers, death, etc
* UI
* AI (from simple random-like to complex hunter-like)

Probably should do it after i dive a bit into some other examples to get more information about best practices.

# Points of interest
* **UnrealBomberGameModeBase** c++ class: all the core logic - explosions, generation, game state "managing"
* **BP_PlayerPawn** blueprint: player controls
* **BP_PickupX** blueprints: simple pickup logic
* **BP_Bomb** blueprint: simple bomb logic
* **BP_WallDestroyable** blueprint: destroyable wall logic - spawning pickups

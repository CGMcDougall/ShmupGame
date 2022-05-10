!!Original Copy found here - "https://github.com/dawsonkelly/GroupProj" !!

Authors: 
	Connor McDougall 101179300
	Dawson Kelly 101184804
	Kevin B 101184184

Project: Year 2 Shmup Project 

Description: 
	This game is a shoot 'em up in which you kill as many minibosses as possible before the final boss spawns when the timer hits zero. Killing minibosses will 	    reduce the final bosses health, but getting hit will reduce your timer significantly. Kill the final boss to win, but if you take any damage when the timer 	is at zero, you will lose. 
	Written in C++, with OpenGL. 

Compilation:
	Use cmake to compile with the libraries included in the libraries folder.	
		

Controls:
	R - switch weapons
	WS - move forward/backward
	AD - rotate left/right
	Space - fire


Notes: 

The same issue at the start of the semester with OpenGL's "delta time" being different for some computers arises in this project too. The specifications for this assignment currently
work on one of our computers, but not for 2 others. The computer that it works on also had the original game demo at the start of the year work fine. 

If adjustment is needed, there are variables in the player_game_object.cpp file that can be changed.

The objective of this game is to defeat the final boss, which is visible in the center of the map, but only finishes being constructed once a timer in the bottom left runs out. 
It can be defeated by shooting it once it spawns. In the bottom right is an arrow that points to the nearest miniboss, or, once the timer has run out, the final boss. 
Killing a miniboss takes health away from the final boss, and causes another miniboss to spawn if the timer is still active. 
If the player is hit while the timer is still active, they do not die, but instead the timer is decremented. Powerups are available around the map, they do the following:

-timer - increases timer by 30s
-nuke - kills all droids
-slow - cuts all existing droids’ speed in half

Marks:

1. The implementation of the timer, the objective of killing minibosses to weaken the final boss, and droids dropping powerups all give meaning to game mechanics by introducing dynamics. 
The player is given the choice of targeting/avoiding droids for powerups or minibosses for boss damage. The better the player does while the timer is active, the easier it is to defeat the final boss.
2. Enemies with three different textures appear throughout the game. Their weapons/AI differs as follows:
	Final boss (2 attack modes):
		-flies towards last known player position 2-4 times
		-spawns droids
	Miniboss: 
 		-spawns droids around itself
		-flees from player if it gets too close
	Droids:
		-constantly flies towards player (heat-seeking)
		-if player too far, wanders

3. The player can switch between a “shotgun” spread of bullets or a faster “machine gun” firing of bullets. We have a collision.cpp class that handles collisions. Shotgun bullets are smaller to differentiate the two.
4. Not implemented
5. Colliding with powerups (described above) brings advantages to the player.
6. Movement is handled through transformations. Most movement is physically based. Droids’ wandering is handled parametrically. The arrow and timer are hierarchically transformed, as they are relative to the player’s position.
7. Collision.cpp handles collisions between enemies, bullets, the player, and powerups
8. The game world is traversable and larger than the screen. The background is composed of multiple tiles so that it isn’t one big texture. Partial work is included included in game.cpp for randomizing background textures.
9. The HUD consists of the arrow and timer in the bottom corners.
10. We properly implemented abstraction, encapsulation, and inheritance to make our code readable and extensible. Non-obvious functions and sections of code are commented.



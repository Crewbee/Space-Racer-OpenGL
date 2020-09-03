# Space-Racer-OpenGL
First complete 3D game made with C++, OpenGL, Bullet Physics, XAudio and 3DSMax
# Synopsis
Building on what I had learned with OpenGL in 2D, I create shaders in GLSL for 3D and started on building the simplistic but addictive game loop for Space Racer. I got the idea when figuring out how to write lighting shaders. I was practicing lighting on a sphere mesh in an empty vertual space. I got the idea to slap a texture of the moon on the sphere to make the test bed more visually appealing. It looked so good I decided I wanted to make a space racing game. I was a big fan of the Star Wars Pod Racer game for the N64 back in the day. I also took inspiration from old space games where you would shoot asteroids and watch them explode. 

# How to Play
Download the repo and run the .exe in the Space Racer folder

Controls:

"W" for acceleration

Use the mouse to stir the ship around planets and asteroids, movement is joystick layout:

Up = Down

Down = Up

Right = Right

Left = Left


Use the Left Mouse Button to fire your laser to destroy planets and asteriods and earn points! So much fun gibbing things!!

Remeber, if you can't blow it up, DODGE!!

Pro Tip: If your a avid gamer, try completing the course on "Ludicrous Speed" which can be turned on or off in the Options menu. Good Luck!

# Highlights 

The biggest hurtle for me in the project was the control of the ship movement. Using the flat XY plane of a mouse and turning it into roll/pitch/yaw of the space ship
took quite a bit of mental gymnastic to come up with a reliable equation for. Thankfully I managed to find a solution that works well enough. Getting the ship control right
was a big part of this game. The ideal functionality of the ship movement was not to be super precise to make things difficult, and also not to be overtly uncontrollable taking
away from the player experience. I was aiming for the sweet spot between the two.

Next was creating object pools with enough objects to fill the scene but not so many as to make it too hard. Again a sweet spot had to be found.
I had to write a manager for the asteroids and planets, or plantoids if you will, that handle the creating and destruction of the objects. The object are destroyed by the
player or are despawned by the manager and returned to the pool if the Z position is behind the camera. 

The targeting system uses a ray shot from the front of the ship and paints the closest obstructing body as the target. The laser is a plane drawnfrom the ship to the target
with a very short lifetime.

The obects are all spawned randomly within the play space using unit circle math.

The music in game was not created by me. However, in order to get around using a full length song, I used Audition to find several clips from the song I 
wanted that could be repeated and used that. 

The three cubes around the ship are actually light sources. Color coded to the color of light they emit. In the context of the game though, they are part of the ships 
navigation system.

# Future Development Plans

I'm considering this game complete as of right now. If I had time I would like to remake it in an engine such as Unity. There, it would be faster easier to make different levels, more difficulties and more immersive lighting effects.

# Libraries

BulletPhysics

XAudio

# Ownership Notice

I do not own the textures or music used in the project:

Menu Music: Sonic Racer 2000

Game Music: Yellowline - Redline (Movie)

Textures: FreeTextures.com

Annoucer voice: Super Smash Bros. Melee

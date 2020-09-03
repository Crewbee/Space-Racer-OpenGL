Space Racer

********************************Progress Week 1:******************************

Meshes for space ship, moon and the asteroid are created and drawing
Added a skybox

Follow camera is implemented and follows the player ship, will need to tweak the campos
and lookat target being passed into the CreateViewLookAt to make it more fluid

Ship can move right/left/up/down with forces using the mouse location on screen to 
steer. Will work on finishing forward accelaration and bank turning.

The player does have a Raycasting call which is bound to 'S' however the problem I'm currently
figuring out is why the unique ID of the broudphasehandle returns as ASTEROID everytime 
when the expected result is MOON. NOTE: CollisionID's are declare in GameObject.h as 
an ENUM and applied to the collisionobjects using the broadphaseproxy struct.

To test the raycast add a break point on player.cpp line 89

The interface class for debug draw is overriden and can be activated use the S key as
requested. 

Lighting shader is mostly implemented however the code has not change much since my first
iteration aside from the ambient modifier.
 
***************************Progress Week 2:*************************************

Updated the Lighting Shader with functions to calculate point lights and the Space Ships
spot light or head light. There is a lighting ImGui window to with all of the require
modifiers in it. You can do the testing without moving the ship. There are ImGui
sliders to change the light position/attenuation/color/strength/on & off. The 
attenuation is managed using the integer index which calls a map of values 
I wrote into the map class that I found worked the best. I could not get it 
to work as well with a incremental float percentage.

Will need to create a light manager class to take care of repositioning the lamps
as the player flys through the course in a later iteration.

I couldn't figure out where the best place to declare the light structure, so I create
a header just for the structs called LightCasters and included it where it was needed.

Aside from the updates to the lighting to meet requirements I added in some asteroids
behind the moon. They are add into a pool and spawned in on a basic timer and removed
from the scene if they are behind the camera. 

A raycast is implemented now and does return a game object the comes into contact with 
it, however my spawn fragments function, the asteroid fragment mesh, and the lasers 
have not been finished yet so at this time the raycast simple calls the name of the asteroid
it hit. You can see the ray in red by enabling debug draw with 'S'. I will have to
update the rays direction so in is cast in the players local forward direction, current
it is just casting from the front of the player mesh to +5000 on the z axis.

I may have to rethink the player controls. I had tried to make the forces on the player
controlled solely by the direction of the camera as require however it simply doesn't
give the maneuverability require to for the game to be playable. I may have to simply
create force movement function for the mouse coords in screen space rather than the
normalized direction of the follow camera. Let me know if this is something that
would cause me to loose marks.

******************************Week 3 Progress***************************************

Lots of work no sleep oh god im dying...Scene Manager is implement and audio, the 
queue I have not finish yet, I did not have time to do it...haven't slept in like 36 hrs
Will try and get something done by tomorrows end. Background music sounds great though, 
I was able to work some magic in Audition to clip the song in a way that I can loop through 
short 7 - 12 sec clips and make them have sense. 

Scenes completed:
Splash Screen - All graphics where made by yours truly is PS.
Main Menu - with buttons that work and light up with mouse left click and mouse over
Options Scene with 2 options that do effect the game. Though the final will have some more robust option
I am hoping to have time to do. I allowed the button to stay lit to show that the option is in place.
HUD - is basically just a score card, you can get points by shooting space bodies with the ray (left mouse button)
Game - while the guts are still a work in progress most of the classes are almost done
I will have to work on filling them out and implementing them this week for the game 
to start taking real shape,
TODO
Pause - I feel as though I could get this done before tuesday, the nub of it at the least
Death Scene - I didn't make this one a priority as I haven't had a chance to do the reset function OR on death functions yet.
Again will try to get something up for the checkpoint by tomorrow.

Audio - Audio Controller is created, I wasn't sure if having 2 sound players would through the audio engine I will have
to run this by you next time I see you.

Other Work completed
UIObject Class
OrthoCamera Class
Audio Event created
Score Event create
Aspect Ratio is modifiable in all scenes
Player is now bound to a cylindrical radius
Player will loose momentom if the ship is not angled and not being accelarated
Ray is now drawing as well as a collision point with the object it hits
Ray callbacks do return the specific object
SpaceBodySpawner class create for asteroids and planetoids
Sudo tweening in menu scene is implemented though hard coded.

***Issues with ImGui, I was about to start on making the ImGui panel for sound test but it is no longer 
responding to mouse clicks for some reason. I have researched online and most articles are saying its
due the the window not being focused. I did try to manually call ImGui::SetWindowFocus() and still no 
results. I even tried using a separate scene to initialize a test checkbox and that did not work either.
It does still respond to mouse over events but aside from that nothing. WIll have to see you
about this when Im in class.


Well I'm going to sleep like the dead for forever and a day now.


Cheers,


********************************Controls********************************************

'W' Move the Player *Edit: Accelarate forward
'Mouse Movement' Push forward for down pull back for up; Move Left - bank left/Move Right - bank right
'S' *Edit: Activate Debug Draw
'C' Capture Mouse for mouse movement/Set Player and follow camera rotation back to 0;
'Left Mouse Button' to navigate menus and shoot ray for points
'Enter' to Pass the Splash Screen
'Tab' will bring you back to main menu from game

Thanks Jimmy,
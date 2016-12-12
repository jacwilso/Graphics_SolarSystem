CSCI441, Computer Graphics, Fall 2016

Guild Name: Galactic Homeboiz

Guild Members
Jacob Wilson (jacwilso@mines.edu) / Shuhei Son of Atlas
William Kelly (wikelly@mines.edu) / Mr. Wiggles
Alan Champion (achampio@mines.edu) / Tegel Slimthot (Submitted source code)

Final Project (FP)

Description: A program that shows the whole universe!! ...or at least our solar system. We have the planets (which means no Pluto) rotating around the sun. 

We allow the user to move between the planets and to interact in some fun easter eggs. 
The solar system is hierarchical - the system draws the planets who draw particles (if you can find out how to make them do so...). 
There are multiple texture application (every planet + sky box)
The sun is a light and the camera is a light (Go close to the backside of a planet to see it). There are also materials for the planets and sun. 
There are quite a bunch of shaders. We have a vertex shader that is texture mapping the sun to make it bumpy. The planets have shaders for lighting and using multiple lights. The particles have a shader that makes them shrink as they live. 
We have sound, it just doesn't work on the school computers. 

Usage:  run gmake, then run the executable like so:
        ./keyToTheKingdom

    Keys:
        Camera: 
            Number key 1: Arc around sun
            2: Arc around Mercury
            3: Arc around Venus
            4: Arc around Earth
            5: Arc around Mars
            6: Arc around Jupiter
            7: Arc around Saturn
            8: Arc around Uranus
            9: Arc around Neptune
            '`': Switch between planets in order

        Lighting
            Space: Turns off l

        Planets
            l: Toggle trail for planet that is current focus
            n: Turns on trail for all planets
            m: Turns off trail for all planets
            f: Set current planet on fire
            d: Set all planets on fire
            p: Turn on easter egg

    Mouse: 
        Left click and drag: Arc ball around current focus
        Ctrl, left click and drag: Zoom in/out

Known bugs: Sometimes if you zoom to quickly, it will go all black but can be fixed by wiggling camera. 
    Shaders do not compile on some personal equipment. 
    Sound does not work on the school computers. 

Distribution of Work:
    Will 33.3%: Planets orbiting, trails and particles, lighting shaders, camera tracking. 
    Alan 33.3%: Planet and sun shaders (bump map and some lighting), texturing, lighting, p easter egg.
    Jacob 33.3%: Setup base classes (hierarchical model), Camera smooth transition, skybox, planet rotation, sound.

Time: ~30 hours

Labs: 8 - The labs were helpful enough. Especially with lighting and texturing. 

Fun: 9 - This was really fun. The requirements were not hard enough to get in the way of enjoyment. 
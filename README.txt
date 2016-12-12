CSCI441, Computer Graphics, Fall 2016

Guild Name: Galactic Homeboiz

Guild Members
William Kelly (wikelly@mines.edu) / Mr. Wiggles
Alan Champion (achampio@mines.edu) / Tegel Slimthot (Submitted source code)

Midterm Project (MP)

Description: A program that displays the two hero's in the guild. One is controllable and the other two are racing in a path. 
    The hero's move along a Bezier patch and they remain standing normal to the surface. There are multiple cameras (free cam, overhead cam, and arc cam for every hero).
    There are menus (and sub-menus) to select cameras and display the curves of the familiars. 


Usage:  run gmake, then run the guildWars executable like so:
        ./guildWars [csv file]

        For my control points, simply run 
        ./guildWars trainingGrounds.csv
        ./guildWars mountainRegion.csv

        The way to create a world file is below.

    Keys:
        Movement: (in hero orientation)
            w: go forward 
            s: go backwards
            d: turn right
            a: turn left
        Movement: (in free cam)
            w: go forward 
            s: go backwards

        Camera Movement: (in hero orientation)
            left click + drag: change perspective (rotate around the hero)
            ctrl + left click + drag: change zoom on hero
        Camera Movement: (in free cam)
            left click + drag: change what you are looking at

        Picture-In-Picture controls
            1: First-person view
            2: Sky cam view (top-down)
            0: disable picture-in-picture

Known bugs: Occasionally the wandering hero will spaz out a little bit
    The path for the heros on the track does not tilt to make ramps very well
    The fps display refuses to show up

Format for World File CSV
    [Bezier Surface Count (only works with one currently)]
    [The 16 points for the surface, each point on new line and formatted as "x, y, z"]
    [Bezier Curve Point Count]
    [The points for the curve, each point on new line and formatted as "x, y, z"]
    [Environmental Pieces Count (i.e. how many trees and buildings)]
    [[Type of environmental piece (tree or building)]
    [x and z position formated in "x, z". The y will be determined from the bezier surface]
    [Rotation in the x, y, and z formated as "x, y, z"]
    [Scaling factor]]

Distribution of Work:
    Will %55(time: %50): Bezier Patch, Bezier Curve, arc length parameterization, terrain adjustment, camera
    Alan %45(time: %50): Hero drawing, Bezier Curve drawing, Environment drawing, Terrain coloring, camera

Time: ~40 hours

Lab Help: 7 - The labs were alright. The only thing that we felt bad about were the bezier surface and the text

Fun: 6 - Hard to have fun when panicking
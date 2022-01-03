# Xspec
Fish-eye lens curved perspective
--------------------------------
This program draws 3-D wireframe graphics displayed in curved perspective.

## What is curved perspective?
Perspective drawings are calculated by projecting points from a 3 dimensional scene onto a 2 dimensional plane between the scene and the eye. This produces realistic looking images as long as the scene doesn't get too wide, at which point things get distorted. Imagine standing in a subway station, looking across the tracks. At the far left and right or your field of vision, the tracks recede off into the distance, and in a true image of the scene should converge to two points on the resulting picture plane. Likewise if you were leaning on a balcony rail, looking back at a tall skyscraper, the top and bottom of your field of view would converge at the zenith and nadir. To show all these points on the picture plane, we use a spherical plane surrounding the eye. Then if you had a spherical canvas you would be done. It would only look right from one point, of course, so maybe you could build it into your VR goggles. But more likely the resulting image would need to be flattened onto a normal picture plane, which here is done using an azimuthal projection, like the United Nations globe logo.

## Version in C
Using X Window and Motif libraries. Has a drawing window and a control panel in a separate window with sliders for eye position and direction, and field of view. So you have full range of freedom and can move around and view the shapes like an awkward video game.
Reads data from its own *.xsp text files.

## Version in Python
Uses Pygame library, and data from Python data files.
Assumes the eye is on the negative z axis (eg. 0,0,1,) looking directly at the origin. The images rotate around the origin.


![screenshot](https://github.com/bitslayer42/Xspec/blob/main/Xspec_lg.JPG)
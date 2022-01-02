
import math
import random

def normalize(x,y,z):
    magnitude = math.sqrt(x**2 + y**2 + z**2)
    return [x/magnitude,y/magnitude,z/magnitude]
numpoints = 500
distance = 0.3
points = []
x = range(numpoints)
for n in x:
    x = random.random() * 2 - 1
    y = random.random() * 2 - 1
    z = random.random() * 2 - 1
    [x,y,z] = normalize(x,y,z)
    points.append([[x],[y],[z]])

z_distance = 1.5
drawdots = True

# only connect lines that are close enough to each other
lines = []
for pix, p in enumerate(points):
    for qix, q in enumerate(points):
        if not pix==qix:
            dist = math.sqrt((p[0][0] - q[0][0])**2 + (p[1][0] - q[1][0])**2 + (p[2][0] - q[2][0])**2)
            if dist <= distance:
                lines.append([pix,qix])
            


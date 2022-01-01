
import math
import random

areasize = 10
numpoints = 600
distance = 1.5
points = []
x = range(numpoints)
for n in x:
    x = random.random() * areasize - (areasize//2)
    y = random.random() * areasize - (areasize//2)
    z = random.random() * areasize - (areasize//2)
    points.append([[x],[y],[z]])


# only connect lines that are close enough to each other
lines = []
for pix, p in enumerate(points):
    for qix, q in enumerate(points):
        if not pix==qix:
            dist = math.sqrt((p[0][0] - q[0][0])**2 + (p[1][0] - q[1][0])**2 + (p[2][0] - q[2][0])**2)
            if dist <= distance:
                lines.append([pix,qix])
            


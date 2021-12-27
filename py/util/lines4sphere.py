# move out of util dir to run
import math
from spheredata import points

listy = []
for pix, p in enumerate(points):
    for qix, q in enumerate(points):
        if not pix==qix:
            dist = math.sqrt((p[0][0] - q[0][0])**2 + (p[1][0] - q[1][0])**2 + (p[2][0] - q[2][0])**2)
            if dist <= 0.62:
                print(pix,qix)
            listy.append(dist)
listy.sort()
# print(listy)

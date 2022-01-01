# I used this to create the box plane: mirrors array of boxes
# move out of util dir to run
from data.boxplane import points,lines
import json

newp = []
for point in points:
    newp.append([[point[0][0]*-1],[point[1][0]],[point[2][0]]])
points.extend(newp)

print('points = ', end = '')
print(json.dumps(points,indent=4))

newl = []
for line in lines:
    newl.append([line[0]+2400,line[1]+2400])
lines.extend(newl)

print('lines = ', end = '')
print(json.dumps(lines,indent=4))
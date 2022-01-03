z_distance = 15
drawdots = False

points = []
l = 60 # number of boxes across in grid x,y
mingrid = l // -2 + 1
maxgrid = l // 2 + 1

for i in range(mingrid,maxgrid):
    for j in range(mingrid,maxgrid):
        points.append([[i], [j], [0]])

lines = []
for i in range(0,l):
    for j in range(0,l-1):
        lines.append([i*l + j,i*l + j + 1])
for i in range(0,l-1):
    for j in range(0,l):
        lines.append([i*l + j,i*l + j + l])

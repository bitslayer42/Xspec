points = []
l = 15 # number of boxes across in grid x,y,and z
mingrid = l // -2 + 1
maxgrid = l // 2 + 1

for i in range(mingrid,maxgrid):
    for j in range(mingrid,maxgrid):
        for k in range(mingrid,maxgrid):
            points.append([[i], [j], [k]])

lines = []
for i in range(0,l):
    for j in range(0,l):
        for k in range(0,l-1):
            lines.append([k + l*j + l*l*i, k + l*j + l*l*i+1])
for i in range(0,l):
    for j in range(0,l-1):
        for k in range(0,l):
            lines.append([k + l*j + l*l*i, k + l*j + l*l*i + l])
for i in range(0,l-1):
    for j in range(0,l):
        for k in range(0,l):
            lines.append([k + l*j + l*l*i, k + l*j + l*l*i + l*l])            
# for m in range(len(points)):
#     connect_point(m, random.randint(0, len(points)-1))


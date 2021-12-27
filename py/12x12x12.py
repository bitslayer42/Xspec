import pygame
import os
import math
import random
from matrix import matrix_multiplication
from math import acos,atan2,sqrt,cos,sin
fieldOfView = 0.63
# from icosphere import points

os.environ["SDL_VIDEO_CENTERED"]='1'  # Center window
black, white, blue  = (0, 0, 0), (230, 230, 230), (0, 154, 255)
scr_width, scr_height = 1680, 1050

pygame.init()
pygame.display.set_caption("Xspec")
screen = pygame.display.set_mode((scr_width, scr_height))
clock = pygame.time.Clock()
fps = 60

anglex = 0
angley = 0
anglez = 0
speedx = 0.04
speedy = 0.02
speedz = 0.01
scr_center = [scr_width//2, scr_height//2]
scale = 800
speed = 0.01
points = []
l = 12 # number of points across in grid x,y,and z
mingrid = l // -2 + 1
maxgrid = l // 2 + 1

for i in range(mingrid,maxgrid):
    for j in range(mingrid,maxgrid):
        for k in range(mingrid,maxgrid):
            points.append([[i], [j], [k]])



def connect_point(i, j, projected_points):
    a = projected_points[i]
    b = projected_points[j]
    pygame.draw.line(screen, black, (a[0], a[1]), (b[0], b[1]), 1)

def getCameraCoords(x,y,z):
    if x==y==z==0:
        return [0,0]
    x1 = (acos(-(z / sqrt(x**2 + y**2 + z**2)))) * fieldOfView * cos( atan2(y,x) )
    y1 = (acos(-(z / sqrt(x**2 + y**2 + z**2)))) * fieldOfView * sin( atan2(y,x) )
    return [x1,y1]

run = True
while run:
    clock.tick(fps)
    screen.fill(white)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False

    index = 0
    projected_points = [j for j in range(len(points))]

    rotation_x = [[1, 0, 0],
                  [0, math.cos(anglex), -math.sin(anglex)],
                  [0, math.sin(anglex), math.cos(anglex)]]

    rotation_y = [[math.cos(angley), 0, -math.sin(angley)],
                  [0, 1, 0],
                  [math.sin(angley), 0, math.cos(angley)]]

    rotation_z = [[math.cos(anglez), -math.sin(anglez), 0],
                  [math.sin(anglez), math.cos(anglez), 0],
                  [0, 0 ,1]]

    for point in points:
        # rotate shape in 3d
        rotated_2d = matrix_multiplication(rotation_x, point)
        rotated_2d = matrix_multiplication(rotation_y, rotated_2d)
        rotated_2d = matrix_multiplication(rotation_z, rotated_2d)
        
        # project to 2d
        projected_2d = getCameraCoords(rotated_2d[0][0],rotated_2d[1][0],rotated_2d[2][0]-l)
        x = int(projected_2d[0] * scale) + scr_center[0]
        y = int(projected_2d[1] * scale) + scr_center[1]

        projected_points[index] = [x, y]
        pygame.draw.circle(screen, black, (x, y), 3)
        index += 1
    #draw edges
    for i in range(0,l):
        for j in range(0,l):
            for k in range(0,l-1):
                connect_point(k + l*j + l*l*i, k + l*j + l*l*i+1, projected_points)
    for i in range(0,l):
        for j in range(0,l-1):
            for k in range(0,l):
                connect_point(k + l*j + l*l*i, k + l*j + l*l*i + l, projected_points)
    for i in range(0,l-1):
        for j in range(0,l):
            for k in range(0,l):
                connect_point(k + l*j + l*l*i, k + l*j + l*l*i + l*l, projected_points)            
    # for m in range(len(points)):
    #     connect_point(m, random.randint(0, len(points)-1), projected_points)
    

    anglex += speedx
    angley += speedy
    anglez += speedz
    pygame.display.update()

pygame.quit()

import sys
import os
os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = '1'
import pygame
import math
import random
from lib.matrix import matrix_multiplication
from math import acos,atan2,sqrt,cos,sin
import importlib
from os.path import dirname, basename, isfile, join
import glob
from lib.blit_text import blit_text

fieldOfView = 0.63
if len(sys.argv) >= 2:
    shape = importlib.import_module("data." + sys.argv[1])
    print(shape)
else:
    datadir = os.getcwd() + "/data"
    modules = glob.glob(join(datadir, "*.py"))
    __all__ = [ basename(f)[:-3] for f in modules if isfile(f) and not f.endswith('__init__.py')]
    print ("Add a shape as command line parameter. One of:")
    print(__all__)
    quit()
points, lines = shape.points, shape.lines
try: shape.z_distance
except AttributeError: z_distance = 10
else: z_distance = shape.z_distance

#os.environ["SDL_VIDEO_CENTERED"]='1'  # Center window
black, white, blue  = (0, 0, 0), (230, 230, 230), (0, 154, 255)
scr_width, scr_height = 1680, 1050
# scr_width, scr_height = 3000, 2000 
pygame.init()
pygame.display.set_caption("Xspec")
screen = pygame.display.set_mode((scr_width, scr_height))
clock = pygame.time.Clock()
fps = 60
font = pygame.font.SysFont('Arial', 32)

anglex = 0
angley = 0
anglez = 0
speedx = 0
speedy = 0
speedz = 0
#z_distance = defaults["z_distance"]
scr_center = [scr_width//2, scr_height//2]
scale = scr_height * 2 // 3
speedup = 0.01

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
hidetext = False
while run:
    clock.tick(fps)
    screen.fill(white)
    if not hidetext:
        text = 'Use the arrow keys or WASD to rotate shape.' \
        + '\nZ to zoom in, X to zoom out.' \
        + '\nH to hide this text.' \
        + '\nESC to quit.' \
        + '\n' \
        + '\nX Speed: ' + str(speedx) \
        + '\nY Speed: ' + str(speedy) \
        + '\nZ Distance: ' + str(z_distance)

        blit_text(screen, text, (20,20), font)

    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT or event.key == ord('a'):
                speedy = speedy + speedup
            if event.key == pygame.K_RIGHT or event.key == ord('d'):
                speedy = speedy - speedup
            if event.key == pygame.K_UP or event.key == ord('w'):
                speedx = speedx + speedup
            if event.key == pygame.K_DOWN or event.key == ord('s'):
                speedx = speedx - speedup
            if event.key == ord('z'):
                z_distance = z_distance - 0.9
            if event.key == ord('x'):
                z_distance = z_distance + 0.9
            if event.key == ord('h'):
                hidetext = True
        if event.type == pygame.KEYUP:
            if event.key == ord('q') or event.key == pygame.K_ESCAPE:
                pygame.quit()
                sys.exit()
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
        projected_2d = getCameraCoords(rotated_2d[0][0],rotated_2d[1][0],rotated_2d[2][0]-z_distance)
        x = int(projected_2d[0] * scale) + scr_center[0]
        y = int(projected_2d[1] * scale) + scr_center[1]

        projected_points[index] = [x, y]
        # pygame.draw.circle(screen, black, (x, y), 3)
        index += 1
    #draw edges       
    for line in lines:
        connect_point(line[0], line[1], projected_points)
    

    anglex += speedx
    angley += speedy
    anglez += speedz
    pygame.display.update()

pygame.quit()

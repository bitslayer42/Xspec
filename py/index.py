#new working version that outsources a lot
import sys
import os
os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = '1'
import pygame
import importlib
from os.path import dirname, basename, isfile, join
import glob
from lib.blit_text import blit_text
from lib.calc_points import calc_points
from lib.load_shape import load_shape

fieldOfView = 0.63
if len(sys.argv) >= 2:
    [ points, lines, z_distance, drawdots ] = load_shape(sys.argv[1])
else:  # no cli param provided
    datadir = os.getcwd() + "/data"
    modules = glob.glob(join(datadir, "*.py"))
    filelist = [ basename(f)[:-3] for f in modules if isfile(f) and not f.endswith('__init__.py')]
    print ("Add a shape as command line parameter. One of:")
    print(filelist)
    quit()

#os.environ["SDL_VIDEO_CENTERED"]='1'  # Center window
black, white, blue  = (0, 0, 0), (230, 230, 230), (0, 154, 255)
# scr_width, scr_height = 1680, 1050
scr_width, scr_height = 1000, 800 
pygame.init()
pygame.display.set_caption("Xspec")
screen = pygame.display.set_mode((scr_width, scr_height))
clock = pygame.time.Clock()
fps = 60
font = pygame.font.SysFont('Arial', 14)

anglex = 0;angley = 0;anglez = 0
speedx = 0;speedy = 0;speedz = 0
#z_distance = defaults["z_distance"]
scr_center = [scr_width//2, scr_height//2]
scale = scr_height * 2 // 3
speedup = 0.01
text = 'Use the arrow keys or WASD to rotate shape.' \
+ '\nZ to zoom in, X to zoom out.' \
+ '\nH to hide this text.' \
+ '\nESC to quit.' \
+ '\n' \
+ '\nX Speed: {}' \
+ '\nY Speed: {}' \
+ '\nZ Distance: {}'

run = True
hidetext = False
while run:
    clock.tick(fps)
    screen.fill(white)
    if not hidetext:
        ftext = text.format(str(speedx),str(speedy),str(z_distance))
        blit_text(screen, ftext, (20,20), font)

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

    anglex += speedx
    angley += speedy
    anglez += speedz

    calc_points(screen,points,lines,anglex,angley,anglez,z_distance,drawdots,fieldOfView)
    
    pygame.display.update()

pygame.quit()

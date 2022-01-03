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


datadir = os.getcwd() + "/data"
modules = glob.glob(join(datadir, "*.py"))
filelist = [ basename(f)[:-3] for f in modules if isfile(f) and not f.endswith('__init__.py')]
print ("Add a shape as command line parameter. One of:")
print(filelist)

#os.environ["SDL_VIDEO_CENTERED"]='1'  # Center window
black, white, blue  = (0, 0, 0), (230, 230, 230), (0, 154, 255)
scr_width, scr_height = 1680, 1050
# scr_width, scr_height = 3000, 2000 
pygame.init()
pygame.display.set_caption("Xspec")
screen = pygame.display.set_mode((scr_width, scr_height))
font = pygame.font.SysFont('Arial', 24)

screen.fill(white)

text = 'Enter a number to choose a file to load.'
for index, filenm in enumerate(filelist, start=1):
    text = text + '\n' + str(index) + ': ' + filenm
blit_text(screen, text, (20,20), font)

run = True
while run:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            print (chr(event.key))
        if event.type == pygame.KEYUP:
            if event.key == ord('q') or event.key == pygame.K_ESCAPE:
                pygame.quit()
                sys.exit()

    pygame.display.update()

pygame.quit()

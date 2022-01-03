import sys
import os
os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = '1'
import pygame
from os.path import dirname, basename, isfile, join
import glob
import pygame_widgets
from pygame_widgets.button import Button
from loadshape import loadshape

datadir = os.getcwd() + "/data"
modules = glob.glob(join(datadir, "*.py"))
filelist = [ basename(f)[:-3] for f in modules if isfile(f) and not f.endswith('__init__.py')]

#os.environ["SDL_VIDEO_CENTERED"]='1'  # Center window
black, white, blue  = (0, 0, 0), (230, 230, 230), (0, 154, 255)
scr_width, scr_height = 1680, 1050
# scr_width, scr_height = 3000, 2000 
pygame.init()
pygame.display.set_caption("Xspec")
screen = pygame.display.set_mode((scr_width, scr_height))
font = pygame.font.SysFont('Arial', 14)

screen.fill(white)

text = 'Enter a number to choose a file to load.'
for ix, filenm in enumerate(filelist):
    Button(
        screen,10,35*ix,180,30,text=filenm,
        onClick=loadshape,
        onClickParams=[filenm])

run = True
while run:
    events = pygame.event.get()
    for event in events:
        if event.type == pygame.KEYDOWN:
            print (chr(event.key))
        if event.type == pygame.KEYUP:
            if event.key == ord('q') or event.key == pygame.K_ESCAPE:
                pygame.quit()
                sys.exit()
    pygame_widgets.update(events)
    pygame.display.update()

pygame.quit()
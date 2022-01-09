import os
import glob
from os.path import dirname, basename, isfile, join
import pygame
import pygame_widgets
from pygame_widgets.button import Button
from lib.load_shape import load_shape
from lib.connect_point import connect_point
from lib.get_camera_coords import get_camera_coords

datadir = os.getcwd() + "/data"
modules = glob.glob(join(datadir, "*.py"))
filelist = [ basename(f)[:-3] for f in modules if isfile(f) and not f.endswith('__init__.py')]

black, white, blue  = (0, 0, 0), (230, 230, 230), (0, 154, 255)
scr_width, scr_height = 1680, 1050

pygame.init()
pygame.display.set_caption("Xspec")
screen = pygame.display.set_mode((scr_width, scr_height))
clock = pygame.time.Clock()
fps = 60
font = pygame.font.SysFont('Arial', 14)

screen.fill(white)
for ix, filenm in enumerate(filelist):
    Button(
        screen,10,35*ix+200,180,30,text=filenm,
        onClick=load_shape,
        onClickParams=[filenm, screen])
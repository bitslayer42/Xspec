import os
os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = '1'
from lib.matrix import matrix_multiplication
import importlib
from os.path import dirname, basename, isfile, join
from lib.blit_text import blit_text
import pygame



def load_shape(filenm):
    shape = importlib.import_module("data." + filenm)
    points, lines = shape.points, shape.lines
    try: shape.z_distance
    except AttributeError: z_distance = 10
    else: z_distance = shape.z_distance
    try: shape.drawdots
    except AttributeError: drawdots = False
    else: drawdots = shape.drawdots
    #return [ points, lines, z_distance, drawdots ]
    
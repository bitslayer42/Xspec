import importlib

def load_shape(filenm):
    shape = importlib.import_module("data." + filenm)
    points, lines = shape.points, shape.lines
    try: shape.z_distance
    except AttributeError: z_distance = 10
    else: z_distance = shape.z_distance
    try: shape.drawdots
    except AttributeError: drawdots = False
    else: drawdots = shape.drawdots
    return [ points, lines, z_distance, drawdots ]
    
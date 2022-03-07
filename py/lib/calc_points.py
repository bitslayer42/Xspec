import math
import pygame
from lib.matrix import matrix_multiplication
from lib.get_camera_coords import get_camera_coords

def calc_points(screen,points,lines,anglex,angley,anglez,z_distance,drawdots,fieldOfView):
    index = 0
    scr_width, scr_height = screen.get_size()
    scr_center = [scr_width//2, scr_height//2]
    scale = scr_height * 2 // 3

    projected_points = []

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
        projected_2d = get_camera_coords(rotated_2d[0][0],rotated_2d[1][0],rotated_2d[2][0]-z_distance,fieldOfView)
        x = int(projected_2d[0] * scale) + scr_center[0]
        y = int(projected_2d[1] * scale) + scr_center[1]

        projected_points.append([x, y])
        if drawdots:
            pygame.draw.circle(screen, (0, 0, 0), (x, y), 3)
        index += 1
    #draw edges       
    for line in lines:
        pygame.draw.line(screen, (0, 0, 0), (projected_points[line[0]]), (projected_points[line[1]]), 1)

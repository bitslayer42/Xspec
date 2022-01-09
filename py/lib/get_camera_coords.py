from math import acos,atan2,sqrt,cos,sin

def get_camera_coords(x, y, z, fieldOfView):
    if x==y==z==0:
        return [0,0]
    x1 = (acos(-(z / sqrt(x**2 + y**2 + z**2)))) * fieldOfView * cos( atan2(y,x) )
    y1 = (acos(-(z / sqrt(x**2 + y**2 + z**2)))) * fieldOfView * sin( atan2(y,x) )
    return [x1,y1]    
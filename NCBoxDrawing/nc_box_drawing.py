#!/usr/bin/python3

from termbox import WHITE,DEFAULT,Termbox,EVENT_KEY

BOX_HORIZONTAL = 9472
BOX_VERTICAL = 9474
BOX_CORNER_TL = 9484
BOX_CORNER_TR = 9488
BOX_CORNER_BL = 9492
BOX_CORNER_BR = 9496
BOX_VERTICAL_HORIZONTAL_RIGHT = 9500
BOX_VERTICAL_HORIZONTAL_LEFT = 9508
BOX_HORIZONTAL_VERTICAL_TOP = 9524
BOX_HORIZONTAL_VERTICAL_BOTTOM = 9516
BOX_VERTICAL_HORIZONTAL = 9532

def draw_vertical(tb,x,y1,y2):
    for y in range(y1,y2):
        tb.change_cell(x,y,BOX_VERTICAL,WHITE,DEFAULT)


def draw_horizontal(tb,x1,x2,y):
    for x in range(x1,x2):
        tb.change_cell(x,y,BOX_HORIZONTAL,WHITE,DEFAULT)

def draw_char(tb,x,y,char):
    tb.change_cell(x,y,char,WHITE,DEFAULT)


class cell(object):

    def __init__(self,vertex=False,top=False,bottom=False,left=False,right=False):
        self.vertex = vertex
        self.top = top
        self.bottom = bottom
        self.left = left
        self.right = right


def draw_ui_boxes(tb,vertices):
    width = tb.width()
    height = tb.height()
    matrix = [[None for y in range(height)] for x in range(width)]
    for x in range(width):
        for y in range(height):
            matrix[x][y] = cell()
    for x,y in vertices: matrix[x][y].vertex = True
    for (vx,vy) in vertices:
        if vy > -1 and not matrix[vx][vy].top:
            y = vy - 1
            while y > -1:
                if matrix[vx][y].vertex:
                    matrix[vx][y].bottom = True
                    draw_vertical(tb,vx,vy - 1,y)
                    matrix[vx][vy].top = True
                    break
                y -= 1
        if vy < height and not matrix[vx][vy].bottom:
            y = vy + 1
            while y < height:
                if matrix[vx][y].vertex:
                    matrix[vx][y].top = True
                    draw_vertical(tb,vx,vy + 1,y)
                    matrix[vx][vy].bottom = True
                    break
                y += 1
        if vx > -1 and not matrix[vx][vy].left:
            x = vx - 1
            while x > -1:
                if matrix[x][vy].vertex:
                    matrix[x][vy].right = True
                    draw_horizontal(tb,vx - 1,x,vy)
                    matrix[vx][vy].left = True
                    break
                x -= 1
        if vx < width and not matrix[vx][vy].right:
            x = vx + 1
            while x < width:
                if matrix[x][vy].vertex:
                    matrix[x][vy].left = True
                    draw_horizontal(tb,vx + 1,x,vy)
                    matrix[vx][vy].right = True
                    break
                x += 1
    for (x,y) in vertices:
        if matrix[x][y].bottom and matrix[x][y].right and not matrix[x][y].top and not matrix[x][y].left: draw_char(tb,x,y,BOX_CORNER_TL)
        elif matrix[x][y].bottom and not matrix[x][y].right and not matrix[x][y].top and matrix[x][y].left: draw_char(tb,x,y,BOX_CORNER_TR)
        elif not matrix[x][y].bottom and matrix[x][y].right and matrix[x][y].top and not matrix[x][y].left: draw_char(tb,x,y,BOX_CORNER_BL)
        elif not matrix[x][y].bottom and not matrix[x][y].right and matrix[x][y].top and matrix[x][y].left: draw_char(tb,x,y,BOX_CORNER_BR)
        elif matrix[x][y].bottom and matrix[x][y].right and matrix[x][y].top and not matrix[x][y].left: draw_char(tb,x,y,BOX_VERTICAL_HORIZONTAL_RIGHT)
        elif matrix[x][y].bottom and not matrix[x][y].right and matrix[x][y].top and matrix[x][y].left: draw_char(tb,x,y,BOX_VERTICAL_HORIZONTAL_LEFT)
        elif not matrix[x][y].bottom and matrix[x][y].right and matrix[x][y].top and matrix[x][y].left: draw_char(tb,x,y,BOX_HORIZONTAL_VERTICAL_TOP)
        elif matrix[x][y].bottom and matrix[x][y].right and not matrix[x][y].top and matrix[x][y].left: draw_char(tb,x,y,BOX_HORIZONTAL_VERTICAL_BOTTOM)
        elif matrix[x][y].bottom and matrix[x][y].right and matrix[x][y].top and matrix[x][y].left: draw_char(tb,x,y,BOX_VERTICAL_HORIZONTAL)
        elif not matrix[x][y].right and not matrix[x][y].left: draw_char(tb,x,y,BOX_VERTICAL)
        elif not matrix[x][y].top and not matrix[x][y].bottom: draw_char(tb,x,y,BOX_HORIZONTAL)


def draw(tb):
    w = tb.width() - 1
    h = tb.height() - 1
    draw_ui_boxes(tb,[(0,0),(w // 3,0),(w,0),(0,2),(w // 3,2),(0,4),(w // 3,4),(0,6),(w // 3,6),(w // 3,h - 2),(w,h - 2),(0,h),(w // 3,h),(w,h)])
    tb.present()

with Termbox() as tb:
    exit_pressed = False
    draw(tb)

    while not exit_pressed:
        tb.clear()
        draw(tb)
        event = tb.peek_event(500)
        if event:
            event_type, character, key = event[:3]
            if event_type is EVENT_KEY:
                exit_pressed = True

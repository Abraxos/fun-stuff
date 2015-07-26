#NCurses Box Drawing Algorithm

This is just a cute algorithm I came up with one afternoon for easily drawing NCurses GUI outlines. It's made mostly to demonstrate the algorithm, it only draws the boxes in white and default and doesn't do things like dotted lines or double-thick lines.

The input is a set of points that can be connected by vertical and horizontal lines and the function draws all the points with all of the correct unicode box characters. For example:
```
w = tb.width() - 1
h = tb.height() - 1
draw_ui_boxes(tb,[(0,0),(w // 3,0),(w,0),(0,2),(w // 3,2),(0,4),(w // 3,4),(0,6),(w // 3,6),(w // 3,h - 2),(w,h - 2),(0,h),(w // 3,h),(w,h)])
```

Produces the following output:

![Drawn UI](/Result.png)

The algorithm works by determining the kinds of intersections that occur at every vertex and draws the lines between them first and then adds the pieces at each intersection. Again, it is fairly simple, I just figured that I could put up a demo of it as a reference for how to easily make NCurses UI Elements.

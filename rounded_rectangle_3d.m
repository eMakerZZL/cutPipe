close all;
clear all;
clc;
hold on;

segments = 180;

pipe.length = 100;
pipe.width  = 20;
pipe.height = 50;
pipe.radius = 5;

rotate_center = [10, 20, 20];

outter_rectangle.length = pipe.length; %Main Edge
outter_rectangle.width  = pipe.width;
outter_rectangle.radius = pipe.radius;

inner_rectangle.length = outter_rectangle.length / 2 - outter_rectangle.radius;
inner_rectangle.width  = outter_rectangle.width  / 2 - outter_rectangle.radius;

outter_points = [
        outter_rectangle.length / 2, outter_rectangle.width / 2, 0;%RU
       -outter_rectangle.length / 2, outter_rectangle.width / 2, 0;%LU
       -outter_rectangle.length / 2,-outter_rectangle.width / 2, 0;%LD
        outter_rectangle.length / 2,-outter_rectangle.width / 2, 0;%RD
];

outter_points_ = [
        outter_rectangle.length / 2, outter_rectangle.width / 2, pipe.height;%RU
       -outter_rectangle.length / 2, outter_rectangle.width / 2, pipe.height;%LU
       -outter_rectangle.length / 2,-outter_rectangle.width / 2, pipe.height;%LD
        outter_rectangle.length / 2,-outter_rectangle.width / 2, pipe.height;%RD
];

inner_points = [
        inner_rectangle.length,  inner_rectangle.width, 0; %RU
       -inner_rectangle.length,  inner_rectangle.width, 0; %LU
       -inner_rectangle.length, -inner_rectangle.width, 0; %LD
        inner_rectangle.length, -inner_rectangle.width, 0; %RD
];

inner_points_ = [
        inner_rectangle.length,  inner_rectangle.width, pipe.height; %RU
       -inner_rectangle.length,  inner_rectangle.width, pipe.height; %LU
       -inner_rectangle.length, -inner_rectangle.width, pipe.height; %LD
        inner_rectangle.length, -inner_rectangle.width, pipe.height; %RD
];

arc_points.right_up(:,1)   = inner_points(1,1) + outter_rectangle.radius * cos(linspace(0, pi/2, segments))';
arc_points.right_up(:,2)   = inner_points(1,2) + outter_rectangle.radius * sin(linspace(0, pi/2, segments))';
arc_points.right_up(:,3)   = zeros(segments,1);

arc_points.left_up(:,1)    = inner_points(2,1) + outter_rectangle.radius * cos(linspace(pi/2, pi, segments))';
arc_points.left_up(:,2)    = inner_points(2,2) + outter_rectangle.radius * sin(linspace(pi/2, pi, segments))';
arc_points.left_up(:,3)    = zeros(segments,1);

arc_points.left_down(:,1)  = inner_points(3,1) + outter_rectangle.radius * cos(linspace(pi, 3 * pi / 2, segments))';
arc_points.left_down(:,2)  = inner_points(3,2) + outter_rectangle.radius * sin(linspace(pi, 3 * pi / 2, segments))';
arc_points.left_down(:,3)  = zeros(segments,1);

arc_points.right_down(:,1) = inner_points(4,1) + outter_rectangle.radius * cos(linspace(3 * pi / 2, 2 * pi, segments))';
arc_points.right_down(:,2) = inner_points(4,2) + outter_rectangle.radius * sin(linspace(3 * pi / 2, 2 * pi, segments))';
arc_points.right_down(:,3) = zeros(segments,1);

arc_points_ = arc_points;
arc_points_.right_up(:,3)   = pipe.height * ones(segments,1);
arc_points_.left_up(:,3)    = pipe.height * ones(segments,1);
arc_points_.left_down(:,3)  = pipe.height * ones(segments,1);
arc_points_.right_down(:,3) = pipe.height * ones(segments,1);

rectangle_points = [
    inner_points(1,1), inner_points(1,2) + outter_rectangle.radius,inner_points(1,3);
    inner_points(2,1), inner_points(2,2) + outter_rectangle.radius,inner_points(2,3);
                                                                                  
    inner_points(2,1) - outter_rectangle.radius, inner_points(2,2),inner_points(2,3);
    inner_points(3,1) - outter_rectangle.radius, inner_points(3,2),inner_points(3,3);
                                                                                  
    inner_points(3,1), inner_points(3,2) - outter_rectangle.radius,inner_points(3,3);
    inner_points(4,1), inner_points(4,2) - outter_rectangle.radius,inner_points(4,3);
                                                                                  
    inner_points(4,1) + outter_rectangle.radius, inner_points(4,2),inner_points(4,3);
    inner_points(1,1) + outter_rectangle.radius, inner_points(1,2),inner_points(1,3);
];

rectangle_points_ = [
    inner_points_(1,1), inner_points_(1,2) + outter_rectangle.radius,inner_points_(1,3);
    inner_points_(2,1), inner_points_(2,2) + outter_rectangle.radius,inner_points_(2,3);
                                                                                  
    inner_points_(2,1) - outter_rectangle.radius, inner_points_(2,2),inner_points_(2,3);
    inner_points_(3,1) - outter_rectangle.radius, inner_points_(3,2),inner_points_(3,3);
                                                                                  
    inner_points_(3,1), inner_points_(3,2) - outter_rectangle.radius,inner_points_(3,3);
    inner_points_(4,1), inner_points_(4,2) - outter_rectangle.radius,inner_points_(4,3);
                                                                                  
    inner_points_(4,1) + outter_rectangle.radius, inner_points_(4,2),inner_points_(4,3);
    inner_points_(1,1) + outter_rectangle.radius, inner_points_(1,2),inner_points_(1,3);
];

axis equal;

color = 'b';
offset = 0;
handle_body(offset + 1) = plot3(rectangle_points(1:2,1), rectangle_points(1:2,2) , rectangle_points(1:2,3) , color);
handle_body(offset + 2) = plot3(rectangle_points(3:4,1), rectangle_points(3:4,2) , rectangle_points(3:4,3) , color);
handle_body(offset + 3) = plot3(rectangle_points(5:6,1), rectangle_points(5:6,2) , rectangle_points(5:6,3) , color);
handle_body(offset + 4) = plot3(rectangle_points(7:8,1), rectangle_points(7:8,2) , rectangle_points(7:8,3) , color);

offset = offset + 4;
handle_body(offset + 1) = plot3(rectangle_points_(1:2,1), rectangle_points_(1:2,2) , rectangle_points_(1:2,3) , color);
handle_body(offset + 2) = plot3(rectangle_points_(3:4,1), rectangle_points_(3:4,2) , rectangle_points_(3:4,3) , color);
handle_body(offset + 3) = plot3(rectangle_points_(5:6,1), rectangle_points_(5:6,2) , rectangle_points_(5:6,3) , color);
handle_body(offset + 4) = plot3(rectangle_points_(7:8,1), rectangle_points_(7:8,2) , rectangle_points_(7:8,3) , color);

offset = offset + 4;
handle_body(offset + 1) = plot3(arc_points.right_up(:,1),   arc_points.right_up(:,2)   , arc_points.right_up(:,3)   ,color);
handle_body(offset + 2) = plot3(arc_points.left_up(:,1),    arc_points.left_up(:,2)    , arc_points.left_up(:,3)    ,color);
handle_body(offset + 3) = plot3(arc_points.left_down(:,1),  arc_points.left_down(:,2)  , arc_points.left_down(:,3)  ,color);
handle_body(offset + 4) = plot3(arc_points.right_down(:,1), arc_points.right_down(:,2) , arc_points.right_down(:,3) ,color);

offset = offset + 4;
handle_body(offset + 1) = plot3(arc_points_.right_up(:,1),   arc_points_.right_up(:,2)   , arc_points_.right_up(:,3)   ,color);
handle_body(offset + 2) = plot3(arc_points_.left_up(:,1),    arc_points_.left_up(:,2)    , arc_points_.left_up(:,3)    ,color);
handle_body(offset + 3) = plot3(arc_points_.left_down(:,1),  arc_points_.left_down(:,2)  , arc_points_.left_down(:,3)  ,color);
handle_body(offset + 4) = plot3(arc_points_.right_down(:,1), arc_points_.right_down(:,2) , arc_points_.right_down(:,3) ,color);

offset = offset + 4;
handle_body(offset + 1) = plot3([rectangle_points(1,1),rectangle_points_(1,1)]', [rectangle_points(1,2),rectangle_points_(1,2)]', [rectangle_points(1,3),rectangle_points_(1,3)]', color);
handle_body(offset + 2) = plot3([rectangle_points(2,1),rectangle_points_(2,1)]', [rectangle_points(2,2),rectangle_points_(2,2)]', [rectangle_points(2,3),rectangle_points_(2,3)]', color);
handle_body(offset + 3) = plot3([rectangle_points(3,1),rectangle_points_(3,1)]', [rectangle_points(3,2),rectangle_points_(3,2)]', [rectangle_points(3,3),rectangle_points_(3,3)]', color);
handle_body(offset + 4) = plot3([rectangle_points(4,1),rectangle_points_(4,1)]', [rectangle_points(4,2),rectangle_points_(4,2)]', [rectangle_points(4,3),rectangle_points_(4,3)]', color);

offset = offset + 4;
handle_body(offset + 1) = plot3([rectangle_points(5,1),rectangle_points_(5,1)]', [rectangle_points(5,2),rectangle_points_(5,2)]', [rectangle_points(5,3),rectangle_points_(5,3)]', color);
handle_body(offset + 2) = plot3([rectangle_points(6,1),rectangle_points_(6,1)]', [rectangle_points(6,2),rectangle_points_(6,2)]', [rectangle_points(6,3),rectangle_points_(6,3)]', color);
handle_body(offset + 3) = plot3([rectangle_points(7,1),rectangle_points_(7,1)]', [rectangle_points(7,2),rectangle_points_(7,2)]', [rectangle_points(7,3),rectangle_points_(7,3)]', color);
handle_body(offset + 4) = plot3([rectangle_points(8,1),rectangle_points_(8,1)]', [rectangle_points(8,2),rectangle_points_(8,2)]', [rectangle_points(8,3),rectangle_points_(8,3)]', color);

offset = offset + 4;
handle_sum = offset;

rot_rectangle_points = rectangle_points;
rot_arc_points = arc_points;

rot_rectangle_points_ = rectangle_points_;
rot_arc_points_ = arc_points_;

rot_radians = -pi / 2 / segments;
for i = 1 : segments
    rot_rectangle_points      = rotate_coord3(rot_rectangle_points      , rot_radians);
    rot_arc_points.right_up   = rotate_coord3(rot_arc_points.right_up   , rot_radians);
    rot_arc_points.left_up    = rotate_coord3(rot_arc_points.left_up    , rot_radians);
    rot_arc_points.left_down  = rotate_coord3(rot_arc_points.left_down  , rot_radians);
    rot_arc_points.right_down = rotate_coord3(rot_arc_points.right_down , rot_radians);

    rot_rectangle_points_      = rotate_coord3(rot_rectangle_points      , rot_radians);
    rot_arc_points_.right_up   = rotate_coord3(rot_arc_points.right_up   , rot_radians);
    rot_arc_points_.left_up    = rotate_coord3(rot_arc_points.left_up    , rot_radians);
    rot_arc_points_.left_down  = rotate_coord3(rot_arc_points.left_down  , rot_radians);
    rot_arc_points_.right_down = rotate_coord3(rot_arc_points.right_down , rot_radians);

    set(handle_body(1 ) , 'xdata' , rot_rectangle_points(1:2     , 1) , 'ydata' , rot_rectangle_points(1:2     , 2) , 'zdata' , rot_rectangle_points(1:2     , 3));
    set(handle_body(2 ) , 'xdata' , rot_rectangle_points(3:4     , 1) , 'ydata' , rot_rectangle_points(3:4     , 2) , 'zdata' , rot_rectangle_points(3:4     , 3));
    set(handle_body(3 ) , 'xdata' , rot_rectangle_points(5:6     , 1) , 'ydata' , rot_rectangle_points(5:6     , 2) , 'zdata' , rot_rectangle_points(5:6     , 3));
    set(handle_body(4 ) , 'xdata' , rot_rectangle_points(7:8     , 1) , 'ydata' , rot_rectangle_points(7:8     , 2) , 'zdata' , rot_rectangle_points(7:8     , 3));
    set(handle_body(5 ) , 'xdata' , rot_rectangle_points_(1:2    , 1) , 'ydata' , rot_rectangle_points_(1:2    , 2) , 'zdata' , rot_rectangle_points_(1:2    , 3));
    set(handle_body(6 ) , 'xdata' , rot_rectangle_points_(3:4    , 1) , 'ydata' , rot_rectangle_points_(3:4    , 2) , 'zdata' , rot_rectangle_points_(3:4    , 3));
    set(handle_body(7 ) , 'xdata' , rot_rectangle_points_(5:6    , 1) , 'ydata' , rot_rectangle_points_(5:6    , 2) , 'zdata' , rot_rectangle_points_(5:6    , 3));
    set(handle_body(8 ) , 'xdata' , rot_rectangle_points_(7:8    , 1) , 'ydata' , rot_rectangle_points_(7:8    , 2) , 'zdata' , rot_rectangle_points_(7:8    , 3));
    set(handle_body(9 ) , 'xdata' , rot_arc_points.right_up(:    , 1) , 'ydata' , rot_arc_points.right_up(:    , 2) , 'zdata' , rot_arc_points.right_up(:    , 3));
    set(handle_body(10) , 'xdata' , rot_arc_points.left_up(:     , 1) , 'ydata' , rot_arc_points.left_up(:     , 2) , 'zdata' , rot_arc_points.left_up(:     , 3));
    set(handle_body(11) , 'xdata' , rot_arc_points.left_down(:   , 1) , 'ydata' , rot_arc_points.left_down(:   , 2) , 'zdata' , rot_arc_points.left_down(:   , 3));
    set(handle_body(12) , 'xdata' , rot_arc_points.right_down(:  , 1) , 'ydata' , rot_arc_points.right_down(:  , 2) , 'zdata' , rot_arc_points.right_down(:  , 3));
    set(handle_body(13) , 'xdata' , rot_arc_points_.right_up(:   , 1) , 'ydata' , rot_arc_points_.right_up(:   , 2) , 'zdata' , rot_arc_points_.right_up(:   , 3));
    set(handle_body(14) , 'xdata' , rot_arc_points_.left_up(:    , 1) , 'ydata' , rot_arc_points_.left_up(:    , 2) , 'zdata' , rot_arc_points_.left_up(:    , 3));
    set(handle_body(15) , 'xdata' , rot_arc_points_.left_down(:  , 1) , 'ydata' , rot_arc_points_.left_down(:  , 2) , 'zdata' , rot_arc_points_.left_down(:  , 3));
    set(handle_body(16) , 'xdata' , rot_arc_points_.right_down(: , 1) , 'ydata' , rot_arc_points_.right_down(: , 2) , 'zdata' , rot_arc_points_.right_down(: , 3));
    set(handle_body(17) , 'xdata' , [rot_rectangle_points(1,1),rot_rectangle_points_(1,1)]', 'ydata', [rot_rectangle_points(1,2),rot_rectangle_points_(1,2)]', 'zdata', [rot_rectangle_points(1,3),rot_rectangle_points_(1,3)]');
    set(handle_body(18) , 'xdata' , [rot_rectangle_points(2,1),rot_rectangle_points_(2,1)]', 'ydata', [rot_rectangle_points(2,2),rot_rectangle_points_(2,2)]', 'zdata', [rot_rectangle_points(2,3),rot_rectangle_points_(2,3)]');
    set(handle_body(19) , 'xdata' , [rot_rectangle_points(3,1),rot_rectangle_points_(3,1)]', 'ydata', [rot_rectangle_points(3,2),rot_rectangle_points_(3,2)]', 'zdata', [rot_rectangle_points(3,3),rot_rectangle_points_(3,3)]');
    set(handle_body(20) , 'xdata' , [rot_rectangle_points(4,1),rot_rectangle_points_(4,1)]', 'ydata', [rot_rectangle_points(4,2),rot_rectangle_points_(4,2)]', 'zdata', [rot_rectangle_points(4,3),rot_rectangle_points_(4,3)]');
    set(handle_body(21) , 'xdata' , [rot_rectangle_points(5,1),rot_rectangle_points_(5,1)]', 'ydata', [rot_rectangle_points(5,2),rot_rectangle_points_(5,2)]', 'zdata', [rot_rectangle_points(5,3),rot_rectangle_points_(5,3)]');
    set(handle_body(22) , 'xdata' , [rot_rectangle_points(6,1),rot_rectangle_points_(6,1)]', 'ydata', [rot_rectangle_points(6,2),rot_rectangle_points_(6,2)]', 'zdata', [rot_rectangle_points(6,3),rot_rectangle_points_(6,3)]');
    set(handle_body(23) , 'xdata' , [rot_rectangle_points(7,1),rot_rectangle_points_(7,1)]', 'ydata', [rot_rectangle_points(7,2),rot_rectangle_points_(7,2)]', 'zdata', [rot_rectangle_points(7,3),rot_rectangle_points_(7,3)]');
    set(handle_body(24) , 'xdata' , [rot_rectangle_points(8,1),rot_rectangle_points_(8,1)]', 'ydata', [rot_rectangle_points(8,2),rot_rectangle_points_(8,2)]', 'zdata', [rot_rectangle_points(8,3),rot_rectangle_points_(8,3)]');

    drawnow;
end

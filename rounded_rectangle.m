close all;
clear all;
clc;
hold on;

pipe.length = 30;
pipe.width  = 15;
pipe.height = 100;
pipe.radius = 5;

outter_rectangle.length = pipe.length; %Main Edge
outter_rectangle.width  = pipe.width;
outter_rectangle.radius = pipe.radius;
segments = 180;

inner_rectangle.length = outter_rectangle.length / 2 - outter_rectangle.radius;
inner_rectangle.width  = outter_rectangle.width  / 2 - outter_rectangle.radius;

outter_points = [
        outter_rectangle.length / 2, outter_rectangle.width / 2; %RU
       -outter_rectangle.length / 2, outter_rectangle.width / 2; %LU
       -outter_rectangle.length / 2,-outter_rectangle.width / 2; %LD
        outter_rectangle.length / 2,-outter_rectangle.width / 2; %RD
];

inner_points = [
        inner_rectangle.length,  inner_rectangle.width; %RU
       -inner_rectangle.length,  inner_rectangle.width; %LU
       -inner_rectangle.length, -inner_rectangle.width; %LD
        inner_rectangle.length, -inner_rectangle.width; %RD
];

arc_points.right_up(:,1)   = inner_points(1,1) + outter_rectangle.radius * cos(linspace(0, pi/2, segments))';
arc_points.right_up(:,2)   = inner_points(1,2) + outter_rectangle.radius * sin(linspace(0, pi/2, segments))';

arc_points.left_up(:,1)    = inner_points(2,1) + outter_rectangle.radius * cos(linspace(pi/2, pi, segments))';
arc_points.left_up(:,2)    = inner_points(2,2) + outter_rectangle.radius * sin(linspace(pi/2, pi, segments))';

arc_points.left_down(:,1)  = inner_points(3,1) + outter_rectangle.radius * cos(linspace(pi, 3 * pi / 2, segments))';
arc_points.left_down(:,2)  = inner_points(3,2) + outter_rectangle.radius * sin(linspace(pi, 3 * pi / 2, segments))';

arc_points.right_down(:,1) = inner_points(4,1) + outter_rectangle.radius * cos(linspace(3 * pi / 2, 2 * pi, segments))';
arc_points.right_down(:,2) = inner_points(4,2) + outter_rectangle.radius * sin(linspace(3 * pi / 2, 2 * pi, segments))';

rectangle_points = [
    inner_points(1,1), inner_points(1,2) + outter_rectangle.radius;
    inner_points(2,1), inner_points(2,2) + outter_rectangle.radius;

    inner_points(2,1) - outter_rectangle.radius, inner_points(2,2);
    inner_points(3,1) - outter_rectangle.radius, inner_points(3,2);

    inner_points(3,1), inner_points(3,2) - outter_rectangle.radius;
    inner_points(4,1), inner_points(4,2) - outter_rectangle.radius;

    inner_points(4,1) + outter_rectangle.radius, inner_points(4,2);
    inner_points(1,1) + outter_rectangle.radius, inner_points(1,2);
];

axis equal;
max_limits = max(outter_rectangle.length, outter_rectangle.width) + 5;
axis([-max_limits, max_limits, -max_limits, max_limits]);

draw_rounded_rectangle = @(rectangle_points, arc_points, color) {
    plot(rectangle_points(1:2,1), rectangle_points(1:2,2) , color);
    plot(rectangle_points(3:4,1), rectangle_points(3:4,2) , color);
    plot(rectangle_points(5:6,1), rectangle_points(5:6,2) , color);
    plot(rectangle_points(7:8,1), rectangle_points(7:8,2) , color);

    plot(arc_points.right_up(:,1),   arc_points.right_up(:,2)   , color);
    plot(arc_points.left_up(:,1),    arc_points.left_up(:,2)    , color);
    plot(arc_points.left_down(:,1),  arc_points.left_down(:,2)  , color);
    plot(arc_points.right_down(:,1), arc_points.right_down(:,2) , color);
};

draw_rounded_rectangle(rectangle_points, arc_points, 'b');

alpha = linspace(0, 2 * pi, segments);
circle.radius = sqrt(outter_points(1,1)^2 + outter_points(1,2)^2);
circle.x = circle.radius * cos(alpha);
circle.y = circle.radius * sin(alpha);
plot(circle.x, circle.y, 'm', 'LineWidth', 3);

rot_rectangle_points = rectangle_points;
rot_arc_points = arc_points;

handle_up    = plot(rot_rectangle_points(1:2,1), rot_rectangle_points(1:2,2) , 'r');
handle_left  = plot(rot_rectangle_points(3:4,1), rot_rectangle_points(3:4,2) , 'r');
handle_down  = plot(rot_rectangle_points(5:6,1), rot_rectangle_points(5:6,2) , 'r');
handle_right = plot(rot_rectangle_points(7:8,1), rot_rectangle_points(7:8,2) , 'r');

handle_right_up   = plot(rot_arc_points.right_up(:,1)  , rot_arc_points.right_up(:,2)   , 'r');
handle_left_up    = plot(rot_arc_points.left_up(:,1)   , rot_arc_points.left_up(:,2)    , 'r');
handle_left_down  = plot(rot_arc_points.left_down(:,1) , rot_arc_points.left_down(:,2)  , 'r');
handle_right_down = plot(rot_arc_points.right_down(:,1), rot_arc_points.right_down(:,2) , 'r');

laser.trail_1 = zeros(segments,2);
laser.trail_2 = zeros(segments,2);
laser.trail_3 = zeros(segments,2);
laser.trail_4 = zeros(segments,2);
laser.trail_5 = zeros(segments,2);
laser.trail_6 = zeros(segments,2);
laser.trail_7 = zeros(segments,2);
laser.trail_8 = zeros(segments,2);
laser.trail_9 = zeros(segments,2);

workpoints(1,1) = (rot_rectangle_points(1,1) + rot_rectangle_points(2,1)) / 2;
workpoints(1,2) =  rot_rectangle_points(1,1);

laser.trail_1(:,1) = linspace(workpoints(1,1), rot_rectangle_points(2,1), segments);
laser.trail_1(:,2) = workpoints(1,2) * ones(segments,1);

laser.trail_2 = zeros(segments,2);


alpha = -pi/ 2 / segments;
rot_radians = alpha;
for i = 1 : segments
    rot_rectangle_points      = rotate_coord(rot_rectangle_points      , rot_radians);
    rot_arc_points.right_up   = rotate_coord(rot_arc_points.right_up   , rot_radians);
    rot_arc_points.left_up    = rotate_coord(rot_arc_points.left_up    , rot_radians);
    rot_arc_points.left_down  = rotate_coord(rot_arc_points.left_down  , rot_radians);
    rot_arc_points.right_down = rotate_coord(rot_arc_points.right_down , rot_radians);

    set(handle_up   , 'xdata', rot_rectangle_points(1:2,1), 'ydata', rot_rectangle_points(1:2,2));
    set(handle_left , 'xdata', rot_rectangle_points(3:4,1), 'ydata', rot_rectangle_points(3:4,2));
    set(handle_down , 'xdata', rot_rectangle_points(5:6,1), 'ydata', rot_rectangle_points(5:6,2));
    set(handle_right, 'xdata', rot_rectangle_points(7:8,1), 'ydata', rot_rectangle_points(7:8,2));

    set(handle_right_up  , 'xdata', rot_arc_points.right_up(:,1)  , 'ydata', rot_arc_points.right_up(:,2)   );
    set(handle_left_up   , 'xdata', rot_arc_points.left_up(:,1)   , 'ydata', rot_arc_points.left_up(:,2)    );
    set(handle_left_down , 'xdata', rot_arc_points.left_down(:,1) , 'ydata', rot_arc_points.left_down(:,2)  );
    set(handle_right_down, 'xdata', rot_arc_points.right_down(:,1), 'ydata', rot_arc_points.right_down(:,2) );

    drawnow;
end

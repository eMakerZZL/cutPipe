draw_ractangle = @(points, style) {
    plot(points(:,1),points(:,2), style);
    plot([points(end,1),points(1,1)]',...
         [points(end,2),points(1,2)]', style);

    plot([points(1,1),points(3,1)]',...
         [points(1,2),points(3,2)]', ['--',style]);

    plot([points(2,1),points(4,1)]',...
         [points(2,2),points(4,2)]', ['--',style]);
};

rotate_z = @(theta) [ cos(theta),sin(theta);
                     -sin(theta),cos(theta)];

get_angle = @(coord) atan2(coord(1,2),coord(1,1));

square_pipe.length = 10;
square_pipe.width  = 20;

rotate_center.x    = 0;
rotate_center.y    = 0;

rot_angle = pi / 2;

ori_points = [
     square_pipe.length / 2  ,  square_pipe.width / 2;
    -square_pipe.length / 2  ,  square_pipe.width / 2;
    -square_pipe.length / 2  , -square_pipe.width / 2;
     square_pipe.length / 2  , -square_pipe.width / 2;
];

hold on;
axis equal;
show_limits = max(square_pipe.length, square_pipe.width);
axis([-show_limits show_limits -show_limits show_limits]);

draw_ractangle(ori_points,'b');

rot_points = ori_points;
for col = 1 : size(rot_points)
    rot_points(col,:) =rot_points(col,:) * rotate_z(rot_angle);
end
draw_ractangle(rot_points,'r');

radius = sqrt(ori_points(1,1)^2 + ori_points(1,2)^2);
theta = linspace(0, 2 * pi, 100);
circle.x = radius * cos(theta);
circle.y = radius * sin(theta);
plot(circle.x, circle.y, 'g');

% plot(ori_points(1,1),ori_points(1,2),'bo');
% plot(rot_points(1,1),rot_points(1,2),'ro');

% delta_x = rot_points(1,1) - ori_points(1,1);
% delta_y = rot_points(1,2) - ori_points(1,2);
% chord_length = sqrt(delta_x^2 + delta_y^2);
% delta_angle = asin(chord_length / (2 * radius));
% delta_angle = delta_angle * 2;

% start_angle = atan(ori_points(1,2) / ori_points(1,1));
% end_angle = start_angle + delta_angle;
% alpha = linspace(start_angle, end_angle, 100);
% run.x = (radius) * cos(alpha);
% run.y = (radius) * sin(alpha);
% plot(run.x, run.y, 'mo');

% animation_x = (radius + 1) * cos(alpha);
% animation_y = (radius + 1) * sin(alpha);
% for i = 1 : 100
    % plot(animation_x(i) ,animation_y(i), 'co');
    % pause(0.1);
% end
laser.trail_1 = [linspace(0, -square_pipe.length / 2, 100).', square_pipe.width / 2 * ones(100,1)];

start_point = ori_points(2,:);
end_point   = start_point * rotate_z(-rot_angle);
start_angle = get_angle(start_point);
end_angle   = get_angle(end_point);
alpha = linspace(start_angle, end_angle, 100);
for i = 1 : 100
    mat(i,1) = radius * cos(alpha(1,i));
    mat(i,2) = radius * sin(alpha(1,i));
end
laser.trail_2 = [mat];

rot_points = ori_points;
for col = 1 : size(rot_points)
    rot_points(col,:) =rot_points(col,:) * rotate_z(-rot_angle);
end
start_point = rot_points(2,:);
end_point   = rot_points(3,:);
laser.trail_3 = [linspace(start_point(1,1), end_point(1,1), 100).', linspace(start_point(1,2), end_point(1,2), 100).'];

start_point = end_point;
end_point   = start_point * rotate_z(-rot_angle);
start_angle = get_angle(start_point);
end_angle   = get_angle(end_point);
alpha = linspace(start_angle, end_angle, 100);
for i = 1 : 100
    mat(i,1) = radius * cos(alpha(1,i));
    mat(i,2) = radius * sin(alpha(1,i));
end
laser.trail_4 = [mat];

for col = 1 : size(rot_points)
    rot_points(col,:) =rot_points(col,:) * rotate_z(-rot_angle);
end
start_point = rot_points(3,:);
end_point   = rot_points(4,:);
laser.trail_5 = [linspace(start_point(1,1), end_point(1,1), 100).', linspace(start_point(1,2), end_point(1,2), 100).'];

start_point = end_point;
end_point   = start_point * rotate_z(-rot_angle);
start_angle = get_angle(start_point);
end_angle   = get_angle(end_point);
alpha = linspace(start_angle, end_angle, 100);
for i = 1 : 100
    mat(i,1) = radius * cos(alpha(1,i));
    mat(i,2) = radius * sin(alpha(1,i));
end
laser.trail_6 = [mat];

for col = 1 : size(rot_points)
    rot_points(col,:) =rot_points(col,:) * rotate_z(-rot_angle);
end
start_point = rot_points(4,:);
end_point   = rot_points(1,:);
laser.trail_7 = [linspace(start_point(1,1), end_point(1,1), 100).', linspace(start_point(1,2), end_point(1,2), 100).'];

start_point = end_point;
end_point   = start_point * rotate_z(-rot_angle);
start_angle = get_angle(start_point);
end_angle   = get_angle(end_point);
alpha = linspace(start_angle, end_angle, 100);
for i = 1 : 100
    mat(i,1) = radius * cos(alpha(1,i));
    mat(i,2) = radius * sin(alpha(1,i));
end
laser.trail_8 = [mat];

laser.trail_9 = [linspace(square_pipe.length / 2, 0, 100).', square_pipe.width / 2 * ones(100,1)];

for i = 1 : 100
    plot(laser.trail_1(i,1), laser.trail_1(i,2), '-mo', 'LineWidth',1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_2(i,1), laser.trail_2(i,2), '-mo', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_3(i,1), laser.trail_3(i,2), '-mo', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_4(i,1), laser.trail_4(i,2), '-mo', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_5(i,1), laser.trail_5(i,2), '-go', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_6(i,1), laser.trail_6(i,2), '-go', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_7(i,1), laser.trail_7(i,2), '-go', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_8(i,1), laser.trail_8(i,2), '-go', 'LineWidth', 1);
    pause(0.01);
end

for i = 1 : 100
    plot(laser.trail_9(i,1), laser.trail_9(i,2), '-ro', 'LineWidth', 1);
    pause(0.01);
end

clear all;
clc;
axis equal;
hold on;
format short;

xlabel('x');
ylabel('y');
zlabel('z');
view(90,0);

rotate_x = @(theta)[
                    1 , 0           , 0          , 0;
                    0 , cos(theta)  , sin(theta) , 0;
                    0 , -sin(theta) , cos(theta) , 0;
                    0 , 0           , 0          , 1;
                    ];

rotate_y = @(theta)[
                    cos(theta)  , 0 , sin(theta) , 0;
                    0           , 1 , 0          , 0;
                    -sin(theta) , 0 , cos(theta) , 0;
                    0           , 0 , 0          , 1;
                   ];

rotate_z = @(theta)[
                    cos(theta)  , sin(theta) , 0 , 0;
                    -sin(theta) , cos(theta) , 0 , 0;
                    0           , 0          , 1 , 0;
                    0           , 0          , 0 , 1;
                   ];

pipe_length = 20;
pipe_raidus = 10;

unit_arc_length = 0.1;

arc_guide_line_radius = pipe_raidus / 4;
arc_guide_line_radians_length = 5;

%laser working point is half pipe x length
laser_working_point_x = pipe_length / 2;
laser_offset_center_distance = 0;
laser_up_lift_distance = 5;
laser_circle_raduis = pipe_raidus / 2;

unit_circle_center_angle = unit_arc_length / (pi/180 * pipe_raidus);
unit_arc_rad = unit_circle_center_angle * pi / 180;

theta = 0 : unit_arc_rad : 2 * pi;
[col,row] = size(theta);
% pipe_rotate_x_angle = 0;
% pipe_rotate_y_angle = 0;
% pipe_rotate_z_angle = 0;

[cylinder_x, cylinder_y, cylinder_z] = cylinder(pipe_raidus, 100);
cylinder_z = cylinder_z * pipe_length;
pipe_x = cylinder_z;
pipe_y = cylinder_x;
pipe_z = cylinder_y;

%draw pipe
%mesh(pipe_x, pipe_y, pipe_z);

%laser working point is half pipe x length
% laser_working_point_x = pipe_length / 2;
% laser_offset_center_distance = 0;
% laser_up_lift_distance = 5;
% laser_circle_raduis = pipe_raidus;

laser_pos_x = laser_working_point_x;
laser_pos_y = laser_offset_center_distance;
laser_pos_z = pipe_raidus + laser_up_lift_distance;

%draw laser trail
laser_trail_x = laser_pos_x + laser_circle_raduis * cos(theta);
laser_trail_y = laser_pos_y + laser_circle_raduis * sin(theta);
laser_trail_z = laser_pos_z * ones(1, row);

laser_trail = [laser_trail_x',laser_trail_y',laser_trail_z';];
plot3(laser_trail_x, laser_trail_y, laser_trail_z, 'b*');

%draw cilcle trail
cut_trail_x = laser_trail_x;
cut_trail_y = laser_trail_y;
cut_trail_z = sqrt(pipe_raidus^2 - cut_trail_y.^2);

cut_trail = [cut_trail_x',cut_trail_y',cut_trail_z'];

angle_cut_y = acos(cut_trail_y ./ pipe_raidus);
angle_cut_y = rad2deg(angle_cut_y);
angle_cut_trail = [cut_trail_x',angle_cut_y',cut_trail_z'];
need_penetrate = false;
for i = 1 : row
    if (abs( cut_trail_y(1, i)) > pipe_raidus)
        need_penetrate = true;
        continue;
    end
    if (cut_trail_x(1,i) < 0 || cut_trail_x(1,i) > pipe_length)
        continue;
    end
    plot3(cut_trail_x(1,i), cut_trail_y(1,i), cut_trail_z(1,i), 'r*');
end

if (need_penetrate)
    for i = 1 : row
        if (abs( cut_trail_y(1, i)) > pipe_raidus)
            continue;
        end
        if (cut_trail_x(1,i) < 0 || cut_trail_x(1,i) > pipe_length)
            continue;
        end
        plot3(cut_trail_x(1,i), cut_trail_y(1,i), - cut_trail_z(1,i), 'y*');
    end
end

% arc_guide_line_radians = arc_guide_line_radians_length / arc_guide_line_radius;
% arc_guide_line_circle_center_x = laser_pos_x + laser_circle_raduis - arc_guide_line_radius;
% arc_guide_line_circle_center_y = laser_pos_y;
% arc_guide_line_circle_center_z = laser_pos_z;
%judge guide line in/out circle
is_arc_guide_line_out_circle = true;
%add arc in guide line 
% arc_guide_line_in_theta = arc_guide_line_radians : -unit_arc_rad : 0;
% [arc_guide_line_in_col, arc_guide_line_in_row] = size(arc_guide_line_in_theta);
% arc_trail_in_x = arc_guide_line_circle_center_x + arc_guide_line_radius * cos(arc_guide_line_in_theta);
% arc_trail_in_y = arc_guide_line_circle_center_y + arc_guide_line_radius * sin(arc_guide_line_in_theta);
% arc_trail_in_z = arc_guide_line_circle_center_z * ones(1, arc_guide_line_in_row);
% if (is_arc_guide_line_out_circle)
%     parallel_trail_in_x = arc_trail_in_x - arc_guide_line_circle_center_x;
%     parallel_trail_in_y = arc_trail_in_y - arc_guide_line_circle_center_y;
%     parallel_trail_in_z = arc_trail_in_z - arc_guide_line_circle_center_z;
% 
%     rotate_y_trail_in_x = parallel_trail_in_x * cos(pi) - parallel_trail_in_z * sin(pi);
%     rotate_y_trail_in_y = parallel_trail_in_y;
%     rotate_y_trail_in_z = parallel_trail_in_x * sin(pi) + parallel_trail_in_z * cos(pi);
% 
%     arc_trail_in_x = rotate_y_trail_in_x + arc_guide_line_circle_center_x + 2 * arc_guide_line_radius;
%     arc_trail_in_y = rotate_y_trail_in_y + arc_guide_line_circle_center_y;
%     arc_trail_in_z = rotate_y_trail_in_z + arc_guide_line_circle_center_z;
% end
% plot3(arc_trail_in_x, arc_trail_in_y, arc_trail_in_z, 'co');
% arc_trail_in = [arc_trail_in_x',arc_trail_in_y',arc_trail_in_z'];
%add arc out guide line
% arc_guide_line_out_theta = 2 * pi : -unit_arc_rad : 2 * pi - arc_guide_line_radians;
% [arc_guide_line_out_col, arc_guide_line_out_row] = size(arc_guide_line_out_theta);
% arc_trail_out_x = arc_guide_line_circle_center_x + arc_guide_line_radius * cos(arc_guide_line_out_theta);
% arc_trail_out_y = arc_guide_line_circle_center_y + arc_guide_line_radius * sin(arc_guide_line_out_theta);
% arc_trail_out_z = arc_guide_line_circle_center_z * ones(1, arc_guide_line_out_row);
% if (is_arc_guide_line_out_circle)
%     parallel_trail_out_x = arc_trail_out_x - arc_guide_line_circle_center_x;
%     parallel_trail_out_y = arc_trail_out_y - arc_guide_line_circle_center_y;
%     parallel_trail_out_z = arc_trail_out_z - arc_guide_line_circle_center_z;
% 
%     rotate_y_trail_out_x = parallel_trail_out_x * cos(pi) - parallel_trail_out_z * sin(pi);
%     rotate_y_trail_out_y = parallel_trail_out_y;
%     rotate_y_trail_out_z = parallel_trail_out_x * sin(pi) + parallel_trail_out_z * cos(pi);
% 
%     arc_trail_out_x = rotate_y_trail_out_x + arc_guide_line_circle_center_x + 2 * arc_guide_line_radius;
%     arc_trail_out_y = rotate_y_trail_out_y + arc_guide_line_circle_center_y;
%     arc_trail_out_z = rotate_y_trail_out_z + arc_guide_line_circle_center_z;
% end
% plot3(arc_trail_out_x, arc_trail_out_y, arc_trail_out_z, 'mo');
% 
% arc_trail_out = [arc_trail_out_x',arc_trail_out_y',arc_trail_out_z'];
% draw arc in guide line trail
% cut_arc_guide_line_in_x = arc_trail_in_x;
% cut_arc_guide_line_in_y = arc_trail_in_y;
% cut_arc_guide_line_in_z = sqrt(pipe_raidus^2 - cut_arc_guide_line_in_y.^2);
% plot3(cut_arc_guide_line_in_x, cut_arc_guide_line_in_y, cut_arc_guide_line_in_z, 'r*');
% cut_arc_guide_line_in = [cut_arc_guide_line_in_x',cut_arc_guide_line_in_y',cut_arc_guide_line_in_z'];
% draw arc out guide line trail
% cut_arc_guide_line_out_x = arc_trail_out_x;
% cut_arc_guide_line_out_y = arc_trail_out_y;
% cut_arc_guide_line_out_z = sqrt(pipe_raidus^2 - cut_arc_guide_line_out_y.^2);
% plot3(cut_arc_guide_line_out_x, cut_arc_guide_line_out_y, cut_arc_guide_line_out_z, 'r*');
% cut_arc_guide_line_out = [cut_arc_guide_line_out_x',cut_arc_guide_line_out_y',cut_arc_guide_line_out_z'];

% A = arc_trail_in == cut_arc_guide_line_in;
% B = arc_trail_out == cut_arc_guide_line_out;
% 
% [~, frame_row_count] = size(arc_guide_line_in_theta);
% move_laser_x = 0;
% move_laser_y = 0;
% move_laser_z = laser_pos_z;
% hlaser = plot3(move_laser_x, move_laser_y, move_laser_z, 'ro');
% for i = 1 : frame_row_count
%     move_laser_x = arc_trail_in_x(1, i);
%     angle = acos(arc_trail_in_y(1, i) / pipe_raidus);
%     set(hlaser, 'xdata', move_laser_x, 'ydata', move_laser_y, 'zdata', move_laser_z);
%     drawnow;
%     pause(0.01);
% end
% 
% [~, frame_row_count] = size(theta);
% for i = 1 : frame_row_count
%     move_laser_x = cut_trail_x(1,i);
%     set(hlaser, 'xdata', move_laser_x, 'ydata', move_laser_y, 'zdata', move_laser_z);
%     drawnow;
%     pause(0.01);
% end
% 
% [~, frame_row_count] = size(arc_guide_line_out_theta);
% for i = 1 : frame_row_count
%     move_laser_x = arc_trail_out_x(1,i);
%     set(hlaser, 'xdata', move_laser_x, 'ydata', move_laser_y, 'zdata', move_laser_z);
%     drawnow;
%     pause(0.01);
% end

guide_line_length = 8;
guide_line_degree = 30;
guide_line_x = laser_pos_x + laser_circle_raduis;
guide_line_y = laser_pos_y;
guide_line_z = laser_pos_z;
guide_line_radian = deg2rad(guide_line_degree);

guide_line_xs = guide_line_x + guide_line_length * cos(guide_line_radian - pi / 2);
guide_line_ys = guide_line_y + guide_line_length * sin(guide_line_radian - pi / 2);
guide_line_zs = guide_line_z;

guide_line_xe = guide_line_x;
guide_line_ye = guide_line_y;
guide_line_ze = guide_line_z;

guide_line_unit_line = 0.1;
guide_line_plan_length = 0;
% guide_line_delta = 0 : 0.1 : 1;
% [~, guide_line_col] = size(guide_line_delta);
count = 1;
while guide_line_plan_length <= guide_line_length
    guide_line_delta = guide_line_plan_length / guide_line_length;
    guide_line_fx(1,count) = (1 - guide_line_delta) .* guide_line_xs + guide_line_delta .* guide_line_xe;
    guide_line_fy(1,count) = (1 - guide_line_delta) .* guide_line_ys + guide_line_delta .* guide_line_ye;
    guide_line_fz(1,count) =  guide_line_ze;
    guide_line_plan_length = guide_line_plan_length + guide_line_unit_line;
    count = count + 1;
end
guide_line = [guide_line_fx' guide_line_fy' guide_line_fz'];
guide_line_out = flipud(guide_line);
plot3(guide_line_fx,guide_line_fy,guide_line_fz,'ro');

guide_line_cut_trail_x = guide_line_fx;
guide_line_cut_trail_y = guide_line_fy;
guide_line_cut_trail_z = sqrt(pipe_raidus^2 - guide_line_cut_trail_y.^2);
guide_line_cut_trail = [guide_line_cut_trail_x' guide_line_cut_trail_y' guide_line_cut_trail_z'];
guide_line_cut_out_trail = flipud(guide_line);
plot3(guide_line_cut_trail_x,guide_line_cut_trail_y,guide_line_cut_trail_z,'r','LineWidth',5);

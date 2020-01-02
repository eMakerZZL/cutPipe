function main
close all;
clear all;
clc;

hold on;
axis equal;

pipe_radius = 30;
pipe_length = 100;

unit_radian = pi / 360;

laser_y_offset = 0;
laser_z_height = 10;

CreatePipe(pipe_radius, pipe_length);

circle_x_distance = 10;
circle_radius = 30;
CircleTrail = CreateCircle(circle_x_distance, circle_radius);
firstPoint = CircleTrail(1,:);

guide_line_dir = pi / 3;
guide_line_length = 40;
GuideLineTrail = CreateGuideLine(guide_line_dir, guide_line_length);

GenerateCutTrail(CircleTrail);

GenerateGuideLineTrail(GuideLineTrail);

xlabel('X');
ylabel('Y');
zlabel('Z');

view(3);
hold off;

function CreatePipe(radius, length)
    [cylinder_x, cylinder_y, cylinder_z] = cylinder(radius, 100);
    cylinder_z = cylinder_z * length;

    [pipe_x, pipe_y, pipe_z] = ConvertAxis(cylinder_x, cylinder_y, cylinder_z);

    mesh(pipe_x, pipe_y, pipe_z);

    function [X, Y, Z] = ConvertAxis(x, y, z)
        X = z;
        Y = x;
        Z = y;
    end

end

function LaserTrail = CreateCircle(x_offset, radius)
    CenterPoint(1) = x_offset;
    CenterPoint(2) = laser_y_offset;
    CenterPoint(3) = pipe_radius + laser_z_height;

    segments = 2 * pi / unit_radian;
    
    theta = linspace(0, 2 * pi, segments);

    CenterPoint(1) = CenterPoint(1) + radius;

    x = CenterPoint(1) + radius * cos(theta);
    y = CenterPoint(2) + radius * sin(theta);
    z = CenterPoint(3) * ones(1, segments);  

    handle = plot3(x,y,z);
    set(handle,'LineWidth', 3);
    set(handle,'Color', 'r');

    LaserTrail = [x', y', z'];
end

function GuideLineTrail = CreateGuideLine(dir_radian, length)
    segments = 100;

    x0 = firstPoint(1);
    y0 = firstPoint(2);

    x1 = x0 + cos(dir_radian) * length;
    y1 = y0 + sin(dir_radian) * length;

    t = linspace(0, 1, segments);

    x = (1 - t) .* x0 + t .* x1;
    y = (1 - t) .* y0 + t .* y1;
    z = ones(1,segments) .* firstPoint(3);

    GuideLineTrail = [x', y', z'];

    handle = plot3(x,y,z);
    set(handle,'LineWidth', 3);
    set(handle,'Color', 'r');
end

function GenerateCutTrail(LaserTrail)
    CutTrail = zeros(size(LaserTrail));

    CutTrail(:,[1,2]) = LaserTrail(:,[1,2]);

    if (LaserTrail(1,3) >= 0)
        CutTrail(:,3) = sqrt(pipe_radius^2 - CutTrail(:,2).^2);
    else
        CutTrail(:,3) = -sqrt(pipe_radius^2 - CutTrail(:,2).^2);
    end

    handle = plot3(CutTrail(:,1), CutTrail(:,2), CutTrail(:,3));
    set(handle,'LineWidth', 3);
    set(handle,'Color', 'b');
end

function GenerateGuideLineTrail(GuideLineTrail)
    a = pipe_radius / cos(pi - guide_line_dir);
    b = pipe_radius;

    theta = linspace(0, 2 * pi, 360);

    x = zeros(1, 360);
    y = a * cos(theta);
    z = b * sin(theta);
    
%     x = x * cos(pi - guide_line_dir) - y * sin(pi - guide_line_dir);
%     y = x * sin(pi - guide_line_dir) + y * cos(pi - guide_line_dir);
    
    x = x + firstPoint(1);

    handle = plot3(x,y,z);
    set(handle,'LineWidth', 3);
    set(handle,'Color', 'r');
end

end


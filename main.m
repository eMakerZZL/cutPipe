function main
close all;
clear all;
clc;

hold on;
axis equal;
view(3);

pipe_radius = 30;
pipe_length = 100;

unit_radian = pi / 360;
unit_arc_length = 0.1;
unit_arc_radian = unit_arc_length / pipe_radius;

laser_y_offset = 0;
laser_z_height = 10;

CreatePipe(pipe_radius, pipe_length);

circle_x_distance = 10;
circle_radius = 30;
CircleOverCutLength = 10;
Laser_CircleTrail = CreateCircle(circle_x_distance, circle_radius, CircleOverCutLength);
firstPoint = Laser_CircleTrail(1, :);
endPoint = Laser_CircleTrail(end, :);

guide_line_dir = -pi / 3;
guide_line_length = 30;
[Laser_GuideLineTrail_S, Laser_GuideLineTrail_E] = CreateGuideLine(guide_line_dir, guide_line_length, CircleOverCutLength);

Cut_CircleTrail = GenerateCutTrail(Laser_CircleTrail);
[Cut_GuideLineTrail_S, Cut_GuideLineTrail_E] = GenerateGuideLineTrail(guide_line_dir, guide_line_length, CircleOverCutLength);
% BevelCut(pi / 4);

xlabel('X');
ylabel('Y');
zlabel('Z');

LaserTrail = [Laser_GuideLineTrail_S; Laser_CircleTrail; Laser_GuideLineTrail_E];
CutTrail   = [Cut_GuideLineTrail_S;   Cut_CircleTrail;   Cut_GuideLineTrail_E];
PrintTrail(LaserTrail, CutTrail);

hold off;

function PrintTrail(LaserTrail, CutTrail)
    for i = 1 : 10 :size(LaserTrail, 1)
        plot3(LaserTrail(i,1), LaserTrail(i,2), LaserTrail(i,3), 'ro');
%         pause(0.001);
    end

    for i = 1 : 10 :size(CutTrail, 1)
        plot3(CutTrail(i,1),   CutTrail(i,2),   CutTrail(i,3), 'bo');
%         pause(0.001);
    end
end

function CreatePipe(radius, length)
    [cylinder_x, cylinder_y, cylinder_z] = cylinder(radius, 50);
    cylinder_z = cylinder_z * length;

    [pipe_x, pipe_y, pipe_z] = ConvertAxis(cylinder_x, cylinder_y, cylinder_z);

    mesh(pipe_x, pipe_y, pipe_z);

    function [X, Y, Z] = ConvertAxis(x, y, z)
        X = z;
        Y = x;
        Z = y;
    end

end

function LaserTrail = CreateCircle(x_offset, radius, OverCutLength)
    CenterPoint(1) = x_offset;
    CenterPoint(2) = laser_y_offset;
    CenterPoint(3) = pipe_radius + laser_z_height;

    OverCutRadian = OverCutLength / radius;

    segments = round((2 * pi + OverCutRadian) / unit_arc_radian);
    theta = linspace(0, 2 * pi + OverCutRadian, segments);

    CenterPoint(1) = CenterPoint(1) + radius;

    x = CenterPoint(1) + radius * cos(theta);
    y = CenterPoint(2) + radius * sin(theta);
    z = CenterPoint(3) * ones(1, segments);  

    LaserTrail = [x', y', z'];
end

function [GuideLineTrail_S, GuideLineTrail_E] = CreateGuideLine(dir_radian, length, OverCutLength)
    segments = round(length / unit_arc_length);

    x0 = firstPoint(1);
    y0 = firstPoint(2);

    x1 = x0 + cos(dir_radian) * length;
    y1 = y0 + sin(dir_radian) * length;

    t = linspace(1, 0, segments);

    x = (1 - t) .* x0 + t .* x1;
    y = (1 - t) .* y0 + t .* y1;
    z = ones(1,segments) .* firstPoint(3);

    GuideLineTrail_S = [x', y', z'];

    x0 = endPoint(1);
    y0 = endPoint(2);

    offset_radian = OverCutLength / circle_radius;

    x1 = x0 + cos(dir_radian + offset_radian) * length;
    y1 = y0 + sin(dir_radian + offset_radian) * length;

    t = linspace(0, 1, segments);

    x = (1 - t) .* x0 + t .* x1;
    y = (1 - t) .* y0 + t .* y1;
    z = ones(1,segments) .* endPoint(3);

    GuideLineTrail_E = [x', y', z'];
end

function CutTrail = GenerateCutTrail(LaserTrail)
    CutTrail = zeros(size(LaserTrail));

    CutTrail(:,[1,2]) = LaserTrail(:,[1,2]);

    CutTrail(:,3) = sqrt(pipe_radius^2 - CutTrail(:,2).^2);
end

function BevelCut(angle)
    a = pipe_radius / sin(angle);
    b = pipe_radius;

    theta = 0.5 * pi : unit_arc_radian : 2.5 * pi;

    x = zeros(1, size(theta,2));
    y = a * cos(theta);
    z = b * sin(theta);
    
    x1 = x * cos(pi/2 + angle) - y * sin(pi/2 + angle);
    y1 = x * sin(pi/2 + angle) + y * cos(pi/2 + angle);
    z1 = z;

    x1 = x1 + firstPoint(1);

    CutTrail = Generate_OverCutTrail([x1',y1',z1'], 30);
end

function [OutPut] = Generate_OverCutTrail(CutTrail, OverCutLength)
    OverCutSegments =  OverCutLength / unit_arc_length;

    OverCutTrail = CutTrail(1 : OverCutSegments, :);

    OutPut = cat(1, CutTrail, OverCutTrail);
end

function [Cut_GuideLine_S, Cut_GuideLine_E] = GenerateGuideLineTrail(dir, length, overCut)
    r = pipe_radius;
    c = 2 * pi * r;
    k = pi / 2 + dir;
    h = k * c;
    
    radian = length / pipe_radius;

    theta = linspace(0.5 * pi , 0.5 * pi + radian, 500);
    x1 = h * theta / (2 * pi);
    y1 = r * cos(theta);
    z1 = r * sin(theta);

    delta_x = firstPoint(1) - x1(1);
    x1 = x1 + delta_x;

    Cut_GuideLine_S = [x1', y1', z1'];
    
    offset_radian = overCut / circle_radius;
    y_componment = sin(offset_radian) * circle_radius;
    pipe_offset_radian = asin(y_componment / pipe_radius);

    r = pipe_radius;
    c = 2 * pi * r;
    k = pi / 2 + dir + offset_radian;
    h = k * c;

    theta = linspace(0.5 * pi - pipe_offset_radian, 0.5 * pi + radian - pipe_offset_radian, 500);
    x1 = h * theta / (2 * pi);
    y1 = r * cos(theta);
    z1 = r * sin(theta);

    delta_x = endPoint(1) - x1(1);
    x1 = x1 + delta_x;

    Cut_GuideLine_E = [x1', y1', z1'];
end

end


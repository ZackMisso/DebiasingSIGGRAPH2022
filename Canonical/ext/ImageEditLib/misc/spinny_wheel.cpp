#include <iostream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

#define PI 3.1415269

struct Circle
{
    Circle()
        : xpos(0.f),
          ypos(0.f),
          outer_radius(0.f),
          inner_radius(0.f) { }

    Circle(float xpos,
           float ypos,
           float inner_radius,
           float outer_radius)
        : xpos(xpos),
          ypos(ypos),
          inner_radius(inner_radius),
          outer_radius(outer_radius) { }

    // returns a 1 if hit, otherwise returns 0
    float evaluate(float x, float y) const
    {
        float sqr_dist = (xpos - x) * (xpos - x) +
                         (ypos - y) * (ypos - y);

        if (sqr_dist > inner_radius * inner_radius &&
            sqr_dist < outer_radius * outer_radius)
        {
            return 1.f;
        }

        return 0.f;
    }

    float get_angle(float x, float y, float base_angle) const
    {
        // edge case
        if (x == xpos && y == ypos) return 0.f;

        float dx = x - xpos;
        float dy = y - ypos;
        float dist = sqrt(dx*dx + dy*dy);
        dx /= dist;
        dy /= dist;

        // a dot b = |a||b|cosT
        float xx = cos(base_angle);
        float yy = sin(base_angle);
        return acos(dx*xx + dy*yy);
    }

    float xpos;
    float ypos;
    float outer_radius;
    float inner_radius;
};

struct Line
{
    Line()
        : start_x(0.f),
          start_y(0.f),
          end_x(0.f),
          end_y(0.f),
          thickness(0.f) { }
    Line(float start_x,
         float start_y,
         float end_x,
         float end_y,
         float thickness)
        : start_x(start_x),
          start_y(start_y),
          end_x(end_x),
          end_y(end_y),
          thickness(thickness) { }

    // returns a 1 if hit, otherwise returns 0
    float evaluate(float x, float y) const
    {
        float dx = x - start_x;
        float dy = y - start_y;

        float xx = end_x - start_x;
        float yy = end_y - start_y;

        float sqr = sqrt(xx*xx + yy*yy);
        xx /= sqr;
        yy /= sqr;

        float dist_perp = dx * xx + dy * yy;
        float perp_x = dist_perp * xx + end_x;
        float perp_y = dist_perp * yy + end_y;

        float proxy = 0.f;
        if (start_x != 0.0 || end_x != 0.0)
        {
            proxy = (perp_x - start_x) / (end_x - start_x);
        }
        else
        {
            proxy = (perp_y - start_y) / (end_y - start_y);
        }

        if (proxy < 0.0 || proxy > 1.0)
            dist_perp = 100000000.0;

        float dist_start = sqrt((x-start_x) * (x-start_x) +
                                (y-start_y) * (y-start_y));
        float dist_end = sqrt((x-end_x) * (x-end_x) +
                              (y-end_y) * (y-end_y));

        if (std::abs(dist_perp) < thickness ||
            dist_start < thickness ||
            dist_end < thickness) return 1.f;

        return 0.f;
    }

    float start_x;
    float start_y;
    float end_x;
    float end_y;
    float thickness;
};

struct DrawEvent
{
    DrawEvent()
        : start_frame(0), end_frame(0) { }
    DrawEvent(int start_frame, int end_frame)
        : start_frame(start_frame),
          end_frame(end_frame) { }

    virtual ~DrawEvent() { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const = 0;

    int start_frame;
    int end_frame;
};

struct DrawFullCircle : public DrawEvent
{
    DrawFullCircle(Circle& circle,
                   imedit::Pixel color)
        : DrawEvent(), circle(circle), color(color) { }
    DrawFullCircle(Circle& circle,
                   imedit::Pixel color,
                   int start_frame,
                   int end_frame)
        : DrawEvent(start_frame, end_frame),
          circle(circle),
          color(color) { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const
    {
        if (circle.evaluate(x, y) > 0.f) pixel = color;
    }

    Circle circle;
    imedit::Pixel color;
};

struct DrawCircleFromAngleOutwards : public DrawEvent
{
    DrawCircleFromAngleOutwards(Circle& circle,
                                float angle,
                                imedit::Pixel color,
                                int start_frame,
                                int end_frame)
        : DrawEvent(start_frame, end_frame),
          circle(circle),
          angle(angle),
          color(color) { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const
    {
        if (circle.evaluate(x, y) > 0.f)
        {
            float ang = circle.get_angle(x, y, angle);

            float proxy = float(frame - start_frame) / float(end_frame - start_frame);

            if (ang / PI < proxy) pixel = color;
        }
    }

    Circle circle;
    imedit::Pixel color;
    float angle;
};

struct DrawFullLine : public DrawEvent
{
    DrawFullLine(Line& line,
                 imedit::Pixel color)
        : DrawEvent(), line(line), color(color) { }
    DrawFullLine(Line& line,
                 imedit::Pixel color,
                 int start_frame,
                 int end_frame)
        : DrawEvent(start_frame, end_frame),
          line(line),
          color(color) { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const
    {
        if (line.evaluate(x, y) > 0.f) pixel = color;
    }

    Line line;
    imedit::Pixel color;
};

struct DrawLineFromStartToFinish : public DrawEvent
{
    DrawLineFromStartToFinish(Line& line,
                              imedit::Pixel color)
        : DrawEvent(), line(line), color(color) { }
    DrawLineFromStartToFinish(Line& line,
                              imedit::Pixel color,
                              int start_frame,
                              int end_frame)
        : DrawEvent(start_frame, end_frame),
          line(line),
          color(color) { }

    virtual void evaluate(float x,
                          float y,
                          imedit::Pixel& pixel,
                          int frame) const
    {
        float proxy = float(frame - start_frame) / float(end_frame - start_frame);

        float dx = line.end_x - line.start_x;
        float dy = line.end_y - line.start_y;

        proxy = 0.5;

        // std::cout << "dx: " << dx << std::endl;
        // std::cout << "dy: " << dy << std::endl;
        // std::cout << "proxy: " << proxy << std::endl;

        Line tmp = Line(line.start_x,
                        line.start_y,
                        dx * proxy + line.start_x,
                        dy * proxy + line.start_y,
                        line.thickness);

        if (tmp.evaluate(x, y) > 0.f) pixel = color;
    }

    Line line;
    imedit::Pixel color;
};

// TODO: this is basically a shader, I should just implement this in shader toy
void evaluate_pixel(float x,
                    float y,
                    imedit::Pixel& pixel,
                    std::vector<DrawEvent*>& events,
                    int frame)
{
    for (int i = 0; i < events.size(); ++i)
    {
        if (events[i]->start_frame <= frame && frame < events[i]->end_frame)
        {
            events[i]->evaluate(x, y, pixel, frame);
        }
    }
}

int main(int argc, char* argv[])
{
    int x_res = 512;
    int y_res = 512;
    int samples = 1;
    pcg32 rng = pcg32(0x1417, 0x32ad);

    std::string publish_command = "ffmpeg -r 24 -f image2 -i spinny_vis/spinny_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M spinny_vis/spinny.mp4";;
    system("mkdir spinny_vis");

    imedit::Image image = imedit::Image(x_res, y_res);
    Circle circle = Circle(0.5, 0.5, 0.40, 0.41);
    Line line = Line(0.0, 0.405, 0.0, -0.405, 0.005);

    std::vector<DrawEvent*> events = std::vector<DrawEvent*>();
    events.push_back(new DrawCircleFromAngleOutwards(circle, PI / 4.0, imedit::Pixel(1.f, 0.f, 0.f), 0, 100));
    events.push_back(new DrawLineFromStartToFinish(line, imedit::Pixel(0.f, 1.f, 0.f), 100, 200));

    int start_frame = 0;
    int end_frame = 200;

    for (int frame = start_frame; frame < end_frame; frame++)
    {
        std::cout << "Rendering frame: " << frame << std::endl;
        imedit::Image image = imedit::Image(x_res, y_res);
        for (int i = 0; i < y_res; ++i)
        {
            for(int j = 0; j < x_res; ++j)
            {
                for (int k = 0; k < samples; ++k)
                {
                    imedit::Pixel pixel = imedit::Pixel(0.f);
                    evaluate_pixel(float(j) / float(x_res),
                                   float(i) / float(y_res),
                                   pixel,
                                   events,
                                   frame);
                    image(j, i) += pixel;
                }
                image(j, i) /= float(samples);
            }
        }
        char str[5];
        snprintf(str, 5, "%04d", frame);
        std::string name = "spinny_vis/spinny_" + std::string(str);
        image.write(name + ".exr");
        image.write(name + ".png");
    }

    system(publish_command.c_str());

    for (int i = 0; i < events.size(); ++i)
    {
        delete events[i];
        events.clear();
    }

    return 0;
}

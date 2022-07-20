#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

void combine(imedit::Image& render, imedit::Image& ui, imedit::Image& results)
{
    for (int i = 0; i < render.height(); ++i)
    {
        for (int j = 0; j < render.width(); ++j)
        {
            // std::cout << "j:" << j << " i:" << i << std::endl;
            results(j, i) = render(j, i);
        }
    }

    for (int i = 0; i < 1080; ++i)
    {
        for (int j = 0; j < 840; ++j)
        {
            // std::cout << "j:" << j << " i:" << i << std::endl;
            results(j+render.width(), i) = ui(j+1, i+1) * 0.5;
        }
    }
}

std::string zero_pad(int num)
{
	std::stringstream ss;

	// the number is converted to string with the help of stringstream
	ss << num;
	std::string ret;
	ss >> ret;

	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < 4 - str_length; i++)
		ret = "0" + ret;
	return ret;
}

int main()
{
    std::string ui_path = "/Users/corneria/Documents/Videos/hetero_2/rendered_ui/ui_";
    std::string render_path = "/Users/corneria/Documents/Videos/hetero_2/rendered_frames/medium_hetero_";
    std::string final_path = "/Users/corneria/Documents/Videos/hetero_2/final_frames_2/frame_";

    int actual_index = 0;

    for (int i = 0; i < 500; ++i)
    {
        std::cout << "compositing frame: " << actual_index << std::endl;

        std::string index = zero_pad(i);
        std::string actual_str = zero_pad(actual_index);

        imedit::Image render = imedit::Image(render_path + index + ".exr");
        imedit::Image ui = imedit::Image(ui_path + index + ".png");

        imedit::Image results = imedit::Image(render.width() + ui.width(), render.height());
        combine(render, ui, results);

        results.write(final_path + actual_str + ".exr");
        actual_index++;
    }

    for (int i = 499; i >= 100; i--)
    {
        std::cout << "compositing frame: " << actual_index << std::endl;

        std::string index = zero_pad(i);
        std::string actual_str = zero_pad(actual_index);

        imedit::Image render = imedit::Image(render_path + index + ".exr");
        imedit::Image ui = imedit::Image(ui_path + index + ".png");

        imedit::Image results = imedit::Image(render.width() + ui.width(), render.height());
        combine(render, ui, results);

        results.write(final_path + actual_str + ".exr");
        actual_index++;
    }
}

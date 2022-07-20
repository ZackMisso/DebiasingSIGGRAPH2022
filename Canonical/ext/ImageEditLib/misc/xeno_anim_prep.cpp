#include <iostream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/im_color_maps.h"

void non_slowed_down_frames()
{
    std::vector<std::pair<int, int> > modifications = std::vector<std::pair<int, int> >();
    modifications.push_back(std::pair<int, int>(1, 24));
    modifications.push_back(std::pair<int, int>(292, 24));
    modifications.push_back(std::pair<int, int>(366, 24));
    modifications.push_back(std::pair<int, int>(432, 48));
    modifications.push_back(std::pair<int, int>(524, 72));
    modifications.push_back(std::pair<int, int>(525, 72));
    modifications.push_back(std::pair<int, int>(619, 96));
    modifications.push_back(std::pair<int, int>(680, 72));
    modifications.push_back(std::pair<int, int>(900, 48));
    modifications.push_back(std::pair<int, int>(971, 72));

    std::string orig_path = "/Users/corneria/Documents/Videos/shulks_workshop/base_frames/";
    std::string modified_path = "/Users/corneria/Documents/Videos/shulks_workshop/moded_frames/";
    std::string rm_cmd = "rm -rf " + modified_path;
    std::string mkdir_cmd = "mkdir " + modified_path;

    system(rm_cmd.c_str());
    system(mkdir_cmd.c_str());

    int actual_frame = 0;
    int mod_index = 0;

    for (int orig_frame = 1; orig_frame < 972; ++orig_frame)
    {
        // std::cout << "processing frame: " << orig_frame << std::endl;
        char orig_str[5];
        snprintf(orig_str, 5, "%04d", orig_frame);
        std::string orig_name = orig_path + "frame_" + std::string(orig_str) + ".exr";

        int copies = 1;
        if (modifications[mod_index].first == orig_frame)
        {
            std::cout << "modifying frame: " << orig_frame << std::endl;
            copies = modifications[mod_index].second;
            mod_index++;
        }
        if (orig_frame >= 839 && orig_frame < 900)
        {
            copies = 2;
        }

        if (orig_frame == 971)
        {
            std::cout << "copies: " << copies << std::endl;
        }

        for (int i = 0; i < copies; ++i)
        {
            char mod_str[5];
            snprintf(mod_str, 5, "%04d", actual_frame);
            std::string mod_name = modified_path + "frame_" + std::string(mod_str) + ".exr";

            std::string cmd = "cp " + orig_name + " " + mod_name;

            system(cmd.c_str());

            actual_frame++;
        }
    }
}

void do_sequence(std::string path,
                 std::string name,
                 std::vector<std::pair<int, int> >& modifications,
                 int start_frame,
                 int end_frame)
{
    std::string orig_path = "/Users/corneria/Documents/Videos/shulks_workshop/base_frames/";
    std::string modified_path = path;
    std::string rm_cmd = "rm -rf " + modified_path;
    std::string mkdir_cmd = "mkdir " + modified_path;

    system(rm_cmd.c_str());
    system(mkdir_cmd.c_str());

    modified_path = modified_path + name;

    int actual_frame = 0;
    int mod_index = 0;

    for (int i = 0; i < modifications.size(); ++i)
    {
        if (modifications[i].first < start_frame) mod_index++;
    }

    for (int orig_frame = start_frame; orig_frame < end_frame+1; ++orig_frame)
    {
        // std::cout << "processing frame: " << orig_frame << std::endl;
        char orig_str[5];
        snprintf(orig_str, 5, "%04d", orig_frame);
        std::string orig_name = orig_path + "frame_" + std::string(orig_str) + ".exr";

        int copies = 1;
        if (modifications[mod_index].first == orig_frame)
        {
            std::cout << "modifying frame: " << orig_frame << std::endl;
            copies = modifications[mod_index].second;
            mod_index++;
        }
        // if (orig_frame >= 839 && orig_frame < 900)
        // {
        //     copies = 2;
        // }

        // if (orig_frame == 971)
        // {
        //     std::cout << "copies: " << copies << std::endl;
        // }

        for (int i = 0; i < copies; ++i)
        {
            char mod_str[5];
            snprintf(mod_str, 5, "%04d", actual_frame);
            std::string mod_name = modified_path + "frame_" + std::string(mod_str) + ".exr";

            std::string cmd = "cp " + orig_name + " " + mod_name;

            system(cmd.c_str());

            actual_frame++;
        }
    }
}


void sequence_split_frames()
{
    std::vector<std::pair<int, int> > modifications = std::vector<std::pair<int, int> >();
    modifications.push_back(std::pair<int, int>(1, 24));
    modifications.push_back(std::pair<int, int>(292, 24));
    modifications.push_back(std::pair<int, int>(366, 24));
    modifications.push_back(std::pair<int, int>(432, 48));
    modifications.push_back(std::pair<int, int>(524, 72));
    modifications.push_back(std::pair<int, int>(525, 72));
    modifications.push_back(std::pair<int, int>(619, 48));
    modifications.push_back(std::pair<int, int>(619, 48));
    modifications.push_back(std::pair<int, int>(680, 72));
    modifications.push_back(std::pair<int, int>(900, 48));
    modifications.push_back(std::pair<int, int>(971, 72));

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_one/",
                "one_",
                modifications,
                1, 370);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_two/",
                "two_",
                modifications,
                371, 416);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_three/",
                "three_",
                modifications,
                417, 433);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_four/",
                "four_",
                modifications,
                434, 452);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_five/",
                "five_",
                modifications,
                452, 524);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_six/",
                "six_",
                modifications,
                525, 680);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_seven/",
                "seven_",
                modifications,
                681, 838);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_eight/",
                "eight_",
                modifications,
                838, 900);

    do_sequence("/Users/corneria/Documents/Videos/shulks_workshop/sequence_nine/",
                "nine_",
                modifications,
                901, 971);
}

int main(int argc, char* argv[])
{
    std::cout << "creating non-slowed-down frames" << std::endl;
    non_slowed_down_frames();
    std::cout << "creating split frames" << std::endl;
    sequence_split_frames();

    return 0;
}

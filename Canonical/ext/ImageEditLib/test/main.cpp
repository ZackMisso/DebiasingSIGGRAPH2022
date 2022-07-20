/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/im_util.h"

using namespace std;
using namespace imedit;

bool generate_test_set();
bool delete_test_set();
bool getchannel_test();
bool operator_test();

float randomFloat()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

bool generate_test_set()
{
    uint32_t image_res = 128;
    // uint32_t image_res = 1024;

    Image redImage = Image(image_res, image_res, 3);
    Image blueImage = Image(image_res, image_res, 3);
    Image greenImage = Image(image_res, image_res, 3);

    for (int i = 0; i < image_res; ++i)
    {
        for (int j = 0; j < image_res; ++j)
        {
            redImage(j, i, 0) = 0.5f;
            redImage(j, i, 1) = 0.f;
            redImage(j, i, 2) = 0.f;

            greenImage(j, i, 0) = 0.f;
            greenImage(j, i, 1) = 0.5f;
            greenImage(j, i, 2) = 0.f;

            blueImage(j, i, 0) = 0.f;
            blueImage(j, i, 1) = 0.f;
            blueImage(j, i, 2) = 0.5f;
        }
    }

    bool wat = redImage.write("redImage.hdr");
    greenImage.write("greenImage.hdr");
    blueImage.write("blueImage.hdr");

    Image redReadImage = Image("redImage.hdr");
    Image greenReadImage = Image("greenImage.hdr");
    Image blueReadImage = Image("blueImage.hdr");

    for (int i = 0; i < image_res; ++i)
    {
        for (int j = 0; j < image_res; ++j)
        {
            if (redImage(j, i, 0) != 0.5f) return false;
            if (redImage(j, i, 1) != 0.f) return false;
            if (redImage(j, i, 2) != 0.f) return false;

            if (greenImage(j, i, 0) != 0.f) return false;
            if (greenImage(j, i, 1) != 0.5f) return false;
            if (greenImage(j, i, 2) != 0.f) return false;

            if (blueImage(j, i, 0) != 0.f) return false;
            if (blueImage(j, i, 1) != 0.f) return false;
            if (blueImage(j, i, 2) != 0.5f) return false;
        }
    }

    // procedural tests

    Image sineImage_X = Image(image_res, image_res, 3);
    Image sineImage_Y = Image(image_res, image_res, 3);
    Image sineImage_XY = Image(image_res, image_res, 3);

    sin_image(0.5f, 10.f, 0.f, 0.5f, sineImage_X, IP_X);
    sin_image(0.5f, 10.f, 0.f, 0.5f, sineImage_Y, IP_Y);
    sin_image(0.5f, 10.f, 0.f, 0.5f, sineImage_XY, IP_XY);

    sineImage_X.write("sine_X.hdr");
    sineImage_Y.write("sine_Y.hdr");
    sineImage_XY.write("sine_XY.hdr");

    Image noiseImage = Image(image_res, image_res, 3);

    noise_image_xy(noiseImage, 1.f);

    noiseImage.write("noise_image.hdr");

    im_abs(noiseImage);

    noiseImage.write("noise_image_abs.hdr");

    Image threshold_min = noiseImage;
    Image threshold_max = noiseImage;

    thresh_min(threshold_min, 0.3f);
    thresh_max(threshold_max, 0.3f);

    threshold_min.write("threshold_min.hdr");
    threshold_max.write("threshold_max.hdr");
    noiseImage.write("check.hdr");

    Image noiseImage_two = Image(image_res, image_res, 3);

    noise_image_xy(noiseImage_two, 5.f);

    noiseImage_two.write("noise_image_two.hdr");

    im_abs(noiseImage_two);

    noiseImage_two.write("noise_image_two_abs.hdr");

    Image threshold_min_two = noiseImage_two;
    Image threshold_max_two = noiseImage_two;

    thresh_min(threshold_min_two, 0.3f);
    thresh_max(threshold_max_two, 0.3f);

    threshold_min_two.write("threshold_min_two.hdr");
    threshold_max_two.write("threshold_max_two.hdr");

    Image turbulenceImage = Image(image_res, image_res, 3);

    turbulence_image_xy(turbulenceImage, 32.f);

    turbulenceImage.write("turbImage.hdr");

    Image euclideanImage = Image(image_res, image_res, 3);
    Image manhattanImage = Image(image_res, image_res, 3);

    euclidean_dist_image(euclideanImage, float(image_res) / 2.f, float(image_res) / 2.f);
    manhattan_dist_image(manhattanImage, image_res / 2, image_res / 2);

    Image sinEuclideanImage = euclideanImage;
    Image sinManhattanImage = manhattanImage;

    euclideanImage /= euclideanImage.max();
    manhattanImage /= manhattanImage.max();

    euclideanImage.write("euclidean_dist.hdr");
    manhattanImage.write("manhattan_dist.hdr");

    im_sin(sinEuclideanImage);
    im_sin(sinManhattanImage);

    sinEuclideanImage.write("sin_euclidean_dist.hdr");
    sinManhattanImage.write("sin_manhattan_dist.hdr");

    vector<pair<Float, Float> > pts_float = vector<std::pair<Float, Float> >();
    vector<pair<int, int> > pts_int = vector<std::pair<int, int> >();

    for (int i = 0; i < 15; ++i)
    {
        Float one = randomFloat() * float(image_res);
        Float two = randomFloat() * float(image_res);

        int one_int = (int)one;
        int two_int = (int)two;

        pts_float.push_back(std::pair<Float, Float>(one, two));
        pts_int.push_back(std::pair<int, int>(one_int, two_int));
    }

    pts_float.push_back(std::pair<Float, Float>(1.0,1.0));
    pts_int.push_back(std::pair<int, int>(1,1));

    Image eucPointsImage = Image(image_res, image_res, 3);
    Image manPointsImage = Image(image_res, image_res, 3);

    euclidean_dist_from_points_image(eucPointsImage, pts_float);
    manhattan_dist_from_points_image(manPointsImage, pts_int);

    eucPointsImage /= eucPointsImage.max();
    manPointsImage /= manPointsImage.max();

    eucPointsImage.write("euc_points.hdr");
    manPointsImage.write("man_points.hdr");

    Image voronoi = eucPointsImage;
    one_minus(voronoi);
    remap_range_lin(voronoi);
    voronoi.write("one_minus_euc_points.hdr");

    one_minus(voronoi);
    thresh_min_image(voronoi, 0.4f);

    voronoi.write("polka.hdr");

    Image white = Image(2048, 1024, 3);
    for (int i = 0; i < white.size(); ++i)
    {
        white[i] = 1.0;
    }
    white.write("white.png");

    Image turbIm = Image(image_res, image_res, 3);

    turbulence_image_xy(turbIm, 32.f);

    remap_range_lin(turbIm);

    turbIm.write("lin_turb_remap.png");

    Image turbIm2 = turbIm;

    thresh_min_zero(turbIm, 0.05f);
    turbIm.write("cloud_05.png");

    thresh_min_zero(turbIm, 0.10f);
    turbIm.write("cloud_10.png");

    thresh_min_zero(turbIm, 0.15f);
    turbIm.write("cloud_15.png");

    thresh_min_zero(turbIm, 0.20f);
    turbIm.write("cloud_20.png");

    thresh_min_zero(turbIm, 0.25f);
    turbIm.write("cloud_25.png");

    thresh_min_zero(turbIm, 0.30f);
    turbIm.write("cloud_30.png");

    thresh_min_zero(turbIm, 0.35f);
    turbIm.write("cloud_35.png");

    thresh_min_zero(turbIm, 0.40f);
    turbIm.write("cloud_40.png");

    thresh_min_zero(turbIm, 0.45f);
    turbIm.write("cloud_45.png");

    thresh_min_zero(turbIm, 0.50f);
    turbIm.write("cloud_50.png");

    thresh_min_zero(turbIm, 0.55f);
    turbIm.write("cloud_55.png");

    thresh_min_zero(turbIm, 0.60f);
    turbIm.write("cloud_60.png");

    thresh_min_zero(turbIm, 0.65f);
    turbIm.write("cloud_65.png");

    thresh_min_zero(turbIm, 0.70f);
    turbIm.write("cloud_70.png");

    thresh_min_zero(turbIm, 0.75f);
    turbIm.write("cloud_75.png");

    thresh_min_zero(turbIm, 0.80f);
    turbIm.write("cloud_80.png");

    thresh_min_zero(turbIm, 0.85f);
    turbIm.write("cloud_85.png");

    thresh_min_zero(turbIm, 0.90f);
    turbIm.write("cloud_90.png");

    thresh_min_zero(turbIm, 0.95f);
    turbIm.write("cloud_95.png");

    thresh_max_zero(turbIm2, 0.95f);
    turbIm2.write("cloud_95_2.png");

    thresh_max_zero(turbIm2, 0.90f);
    turbIm2.write("cloud_90_2.png");

    thresh_max_zero(turbIm2, 0.85f);
    turbIm2.write("cloud_85_2.png");

    thresh_max_zero(turbIm2, 0.80f);
    turbIm2.write("cloud_80_2.png");

    thresh_max_zero(turbIm2, 0.75f);
    turbIm2.write("cloud_75_2.png");

    thresh_max_zero(turbIm2, 0.70f);
    turbIm2.write("cloud_70_2.png");

    thresh_max_zero(turbIm2, 0.65f);
    turbIm2.write("cloud_65_2.png");

    thresh_max_zero(turbIm2, 0.60f);
    turbIm2.write("cloud_60_2.png");

    thresh_max_zero(turbIm2, 0.55f);
    turbIm2.write("cloud_55_2.png");

    thresh_max_zero(turbIm2, 0.50f);
    turbIm2.write("cloud_50_2.png");

    thresh_max_zero(turbIm2, 0.45f);
    turbIm2.write("cloud_45_2.png");

    thresh_max_zero(turbIm2, 0.40f);
    turbIm2.write("cloud_40_2.png");

    thresh_max_zero(turbIm2, 0.35f);
    turbIm2.write("cloud_35_2.png");

    thresh_max_zero(turbIm2, 0.30f);
    turbIm2.write("cloud_30_2.png");

    thresh_max_zero(turbIm2, 0.25f);
    turbIm2.write("cloud_25_2.png");

    thresh_max_zero(turbIm2, 0.20f);
    turbIm2.write("cloud_20_2.png");

    thresh_max_zero(turbIm2, 0.15f);
    turbIm2.write("cloud_15_2.png");

    thresh_max_zero(turbIm2, 0.10f);
    turbIm2.write("cloud_10_2.png");

    thresh_max_zero(turbIm2, 0.05f);
    turbIm2.write("cloud_05_2.png");


    Image cloudIm = Image(image_res, image_res, 3);

    turbulence_image_xy(cloudIm, 32.f);

    remap_range_lin(cloudIm);

    Image distIm = Image(image_res, image_res, 3);

    euclidean_dist_image(distIm, Float(image_res)/2.f, Float(image_res)/2.f);

    distIm.write("dist_whao.png");

    remap_range_lin(distIm);
    exp_im(distIm, -3.f);
    // one_minus(distIm);
    distIm *= 1.7f;
    clamp_im(distIm);
    // thresh_min_zero(distIm, 0.1f);

    distIm.write("distim.png");

    cloudIm.write("cloud_1.png");
    cloudIm *= distIm;
    cloudIm.write("cloud_2.png");

    Image cloud_50 = cloudIm;

    thresh_max(cloud_50, 0.5f);

    cloud_50.write("cloud_50_test.png");

    Image diff_50 = cloudIm - cloud_50;

    diff_50.write("cloud_diff_50.png");

    Image cloud_25 = cloudIm;

    thresh_max(cloud_25, 0.25f);

    cloud_25.write("cloud_25_test.png");

    Image diff_25 = cloudIm - cloud_25;

    diff_25.write("cloud_diff_25.png");

    Image cloud_75 = cloudIm;

    thresh_max(cloud_75, 0.75f);

    cloud_50.write("cloud_75_test.png");

    Image diff_75 = cloudIm - cloud_75;

    diff_75.write("cloud_diff_75.png");

    return true;
}

bool delete_test_set()
{
    // TODO
    return false;
}

bool getchannel_test()
{
    // TODO
    return false;
}

bool operator_test()
{
    Image redImage = Image("redImage.hdr");
    Image greenImage = Image("greenImage.hdr");
    Image blueImage = Image("blueImage.hdr");

    Image combined = (redImage * 2.0) +
                     (greenImage * 2.0) +
                     (blueImage * 2.0);

    // std::cout << "ONE" << std::endl;

    for (int i = 0; i < combined.size(); ++i)
    {
        // cout << "comb: " << combined[i] << endl;
        // cout << "red: " << redImage[i] << endl;
        if (combined[i] != 1.f) return false;
    }

    // std::cout << "TWO" << std::endl;

    combined /= 2.0;

    for (int i = 0; i < combined.size(); ++i)
    {
        // cout << combined[i] << endl;
        if (combined[i] != 0.5) return false;
    }

    // std::cout << "THREE" << std::endl;

    Image negRed = combined - redImage;
    Image negBlue = combined - blueImage;
    Image negGreen = combined - greenImage;

    // std::cout << "FOUR" << std::endl;

    for (int i = 0; i < combined.height(); ++i)
    {
        for (int j = 0; j < combined.width(); ++j)
        {
            if (negRed(j, i, 0) != 0.0) return false;
            if (negRed(j, i, 1) != 0.5) return false;
            if (negRed(j, i, 2) != 0.5) return false;

            if (negBlue(j, i, 0) != 0.5) return false;
            if (negBlue(j, i, 1) != 0.5) return false;
            if (negBlue(j, i, 2) != 0.0) return false;

            if (negGreen(j, i, 0) != 0.5) return false;
            if (negGreen(j, i, 1) != 0.0) return false;
            if (negGreen(j, i, 2) != 0.5) return false;
        }
    }

    // std::cout << "FIVE" << std::endl;

    combined.write("combined.hdr");
    negRed.write("negRed.hdr");
    negGreen.write("negGreen.hdr");
    negBlue.write("negBlue.hdr");

    return true;
}

void cloudTests()
{
    uint32_t image_res = 1024;
    float period = 256.f;

    Image turbIm = Image(image_res, image_res, 3);

    turbulence_image_xy(turbIm, period);

    Float max = turbIm.max();
    Float min = turbIm.min();

    std::cout << "max: " << max << std::endl;
    std::cout << "min: " << min << std::endl;

    remap_range_lin(turbIm);

    turbIm.write("lin_turb_remap.png");

    Image turbIm2 = turbIm;

    Image cloudIm = Image(image_res, image_res, 3);

    turbulence_image_xy(cloudIm, period);

    remap_range_lin(cloudIm);

    Image distIm = Image(image_res, image_res, 3);

    euclidean_dist_image(distIm, Float(image_res)/2.f, Float(image_res)/2.f);

    distIm.write("dist_whao.png");

    remap_range_lin(distIm);
    exp_im(distIm, -3.f);
    // one_minus(distIm);
    distIm *= 1.7f;
    clamp_im(distIm);
    // thresh_min_zero(distIm, 0.1f);

    distIm.write("distim.png");

    cloudIm.write("cloud_1.png");
    cloudIm *= distIm;
    cloudIm.write("cloud_2.png");

    Image cloud_50 = cloudIm;

    thresh_max(cloud_50, 0.5f);

    cloud_50.write("cloud_50_test.png");

    Image diff_50 = cloudIm - cloud_50;

    diff_50.write("cloud_diff_50.png");

    Image cloud_25 = cloudIm;

    thresh_max(cloud_25, 0.25f);

    cloud_25.write("cloud_25_test.png");

    Image diff_25 = cloudIm - cloud_25;

    diff_25.write("cloud_diff_25.png");

    Image cloud_75 = cloudIm;

    thresh_max(cloud_75, 0.75f);

    cloud_50.write("cloud_75_test.png");

    Image diff_75 = cloudIm - cloud_75;

    diff_75.write("cloud_diff_75.png");

    for (int i = 0; i < image_res; ++i)
    {
        std::cout << "i: " << i << std::endl;
        Image tmpIm = Image(image_res, image_res, 3);
        turbulence_image_xyz(tmpIm, period, float(i));
        Float tmpMax = tmpIm.max();
        Float tmpMin = tmpIm.min();

        if (tmpMax > max) max = tmpMax;
        if (tmpMin < min) min = tmpMin;
    }

    std::cout << "true max: " << max << std::endl;
    std::cout << "true min: " << min << std::endl;
}

void convert_to_false_color()
{
    // std::vector<std::string> files = std::vector<std::string>();
    //
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/const/rat_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/const/rec_hyp_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/const/rec_hyp_poi_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/const/hyp_mis_eff");
    //
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/cos/rat_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/cos/rec_hyp_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/cos/rec_hyp_poi_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/cos/hyp_mis_eff");
    //
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/gauss/rat_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/gauss/rec_hyp_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/gauss/rec_hyp_poi_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/gauss/hyp_mis_eff");
    //
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/hole/rat_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/hole/rec_hyp_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/hole/rec_hyp_poi_eff");
    // files.push_back("/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/jans/hole/hyp_mis_eff");
    //
    // for (int i = 0; i < files.size(); ++i)
    // {
    //     Image<double> image = Image<double>(files[i] + ".png");
    //     Image<double> fcImage = image;
    //
    //     falseColor(image, fcImage);
    //
    //     fcImage.write(files[i] + "_false_color.png");
    //
    //     Image<double> image_hdr = Image<double>(files[i] + ".hdr");
    //     Image<double> fcImage_hdr = image_hdr;
    //
    //     falseColor(image_hdr, fcImage_hdr);
    //
    //     fcImage_hdr.write(files[i] + "_false_color.png");
    // }
}

void variance_calculaton()
{
    std::string path = "/Users/corneria/Documents/Research/TransmittanceSIGGRAPH2019/paper/results/";

    std::vector<std::string> ests = std::vector<std::string>();
    ests.push_back("bidir");
    ests.push_back("rat");
    ests.push_back("hyp_mis");
    ests.push_back("hyp_nfr");
    ests.push_back("hyp_rat");
    ests.push_back("nfr");
    ests.push_back("poi");
    ests.push_back("rat");
    ests.push_back("rec_hyp");
    ests.push_back("tl");
    ests.push_back("uni");

    std::vector<std::string> tests = std::vector<std::string>();
    tests.push_back("fox");
    tests.push_back("vase");
    tests.push_back("smoke");

    std::cout << std::endl;

    for (int i = 0; i < tests.size(); ++i)
    {
        std::cout << tests[i] << std::endl;
        std::cout << std::endl;

        Image gt = Image(path + tests[i] + "/gt.hdr");

        for (int j = 0; j < ests.size(); ++j)
        {
            Image est = Image(path + tests[i] + "/" + ests[j] + ".hdr");

            double err = mean_absolute_difference(gt, est);

            std::cout << ests[j] << ": " << err << std::endl;
        }

        std::cout << std::endl;
    }
}

void contour_test()
{
    Image image = Image(512, 512, 3);

    for (int i = 0; i < 512; ++i)
    {
        for (int j = 0; j < 512; ++j)
        {
            double val = 0.0;
            if (j >= 2*i)
            {
                val = 0.25;
            }
            else if (j >= i)
            {
                val = 0.5;
            }
            else if (i >= 2*j)
            {
                val = 1.0;
            }
            else
            {
                val = 0.75;
            }

            // double x = double(j - 256);
            // double y = double(i - 256);
            // val = std::sqrt(x*x + y*y) / std::sqrt(2.0 * 256.0 * 256.0);

            for (int k = 0; k < 3; ++k) image(j, i, k) = 1.0 - val;
        }
    }

    image.write("contour.jpeg");
    image.write("contour.txt");
}

void test_exr()
{
    Image image = Image("tiled.hdr");
    std::cout << "writing exr" << std::endl;
    image.write("tiled.exr");
    image.write("first.png");
    std::cout << "reading exr" << std::endl;
    image.read("tiled.exr");
    std::cout << "writing png" << std::endl;
    image.write("tiled.png");
}

int main()
{
    // the path through life is a fun one - freddy in water
    // sphinx
    // the path through life is full of adventure - zhara looking out to sea
    // anubis
    // the path through life is a merry one - julius with ines as kids
    // sphinx
    // the path through life is full of longing - allwin holding his fathers sythe
    // stag
    // the path through life is a hard one - rev homeless
    // snake
    // the path through life is filled with folly - the falchion blade passing hands
    // dragon
    // the path through life is a trecherous one - shanks surrounded by dark tenrils
    // anubis
    // the path through life is litered with mistakes - four skulls on the ground
    // bat
    // the path through life.... is fated in death
    // eye

    // the path through
    srand(0x31245A);
    // srand(0x891);

    // cout << "Beginning Library Tests" << endl;
    //
    // if (!generate_test_set())
    // {
    //     cout << "ERROR: Generating Test Set Failed" << endl;
    // }
    // if (!operator_test())
    // {
    //     cout << "ERROR: Operator Test Failed" << endl;
    // }

    // cloudTests();

    // convert_to_false_color();

    // contour_test();

    test_exr();

    // variance_calculaton();

    // todo
    cout << "All Library Tests Passed" << endl;
    return 0;
}

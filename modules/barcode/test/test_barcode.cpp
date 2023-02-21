// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"

namespace opencv_test{namespace{
//init validation map
datasetType initValidation(std::string path)
{
    const std::string valid_path = findDataFile(path);
    return buildDataSet(valid_path);
}

std::vector<std::string> splitBy(std::ifstream& input, char delimiter) {
    std::string segment;
    std::vector<std::string> ret;

    std::string line;
    std::getline(input, line);

    std::istringstream line_stream(line);

    while(std::getline(line_stream, segment, delimiter)) {
        ret.push_back(segment);
    }
    return ret;
}

std::vector<std::string> myBuildDataSet(const std::string& result_file_path) {
    std::ifstream result_file;
    datasetType dataset;
    result_file.open(result_file_path);
    std::string line;
    if (!result_file.is_open()) {
        throw std::invalid_argument("file not found");
    }

    std::vector<std::string> barcodes = splitBy(result_file, ',');
    result_file.close();
    return barcodes;
}

void test_generic(const std::string& filestem) {
    const std::string valid_path = findDataFile(filestem + ".csv");
    std::vector<std::string> expected_codes = myBuildDataSet(valid_path);
    auto bardet = barcode::BarcodeDetector();

    std::string image_path = findDataFile(filestem + ".jpg");
    Mat img = imread(image_path);
    EXPECT_FALSE(img.empty()) << "Can't read image: " << image_path;
    std::vector<cv::Point2f> points;
    std::vector<std::string> infos;
    std::vector<cv::barcode::BarcodeType> formats;
    bardet.detectAndDecode(img, infos, formats, points);
    EXPECT_EQ(infos, expected_codes);
}

TEST(Barcode_BarcodeDetector_small_all, regression)
{
    const std::string root = "barcode/small/";
    test_generic(root + "baron_perche");
    test_generic(root + "vance");
}

TEST(Barcode_BarcodeDetector_single, regression)
{
    const std::string root = "barcode/single/";
    datasetType validation = initValidation(root + "result.csv");
    auto bardet = barcode::BarcodeDetector();
    datasetType::iterator iterator = validation.begin();
    while (iterator != validation.end())
    {
        std::string img_name = iterator->first;
        std::string result = iterator->second;
        std::string image_path = findDataFile(root + img_name);
        Mat img = imread(image_path);
        EXPECT_FALSE(img.empty()) << "Can't read image: " << image_path;
        std::vector<cv::Point2f> points;
        std::vector<std::string> infos;
        std::vector<cv::barcode::BarcodeType> formats;
        bardet.detectAndDecode(img, infos, formats, points);
        EXPECT_FALSE(points.empty()) << "Nothing detected: " << image_path;
        bool is_correct = false;
        for (const auto &ans : infos)
        {
            if (ans == result)
            {
                is_correct = true;
                break;
            }
        }
        EXPECT_TRUE(is_correct) << "No results for " << img_name;
        iterator++;
    }
}

TEST(Barcode_BarcodeDetector_detect_multi, detect_regression)
{
    const std::string root = "barcode/multiple/";
    datasetType validation = initValidation(root + "result.csv");
    auto bardet = barcode::BarcodeDetector();
    datasetType::iterator iterator = validation.begin();
    while (iterator != validation.end())
    {
        std::string img = iterator->first;
        size_t expect_corners_size = std::stoi(iterator->second);
        std::string image_path = findDataFile(root + img);
        Mat src = imread(image_path);
        EXPECT_FALSE(src.empty()) << "Can't read image: " << image_path;

        std::vector<Point> corners;
        bardet.detect(src, corners);
        EXPECT_EQ(corners.size(), expect_corners_size) << "Can't detect all barcodes: " << img;
        iterator++;
    }
}

TEST(Barcode_BarcodeDetector_basic, not_found_barcode)
{
    auto bardet = barcode::BarcodeDetector();
    std::vector<Point> corners;
    vector<cv::String> decoded_info;
    vector<barcode::BarcodeType> decoded_type;
    Mat zero_image = Mat::zeros(256, 256, CV_8UC1);
    EXPECT_FALSE(bardet.detect(zero_image, corners));
    corners = std::vector<Point>(4);
    EXPECT_ANY_THROW(bardet.decode(zero_image, corners, decoded_info, decoded_type));
}
}} // namespace

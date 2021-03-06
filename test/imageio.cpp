/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <gtest/gtest.h>
#include <arrayfire.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <testHelpers.hpp>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using af::af_cfloat;
using af::af_cdouble;

template<typename T>
class ImageIO : public ::testing::Test
{
    public:
        virtual void SetUp() {
        }
};

typedef ::testing::Types<float> TestTypes;

// register the type list
TYPED_TEST_CASE(ImageIO, TestTypes);

// Disable tests if FreeImage is not found
#if defined(WITH_FREEIMAGE)
void loadImageTest(string pTestFile, string pImageFile, const bool isColor)
{
    vector<af::dim4> numDims;

    vector<vector<float>>   in;
    vector<vector<float>>   tests;
    readTests<float, float, float>(pTestFile,numDims,in,tests);
    af::dim4 dims       = numDims[0];

    af_array imgArray = 0;
    ASSERT_EQ(AF_SUCCESS, af_load_image(&imgArray, pImageFile.c_str(), isColor));

    // Get result
    float *imgData = new float[dims.elements()];
    ASSERT_EQ(AF_SUCCESS, af_get_data_ptr((void*) imgData, imgArray));

    // Compare result
    size_t nElems = in[0].size();
    for (size_t elIter = 0; elIter < nElems; ++elIter) {
        ASSERT_EQ(in[0][elIter], imgData[elIter]) << "at: " << elIter << std::endl;
    }

    // Delete
    delete[] imgData;

    if(imgArray != 0) af_destroy_array(imgArray);
}

TYPED_TEST(ImageIO, ColorSmall)
{
    loadImageTest(string(TEST_DIR"/imageio/color_small.test"), string(TEST_DIR"/imageio/color_small.png"), true);
}

TYPED_TEST(ImageIO, GraySmall)
{
    loadImageTest(string(TEST_DIR"/imageio/gray_small.test"), string(TEST_DIR"/imageio/gray_small.jpg"), false);
}

TYPED_TEST(ImageIO, GraySeq)
{
    loadImageTest(string(TEST_DIR"/imageio/gray_seq.test"), string(TEST_DIR"/imageio/gray_seq.png"), false);
}

TYPED_TEST(ImageIO, ColorSeq)
{
    loadImageTest(string(TEST_DIR"/imageio/color_seq.test"), string(TEST_DIR"/imageio/color_seq.png"), true);
}

void loadimageArgsTest(string pImageFile, const bool isColor)
{
    af_array imgArray = 0;

    ASSERT_EQ(AF_ERR_ARG, af_load_image(&imgArray, pImageFile.c_str(), isColor));

    if(imgArray != 0) af_destroy_array(imgArray);
}

TYPED_TEST(ImageIO,InvalidArgsMissingFile)
{
    loadimageArgsTest(string(TEST_DIR"/imageio/nofile.png"), false);
}

TYPED_TEST(ImageIO,InvalidArgsWrongExt)
{
    loadimageArgsTest(string(TEST_DIR"/imageio/image.wrongext"), true);
}

////////////////////////////////// CPP //////////////////////////////////////
TEST(ImageIO, CPP)
{
    vector<af::dim4> numDims;

    vector<vector<float>>   in;
    vector<vector<float>>   tests;
    readTests<float, float, float>(string(TEST_DIR"/imageio/color_small.test"),numDims,in,tests);

    af::dim4 dims = numDims[0];
    af::array img = af::loadImage(string(TEST_DIR"/imageio/color_small.png").c_str(), true);

    // Get result
    float *imgData = new float[dims.elements()];
    img.host((void*)imgData);

    // Compare result
    size_t nElems = in[0].size();
    for (size_t elIter = 0; elIter < nElems; ++elIter) {
        ASSERT_EQ(in[0][elIter], imgData[elIter]) << "at: " << elIter << std::endl;
    }

    // Delete
    delete[] imgData;
}
#endif // WITH_FREEIMAGE

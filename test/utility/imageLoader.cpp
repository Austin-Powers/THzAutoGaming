#include "THzAutoGaming/utility/imageLoader.hpp"

#include "THzImage/common/image.hpp"
#include "THzImage/io/qoiWriter.hpp"
#include "THzImage/io/testImageGenerator.hpp"
#include "THzImage/processing/dataReductionNode.hpp"
#include "THzImage/processing/imageInputNode.hpp"

#include <filesystem>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct UtilityImageLoader : public testing::Test
{
    static void SetUpTestSuite()
    {
        BGRAImage          image{};
        TestImageGenerator generator{Rectangle{16U, 16U}};
        (void)image.readFrom(generator);

        std::filesystem::create_directories("imageLoaderTest/subDir");

        QOI::Writer writer0{"imageLoaderTest/image0.qoi"};
        (void)writer0.writeContentOf(image);

        QOI::Writer writer1{"imageLoaderTest/subDir/image1.qoi"};
        (void)writer1.writeContentOf(image);
    }

    static void TearDownTestSuite() { std::filesystem::remove_all("imageLoaderTest"); }
};

TEST_F(UtilityImageLoader, NoImageAreLoadedIfNoneAreRegistered)
{
    ImageLoader sut{"imageLoaderTest"};
    EXPECT_EQ(sut.loadImages(), 0U);
}

TEST_F(UtilityImageLoader, LoadingNonExistentImages)
{
    ImageLoader sut{"imageLoaderTest"};

    BGRAImage    bgraBuffer{};
    MiniHSVImage miniHSVBuffer{};

    sut.registerImage(bgraBuffer, "myImage.qoi");
    sut.registerImage(miniHSVBuffer, "myOtherImage.qoi");
    EXPECT_EQ(sut.loadImages(), 0U);

    Rectangle const defaultDimensions{};
    EXPECT_EQ(bgraBuffer.dimensions(), defaultDimensions);
    EXPECT_EQ(miniHSVBuffer.dimensions(), defaultDimensions);
}

TEST_F(UtilityImageLoader, NumberOfLoadedImagesPutOutCorrectly)
{
    ImageLoader sut{"imageLoaderTest"};
    BGRAImage   image0{};
    BGRAImage   image1{};

    sut.registerImage(image0, "image0.qoi");
    sut.registerImage(image1, "subDir/image2.qoi");

    EXPECT_EQ(sut.loadImages(), 1U);
}

TEST_F(UtilityImageLoader, LoadingBGRAImages)
{
    ImageLoader sut{"imageLoaderTest"};
    BGRAImage   image0{};
    BGRAImage   image1{};

    sut.registerImage(image0, "image0.qoi");
    sut.registerImage(image1, "subDir/image1.qoi");
    EXPECT_EQ(sut.loadImages(), 2U);

    BGRAImage          expectedImage{};
    TestImageGenerator generator{Rectangle{16U, 16U}};
    (void)expectedImage.readFrom(generator);

    EXPECT_EQ(image0, expectedImage);
    EXPECT_EQ(image1, expectedImage);
}

TEST_F(UtilityImageLoader, LoadingMiniHSVImages)
{
    ImageLoader  sut{"imageLoaderTest"};
    MiniHSVImage image0{};
    MiniHSVImage image1{};

    sut.registerImage(image0, "image0.qoi");
    sut.registerImage(image1, "subDir/image1.qoi");
    EXPECT_EQ(sut.loadImages(), 2U);

    ImageProcessing::ImageInputNode baseImage{};
    TestImageGenerator              generator{Rectangle{16U, 16U}};
    (void)baseImage.image().readFrom(generator);
    ImageProcessing::DataReductionNode expectedImage{baseImage, 1U, 1U};

    EXPECT_EQ(image0, expectedImage[0U]);
    EXPECT_EQ(image1, expectedImage[0U]);
}

TEST_F(UtilityImageLoader, LoadingSameImageIntoMiniHSVAndBGRA)
{
    ImageLoader  sut{"imageLoaderTest"};
    BGRAImage    image0{};
    BGRAImage    image1{};
    MiniHSVImage image2{};
    MiniHSVImage image3{};

    sut.registerImage(image0, "image0.qoi");
    sut.registerImage(image1, "subDir/image1.qoi");
    sut.registerImage(image2, "image0.qoi");
    sut.registerImage(image3, "subDir/image1.qoi");
    EXPECT_EQ(sut.loadImages(), 4U);

    ImageProcessing::ImageInputNode baseImage{};
    TestImageGenerator              generator{Rectangle{16U, 16U}};
    (void)baseImage.image().readFrom(generator);
    ImageProcessing::DataReductionNode expectedImage{baseImage, 1U, 1U};

    EXPECT_EQ(image0, baseImage.image());
    EXPECT_EQ(image1, baseImage.image());
    EXPECT_EQ(image2, expectedImage[0U]);
    EXPECT_EQ(image3, expectedImage[0U]);
}

} // namespace Terrahertz::UnitTests

#include "THzAutoGaming/utility/imageLoader.hpp"

#include "THzCommon/logging/logging.hpp"
#include "THzImage/io/autoFileReader.hpp"
#include "THzImage/processing/dataReductionNode.hpp"
#include "THzImage/processing/imageInputNode.hpp"

namespace Terrahertz {

/// @brief Name provider for THzAutoGaming.Utility.ImageLoader class.
struct ImageLoaderProject
{
    static constexpr char const *name() noexcept { return "THzAutoGaming.Utility.ImageLoader"; }
};

ImageLoader::ImageLoader(std::filesystem::path const directorypath, std::uint8_t const scaleFactor) noexcept
    : _directorypath{directorypath}, _scaleFactor{scaleFactor}
{
    Logger::globalInstance().addProject<ImageLoaderProject>();
}

void ImageLoader::registerImage(BGRAImage &buffer, std::filesystem::path const filepath) noexcept
{
    _bgraImages.emplace((_directorypath / filepath), &buffer);
}

void ImageLoader::registerImage(MiniHSVImage &buffer, std::filesystem::path const filepath) noexcept
{
    _miniHsvImages.emplace(filepath, &buffer);
}

std::uint16_t ImageLoader::loadImages() noexcept
{
    if (_bgraImages.empty() && _miniHsvImages.empty())
    {
        return 0U;
    }

    std::uint16_t loadedImages{};

    ImageProcessing::ImageInputNode    temp{};
    ImageProcessing::DataReductionNode dataReduction{temp, _scaleFactor, 1U};
    for (auto const &dirEntry : std::filesystem::recursive_directory_iterator(_directorypath))
    {
        if (dirEntry.is_directory())
        {
            continue;
        }
        auto const &path = dirEntry.path();
        if (_bgraImages.contains(path))
        {
            logMessage<LogLevel::Info, ImageLoaderProject>("Loading BGRAImage:");
            logMessage<LogLevel::Info, ImageLoaderProject>(path.string());
            AutoFile::Reader reader{path};
            if (_bgraImages[path]->readFrom(reader))
            {
                _bgraImages.erase(path);
                ++loadedImages;
            }
        }
        if (_miniHsvImages.contains(path))
        {
            logMessage<LogLevel::Info, ImageLoaderProject>("Loading MiniHSVImage:");
            logMessage<LogLevel::Info, ImageLoaderProject>(dirEntry.path().string());
            AutoFile::Reader reader{path};
            if (temp.image().readFrom(reader))
            {
                if (dataReduction.next())
                {
                    *(_miniHsvImages[path]) = dataReduction[0U];
                    _miniHsvImages.erase(path);
                    ++loadedImages;
                }
            }
        }
    }
    return loadedImages;
}

} // namespace Terrahertz

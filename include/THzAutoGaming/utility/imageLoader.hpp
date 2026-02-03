#ifndef THZ_AUTOGAMING_UTILITY_IMAGELOADER_HPP
#define THZ_AUTOGAMING_UTILITY_IMAGELOADER_HPP

#include "THzImage/common/image.hpp"

#include <cstdint>
#include <filesystem>
#include <map>

namespace Terrahertz {

/// @brief Centralizes the loading of images for actions.
class ImageLoader
{
public:
    /// @brief Initializes a new ImageLoader using the given values.
    ///
    /// @param directorypath The path of the directory from where to load the images.
    /// @param scaleFactor The scale factor applied to the images on reduction.
    ImageLoader(std::filesystem::path const directorypath, std::uint8_t const scaleFactor = 1U) noexcept;

    /// @brief Registers a BGRAImage to load.
    ///
    /// @param buffer The buffer into which to put the image.
    /// @param filepath The path of the image inside the image directory.
    /// @remark The BGRAImages will not be scaled.
    void registerImage(BGRAImage &buffer, std::filesystem::path const filepath) noexcept;

    /// @brief Registers a MiniHSVImage to load.
    ///
    /// @param buffer The buffer into which to put the image.
    /// @param filepath The path of the image inside the image directory.
    void registerImage(MiniHSVImage &buffer, std::filesystem::path const filepath) noexcept;

    /// @brief Loads the images from the given directory into the buffers given to the load previously.
    ///
    /// @return The number of images that have been loaded succesfully.
    /// @remark If an image was succesfully loaded the buffer is removed from the internal listing.
    std::uint16_t loadImages() noexcept;

private:
    /// @brief The path of directory from where to load the images.
    std::filesystem::path _directorypath;

    /// @brief The scale factor applied to the images on reduction.
    std::uint8_t _scaleFactor;

    /// @brief Maps the paths of all BGRAImages to load their buffers.
    std::map<std::filesystem::path, BGRAImage *> _bgraImages{};

    /// @brief Maps the paths of all MiniHSVImages to load their buffers.
    std::map<std::filesystem::path, MiniHSVImage *> _miniHsvImages{};
};

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_UTILITY_IMAGELOADER_HPP

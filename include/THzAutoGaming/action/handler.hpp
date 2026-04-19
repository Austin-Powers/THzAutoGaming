#ifndef THZ_AUTOGAMING_ACTION_HANDLER_HPP
#define THZ_AUTOGAMING_ACTION_HANDLER_HPP

#include "THzAutoGaming/action/iTriggerAction.hpp"
#include "THzImage/common/pixel.hpp"
#include "THzImage/processing/iNode.hpp"

namespace Terrahertz::Action {

/// @brief Uses processed screenshots to determine what actions to perform.
class Handler
{
public:
    /// @brief Initializes a new ActionHandler.
    ///
    /// @param input The node used for inputting images the handler works with.
    Handler(ImageProcessing::INode<BGRAPixel> *const imageInput) noexcept;

    /// @brief Adds a new trigger action the the handler.
    ///
    /// @param action The trigger action to add.
    void addTriggerAction(ITriggerAction *const action) noexcept;

    /// @brief Retrieves the next image from the input node and performs actions based on the image.
    ///
    /// @return True if operation was successful, false otherwise.
    bool next() noexcept;

    // TODO Add ScannerAction that will be handed an image (BGRA or MINIHSV) and scans it
};

} // namespace Terrahertz::Action

#endif // !THZ_AUTOGAMING_ACTION_HANDLER_HPP

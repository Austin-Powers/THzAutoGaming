#ifndef THZ_AUTOGAMING_ACTION_HANDLER_HPP
#define THZ_AUTOGAMING_ACTION_HANDLER_HPP

#include "THzAutoGaming/action/iAction.hpp"
#include "THzAutoGaming/input/emulator.hpp"
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
    Handler(ImageProcessing::INode<BGRAPixel> *const imageInput, Input::Emulator *const emulator) noexcept;

    /// @brief Adds a new action the the handler.
    ///
    /// @param action The action to add.
    void addAction(IAction *const action) noexcept;

    /// @brief Retrieves the next image from the input node and performs actions based on the image.
    ///
    /// @return True if operation was successful, false otherwise.
    bool next() noexcept;

    // TODO Add ScannerAction and TriggerAction
    // Scanner will get an image (BGRA or MINIHSV) and scan the image for stuff
    // Trigger will Akt like the Current IAction
    // TriggerActions can be dealt with by a specific internal ScannerAction
};

} // namespace Terrahertz::Action

#endif // !THZ_AUTOGAMING_ACTION_HANDLER_HPP

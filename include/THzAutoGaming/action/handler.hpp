#ifndef THZ_AUTOGAMING_ACTION_HANDLER_HPP
#define THZ_AUTOGAMING_ACTION_HANDLER_HPP

#include "THzAutoGaming/action/iAction.hpp"
#include "THzImage/common/pixel.hpp"
#include "THzImage/processing/iNode.hpp"

namespace Terrahertz {

/// @brief Uses processed screenshots to determine what actions to perform.
class ActionHandler
{
public:
    /// @brief Initializes a new ActionHandler.
    ///
    /// @param input The node used for inputting images the handler works with.
    ActionHandler(INode<MiniHSVPixel> *const input) noexcept;

    /// @brief Adds a new action the the handler.
    ///
    /// @param action The action to add.
    void addAction(IAction *const action) noexcept;

    /// @brief Retrieves the next image from the input node and performs actions based on the image.
    ///
    /// @return True if operation was successful, false otherwise.
    bool next() noexcept;
};

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_ACTION_HANDLER_HPP

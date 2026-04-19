#ifndef THZ_AUTOGAMING_ACTION_ITRIGGERACTION_HPP
#define THZ_AUTOGAMING_ACTION_ITRIGGERACTION_HPP

#include "THzCommon/math/point.hpp"
#include "THzCommon/math/rectangle.hpp"
#include "THzImage/common/image.hpp"

namespace Terrahertz {

/// @brief Interface for trigger based actions.
class ITriggerAction
{
public:
    /// @brief Explicitly default the destrcutor to make it virtual.
    virtual ~ITriggerAction() noexcept {}

    /// @brief Signals if the action can be performed at the moment.
    ///
    /// @return True if the action can be performed, false otherwise.
    virtual bool enabled() const noexcept = 0;

    /// @brief Return the zone on the screen to look for the trigger image in.
    ///
    /// @return The zone on the screen to look for the trigger image in, default Rectangle for entire screen.
    virtual Rectangle searchZone() const noexcept = 0;

    /// @brief Flag signalling if the image can overlap with other trigger images.
    ///
    /// @return True if the image can overlap, false otherwise.
    virtual bool overlapping() const noexcept = 0;

    /// @brief The image the action handler shall look for in the screenshot to trigger the action.
    ///
    /// @return The image triggering the action.
    virtual MiniHSVImage const &trigger() const noexcept = 0;

    /// @brief Method called should the trigger image be found in the current screenshot.
    ///
    /// @param position The postition on the screen the upper left corner of the image was found.
    virtual void execute(Point const position) noexcept = 0;
};

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_ACTION_ITRIGGERACTION_HPP

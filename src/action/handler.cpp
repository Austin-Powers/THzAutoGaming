#include "THzAutoGaming/action/handler.hpp"

namespace Terrahertz::Action {

Handler::Handler(ImageProcessing::INode<BGRAPixel> *const imageInput) noexcept {}

void Handler::addTriggerAction(ITriggerAction *const action) noexcept {}

bool Handler::next() noexcept { return false; }

} // namespace Terrahertz::Action

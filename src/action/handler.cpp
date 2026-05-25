#include "THzAutoGaming/action/handler.hpp"

namespace Terrahertz::Action {

Handler::Handler(ImageProcessing::INode<BGRAPixel> *const imageInput) noexcept : _bgraImageInput{imageInput} {}

void Handler::addTriggerAction(ITriggerAction *const action) noexcept {}

bool Handler::next() noexcept { return _bgraImageInput->next(); }

} // namespace Terrahertz::Action

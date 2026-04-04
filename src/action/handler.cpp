#include "THzAutoGaming/action/handler.hpp"

namespace Terrahertz::Action {

Handler::Handler(ImageProcessing::INode<BGRAPixel> *const imageInput, Input::Emulator *const emulator) noexcept {}

bool Handler::next() noexcept {return false;}

} // namespace Terrahertz::Action

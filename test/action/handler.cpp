#include "THzAutoGaming/action/handler.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct ActionHandler : public testing::Test
{
    class MockNode : public ImageProcessing::INode<BGRAPixel>
    {
    public:
        [[nodiscard]] bool next(bool const countFailure) noexcept override { return nextReturnValue; }

        [[nodiscard]] ImageProcessing::ToCountResult toCount(size_t const target,
                                                             bool const   force = false) noexcept override
        {
            return ImageProcessing::ToCountResult::Failure;
        }

        [[nodiscard]] BGRAImage &operator[](size_t const index) noexcept override { return image; }

        [[nodiscard]] size_t slots() const noexcept override { return 1U; }

        [[nodiscard]] size_t count() const noexcept override { return 1U; }

        BGRAImage image{};

        bool nextReturnValue = true;
    };
};

TEST_F(ActionHandler, NextFailsIfNextOfNodeFails)
{
    MockNode node{};
    node.nextReturnValue = false;
    Action::Handler sut{&node};

    EXPECT_FALSE(sut.next());
}

TEST_F(ActionHandler, NextCallsNextOnNode)
{
    MockNode        node{};
    Action::Handler sut{&node};

    // EXPECT_TRUE(sut.next());
}

} // namespace Terrahertz::UnitTests

#include "THzAutoGaming/simulations/tileMatchingPuzzle.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Simulations_TileMatchingPuzzle : public testing::Test
{
    static constexpr std::uint8_t GridWidth{8U};
    static constexpr std::uint8_t GridHeight{8U};
    static constexpr std::uint8_t TypeCount{5U};

    Simulations::TileMatchingPuzzle sut{GridWidth, GridHeight, TypeCount};

    std::array<std::array<std::uint8_t, GridHeight>, GridWidth> stateReplica{};

    void replicate() noexcept
    {
        for (auto y = 0U; y < GridHeight; ++y)
        {
            for (auto x = 0U; x < GridWidth; ++x)
            {
                stateReplica[x][y] = sut(x, y);
            }
        }
    }

    void fillGrid() noexcept
    {
        auto lineStart = 1U;
        for (auto y = 0U; y < GridHeight; ++y)
        {
            auto nextCell = lineStart;
            for (auto x = 0U; x < GridWidth; ++x)
            {
                ASSERT_TRUE(sut.setTile(x, y, nextCell));
                ++nextCell;
            }
        }
    }
};

TEST_F(Simulations_TileMatchingPuzzle, ValidStateAfterConstruction)
{
    EXPECT_EQ(sut.width(), GridWidth);
    EXPECT_EQ(sut.height(), GridHeight);
    EXPECT_EQ(sut.typeCount(), TypeCount);

    std::array<bool, TypeCount> found{};
    for (auto y = 0U; y < GridHeight; ++y)
    {
        for (auto x = 0U; x < GridWidth; ++x)
        {
            auto const value = sut(x, y);
            ASSERT_NE(Simulations::TileMatchingPuzzle::EmptyTile, value);
            ASSERT_NE(Simulations::TileMatchingPuzzle::ErrorTile, value);
            found[value - 1U] = true;
        }
    }
    auto foundTypes = 0U;
    for (auto const f : found)
    {
        if (f)
        {
            ++foundTypes;
        }
    }
    EXPECT_GE(foundTypes, TypeCount - 1U);

    // Check if grid is stable
    replicate();
    sut.simulate();
    for (auto y = 0U; y < GridHeight; ++y)
    {
        for (auto x = 0U; x < GridWidth; ++x)
        {
            ASSERT_EQ(stateReplica[x][y], sut(x, y));
        }
    }
}

TEST_F(Simulations_TileMatchingPuzzle, TileCoordinatesOutOfBoundReturnsErrorTile)
{
    EXPECT_EQ(sut(GridWidth, 0U), Simulations::TileMatchingPuzzle::ErrorTile);
    EXPECT_EQ(sut(0U, GridHeight), Simulations::TileMatchingPuzzle::ErrorTile);
    EXPECT_EQ(sut(GridWidth, GridHeight), Simulations::TileMatchingPuzzle::ErrorTile);
}

TEST_F(Simulations_TileMatchingPuzzle, SetTileOutOfBoundReturnsFalse)
{
    EXPECT_FALSE(sut.setTile(GridWidth, 0U, 2U));
    EXPECT_FALSE(sut.setTile(0U, GridHeight, 2U));
    EXPECT_FALSE(sut.setTile(GridWidth, GridHeight, 2U));
}

TEST_F(Simulations_TileMatchingPuzzle, SetTileToIllegalValueReturnsFalseAndDoesNoChange)
{
    std::uint8_t const x{2U};
    std::uint8_t const y{2U};

    auto const originalCellValue = sut(x, y);
    EXPECT_FALSE(sut.setTile(2U, 2U, Simulations::TileMatchingPuzzle::ErrorTile));
    EXPECT_EQ(originalCellValue, sut(x, y));
    EXPECT_FALSE(sut.setTile(2U, 2U, TypeCount + 1U));
    EXPECT_EQ(originalCellValue, sut(x, y));
}

TEST_F(Simulations_TileMatchingPuzzle, SettingValueUpdatesGrid)
{
    std::uint8_t const x{3U};
    std::uint8_t const y{3U};
    EXPECT_TRUE(sut.setTile(x, y, 2U));
    EXPECT_EQ(sut(x, y), 2U);
}

TEST_F(Simulations_TileMatchingPuzzle, GravityFillsCellsWithTheContentFromAbove) {}

} // namespace Terrahertz::UnitTests

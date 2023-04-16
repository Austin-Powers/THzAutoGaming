#include "THzAutoGaming/simulations/tileMatchingPuzzle.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <utility>

namespace Terrahertz::UnitTests {

struct Simulations_TileMatchingPuzzle : public testing::Test
{
    static constexpr std::uint8_t GridWidth{8U};
    static constexpr std::uint8_t GridHeight{8U};
    static constexpr std::uint8_t TypeCount{5U};

    Simulations::TileMatchingPuzzle sut{GridWidth, GridHeight, TypeCount};

    std::array<std::array<std::uint8_t, GridHeight>, GridWidth> stateReplica{};

    std::uint8_t const EmptyTile = Simulations::TileMatchingPuzzle::EmptyTile;

    /// @brief Fills grid with a pattern to prevent randomness interfering with the tests
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
                if (nextCell > TypeCount)
                {
                    nextCell = 1U;
                }
            }
            ++lineStart;
            if (lineStart > TypeCount)
            {
                lineStart = 1U;
            }
        }
    }

    /// @brief Replicates the state of sut into the stateReplica
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

    /// @brief Simulates gravity on the state replica
    void simulateGravity()
    {
        for (auto i = 0U; i < GridHeight; ++i)
        {
            for (auto y = 1U; y < GridHeight; ++y)
            {
                for (auto x = 0U; x < GridWidth; ++x)
                {
                    if (stateReplica[x][y] == EmptyTile)
                    {
                        std::swap(stateReplica[x][y], stateReplica[x][y - 1]);
                    }
                }
            }
        }
    }

    /// @brief Compares the state of sut to the stateReplica.
    ///
    /// @param refill If true comparison expects sut to refill empty cells,
    /// this means if a cell in the replica is empty it is expected that the cell
    void compareReplica(bool const refill)
    {
        for (auto y = 0U; y < GridHeight; ++y)
        {
            for (auto x = 0U; x < GridWidth; ++x)
            {
                if (refill && (stateReplica[x][y] == EmptyTile))
                {
                    ASSERT_NE(sut(x, y), EmptyTile) << "x: " << x << " y: " << y;
                    ASSERT_NE(sut(x, y), Simulations::TileMatchingPuzzle::ErrorTile) << "x: " << x << " y: " << y;
                }
                else
                {
                    ASSERT_EQ(sut(x, y), stateReplica[x][y]) << "x: " << x << " y: " << y;
                }
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
    compareReplica(false);
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

TEST_F(Simulations_TileMatchingPuzzle, GravityFillsCellsWithTheContentFromAbove)
{
    fillGrid();

    // one tile
    EXPECT_TRUE(sut.setTile(0U, 0U, EmptyTile));

    // horizontal line
    EXPECT_TRUE(sut.setTile(3U, 3U, EmptyTile));
    EXPECT_TRUE(sut.setTile(4U, 3U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 3U, EmptyTile));
    EXPECT_TRUE(sut.setTile(6U, 3U, EmptyTile));

    // vertical line
    EXPECT_TRUE(sut.setTile(5U, 4U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 5U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 6U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 7U, EmptyTile));

    replicate();

    auto const result = sut.simulate(false);
    ASSERT_TRUE(result.empty());

    simulateGravity();
    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, GridGetsRefilledAfterApplyingGravity)
{
    fillGrid();

    // one tile
    EXPECT_TRUE(sut.setTile(0U, 0U, EmptyTile));

    // horizontal line
    EXPECT_TRUE(sut.setTile(3U, 3U, EmptyTile));
    EXPECT_TRUE(sut.setTile(4U, 3U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 3U, EmptyTile));
    EXPECT_TRUE(sut.setTile(6U, 3U, EmptyTile));

    // vertical line
    EXPECT_TRUE(sut.setTile(5U, 4U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 5U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 6U, EmptyTile));
    EXPECT_TRUE(sut.setTile(5U, 7U, EmptyTile));

    replicate();

    // we cannot check the result as the refill might lead to collapses
    sut.simulate();

    simulateGravity();
    compareReplica(true);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfHorizontalLine3)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 0 0 0 1 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 3 4 5 2 3 4
    // 2| 3 4 5 1 2 3 4 5    2| 3 4 4 5 1 3 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 5 1 2 4 5 1
    // 4| 5 1 2 3 4 5 1 2    4| 5 1 1 2 3 5 1 2
    // 5| 1 2 3 3 3 1 2 3    5| 1 2 2 3 4 1 2 3
    // 6| 2 3 4 5 1 2 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(3U, 5U, 3U));
    EXPECT_TRUE(sut.setTile(4U, 5U, 3U));

    replicate();
    // simulate collapse
    stateReplica[2U][5U] = EmptyTile;
    stateReplica[3U][5U] = EmptyTile;
    stateReplica[4U][5U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 3U);
    EXPECT_EQ(result[0U].amount, 3U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 0 0 0 1 2 3    0| 1 2 0 0 0 0 0 3
    // 1| 2 3 3 4 5 2 3 4    1| 2 3 3 4 0 1 2 4
    // 2| 3 4 4 5 1 3 4 5    2| 3 4 4 5 5 2 3 5
    // 3| 4 5 5 1 2 4 5 1 -> 3| 4 5 5 1 1 3 4 1
    // 4| 5 1 1 2 3 5 1 2    4| 5 1 1 2 2 4 5 2
    // 5| 1 2 2 3 4 4 4 3    5| 1 2 2 3 3 5 1 3
    // 6| 2 3 4 5 1 2 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 5U, 4U));
    EXPECT_TRUE(sut.setTile(6U, 5U, 4U));

    replicate();
    // simulate collapse
    stateReplica[4U][5U] = EmptyTile;
    stateReplica[5U][5U] = EmptyTile;
    stateReplica[6U][5U] = EmptyTile;
    simulateGravity();

    result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 3U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfHorizontalLine4)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 0 0 0 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 3 4 5 1 3 4
    // 2| 3 4 5 1 2 3 4 5    2| 3 4 4 5 1 2 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 5 1 2 3 5 1
    // 4| 5 1 2 3 4 5 1 2    4| 5 1 1 2 3 4 1 2
    // 5| 1 2 3 3 3 3 2 3    5| 1 2 2 3 4 5 2 3
    // 6| 2 3 4 5 1 2 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(3U, 5U, 3U));
    EXPECT_TRUE(sut.setTile(4U, 5U, 3U));
    EXPECT_TRUE(sut.setTile(5U, 5U, 3U));

    replicate();
    // simulate collapse
    stateReplica[2U][5U] = EmptyTile;
    stateReplica[3U][5U] = EmptyTile;
    stateReplica[4U][5U] = EmptyTile;
    stateReplica[5U][5U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 3U);
    EXPECT_EQ(result[0U].amount, 4U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfHorizontalLine5)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 0 0 0 0 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 2 3 4 5 1 3 4
    // 2| 3 4 5 1 2 3 4 5    2| 3 3 4 5 1 2 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 4 5 1 2 3 5 1
    // 4| 5 1 2 3 4 5 1 2    4| 5 5 1 2 3 4 1 2
    // 5| 1 3 3 3 3 3 2 3    5| 1 1 2 3 4 5 2 3
    // 6| 2 3 4 5 1 2 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(1U, 5U, 3U));
    EXPECT_TRUE(sut.setTile(3U, 5U, 3U));
    EXPECT_TRUE(sut.setTile(4U, 5U, 3U));
    EXPECT_TRUE(sut.setTile(5U, 5U, 3U));

    replicate();
    // simulate collapse
    stateReplica[1U][5U] = EmptyTile;
    stateReplica[2U][5U] = EmptyTile;
    stateReplica[3U][5U] = EmptyTile;
    stateReplica[4U][5U] = EmptyTile;
    stateReplica[5U][5U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 3U);
    EXPECT_EQ(result[0U].amount, 5U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfVerticalLine3)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 3 4 5 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 4 5 1 0 3 4
    // 2| 3 4 5 1 2 3 4 5    2| 3 4 5 1 2 0 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 1 2 3 1 5 1
    // 4| 5 1 2 3 4 4 1 2    4| 5 1 2 3 4 2 1 2
    // 5| 1 2 3 4 5 4 2 3    5| 1 2 3 4 5 3 2 3
    // 6| 2 3 4 5 1 2 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 5U, 4U));

    replicate();
    // simulate collapse
    stateReplica[5U][3U] = EmptyTile;
    stateReplica[5U][4U] = EmptyTile;
    stateReplica[5U][5U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 3U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfVerticalLine4)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 3 4 5 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 4 5 1 0 3 4
    // 2| 3 4 5 1 2 4 4 5    2| 3 4 5 1 2 0 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 1 2 3 0 5 1
    // 4| 5 1 2 3 4 4 1 2    4| 5 1 2 3 4 1 1 2
    // 5| 1 2 3 4 5 4 2 3    5| 1 2 3 4 5 2 2 3
    // 6| 2 3 4 5 1 2 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 2U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 5U, 4U));

    replicate();
    // simulate collapse
    stateReplica[5U][2U] = EmptyTile;
    stateReplica[5U][3U] = EmptyTile;
    stateReplica[5U][4U] = EmptyTile;
    stateReplica[5U][5U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 4U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfVerticalLine5)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 3 4 5 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 4 5 1 0 3 4
    // 2| 3 4 5 1 2 4 4 5    2| 3 4 5 1 2 0 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 1 2 3 0 5 1
    // 4| 5 1 2 3 4 4 1 2    4| 5 1 2 3 4 0 1 2
    // 5| 1 2 3 4 5 4 2 3    5| 1 2 3 4 5 1 2 3
    // 6| 2 3 4 5 1 4 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 2U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 5U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 6U, 4U));

    replicate();
    // simulate collapse
    stateReplica[5U][2U] = EmptyTile;
    stateReplica[5U][3U] = EmptyTile;
    stateReplica[5U][4U] = EmptyTile;
    stateReplica[5U][5U] = EmptyTile;
    stateReplica[5U][6U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 5U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfAngle)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 3 0 0 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 4 4 5 0 3 4
    // 2| 3 4 5 1 2 4 4 5    2| 3 4 5 5 1 0 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 1 1 2 1 5 1
    // 4| 5 1 2 4 4 4 1 2    4| 5 1 2 2 3 2 1 2
    // 5| 1 2 3 4 5 1 2 3    5| 1 2 3 4 5 1 2 3
    // 6| 2 3 4 5 1 4 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 2U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(4U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(3U, 4U, 4U));

    replicate();
    // simulate collapse
    stateReplica[5U][2U] = EmptyTile;
    stateReplica[5U][3U] = EmptyTile;
    stateReplica[5U][4U] = EmptyTile;
    stateReplica[4U][4U] = EmptyTile;
    stateReplica[3U][4U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 5U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfTPiece)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 3 0 0 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 4 4 5 0 3 4
    // 2| 3 4 5 1 2 4 4 5    2| 3 4 5 5 1 0 4 5
    // 3| 4 5 1 4 4 4 5 1 -> 3| 4 5 1 1 2 1 5 1
    // 4| 5 1 2 3 4 4 1 2    4| 5 1 2 2 3 2 1 2
    // 5| 1 2 3 4 5 1 2 3    5| 1 2 3 4 5 1 2 3
    // 6| 2 3 4 5 1 4 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 2U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(4U, 3U, 4U));
    EXPECT_TRUE(sut.setTile(3U, 3U, 4U));

    replicate();
    // simulate collapse
    stateReplica[5U][2U] = EmptyTile;
    stateReplica[5U][3U] = EmptyTile;
    stateReplica[5U][4U] = EmptyTile;
    stateReplica[4U][3U] = EmptyTile;
    stateReplica[3U][3U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 5U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfPlusPiece)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 3 4 0 0 0 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 4 5 5 0 2 4
    // 2| 3 4 5 1 2 4 4 5    2| 3 4 5 1 1 0 3 5
    // 3| 4 5 1 2 4 4 4 1 -> 3| 4 5 1 2 2 1 4 1
    // 4| 5 1 2 3 4 4 1 2    4| 5 1 2 3 3 2 1 2
    // 5| 1 2 3 4 5 1 2 3    5| 1 2 3 4 5 1 2 3
    // 6| 2 3 4 5 1 4 3 4    6| 2 3 4 5 1 2 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(5U, 2U, 4U));
    EXPECT_TRUE(sut.setTile(5U, 4U, 4U));
    EXPECT_TRUE(sut.setTile(4U, 3U, 4U));
    EXPECT_TRUE(sut.setTile(6U, 3U, 4U));

    replicate();
    // simulate collapse
    stateReplica[5U][2U] = EmptyTile;
    stateReplica[5U][3U] = EmptyTile;
    stateReplica[5U][4U] = EmptyTile;
    stateReplica[4U][3U] = EmptyTile;
    stateReplica[6U][3U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 4U);
    EXPECT_EQ(result[0U].amount, 5U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfComplexGroup)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 5 3 5 5 1 2 3    0| 1 0 0 0 5 1 2 3
    // 1| 2 5 4 5 1 2 3 4    1| 2 0 3 0 1 2 3 4
    // 2| 3 5 5 5 2 3 4 5    2| 3 0 4 0 2 3 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 0 1 2 3 4 5 1
    // 4| 5 1 2 3 4 5 1 2    4| 5 1 2 3 4 5 1 2
    // 5| 1 2 3 4 5 1 2 3    5| 1 2 3 4 5 1 2 3
    // 6| 2 3 4 5 1 4 3 4    6| 2 3 4 5 1 4 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(1U, 0U, 5U));
    EXPECT_TRUE(sut.setTile(1U, 1U, 5U));
    EXPECT_TRUE(sut.setTile(1U, 2U, 5U));
    EXPECT_TRUE(sut.setTile(2U, 2U, 5U));
    EXPECT_TRUE(sut.setTile(3U, 2U, 5U));
    EXPECT_TRUE(sut.setTile(3U, 1U, 5U));
    EXPECT_TRUE(sut.setTile(3U, 0U, 5U));

    replicate();
    // simulate collapse
    stateReplica[1U][0U] = EmptyTile;
    stateReplica[1U][1U] = EmptyTile;
    stateReplica[1U][2U] = EmptyTile;
    stateReplica[1U][3U] = EmptyTile;
    stateReplica[2U][2U] = EmptyTile;
    stateReplica[3U][2U] = EmptyTile;
    stateReplica[3U][1U] = EmptyTile;
    stateReplica[3U][0U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 1U);
    EXPECT_EQ(result[0U].type, 5U);
    EXPECT_EQ(result[0U].amount, 8U);
    EXPECT_EQ(result[0U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseOfTwoSeparateLines)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 0 0 0 0 2 3
    // 1| 2 3 4 5 1 2 3 4    1| 2 3 0 4 5 1 3 4
    // 2| 3 4 5 5 5 5 4 5    2| 3 4 0 5 1 2 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 0 2 3 4 5 1
    // 4| 5 1 2 3 4 5 1 2    4| 5 1 3 3 4 5 1 2
    // 5| 1 2 3 4 5 1 2 3    5| 1 2 4 4 5 1 2 3
    // 6| 2 3 3 5 1 4 3 4    6| 2 3 1 5 1 4 3 4
    // 7| 3 4 3 1 2 3 4 5    7| 3 4 2 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(3U, 2U, 5U));
    EXPECT_TRUE(sut.setTile(4U, 2U, 5U));
    EXPECT_TRUE(sut.setTile(5U, 2U, 5U));
    EXPECT_TRUE(sut.setTile(2U, 6U, 3U));
    EXPECT_TRUE(sut.setTile(2U, 7U, 3U));

    replicate();
    // simulate collapse
    stateReplica[2U][2U] = EmptyTile;
    stateReplica[3U][2U] = EmptyTile;
    stateReplica[4U][2U] = EmptyTile;
    stateReplica[5U][2U] = EmptyTile;
    stateReplica[2U][5U] = EmptyTile;
    stateReplica[2U][6U] = EmptyTile;
    stateReplica[2U][7U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 2U);
    EXPECT_EQ(result[0U].type, 5U);
    EXPECT_EQ(result[0U].amount, 4U);
    EXPECT_EQ(result[0U].wave, 0U);
    EXPECT_EQ(result[1U].type, 3U);
    EXPECT_EQ(result[1U].amount, 3U);
    EXPECT_EQ(result[1U].wave, 0U);

    compareReplica(false);
}

TEST_F(Simulations_TileMatchingPuzzle, CollapseCascadesCorrectly)
{
    fillGrid();

    //    0 1 2 3 4 5 6 7       0 1 2 3 4 5 6 7
    //  _________________     _________________
    // 0| 1 2 3 4 5 1 2 3    0| 1 2 0 0 0 0 0 3
    // 1| 2 3 5 5 1 1 1 4    1| 2 3 3 4 0 1 2 4
    // 2| 3 4 5 1 2 3 4 5    2| 3 4 5 1 2 3 4 5
    // 3| 4 5 1 2 3 4 5 1 -> 3| 4 5 1 2 3 4 5 1
    // 4| 5 1 2 3 4 5 1 2    4| 5 1 2 3 4 5 1 2
    // 5| 1 2 3 4 5 1 2 3    5| 1 2 3 4 5 1 2 3
    // 6| 2 3 4 5 1 4 3 4    6| 2 3 4 5 1 4 3 4
    // 7| 3 4 5 1 2 3 4 5    7| 3 4 5 1 2 3 4 5

    EXPECT_TRUE(sut.setTile(2U, 1U, 5U));
    EXPECT_TRUE(sut.setTile(3U, 1U, 5U));
    EXPECT_TRUE(sut.setTile(5U, 1U, 1U));
    EXPECT_TRUE(sut.setTile(6U, 1U, 1U));

    replicate();
    // simulate collapse
    stateReplica[2U][1U] = EmptyTile;
    stateReplica[3U][1U] = EmptyTile;
    stateReplica[4U][1U] = EmptyTile;
    stateReplica[4U][0U] = EmptyTile;
    stateReplica[5U][1U] = EmptyTile;
    stateReplica[6U][1U] = EmptyTile;
    simulateGravity();

    auto result = sut.simulate(false);
    ASSERT_EQ(result.size(), 2U);
    EXPECT_EQ(result[0U].type, 1U);
    EXPECT_EQ(result[0U].amount, 3U);
    EXPECT_EQ(result[0U].wave, 0U);
    EXPECT_EQ(result[1U].type, 5U);
    EXPECT_EQ(result[1U].amount, 3U);
    EXPECT_EQ(result[1U].wave, 1U);

    compareReplica(false);
}

} // namespace Terrahertz::UnitTests

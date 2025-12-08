#ifndef THZ_AUTOGAMING_SIMULATIONS_TILEMATCHINGPUZZLE_HPP
#define THZ_AUTOGAMING_SIMULATIONS_TILEMATCHINGPUZZLE_HPP

#include <cstddef>
#include <cstdint>
#include <gsl/gsl>
#include <random>
#include <vector>

namespace Terrahertz::Simulations {

/// @brief Simulation of a tile matching puzzle.
///
/// @remarks This class only does simulation, the specifics of the puzzle have to be done by the user.
class TileMatchingPuzzle
{
public:
    /// @brief The value of an empty tile.
    static constexpr std::uint8_t EmptyTile{0x00U};

    /// @brief The value of an error tile.
    static constexpr std::uint8_t ErrorTile{0xFFU};

    /// @brief Structure containing basic information about a collapse of a line of tiles.
    struct Collapse
    {
        /// @brief The number of the type of tile.
        std::uint8_t type{};

        /// @brief The amount of tiles that collapsed.
        std::uint16_t amount{};

        /// @brief The wave of the simulation in which the collapse took place.
        std::uint16_t wave{};
    };

    /// @brief Initializes a new simulation instance.
    ///
    /// @param pWidth The width of the grid.
    /// @param pHeight The height of the grid.
    /// @param pTypeCount The number of different tile types.
    TileMatchingPuzzle(std::uint8_t const pWidth, std::uint8_t const pHeight, std::uint8_t const pTypeCount) noexcept;

    /// @brief Returns the width of the grid.
    ///
    /// @return The width of the grid.
    std::uint8_t width() const noexcept;

    /// @brief Returns the height of the grid.
    ///
    /// @return The height of the grid.
    std::uint8_t height() const noexcept;

    /// @brief Returns the number of different tile types.
    ///
    /// @return The number of different tile types.
    std::uint8_t typeCount() const noexcept;

    /// @brief Provides access to the tile at the given coordinates.
    ///
    /// @param x The row in the grid [left-right].
    /// @param y The column in the grid [top-bottom].
    /// @return The content of the tile, 0xFF if the tile is out of range.
    std::uint8_t operator()(std::uint8_t const x, std::uint8_t const y) const noexcept;

    /// @brief Sets a new value for the tile at the given coordinates.
    ///
    /// @param x The row in the grid [left-right].
    /// @param y The column in the grid [top-bottom].
    /// @param newContent The new value of the tile.
    /// @return True if the tile was set, false otherwise.
    bool setTile(std::uint8_t const x, std::uint8_t const y, std::uint8_t const newContent) noexcept;

    /// @brief Simulates the next step in the game.
    ///
    /// @param refill True if the grid shall be refilled after gravity has been applied.
    /// @return The collapses that happened during simulation.
    /// @remarks Repeatedly collapses matching tiles and applies gravity until nothing is changed during collapse.
    gsl::span<Collapse> simulate(bool const refill = true) noexcept;

private:
    /// @brief Structure containing all information about a tile.
    struct Tile
    {
        /// @brief The type of the tile.
        std::uint8_t type{};

        /// @brief The id of horizontal group the tile belongs to.
        std::uint16_t horizontalGroupId{};

        /// @brief The id of verical group the tile belongs to.
        std::uint16_t verticalGroupId{};
    };

    /// @brief Collapses all lines of similar cells that are 3 tiles or longer.
    ///
    /// @param wave The current wave of the simulation.
    void collapse(std::uint16_t const wave) noexcept;

    /// @brief Fills up empty cells by moving the content of other cells above into them.
    ///
    /// @return True if there were any changes to the grid, false otherwise.
    bool gravity() noexcept;

    /// @brief Fills empty cells with random new values.
    ///
    /// @return True if there were any changes to the grid, false otherwise.
    bool fill() noexcept;

    /// @brief The width of the grid.
    std::uint8_t _width{};

    /// @brief The height of the grid.
    std::uint8_t _height{};

    /// @brief The number of different tile types.
    std::uint8_t _typeCount{};

    /// @brief The grid of the puzzle.
    std::vector<Tile> _grid{};

    /// @brief Vector mapping group Ids to _collapses indeces.
    std::vector<std::uint16_t> _groups{};

    /// @brief Vector of all the collapses happening during the current simulation step.
    std::vector<Collapse> _collapses{};

    /// @brief The random number generator used for filling the grid.
    std::default_random_engine _rng{};

    /// @brief The distribution of the random numbers used for filling the grid.
    std::uniform_int_distribution<std::uint16_t> _rngDist{};
};

} // namespace Terrahertz::Simulations

#endif // !THZ_AUTOGAMING_SIMULATIONS_TILEMATCHINGPUZZLE_HPP

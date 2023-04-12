#include "THzAutoGaming/simulations/tileMatchingPuzzle.hpp"

namespace Terrahertz::Simulations {

constexpr std::uint8_t TileMatchingPuzzle::EmptyTile;
constexpr std::uint8_t TileMatchingPuzzle::ErrorTile;

TileMatchingPuzzle::TileMatchingPuzzle(std::uint8_t const width,
                                       std::uint8_t const height,
                                       std::uint8_t const typeCount) noexcept
    : _width{width}, _height{height}, _typeCount{typeCount}, _rngDist{1U, typeCount}
{
    _grid.resize(static_cast<size_t>(_width) * _height);
    simulate();
}

std::uint8_t TileMatchingPuzzle::width() const noexcept { return _width; }

std::uint8_t TileMatchingPuzzle::height() const noexcept { return _height; }

std::uint8_t TileMatchingPuzzle::typeCount() const noexcept { return _typeCount; }

std::uint8_t TileMatchingPuzzle::operator()(std::uint8_t const x, std::uint8_t const y) const noexcept
{
    if ((x >= _width) || (y >= _height))
    {
        return ErrorTile;
    }
    return _grid[(static_cast<size_t>(x) * _height) + y];
}

bool TileMatchingPuzzle::setTile(std::uint8_t const x, std::uint8_t const y, std::uint8_t const newContent) noexcept
{
    if ((x >= _width) || (y >= _height))
    {
        return false;
    }
    if (newContent > _typeCount)
    {
        return false;
    }
    _grid[(static_cast<size_t>(x) * _height) + y] = newContent;
    return true;
}

gsl::span<TileMatchingPuzzle::Collapse> TileMatchingPuzzle::simulate(bool const refill) noexcept
{
    while (true)
    {
        collapse();
        auto changes = gravity();
        // we cannot check skipped here, because if a cell in the top row is empty
        // gravity would return false but fill would still need to run
        if (refill)
        {
            // we do not need to remember the result of gravity
            // because if gravity changed something fill will change something as well
            changes = fill();
        }
        if (!changes)
        {
            break;
        }
    }
    return {};
}

void TileMatchingPuzzle::collapse() noexcept {}

bool TileMatchingPuzzle::gravity() noexcept
{
    auto changes = false;
    for (auto x = 0U; x < _width; ++x)
    {
        // pointer that needs to be reached by topPtr in order to finish the column
        auto const stopPtr = _grid.data() + (x * _height) - 1U;
        // pointer to the bottom cell receiving content if empty
        auto bottomPtr = stopPtr + _height;
        // pointer to the top cell containing content to fill the bottom cell
        auto topPtr = bottomPtr - 1U;

        while (topPtr != stopPtr)
        {
            if ((*bottomPtr) == EmptyTile)
            {
                if ((*topPtr) == EmptyTile)
                {
                    --topPtr;
                }
                else
                {
                    // we do not need to swap as we know bottom is an empty tile
                    (*bottomPtr) = (*topPtr);
                    (*topPtr)    = EmptyTile;
                    changes      = true;
                    --bottomPtr;
                    --topPtr;
                }
            }
            else
            {
                --bottomPtr;
                --topPtr;
            }
        }
    }
    return changes;
}

bool TileMatchingPuzzle::fill() noexcept
{
    auto changes = false;
    for (auto &cell : _grid)
    {
        if (cell == EmptyTile)
        {
            cell    = static_cast<std::uint8_t>(_rngDist(_rng));
            changes = true;
        }
    }
    return changes;
}

} // namespace Terrahertz::Simulations

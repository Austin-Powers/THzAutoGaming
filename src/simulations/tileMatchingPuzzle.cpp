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
    return _grid[(static_cast<size_t>(y) * _width) + x];
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
    _grid[(static_cast<size_t>(y) * _width) + x] = newContent;
    return true;
}

gsl::span<TileMatchingPuzzle::Collapse> TileMatchingPuzzle::simulate() noexcept
{
    while (collapse())
    {
        gravity();
        fill();
    }
    return {};
}

bool TileMatchingPuzzle::collapse() noexcept
{
    auto emptyTiles = false;
    for (auto &cell : _grid)
    {
        if (cell == EmptyTile)
        {
            emptyTiles = true;
            break;
        }
    }
    return emptyTiles;
}

void TileMatchingPuzzle::gravity() noexcept {}

void TileMatchingPuzzle::fill() noexcept
{
    for (auto &cell : _grid)
    {
        if (cell == EmptyTile)
        {
            cell = static_cast<std::uint8_t>(_rngDist(_rng));
        }
    }
}

} // namespace Terrahertz::Simulations

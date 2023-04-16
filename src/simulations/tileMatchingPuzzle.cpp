#include "THzAutoGaming/simulations/tileMatchingPuzzle.hpp"

#include "THzCommon/utility/spanhelpers.hpp"

#include <algorithm>
#include <array>

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
    return _grid[(static_cast<size_t>(x) * _height) + y].type;
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
    _grid[(static_cast<size_t>(x) * _height) + y].type = newContent;
    return true;
}

gsl::span<TileMatchingPuzzle::Collapse> TileMatchingPuzzle::simulate(bool const refill) noexcept
{
    for (auto &collapse : _collapses)
    {
        collapse.type   = EmptyTile;
        collapse.amount = 0U;
    }
    for (auto wave = 0U; wave < 128U; ++wave)
    {
        collapse(wave);
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

    auto newEnd =
        std::remove_if(_collapses.begin(), _collapses.end(), [](Collapse const &c) { return c.type == EmptyTile; });
    return toSpan<Collapse>(_collapses).subspan(0U, std::distance(_collapses.begin(), newEnd));
}

void TileMatchingPuzzle::collapse(std::uint16_t const wave) noexcept
{
    std::array<Tile *, 3U> triplet{};
    std::uint16_t          nextGroupId = 1U;
    Tile const *const      stopPtr     = _grid.data() + _grid.size();

    // look for groups vertically
    triplet[0U] = _grid.data();
    triplet[1U] = _grid.data() + 1U;
    triplet[2U] = _grid.data() + 2U;
    while (triplet[2U] != stopPtr)
    {
        for (auto y = 0U; y < (_height - 2U); ++y)
        {
            if ((triplet[0U]->type != EmptyTile) && (triplet[0U]->type == triplet[1U]->type) &&
                (triplet[1U]->type == triplet[2U]->type))
            {
                if (triplet[0U]->verticalGroupId != 0U)
                {
                    triplet[1U]->verticalGroupId = triplet[0U]->verticalGroupId;
                    triplet[2U]->verticalGroupId = triplet[0U]->verticalGroupId;
                }
                else
                {
                    triplet[0U]->verticalGroupId = nextGroupId;
                    triplet[1U]->verticalGroupId = nextGroupId;
                    triplet[2U]->verticalGroupId = nextGroupId;
                    ++nextGroupId;
                }
            }
            ++triplet[0U];
            ++triplet[1U];
            ++triplet[2U];
            if (triplet[2U] == stopPtr)
            {
                break;
            }
        }
        if (triplet[2U] == stopPtr)
        {
            break;
        }
        triplet[0U] += 2U;
        triplet[1U] += 2U;
        triplet[2U] += 2U;
    }

    // look for groups horizontally
    triplet[0U] = _grid.data();
    triplet[1U] = _grid.data() + _height;
    triplet[2U] = _grid.data() + _height + _height;
    while (triplet[2U] != stopPtr)
    {
        if ((triplet[0U]->type != EmptyTile) && (triplet[0U]->type == triplet[1U]->type) &&
            (triplet[1U]->type == triplet[2U]->type))
        {
            if (triplet[0U]->horizontalGroupId != 0U)
            {
                triplet[1U]->horizontalGroupId = triplet[0U]->horizontalGroupId;
                triplet[2U]->horizontalGroupId = triplet[0U]->horizontalGroupId;
            }
            else
            {
                triplet[0U]->horizontalGroupId = nextGroupId;
                triplet[1U]->horizontalGroupId = nextGroupId;
                triplet[2U]->horizontalGroupId = nextGroupId;
                ++nextGroupId;
            }
        }
        ++triplet[0U];
        ++triplet[1U];
        ++triplet[2U];
    }

    // final analysis
    for (auto &group : _groups)
    {
        group = 0U;
    }
    auto const accessGroup = [&](std::uint16_t const index) noexcept -> std::uint16_t & {
        while (_groups.size() <= index)
        {
            _groups.emplace_back();
        }
        return _groups[index];
    };
    auto const accessCollapse = [&](std::uint16_t &group) noexcept -> Collapse & {
        if (group == 0U)
        {
            for (; group < _collapses.size(); ++group)
            {
                if (_collapses[group].type == EmptyTile)
                {
                    break;
                }
            }
            if (group == _collapses.size())
            {
                _collapses.emplace_back();
            }
            _collapses[static_cast<size_t>(group)].wave = wave;
            ++group;
        }
        return _collapses[static_cast<size_t>(group) - 1U];
    };
    for (auto &tile : _grid)
    {
        if ((tile.horizontalGroupId != 0U) || (tile.verticalGroupId != 0U))
        {
            std::uint16_t id{};
            if (tile.horizontalGroupId == 0U)
            {
                id = tile.verticalGroupId;
            }
            else if (tile.verticalGroupId == 0U)
            {
                id = tile.horizontalGroupId;
            }
            else // if ((tile.horizontalGroupId != 0U) && (tile.verticalGroupId != 0U))
            {
                auto &groupH = accessGroup(tile.horizontalGroupId);
                auto &groupV = accessGroup(tile.verticalGroupId);
                if (groupH != groupV)
                {
                    auto &collapseH = accessCollapse(groupH);
                    auto &collapseV = accessCollapse(groupV);
                    collapseH.amount += collapseV.amount;
                    collapseV.type   = EmptyTile;
                    collapseV.amount = 0U;
                    groupV           = groupH;
                }
                id = tile.horizontalGroupId;
            }

            auto &collapse = accessCollapse(accessGroup(id));
            collapse.type  = tile.type;
            ++collapse.amount;

            // reset tile
            tile.type              = 0U;
            tile.horizontalGroupId = 0U;
            tile.verticalGroupId   = 0U;
        }
    }
}

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
            if (bottomPtr->type == EmptyTile)
            {
                if (topPtr->type == EmptyTile)
                {
                    --topPtr;
                }
                else
                {
                    // we do not need to swap as we know bottom is an empty tile
                    bottomPtr->type = topPtr->type;
                    topPtr->type    = EmptyTile;
                    changes         = true;
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
        if (cell.type == EmptyTile)
        {
            cell.type = static_cast<std::uint8_t>(_rngDist(_rng));
            changes   = true;
        }
    }
    return changes;
}

} // namespace Terrahertz::Simulations

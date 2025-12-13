#ifndef THZ_AUTOGAMING_UTILITY_ICONDITION_HPP
#define THZ_AUTOGAMING_UTILITY_ICONDITION_HPP

namespace Terrahertz {

/// @brief Interface for classes acting as conditions for events.
class ICondition
{
public:
    /// @brief Default destructor to make it virtual.
    virtual ~ICondition() noexcept {}

    /// @brief Checks if the condition is met.
    ///
    /// @return True if condition is met, false otherwise.
    virtual bool check() noexcept = 0;
};

} // namespace Terrahertz

#endif // !THZ_AUTOGAMING_UTILITY_ICONDITION_HPP

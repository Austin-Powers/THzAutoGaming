# THzAutoGaming
Fun projects all around automated playing of computer games.
## Details
Below is more detailed information about the contents of each subdirectory.

### Action
- __`class ActionHandler`__ _(handler.hpp)_ Uses processed screenshots to determine what actions to perform.
  
- __`class IAction`__ _(iAction.hpp)_ Interface for actions.
  

### Common
- __`definition Ms`__ _(timeDefinitions.hpp)_ Shorthand for milliseconds.
- __`definition Clock`__ _(timeDefinitions.hpp)_ The type of clock used throughout the library.
- __`definition TimePoint`__ _(timeDefinitions.hpp)_ The type of time_point used throughout the library.
  

### Input
- __`enum MouseButton`__ _(common.hpp)_ Enumeration of all common mouse buttons.
- __`enum KeyboardLock`__ _(common.hpp)_ Enumeration of common keyboard locks.
- __`enum Key`__ _(common.hpp)_ Enumeration of keys on the keyboard.
- __`concept SystemInterface`__ _(common.hpp)_ Concept for the system interface used to 
  
- __`class BaseEmulator`__ _(emulator.hpp)_ Emulates user input via keyboard and mouse.
- __`definition Emulator`__ _(emulator.hpp)_ The emulator using the current system interface.
- __`definition Emulator`__ _(emulator.hpp)_ The emulator using the current system interface.
  
- __`class IDeviationStrategy`__ _(iDeviationStrategy.hpp)_ Interface for calculating the deviation of the inputs done by an emulator.
  
- __`class LinuxInterface`__ _(linuxInterface.hpp)_ Implementation of the SystemInterface concept for windows.
  
- __`class NormalDeviationStrategy`__ _(normalDeviationStrategy.hpp)_ Implements the IDeviationStrategy using Parameters and std::normal_distribution for devation.
  
- __`class Parameters`__ _(parameters.hpp)_ The parameter for the input emulator.
  
- __`class WindowsInterface`__ _(windowsInterface.hpp)_ Implementation of the SystemInterface concept for windows.
  

### Optimisation
- __`concept Individual`__ _(algorithm.hpp)_ Concept for an individual of the population for the evolutionary algorithm.
- __`concept Evaluator`__ _(algorithm.hpp)_ Concept for a evaluator for individuals.
- __`struct Parameters`__ _(algorithm.hpp)_ Structure containing the parameters for the evolution run.
- __`class Algorithm`__ _(algorithm.hpp)_ Tempalte class encapsulating the evolutionary algorithm.
  

### Simulations
- __`class TileMatchingPuzzle`__ _(tileMatchingPuzzle.hpp)_ Simulation of a tile matching puzzle.
  

### Utility
- __`class CapsLockActive`__ _(commonConditions.hpp)_ Condition checking if Caps-Lock is active.
- __`class NumLockActive`__ _(commonConditions.hpp)_ Condition checking if Num-Lock is active.
  
- __`class ICondition`__ _(iCondition.hpp)_ Interface for classes acting as conditions for events.
  
- __`class ImageLoader`__ _(imageLoader.hpp)_ Centralizes the loading of images for actions.
  
- __`class LoopControl`__ _(loopControl.hpp)_ Ensures main loop is executed in set intervals and shutdown conditions are acted upon.
  


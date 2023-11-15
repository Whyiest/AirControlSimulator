# AirControlSimulator

## Description
AirControlSimulator is an air traffic control simulator developed in C++ that visualizes and manages airplanes on a virtual map. The project employs Dijkstra's algorithm for calculating the shortest routes between two points and allows for the scheduling of stopovers for the aircraft. Planes are generated randomly, and the simulator incorporates weather events and potential malfunctions to add a level of realism.

## Features
- **Random Airplane Generation**: Airplanes are automatically generated with various characteristics.
- **Route Calculation**: Uses Dijkstra's algorithm to determine the shortest path between two points.
- **Stopover Scheduling**: Ability to plan stopovers for the airplanes.
- **Weather Events and Malfunctions**: Simulation of unexpected events affecting the flight of the airplanes.
- **Graphical Display**: Uses the SFML graphics library for an interactive visual representation of the airplanes and their routes.

## Technology
- **Language**: C++
- **Graphics Library**: SFML
- **Compilation Tools**: CMake (CMakeLists.txt file included for ease of compilation)

## Installation
To compile and run AirControlSimulator, follow these steps:
1. Clone the repository:
   ```
   git clone https://github.com/Whyiest/AirControlSimulator.git
   ```
2. Ensure SFML and CMake are installed on your system.
3. Use CMake to configure the project and generate the necessary build files.
4. Compile and execute the project.

## Usage
After launching, the simulator will display airplanes moving on the virtual map. Users can observe the automated system's operation or interact to schedule stopovers and view calculated routes.

## Contributing
Contributions are welcome! If you have suggestions for improvements or corrections, feel free to create an issue or a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

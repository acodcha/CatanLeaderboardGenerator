# Catan Leaderboard Generator
![build and test](https://github.com/acodcha/CatanLeaderboardGenerator/workflows/build%20and%20test/badge.svg?branch=main)

Generates a simple leaderboard for Catan games. The leaderboard consists of Markdown files containing tables and plots.

See https://github.com/acodcha/CatanLeaderboard for an example of a leaderboard that uses this program.

## Dependencies
The following packages are required:
- **C++17 Compiler:** Any C++17 compiler will do, such as GCC or Clang. On Ubuntu, install GCC with `sudo apt install g++` or Clang with `sudo apt install clang`.
- **CMake:** On Ubuntu, install with `sudo apt install cmake`.
- **Gnuplot:** On Ubuntu, install with `sudo apt install gnuplot`.

## Configuration and Build
Configure and build the program with:

```
mkdir build
cd build
cmake ..
make
```

## Installation
Once you have configured and built the program, install it from the `build` directory with:

```
sudo make install
```

This installs the program to `/usr/local/bin/catan_leaderboard_generator`. To uninstall the program, simply delete it.

## Documentation
Building the documentation is optional and requires additional packages:
- **Doxygen:** On Ubuntu, install with `sudo apt install doxygen`.
- **Graphviz:** On Ubuntu, install with `sudo apt install graphviz`.
- **TeX Live:** On Ubuntu, install with `sudo apt install texlive texlive-fonts-extra`.

Documentation is disabled by default but can be generated from the `build` directory with:

```
cmake .. -DBUILD_DOCS=ON
make docs
```

This generates HTML documentation using Doxygen. The documentation is located in `docs/html`. Open the `docs/html/index.html` file in any web browser to view the documentation.

## Testing
Once you have configured and built the program, run tests from the `build` directory with:

```
make test
```

## Usage
Run with no arguments or with the `--help` argument to obtain usage information.

Otherwise, for regular use, run with:

```
catan_leaderboard_generator --games <path> --leaderboard <path>
```

- `--games <path>` specifies the path to the games file to be read. Required.
- `--leaderboard <path>` specifies the path to the directory in which the leaderboard will be written. Optional. If omitted, no leaderboard is written.

### Games File
The games file must have the following format:

```
2020-03-15 : Alice 10 , Bob 8 , Claire 7 , David 5
2020-03-15 : Alice 10 , Bob 9 , Claire 8
2020-03-17 : Claire 10 , Bob 9 , Alice 9 , Francis 8 , Edith 8
2020-03-18 : David 10 , Claire 9 , Alice 8 , Bob 8 , Edith 7 , Francis 6
2020-03-20 : Claire 10 * , Alice 10 , Bob 7 , David 7 , Edith 5
2020-03-20 : Bob 10 , Alice 7 , David 7 , Claire 6
```

- Each line represents a game.
- Each game consists of a date and a list of player names and points.
- The date must be in the YYYY-MM-DD format.
- A colon (`:`) is used to separate the date from the list of player names and points.
- Each player name is case-sensitive and must be followed by that player's points at game end.
- A comma (`,`) is used to separate players.
- Except for line endings, all whitespace is ignored. However, as in the above example, the use of whitespace is recommended for readability.
- The ordering of games is unimportant. The games will be sorted by date during processing.
- The ordering of players in a game is unimportant. The winning player is the player with the most points. Players can be tied for 2nd place, 3rd place, and so on depending on their points.
- In 5+ player games, because of the special build phase and because a player can only win during their turn, it is possible for the winning player to be tied for most points or to not have the most points. In such cases, identify the winning player by placing an asterisk (`*`) after their points, as in the above example.

## License
This work is licensed under the MIT License. For more details, see the [LICENSE](LICENSE) file or <https://mit-license.org/>.

## Maintainer
- Alexandre Coderre-Chabot (<https://github.com/acodcha>)

#ifndef RANDOMUTILITIES_H
#define RANDOMUTILITIES_H
#include <fstream>
#include <iostream>
#include <random>
#include <QColor>
/**
 * @file RandomUtilities.h
 * @brief Defines a utility class for random number generation.
 */

/**
 * @class RandomGenerator
 * @brief A helper class that wraps the C++ <random> library.
 *
 * Provides simple methods to get random numbers, colors,
 * and choices from vectors, using a single std::mt19937 engine.
 */

class RandomGenerator {
private:
    std::mt19937 gen; ///< The core Mersenne Twister engine.
public:
    /**
     * @brief Constructor.
     * Initializes the random engine with a seed from std::random_device.
     */
    RandomGenerator() : gen(std::random_device{}()) {}

    RandomGenerator(unsigned int seed) : gen(seed) {}

    /**
             * @brief Gets a random integer within a specified range (inclusive).
             * @param min The minimum possible value.
             * @param max The maximum possible value.
             * @return A random integer.
             */
    int getInt(int min, int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
    /**
             * @brief Gets a random double within a specified range.
             * @param min The minimum possible value.
             * @param max The maximum possible value.
             * @return A random double.
             */
    double getDouble(double min, double max) {
        std::uniform_real_distribution<> dist(min, max);
        return dist(gen);
    }
    /**
     * @brief Gets a random boolean value.
     * @return True or false, with a 50% probability.
     */
    bool getBool() {
        std::bernoulli_distribution dist(0.5);
        return dist(gen);
    }
    /**
             * @brief Generates a random, bright, and saturated QColor.
             * @return A QColor object (useful for UI).
             */
    QColor getRandomColor() {
        int hue = getInt(0, 359);   // Hue (0-360)
        int sat = getInt(150, 255); // Saturation (kept high)
        int val = getInt(180, 255); // Value/Brightness (kept high)
        return QColor::fromHsv(hue, sat, val);
    }
    /**
             * @brief Picks a random element from a given vector.
             * @tparam T The type of element in the vector.
             * @param items The vector to pick from.
             * @return A copy of a randomly selected element.
             * @throws std::runtime_error if the input vector is empty.
             */
    template<typename T>
    T getChoice(const std::vector<T>& items) {
        if (items.empty()) {
            throw std::runtime_error("getChoice() called with empty vector!");
        }
        std::uniform_int_distribution<> dist(0, static_cast<int>(items.size()) - 1);
        return items[dist(gen)];
    }
    /**
             * @brief Loads names from a text file (one name per line).
             * @param filePath The path to the file.
             * @return A vector of strings. Returns empty vector on failure.
             */
    std::vector<std::string> loadNamesFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        std::vector<std::string> names;
        std::string line;
        if (!file.is_open()) {
            std::cerr << "Не вдалося відкрити файл: " << filePath << std::endl;
            return names;
        }
        while (std::getline(file, line)) {
            if (!line.empty()) names.push_back(line);
        }
        return names;
    }
    /**
            * @brief Picks a random name from a file.
            * @param filePath The path to the file containing names.
            * @return A single random name. Returns "Unnamed" on failure.
            */
    std::string getRandomNameFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Cannot open name file: " << filePath << std::endl;
            return "Unnamed";
        }
        std::vector<std::string> names = loadNamesFromFile(filePath);
        if (names.empty()) return "Unnamed";
        return getChoice(names);
    }
    /**
     * @brief Gets a reference to the internal random engine.
     * @note This is needed for algorithms like std::shuffle.
     * @return A reference to the std::mt19937 engine.
     */
    std::mt19937& getEngine() { return gen; }



};
#endif //RANDOMUTILITIES_H

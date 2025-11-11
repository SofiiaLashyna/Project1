#ifndef RANDOMUTILITIES_H
#define RANDOMUTILITIES_H
#include <fstream>
#include <iostream>
#include <random>
#include <QColor>


class RandomGenerator {
private:
    std::mt19937 gen;
public:
    RandomGenerator() : gen(std::random_device{}()) {}

    int getInt(int min, int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }

    double getDouble(double min, double max) {
        std::uniform_real_distribution<> dist(min, max);
        return dist(gen);
    }
    bool getBool() {
        std::bernoulli_distribution dist(0.5);
        return dist(gen);
    }

    QColor getRandomColor() {
        int hue = getInt(0, 359);   // Відтінок (0-360)
        int sat = getInt(150, 255); // Насиченість (чуттвво > щоб не виглядало блякло)
        int val = getInt(180, 255); // Яскравість
        return QColor::fromHsv(hue, sat, val);
    }

    template<typename T>
    T getChoice(const std::vector<T>& items) {
        if (items.empty()) {
            throw std::runtime_error("getChoice() called with empty vector!");
        }
        std::uniform_int_distribution<> dist(0, static_cast<int>(items.size()) - 1);
        return items[dist(gen)];
    }

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


};
#endif //RANDOMUTILITIES_H

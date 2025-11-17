#ifndef RAIIGUARD_H
#define RAIIGUARD_H

#include <iostream>
#include <sstream>

/**
 * @brief RAII guard for temporarily redirecting std::cout to a std::stringstream.
 *
 * Useful for testing or capturing function output without modifying global std::cout.
 * Restores the original buffer in the destructor.
 */
struct CoutGuard {
    std::streambuf* old_buf; ///< Original std::cout buffer
    std::stringstream& ss;   ///< Temporary buffer to store output

    /**
     * @brief Constructs the guard and redirects std::cout to the given stringstream.
     * @param new_output Reference to a stringstream to capture output.
     */
    CoutGuard(std::stringstream& new_output) : ss(new_output) {
        old_buf = std::cout.rdbuf(ss.rdbuf());
    }

    /**
     * @brief Destructor restores the original std::cout buffer.
     */
    ~CoutGuard() {
        std::cout.rdbuf(old_buf);
    }
};

#endif // RAIIGUARD_H

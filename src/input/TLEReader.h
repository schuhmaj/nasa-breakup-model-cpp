#pragma once

#include <utility>
#include <array>
#include <vector>
#include <string>
#include <fstream>

/**
 * Provides the functionality to parse a TLE (Two-Line-Format).
 * @note The TLE reader ONLY extracts arguments used by the simulation the rest is "thrown away". This behavior can be
 * modified if wished.
 */
class TLEReader {

    const std::string _filename;

public:

    explicit TLEReader(std::string filename)
        : _filename(std::move(filename)) {}

    virtual ~TLEReader() = default;

    /**
     * Returns an vector consisting of pairs of <ID, KeplerianElements> from the TLE file.
     * The Keplerian Elements are sorted in the order:<br>
     * a (semir-major-axis) , e (eccentricity), i (inclination), W (longitude of the ascending node),
     * w (argument of periapsis), MA (mean Anomaly)
     * @return vector of paris containing <ID, KeplerElements>
     */
    std::vector<std::pair<size_t, std::array<double, 6>>> getIDKepler();

private:

    /**
     * Parses the next two lines of the given istream.
     * @param istream - the input stream
     * @return a pair of <ID, KeplerElements>
     */
    std::pair<size_t, std::array<double, 6>> getTwoLine(std::istream &istream);

};

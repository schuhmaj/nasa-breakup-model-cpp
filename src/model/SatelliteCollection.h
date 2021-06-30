#pragma once

#include <vector>
#include "Satellite.h"

/**
 * Representation of a Quantity of satellites.
 */
class SatelliteCollection {

    std::vector<Satellite> _satellites;

public:

    /**
     * Creates a new empty SatelliteCollection.
     */
    SatelliteCollection()
            : _satellites{} {}

    /**
     * Creates a new empty SatelliteCollection and tries to reserve memory.
     * @param reserve - how much memory should be reserved
     */
    explicit SatelliteCollection(long reserve)
            : _satellites{} {
        _satellites.reserve(reserve);
    }

    /**
     * Initializes a SatelliteCollection with an already filled vector
     * @param satellites - vector containing the satellites
     */
    explicit SatelliteCollection(std::vector<Satellite> &satellites)
            : _satellites{satellites} {}


    /**
     * Push back a new Satellite to the end of the SatelliteCollection.
     * @param satellite - const Satellite&
     */
    void push_back(const Satellite &satellite);

    /**
     * Push back a new Satellite to the end of the SatelliteCollection (Move-Value variant).
     * @param satellite - Satellite&&
     */
    void push_back(Satellite &&satellite);

    /**
     * Removes a specific satellite from the SatelliteCollection.
     * @attention Does not sustain the order!
     * @param satellite - Satellite&
     */
    void remove(Satellite &satellite);

    /**
     * Returns the size of the SatelliteCollection.
     * @return size
     */
    long size();

    /**
     * Returns an iterator to the beginning of the SatelliteCollection.
     * @return iterator
     */
    std::vector<Satellite>::const_iterator begin() const;

    /**
    * Returns an iterator to the end of the SatelliteCollection.
    * @return iterator
    */
    std::vector<Satellite>::const_iterator end() const;

    /**
     * Returns element n
     * @param n - the index
     * @return reference to Satellite
     */
    Satellite &operator[](unsigned long n);

};


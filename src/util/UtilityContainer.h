#pragma once

#include <array>
#include <numeric>
#include <functional>
#include <cmath>

namespace util {

    /**
     * Applies a binary function to elements of two objects piece by piece. The objects must
     * be iterable and should have the same size. If they have not the same size the function is applied as long
     * as the smaller one still have elements.
     * @tparam Container - a iterable object like an array or vector
     * @tparam BinOp - a binary function to apply
     * @param lhs - the first container
     * @param rhs - the second container
     * @param binOp - the binary function
     * @return a container containing the result
     */
    template<typename Container, typename BinOp>
    Container applyBinaryFunction(const Container &lhs, const Container &rhs, BinOp binOp) {
        Container ret = lhs;

        auto lhsIt = std::begin(lhs);
        auto lhsEnd = std::end(lhs);
        auto rhsIt = std::begin(rhs);
        auto rhsEnd = std::end(rhs);

        for (auto retIt = std::begin(ret); lhsIt != lhsEnd && rhsIt != rhsEnd; ++lhsIt, ++rhsIt, ++retIt) {
            *retIt = binOp(*lhsIt, *rhsIt);
        }
        return ret;
    }

    /**
     * Applies a binary function to elements of two objects piece by piece. The objects must
     * be iterable and should have the same size. If they have not the same size the function is applied as long
     * as the smaller one still have elements.
     * @tparam Container - a iterable object like an array or vector
     * @tparam Scalar - a scalar to use on each element
     * @tparam BinOp - a binary function to apply
     * @param lhs - the first container
     * @param scalar - a scalar to use on each element
     * @param binOp - the binary function
     * @return a container containing the result
     */
    template<typename Container, typename Scalar,typename BinOp>
    Container applyBinaryFunction(const Container &lhs, const Scalar &scalar, BinOp binOp) {
        Container ret = lhs;

        auto lhsIt = std::begin(lhs);
        auto lhsEnd = std::end(lhs);

        for (auto retIt = std::begin(ret); lhsIt != lhsEnd; ++lhsIt, ++retIt) {
            *retIt = binOp(*lhsIt, scalar);
        }
        return ret;
    }

    /**
     * Applies the Operation Minus to two Container piece by piece.
     * @example {1, 2, 3} - {1, 1, 1} = {0, 1, 2}
     * @tparam Container
     * @param lhs - minuend
     * @param rhs - subtrahend
     * @return the difference
     */
    template<typename Container>
    Container operator-(const Container &lhs, const Container &rhs) {
        return applyBinaryFunction(lhs, rhs, std::minus<>());
    }

    /**
    * Applies the Operation Plus to two Container piece by piece.
    * @example {1, 2, 3} + {1, 1, 1} = {2, 3, 4}
    * @tparam Container
    * @param lhs - addend
    * @param rhs - addend
    * @return the sum
    */
    template<typename Container>
    Container operator+(const Container &lhs, const Container &rhs) {
        return applyBinaryFunction(lhs, rhs, std::plus<>());
    }

    /**
     * Applies the euclidean norm/ L2-norm to a Container (e.g. a vector)
     * @tparam Container - must be iterable
     * @param container - e.g. a vector
     * @return an double containing the L2 norm
     */
    template<typename Container>
    double euclideanNorm(const Container &container) {
        return std::sqrt(
                std::accumulate(
                        std::begin(container),
                        std::end(container),
                        0,
                        [](const double acc, const double x) -> double { return acc + x * x; }
                )
        );
    }

}
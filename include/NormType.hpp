#ifndef NORMTYPE_HPP
#define NORMTYPE_HPP

/**
 * @brief Enumeration of different matrix norm types.
 * 
 * This enum class defines the available types of norms that can be computed for a matrix:
 * - @ref NormType::One : Maximum absolute column sum.
 * - @ref NormType::Infinity : Maximum absolute row sum.
 * - @ref NormType::Frobenius : Square root of the sum of the absolute squares of its elements.
 */
enum class NormType {
    One,        /**< One norm (maximum column sum). */
    Infinity,   /**< Infinity norm (maximum row sum). */
    Frobenius   /**< Frobenius norm (square root of sum of squares). */
};

#endif // NORMTYPE_HPP

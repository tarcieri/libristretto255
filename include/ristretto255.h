/**
 * @file ristretto255.h
 * @author Mike Hamburg
 *
 * @copyright
 *   Copyright (c) 2015-2018 Ristretto Developers, Cryptography Research, Inc.  \n
 *   Released under the MIT License.  See LICENSE.txt for license information.
 *
 * @brief A group of prime order p, based on Curve25519.
 */

#include <stdint.h>
#if defined (__GNUC__)  // File only exists for GNU compilers
#include <sys/types.h>
#endif

#ifndef __RISTRETTO255_H__
#define __RISTRETTO255_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

/* Aliasing MSVC preprocessing to GNU preprocessing */
#if defined _MSC_VER
#   define __attribute__(x)        // Turn off attribute code
#   define __attribute(x)
#   define __restrict__ __restrict  // Use MSVC restrict code
//#   define RISTRETTO_NOINLINE __declspec(noinline) // MSVC for noinline
//#   define RISTRETTO_INLINE __forceinline // MSVC for always inline
//#   define RISTRETTO_WARN_UNUSED _Check_return_
#endif

// The following are disabled for MSVC
#define RISTRETTO_NOINLINE  __attribute__((noinline))
#define RISTRETTO_INLINE inline __attribute__((always_inline,unused))
#define RISTRETTO_WARN_UNUSED __attribute__((warn_unused_result))
#define RISTRETTO_NONNULL __attribute__((nonnull))
// Cribbed from libnotmuch
#if defined (__clang_major__) && __clang_major__ >= 3 \
    || defined (__GNUC__) && __GNUC__ >= 5 \
    || defined (__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 5
#define RISTRETTO_DEPRECATED(msg) __attribute__ ((deprecated(msg)))
#else
#define RISTRETTO_DEPRECATED(msg) __attribute__ ((deprecated))
#endif
/** @endcond */

/* Internal word types.
 *
 * Somewhat tricky.  This could be decided separately per platform.  However,
 * the structs do need to be all the same size and alignment on a given
 * platform to support dynamic linking, since even if you header was built
 * with eg arch_neon, you might end up linking a library built with arch_arm32.
 */
#ifndef RISTRETTO_WORD_BITS
    #if (defined(__ILP64__) || defined(__amd64__) || defined(__x86_64__) || (((__UINT_FAST32_MAX__)>>30)>>30))
        #define RISTRETTO_WORD_BITS 64 /**< The number of bits in a word */
    #else
        #define RISTRETTO_WORD_BITS 32 /**< The number of bits in a word */
    #endif
#endif

#if RISTRETTO_WORD_BITS == 64
typedef uint64_t ristretto_word_t;      /**< Word size for internal computations */
typedef int64_t ristretto_sword_t;      /**< Signed word size for internal computations */
typedef uint64_t ristretto_bool_t;      /**< "Boolean" type, will be set to all-zero or all-one (i.e. -1u) */
typedef __uint128_t ristretto_dword_t;  /**< Double-word size for internal computations */
typedef __int128_t ristretto_dsword_t;  /**< Signed double-word size for internal computations */
#elif RISTRETTO_WORD_BITS == 32         /**< The number of bits in a word */
typedef uint32_t ristretto_word_t;      /**< Word size for internal computations */
typedef int32_t ristretto_sword_t;      /**< Signed word size for internal computations */
typedef uint32_t ristretto_bool_t;      /**< "Boolean" type, will be set to all-zero or all-one (i.e. -1u) */
typedef uint64_t ristretto_dword_t;     /**< Double-word size for internal computations */
typedef int64_t ristretto_dsword_t;     /**< Signed double-word size for internal computations */
#else
#error "Only supporting RISTRETTO_WORD_BITS = 32 or 64 for now"
#endif

/** RISTRETTO_TRUE = -1 so that RISTRETTO_TRUE & x = x */
extern const ristretto_bool_t RISTRETTO_TRUE;

/** RISTRETTO_FALSE = 0 so that RISTRETTO_FALSE & x = 0 */
extern const ristretto_bool_t RISTRETTO_FALSE;

/** Another boolean type used to indicate success or failure. */
typedef enum {
    RISTRETTO_SUCCESS = -1, /**< The operation succeeded. */
    RISTRETTO_FAILURE = 0   /**< The operation failed. */
} ristretto_error_t;


/** Return success if x is true */
static RISTRETTO_INLINE ristretto_error_t
ristretto_succeed_if(ristretto_bool_t x) {
    return (ristretto_error_t)x;
}

/** Return RISTRETTO_TRUE iff x == RISTRETTO_SUCCESS */
static RISTRETTO_INLINE ristretto_bool_t
ristretto_successful(ristretto_error_t e) {
    ristretto_dword_t w = ((ristretto_word_t)e) ^ ((ristretto_word_t)RISTRETTO_SUCCESS);
    return (w-1)>>RISTRETTO_WORD_BITS;
}

/** Overwrite data with zeros.  Uses memset_s if available. */
void ristretto_bzero (
    void *data,
    size_t size
) RISTRETTO_NONNULL;

/** @cond internal */
#define RISTRETTO255_SCALAR_LIMBS ((253-1)/RISTRETTO_WORD_BITS+1)
/** @endcond */

/** The number of bits in a scalar */
#define RISTRETTO255_SCALAR_BITS 253

/** @cond internal */
/** @brief Galois field element internal structure */
#define RISTRETTO255_FIELD_LIMBS (40/sizeof(ristretto_word_t))

typedef struct {
    ristretto_word_t limb[RISTRETTO255_FIELD_LIMBS];
} __attribute__((aligned(32))) gf_25519_t;
/** @endcond */

/** Number of bytes in a serialized point. */
#define RISTRETTO255_SER_BYTES 32

/** Number of bytes in an elligated point.  For now set the same as SER_BYTES
 * but could be different for other curves.
 */
#define RISTRETTO255_HASH_BYTES 32

/** Number of bytes in a serialized scalar. */
#define RISTRETTO255_SCALAR_BYTES 32

/** Number of bits in the "which" field of an elligator inverse */
#define RISTRETTO255_INVERT_ELLIGATOR_WHICH_BITS 5

/** The cofactor the curve would have, if we hadn't removed it */
#define RISTRETTO255_REMOVED_COFACTOR 8

/** Representation of a point on the elliptic curve. */
typedef struct {
    /** @cond internal */
    gf_25519_t x,y,z,t; /* Twisted extended homogeneous coordinates */
    /** @endcond */
} ristretto255_point_t;

/** Precomputed table based on a point.  Can be trivial implementation. */
struct ristretto255_precomputed_s;

/** Precomputed table based on a point.  Can be trivial implementation. */
typedef struct ristretto255_precomputed_s ristretto255_precomputed_s;

/** Size and alignment of precomputed point tables. */
extern const size_t ristretto255_sizeof_precomputed_s, ristretto255_alignof_precomputed_s;

/** Representation of an element of the scalar field. */
typedef struct {
    /** @cond internal */
    ristretto_word_t limb[RISTRETTO255_SCALAR_LIMBS];
    /** @endcond */
} ristretto255_scalar_t;

#if defined _MSC_VER

/** The scalar 1. */
extern const ristretto255_scalar_t ristretto255_scalar_one;

/** The scalar 0. */
extern const ristretto255_scalar_t ristretto255_scalar_zero;

/** The identity (zero) point on the curve. */
extern const ristretto255_point_t ristretto255_point_identity;

/** An arbitrarily-chosen base point on the curve. */
extern const ristretto255_point_t ristretto255_point_base;

/** Precomputed table of multiples of the base point on the curve. */
extern const struct ristretto255_precomputed_s *ristretto255_precomputed_base;


#else // _MSC_VER

/** The scalar 1. */
extern const ristretto255_scalar_t ristretto255_scalar_one;

/** The scalar 0. */
extern const ristretto255_scalar_t ristretto255_scalar_zero;

/** The identity (zero) point on the curve. */
extern const ristretto255_point_t ristretto255_point_identity;

/** An arbitrarily-chosen base point on the curve. */
extern const ristretto255_point_t ristretto255_point_base;

/** Precomputed table of multiples of the base point on the curve. */
extern const ristretto255_precomputed_s *ristretto255_precomputed_base;

#endif // _MSC_VER
/**
 * @brief Read a scalar from wire format or from bytes.
 *
 * @param [in] ser Serialized form of a scalar.
 * @param [out] out Deserialized form.
 *
 * @retval RISTRETTO_SUCCESS The scalar was correctly encoded.
 * @retval RISTRETTO_FAILURE The scalar was greater than the modulus,
 * and has been reduced modulo that modulus.
 */
ristretto_error_t ristretto255_scalar_decode (
    ristretto255_scalar_t *out,
    const unsigned char ser[RISTRETTO255_SCALAR_BYTES]
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Read a scalar from wire format or from bytes.  Reduces mod
 * scalar prime.
 *
 * @param [in] ser Serialized form of a scalar.
 * @param [in] ser_len Length of serialized form.
 * @param [out] out Deserialized form.
 */
void ristretto255_scalar_decode_long (
    ristretto255_scalar_t *out,
    const unsigned char *ser,
    size_t ser_len
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Serialize a scalar to wire format.
 *
 * @param [out] ser Serialized form of a scalar.
 * @param [in] s Deserialized scalar.
 */
void ristretto255_scalar_encode (
    unsigned char ser[RISTRETTO255_SCALAR_BYTES],
    const ristretto255_scalar_t *s
) RISTRETTO_NONNULL RISTRETTO_NOINLINE RISTRETTO_NOINLINE;

/**
 * @brief Add two scalars.  The scalars may use the same memory.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @param [out] out a+b.
 */
void ristretto255_scalar_add (
    ristretto255_scalar_t *out,
    const ristretto255_scalar_t *a,
    const ristretto255_scalar_t *b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Compare two scalars.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @retval RISTRETTO_TRUE The scalars are equal.
 * @retval RISTRETTO_FALSE The scalars are not equal.
 */
ristretto_bool_t ristretto255_scalar_eq (
    const ristretto255_scalar_t *a,
    const ristretto255_scalar_t *b
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Subtract two scalars.  The scalars may use the same memory.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @param [out] out a-b.
 */
void ristretto255_scalar_sub (
    ristretto255_scalar_t *out,
    const ristretto255_scalar_t *a,
    const ristretto255_scalar_t *b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply two scalars.  The scalars may use the same memory.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @param [out] out a*b.
 */
void ristretto255_scalar_mul (
    ristretto255_scalar_t *out,
    const ristretto255_scalar_t *a,
    const ristretto255_scalar_t *b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
* @brief Halve a scalar.  The scalars may use the same memory.
* @param [in] a A scalar.
* @param [out] out a/2.
*/
void ristretto255_scalar_halve (
   ristretto255_scalar_t *out,
   const ristretto255_scalar_t *a
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Invert a scalar.  When passed zero, return 0.  The input and output may alias.
 * @param [in] a A scalar.
 * @param [out] out 1/a.
 * @return RISTRETTO_SUCCESS The input is nonzero.
 */
ristretto_error_t ristretto255_scalar_invert (
    ristretto255_scalar_t *out,
    const ristretto255_scalar_t *a
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Copy a scalar.  The scalars may use the same memory, in which
 * case this function does nothing.
 * @param [in] a A scalar.
 * @param [out] out Will become a copy of a.
 */
static inline void RISTRETTO_NONNULL ristretto255_scalar_copy (
    ristretto255_scalar_t *out,
    const ristretto255_scalar_t *a
) {
    *out = *a;
}

/**
 * @brief Set a scalar to an unsigned 64-bit integer.
 * @param [in] a An integer.
 * @param [out] out Will become equal to a.
 */
void ristretto255_scalar_set_unsigned (
    ristretto255_scalar_t *out,
    uint64_t a
) RISTRETTO_NONNULL;

/**
 * @brief Encode a point as a sequence of bytes.
 *
 * @param [out] ser The byte representation of the point.
 * @param [in] pt The point to encode.
 */
void ristretto255_point_encode (
    uint8_t ser[RISTRETTO255_SER_BYTES],
    const ristretto255_point_t *pt
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Decode a point from a sequence of bytes.
 *
 * Every point has a unique encoding, so not every
 * sequence of bytes is a valid encoding.  If an invalid
 * encoding is given, the output is undefined.
 *
 * @param [out] pt The decoded point.
 * @param [in] ser The serialized version of the point.
 * @param [in] allow_identity RISTRETTO_TRUE if the identity is a legal input.
 * @retval RISTRETTO_SUCCESS The decoding succeeded.
 * @retval RISTRETTO_FAILURE The decoding didn't succeed, because
 * ser does not represent a point.
 */
ristretto_error_t ristretto255_point_decode (
    ristretto255_point_t *pt,
    const uint8_t ser[RISTRETTO255_SER_BYTES],
    ristretto_bool_t allow_identity
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Copy a point.  The input and output may alias,
 * in which case this function does nothing.
 *
 * @param [out] a A copy of the point.
 * @param [in] b Any point.
 */
static inline void RISTRETTO_NONNULL ristretto255_point_copy (
    ristretto255_point_t *a,
    const ristretto255_point_t *b
) {
    *a=*b;
}

/**
 * @brief Test whether two points are equal.  If yes, return
 * RISTRETTO_TRUE, else return RISTRETTO_FALSE.
 *
 * @param [in] a A point.
 * @param [in] b Another point.
 * @retval RISTRETTO_TRUE The points are equal.
 * @retval RISTRETTO_FALSE The points are not equal.
 */
ristretto_bool_t ristretto255_point_eq (
    const ristretto255_point_t *a,
    const ristretto255_point_t *b
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Add two points to produce a third point.  The
 * input points and output point can be pointers to the same
 * memory.
 *
 * @param [out] sum The sum a+b.
 * @param [in] a An addend.
 * @param [in] b An addend.
 */
void ristretto255_point_add (
    ristretto255_point_t *sum,
    const ristretto255_point_t *a,
    const ristretto255_point_t *b
) RISTRETTO_NONNULL;

/**
 * @brief Double a point.  Equivalent to
 * ristretto255_point_add(two_a,a,a), but potentially faster.
 *
 * @param [out] two_a The sum a+a.
 * @param [in] a A point.
 */
void ristretto255_point_double (
    ristretto255_point_t *two_a,
    const ristretto255_point_t *a
) RISTRETTO_NONNULL;

/**
 * @brief Subtract two points to produce a third point.  The
 * input points and output point can be pointers to the same
 * memory.
 *
 * @param [out] diff The difference a-b.
 * @param [in] a The minuend.
 * @param [in] b The subtrahend.
 */
void ristretto255_point_sub (
    ristretto255_point_t *diff,
    const ristretto255_point_t *a,
    const ristretto255_point_t *b
) RISTRETTO_NONNULL;

/**
 * @brief Negate a point to produce another point.  The input
 * and output points can use the same memory.
 *
 * @param [out] nega The negated input point
 * @param [in] a The input point.
 */
void ristretto255_point_negate (
   ristretto255_point_t *nega,
   const ristretto255_point_t *a
) RISTRETTO_NONNULL;

/**
 * @brief Multiply a base point by a scalar: scaled = scalar*base.
 *
 * @param [out] scaled The scaled point base*scalar
 * @param [in] base The point to be scaled.
 * @param [in] scalar The scalar to multiply by.
 */
void ristretto255_point_scalarmul (
    ristretto255_point_t *scaled,
    const ristretto255_point_t *base,
    const ristretto255_scalar_t *scalar
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply a base point by a scalar: scaled = scalar*base.
 * This function operates directly on serialized forms.
 *
 * @warning This function is experimental.  It may not be supported
 * long-term.
 *
 * @param [out] scaled The scaled point base*scalar
 * @param [in] base The point to be scaled.
 * @param [in] scalar The scalar to multiply by.
 * @param [in] allow_identity Allow the input to be the identity.
 * @param [in] short_circuit Allow a fast return if the input is illegal.
 *
 * @retval RISTRETTO_SUCCESS The scalarmul succeeded.
 * @retval RISTRETTO_FAILURE The scalarmul didn't succeed, because
 * base does not represent a point.
 */
ristretto_error_t ristretto255_direct_scalarmul (
    uint8_t scaled[RISTRETTO255_SER_BYTES],
    const uint8_t base[RISTRETTO255_SER_BYTES],
    const ristretto255_scalar_t *scalar,
    ristretto_bool_t allow_identity,
    ristretto_bool_t short_circuit
) RISTRETTO_NONNULL RISTRETTO_WARN_UNUSED RISTRETTO_NOINLINE;

/**
 * @brief Precompute a table for fast scalar multiplication.
 * Some implementations do not include precomputed points; for
 * those implementations, this implementation simply copies the
 * point.
 *
 * @param [out] a A precomputed table of multiples of the point.
 * @param [in] b Any point.
 */
void ristretto255_precompute (
    ristretto255_precomputed_s *a,
    const ristretto255_point_t *b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply a precomputed base point by a scalar:
 * scaled = scalar*base.
 * Some implementations do not include precomputed points; for
 * those implementations, this function is the same as
 * ristretto255_point_scalarmul
 *
 * @param [out] scaled The scaled point base*scalar
 * @param [in] base The point to be scaled.
 * @param [in] scalar The scalar to multiply by.
 */
void ristretto255_precomputed_scalarmul (
    ristretto255_point_t *scaled,
    const ristretto255_precomputed_s *base,
    const ristretto255_scalar_t *scalar
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply two base points by two scalars:
 * scaled = scalar1*base1 + scalar2*base2.
 *
 * Equivalent to two calls to ristretto255_point_scalarmul, but may be
 * faster.
 *
 * @param [out] combo The linear combination scalar1*base1 + scalar2*base2.
 * @param [in] base1 A first point to be scaled.
 * @param [in] scalar1 A first scalar to multiply by.
 * @param [in] base2 A second point to be scaled.
 * @param [in] scalar2 A second scalar to multiply by.
 */
void ristretto255_point_double_scalarmul (
    ristretto255_point_t *combo,
    const ristretto255_point_t *base1,
    const ristretto255_scalar_t *scalar1,
    const ristretto255_point_t *base2,
    const ristretto255_scalar_t *scalar2
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * Multiply one base point by two scalars:
 *
 * a1 = scalar1 * base
 * a2 = scalar2 * base
 *
 * Equivalent to two calls to ristretto255_point_scalarmul, but may be
 * faster.
 *
 * @param [out] a1 The first multiple.  It may be the same as the input point.
 * @param [out] a2 The second multiple.  It may be the same as the input point.
 * @param [in] base1 A point to be scaled.
 * @param [in] scalar1 A first scalar to multiply by.
 * @param [in] scalar2 A second scalar to multiply by.
 */
void ristretto255_point_dual_scalarmul (
    ristretto255_point_t *a1,
    ristretto255_point_t *a2,
    const ristretto255_point_t *base1,
    const ristretto255_scalar_t *scalar1,
    const ristretto255_scalar_t *scalar2
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply two base points by two scalars:
 * scaled = scalar1*ristretto255_point_base + scalar2*base2.
 *
 * Otherwise equivalent to ristretto255_point_double_scalarmul, but may be
 * faster at the expense of being variable time.
 *
 * @param [out] combo The linear combination scalar1*base + scalar2*base2.
 * @param [in] scalar1 A first scalar to multiply by.
 * @param [in] base2 A second point to be scaled.
 * @param [in] scalar2 A second scalar to multiply by.
 *
 * @warning: This function takes variable time, and may leak the scalars
 * used.  It is designed for signature verification.
 */
void ristretto255_base_double_scalarmul_non_secret (
    ristretto255_point_t *combo,
    const ristretto255_scalar_t *scalar1,
    const ristretto255_point_t *base2,
    const ristretto255_scalar_t *scalar2
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Constant-time decision between two points.  If pick_b
 * is zero, out = a; else out = b.
 *
 * @param [out] out The output.  It may be the same as either input.
 * @param [in] a Any point.
 * @param [in] b Any point.
 * @param [in] pick_b If nonzero, choose point b.
 */
void ristretto255_point_cond_sel (
    ristretto255_point_t *out,
    const ristretto255_point_t *a,
    const ristretto255_point_t *b,
    ristretto_word_t pick_b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Constant-time decision between two scalars.  If pick_b
 * is zero, out = a; else out = b.
 *
 * @param [out] out The output.  It may be the same as either input.
 * @param [in] a Any scalar.
 * @param [in] b Any scalar.
 * @param [in] pick_b If nonzero, choose scalar b.
 */
void ristretto255_scalar_cond_sel (
    ristretto255_scalar_t *out,
    const ristretto255_scalar_t *a,
    const ristretto255_scalar_t *b,
    ristretto_word_t pick_b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Test that a point is valid, for debugging purposes.
 *
 * @param [in] to_test The point to test.
 * @retval RISTRETTO_TRUE The point is valid.
 * @retval RISTRETTO_FALSE The point is invalid.
 */
ristretto_bool_t ristretto255_point_valid (
    const ristretto255_point_t *to_test
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Torque a point, for debugging purposes.  The output
 * will be equal to the input.
 *
 * @param [out] q The point to torque.
 * @param [in] p The point to torque.
 */
void ristretto255_point_debugging_torque (
    ristretto255_point_t *q,
    const ristretto255_point_t *p
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Projectively scale a point, for debugging purposes.
 * The output will be equal to the input, and will be valid
 * even if the factor is zero.
 *
 * @param [out] q The point to scale.
 * @param [in] p The point to scale.
 * @param [in] factor Serialized GF factor to scale.
 */
void ristretto255_point_debugging_pscale (
    ristretto255_point_t *q,
    const ristretto255_point_t *p,
    const unsigned char factor[RISTRETTO255_SER_BYTES]
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Almost-Elligator-like hash to curve.
 *
 * Call this function with the output of a hash to make a hash to the curve.
 *
 * This function runs Elligator2 on the ristretto255 Jacobi quartic model.  It then
 * uses the isogeny to put the result in twisted Edwards form.  As a result,
 * it is safe (cannot produce points of order 4), and would be compatible with
 * hypothetical other implementations of Decaf using a Montgomery or untwisted
 * Edwards model.
 *
 * Unlike Elligator, this function may be up to 4:1 on [0,(p-1)/2]:
 *   A factor of 2 due to the isogeny.
 *   A factor of 2 because we quotient out the 2-torsion.
 *
 * This makes it about 8:1 overall, or 16:1 overall on curves with cofactor 8.
 *
 * Negating the input (mod q) results in the same point.  Inverting the input
 * (mod q) results in the negative point.  This is the same as Elligator.
 *
 * This function isn't quite indifferentiable from a random oracle.
 * However, it is suitable for many protocols, including SPEKE and SPAKE2 EE.
 * Furthermore, calling it twice with independent seeds and adding the results
 * is indifferentiable from a random oracle.
 *
 * @param [in] hashed_data Output of some hash function.
 * @param [out] pt The data hashed to the curve.
 */
void ristretto255_point_from_hash_nonuniform (
    ristretto255_point_t *pt,
    const unsigned char hashed_data[RISTRETTO255_HASH_BYTES]
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Indifferentiable hash function encoding to curve.
 *
 * Equivalent to calling ristretto255_point_from_hash_nonuniform twice and adding.
 *
 * @param [in] hashed_data Output of some hash function.
 * @param [out] pt The data hashed to the curve.
 */
void ristretto255_point_from_hash_uniform (
    ristretto255_point_t *pt,
    const unsigned char hashed_data[2*RISTRETTO255_HASH_BYTES]
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Inverse of elligator-like hash to curve.
 *
 * This function writes to the buffer, to make it so that
 * ristretto255_point_from_hash_nonuniform(buffer) = pt if
 * possible.  Since there may be multiple preimages, the
 * "which" parameter chooses between them.  To ensure uniform
 * inverse sampling, this function succeeds or fails
 * independently for different "which" values.
 *
 * This function isn't guaranteed to find every possible
 * preimage, but it finds all except a small finite number.
 * In particular, when the number of bits in the modulus isn't
 * a multiple of 8 (i.e. for curve25519), it sets the high bits
 * independently, which enables the generated data to be uniform.
 * But it doesn't add p, so you'll never get exactly p from this
 * function.  This might change in the future, especially if
 * we ever support eg Brainpool curves, where this could cause
 * real nonuniformity.
 *
 * @param [out] recovered_hash Encoded data.
 * @param [in] pt The point to encode.
 * @param [in] which A value determining which inverse point
 * to return.
 *
 * @retval RISTRETTO_SUCCESS The inverse succeeded.
 * @retval RISTRETTO_FAILURE The inverse failed.
 */
ristretto_error_t ristretto255_invert_elligator_nonuniform (
    unsigned char recovered_hash[RISTRETTO255_HASH_BYTES],
    const ristretto255_point_t *pt,
    uint32_t which
) RISTRETTO_NONNULL RISTRETTO_NOINLINE RISTRETTO_WARN_UNUSED;

/**
 * @brief Inverse of elligator-like hash to curve.
 *
 * This function writes to the buffer, to make it so that
 * ristretto255_point_from_hash_uniform(buffer) = pt if
 * possible.  Since there may be multiple preimages, the
 * "which" parameter chooses between them.  To ensure uniform
 * inverse sampling, this function succeeds or fails
 * independently for different "which" values.
 *
 * @param [out] recovered_hash Encoded data.
 * @param [in] pt The point to encode.
 * @param [in] which A value determining which inverse point
 * to return.
 *
 * @retval RISTRETTO_SUCCESS The inverse succeeded.
 * @retval RISTRETTO_FAILURE The inverse failed.
 */
ristretto_error_t ristretto255_invert_elligator_uniform (
    unsigned char recovered_hash[2*RISTRETTO255_HASH_BYTES],
    const ristretto255_point_t *pt,
    uint32_t which
) RISTRETTO_NONNULL RISTRETTO_NOINLINE RISTRETTO_WARN_UNUSED;

/** Securely erase a scalar. */
void ristretto255_scalar_destroy (
    ristretto255_scalar_t *scalar
) RISTRETTO_NONNULL;

/** Securely erase a point by overwriting it with zeros.
 * @warning This causes the point object to become invalid.
 */
void ristretto255_point_destroy (
    ristretto255_point_t *point
) RISTRETTO_NONNULL;

/** Securely erase a precomputed table by overwriting it with zeros.
 * @warning This causes the table object to become invalid.
 */
void ristretto255_precomputed_destroy (
    ristretto255_precomputed_s *pre
) RISTRETTO_NONNULL;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __RISTRETTO255_H__ */

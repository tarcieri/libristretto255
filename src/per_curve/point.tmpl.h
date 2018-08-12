/** @brief A group of prime order p, based on $(iso_to). */

#ifdef __cplusplus
extern "C" {
#endif

/** @cond internal */
#define RISTRETTO$(gf_bits)_SCALAR_LIMBS (($(scalar_bits)-1)/RISTRETTO_WORD_BITS+1)
/** @endcond */

/** The number of bits in a scalar */
#define RISTRETTO$(gf_bits)_SCALAR_BITS $(scalar_bits)

/** @cond internal */
#ifndef __RISTRETTO_$(gf_shortname)_GF_DEFINED__
#define __RISTRETTO_$(gf_shortname)_GF_DEFINED__ 1
/** @brief Galois field element internal structure */
typedef struct gf_$(gf_shortname)_s {
    ristretto$(gf_bits)_word_t limb[$(gf_impl_bits)/RISTRETTO_WORD_BITS];
} __attribute__((aligned(32))) gf_$(gf_shortname)_s, gf_$(gf_shortname)_t[1];
#endif /* __RISTRETTO_$(gf_shortname)_GF_DEFINED__ */
/** @endcond */

/** Number of bytes in a serialized point. */
#define RISTRETTO$(gf_bits)_SER_BYTES $((gf_bits-2)//8 + 1)

/** Number of bytes in an elligated point.  For now set the same as SER_BYTES
 * but could be different for other curves.
 */
#define RISTRETTO$(gf_bits)_HASH_BYTES $((gf_bits-2)//8 + 1)

/** Number of bytes in a serialized scalar. */
#define RISTRETTO$(gf_bits)_SCALAR_BYTES $((scalar_bits-1)//8 + 1)

/** Number of bits in the "which" field of an elligator inverse */
#define RISTRETTO$(gf_bits)_INVERT_ELLIGATOR_WHICH_BITS $(ceil_log2(cofactor) + 7 + elligator_onto - ((gf_bits-2) % 8))

/** The cofactor the curve would have, if we hadn't removed it */
#define RISTRETTO$(gf_bits)_REMOVED_COFACTOR $(cofactor)

/** Representation of a point on the elliptic curve. */
typedef struct ristretto$(gf_bits)_point_s {
    /** @cond internal */
    gf_$(gf_shortname)_t x,y,z,t; /* Twisted extended homogeneous coordinates */
    /** @endcond */
} ristretto$(gf_bits)_point_t[1];

/** Precomputed table based on a point.  Can be trivial implementation. */
struct ristretto$(gf_bits)_precomputed_s;

/** Precomputed table based on a point.  Can be trivial implementation. */
typedef struct ristretto$(gf_bits)_precomputed_s ristretto$(gf_bits)_precomputed_s; 

/** Size and alignment of precomputed point tables. */
RISTRETTO_API_VIS extern const size_t ristretto$(gf_bits)_sizeof_precomputed_s, ristretto$(gf_bits)_alignof_precomputed_s;

/** Representation of an element of the scalar field. */
typedef struct ristretto$(gf_bits)_scalar_s {
    /** @cond internal */
    ristretto$(gf_bits)_word_t limb[RISTRETTO$(gf_bits)_SCALAR_LIMBS];
    /** @endcond */
} ristretto$(gf_bits)_scalar_t[1];

#if defined _MSC_VER

/** The scalar 1. */
extern const ristretto$(gf_bits)_scalar_t RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_one;

/** The scalar 0. */
extern const ristretto$(gf_bits)_scalar_t RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_zero;

/** The identity (zero) point on the curve. */
extern const ristretto$(gf_bits)_point_t RISTRETTO_API_VIS ristretto$(gf_bits)_point_identity;

/** An arbitrarily-chosen base point on the curve. */
extern const ristretto$(gf_bits)_point_t RISTRETTO_API_VIS ristretto$(gf_bits)_point_base;

/** Precomputed table of multiples of the base point on the curve. */
extern const struct RISTRETTO_API_VIS ristretto$(gf_bits)_precomputed_s *ristretto$(gf_bits)_precomputed_base;


#else // _MSC_VER

/** The scalar 1. */
RISTRETTO_API_VIS extern const ristretto$(gf_bits)_scalar_t ristretto$(gf_bits)_scalar_one;

/** The scalar 0. */
RISTRETTO_API_VIS extern const ristretto$(gf_bits)_scalar_t ristretto$(gf_bits)_scalar_zero;

/** The identity (zero) point on the curve. */
RISTRETTO_API_VIS extern const ristretto$(gf_bits)_point_t ristretto$(gf_bits)_point_identity;

/** An arbitrarily-chosen base point on the curve. */
RISTRETTO_API_VIS extern const ristretto$(gf_bits)_point_t ristretto$(gf_bits)_point_base;

/** Precomputed table of multiples of the base point on the curve. */
RISTRETTO_API_VIS extern const struct ristretto$(gf_bits)_precomputed_s *ristretto$(gf_bits)_precomputed_base;

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
ristretto$(gf_bits)_error_t RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_decode (
    ristretto$(gf_bits)_scalar_t out,
    const unsigned char ser[RISTRETTO$(gf_bits)_SCALAR_BYTES]
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Read a scalar from wire format or from bytes.  Reduces mod
 * scalar prime.
 *
 * @param [in] ser Serialized form of a scalar.
 * @param [in] ser_len Length of serialized form.
 * @param [out] out Deserialized form.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_decode_long (
    ristretto$(gf_bits)_scalar_t out,
    const unsigned char *ser,
    size_t ser_len
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;
    
/**
 * @brief Serialize a scalar to wire format.
 *
 * @param [out] ser Serialized form of a scalar.
 * @param [in] s Deserialized scalar.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_encode (
    unsigned char ser[RISTRETTO$(gf_bits)_SCALAR_BYTES],
    const ristretto$(gf_bits)_scalar_t s
) RISTRETTO_NONNULL RISTRETTO_NOINLINE RISTRETTO_NOINLINE;
        
/**
 * @brief Add two scalars.  The scalars may use the same memory.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @param [out] out a+b.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_add (
    ristretto$(gf_bits)_scalar_t out,
    const ristretto$(gf_bits)_scalar_t a,
    const ristretto$(gf_bits)_scalar_t b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Compare two scalars.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @retval RISTRETTO_TRUE The scalars are equal.
 * @retval RISTRETTO_FALSE The scalars are not equal.
 */    
ristretto$(gf_bits)_bool_t RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_eq (
    const ristretto$(gf_bits)_scalar_t a,
    const ristretto$(gf_bits)_scalar_t b
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Subtract two scalars.  The scalars may use the same memory.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @param [out] out a-b.
 */  
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_sub (
    ristretto$(gf_bits)_scalar_t out,
    const ristretto$(gf_bits)_scalar_t a,
    const ristretto$(gf_bits)_scalar_t b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply two scalars.  The scalars may use the same memory.
 * @param [in] a One scalar.
 * @param [in] b Another scalar.
 * @param [out] out a*b.
 */  
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_mul (
    ristretto$(gf_bits)_scalar_t out,
    const ristretto$(gf_bits)_scalar_t a,
    const ristretto$(gf_bits)_scalar_t b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;
        
/**
* @brief Halve a scalar.  The scalars may use the same memory.
* @param [in] a A scalar.
* @param [out] out a/2.
*/
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_halve (
   ristretto$(gf_bits)_scalar_t out,
   const ristretto$(gf_bits)_scalar_t a
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Invert a scalar.  When passed zero, return 0.  The input and output may alias.
 * @param [in] a A scalar.
 * @param [out] out 1/a.
 * @return RISTRETTO_SUCCESS The input is nonzero.
 */  
ristretto$(gf_bits)_error_t RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_invert (
    ristretto$(gf_bits)_scalar_t out,
    const ristretto$(gf_bits)_scalar_t a
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Copy a scalar.  The scalars may use the same memory, in which
 * case this function does nothing.
 * @param [in] a A scalar.
 * @param [out] out Will become a copy of a.
 */
static inline void RISTRETTO_NONNULL ristretto$(gf_bits)_scalar_copy (
    ristretto$(gf_bits)_scalar_t out,
    const ristretto$(gf_bits)_scalar_t a
) {
    *out = *a;
}

/**
 * @brief Set a scalar to an unsigned 64-bit integer.
 * @param [in] a An integer.
 * @param [out] out Will become equal to a.
 */  
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_set_unsigned (
    ristretto$(gf_bits)_scalar_t out,
    uint64_t a
) RISTRETTO_NONNULL;

/**
 * @brief Encode a point as a sequence of bytes.
 *
 * @param [out] ser The byte representation of the point.
 * @param [in] pt The point to encode.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_encode (
    uint8_t ser[RISTRETTO$(gf_bits)_SER_BYTES],
    const ristretto$(gf_bits)_point_t pt
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
ristretto$(gf_bits)_error_t RISTRETTO_API_VIS ristretto$(gf_bits)_point_decode (
    ristretto$(gf_bits)_point_t pt,
    const uint8_t ser[RISTRETTO$(gf_bits)_SER_BYTES],
    ristretto$(gf_bits)_bool_t allow_identity
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Copy a point.  The input and output may alias,
 * in which case this function does nothing.
 *
 * @param [out] a A copy of the point.
 * @param [in] b Any point.
 */
static inline void RISTRETTO_NONNULL ristretto$(gf_bits)_point_copy (
    ristretto$(gf_bits)_point_t a,
    const ristretto$(gf_bits)_point_t b
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
ristretto$(gf_bits)_bool_t RISTRETTO_API_VIS ristretto$(gf_bits)_point_eq (
    const ristretto$(gf_bits)_point_t a,
    const ristretto$(gf_bits)_point_t b
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_add (
    ristretto$(gf_bits)_point_t sum,
    const ristretto$(gf_bits)_point_t a,
    const ristretto$(gf_bits)_point_t b
) RISTRETTO_NONNULL;

/**
 * @brief Double a point.  Equivalent to
 * ristretto$(gf_bits)_point_add(two_a,a,a), but potentially faster.
 *
 * @param [out] two_a The sum a+a.
 * @param [in] a A point.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_double (
    ristretto$(gf_bits)_point_t two_a,
    const ristretto$(gf_bits)_point_t a
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_sub (
    ristretto$(gf_bits)_point_t diff,
    const ristretto$(gf_bits)_point_t a,
    const ristretto$(gf_bits)_point_t b
) RISTRETTO_NONNULL;
    
/**
 * @brief Negate a point to produce another point.  The input
 * and output points can use the same memory.
 *
 * @param [out] nega The negated input point
 * @param [in] a The input point.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_negate (
   ristretto$(gf_bits)_point_t nega,
   const ristretto$(gf_bits)_point_t a
) RISTRETTO_NONNULL;

/**
 * @brief Multiply a base point by a scalar: scaled = scalar*base.
 *
 * @param [out] scaled The scaled point base*scalar
 * @param [in] base The point to be scaled.
 * @param [in] scalar The scalar to multiply by.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_scalarmul (
    ristretto$(gf_bits)_point_t scaled,
    const ristretto$(gf_bits)_point_t base,
    const ristretto$(gf_bits)_scalar_t scalar
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
ristretto$(gf_bits)_error_t RISTRETTO_API_VIS ristretto$(gf_bits)_direct_scalarmul (
    uint8_t scaled[RISTRETTO$(gf_bits)_SER_BYTES],
    const uint8_t base[RISTRETTO$(gf_bits)_SER_BYTES],
    const ristretto$(gf_bits)_scalar_t scalar,
    ristretto$(gf_bits)_bool_t allow_identity,
    ristretto$(gf_bits)_bool_t short_circuit
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_precompute (
    ristretto$(gf_bits)_precomputed_s *a,
    const ristretto$(gf_bits)_point_t b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply a precomputed base point by a scalar:
 * scaled = scalar*base.
 * Some implementations do not include precomputed points; for
 * those implementations, this function is the same as
 * ristretto$(gf_bits)_point_scalarmul
 *
 * @param [out] scaled The scaled point base*scalar
 * @param [in] base The point to be scaled.
 * @param [in] scalar The scalar to multiply by.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_precomputed_scalarmul (
    ristretto$(gf_bits)_point_t scaled,
    const ristretto$(gf_bits)_precomputed_s *base,
    const ristretto$(gf_bits)_scalar_t scalar
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply two base points by two scalars:
 * scaled = scalar1*base1 + scalar2*base2.
 *
 * Equivalent to two calls to ristretto$(gf_bits)_point_scalarmul, but may be
 * faster.
 *
 * @param [out] combo The linear combination scalar1*base1 + scalar2*base2.
 * @param [in] base1 A first point to be scaled.
 * @param [in] scalar1 A first scalar to multiply by.
 * @param [in] base2 A second point to be scaled.
 * @param [in] scalar2 A second scalar to multiply by.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_double_scalarmul (
    ristretto$(gf_bits)_point_t combo,
    const ristretto$(gf_bits)_point_t base1,
    const ristretto$(gf_bits)_scalar_t scalar1,
    const ristretto$(gf_bits)_point_t base2,
    const ristretto$(gf_bits)_scalar_t scalar2
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;
    
/**
 * Multiply one base point by two scalars:
 *
 * a1 = scalar1 * base
 * a2 = scalar2 * base
 *
 * Equivalent to two calls to ristretto$(gf_bits)_point_scalarmul, but may be
 * faster.
 *
 * @param [out] a1 The first multiple.  It may be the same as the input point.
 * @param [out] a2 The second multiple.  It may be the same as the input point.
 * @param [in] base1 A point to be scaled.
 * @param [in] scalar1 A first scalar to multiply by.
 * @param [in] scalar2 A second scalar to multiply by.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_dual_scalarmul (
    ristretto$(gf_bits)_point_t a1,
    ristretto$(gf_bits)_point_t a2,
    const ristretto$(gf_bits)_point_t base1,
    const ristretto$(gf_bits)_scalar_t scalar1,
    const ristretto$(gf_bits)_scalar_t scalar2
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Multiply two base points by two scalars:
 * scaled = scalar1*ristretto$(gf_bits)_point_base + scalar2*base2.
 *
 * Otherwise equivalent to ristretto$(gf_bits)_point_double_scalarmul, but may be
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_base_double_scalarmul_non_secret (
    ristretto$(gf_bits)_point_t combo,
    const ristretto$(gf_bits)_scalar_t scalar1,
    const ristretto$(gf_bits)_point_t base2,
    const ristretto$(gf_bits)_scalar_t scalar2
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_cond_sel (
    ristretto$(gf_bits)_point_t out,
    const ristretto$(gf_bits)_point_t a,
    const ristretto$(gf_bits)_point_t b,
    ristretto$(gf_bits)_word_t pick_b
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_cond_sel (
    ristretto$(gf_bits)_scalar_t out,
    const ristretto$(gf_bits)_scalar_t a,
    const ristretto$(gf_bits)_scalar_t b,
    ristretto$(gf_bits)_word_t pick_b
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Test that a point is valid, for debugging purposes.
 *
 * @param [in] to_test The point to test.
 * @retval RISTRETTO_TRUE The point is valid.
 * @retval RISTRETTO_FALSE The point is invalid.
 */
ristretto$(gf_bits)_bool_t RISTRETTO_API_VIS ristretto$(gf_bits)_point_valid (
    const ristretto$(gf_bits)_point_t to_test
) RISTRETTO_WARN_UNUSED RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Torque a point, for debugging purposes.  The output
 * will be equal to the input.
 *
 * @param [out] q The point to torque.
 * @param [in] p The point to torque.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_debugging_torque (
    ristretto$(gf_bits)_point_t q,
    const ristretto$(gf_bits)_point_t p
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
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_debugging_pscale (
    ristretto$(gf_bits)_point_t q,
    const ristretto$(gf_bits)_point_t p,
    const unsigned char factor[RISTRETTO$(gf_bits)_SER_BYTES]
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Almost-Elligator-like hash to curve.
 *
 * Call this function with the output of a hash to make a hash to the curve.
 *
 * This function runs Elligator2 on the ristretto$(gf_bits) Jacobi quartic model.  It then
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
void RISTRETTO_API_VIS
ristretto$(gf_bits)_point_from_hash_nonuniform (
    ristretto$(gf_bits)_point_t pt,
    const unsigned char hashed_data[RISTRETTO$(gf_bits)_HASH_BYTES]
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Indifferentiable hash function encoding to curve.
 *
 * Equivalent to calling ristretto$(gf_bits)_point_from_hash_nonuniform twice and adding.
 *
 * @param [in] hashed_data Output of some hash function.
 * @param [out] pt The data hashed to the curve.
 */ 
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_from_hash_uniform (
    ristretto$(gf_bits)_point_t pt,
    const unsigned char hashed_data[2*RISTRETTO$(gf_bits)_HASH_BYTES]
) RISTRETTO_NONNULL RISTRETTO_NOINLINE;

/**
 * @brief Inverse of elligator-like hash to curve.
 *
 * This function writes to the buffer, to make it so that
 * ristretto$(gf_bits)_point_from_hash_nonuniform(buffer) = pt if
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
ristretto$(gf_bits)_error_t RISTRETTO_API_VIS
ristretto$(gf_bits)_invert_elligator_nonuniform (
    unsigned char recovered_hash[RISTRETTO$(gf_bits)_HASH_BYTES],
    const ristretto$(gf_bits)_point_t pt,
    uint32_t which
) RISTRETTO_NONNULL RISTRETTO_NOINLINE RISTRETTO_WARN_UNUSED;

/**
 * @brief Inverse of elligator-like hash to curve.
 *
 * This function writes to the buffer, to make it so that
 * ristretto$(gf_bits)_point_from_hash_uniform(buffer) = pt if
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
ristretto$(gf_bits)_error_t RISTRETTO_API_VIS
ristretto$(gf_bits)_invert_elligator_uniform (
    unsigned char recovered_hash[2*RISTRETTO$(gf_bits)_HASH_BYTES],
    const ristretto$(gf_bits)_point_t pt,
    uint32_t which
) RISTRETTO_NONNULL RISTRETTO_NOINLINE RISTRETTO_WARN_UNUSED;

/** Securely erase a scalar. */
void RISTRETTO_API_VIS ristretto$(gf_bits)_scalar_destroy (
    ristretto$(gf_bits)_scalar_t scalar
) RISTRETTO_NONNULL;

/** Securely erase a point by overwriting it with zeros.
 * @warning This causes the point object to become invalid.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_point_destroy (
    ristretto$(gf_bits)_point_t point
) RISTRETTO_NONNULL;

/** Securely erase a precomputed table by overwriting it with zeros.
 * @warning This causes the table object to become invalid.
 */
void RISTRETTO_API_VIS ristretto$(gf_bits)_precomputed_destroy (
    ristretto$(gf_bits)_precomputed_s *pre
) RISTRETTO_NONNULL;

#ifdef __cplusplus
} /* extern "C" */
#endif
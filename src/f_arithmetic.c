/**
 * @cond internal
 * @file f_arithmetic.c
 * @copyright
 *   Copyright (c) 2014-2018 Ristretto Developers, Cryptography Research, Inc.  \n
 *   Released under the MIT License.  See LICENSE.txt for license information.
 * @author Mike Hamburg
 * @brief Field arithmetic.
 */

#include <ristretto255.h>
#include "field.h"
#include "constant_time.h"

static const gf_25519_t MODULUS = FIELD_LITERAL(
    0x7ffffffffffed, 0x7ffffffffffff, 0x7ffffffffffff, 0x7ffffffffffff, 0x7ffffffffffff
);

const gf_25519_t SQRT_MINUS_ONE = FIELD_LITERAL(
    0x61b274a0ea0b0, 0x0d5a5fc8f189d, 0x7ef5e9cbd0c60, 0x78595a6804c9e, 0x2b8324804fc1d
);

/* Guarantee: a^2 x = 0 if x = 0; else a^2 x = 1 or SQRT_MINUS_ONE; */
mask_t gf_isr (gf_25519_t *a, const gf_25519_t *x) {
    gf_25519_t L0, L1, L2, L3;

    gf_sqr (&L0, x);
    gf_mul (&L1, &L0, x);
    gf_sqr (&L0, &L1);
    gf_mul (&L1, &L0, x);
    gf_sqrn(&L0, &L1, 3);
    gf_mul (&L2, &L0, &L1);
    gf_sqrn(&L0, &L2, 6);
    gf_mul (&L1, &L2, &L0);
    gf_sqr (&L2, &L1);
    gf_mul (&L0, &L2, x);
    gf_sqrn(&L2, &L0, 12);
    gf_mul (&L0, &L2, &L1);
    gf_sqrn(&L2, &L0, 25);
    gf_mul (&L3, &L2, &L0);
    gf_sqrn(&L2, &L3, 25);
    gf_mul (&L1, &L2, &L0);
    gf_sqrn(&L2, &L1, 50);
    gf_mul (&L0, &L2, &L3);
    gf_sqrn(&L2, &L0, 125);
    gf_mul (&L3, &L2, &L0);
    gf_sqrn(&L2, &L3, 2);
    gf_mul (&L0, &L2, x);

    gf_sqr (&L2, &L0);
    gf_mul (&L3, &L2, x);
    gf_add(&L1,&L3,&ONE);
    mask_t one = gf_eq(&L3,&ONE);
    mask_t succ = one | gf_eq(&L1, &ZERO);
    mask_t qr   = one | gf_eq(&L3, &SQRT_MINUS_ONE);

    constant_time_select(&L2, &SQRT_MINUS_ONE, &ONE, sizeof(L2), qr, 0);
    gf_mul (a,&L2,&L0);
    return succ;
}

/** Serialize to wire format. */
void gf_serialize (uint8_t serial[SER_BYTES], const gf_25519_t *x, int with_hibit) {
    gf_25519_t red;
    gf_copy(&red, x);
    gf_strong_reduce(&red);
    if (!with_hibit) { assert(gf_hibit(&red) == 0); }

    unsigned int j=0, fill=0;
    dword_t buffer = 0;
    UNROLL for (unsigned int i=0; i<SER_BYTES; i++) {
        if (fill < 8 && j < RISTRETTO255_FIELD_LIMBS) {
            buffer |= ((dword_t)red.limb[LIMBPERM(j)]) << fill;
            fill += LIMB_PLACE_VALUE(LIMBPERM(j));
            j++;
        }
        serial[i] = buffer;
        fill -= 8;
        buffer >>= 8;
    }
}

/** Return high bit of x = low bit of 2x mod p */
mask_t gf_hibit(const gf_25519_t *x) {
    gf_25519_t y;
    gf_add(&y,x,x);
    gf_strong_reduce(&y);
    return -(y.limb[0]&1);
}

/** Return high bit of x = low bit of 2x mod p */
mask_t gf_lobit(const gf_25519_t *x) {
    gf_25519_t y;
    gf_copy(&y,x);
    gf_strong_reduce(&y);
    return -(y.limb[0]&1);
}

/** Deserialize from wire format; return -1 on success and 0 on failure. */
mask_t gf_deserialize (gf_25519_t *x, const uint8_t serial[SER_BYTES], int with_hibit, uint8_t hi_nmask) {
    unsigned int j=0, fill=0;
    dword_t buffer = 0;
    dsword_t scarry = 0;
    UNROLL for (unsigned int i=0; i<RISTRETTO255_FIELD_LIMBS; i++) {
        UNROLL while (fill < LIMB_PLACE_VALUE(LIMBPERM(i)) && j < SER_BYTES) {
            uint8_t sj = serial[j];
            if (j==SER_BYTES-1) sj &= ~hi_nmask;
            buffer |= ((dword_t)sj) << fill;
            fill += 8;
            j++;
        }
        x->limb[LIMBPERM(i)] = (i<RISTRETTO255_FIELD_LIMBS-1) ? buffer & LIMB_MASK(LIMBPERM(i)) : buffer;
        fill -= LIMB_PLACE_VALUE(LIMBPERM(i));
        buffer >>= LIMB_PLACE_VALUE(LIMBPERM(i));
        scarry = (scarry + x->limb[LIMBPERM(i)] - MODULUS.limb[LIMBPERM(i)]) >> (8*sizeof(word_t));
    }
    mask_t succ = with_hibit ? -(mask_t)1 : ~gf_hibit(x);
    return succ & word_is_zero(buffer) & ~word_is_zero(scarry);
}

/** Reduce to canonical form. */
void gf_strong_reduce (gf_25519_t *a) {
    /* first, clear high */
    gf_weak_reduce(a); /* Determined to have negligible perf impact. */

    /* now the total is less than 2p */

    /* compute total_value - p.  No need to reduce mod p. */
    dsword_t scarry = 0;
    for (unsigned int i=0; i<RISTRETTO255_FIELD_LIMBS; i++) {
        scarry = scarry + a->limb[LIMBPERM(i)] - MODULUS.limb[LIMBPERM(i)];
        a->limb[LIMBPERM(i)] = scarry & LIMB_MASK(LIMBPERM(i));
        scarry >>= LIMB_PLACE_VALUE(LIMBPERM(i));
    }

    /* uncommon case: it was >= p, so now scarry = 0 and this = x
     * common case: it was < p, so now scarry = -1 and this = x - p + 2^255
     * so let's add back in p.  will carry back off the top for 2^255.
     */
    assert(word_is_zero(scarry) | word_is_zero(scarry+1));

    word_t scarry_0 = scarry;
    dword_t carry = 0;

    /* add it back */
    for (unsigned int i=0; i<RISTRETTO255_FIELD_LIMBS; i++) {
        carry = carry + a->limb[LIMBPERM(i)] + (scarry_0 & MODULUS.limb[LIMBPERM(i)]);
        a->limb[LIMBPERM(i)] = carry & LIMB_MASK(LIMBPERM(i));
        carry >>= LIMB_PLACE_VALUE(LIMBPERM(i));
    }

    assert(word_is_zero(carry + scarry_0));
}

/** Subtract two gf elements d=a-b */
void gf_sub (gf_25519_t *d, const gf_25519_t *a, const gf_25519_t *b) {
    gf_sub_RAW ( d, a, b );
    gf_bias( d, 2 );
    gf_weak_reduce ( d );
}

/** Add two field elements d = a+b */
void gf_add (gf_25519_t *d, const gf_25519_t *a, const gf_25519_t *b) {
    gf_add_RAW ( d, a, b );
    gf_weak_reduce ( d );
}

/** Compare a==b */
mask_t gf_eq(const gf_25519_t *a, const gf_25519_t *b) {
    gf_25519_t c;
    gf_sub(&c,a,b);
    gf_strong_reduce(&c);
    mask_t ret=0;
    for (unsigned int i=0; i<RISTRETTO255_FIELD_LIMBS; i++) {
        ret |= c.limb[LIMBPERM(i)];
    }

    return word_is_zero(ret);
}

/* Copyright (c) 2014-2018 Ristretto Developers, Cryptography Research, Inc.
 * Released under the MIT License.  See LICENSE.txt for license information.
 */

#define GF_HEADROOM 933
#define FIELD_LITERAL(a,b,c,d,e) {{ a,b,c,d,e }}

#define LIMB_PLACE_VALUE(i) 51

void gf_add_RAW (gf_25519_t *out, const gf_25519_t *a, const gf_25519_t *b) {
    for (unsigned int i=0; i<5; i++) {
        out->limb[i] = a->limb[i] + b->limb[i];
    }
}

void gf_sub_RAW (gf_25519_t *out, const gf_25519_t *a, const gf_25519_t *b) {
    for (unsigned int i=0; i<5; i++) {
        out->limb[i] = a->limb[i] - b->limb[i];
    }
}

void gf_bias (gf_25519_t *a, int amt) {
    a->limb[0] += ((uint64_t)(amt)<<52) - 38*amt;
    for (unsigned int i=1; i<5; i++) {
        a->limb[i] += ((uint64_t)(amt)<<52)-2*amt;
    }
}

void gf_weak_reduce (gf_25519_t *a) {
    uint64_t mask = (1ull<<51) - 1;
    uint64_t tmp = a->limb[4] >> 51;
    for (unsigned int i=4; i>0; i--) {
        a->limb[i] = (a->limb[i] & mask) + (a->limb[i-1]>>51);
    }
    a->limb[0] = (a->limb[0] & mask) + tmp*19;
}

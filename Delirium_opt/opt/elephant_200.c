#include "elephant_200.h"

inline static void __attribute__((always_inline))
memset4(uint32_t *arr, const uint32_t value, const uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        arr[i] = value;
    }
}

inline static uint8_t __attribute__((always_inline)) rotl(const uint8_t b) {
    return (b << 1) | (b >> 7);
}

inline static uint32_t __attribute__((always_inline)) rotl4(const uint32_t n) {
    return (n << 4) | (n >> 28);
}

inline static uint8_t __attribute__((always_inline))
constcmp(const uint8_t *a, const uint8_t *b, const uint32_t length) {
    uint8_t r = 0;

    for (uint32_t i = 0; i < length; ++i)
        r |= a[i] ^ b[i];
    return r;
}

// State should be BLOCK_SIZE bytes long
// Note: input may be equal to output
void lfsr_step(uint8_t *output, uint8_t *input) {
    uint8_t temp = rotl(input[0]) ^ rotl(input[2]) ^ (input[13] << 1);
    for (uint32_t i = 0; i < BLOCK_SIZE - 1; ++i)
        output[i] = input[i + 1];
    output[BLOCK_SIZE - 1] = temp;
}

inline static void __attribute__((always_inline))
lfsr_step4(uint32_t *output, uint32_t *input) {
    uint32_t temp = rotl4(input[0]) ^ rotl4(input[2]) ^ (input[13] << 4);
    for (uint32_t i = 0; i < BLOCK_SIZE - 1; ++i)
        output[i] = input[i + 1];
    output[BLOCK_SIZE - 1] = temp;
}

inline static void __attribute__((always_inline))
xor_block(uint8_t *state, const uint8_t *block, const uint32_t size) {
    for (uint32_t i = 0; i < size; ++i)
        state[i] ^= block[i];
}

inline static void __attribute__((always_inline))
xor_block_4(uint32_t *state, const uint32_t *block, const uint32_t size) {

    for (uint32_t i = 0; i < size; ++i) {
        state[i] ^= block[i];
    }
}

inline static void __attribute__((always_inline))
and_block_4(uint32_t *state, const uint32_t *block, const uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        state[i] &= block[i];
    }
}

inline static void __attribute__((always_inline))
interleave_blocks(const uint8_t *in, const SIZE inlen, uint32_t *in4,
                  const uint32_t nblocks_in) {
    uint32_t offset = 0;
    uint32_t i = 0;
    for (i = 0; i + 4 < nblocks_in; i += 4) {
        interleave_4_blocks(in4 + offset, in + i * BLOCK_SIZE,
                            in + i * BLOCK_SIZE + BLOCK_SIZE,
                            in + i * BLOCK_SIZE + 2 * BLOCK_SIZE,
                            in + i * BLOCK_SIZE + 3 * BLOCK_SIZE, BLOCK_SIZE);
        offset += BLOCK_SIZE;
    }
    if (i < nblocks_in) {

        uint8_t block1[4 * BLOCK_SIZE] = {0};
        uint8_t *block2 = block1 + BLOCK_SIZE;
        uint8_t *block3 = block1 + 2 * BLOCK_SIZE;
        uint8_t *block4 = block1 + 3 * BLOCK_SIZE;
        memcpy(block1, in + i * BLOCK_SIZE, inlen - (i * BLOCK_SIZE));
        interleave_4_blocks(in4 + offset, block1, block2, block3, block4,
                            BLOCK_SIZE);
    }
}

// Write the ith assocated data block to "output".
// The nonce is prepended and padding is added as required.
// adlen is the length of the associated data in bytes
void get_ad_block_4(uint32_t *output, const uint32_t *ad, const uint32_t adlen,
                    const uint32_t i) {

    const uint32_t block_offset = i * BLOCK_SIZE;
    // If adlen is divisible by BLOCK_SIZE, add an additional padding block
    const uint32_t r_adlen = adlen - 4 * block_offset;

    // Fill with associated data if available
    if (4 * BLOCK_SIZE <= r_adlen) { // enough ciphertext
        memcpy(output, ad + block_offset, 4 * BLOCK_SIZE);
    } else { // not enough AD, need to pad

        uint8_t onetwothreefour[4 * BLOCK_SIZE] = {0};
        uint8_t *one = onetwothreefour;
        uint8_t *two = one + BLOCK_SIZE;
        uint8_t *three = two + BLOCK_SIZE;
        uint8_t *four = three + BLOCK_SIZE;

        deinterleave_uint32_to_blocks(one, two, three, four, ad + block_offset,
                                      BLOCK_SIZE);
        memset(onetwothreefour + r_adlen, (uint8_t)0, 4 * BLOCK_SIZE - r_adlen);
        onetwothreefour[r_adlen] = 0x01;

        interleave_4_blocks(output, one, two, three, four, BLOCK_SIZE);
    }
}

void get_c_block_4(uint32_t *output, const uint32_t *c, uint32_t clen,
                   uint32_t i) {
    const uint32_t block_offset = i * BLOCK_SIZE;
    // If clen is divisible by BLOCK_SIZE, add an additional padding block
    const uint32_t r_clen = clen - 4 * block_offset;
    // Fill with ciphertext if available
    if (4 * BLOCK_SIZE <= r_clen) { // enough ciphertext
        memcpy(output, c + block_offset, 4 * BLOCK_SIZE);
    } else { // not enough ciphertext, need to pad
        uint8_t onetwothreefour[4 * BLOCK_SIZE] = {0};
        uint8_t *one = onetwothreefour;
        uint8_t *two = one + BLOCK_SIZE;
        uint8_t *three = two + BLOCK_SIZE;
        uint8_t *four = three + BLOCK_SIZE;

        deinterleave_uint32_to_blocks(one, two, three, four, c + block_offset,
                                      BLOCK_SIZE);

        // print_state8(onetwothreefour);

        memset(onetwothreefour + r_clen, (uint8_t)0, 4 * BLOCK_SIZE - r_clen);
        onetwothreefour[r_clen] = 0x01;

        interleave_4_blocks(output, one, two, three, four, BLOCK_SIZE);
    }
}

inline static void __attribute__((always_inline))
mask_tag4(const uint32_t i, const uint32_t inlen, uint32_t *tag4_buffer) {
    const int r_clen = inlen - i * 4 * BLOCK_SIZE;
    if (r_clen < 3 * BLOCK_SIZE) {
        uint32_t and4_values[BLOCK_SIZE] = {0};
        if (r_clen >= 2 * BLOCK_SIZE)
            memset4(and4_values, (uint32_t)(0xEEEEEEEE), BLOCK_SIZE);
        else if (r_clen >= BLOCK_SIZE)
            memset4(and4_values, (uint32_t)(0xCCCCCCCC), BLOCK_SIZE);
        else
            memset4(and4_values, (uint32_t)(0x88888888), BLOCK_SIZE);

        and_block_4(tag4_buffer, and4_values, BLOCK_SIZE);
    }
}

void crypto_aead_impl_4_blocks(uint8_t *c, uint8_t *tag, const uint8_t *m,
                               SIZE mlen, const uint8_t *ad, SIZE adlen,
                               const uint8_t *npub, const uint8_t *k,
                               int encrypt) {
    // Compute number of blocks
    const uint32_t adnpub_len = adlen + CRYPTO_NPUBBYTES;
    const uint32_t nblocks_c = 1 + mlen / BLOCK_SIZE;
    const uint32_t nblocks_m = (mlen % BLOCK_SIZE) ? nblocks_c : nblocks_c - 1;
    const uint32_t nblocks_ad = 1 + adnpub_len / BLOCK_SIZE;
    const uint32_t nb_it =
        (nblocks_c > nblocks_ad) ? nblocks_c : nblocks_ad + 1;
    const uint32_t nb_it4 = nb_it / 4 + 1;

    // generate 4 npub interleaved
    uint32_t npub4[BLOCK_SIZE];
    uint8_t npub_buffer[BLOCK_SIZE] = {0};
    memcpy(npub_buffer, npub, CRYPTO_NPUBBYTES);

    interleave_4_blocks(npub4, npub_buffer, npub_buffer, npub_buffer,
                        npub_buffer, BLOCK_SIZE);

    // generate buffer for:
    // interleaved cipher text,
    // interleaved message,
    // interleaved ad
    uint32_t c4[mlen / 4 + BLOCK_SIZE];
    uint32_t m4[mlen / 4 + BLOCK_SIZE];
    uint32_t ad4[adnpub_len / 4 + BLOCK_SIZE];
    interleave_blocks(m, mlen, m4, nblocks_m);

    uint8_t ad_with_pub[adnpub_len];
    memcpy(ad_with_pub, npub, CRYPTO_NPUBBYTES);
    memcpy(ad_with_pub + CRYPTO_NPUBBYTES, ad, adlen);

    // interleave_ad_blocks(ad_with_pub, adlen, ad4, nblocks_ad);
    interleave_blocks(ad_with_pub, adnpub_len, ad4, nblocks_ad);

    // Storage for the expanded key L
    uint8_t expanded_key[BLOCK_SIZE] = {0};
    memcpy(expanded_key, k, CRYPTO_KEYBYTES);

    // Since LFSR is (not yet) paralellisable and permutation is only
    // possible on uint32, we must deinterleave the expanded_key 4 again to
    // get original permutated expanded_key
    uint32_t expanded_key4[BLOCK_SIZE] = {0};
    interleave_4_blocks(expanded_key4, expanded_key, expanded_key, expanded_key,
                        expanded_key, BLOCK_SIZE);
    permutation_4(expanded_key4);
    deinterleave_uint32_to_blocks(expanded_key, expanded_key, expanded_key,
                                  expanded_key, expanded_key4, BLOCK_SIZE);

    // Buffers for storing previous, current and next mask
    uint8_t current_mask[BLOCK_SIZE] = {0};
    uint8_t next_mask[BLOCK_SIZE] = {0};
    uint8_t after_next_mask[BLOCK_SIZE] = {0};
    uint8_t latest_mask[BLOCK_SIZE] = {0};
    memcpy(current_mask, expanded_key, BLOCK_SIZE);

    uint32_t previous_mask4[BLOCK_SIZE] = {0};
    uint32_t current_mask4[BLOCK_SIZE] = {0};
    uint32_t next_mask4[BLOCK_SIZE] = {0};

    // Buffer to store current ciphertext block
    uint32_t c4_buffer[BLOCK_SIZE];

    // Tag buffer and initialization of tag to zero
    uint32_t tag4[CRYPTO_ABYTES] = {0};
    uint32_t tag4_buffer[BLOCK_SIZE] = {0};
    memset(tag, 0, CRYPTO_ABYTES);

    lfsr_step(next_mask, current_mask);
    lfsr_step(after_next_mask, next_mask);
    lfsr_step(latest_mask, after_next_mask);
    interleave_4_blocks(previous_mask4, current_mask, next_mask,
                        after_next_mask, latest_mask, BLOCK_SIZE);

    uint32_t offset = 0;
    uint32_t i = 0;
    for (i = 0; i < nb_it4; ++i) {
        // Compute mask for the next message
        lfsr_step4(current_mask4,
                   previous_mask4); // generate next mask for encryption
        lfsr_step4(next_mask4,
                   current_mask4); // generate next next for AD

        if (i * 4 < nblocks_m) {
            // Compute ciphertext block
            memcpy(c4_buffer, npub4, 4 * BLOCK_SIZE);
            xor_block_4(c4_buffer, previous_mask4, BLOCK_SIZE);
            permutation_4(c4_buffer);
            xor_block_4(c4_buffer, previous_mask4, BLOCK_SIZE);
            xor_block_4(c4_buffer, m4 + offset, BLOCK_SIZE);
            memcpy(c4 + offset, c4_buffer, 4 * BLOCK_SIZE);
        }

        if (i * 4 < nblocks_c) {

            // Compute tag for ciphertext block
            get_c_block_4(tag4_buffer, encrypt ? c4 : m4, mlen, i);
            xor_block_4(tag4_buffer, previous_mask4, BLOCK_SIZE);
            xor_block_4(tag4_buffer, current_mask4, BLOCK_SIZE);
            permutation_4(tag4_buffer);
            xor_block_4(tag4_buffer, previous_mask4, BLOCK_SIZE);
            xor_block_4(tag4_buffer, current_mask4, BLOCK_SIZE);

            mask_tag4(i, mlen, tag4_buffer);

            xor_block_4(tag4, tag4_buffer, CRYPTO_ABYTES);
        }

        // If there is any AD left and i > 0, compute tag for AD block
        if (i * 4 < nblocks_ad) {
            get_ad_block_4(tag4_buffer, ad4, adnpub_len, i);
            xor_block_4(tag4_buffer, previous_mask4, BLOCK_SIZE);
            xor_block_4(tag4_buffer, next_mask4, BLOCK_SIZE);
            permutation_4(tag4_buffer);
            xor_block_4(tag4_buffer, previous_mask4, BLOCK_SIZE);
            xor_block_4(tag4_buffer, next_mask4, BLOCK_SIZE);

            mask_tag4(i, adnpub_len, tag4_buffer);

            xor_block_4(tag4, tag4_buffer, CRYPTO_ABYTES);
        }

        // Cyclically shift the mask buffers
        // Value of next_mask will be computed in the next iteration
        // lfsr_step(current_mask, latest_mask);
        lfsr_step4(current_mask4, next_mask4);
        lfsr_step4(previous_mask4, current_mask4);

        offset += BLOCK_SIZE;
    }

    int length_left = mlen;
    offset = 0;
    // Deinterleaves and writes the interleaved ciphertext to
    // non-interleaved ciphertext
    for (i = 0; length_left > 0; i++) {
        uint8_t onetwothreefour[4 * BLOCK_SIZE] = {0};
        uint8_t *one = onetwothreefour;
        uint8_t *two = one + BLOCK_SIZE;
        uint8_t *three = two + BLOCK_SIZE;
        uint8_t *four = three + BLOCK_SIZE;
        deinterleave_uint32_to_blocks(one, two, three, four,
                                      c4 + i * BLOCK_SIZE, BLOCK_SIZE);

        if (length_left > 4 * BLOCK_SIZE) {

            memcpy(c + offset, onetwothreefour, 4 * BLOCK_SIZE);

            offset += 4 * BLOCK_SIZE;
            length_left -= 4 * BLOCK_SIZE;
        } else if (length_left > 0) {

            memcpy(c + offset, onetwothreefour, length_left);
            offset += length_left;
            length_left = 0;
        }
    }

    // Deinterleaves interleaved blocktags and XOR's them to original tag
    uint8_t onetwothreefour[4 * CRYPTO_ABYTES] = {0};
    uint8_t *one = onetwothreefour;
    uint8_t *two = one + CRYPTO_ABYTES;
    uint8_t *three = two + CRYPTO_ABYTES;
    uint8_t *four = three + CRYPTO_ABYTES;

    deinterleave_uint32_to_blocks(one, two, three, four, tag4, CRYPTO_ABYTES);
    xor_block(tag, one, CRYPTO_ABYTES);
    xor_block(tag, two, CRYPTO_ABYTES);
    xor_block(tag, three, CRYPTO_ABYTES);
    xor_block(tag, four, CRYPTO_ABYTES);
}

// Remark: c must be at least mlen + CRYPTO_ABYTES long
int elephant_encrypt(unsigned char *c, unsigned long long *clen,
                     const unsigned char *m, unsigned long long mlen,
                     const unsigned char *ad, unsigned long long adlen,
                     const unsigned char *nsec, const unsigned char *npub,
                     const unsigned char *k) {
    (void)nsec;
    *clen = mlen + CRYPTO_ABYTES;
    uint8_t tag[CRYPTO_ABYTES];
    crypto_aead_impl_4_blocks(c, tag, m, mlen, ad, adlen, npub, k, 1);

    memcpy(c + mlen, tag, CRYPTO_ABYTES);
    return 0;
}

int elephant_decrypt(unsigned char *m, unsigned long long *mlen,
                     unsigned char *nsec, const unsigned char *c,
                     unsigned long long clen, const unsigned char *ad,
                     unsigned long long adlen, const unsigned char *npub,
                     const unsigned char *k) {
    (void)nsec;
    if (clen < CRYPTO_ABYTES) {
        return -1;
    }

    *mlen = clen - CRYPTO_ABYTES;
    uint8_t tag[CRYPTO_ABYTES];

    crypto_aead_impl_4_blocks(m, tag, c, *mlen, ad, adlen, npub, k, 0);

    return (constcmp(c + *mlen, tag, CRYPTO_ABYTES) == 0) ? 0 : -1;
}


/*!
@addtogroup crypto_block_aes_zscrypto_v2 AES ZSCrypto C2
@ingroup crypto_block_aes
@{
*/

#include "riscvcrypto/crypto_block/aes/api_aes.h"
#include "riscvcrypto/share/riscv-crypto-intrinsics.h"

uint8_t AES_MULX[] = { 
  0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 
  0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E,   
  0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 
  0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E,   
  0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E, 
  0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E,   
  0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E, 
  0x70, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E,   
  0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E, 
  0x90, 0x92, 0x94, 0x96, 0x98, 0x9A, 0x9C, 0x9E,   
  0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE, 
  0xB0, 0xB2, 0xB4, 0xB6, 0xB8, 0xBA, 0xBC, 0xBE,   
  0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE, 
  0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,   
  0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 
  0xF0, 0xF2, 0xF4, 0xF6, 0xF8, 0xFA, 0xFC, 0xFE,   
  0x1B, 0x19, 0x1F, 0x1D, 0x13, 0x11, 0x17, 0x15, 
  0x0B, 0x09, 0x0F, 0x0D, 0x03, 0x01, 0x07, 0x05,   
  0x3B, 0x39, 0x3F, 0x3D, 0x33, 0x31, 0x37, 0x35, 
  0x2B, 0x29, 0x2F, 0x2D, 0x23, 0x21, 0x27, 0x25,   
  0x5B, 0x59, 0x5F, 0x5D, 0x53, 0x51, 0x57, 0x55, 
  0x4B, 0x49, 0x4F, 0x4D, 0x43, 0x41, 0x47, 0x45,   
  0x7B, 0x79, 0x7F, 0x7D, 0x73, 0x71, 0x77, 0x75, 
  0x6B, 0x69, 0x6F, 0x6D, 0x63, 0x61, 0x67, 0x65,   
  0x9B, 0x99, 0x9F, 0x9D, 0x93, 0x91, 0x97, 0x95, 
  0x8B, 0x89, 0x8F, 0x8D, 0x83, 0x81, 0x87, 0x85,   
  0xBB, 0xB9, 0xBF, 0xBD, 0xB3, 0xB1, 0xB7, 0xB5, 
  0xAB, 0xA9, 0xAF, 0xAD, 0xA3, 0xA1, 0xA7, 0xA5,   
  0xDB, 0xD9, 0xDF, 0xDD, 0xD3, 0xD1, 0xD7, 0xD5, 
  0xCB, 0xC9, 0xCF, 0xCD, 0xC3, 0xC1, 0xC7, 0xC5,   
  0xFB, 0xF9, 0xFF, 0xFD, 0xF3, 0xF1, 0xF7, 0xF5, 
  0xEB, 0xE9, 0xEF, 0xED, 0xE3, 0xE1, 0xE7, 0xE5 
};

#define AES_DEC_RND_MIX_STEP(a,b,c,d        ) {   \
  uint8_t __a1 = s[ a ], __a2 = AES_MULX[ __a1 ]; \
  uint8_t __b1 = s[ b ], __b2 = AES_MULX[ __b1 ]; \
  uint8_t __c1 = s[ c ], __c2 = AES_MULX[ __c1 ]; \
  uint8_t __d1 = s[ d ], __d2 = AES_MULX[ __d1 ]; \
                                                  \
  uint8_t                __a4 = AES_MULX[ __a2 ]; \
  uint8_t                __b4 = AES_MULX[ __b2 ]; \
  uint8_t                __c4 = AES_MULX[ __c2 ]; \
  uint8_t                __d4 = AES_MULX[ __d2 ]; \
                                                  \
  uint8_t                __a8 = AES_MULX[ __a4 ]; \
  uint8_t                __b8 = AES_MULX[ __b4 ]; \
  uint8_t                __c8 = AES_MULX[ __c4 ]; \
  uint8_t                __d8 = AES_MULX[ __d4 ]; \
                                                  \
  uint8_t __aA = __a2 ^ __a8;                     \
  uint8_t __bA = __b2 ^ __b8;                     \
  uint8_t __cA = __c2 ^ __c8;                     \
  uint8_t __dA = __d2 ^ __d8;                     \
                                                  \
  uint8_t __aC = __a4 ^ __a8;                     \
  uint8_t __bC = __b4 ^ __b8;                     \
  uint8_t __cC = __c4 ^ __c8;                     \
  uint8_t __dC = __d4 ^ __d8;                     \
                                                  \
  s[ a ] = __aC ^ __a2 ^ __bA ^ __b1 ^            \
           __cC ^ __c1 ^ __d8 ^ __d1 ;            \
  s[ b ] = __a8 ^ __a1 ^ __bC ^ __b2 ^            \
           __cA ^ __c1 ^ __dC ^ __d1 ;            \
  s[ c ] = __aC ^ __a1 ^ __b8 ^ __b1 ^            \
           __cC ^ __c2 ^ __dA ^ __d1 ;            \
  s[ d ] = __aA ^ __a1 ^ __bC ^ __b1 ^            \
           __c8 ^ __c1 ^ __dC ^ __d2 ;            \
}

/*!
*/
void    aes_128_dec_key_schedule (
    uint32_t    rk [AES_128_RK_WORDS ],
    uint8_t     ck [AES_128_KEY_BYTES]
){
    aes_128_enc_key_schedule(rk, ck);

    uint8_t * brk = (uint8_t *)rk;

    for(int i = 1; i < AES_128_NR; i ++) {
        
        uint8_t * s = brk + ((4*AES_128_NB)*i);
    
        AES_DEC_RND_MIX_STEP(  0,  1,  2,  3 );
        AES_DEC_RND_MIX_STEP(  4,  5,  6,  7 );
        AES_DEC_RND_MIX_STEP(  8,  9, 10, 11 );
        AES_DEC_RND_MIX_STEP( 12, 13, 14, 15 );

    }
}


/*!
@brief Generic single-block AES encrypt function
@param [out] pt - Output plaintext
@param [in]  ct - Input cipher text
@param [in]  rk - The expanded key schedule
@param [in]  nr - Number of decryption rounds to perform.
*/
void    aes_ecb_decrypt (
    uint8_t     pt [AES_BLOCK_BYTES],
    uint8_t     ct [AES_BLOCK_BYTES],
    uint32_t  * rk,
    int         nr
){
    int round = 0;

    uint32_t *kp = &rk[4*nr];

    uint32_t t4, t5, t6, t7;

    uint32_t t0 = U8_TO_U32LE(ct     ) ^ kp[0]; // First AddRoundKey
    uint32_t t1 = U8_TO_U32LE(ct +  4) ^ kp[1];
    uint32_t t2 = U8_TO_U32LE(ct +  8) ^ kp[2];
    uint32_t t3 = U8_TO_U32LE(ct + 12) ^ kp[3];

    kp -= 4;
    
    for(round = nr - 1; round >= 1; round --) {
        
        t4 = _saes_v2_sub_dec(t0, t3);      // SubBytes & Partial ShiftRows
        t5 = _saes_v2_sub_dec(t1, t0);
        t6 = _saes_v2_sub_dec(t2, t1);
        t7 = _saes_v2_sub_dec(t3, t2);

        t0 = _saes_v2_mix_dec(t4, t6);      // Partial ShiftRows & MixColumns
        t1 = _saes_v2_mix_dec(t5, t7);
        t2 = _saes_v2_mix_dec(t6, t4);
        t3 = _saes_v2_mix_dec(t7, t5);
    
        t0 ^= kp[0];                        // AddRoundKey
        t1 ^= kp[1];
        t2 ^= kp[2];
        t3 ^= kp[3];
    
        kp -= 4;

    }
    

    t4 = _saes_v2_sub_dec(t2, t1);          // SubBytes & Partial ShiftRows
    t5 = _saes_v2_sub_dec(t3, t2);
    t6 = _saes_v2_sub_dec(t0, t3);
    t7 = _saes_v2_sub_dec(t1, t0);

    t0 = (t6 & 0x0000FFFF) | (t4 & 0xFFFF0000); // Finish shift rows
    t1 = (t7 & 0x0000FFFF) | (t5 & 0xFFFF0000);
    t2 = (t4 & 0x0000FFFF) | (t6 & 0xFFFF0000);
    t3 = (t5 & 0x0000FFFF) | (t7 & 0xFFFF0000);

    t0 ^= kp[0];                            // AddRoundKey
    t1 ^= kp[1];
    t2 ^= kp[2];
    t3 ^= kp[3];

    
    U32_TO_U8LE(pt , t0,  0);               // Write ciphertext block
    U32_TO_U8LE(pt , t1,  4);
    U32_TO_U8LE(pt , t2,  8);
    U32_TO_U8LE(pt , t3, 12);
}

//!@}

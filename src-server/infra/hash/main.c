//
// Copyright (c) 2026-Present ScorpionC2 public-person "Lucas de Moraes Claro" and all anonymous contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//
// Some sources and logics was taken from
//  djb2:
//      https://theartincode.stanis.me/008-djb2/
//      https://hackmd.io/@sIQnCbQ0T56A3KLAiNrlhQ/r1GtXjBVP
//      source code for crackme in https://crackmes.one/crackme/6973ebc6d735cd51e7a1aa97
//

#include "src-server/infra/hash/main.h"

#include "src-server/shared/utils/comparision/main.h"
#include "src-server/shared/utils/math/main.h"
#include "src-server/shared/utils/mixers/main.h"
#include <inttypes.h>
#include <iso646.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bytes_t HashDjb2(bytes_t src) {
    ulong_t hash = 5381;

    for (size_t i = 0; i < src.len; i++) {
        hash = ((hash << 5) + hash) + src.b[i];
    }

    bytes_t out = {.len = sizeof(hash), .b = malloc(sizeof(hash))

    };

    memcpy(out.b, &hash, sizeof(hash));
    return out;
};

scorpionSettings settings = {
    .hashSize = godzilla, .seed = 0xF1FB230E, .shiftSeed = 18

};

#define SH seed_helper

uint32_t *getState(int wordLen) {
    // 512 is the higher wordLen value because max hashSize is 2048 and 2048 / 4 = 512
    static const uint32_t stateConst[512] = {
        0x2D88371A, 0x1DE48F52, 0x69A2BA8B, 0x58EFA8C9, 0x2F946493, 0x83A8C45E,
        0x21C3E077, 0xCE622776, 0xE267372A, 0x1458C78D, 0x18D047C5, 0x47B42CD2,
        0x898AE62E, 0xD856F2D2, 0xDBF58168, 0x86B4AC80, 0x8A91140F, 0x7E32EFB9,
        0x290C8EE2, 0x5B7D59CA, 0xA4C9521C, 0xD1D1799B, 0x1A2594D0, 0xA44AB7BB,
        0x1F8ABAB7, 0xF6D897DD, 0x63B8FFCF, 0xDFF7DB67, 0x43816120, 0x22359CB6,
        0x40CCB76B, 0x402D06F7, 0xF73FB1EE, 0xCD4AB1FB, 0x79442CEF, 0xDCD9402D,
        0xF34A51D0, 0x935E442F, 0x83BAC16D, 0xAB93F4B7, 0xE520703B, 0xA2506D04,
        0x409BD7B1, 0xF5A5FF2A, 0x3ABFB3C1, 0x76BBAFB0, 0xA8B6E9C1, 0x4941A117,
        0x682F5ECC, 0x7655D740, 0x8DEA5B8C, 0x23EC58A8, 0xCE4592A8, 0xDF622A5B,
        0x74BB5D87, 0x043999E9, 0x9A1AE52C, 0x4B23D0C1, 0x61D22FB9, 0x4662183C,
        0xE42A3CDE, 0xDE4AB9D2, 0x797F1498, 0x4E0C14D7, 0x233678AB, 0xDD3A9F5F,
        0x5686BBA4, 0xCB1908F9, 0xA338DCC5, 0xE6B804C1, 0xA1FF196F, 0xDB8A5F72,
        0x8192B98F, 0x82E05B1E, 0x4AF3C627, 0xD4AB717D, 0x1C998363, 0x80CD7CBC,
        0xC123B70C, 0x9F5DE870, 0x104817F1, 0xE9546E0B, 0x2AC8B795, 0x6225B559,
        0x458ECB7A, 0xB92EAB3B, 0x3E2D45D7, 0x2BB3F57B, 0xF8AF8D61, 0xE7443A6B,
        0xD7F22703, 0x5E51DFF8, 0x92E1BE28, 0x38258B5F, 0xBED6DD6E, 0x3951EE80,
        0x9A7163BC, 0x1F0E3881, 0xF796E4EC, 0x4272CA85, 0x66F883D0, 0xAF8567C1,
        0x3CA96205, 0x60004773, 0xC6419C2A, 0x473A59E7, 0x3E650052, 0x9907F300,
        0x34DA0264, 0xABA67468, 0x19255983, 0x82A39A70, 0xA86D7C1F, 0x02F6B059,
        0x21D1130A, 0x76391F5D, 0x7AC34687, 0xC39B30A7, 0x8958F277, 0x4E8BC1E0,
        0x710DEFEE, 0x491A05E2, 0x29738959, 0xDBCA7AFD, 0x8A2E6526, 0xE6E76F18,
        0x714ABA23, 0xB05F054A, 0xD548D80F, 0xE2C30BC5, 0x3A7052CA, 0x1B7982C5,
        0x1B2747B8, 0xBDE8EDBE, 0x7D0AFE21, 0x619D1ED4, 0x34DD937C, 0x2C75E03A,
        0xCD3C682A, 0x2F17218A, 0x3A4938AC, 0x5FAC9B3A, 0x18888E8B, 0x30078DA6,
        0xEEB737D2, 0xC155C5C7, 0x673A1EC3, 0x8AC14C2D, 0x1CEC2304, 0x6D9052CF,
        0x73597349, 0xF0493500, 0xBBD6A8B5, 0x81B382E1, 0xFC891A51, 0x57C96225,
        0x4A60603D, 0xE25F44C1, 0x126C3CF5, 0xE9676FF2, 0x507C9C6A, 0xA1A792AB,
        0xD8F6AEA3, 0x2AC1CC63, 0xE72FEEA0, 0xB8F6A039, 0xF72CEF88, 0xFCF0BAA4,
        0x305900FF, 0xFFA79B6A, 0x39DC1DDF, 0xC7181034, 0xB36EC9C3, 0x1F069EC4,
        0x97174977, 0x09D3056A, 0x414B9E8D, 0x8C6FD004, 0xF0408296, 0x43FF86EF,
        0xBB403733, 0xCD6802DE, 0x0049965D, 0x2A6CB700, 0xEA0E8427, 0xF6E03986,
        0x4C083B73, 0xD8A35E2F, 0x7575EF37, 0xD3C78497, 0xC87321CB, 0x991C2826,
        0x65F7F634, 0xCAD090D8, 0x7C44191F, 0xD1E6EC93, 0x2D5F4B91, 0xEB7C565D,
        0x41B990CA, 0x50FADF4A, 0xED046F41, 0x7B53AB28, 0x07DA2744, 0x87A3B8CE,
        0x2F4427C8, 0x9BA0FCA6, 0x72C25AA6, 0x3119E818, 0x36E82073, 0xEB97318A,
        0xBE48E273, 0x8686011B, 0x8CF10619, 0x9ADD6389, 0x8D6BFE28, 0xE995B146,
        0xBEB28411, 0x11704809, 0xCE2BD0A7, 0x2805CE0F, 0x32FB7717, 0xB7ED2153,
        0xC52DECC2, 0x00C9740D, 0xA5275DB6, 0x2B765EE6, 0x7E1C049C, 0xC20162D9,
        0x5E9E8DD0, 0x8FD4B65B, 0x3F1A78E6, 0x217C5D61, 0x9820876D, 0xB0ABDAD5,
        0xBB527E3B, 0x81CB2BDE, 0x48A1954B, 0x13A7DE6F, 0x003C64F6, 0xE29879B5,
        0xBEEC29B2, 0x9333912B, 0xE91C8336, 0x5D390C80, 0x4BC44ACE, 0xF6741598,
        0x06A01399, 0x85B6BB8B, 0x2D0F941C, 0x736EAE10, 0xE1873613, 0x2B7476AE,
        0x03FE3A87, 0x8A8AE461, 0xA24C73B2, 0xB3643AF1, 0x6B8A62B7, 0xA1B081ED,
        0xF708D57F, 0xC0F9249F, 0x146BC234, 0x2FAECF6A, 0x866D9402, 0x4074A895,
        0x67450047, 0x389C36DD, 0x420D88C2, 0x56142D28, 0x284CB007, 0xB9BC631F,
        0x44662415, 0x44FDF9F1, 0x8D48F8E5, 0x0AB6391D, 0xD2260B74, 0x18763E9C,
        0x6B2606D3, 0x866B86A8, 0x0276F43C, 0x8C2E85FA, 0xCB63127B, 0x8B18426C,
        0x777DFC50, 0x76374BAD, 0x806920D3, 0xCF77795D, 0x4DB2DBF1, 0x5F057103,
        0x6DD1AE2A, 0xD445753C, 0x36CBDC7A, 0x171E3076, 0xB0B1D452, 0x9F726DB3,
        0xE808C1CD, 0x2652A0C7, 0x5FF63CE5, 0xAB0D1B2D, 0x422B976E, 0xE95E70C2,
        0x2173AA8A, 0x1CB97D13, 0x0D5AE919, 0xC572A2F5, 0x89F67C5B, 0xC24B288E,
        0x198D7E54, 0x8F6005F3, 0x56A12B03, 0x2441BB4A, 0xBA4D666F, 0xDECD3942,
        0x2F9FEA87, 0x191B84C5, 0x26C6F858, 0x9EE51439, 0x14EE2CB1, 0x87CB667D,
        0xAF41EEC7, 0xC9CF8C10, 0x3AE6241C, 0x1FF6CB4B, 0x85FF38A9, 0x911FFCF5,
        0x2382E96C, 0x1773AE7A, 0x13DE7487, 0x2DBB88BE, 0xB80768E7, 0xFB5BFEC3,
        0xE772A741, 0x42894FDC, 0x170D32C6, 0x8CA3EBF3, 0x241BE666, 0x1378E5A3,
        0x171C7935, 0xD6C28E18, 0x30026E06, 0xCA734D66, 0x2220883F, 0xBB8FB99E,
        0xE886FCB9, 0x5AFE8AF7, 0x6322CEF9, 0x856E8036, 0x437C899F, 0xF79793F5,
        0xA05DED8B, 0x5CDDC81B, 0xB5BAF376, 0xE52BBE0A, 0xAA78155C, 0x5F6587C9,
        0xCF234E23, 0x84A9E5B4, 0x3418A049, 0x712B6623, 0x9508EF6E, 0x1618963D,
        0x1F79F646, 0x1A983B50, 0xB0263F05, 0x7E150824, 0xE9BE6C3D, 0x87D01C07,
        0x1BD4B92B, 0xABD02371, 0x5575212E, 0x3C5551B0, 0x94C455DB, 0x0D9D8C62,
        0x047C31C2, 0x771BF414, 0xD0962E9B, 0xFBEF4672, 0xE10E9749, 0x446AB18E,
        0x7CD919B2, 0xCE33CCA7, 0xA9AD5892, 0xA049C2C9, 0xC524AC21, 0xF03432EB,
        0x1C10D5B0, 0x13DF1A63, 0x3A22644B, 0xFC9810D5, 0x43BDEE9B, 0x043712DA,
        0x241F1109, 0x2D70B60C, 0x1971CA2A, 0x874A3BDD, 0xA39FABCF, 0x469B3ED7,
        0xC4AA294C, 0x69A9DA7F, 0xB2ACBE08, 0x02F1B957, 0x2F7E0C7D, 0xC78BE7E6,
        0x14BF13CC, 0x24CC8BB3, 0x34B12E8B, 0x63721466, 0x80537B38, 0x18783A29,
        0x4D0EBEEF, 0xC2E9FE2B, 0x9EC705BB, 0xE6DE4ECA, 0x023E4C4C, 0x19730AC6,
        0x8A1C86E8, 0x6583638B, 0x087A730C, 0xF597FEE2, 0xD7363638, 0x638CC258,
        0x69A02079, 0xFAD14EDC, 0x06ACFE81, 0x6BDE8C75, 0x369DE77C, 0xC8D95A71,
        0xBBED3237, 0x0703345E, 0xA732314F, 0x50755A05, 0x61C3E34E, 0x2A96C729,
        0x4B130155, 0x6A3A14F0, 0x3A273791, 0xC15AAED1, 0xBFD23B76, 0x72EC819C,
        0x0E8DA2F3, 0xD85F5DFF, 0xDF182F49, 0xEF0AB384, 0x199ACC06, 0x99DD0B60,
        0x10596CD9, 0xBFDB111F, 0x5E1D3576, 0xD9F1F914, 0x89147D4E, 0xD5164D10,
        0x71B3D746, 0x31252EC6, 0xA096BFAB, 0x82568237, 0x33C5E518, 0xDE74EFA2,
        0x9BCE1B7D, 0xF8D068A9, 0x819E031A, 0x952E64D1, 0xA3A50E6C, 0x8D6294D8,
        0x7C80E2BC, 0x3794C4B0, 0x703E6CF2, 0xEA61662E, 0x77DF71AC, 0x7F3B4BE0,
        0x43E1C1D9, 0xCCFC9C82, 0xCB722689, 0x9835F010, 0x4B268087, 0x9C41F414,
        0x5689368D, 0x835A0A1C, 0x8F6DA296, 0x99B48166, 0xB1D95AC2, 0xC30DE9E8,
        0x4896B8E2, 0x4474D661, 0x413F64E8, 0xA138234D, 0x7B486EE9, 0x7811599D,
        0x68EBF26F, 0x66B43D5F, 0x8948693B, 0x64139BC9, 0x872CF994, 0xD907B8B8,
        0x8AE8E447, 0x77C8118B, 0xBADFAAF6, 0xB8B69B5F, 0x4321DFD8, 0x5A096807,
        0xA6AB0F1B, 0x79308011, 0x901FAACE, 0x06A2958F, 0xAB6CD53C, 0xFE8CD5A4,
        0x81560E6A, 0x801F92C9, 0x58716D75, 0xD53A69CB, 0x259E95CA, 0x2993B5B2,
        0xB65ABE05, 0x8593DF20,

    };

    uint32_t *state = malloc(wordLen * sizeof(uint32_t));
    if (state == NULL)
        return NULL;

    memcpy(state, stateConst, wordLen * sizeof(uint32_t));

    return state;
}

void seedState(int wordLen, uint32_t *state, const scorpionSettings *ctx) {
    for (int i = 0; i < wordLen; i++) {
        state[i] = seed_helper(state[i], ctx->seed, ctx->shiftSeed);
    }
}

void absorb(bytes_t src, uint32_t *state, int wordLen) {
    size_t srcLen = src.len;

    for (int i = 0; i < (int)srcLen; i++) {
        uint32_t curByte = src.b[wrap_idx(i, srcLen)] |
                           (src.b[wrap_idx(i + 1, srcLen)] << 8) |
                           (src.b[wrap_idx(i + 2, srcLen)] << 16) |
                           (src.b[wrap_idx(i + 3, srcLen)] << 24);

        uint32_t secByte = src.b[wrap_idx(i + 19, srcLen)] |
                           (src.b[wrap_idx(i + 28, srcLen)] << 8) |
                           (src.b[wrap_idx(i + 36, srcLen)] << 16) |
                           (src.b[wrap_idx(i + 11, srcLen)] << 24);

        uint32_t word = state[i & (wordLen - 1)];

        curByte = fmix32(curByte);
        secByte = fmix32(secByte);

        // Now we iterate inside the current word
        const struct minimix_src indexes = {
            .src = {curByte, secByte, word ^ rotr(curByte, secByte & 0x1F)}};

        minimix32(indexes, &word);

        state[i & (wordLen - 1)] = word;
    }
}

void mixState(int wordLen, int miniWordLen, uint32_t *state) {
    int i = 0;
    for (int r = 0; r < miniWordLen; r++) {
        arraymix32(state, &r, wordLen, &i);
        dependency32(wordLen, state, &i);

        i += (r + 1) / 2;
    }
}

void finalizer(int wordLen, uint32_t *state) {
    uint32_t mask = wordLen - 1;

    // Cross-lane mixing
    for (int i = 0; i < wordLen; i++) {
        uint32_t old = state[i & mask];

        state[i & mask] ^= state[(i + 10) & mask];
        state[i & mask] += state[(i + 29) & mask];
        state[i & mask] ^= rotl(state[(i + 38) & mask], 11);

        state[i & mask] ^= old;
    }

    // Self mixing
    for (int i = 0; i < 4; i++) {
        struct minimix_src mixSrc = {.src = {state[(i + 83) & mask],
                                             state[(i + 17) & mask],
                                             state[(i + 27) & mask]}};

        uint16_t a = state[i] & 0xFFFF;
        uint16_t b = (state[i & mask] >> 16) & 0xFFFF;

        minimix16(mixSrc, &a);
        minimix16(mixSrc, &b);

        state[i] = a | ((uint32_t)b << 16);
    }

    // Lane Shuffle Mixing
    for (int i = 0; i < wordLen; i++) {
        uint32_t old = state[i & mask];

        state[i & mask] *= 0x85F7B117;
        state[i & mask] ^= rotl(state[(i + 50) & mask], 16);
        state[i & mask] *= 0x9FD223F;
        state[i & mask] ^= rotr(state[(i + 100) & mask], 11);
    }
}

bytes_t HashScorpionX(bytes_t src) {
    if (src.b == NULL || src.len <= 0)
        return (bytes_t){NULL, 0};

    int hashLen = coalesce_int(Hash.settings->hashSize, 128);
    int wordLen = hashLen / 4;
    int miniWordLen = wordLen / 8;
    uint32_t seed = coalesce_uint32(Hash.settings->seed, 0xF899D28F);
    uint32_t shiftSeed = coalesce_uint32(Hash.settings->shiftSeed & 0x1F, 16);

    uint32_t *state = getState(wordLen);
    if (state == NULL)
        return (bytes_t){NULL, 0};

    seedState(wordLen, state, Hash.settings);
    absorb(src, state, wordLen);
    mixState(wordLen, miniWordLen, state);

    for (int i = 0; i < 2; i++)
        finalizer(wordLen, state);

    bytes_t out;
    out.len = hashLen;
    out.b = malloc(out.len);

    for (int i = 0; i < wordLen; i++) {
        uint32_t v = fmix32(state[i & (wordLen - 1)]);
        smallmix32(&v);

        out.b[i * 4 + 0] = (uint8_t)(v);
        out.b[i * 4 + 1] = (uint8_t)(v >> 8);
        out.b[i * 4 + 2] = (uint8_t)(v >> 16);
        out.b[i * 4 + 3] = (uint8_t)(v >> 24);
    }

    free(state);
    return out;
};

HashInstance Hash = {
    .djb2 = HashDjb2,
    .ScorpionX = HashScorpionX,
};

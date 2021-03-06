/*  save this to gamma.h
 *  gamma was 2.3
 *  tablesize was 256
 *  pwmres was 512
 *  x was 670.843961454218 */

#ifndef _GAMMA_H_
#define _GAMMA_H_

#include <stdint.h>
#include <avr/pgmspace.h>

const uint16_t gammatbl[256] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
    1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 
    3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 
    7, 7, 8, 8, 8, 9, 9, 10, 10, 11, 
    12, 12, 13, 13, 14, 15, 15, 16, 16, 17, 
    18, 19, 19, 20, 21, 22, 22, 23, 24, 25, 
    26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 
    35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 
    46, 47, 48, 50, 51, 52, 54, 55, 56, 57, 
    59, 60, 62, 63, 64, 66, 67, 69, 70, 72, 
    73, 75, 77, 78, 80, 81, 83, 85, 86, 88, 
    90, 91, 93, 95, 97, 99, 100, 102, 104, 106, 
    108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 
    128, 130, 132, 135, 137, 139, 141, 143, 146, 148, 
    150, 153, 155, 157, 160, 162, 165, 167, 169, 172, 
    174, 177, 179, 182, 185, 187, 190, 193, 195, 198, 
    201, 203, 206, 209, 212, 214, 217, 220, 223, 226, 
    229, 232, 235, 238, 241, 244, 247, 250, 253, 256, 
    259, 262, 266, 269, 272, 275, 278, 282, 285, 288, 
    292, 295, 299, 302, 305, 309, 312, 316, 319, 323, 
    326, 330, 334, 337, 341, 345, 348, 352, 356, 360, 
    363, 367, 371, 375, 379, 383, 387, 391, 395, 399, 
    403, 407, 411, 415, 419, 423, 427, 431, 436, 440, 
    444, 448, 453, 457, 461, 466, 470, 474, 479, 483, 
    488, 492, 497, 501, 506, 511
};

#endif /* _GAMMA_H_ */

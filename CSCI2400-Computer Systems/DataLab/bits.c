/*
 * CS:APP Data Lab
 *
 * Adam Ten Hoeve 106105239
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }


  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function.
     The max operator count is checked by dlc. Note that '=' is not
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
/*
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
  int a = 0x55; // Bit string 01010101
  int b = a << 24; // Shift that to 0x55000000
  int c = a << 16; // Shift to 0x00550000
  int d = a << 8; // Shift to 0x00005500
  int total = a | b | c | d; // | togeteher to make 0x55555555
  return total;
}
/*
 * minusOne - return a value of -1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  int a = 1; // Begin with 1
  int b = ~a + 1; // Take the complement (-2) plus one = -1
  return b;
}
/*
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
  x = x << 31; // Right shift 31 to get LSB on its own
  x = x >> 31; // Left shift 31 to copy the LSB to all the other bits because of arithmetic shifts.
  return x;
}
/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  int sign = x >> 31; // 0xff if negative, 0x00 if positive
  int bias = ((1 << n) + ~0x00) & sign; // (x + 2^n - 1) / 2^n, bias will be = 0 if positive
  int num = x + bias;
  int result = num >> n;
  return result;
}
/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  int helper = 0xff;
  int leftShift = helper << (n << 3); // Leftshift 3 is like *8 to move the bits
  int y = leftShift & x;
  int answer = y >> (n << 3);
  int answer2 = answer & helper;
	return answer2;
}
/*
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
  int sixteen = x | (x >> 16); // variable corresponds to number of bits
  int eight = sixteen | (sixteen >> 8);
  int four = eight | (eight >> 4);
  int two = four | (four >> 2);
  int one = two >> 1;
  int answer = one & 0x1;
  return answer;
}
/*
 * isNegative - return 1 if x < 0, return 0 otherwise
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
    int y = x >> 31; // Left shift all the way right so only MSB matters
    int answer = y & 0x1; // | with 0x1 so 1 if negative and 0 if positive
    return answer;
}
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) { // Going to be one of the questions during the interview
  int add = (0x7f << 24) | (0xff << 16) | (0xff << 8) | 0xc6; // 0x7fffffc6 to overflow anything above 0x39 into negatives
  int greater = x + add;
  int minus = ~0x30 + 1; // ~0x30 to make anything less than 0x30 become negative
  int lesser = x + minus;
  int result = lesser | greater;
  result = result >> 31; // Will only be positive if both conditions were met, so is an ascii digit. Then uses that positive sign.
  return !result; // Would be 0 if within both bounds so return ! to make it one and any that are not positive return 0.
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int fitsBits(int x, int n) {
  int sign = x >> 31;
  int pos = (x & ~sign) | (~x & sign); // always returns positive. See absVal fucntion.

  int shift = (1 << (n + ~0x0)) + ~0x0; // Because MSB is always negative, only care about n-1 bits. Then to get 0xff to compare, need to subtract one from the left shift. 1000 -1 = 0111
  int check = pos & shift;
  return !(pos ^ check); // If final is the same as the positive num, then can fit within that many bits.
}
/*
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  // Overflow happens if x is pos, y is neg, and x - y is neg
  // also happens if x is neg, y is pos, and x - y is pos;
  int neg = (~y + 1);
  int sub = x + neg;
  int subSign = (sub >> 31) & 0x1; // Find all the signs of the variables
  int xSign = (x >> 31) & 0x1;
  int ySign = (y >> 31) & 0x1;
  int sameSign = !(xSign ^ ySign); // 1 if same sign, 0 if opposite
  int resultSign = !(xSign ^ subSign);
  int result = sameSign | resultSign;
  return result;
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) { // If x is nonzero, return y. Else return z.
  int neg = (~x + 1); // Negative of x
  int sign = x >> 31; // sign of x
  int negSign = neg >> 31; // sign of -x
  int isZero = sign | negSign; // or signs together. If 0000, then is 0. If 1111, then is nonzero.
  isZero = isZero & 0x1;
  isZero = isZero << 31;
  isZero = isZero >> 31; // will be all ones if x is nonzero, will be all zeros if x is zero
  return (y & isZero) | (z & ~isZero); // One of the isZeros will all be zeroes depending on what x is.
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    // Performs a binary search on the origional bit string to narrow down what bits there are.
    // Always compares to the left side of the bit string. If the | comparison = 0, then those values don't matter.
    // Left shifts those away and performs the search again on the smaller word.
    // If the | comparison does not = 0 then there are bits there and everything on the right side also has bits,
    // so adds that amount to a sum counter. Returns the final sum at the end.

    int sign, y, total, total2, total3, total4, total5, total6, mask1, mask2, mask3, mask4, mask5, check16, check8, check4, check2, check1, result16, result8, result4, result2, result1;
    int shift16, shift8, shift4, shift2, sum16, sum8, sum4, sum2, sum1;

    sign = x >> 31;
    y = ~x; // complement has same number of 0's as x has 1's
    x = (x & ~sign) | (y & sign); // if x is pos then ~sign is 1111. if x is neg then sign is 0000. Always returns the positive part of complement.

    total = 1; // start at 1 because all have at least one bit.

    mask1 = ((0xff << 8) | 0xff) << 16; // 0xffff0000
    check16 = mask1 & x;
    result16 = !check16; // result16 = 0 if more than 16 bits, 1 if less than 16
    shift16 = result16 << 4; // Shift 16 if less than 16 bits, 0 if more
    sum16 = !result16 << 4; // sum16 = 16 if result = 0, so there are more than 16 bits. 0 Otherwise.
    total2 = total + sum16;
    x = x << shift16; // shifts x left 16 if less than 16 bits or shift left 0 if greater than 16 bits

    mask2 = 0xff << 24; // 0xff000000
    check8 = mask2 & x;
    result8 = !check8; // result8 = 0 if more than 8 or 24 bits, 1 if less than 8 or 24 bits
    shift8 = result8 << 3; // shift 8 if less than 8, 0 if more
    sum8 = !result8 << 3; // sum8 = 8 if result8 = 0, meaning more than 8 more bits needed.
    total3 = total2 + sum8;
    x = x << shift8; // Shift x left 8 if less than 8 bits or shift left 0 if more than 8 more bits.

    mask3 = 0xf0 << 24; // 0xf0000000
    check4 = mask3 & x;
    result4 = !check4; // result4 = 0 if more than 4 more bits, 0 if less
    shift4 = result4 << 2; // shift 4 if less than 4 more, 0 if greater
    sum4 = !result4 << 2; // sum4 = 4 if greater than 4 more bits needed, 0 if less
    total4 = total3 + sum4;
    x = x << shift4;

    mask4 = 0xC0 << 24; // 0xC0000000
    check2 = mask4 & x;
    result2 = !check2; // Return 0 if greater than 2, 1 if less
    shift2 = result2 << 1; // shift left 2 if less than 2 bits, shift 0 if more
    sum2 = !result2 << 1; // sum2 = 2 if greater than 2 more bits, 0 otherwise.
    total5 = total4 + sum2;
    x = x << shift2;

    mask5 = 0x80 << 24; // 0x80000000
    check1 = mask5 & x;
    result1 = !check1; // result1 = 0 if greater than 1 bit, result1 = 1 if less
    sum1 = !result1; // sum1 = 1 if greater bits, 0 if less.
    total6 = total5 + sum1;

    return total6 + !!(~x + 1); // Adds 0 if is zero, 1 if is not zero because all other numbers have extra negative bit
    //return total6 + notzero;
}

/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int isNonZero(int x) {
    int y = (~x + 1); // Negative of x
    int sign = x >> 31; // Get signs of both x and -x
    int negSign = y >> 31;
    int result = sign | negSign; // Compare signs. If signs are different, then one mask will be 0xffffffff and the other will be 0x00000000.
    result = result & 0x1; // Only 0 will have both masks be 0x00000000 because it doesn't change sign.
    return result;
}

/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int sign = x >> 31;
  int y = ~x + 1; // y = -x
  int result = (x & ~sign) | (y & sign);
  // x is neg so ~sign = 0x00, y is pos so sign = 0xff
  // x is pos so ~sign = 0xff, y is neg so sign = 0x00
  return result;
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
  // A bit string with a single 1. Make sure you don't match with a negative number. Can match with 1 b/c 2^0 = 1.
  // x
  // ~x + 1 = -x
  // x & -x = y which is the same as x if x is a power of 2.
  // !(x^y)
  int sign = !(x >> 31); // 0000 if neg, 1111 if positive
  int y = ~x + 0x1; // If x is power of 2, then will be all ones after subtracting ones;
  int same = x & y; // x & -x = x will only be true if x is a power of 2.
  return (!(same ^ x)) & sign & x; // and with sign so 0000 if neg and 0000 if x = 0.
}

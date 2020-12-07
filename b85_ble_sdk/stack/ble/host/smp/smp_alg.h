/********************************************************************************************************
 * @file	smp_alg.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *			The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef SMP_ALG_H_
#define SMP_ALG_H_




/**
 * @brief   	This function is used to generate the confirm values
 * @param[out]  c1: the confirm value, 	little--endian.
 * @param[in]   key: aes key, 			little--endian.
 * @param[in]   r: the plaintext, 		little--endian.
 * @param[in]   pres: packet buffer2, 	little--endian.
 * @param[in]   preq: packet buffer2, 	little--endian.
 * @param[in]   iat: initiate address type
 * @param[in]   ia: initiate address, 	little--endian.
 * @param[in]   rat: response address type
 * @param[in]   ra: response address, 	little--endian.
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void 			blt_smp_alg_c1(unsigned char *c1, unsigned char key[16], unsigned char r[16], unsigned char pres[7], unsigned char preq[7], unsigned char iat, unsigned char ia[6], unsigned char rat, unsigned char ra[6]);

/**
 * @brief   	This function is used to generate the STK during the LE legacy pairing process.
 * @param[out]  *STK - the result of encrypt, little--endian.
 * @param[in]   *key - aes key, little--endian.
 * @param[in]   *r1 - the plaintext1, little--endian.
 * @param[in]   *r2 - the plaintext2, little--endian.
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void			blt_smp_alg_s1(unsigned char *stk, unsigned char key[16], unsigned char r1[16], unsigned char r2[16]);

/**
 * @brief		This function is used to compute confirm value by function f4
 * 				---  Ca: f4(U, V, X, Z) = AES-CMACX (U || V || Z)  ---
 * @param[out]  r: the output of the confirm:128-bits, big--endian.
 * @param[in]   u: 	is the 256-bits, 	big--endian.
 * @param[in]   v: 	is the 256-bits,	big--endian.
 * @param[in]   x:  is the 128-bits, 	big--endian.
 * @param[in]   z:  is the 8-bits
 * @return	none.
 */
void			blt_smp_alg_f4 (unsigned char *r, unsigned char u[32], unsigned char v[32], unsigned char x[16], unsigned char z);

/**
 * @brief	This function is used to generate the numeric comparison values during authentication
 * 			stage 1 of the LE Secure Connections pairing process by function g2
 * @param[in]   u: 	is the 256-bits, 	big--endian.
 * @param[in]   v: 	is the 256-bits, 	big--endian.
 * @param[in]   x:  is the 128-bits, 	big--endian.
 * @param[in]   y:  is the 128-bits, 	big--endian.
 * @return	pincode value: 32-bits.
 */
unsigned int	blt_smp_alg_g2 (unsigned char u[32], unsigned char v[32], unsigned char x[16], unsigned char y[16]);

/**
 * @brief	This function is used to generate derived keying material in order to create the LTK
 * 			and keys for the commitment function f6 by function f5
 * @param[out]  mac: the output of the MAC value:128-bits, big--endian.
 * @param[out]  ltk: the output of the LTK value:128-bits, big--endian.
 * @param[in]   w: 	is the 256-bits, 	big--endian.
 * @param[in]   n1:	is the 128-bits, 	big--endian.
 * @param[in]   n2:	is the 128-bits, 	big--endian.
 * @param[in]   a1:	is the 56-bits, 	big--endian.
 * @param[in]   a2:	is the 56-bits, 	big--endian.
 * @return	none.
 */
void 			blt_smp_alg_f5 (unsigned char *mac, unsigned char *ltk, unsigned char w[32], unsigned char n1[16], unsigned char n2[16],
								unsigned char a1[7], unsigned char a2[7]);

/**
 * @brief	This function is used to generate check values during authentication stage 2 of the
 * 			LE Secure Connections pairing process by function f6
 * @param[out]  *e: the output of Ea or Eb:128-bits, big--endian.
 * @param[in]   w: 	is the 256-bits, 	big--endian.
 * @param[in]   n1:	is the 128-bits, 	big--endian.
 * @param[in]   n2:	is the 128-bits, 	big--endian.
 * @param[in]   a1:	is the 56-bits, 	big--endian.
 * @param[in]   a2:	is the 56-bits, 	big--endian.
 * @return	none.
 */
void 			blt_smp_alg_f6 (unsigned char *e, unsigned char w[16], unsigned char n1[16], unsigned char n2[16],
								unsigned char r[16], unsigned char iocap[3], unsigned char a1[7], unsigned char a2[7]);

/**
 * @brief	This function is used to convert keys of a given size from one key type to another
 * 			key type with equivalent strength
 * @param[out]  r: the output of h6:128-bits, 	big--endian.
 * @param[in]   w: 	is the 128-bits, 			big--endian.
 * @param[in]   keyid:	is the 32-bits, 		big--endian.
 * @return	none.
 */
void 			blt_smp_alg_h6 (unsigned char *r, unsigned char w[16], unsigned char keyid[4]);


/**
 * @brief	This function is used to convert keys of a given size from one key type to another
 * 			key type with equivalent strength
 * 			--- h7(SALT, W) = AES-CMACsalt(W) ---
 * @param[out]  r: the output of h7:128-bits, 	big--endian.
 * @param[in]   salt: is the 128-bits, 			big--endian.
 * @param[in]   w:	is the 128-bits, 			big--endian.
 * @return	none.
 */
void 			blt_smp_alg_h7 (unsigned char *r, unsigned char salt[16], unsigned char w[16]);


/**
 * @brief	This function is used to generate the Group Session Key (GSK) for encrypting or
 * 			decrypting payloads of an encrypted BIS.
 * 			--- h8(K, S, keyID) = AES-CMACik(keyID) ---
 * @param[out]  r: the output of h8:128-bits, 	big--endian.
 * @param[in]   k: is the 128-bits, 			big--endian.
 * @param[in]   s: is the 128-bits, 			big--endian.
 * @param[in]   keyid: is the 32-bits, 			big--endian.
 * @return	none.
 */
void 			blt_smp_alg_h8 (unsigned char *r, unsigned char k[16], unsigned char s[16], unsigned char keyId[4]);


#endif /* SMP_ALG_H_ */





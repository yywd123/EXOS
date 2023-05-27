/*
 * Copyright (c) 1999, 2000
 * Intel Corporation.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 * 
 *    This product includes software developed by Intel Corporation and
 *    its contributors.
 * 
 * 4. Neither the name of Intel Corporation or its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY INTEL CORPORATION AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL INTEL CORPORATION OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#ifndef _EFICONTEXT_H_
#define _EFICONTEXT_H_


//
//  IA-64 processor exception types
//
#define    EXCPT_ALT_DTLB            4
#define    EXCPT_DNESTED_TLB         5
#define    EXCPT_BREAKPOINT         11
#define    EXCPT_EXTERNAL_INTERRUPT	12
#define    EXCPT_GEN_EXCEPT         24
#define    EXCPT_NAT_CONSUMPTION    26
#define    EXCPT_DEBUG_EXCEPT       29
#define    EXCPT_UNALIGNED_ACCESS   30
#define    EXCPT_FP_FAULT           32
#define    EXCPT_FP_TRAP            33
#define    EXCPT_TAKEN_BRANCH       35
#define    EXCPT_SINGLE_STEP        36

//
//  IA-64 processor context definition - must be 512 byte aligned!!!
//
typedef
struct {
	uint64_t reserved;	// necessary to preserve alignment for the correct bits in UNAT and to insure F2 is 16 byte aligned...
    
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t r4;
    uint64_t r5;
    uint64_t r6;
    uint64_t r7;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t r16;
    uint64_t r17;
    uint64_t r18;
    uint64_t r19;
    uint64_t r20;
    uint64_t r21;
    uint64_t r22;
    uint64_t r23;
    uint64_t r24;
    uint64_t r25;
    uint64_t r26;
    uint64_t r27;
    uint64_t r28;
    uint64_t r29;
    uint64_t r30;
    uint64_t r31;
    
    uint64_t f2[2];
    uint64_t f3[2];
    uint64_t f4[2];
    uint64_t f5[2];
    uint64_t f6[2];
    uint64_t f7[2];
    uint64_t f8[2];
    uint64_t f9[2];
    uint64_t f10[2];
    uint64_t f11[2];
    uint64_t f12[2];
    uint64_t f13[2];
    uint64_t f14[2];
    uint64_t f15[2];
    uint64_t f16[2];
    uint64_t f17[2];
    uint64_t f18[2];
    uint64_t f19[2];
    uint64_t f20[2];
    uint64_t f21[2];
    uint64_t f22[2];
    uint64_t f23[2];
    uint64_t f24[2];
    uint64_t f25[2];
    uint64_t f26[2];
    uint64_t f27[2];
    uint64_t f28[2];
    uint64_t f29[2];
    uint64_t f30[2];
    uint64_t f31[2];
    
    uint64_t pr;
    
    uint64_t b0;
    uint64_t b1;
    uint64_t b2;
    uint64_t b3;
    uint64_t b4;
    uint64_t b5;
    uint64_t b6;
    uint64_t b7;
    
    // application registers
    uint64_t ar_rsc;
    uint64_t ar_bsp;
    uint64_t ar_bspstore;
    uint64_t ar_rnat;

    uint64_t ar_fcr;

    uint64_t ar_eflag;
    uint64_t ar_csd;
    uint64_t ar_ssd;
    uint64_t ar_cflg;
    uint64_t ar_fsr;
    uint64_t ar_fir;
    uint64_t ar_fdr;

    uint64_t ar_ccv;

    uint64_t ar_unat;

    uint64_t ar_fpsr;
    
    uint64_t ar_pfs;
    uint64_t ar_lc;
    uint64_t ar_ec;
    
    // control registers
    uint64_t cr_dcr;
    uint64_t cr_itm;
    uint64_t cr_iva;
    uint64_t cr_pta;
    uint64_t cr_ipsr;
    uint64_t cr_isr;
    uint64_t cr_iip;
    uint64_t cr_ifa;
    uint64_t cr_itir;
    uint64_t cr_iipa;
    uint64_t cr_ifs;
    uint64_t cr_iim;
    uint64_t cr_iha;
    
    // debug registers
    uint64_t dbr0;
    uint64_t dbr1;
    uint64_t dbr2;
    uint64_t dbr3;
    uint64_t dbr4;
    uint64_t dbr5;
    uint64_t dbr6;
    uint64_t dbr7;
    
    uint64_t ibr0;
    uint64_t ibr1;
    uint64_t ibr2;
    uint64_t ibr3;
    uint64_t ibr4;
    uint64_t ibr5;
    uint64_t ibr6;
    uint64_t ibr7;
    
    // virtual registers
    uint64_t int_nat;	// nat bits for R1-R31
    
} SYSTEM_CONTEXT;

#endif /* _EFI_CONTEXT_H_ */

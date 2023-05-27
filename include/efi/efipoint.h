/* Copyright (C) 2014 by John Cronin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _EFI_POINT_H
#define _EFI_POINT_H

#define EFI_SIMPLE_POINTER_PROTOCOL_GUID \
	{ 0x31878c87, 0xb75, 0x11d5, { 0x9a, 0x4f, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }

INTERFACE_DECL(_EFI_SIMPLE_POINTER);

typedef struct {
	int32_t RelativeMovementX;
	int32_t RelativeMovementY;
	int32_t RelativeMovementZ;
	bool LeftButton;
	bool RightButton;
} EFI_SIMPLE_POINTER_STATE;

typedef struct {
	uint64_t ResolutionX;
	uint64_t ResolutionY;
	uint64_t ResolutionZ;
	bool LeftButton;
	bool RightButton;
} EFI_SIMPLE_POINTER_MODE;

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_POINTER_RESET) (
	IN struct _EFI_SIMPLE_POINTER *This,
	IN bool ExtendedVerification
);

typedef
EFI_STATUS
(EFIAPI *EFI_SIMPLE_POINTER_GET_STATE) (
	IN struct _EFI_SIMPLE_POINTER *This,
	IN OUT EFI_SIMPLE_POINTER_STATE *State
);

typedef struct _EFI_SIMPLE_POINTER {
	EFI_SIMPLE_POINTER_RESET Reset;
	EFI_SIMPLE_POINTER_GET_STATE GetState;
	EFI_EVENT WaitForInput;
	EFI_SIMPLE_POINTER_MODE *Mode;
} EFI_SIMPLE_POINTER_PROTOCOL;

#define EFI_ABSOLUTE_POINTER_PROTOCOL_GUID \
	{ 0x8D59D32B, 0xC655, 0x4AE9, { 0x9B, 0x15, 0xF2, 0x59, 0x04, 0x99, 0x2A, 0x43 } }

INTERFACE_DECL(_EFI_ABSOLUTE_POINTER_PROTOCOL);

typedef struct {
	uint64_t AbsoluteMinX;
	uint64_t AbsoluteMinY;
	uint64_t AbsoluteMinZ;
	uint64_t AbsoluteMaxX;
	uint64_t AbsoluteMaxY;
	uint64_t AbsoluteMaxZ;
	uint32_t Attributes;
} EFI_ABSOLUTE_POINTER_MODE;

typedef struct {
	uint64_t CurrentX;
	uint64_t CurrentY;
	uint64_t CurrentZ;
	uint32_t ActiveButtons;
} EFI_ABSOLUTE_POINTER_STATE;

#define EFI_ABSP_SupportsAltActive 0x00000001
#define EFI_ABSP_SupportsPressureAsZ 0x00000002
#define EFI_ABSP_TouchActive 0x00000001
#define EFI_ABS_AltActive 0x00000002

typedef
EFI_STATUS
(EFIAPI *EFI_ABSOLUTE_POINTER_RESET) (
	IN struct _EFI_ABSOLUTE_POINTER_PROTOCOL *This,
	IN bool ExtendedVerification
);

typedef
EFI_STATUS
(EFIAPI *EFI_ABSOLUTE_POINTER_GET_STATE) (
	IN struct _EFI_ABSOLUTE_POINTER_PROTOCOL *This,
	IN OUT EFI_ABSOLUTE_POINTER_STATE *State
);

typedef struct _EFI_ABSOLUTE_POINTER_PROTOCOL {
	EFI_ABSOLUTE_POINTER_RESET Reset;
	EFI_ABSOLUTE_POINTER_GET_STATE GetState;
	EFI_EVENT WaitForInput;
	EFI_ABSOLUTE_POINTER_MODE *Mode;
} EFI_ABSOLUTE_POINTER_PROTOCOL;

#endif

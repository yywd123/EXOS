;Font Library for EXOS

global UNICODE_16X16, UNICODE_16X16_LEN
UNICODE_16X16:
    incbin "src/stdc/sys/display/UNICODE.bin"
UNICODE_16X16_LEN   EQU     $ - UNICODE_16X16
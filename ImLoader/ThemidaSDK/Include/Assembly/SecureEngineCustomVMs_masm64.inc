; ******************************************************************************
; Header: SecureEngineCustomVMs_masm64.inc
; Description: MASM64 macros definitions
;
; Author/s: Oreans Technologies 
; (c) 2015 Oreans Technologies
;
; --- File generated automatically from Oreans VM Generator (16/6/2015) ---
; ******************************************************************************

; ******************************************************************************
;                                 Macros definition
; ******************************************************************************

VM_TIGER_WHITE_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    103
    dd    0
    dd    20204c57h
ENDM


VM_TIGER_WHITE_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    503
    dd    0
    dd    20204c57h
ENDM


VM_TIGER_RED_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    104
    dd    0
    dd    20204c57h
ENDM


VM_TIGER_RED_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    504
    dd    0
    dd    20204c57h
ENDM


VM_TIGER_BLACK_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    105
    dd    0
    dd    20204c57h
ENDM


VM_TIGER_BLACK_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    505
    dd    0
    dd    20204c57h
ENDM


VM_FISH_WHITE_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    107
    dd    0
    dd    20204c57h
ENDM


VM_FISH_WHITE_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    507
    dd    0
    dd    20204c57h
ENDM


VM_FISH_RED_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    109
    dd    0
    dd    20204c57h
ENDM


VM_FISH_RED_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    509
    dd    0
    dd    20204c57h
ENDM


VM_FISH_BLACK_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    111
    dd    0
    dd    20204c57h
ENDM


VM_FISH_BLACK_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    511
    dd    0
    dd    20204c57h
ENDM


VM_PUMA_WHITE_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    113
    dd    0
    dd    20204c57h
ENDM


VM_PUMA_WHITE_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    513
    dd    0
    dd    20204c57h
ENDM


VM_PUMA_RED_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    115
    dd    0
    dd    20204c57h
ENDM


VM_PUMA_RED_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    515
    dd    0
    dd    20204c57h
ENDM


VM_PUMA_BLACK_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    117
    dd    0
    dd    20204c57h
ENDM


VM_PUMA_BLACK_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    517
    dd    0
    dd    20204c57h
ENDM


VM_SHARK_WHITE_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    119
    dd    0
    dd    20204c57h
ENDM


VM_SHARK_WHITE_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    519
    dd    0
    dd    20204c57h
ENDM


VM_SHARK_RED_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    121
    dd    0
    dd    20204c57h
ENDM


VM_SHARK_RED_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    521
    dd    0
    dd    20204c57h
ENDM


 MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    123
    dd    0
    dd    20204c57h
ENDM


 MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    523
    dd    0
    dd    20204c57h
ENDM


VM_DOLPHIN_WHITE_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    135
    dd    0
    dd    20204c57h
ENDM


VM_DOLPHIN_WHITE_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    535
    dd    0
    dd    20204c57h
ENDM


VM_DOLPHIN_RED_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    137
    dd    0
    dd    20204c57h
ENDM


VM_DOLPHIN_RED_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    537
    dd    0
    dd    20204c57h
ENDM


VM_DOLPHIN_BLACK_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    139
    dd    0
    dd    20204c57h
ENDM


VM_DOLPHIN_BLACK_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    539
    dd    0
    dd    20204c57h
ENDM


VM_EAGLE_WHITE_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    147
    dd    0
    dd    20204c57h
ENDM


VM_EAGLE_WHITE_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    547
    dd    0
    dd    20204c57h
ENDM


VM_EAGLE_RED_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    149
    dd    0
    dd    20204c57h
ENDM


VM_EAGLE_RED_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    549
    dd    0
    dd    20204c57h
ENDM


VM_EAGLE_BLACK_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    151
    dd    0
    dd    20204c57h
ENDM


VM_EAGLE_BLACK_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    551
    dd    0
    dd    20204c57h
ENDM


VM_MUTATE_ONLY_START MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    16
    dd    0
    dd    20204c57h
ENDM


VM_MUTATE_ONLY_END MACRO

    db    0ebh, 10h
    dd    20204c57h
    dd    17
    dd    0
    dd    20204c57h
ENDM



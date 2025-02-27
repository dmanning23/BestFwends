SFX_F:
    .byte 0, 4 ; Brake
    .byte 0, 19 ; Walk
    .byte 0, 8 ; Run
    .byte 0, 20 ; Rolling
    .byte 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 11, 11, 11, 11, 11 ; Collect
    .byte 0, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1 ; Bump
    .byte 0, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1 ; Crash
    .byte 0, 31, 31, 31, 31, 31, 31, 29, 27, 25, 23, 21, 19, 17, 15, 13, 11, 9, 7, 5, 3, 1, 1, 1, 1 ; Roll
    .byte 0, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 ; GameOver

; calculate size of SFX_F table and validate size
SFX_Fcount = * -SFX_F
    if SFX_Fcount > 256
        echo "SFX Warning: table SFX_F is too large"
    endif
    
SFX_CV:
    .byte 0,$77 ; Brake
sfxBRAKE = *-SFX_CV-1
    .byte 0,$87 ; Walk
sfxWALK = *-SFX_CV-1
    .byte 0,$87 ; Run
sfxRUN = *-SFX_CV-1
    .byte 0,$82 ; Rolling
sfxROLLING = *-SFX_CV-1
    .byte 0,$45, $46, $47, $48, $49, $4a, $4b, $4c, $4d, $4e, $4f, $4a, $4b, $4c, $4d, $4e ; Collect
sfxCOLLECT = *-SFX_CV-1
    .byte 0,$8c, $8c, $8c, $8c, $8c, $8c, $8c, $8c, $8c, $8c, $8c ; Bump
sfxBUMP = *-SFX_CV-1
    .byte 0,$c9, $c9, $c9, $c9, $cc, $cc, $cc, $cc, $cc, $cc, $cc, $cf, $cf, $cf, $cf ; Crash
sfxCRASH = *-SFX_CV-1
    .byte 0,$32, $33, $34, $35, $36, $37, $38, $39, $3a, $3b, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c, $3c ; Roll
sfxROLL = *-SFX_CV-1
    .byte 0,$7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c, $7c ; GameOver
sfxGAMEOVER = *-SFX_CV-1

; calculate size of SFX_CV table and validate size
SFX_CVcount = *-SFX_CV

 if SFX_CVcount > 256
     echo "SFX Warning: table SFX_CV is too large"
 endif
 if SFX_CVcount != SFX_Fcount
    echo "SFX Warning: table SFX_F is not the same size as table SFX_CV"
 endif


SFX_OFF:
    ldx #0             ; silence sound output
    stx SFX_LEFT
    stx SFX_RIGHT
    stx AUDV0
    stx AUDV1
    stx AUDC0
    stx AUDC1
    rts

SFX_TRIGGER:
    ldx SFX_LEFT       ; test left channel
    lda SFX_CV,x        ; CV value will be 0 if channel is idle
    bne .leftnotfree   ; if not 0 then skip ahead
    sty SFX_LEFT       ; channel is idle, use it
    rts                ; all done
.leftnotfree:
    ldx SFX_RIGHT      ; test right channel
    lda SFX_CV,x        ; CV value will be 0 if channel is idle
    bne .rightnotfree  ; if not 0 then skip ahead
    sty SFX_RIGHT      ; channel is idle, use it
    rts                ; all done
.rightnotfree:
    cpy SFX_LEFT       ; test sfx priority with left channel
    bcc .leftnotlower  ; skip ahead if new sfx has lower priority than active sfx
    sty SFX_LEFT       ; new sfx has higher priority so use left channel
    rts                ; all done
.leftnotlower:
    cpy SFX_RIGHT      ; test sfx with right channel
    bcc .rightnotlower ; skip ahead if new sfx has lower priority than active sfx
    sty SFX_RIGHT      ; new sfx has higher priority so use right channel
.rightnotlower:
    rts

SFX_UPDATE:
    ldx SFX_LEFT       ; get the pointer for the left channel
    lda SFX_F,x         ; get the Frequency value
    sta AUDF0          ; update the Frequency register
    lda SFX_CV,x        ; get the combined Control and Volume value
    sta AUDV0          ; update the Volume register
    lsr                ; prep the Control value,
    lsr                ;   it's stored in the upper nybble
    lsr                ;   but must be in the lower nybble
    lsr                ;   when Control is updated
    sta AUDC0          ; update the Control register
    beq .skipleftdec   ; skip ahead if Control = 0
    dec SFX_LEFT       ; update pointer for left channel
.skipleftdec:
    ldx SFX_RIGHT      ; get the pointer for the right channel
    lda SFX_F,x         ; get the Frequency value
    sta AUDF1          ; update the Frequency register
    lda SFX_CV,x        ; get the combined Control and Volume value
    sta AUDV1          ; update the Volume register
    lsr                ; prep the Control value,
    lsr                ;   it's stored in the upper nybble
    lsr                ;   but must be in the lower nybble
    lsr                ;   when Control is updated
    sta AUDC1          ; update the Control register
    beq .skiprightdec  ; skip ahead if Control = 0
    dec SFX_RIGHT      ; update pointer for right channel
.skiprightdec:
    rts                ; all done

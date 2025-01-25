;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; BANK 1

    org  $2000
    rorg $F000
;----The following code is the same on both banks----
Start
    BANK_PROLOGUE
BankSwitch
    BANK_SWITCH_TRAMPOLINE
;----End of bank-identical code----






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; set the player sprite
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

UpdatePlayerSprite subroutine

    ;Check which state the player is in
    lda PlayerState1,y
    cmp RunningState
    beq UpdateRunningSprite

    cmp WalkingState
    beq UpdateWalkingSprite

    cmp BrakingState
    beq UpdateBrakingSprite

    cmp RollingState
    beq .rollingSprite

    cmp StunnedState
    beq .stunnedSprite

    jmp UpdateStandingSprite

;We hit the page limit for doing beq
.rollingSprite
    jmp UpdateRollingSprite

.stunnedSprite
    jmp UpdateStunnedSprite

UpdateRunningSprite subroutine

    ;are we setting sonic or tails?
    cpy #0
    beq .sonic

    ;Set the first player sprite to the Tails animation
    ldx #0
    lda #<TailsRunning11
    sta spritePtr1,x
    inx
    lda #>TailsRunning11
    sta spritePtr1,x

    ;set the animation variables
    lda TailsRunningSpeed
    sta animationTiming
    lda TailsRunningFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

.sonic

    ;Set the first player sprite to the Sonic animation
    ldx #0
    lda #<SonicRunning11
    sta spritePtr1,x
    inx
    lda #>SonicRunning11
    sta spritePtr1,x

    ;set the animation variables
    lda SonicRunningSpeed
    sta animationTiming
    lda SonicRunningFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

UpdateWalkingSprite subroutine

    cpy #0
    beq .sonic

    ldx #0
    lda #<TailsWalking11
    sta spritePtr1,x
    inx
    lda #>TailsWalking11
    sta spritePtr1,x

    lda TailsWalkingSpeed
    sta animationTiming
    lda TailsWalkingFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

.sonic

    ldx #0
    lda #<SonicWalking11
    sta spritePtr1,x
    inx
    lda #>SonicWalking11
    sta spritePtr1,x

    lda SonicWalkingSpeed
    sta animationTiming
    lda SonicWalkingFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

UpdateBrakingSprite subroutine

    cpy #0
    beq .sonic

    ldx #0
    lda #<TailsBraking11
    sta spritePtr1,x
    inx
    lda #>TailsBraking11
    sta spritePtr1,x

    lda TailsBrakingSpeed
    sta animationTiming
    lda TailsBrakingFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

.sonic

    ldx #0
    lda #<SonicBraking11
    sta spritePtr1,x
    inx
    lda #>SonicBraking11
    sta spritePtr1,x

    lda SonicBrakingSpeed
    sta animationTiming
    lda SonicBrakingFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

UpdateRollingSprite subroutine

    cpy #0
    beq .sonic

    ldx #0
    lda #<TailsBall11
    sta spritePtr1,x
    inx
    lda #>TailsBall11
    sta spritePtr1,x

    lda TailsBallSpeed
    sta animationTiming
    lda TailsBallFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

.sonic

    ldx #0
    lda #<SonicBall11
    sta spritePtr1,x
    inx
    lda #>SonicBall11
    sta spritePtr1,x

    lda SonicBallSpeed
    sta animationTiming
    lda SonicBallFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

UpdateStunnedSprite subroutine

    cpy #0
    beq .sonic

    ldx #0
    lda #<TailsStunned11
    sta spritePtr1,x
    inx
    lda #>TailsStunned11
    sta spritePtr1,x

    lda TailsStunnedSpeed
    sta animationTiming
    lda TailsStunnedFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

.sonic

    ldx #0
    lda #<SonicStunned11
    sta spritePtr1,x
    inx
    lda #>SonicStunned11
    sta spritePtr1,x

    lda SonicStunnedSpeed
    sta animationTiming
    lda SonicStunnedFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

UpdateStandingSprite subroutine

    cpy #0
    beq .sonic

    ldx #0
    lda #<TailsStanding11
    sta spritePtr1,x
    inx
    lda #>TailsStanding11
    sta spritePtr1,x

    lda TailsStandingSpeed
    sta animationTiming
    lda TailsStandingFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

.sonic

    ldx #0
    lda #<SonicStanding11
    sta spritePtr1,x
    inx
    lda #>SonicStanding11
    sta spritePtr1,x

    lda SonicStandingSpeed
    sta animationTiming
    lda SonicStandingFrames
    sta numAnimationFrames

    jmp DoneUpdatingPlayerSprite

DoneUpdatingPlayerSprite

UpdateAnimation subroutine

    ;don't update the animation if the number of frames is less than 1
    lda numAnimationFrames     ; Load number of frames
    cmp #2                     ; Compare with 2
    bcs .continue               ; Branch if numAnimationFrames >= 2
    jmp .doneUpdatingAnimation ; Skip if numAnimationFrames <= 1

.continue:

    ;Update the animation time 

    ;Increment the current animation time
    inc currentAnimationTime

    ;Check if it is time to increment the frame
    lda currentAnimationTime ; Load currentAnimationTime into accumulator
    cmp animationTiming ; Compare with animationTiming
    bcc .doneUpdatingAnimationTime ; Branch if currentAnimationTime < animationTiming

    ; If we get here, currentAnimationTime >= animationTiming
    lda #0                     ; Load 0 into accumulator
    sta currentAnimationTime   ; Reset currentAnimationTime to 0
    inc currentAnimationFrame  ; Increment the animation frame

.doneUpdatingAnimationTime

    ;Update the animation frame

    lda currentAnimationFrame ; Load current frame into accumulator
    cmp numAnimationFrames ; Compare with max frames
    ;beq .doneUpdatingAnimationFrame ; If equal, we're good
    bcc .doneUpdatingAnimationFrame ; Branch if currentAnimationFrame < numAnimationFrames

    ; If we get here, currentAnimationFrame >= numAnimationFrames
    lda #0 ; Load 0 into accumulator
    sta currentAnimationFrame ; Reset frame back to 0

.doneUpdatingAnimationFrame:

    ;Move the sprite pointer to point to the correct animation frame

    lda currentAnimationFrame  ; Check if frame is 0
    beq .doneUpdatingSpritePointer                  ; If 0, no adding needed

    ldx currentAnimationFrame ; Use X as our frameCounter
    
.addLoop:
    clc                       ; Clear carry before addition
    lda spritePtr1           ; Add 52 to pointer
    adc #52
    sta spritePtr1
    lda spritePtr1+1         ; Add carry to high byte
    adc #0
    sta spritePtr1+1
    
    dex                      ; Decrement frameCounter
    bne .addLoop             ; Keep going if we're not done

.doneUpdatingSpritePointer

.doneUpdatingAnimation

    ;set up rest of the player sprites

    ;Set the second player sprite
    ldx #0
    lda spritePtr1,x        ; Load the low byte value
    clc                     ; Clear carry before addition
    adc #13                 ; Add 13 to the value
    sta spritePtr2,x
    inx
    lda spritePtr1,x        ; Load the high byte value
    adc #0                  ; Add carry if it occurred from low byte addition
    sta spritePtr2,x

    ;Set the player color
    ldx #0
    lda spritePtr1,x        ; Load the low byte value
    clc                     ; Clear carry before addition
    adc #26                 ; Point at the color frame
    sta spriteColorPtr1,x
    inx
    lda spritePtr1,x        ; Load the high byte value
    adc #0                  ; Add carry if it occurred from low byte addition
    sta spriteColorPtr1,x

    ;Set the second player color
    ldx #0
    lda spritePtr1,x        ; Load the low byte value
    clc                     ; Clear carry before addition
    adc #39                 ; Point at the color frame
    sta spriteColorPtr2,x
    inx
    lda spritePtr1,x        ; Load the high byte value
    adc #0                  ; Add carry if it occurred from low byte addition
    sta spriteColorPtr2,x







    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Flip the player sprite if necessary
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FlipPlayerSprite subroutine

    ;flip the player if necessary
    lda FaceRight1,y
    bne .FlipPlayerSpriteRight

    ;Face the player to the left
    lda #%00001000
    sta REFP0
    sta REFP1
    jmp .DoneFlippingPlayerSprite

.FlipPlayerSpriteRight
    lda #0
    sta REFP0
    sta REFP1

.DoneFlippingPlayerSprite

    jsr UpdatePosition

    ;TODO: flip back to bank0 underscan




    ; Switch to Bank 2 routine
    BANK_SWITCH 0,FinishUnderscan




    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Update Player Positions
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

UpdatePosition subroutine

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Set the horizontal screen position of the correct player
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    lda XPos1,y
    ldx #0
    jsr SetHorizPos
    lda XPos1,y
    ldx #1
    jsr SetHorizPos
    
    ;Set the ring position
    lda ringX
    ldx #4
    jsr SetHorizPos

    sta WSYNC	; sync w/ scanline
    sta HMOVE	; apply fine offsets

    rts





    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; SetHorizPos routine
    ; A = X coordinate of the object
    ; X = index of the desired object:
    ;  0: player 1
    ;  1: player 2
    ;  2: missile 1
    ;  3: missile 2
    ;  4: ball
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SetHorizPos subroutine
    sta WSYNC ; start a new line
    sec ; set carry flag
.DivideLoop
    sbc #15 ; subtract 15
    bcs .DivideLoop ; branch until negative
    eor #7 ; calculate fine offset
    asl
    asl
    asl
    asl
    sta RESP0,x ; fix coarse position
    sta HMP0,x ; set fine offset
    rts ; return to caller






    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; 192 lines of frame
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LinesOfFrame
    ;Draw the score board for 10 lines
    ; First, we'll draw the scoreboard.
    ; Put the playfield into score mode (bit 2) which gives
    ; two different colors for the left/right side of
    ; the playfield (given by COLUP0 and COLUP1).
    lda #%00100010	; score mode + 2 pixel ball
    sta CTRLPF
    lda #$94
    sta COLUP0	; set color for left
    lda #$2c
    sta COLUP1	; set color for right
    ; Now we draw all four digits.
	ldy #0		; Y will contain the frame Y coordinate
ScanLoop1a
    sta WSYNC
    tya
    lsr		; divide Y by two for double-height lines
    tax		; -> X
    lda FontBuf+0,x
    sta PF1		; set left score bitmap
    SLEEP 28
    lda FontBuf+5,x
    sta PF1		; set right score bitmap
    iny
    cpy #10
    bcc ScanLoop1a

    ; Clear the playfield
    lda #0
    sta WSYNC
    sta PF1
    ; Turn playfield reflection off (and turn score mode off)
    lda #%00100100	; no reflection + ball priority + 2 pixel ball
    sta CTRLPF




    ;Which player is being drawn this frame?
    lda counter
    and #$01
    beq DrawP1

    ;load the Tails coordinate
    lda YPos2
    sta Temp
    jmp DrawPlayer

DrawP1
    ;load the sonic coordinate
    lda YPos1
    sta Temp

DrawPlayer subroutine
    ldx #182	; X = 192 scanlines
.LVScan
    txa		; X -> A
    sec		; set carry for subtract
    sbc Temp	; local coordinate
    cmp #SpriteHeight ; in sprite?
    bcc .InSprite	; yes, skip over next
    lda #0		; not in sprite, load 0
.InSprite
    lsr        ; Shift right (divide by 2)
    ;lsr        ; Shift right again (divide by 4)
    tay		; local coord -> Y

    lda (spritePtr1),y	; lookup color
    sta WSYNC	; sync w/ scanline
    sta GRP0	; store bitmap
    lda (spriteColorPtr1),y ; lookup color
    sta COLUP0	; store color

    lda (spritePtr2),y	; lookup color
    sta GRP1	; store bitmap
    lda (spriteColorPtr2),y ; lookup color
    sta COLUP1	; store color

    ;Skip to the next line and draw the ring
    sta WSYNC

    ;Dont draw the ring if game over
    lda GameTimer
    beq .SkipRing
    
    ;Are we drawing the ring?
    txa
    cmp ringY1
    bcc .drawRingBottom

    jmp .SkipRing
    
.drawRingBottom

    ;draw the ball
    cmp ringY
    bcc .SkipRing
    lda #02
    sta ENABL

    jmp .DoneRing

.SkipRing
    lda #00
    sta ENABL
   
.DoneRing

    dex		; decrement X
    dex
    bne .LVScan	; repeat until 192 lines

    BANK_SWITCH 0,OverScan







    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Graphics Data
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;Tails Sprite Data (Height: 13)

;Standing Animation Data:
TailsStanding11
  .byte %00000000 ; Row 1
  .byte %00010001 ; Row 2
  .byte %00001010 ; Row 3
  .byte %00001010 ; Row 4
  .byte %01111000 ; Row 5
  .byte %01101000 ; Row 6
  .byte %00001110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsStanding12
  .byte %00000000 ; Row 1
  .byte %00001010 ; Row 2
  .byte %00000000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %10000110 ; Row 5
  .byte %10000110 ; Row 6
  .byte %01100000 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsStandingColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsStandingColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsStandingSpeed:
  .byte 30 ; Animation speed

TailsStandingFrames:
  .byte 1 ; Number of frames

;Walking Animation Data:
TailsWalking11
  .byte %00000000 ; Row 1
  .byte %00001000 ; Row 2
  .byte %00000001 ; Row 3
  .byte %00001001 ; Row 4
  .byte %01111000 ; Row 5
  .byte %10011000 ; Row 6
  .byte %11001110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsWalking12
  .byte %00000000 ; Row 1
  .byte %00010000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000110 ; Row 5
  .byte %01100110 ; Row 6
  .byte %00000000 ; Row 7
  .byte %11000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsWalkingColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsWalkingColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsWalking21
  .byte %00000000 ; Row 1
  .byte %00100001 ; Row 2
  .byte %00000010 ; Row 3
  .byte %00010010 ; Row 4
  .byte %01111000 ; Row 5
  .byte %10011000 ; Row 6
  .byte %11001110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsWalking22
  .byte %00000000 ; Row 1
  .byte %00000010 ; Row 2
  .byte %00100000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000110 ; Row 5
  .byte %01100110 ; Row 6
  .byte %00000000 ; Row 7
  .byte %11000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsWalkingColor21
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsWalkingColor22
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsWalking31
  .byte %00000000 ; Row 1
  .byte %00000010 ; Row 2
  .byte %00100100 ; Row 3
  .byte %00001100 ; Row 4
  .byte %11111000 ; Row 5
  .byte %00101000 ; Row 6
  .byte %01101110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00011111 ; Row 11
  .byte %00010010 ; Row 12
  .byte %00010100 ; Row 13

TailsWalking32
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000110 ; Row 5
  .byte %11000110 ; Row 6
  .byte %00000000 ; Row 7
  .byte %01100111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00001101 ; Row 12
  .byte %00000000 ; Row 13

TailsWalkingColor31
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsWalkingColor32
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsWalking41
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00001000 ; Row 3
  .byte %00001100 ; Row 4
  .byte %11111000 ; Row 5
  .byte %00101000 ; Row 6
  .byte %01101110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00011111 ; Row 11
  .byte %00010010 ; Row 12
  .byte %00010100 ; Row 13

TailsWalking42
  .byte %00000000 ; Row 1
  .byte %00001000 ; Row 2
  .byte %00000000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000110 ; Row 5
  .byte %11000110 ; Row 6
  .byte %00000000 ; Row 7
  .byte %01100111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00001101 ; Row 12
  .byte %00000000 ; Row 13

TailsWalkingColor41
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsWalkingColor42
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsWalkingSpeed:
  .byte 4 ; Animation speed

TailsWalkingFrames:
  .byte 4 ; Number of frames

;Ball Animation Data:
TailsBall11
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00011000 ; Row 3
  .byte %00111100 ; Row 4
  .byte %01111110 ; Row 5
  .byte %01001110 ; Row 6
  .byte %01001110 ; Row 7
  .byte %00111100 ; Row 8
  .byte %00011000 ; Row 9
  .byte %00000000 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsBall12
  .byte %00000000 ; Row 1
  .byte %00011000 ; Row 2
  .byte %00100100 ; Row 3
  .byte %01000010 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00110000 ; Row 6
  .byte %00110000 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00000000 ; Row 9
  .byte %00000000 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsBallColor11
  .byte $88 ; Row 1
  .byte $2E ; Row 2
  .byte $2E ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsBallColor12
  .byte $38 ; Row 1
  .byte $2A ; Row 2
  .byte $2A ; Row 3
  .byte $2A ; Row 4
  .byte $2A ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $1E ; Row 8
  .byte $1E ; Row 9
  .byte $1E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsBallSpeed:
  .byte 30 ; Animation speed

TailsBallFrames:
  .byte 1 ; Number of frames

;Braking Animation Data:
TailsBraking11
  .byte %00000000 ; Row 1
  .byte %00001000 ; Row 2
  .byte %00100100 ; Row 3
  .byte %00010100 ; Row 4
  .byte %01110000 ; Row 5
  .byte %11001000 ; Row 6
  .byte %00001110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsBraking12
  .byte %00000000 ; Row 1
  .byte %01100100 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00001100 ; Row 5
  .byte %00100110 ; Row 6
  .byte %11000000 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsBrakingColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsBrakingColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsBraking21
  .byte %00000000 ; Row 1
  .byte %00001000 ; Row 2
  .byte %00100100 ; Row 3
  .byte %00010100 ; Row 4
  .byte %01110000 ; Row 5
  .byte %11001000 ; Row 6
  .byte %00001110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsBraking22
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %11010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00001100 ; Row 5
  .byte %00100110 ; Row 6
  .byte %11000000 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsBrakingColor21
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsBrakingColor22
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsBraking31
  .byte %00000000 ; Row 1
  .byte %00001000 ; Row 2
  .byte %00100100 ; Row 3
  .byte %00010100 ; Row 4
  .byte %01110000 ; Row 5
  .byte %11001000 ; Row 6
  .byte %00001110 ; Row 7
  .byte %00001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsBraking32
  .byte %00000000 ; Row 1
  .byte %00110100 ; Row 2
  .byte %00010000 ; Row 3
  .byte %10000000 ; Row 4
  .byte %00001100 ; Row 5
  .byte %00100110 ; Row 6
  .byte %11000000 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsBrakingColor31
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsBrakingColor32
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsBrakingSpeed:
  .byte 4 ; Animation speed

TailsBrakingFrames:
  .byte 3 ; Number of frames

;Running Animation Data:
TailsRunning11
  .byte %00000000 ; Row 1
  .byte %00100000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %01011000 ; Row 4
  .byte %01110000 ; Row 5
  .byte %10101000 ; Row 6
  .byte %10101110 ; Row 7
  .byte %11001000 ; Row 8
  .byte %01011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00011111 ; Row 11
  .byte %00010010 ; Row 12
  .byte %00010100 ; Row 13

TailsRunning12
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00100000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %10001100 ; Row 5
  .byte %01000110 ; Row 6
  .byte %01000000 ; Row 7
  .byte %00100111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00001101 ; Row 12
  .byte %00000000 ; Row 13

TailsRunningColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsRunningColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsRunning21
  .byte %00000000 ; Row 1
  .byte %00100000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %01011000 ; Row 4
  .byte %11110000 ; Row 5
  .byte %10011000 ; Row 6
  .byte %01101110 ; Row 7
  .byte %11001000 ; Row 8
  .byte %00011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00011111 ; Row 11
  .byte %00010010 ; Row 12
  .byte %00010100 ; Row 13

TailsRunning22
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00100000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00001100 ; Row 5
  .byte %01100110 ; Row 6
  .byte %10000000 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00001101 ; Row 12
  .byte %00000000 ; Row 13

TailsRunningColor21
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsRunningColor22
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsRunning31
  .byte %00000000 ; Row 1
  .byte %00100000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00011000 ; Row 4
  .byte %01010000 ; Row 5
  .byte %11011000 ; Row 6
  .byte %10101110 ; Row 7
  .byte %00101000 ; Row 8
  .byte %01011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00011111 ; Row 11
  .byte %00010010 ; Row 12
  .byte %00010100 ; Row 13

TailsRunning32
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00100000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00101100 ; Row 5
  .byte %00100110 ; Row 6
  .byte %01000000 ; Row 7
  .byte %11000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00001101 ; Row 12
  .byte %00000000 ; Row 13

TailsRunningColor31
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsRunningColor32
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsRunning41
  .byte %00000000 ; Row 1
  .byte %00010000 ; Row 2
  .byte %00001000 ; Row 3
  .byte %01001100 ; Row 4
  .byte %01110000 ; Row 5
  .byte %10111000 ; Row 6
  .byte %11001110 ; Row 7
  .byte %01001000 ; Row 8
  .byte %01011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsRunning42
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %10001100 ; Row 5
  .byte %01000110 ; Row 6
  .byte %00100000 ; Row 7
  .byte %00100111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsRunningColor41
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsRunningColor42
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsRunning51
  .byte %00000000 ; Row 1
  .byte %00010000 ; Row 2
  .byte %00001000 ; Row 3
  .byte %01001100 ; Row 4
  .byte %11010000 ; Row 5
  .byte %10001000 ; Row 6
  .byte %01101110 ; Row 7
  .byte %11001000 ; Row 8
  .byte %01011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsRunning52
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00101100 ; Row 5
  .byte %01100110 ; Row 6
  .byte %10000000 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsRunningColor51
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsRunningColor52
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsRunning61
  .byte %00000000 ; Row 1
  .byte %00010000 ; Row 2
  .byte %00001000 ; Row 3
  .byte %00001100 ; Row 4
  .byte %10110000 ; Row 5
  .byte %10101000 ; Row 6
  .byte %01101110 ; Row 7
  .byte %01001000 ; Row 8
  .byte %01011010 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00010010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00010100 ; Row 13

TailsRunning62
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00010000 ; Row 3
  .byte %01000000 ; Row 4
  .byte %01001100 ; Row 5
  .byte %01000110 ; Row 6
  .byte %10000000 ; Row 7
  .byte %10000111 ; Row 8
  .byte %00000101 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00001101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

TailsRunningColor61
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsRunningColor62
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsRunningSpeed:
  .byte 2 ; Animation speed

TailsRunningFrames:
  .byte 6 ; Number of frames

;Stunned Animation Data:
TailsStunned11
  .byte %00000000 ; Row 1
  .byte %01000001 ; Row 2
  .byte %00100010 ; Row 3
  .byte %00110110 ; Row 4
  .byte %00010000 ; Row 5
  .byte %00010001 ; Row 6
  .byte %01111111 ; Row 7
  .byte %10000000 ; Row 8
  .byte %10001010 ; Row 9
  .byte %00101010 ; Row 10
  .byte %00011100 ; Row 11
  .byte %00101010 ; Row 12
  .byte %00101000 ; Row 13

TailsStunned12
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %01000001 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00001100 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00000000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010100 ; Row 9
  .byte %00010100 ; Row 10
  .byte %00100010 ; Row 11
  .byte %00010100 ; Row 12
  .byte %00000000 ; Row 13

TailsStunnedColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsStunnedColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsStunned21
  .byte %00000000 ; Row 1
  .byte %00100010 ; Row 2
  .byte %00010100 ; Row 3
  .byte %00100010 ; Row 4
  .byte %00110010 ; Row 5
  .byte %10010000 ; Row 6
  .byte %10011111 ; Row 7
  .byte %01100001 ; Row 8
  .byte %00010100 ; Row 9
  .byte %00110100 ; Row 10
  .byte %00011100 ; Row 11
  .byte %00101010 ; Row 12
  .byte %00101000 ; Row 13

TailsStunned22
  .byte %00000000 ; Row 1
  .byte %00010100 ; Row 2
  .byte %00000000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00001100 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00000000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00001010 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00100010 ; Row 11
  .byte %00010100 ; Row 12
  .byte %00000000 ; Row 13

TailsStunnedColor21
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $2E ; Row 4
  .byte $2E ; Row 5
  .byte $2E ; Row 6
  .byte $2E ; Row 7
  .byte $2E ; Row 8
  .byte $2E ; Row 9
  .byte $2E ; Row 10
  .byte $2E ; Row 11
  .byte $2E ; Row 12
  .byte $2E ; Row 13

TailsStunnedColor22
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $0E ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $0E ; Row 8
  .byte $8A ; Row 9
  .byte $0E ; Row 10
  .byte $2A ; Row 11
  .byte $2A ; Row 12
  .byte $2A ; Row 13

TailsStunnedSpeed:
  .byte 9 ; Animation speed

TailsStunnedFrames:
  .byte 2 ; Number of frames

































;Sonic Sprite Data (Height: 13)

;Standing Animation Data:
SonicStanding11
  .byte %00000000 ; Row 1
  .byte %00100010 ; Row 2
  .byte %00000000 ; Row 3
  .byte %00010100 ; Row 4
  .byte %00010100 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00010000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00100100 ; Row 12
  .byte %01111110 ; Row 13

SonicStanding12
  .byte %00000000 ; Row 1
  .byte %00010100 ; Row 2
  .byte %00010100 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00011010 ; Row 12
  .byte %00000000 ; Row 13

SonicStandingColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $88 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicStandingColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicStandingSpeed:
  .byte 30 ; Animation speed

SonicStandingFrames:
  .byte 1 ; Number of frames

;Walking Animation Data:
SonicWalking11
  .byte %00000000 ; Row 1
  .byte %00010000 ; Row 2
  .byte %00100001 ; Row 3
  .byte %00100010 ; Row 4
  .byte %00010110 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00010000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00100100 ; Row 12
  .byte %01111110 ; Row 13

SonicWalking12
  .byte %00000000 ; Row 1
  .byte %00100000 ; Row 2
  .byte %00000010 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00011010 ; Row 12
  .byte %00000000 ; Row 13

SonicWalkingColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicWalkingColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $38 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicWalking21
  .byte %00000000 ; Row 1
  .byte %00100001 ; Row 2
  .byte %00000000 ; Row 3
  .byte %00100010 ; Row 4
  .byte %00100100 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00010000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00100100 ; Row 12
  .byte %01111110 ; Row 13

SonicWalking22
  .byte %00000000 ; Row 1
  .byte %01000010 ; Row 2
  .byte %01000010 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00011010 ; Row 12
  .byte %00000000 ; Row 13

SonicWalkingColor21
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $88 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicWalkingColor22
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicWalking31
  .byte %00000000 ; Row 1
  .byte %00000010 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00010100 ; Row 4
  .byte %00010100 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00010000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00111110 ; Row 12
  .byte %01100100 ; Row 13

SonicWalking32
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00100100 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00011010 ; Row 13

SonicWalkingColor31
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $88 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicWalkingColor32
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $0E ; Row 12
  .byte $86 ; Row 13

SonicWalking41
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00011000 ; Row 4
  .byte %00011000 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00010000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00111110 ; Row 12
  .byte %01100100 ; Row 13

SonicWalking42
  .byte %00000000 ; Row 1
  .byte %00001000 ; Row 2
  .byte %00001000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00011010 ; Row 13

SonicWalkingColor41
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $88 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicWalkingColor42
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $0E ; Row 12
  .byte $86 ; Row 13

SonicWalkingSpeed:
  .byte 3 ; Animation speed

SonicWalkingFrames:
  .byte 4 ; Number of frames

;Running Animation Data:
SonicRunning11
  .byte %00000000 ; Row 1
  .byte %01100100 ; Row 2
  .byte %10001000 ; Row 3
  .byte %01000110 ; Row 4
  .byte %01100011 ; Row 5
  .byte %00010000 ; Row 6
  .byte %10010011 ; Row 7
  .byte %11111101 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00100100 ; Row 12
  .byte %01111110 ; Row 13

SonicRunning12
  .byte %00000000 ; Row 1
  .byte %00000010 ; Row 2
  .byte %00010001 ; Row 3
  .byte %00100000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00011010 ; Row 12
  .byte %00000000 ; Row 13

SonicRunningColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $38 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicRunningColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicRunning21
  .byte %00000000 ; Row 1
  .byte %01000000 ; Row 2
  .byte %10001001 ; Row 3
  .byte %01100110 ; Row 4
  .byte %00110010 ; Row 5
  .byte %00010000 ; Row 6
  .byte %01010010 ; Row 7
  .byte %01111110 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00100100 ; Row 12
  .byte %01111110 ; Row 13

SonicRunning22
  .byte %00000000 ; Row 1
  .byte %00100110 ; Row 2
  .byte %00010000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00011010 ; Row 12
  .byte %00000000 ; Row 13

SonicRunningColor21
  .byte $38 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $38 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicRunningColor22
  .byte $0E ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $88 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicRunning31
  .byte %00000000 ; Row 1
  .byte %00000010 ; Row 2
  .byte %10010001 ; Row 3
  .byte %01100110 ; Row 4
  .byte %00010100 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00110000 ; Row 7
  .byte %00111100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00100100 ; Row 12
  .byte %01111110 ; Row 13

SonicRunning32
  .byte %00000000 ; Row 1
  .byte %01100100 ; Row 2
  .byte %00001000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00011010 ; Row 12
  .byte %00000000 ; Row 13

SonicRunningColor31
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $38 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicRunningColor32
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicRunning41
  .byte %00000000 ; Row 1
  .byte %00100110 ; Row 2
  .byte %00010001 ; Row 3
  .byte %01100010 ; Row 4
  .byte %00001000 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00010000 ; Row 7
  .byte %00011100 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00111110 ; Row 12
  .byte %01100100 ; Row 13

SonicRunning42
  .byte %00000000 ; Row 1
  .byte %01000000 ; Row 2
  .byte %10001000 ; Row 3
  .byte %00000100 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00011010 ; Row 13

SonicRunningColor41
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $38 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicRunningColor42
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicRunning51
  .byte %00000000 ; Row 1
  .byte %01100110 ; Row 2
  .byte %00011001 ; Row 3
  .byte %00100000 ; Row 4
  .byte %00010100 ; Row 5
  .byte %00010000 ; Row 6
  .byte %00110010 ; Row 7
  .byte %00111110 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00111110 ; Row 12
  .byte %01100100 ; Row 13

SonicRunning52
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %10000000 ; Row 3
  .byte %01000110 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00011010 ; Row 13

SonicRunningColor51
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $38 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicRunningColor52
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicRunning61
  .byte %00000000 ; Row 1
  .byte %01100110 ; Row 2
  .byte %10011000 ; Row 3
  .byte %00000100 ; Row 4
  .byte %00100001 ; Row 5
  .byte %00010000 ; Row 6
  .byte %01010011 ; Row 7
  .byte %01111101 ; Row 8
  .byte %00010000 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00111110 ; Row 12
  .byte %01100100 ; Row 13

SonicRunning62
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00000001 ; Row 3
  .byte %01100010 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00001110 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00011010 ; Row 13

SonicRunningColor61
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $38 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicRunningColor62
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicRunningSpeed:
  .byte 1 ; Animation speed

SonicRunningFrames:
  .byte 6 ; Number of frames

;Ball Animation Data:
SonicBall11
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %00011000 ; Row 3
  .byte %00111100 ; Row 4
  .byte %01111110 ; Row 5
  .byte %01001110 ; Row 6
  .byte %01001110 ; Row 7
  .byte %00111100 ; Row 8
  .byte %00011000 ; Row 9
  .byte %00000000 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

SonicBall12
  .byte %00000000 ; Row 1
  .byte %00011000 ; Row 2
  .byte %00100100 ; Row 3
  .byte %01000010 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00110000 ; Row 6
  .byte %00110000 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00000000 ; Row 9
  .byte %00000000 ; Row 10
  .byte %00000000 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00000000 ; Row 13

SonicBallColor11
  .byte $88 ; Row 1
  .byte $88 ; Row 2
  .byte $88 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicBallColor12
  .byte $38 ; Row 1
  .byte $84 ; Row 2
  .byte $84 ; Row 3
  .byte $84 ; Row 4
  .byte $FC ; Row 5
  .byte $0E ; Row 6
  .byte $0E ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $00 ; Row 13

SonicBallSpeed:
  .byte 30 ; Animation speed

SonicBallFrames:
  .byte 1 ; Number of frames

;Braking Animation Data:
SonicBraking11
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00010010 ; Row 3
  .byte %00001010 ; Row 4
  .byte %00000101 ; Row 5
  .byte %00000001 ; Row 6
  .byte %00000001 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00001000 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00111010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00110010 ; Row 13

SonicBraking12
  .byte %00000000 ; Row 1
  .byte %01100010 ; Row 2
  .byte %00001000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00000110 ; Row 6
  .byte %00000110 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00000111 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00001101 ; Row 13

SonicBrakingColor11
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicBrakingColor12
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicBraking21
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00010010 ; Row 3
  .byte %00001010 ; Row 4
  .byte %00000101 ; Row 5
  .byte %00000001 ; Row 6
  .byte %00000001 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00001000 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00111010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00110010 ; Row 13

SonicBraking22
  .byte %00000000 ; Row 1
  .byte %00000010 ; Row 2
  .byte %11001000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00000110 ; Row 6
  .byte %00000110 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00000111 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00001101 ; Row 13

SonicBrakingColor21
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicBrakingColor22
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicBraking31
  .byte %00000000 ; Row 1
  .byte %00000100 ; Row 2
  .byte %00010010 ; Row 3
  .byte %00001010 ; Row 4
  .byte %00000101 ; Row 5
  .byte %00000001 ; Row 6
  .byte %00000001 ; Row 7
  .byte %00000111 ; Row 8
  .byte %00001000 ; Row 9
  .byte %00011010 ; Row 10
  .byte %00111010 ; Row 11
  .byte %00011111 ; Row 12
  .byte %00110010 ; Row 13

SonicBraking32
  .byte %00000000 ; Row 1
  .byte %00011010 ; Row 2
  .byte %00001000 ; Row 3
  .byte %10000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00000110 ; Row 6
  .byte %00000110 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00000111 ; Row 9
  .byte %00000101 ; Row 10
  .byte %00000101 ; Row 11
  .byte %00000000 ; Row 12
  .byte %00001101 ; Row 13

SonicBrakingColor31
  .byte $88 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicBrakingColor32
  .byte $38 ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $0E ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicBrakingSpeed:
  .byte 4 ; Animation speed

SonicBrakingFrames:
  .byte 3 ; Number of frames

;Stunned Animation Data:
SonicStunned11
  .byte %00000000 ; Row 1
  .byte %01000001 ; Row 2
  .byte %00100010 ; Row 3
  .byte %00110110 ; Row 4
  .byte %00010100 ; Row 5
  .byte %00010000 ; Row 6
  .byte %01110001 ; Row 7
  .byte %10011111 ; Row 8
  .byte %10000000 ; Row 9
  .byte %00101010 ; Row 10
  .byte %01101010 ; Row 11
  .byte %00011100 ; Row 12
  .byte %01101010 ; Row 13

SonicStunned12
  .byte %00000000 ; Row 1
  .byte %00000000 ; Row 2
  .byte %01000001 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00011100 ; Row 9
  .byte %00010100 ; Row 10
  .byte %00010100 ; Row 11
  .byte %00100010 ; Row 12
  .byte %00010100 ; Row 13

SonicStunnedColor11
  .byte $38 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicStunnedColor12
  .byte $0E ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $38 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicStunned21
  .byte %00000000 ; Row 1
  .byte %00100010 ; Row 2
  .byte %00010100 ; Row 3
  .byte %00100010 ; Row 4
  .byte %00110110 ; Row 5
  .byte %10010000 ; Row 6
  .byte %10010010 ; Row 7
  .byte %01111101 ; Row 8
  .byte %00000001 ; Row 9
  .byte %00110100 ; Row 10
  .byte %01110100 ; Row 11
  .byte %00011100 ; Row 12
  .byte %01101010 ; Row 13

SonicStunned22
  .byte %00000000 ; Row 1
  .byte %00010100 ; Row 2
  .byte %00000000 ; Row 3
  .byte %00000000 ; Row 4
  .byte %00000000 ; Row 5
  .byte %00001100 ; Row 6
  .byte %00001100 ; Row 7
  .byte %00000000 ; Row 8
  .byte %00011100 ; Row 9
  .byte %00001010 ; Row 10
  .byte %00001010 ; Row 11
  .byte %00100010 ; Row 12
  .byte %00010100 ; Row 13

SonicStunnedColor21
  .byte $38 ; Row 1
  .byte $38 ; Row 2
  .byte $38 ; Row 3
  .byte $88 ; Row 4
  .byte $88 ; Row 5
  .byte $88 ; Row 6
  .byte $88 ; Row 7
  .byte $88 ; Row 8
  .byte $88 ; Row 9
  .byte $88 ; Row 10
  .byte $88 ; Row 11
  .byte $88 ; Row 12
  .byte $88 ; Row 13

SonicStunnedColor22
  .byte $0E ; Row 1
  .byte $0E ; Row 2
  .byte $0E ; Row 3
  .byte $38 ; Row 4
  .byte $FC ; Row 5
  .byte $FC ; Row 6
  .byte $FC ; Row 7
  .byte $FC ; Row 8
  .byte $1C ; Row 9
  .byte $C8 ; Row 10
  .byte $0E ; Row 11
  .byte $86 ; Row 12
  .byte $86 ; Row 13

SonicStunnedSpeed:
  .byte 9 ; Animation speed

SonicStunnedFrames:
  .byte 2 ; Number of frames













    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Bank 1 epilogue
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    org  $2FFA
    rorg $FFFA
    BANK_VECTORS
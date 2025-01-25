;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; BANK 0

    org  $1000
    rorg $F000
;----The following code is the same on both banks----
Start
    BANK_PROLOGUE
BankSwitch
    BANK_SWITCH_TRAMPOLINE
;----End of bank-identical code----

Reset_0
    CLEAN_START




    jsr InitializeVariables

NextFrame subroutine

    lsr SWCHB	; test Game Reset switch
    bcc .reset	; reset?

    jmp .continue ;don't reset

.reset

    jsr Random
    ;jsr RandomizeRingPosition
    jsr StartGame

.continue

    ; 1 + 3 lines of VSYNC
    VERTICAL_SYNC

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; 35 lines of underscan
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    TIMER_SETUP 35
    
    jsr Random
    
    lda #0
    sta RESP0
    sta RESP1
    sta HMP0
    sta HMP1

    ;Set up the scores
    lda Player2Score
    ldx #0
    jsr GetBCDBitmap
    lda Player1Score
    ldx #5
    jsr GetBCDBitmap

    ;Increment the frame counter 
    inc counter

    ;Since we are only drawing one player per frame, which one needs to have x pos set?
    jsr SetCharacter

    ;skip if the game is not running
    ;update the gametimer
    jsr UpdateTimer
    lda GameTimer
    bne DontSkipForGameOver

    jmp SkipToHereIfGameOver

DontSkipForGameOver



SetControllerMask subroutine

    ;Since we are only drawing one player per frame, which one needs to have x pos set?
    jsr SetCharacter

    ;set the controller mask properly depending on which player we are updating
    beq .setP1ControllerMask
    lda #%00001111 ;check p2 joystick
    sta controllerMask
    jmp .doneSettingControllerMask

.setP1ControllerMask
    lda #%11110000 ;this mask is used by the MoveJoystick routine to check p1 joystick
    sta controllerMask

.doneSettingControllerMask

    ;update velocity?
    lda counter
    and %00001110      ;Only run this once every 8 frames
    beq UpdatePlayer
    jmp DoneUpdatePlayer



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Update Player Movement
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

UpdatePlayer

IsRollingOrStunned subroutine

    ;Set the player's braking flag to "not braking"
    lda #0
    sta Player1Braking,y

    ;if the player is rolling, skip to the Rolling section
    lda PlayerState1,y
    cmp RollingState
    beq .rolling

    ;if the player is stunned, skip to the Stunned section
    cmp StunnedState
    beq .stunned

    jmp .dontSkip

.rolling

    ;slow the player down a little bit

    ;slow down the x velocity
    lda XVel1,y ;Load the velocity
    beq .skipXDecel ;If XVel1 = 0, skip deceleration
    sec ;Set carry flag
    sbc #1 ;Subtract 1
    sta XVel1,y ;Store the result back

.skipXDecel

    ;slow down the y velocity
    lda YVel1,y ;Load the velocity
    beq .skipYDecel ;If XVel1 = 0, skip deceleration
    sec ;Set carry flag
    sbc #1 ;Subtract 1
    sta YVel1,y ;Store the result back

.skipYDecel

    jmp DoneUpdatingPlayerState

.stunned

    ;Slow the player down drastically

    ;Check if the player is moving faster than 2
    lda XVel1,y
    cmp #3
    bcc .dontBrakeX

    ;Since the player is moving faster than 2, subtract 2 to slow them down drastically
    lda XVel1,y     ;Load the current velocity
    sec             ;Set carry flag before subtraction
    sbc #2          ;Subtract 2
    sta XVel1,y     ;Store it back to memory
    
    ;done updating the player's horizontal velocity
    jmp .doneCheckX

.dontBrakeX

    ;Since the player is walking (moving less than 2), set their velocity to 0
    lda #0
    sta XVel1,y

.doneCheckX

    ;Check if the player is moving faster than 2
    lda YVel1,y
    cmp #3
    bcc .dontBrakeY

    ;Since the player is moving faster than 2, subtract 2 to slow them down drastically
    lda YVel1,y     ;Load the current velocity
    sec             ;Set carry flag before subtraction
    sbc #2          ;Subtract 2
    sta YVel1,y     ;Store it back to memory
    
    ;done updating the player's vertical velocity
    jmp .doneCheckY

.dontBrakeY

    ;Since the player is walking (moving less than 2), set their velocity to 0
    lda #0
    sta YVel1,y

.doneCheckY

    ;Once the stunned player has been slowed down, we are done updating them
    jmp DoneUpdatingPlayerState

.dontSkip

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; update the horizontal movement of the player
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;controller: ?
    ;movement: ?
    ;velocty: ?

    ;check if the player is holding right
    lda #%10001000	;right?
    and controllerMask
    and SWCHA
    bne CheckHoldingLeft ;if not, go check if holding left

    ;if they are:

    ;set the player to facing right
    lda #1
    sta FaceRight1,y

    jmp CheckMovingLeft ;go check if they are moving left

CheckHoldingLeft

    ;controller: not right
    ;movement: ?
    ;velocty: ?

    ;check if the player is holding left
    lda #%01000100	;left?
    and controllerMask
    and SWCHA
    bne CheckMovingHorizontalAtAll ;if not, check if the player is moving at all

    ;if they are:

    ;set the player to facing left
    lda #0
    sta FaceRight1,y

    ;go to check if they are moving right
    jmp CheckMovingRight

CheckMovingLeft

    ;controller: right
    ;movement: ?
    ;velocty: ?

    ;check if the player is moving left
    lda XDirRight1,y

    ;if they are, check if the player is StandingLeft
    beq StandingLeft

    ;if not, go increment the players velocity
    jmp IncrementPlayerVelocityHorizontal

StandingLeft

    ;controller: right
    ;movement: left
    ;velocty: ?

    ;check if the player is standing still
    lda XVel1,y

    ;the player is facing left and not standing still, they need to brake
    bne BrakePlayerHorizontal

    ;the player is facing left, not moving, and holding right on the joystick
    ;Set them to move right
    lda #1
    sta XDirRight1,y

    ;increment their velocity
    jmp IncrementPlayerVelocityHorizontal

CheckMovingRight

    ;controller: left
    ;movement: ?
    ;velocty: ?

    ;check if the player is moving right
    lda XDirRight1,y
    bne StandingRight ;if they are, check if the player is standing right

    ;if not, go increment the player's velocity
    jmp IncrementPlayerVelocityHorizontal

StandingRight

    ;controller: left
    ;movement: right
    ;velocty: ?

    ;check if the player is standing still
    lda XVel1,y

    ;the player is facing right and not standing still, they need to brake
    bne BrakePlayerHorizontal

    ;the player is facing right, not moving, and holding left on the joystick
    ;Set them to move right
    lda #0
    sta XDirRight1,y

    ;increment their velocity
    jmp IncrementPlayerVelocityHorizontal

CheckMovingHorizontalAtAll

    ;controller: none
    ;movement: ?
    ;velocty: ?

    ;check if the player is moving
    lda XVel1,y
    beq DoneUpdateHorizontalMovement ;if the player is not moving, done updating the player's horizontal velocity

    ;if the player is moving:
    ;face the direction of the player movement
    lda XDirRight1,y
    sta FaceRight1,y
    jmp DeceleratePlayerHorizontal ;go to teh decelerate step

BrakePlayerHorizontal subroutine

    ;is the player running?
    lda XVel1,y
    cmp RunningThreshold
    bcs .IsRunning

    ;if the player is walking, don't set them to braking
    jmp .DoneCheckBraking

    ;if the player is moving fast:
.IsRunning:
    ;Since the player is running, change the players state to Braking
    lda #1
    sta Player1Braking,y

.DoneCheckBraking

    ;Check if the player is moving faster than 2
    lda XVel1,y
    cmp #3
    bcc .BrakePlayerNotRunning

    ;Since the player is moving faster than 2, subtract 2 to slow them down drastically
    lda XVel1,y     ;Load the current velocity
    sec             ;Set carry flag before subtraction
    sbc #2          ;Subtract 2
    sta XVel1,y     ;Store it back to memory
    
    ;done updating the player's horizontal velocity
    jmp DoneUpdateHorizontalMovement

.BrakePlayerNotRunning

    ;Since the player is walking (moving less than 2), set their velocity to 0
    lda #0
    sta XVel1,y

    ;done updating the player's vertical velocity
    jmp DoneUpdateHorizontalMovement

IncrementPlayerVelocityHorizontal subroutine

    lda XVel1,y ;Load the value into accumulator
    clc         ;Clear carry flag
    adc #$01    ;Add 1 to accumulator
    sta XVel1,y ;Store it back to memory
    
    ;Clamp the velocity to the max velocity
    lda XVel1,y ;Load the value to check
    cmp MaxVelocity    ;Compare with max velocity
    bcc .NoClamp ;Branch if value < 8 (Carry Clear means less than)
    lda MaxVelocity    ;If we get here, value was >= max velocity, so load max velocity
    sta XVel1,y ;Store it back

.NoClamp
    ;Continue with rest of program
    jmp DoneUpdateHorizontalMovement

DeceleratePlayerHorizontal subroutine

    lda XVel1,y ;Load the velocity
    beq .SkipDeceleratePlayer ;If XVel1 = 0, skip deceleration
    sec ;Set carry flag
    sbc #1 ;Subtract 1
    sta XVel1,y ;Store the result back

.SkipDeceleratePlayer

DoneUpdateHorizontalMovement

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; update the vertical movement of the player
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;controller: ?
    ;movement: ?
    ;velocty: ?

    ;check if the player is holding down
    lda #%00100010	;down?
    and controllerMask
    bit SWCHA
    bne CheckHoldingUp ;if not, go check if holding up

    ;if they are:

    jmp CheckMovingUp ;go check if they are moving up

CheckHoldingUp

    ;controller: not down
    ;movement: ?
    ;velocty: ?

    ;check if the player is holding up
    lda #%00010001	;up?
    and controllerMask
    and SWCHA
    bne CheckMovingVerticalAtAll ;if not, check if the player is moving at all

    ;if they are:

    ;go to check if they are moving down
    jmp CheckMovingDown

CheckMovingUp

    ;controller: down
    ;movement: ?
    ;velocty: ?

    ;check if the player is moving up
    lda YDirDown1,y

    ;if they are, check if the player is standing still
    beq StandingUp

    ;if not, go increment the players velocity
    jmp IncrementPlayerVelocityVertical

StandingUp

    ;controller: down
    ;movement: up
    ;velocty: ?

    ;check if the player is standing still
    lda YVel1,y

    ;the player is facing up and not standing still, they need to brake
    bne BrakePlayerVertical

    ;the player is facing down, not moving, and holding down on the joystick
    ;Set them to move down
    lda #1
    sta YDirDown1,y

    ;increment their velocity
    jmp IncrementPlayerVelocityVertical

CheckMovingDown

    ;controller: up
    ;movement: ?
    ;velocty: ?

    ;check if the player is moving down
    lda YDirDown1,y
    bne StandingDown ;if they are, check if the player is standing still

    ;if not, go increment the player's velocity
    jmp IncrementPlayerVelocityVertical

StandingDown

    ;controller: up
    ;movement: down
    ;velocty: ?

    ;check if the player is standing still
    lda YVel1,y

    ;the player is facing down and not standing still, they need to brake
    bne BrakePlayerVertical

    ;the player is facing down, not moving, and holding left on the joystick
    ;Set them to move up
    lda #0
    sta YDirDown1,y

    ;increment their velocity
    jmp IncrementPlayerVelocityVertical

CheckMovingVerticalAtAll

    ;check if the player is moving
    lda YVel1,y
    beq DoneUpdateVerticalMovement ;if the player is not moving, done updating the player's vertical velocity

    ;if the player is moving:
    jmp DeceleratePlayerVertical ;go to teh decelerate step

BrakePlayerVertical subroutine

    ;is the player running?
    lda YVel1,y
    cmp RunningThreshold
    bcs .IsRunning

    ;if the player is walking, don't set them to braking
    jmp .DoneCheckBraking

    ;if the player is moving fast:
.IsRunning:
    ;Since the player is running, change the players state to Braking
    lda #1
    sta Player1Braking,y

.DoneCheckBraking

    ;Check if the player is moving faster than 2
    lda YVel1,y
    cmp #3
    bcc .BrakePlayerNotRunning

    ;Since the player is moving faster than 2, subtract 2 to slow them down drastically
    lda YVel1,y     ;Load the current velocity
    sec             ;Set carry flag before subtraction
    sbc #2          ;Subtract 2
    sta YVel1,y     ;Store it back to memory
    
    ;done updating the player's vertical velocity
    jmp DoneUpdateVerticalMovement

.BrakePlayerNotRunning

    ;Since the player is walking (moving less than 2), set their velocity to 0
    lda #0
    sta YVel1,y

    ;done updating the player's vertical velocity
    jmp DoneUpdateVerticalMovement

IncrementPlayerVelocityVertical subroutine

    lda YVel1,y ;Load the value into accumulator
    clc         ;Clear carry flag
    adc #$01    ;Add 1 to accumulator
    sta YVel1,y ;Store it back to memory
    
    ;Clamp the velocity to the max velocity
    lda YVel1,y ;Load the value to check
    cmp MaxVelocity    ;Compare with max velocity
    bcc .NoClamp ;Branch if value < 8 (Carry Clear means less than)
    lda MaxVelocity    ;If we get here, value was >= max velocity, so load max velocity
    sta YVel1,y ;Store it back

.NoClamp
    ;Continue with rest of program
    jmp DoneUpdateVerticalMovement

DeceleratePlayerVertical subroutine

    lda YVel1,y ;Load the velocity
    beq .SkipDeceleratePlayer ;If YVel1 = 0, skip deceleration
    sec ;Set carry flag
    sbc #1 ;Subtract 1
    sta YVel1,y ;Store the result back

.SkipDeceleratePlayer

DoneUpdateVerticalMovement



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Update Player State
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

UpdatePlayerState

CheckIsBraking

    ;check if the player is braking
    lda Player1Braking,y

    ;if not, check if the player is running
    beq CheckXRunning

    ;if the player is braking:
    
    ;set their state to "Braking"
    lda BrakingState
    sta PlayerState1,y

    ;Play the braking sound effect
    ldy #sfxBRAKE     ; select sound effect
    jsr SFX_TRIGGER     ; and trigger it

    ;reset the y register
    jsr SetCharacter
    
    ;finish updating state
    jmp DoneUpdatingPlayerState

CheckXRunning subroutine

    ;check if the player's horizontal velocity is over the threshold for "running"
    lda XVel1,y
    cmp RunningThreshold
    bcs .IsRunning

    ;if not, check if the player is running vertically
    jmp CheckYRunning

    ;if the player is moving fast:
.IsRunning:
    ;set the state to "running"
    lda RunningState
    sta PlayerState1,y

    ;finish updating state
    jmp DoneUpdatingPlayerState

CheckYRunning subroutine

    ;check if the player's vertial velocity is over the threshold for "running"
    lda YVel1,y
    cmp RunningThreshold
    bcs .IsRunning

    ;if not, check if the player is walking horizontally
    jmp CheckXWalking

    ;if the player is moving fast:
.IsRunning:
    ;set the state to "running"
    lda RunningState
    sta PlayerState1,y

    ;finish updating state
    jmp DoneUpdatingPlayerState

CheckXWalking subroutine

    ;check if the player's horizontal velocity not equal to 0
    lda XVel1,y
    bne .IsWalking

    ;if the player's velocity is 0:

    ;check if the player is walking vertically
    jmp CheckYWalking

    ;if the player's velocity is not equal to 0:
.IsWalking:
    ;set the state to "walking"
    lda WalkingState
    sta PlayerState1,y

    ;finish updating state
    jmp DoneUpdatingPlayerState

CheckYWalking subroutine

    ;check if the player's vertical velocity not equal to 0
    lda YVel1,y
    bne .IsWalking

    ;if the player's velocity is 0:

    ;set the player's state to "standing"
    lda StandingState
    sta PlayerState1,y

    ;finish updating state
    jmp DoneUpdatingPlayerState

    ;if the player's velocity is not equal to 0:
.IsWalking:
    ;set the state to "walking"
    lda WalkingState
    sta PlayerState1,y

    ;finish updating state
    jmp DoneUpdatingPlayerState

DoneUpdatingPlayerState














;Skip all the way down here if we aren't updating the player this frame
DoneUpdatePlayer







    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Run the player logic
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RunPlayerLogic subroutine
    lda PlayerState1,y
    cmp RunningState
    beq .checkButton

    ;The player is not running

    ;check if the player is rolling
    cmp RollingState
    beq .isRunningSpeed

    ;The player is not running or rolling

    ;check if the player is stunned
    cmp StunnedState
    beq .decrementStunnedTimer

    ;If it gets here, there is no player logic to process
    jmp .doneRunningPlayerLogic

.checkButton

    ;check if the player is pressing the button

    ;which player are we checking?
    tya
    beq .checkP1Button

    ;check the p2 button
    bit INPT5
    bmi .noButton
    jmp .setRollingState

.checkP1Button

    ;check the p1 button
    bit INPT4
    bmi .noButton
    jmp .setRollingState

.decrementStunnedTimer

    ;Check if the player is still stunned

    ;decrement the stunned timer
    lda Player1StunnedTimer,y
    beq .setStandingState
    sec ;Set carry flag
    sbc #1 ;Subtract 1
    sta Player1StunnedTimer,y ;Store the result back

    ;done updating the player's vertical velocity
    jmp .doneRunningPlayerLogic

.noButton

    ;the player isn't pressing the button
    jmp .doneRunningPlayerLogic

.setRollingState

    ;set the player to "rolling"
    lda RollingState
    sta PlayerState1,y

    ;play the rolling sound
    ldy #sfxROLL     ; select sound effect
    jsr SFX_TRIGGER     ; and trigger it

    ;reset the y register
    jsr SetCharacter

    ;double the player's speed
    lda XVel1,y
    asl
    sta XVel1,y
    lda YVel1,y
    asl
    sta YVel1,y

    jmp .doneRunningPlayerLogic

.isRunningSpeed

    ;check if the player's speed has slowed down to walking speed
    ;check if the player's vertial velocity is over the threshold for "running"
    lda XVel1,y
    cmp RunningThreshold
    bcs .IsRunning

    lda YVel1,y
    cmp RunningThreshold
    bcs .IsRunning

    ;The player has slowed down to walking speed
    lda WalkingState
    sta PlayerState1,y

    jmp .doneRunningPlayerLogic

.IsRunning:

    ;The player is still rolling
    jmp .doneRunningPlayerLogic

.setStandingState

    ;set the player state to "standing"
    lda StandingState
    sta PlayerState1,y

    jmp .doneRunningPlayerLogic

.doneRunningPlayerLogic



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Check if the players are colliding and run the player collision logic
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PlayerCollisionLogic subroutine
    jsr CheckCharCollision
    bne .collision
    jmp .done

.collision

    ;so much collision logic

    ;check if p1 is spinning
    lda PlayerState1
    cmp RollingState
    beq .p1IsSpinning

    ;player 1 is not spinning
    lda PlayerState2
    cmp RollingState
    beq .p2IsSpinning

    ;neither player is spinning, nothing else to check
    jmp .done

.p1IsSpinning

    ;p1 is spinning

    ;Check if p2 is spinning
    lda PlayerState2
    cmp RollingState
    beq .clash

    ;Is player 2 stunned?
    cmp StunnedState
    beq .done

    ; player 2 is getting bumped
    ;Set the state to stunned
    lda StunnedState
    sta PlayerState2

    ;start the stun timer
    lda StunAmountTime
    sta Player2StunnedTimer

    ;Perform the bump logic on their velocity
    jsr BumpP2
    
    ;play the bump sound effect

    ldy #sfxBUMP     ; select sound effect
    jsr SFX_TRIGGER     ; and trigger it

    ;reset the y register
    jsr SetCharacter
    
    jmp .done

.p2IsSpinning

    ;Player 1 not spinning, but player 2 is spinning

    ;Is player 1 stunned?
    lda PlayerState1
    cmp StunnedState
    beq .done

    ; player 1 is getting bumped
    ;Set the state to stunned
    lda StunnedState
    sta PlayerState1

    ;start the stun timer
    lda StunAmountTime
    sta Player1StunnedTimer

    ;Perform the bump logic on their velocity
    jsr BumpP1

    ;play the bump sound effect

    ldy #sfxBUMP     ; select sound effect
    jsr SFX_TRIGGER     ; and trigger it

    ;reset the y register
    jsr SetCharacter

    jmp .done

.clash

    ;both players are spinning, perform the clash maneuver
    jsr CrashPlayers

    ;play the crash sound effect
    ldy #sfxCRASH     ; select sound effect
    jsr SFX_TRIGGER     ; and trigger it

    ;reset the y register
    jsr SetCharacter

.done




    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; check collision with ring and perform ring collection logic
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RingCollectionLogic subroutine

.checkPlayerRingCollision

    lda #%01000000
    bit CXP0FB	; collision between player 0 and ball?
    bne .playerCollision

    jmp .noCollision

.playerCollision

    ;add 1 to the current player's score
    lda Player1Score,y
    clc
    adc #1
    sta Player1Score,y

    ;do the ring collection logic

    ;reset the ring to a new position
    jsr RandomizeRingPosition

    ;play the ring collection sound effect

    ldy #sfxCOLLECT     ; select sound effect
    jsr SFX_TRIGGER     ; and trigger it

    ;reset the y register
    jsr SetCharacter

.noCollision

    ; Clear collision registers for next frame
    sta CXCLR




;skip all the way down here if the game is not playing?


AddVelocityToPosition subroutine
    ;is the player moving right?
    lda XDirRight1,y
    bne .AddHorizontalVelocityToPosition

    ;the player is mvoing left, subtract their position
    lda XPos1,y    ; Load first value into accumulator
    sec                ; Clear carry flag before addition
    sbc XVel1,y   ; Add second value to accumulator
    sta XPos1,y         ; Store the sum in result memory location

    ;if the player position underflowed, set to 156
    cmp #157
    bcc .DoneClampingHorizontalUnderflow
    lda #156
    sta XPos1,y 

.DoneClampingHorizontalUnderflow

    jmp .DoneAddingHorizontalVelocityToPosition

.AddHorizontalVelocityToPosition:

    ;add velocity to position
    lda XPos1,y    ; Load first value into accumulator
    clc                ; Clear carry flag before addition
    adc XVel1,y   ; Add second value to accumulator
    sta XPos1,y         ; Store the sum in result memory location

    ;if the player position overflowed, set to 0
    cmp #157
    bcc .DoneClampingHorizontalOverflow
    lda #0
    sta XPos1,y 

.DoneClampingHorizontalOverflow

.DoneAddingHorizontalVelocityToPosition

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Set the vertical screen position of the correct player
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;is the player moving down?
    lda YDirDown1,y
    beq .AddVerticalVelocityToPosition

    ;the player is moving up, subtract their position
    lda YPos1,y    ; Load first value into accumulator
    sec                ; Clear carry flag before addition
    sbc YVel1,y   ; Add second value to accumulator
    sta YPos1,y         ; Store the sum in result memory location

    ;if the player position underflowed, set to 164
    cmp #173
    bcc .DoneClampingVerticalUnderflow
    lda #172
    sta YPos1,y 

.DoneClampingVerticalUnderflow

    jmp .DoneAddingVerticalVelocityToPosition

.AddVerticalVelocityToPosition:

    ;add velocity to position
    lda YPos1,y    ; Load first value into accumulator
    clc                ; Clear carry flag before addition
    adc YVel1,y   ; Add second value to accumulator
    sta YPos1,y         ; Store the sum in result memory location

    ;if the player position overflowed, set to 0
    cmp #173
    bcc .DoneClampingVerticalOverflow
    lda #0
    sta YPos1,y 

.DoneClampingVerticalOverflow

.DoneAddingVerticalVelocityToPosition

SkipToHereIfGameOver

    BANK_SWITCH 1,UpdatePlayerSprite

FinishUnderscan
    TIMER_WAIT

    ; Switch to Bank 2 routine
    BANK_SWITCH 1,LinesOfFrame

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; 29 lines of overscan
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OverScan
    TIMER_SETUP 29

    jsr SFX_UPDATE

    lda $0 ;dont draw the sprite in the overscan
    sta GRP0
    sta COLUP0
    sta GRP1
    sta COLUP1
    TIMER_WAIT
    ; total = 262 lines, go to next frame
    jmp NextFrame





    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Initialize all the variables to their default values
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
InitializeVariables subroutine
    ; set to quad
    lda #5
    sta NUSIZ0
    sta NUSIZ1

    ;set the background art
    lda #%00000000
    sta PF0
    sta PF1
    sta PF2

    ;set the background color
    lda #$08
    sta COLUBK

    lda #%00010101 ; make missiles 2x-wide and set player sprites to double
    ;lda #%00011111 ; make missiles 2x-wide and set player sprites to quad
     ;lda #$20 ; make missiles 4x-wide
    sta NUSIZ0
    sta NUSIZ1

    ;lda #%00110000 ;set ball 8x-wide
    lda #%00100000 ;set ball 4x-wide
    sta CTRLPF

    ;set the ball color
    lda ringColor
    sta COLUPF

    ;set both players to the center horizontally
    lda #72
    sta YPos1
    sta YPos2

    ;set the player 1 x position, right side of screen
    lda #9
    sta XPos1

    ;set the player 2 x position, left side of screen
    lda #147
    sta XPos2
    
    ;initialize all the velocities
    lda #0
    sta XVel1
    sta XVel2
    sta YVel1
    sta YVel2

    ;initialize which direction the player is moving on each axis
    sta YDirDown1
    sta YDirDown2

    ;Set the states of the players
    sta PlayerState1
    sta PlayerState2
    sta Player1Braking
    sta Player2Braking

    ;set the stunned timers
    sta Player1StunnedTimer
    sta Player2StunnedTimer

    ;make both players face right and move in the correct directions
    lda #1
    sta FaceRight1
    sta XDirRight1
    lda #0
    sta FaceRight2
    sta XDirRight2

    ;reset the counters
    lda #0
    sta counter

    ;reset the pointers
    sta spritePtr1
    sta spritePtr1+1
    sta spritePtr2
    sta spritePtr2+1
    sta spriteColorPtr1
    sta spriteColorPtr1+1
    sta spriteColorPtr2
    sta spriteColorPtr2+1

    ;set all the animation variables
    sta currentAnimationTime
    sta currentAnimationFrame
    lda #30
    sta animationTiming
    lda #1
    sta numAnimationFrames

    ; seed the random number generator
    lda INTIM       ; unknown value
    sta RandomSeed       ; use as seed
    
    lda #0 
    sta Temp

    ;set the initial ring position
    jsr RandomizeRingPosition

    ;set the game to not playing
    lda #0
    sta GameTimer

    sta Player1Score
    sta Player2Score

    rts



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; The player hit the reset switch, start a game
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
StartGame subroutine
    ;initialize all the variables to their defaults
    jsr InitializeVariables

    ;set the background color
    lda #$01
    sta COLUBK

    ;Randomize the ring position
    jsr RandomizeRingPosition

    ;start the timer
    lda #90
    sta GameTimer

    rts




Random subroutine
    lda RandomSeed
    lsr
    bcc .noeor
    eor #$D4 
.noeor 
    sta RandomSeed
    rts



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Generate random X and Y coordinates for the ring
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RandomizeRingPosition:
    jsr Random      ; get a random value between 0-255
    and #158        ; limit range to 0-127
    ;sta Temp        ; save it
    ;jsr Random      ; get a random value between 0-255
    ;and #31         ; limit range to 0-31
    ;clc             ; must clear carry for add
    ;adc Temp        ; add in random # from 0-127 for range of 0-142
    adc #6          ; add 5 for range of 5-147

    ;lda #164
    sta ringX   ; save the random X position
    
    jsr Random      ; get a random value between 0-255
    and #183        ; limit range to 0-127
    ;sta Temp        ; save it
    ;jsr Random      ; get a random value between 0-255
    ;and #56         ; limit range to 0-15
    ;clc             ; must clear carry for add
    ;adc Temp        ; add in random # from 0-127 for range of 0-142
    adc #3         ; add 26 for range of 26-168

    ;lda #64
    sta ringY   ; save the random Y position
    adc #8
    sta ringY1
    
    ;ring range:
    ;horizontal:
    ;6-164
    ;vertical:
    ;3-186

    rts



CheckCharCollision subroutine
    ; Check if |XPos1 - XPos2| < width(12)
    sec
    lda XPos1
    sbc XPos2
    bcs .skip1      ; If positive, skip negation
    eor #$FF        ; Negate
    adc #1
.skip1:
    cmp #12          ; width is immediate value
    bcs .noCollision

    ; Check if |YPos1 - YPos2| < height(24)
    sec
    lda YPos1
    sbc YPos2
    bcs .skip2      ; If positive, skip negation
    eor #$FF        ; Negate
    adc #1
.skip2:
    cmp #24         ; height is immediate value
    bcs .noCollision

    ; If we get here, there is a collision
    lda #1
    rts

.noCollision:
    lda #0
    rts



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Player 1 got bumped by player 2
    ; Knock the first player around a bit
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
BumpP1 subroutine
    lda XDirRight2
    sta YDirDown1

    lda YDirDown2
    sta XDirRight1

    lda XVel2
    sta YVel1

    lda YVel2
    sta XVel1

    ;Subtract from the player's score
    lda Player2Score
    beq .skipScore

    dec Player2Score

.skipScore

    rts



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Player 2 got bumped by player 1
    ; Knock the second player around a bit
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
BumpP2 subroutine
    lda XDirRight1
    sta YDirDown2

    lda YDirDown1
    sta XDirRight2

    lda XVel1
    sta YVel2

    lda YVel1
    sta XVel2

    ;Subtract from the player's score
    lda Player1Score
    beq .skipScore

    dec Player1Score

.skipScore

    rts



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Update the game timer
    ; Play the appropriate sounds
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
UpdateTimer subroutine
    lda counter
    and #63
    beq .timerTick   ; tick the timer once every 64 frames
    rts

.timerTick 
    lda GameTimer         ; get TimerPF
    bne .decrementTimer  ; if the upper nybble is not 0 then time remains

    ;otherwise time ran out so end the game

    lda #$08
    sta COLUBK

    ;TODO: play the game over sound effect
    
    ;ldy #sfxGAMEOVER     ; select sound effect
    ;jsr SFX_TRIGGER     ; and trigger it

    rts

.decrementTimer:
    lda GameTimer
    sec ;Set carry flag
    sbc #1 ;Subtract 1
    sta GameTimer

    ;TODO: play the game ending soon sound effect

    ;and #%00011111  ; check the lower 5 bits
    ;bne .noTickSfx   ; branch if there's a value in the lower 5 bits
    
    ;ldy #sfxGAMEOVER     ; select sound effect
    ;jsr SFX_TRIGGER     ; and trigger it

;.noTickSfx:
    rts



    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; The players are spinning and crashed into each other
    ; Bounce them like billiard balls
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CrashPlayers subroutine

    ;Swap the horizontal directions
    lda XDirRight1
    sta Temp
    lda XDirRight2
    sta XDirRight1
    lda Temp
    sta XDirRight2

    ;swap vertical directions
    lda YDirDown1
    sta Temp
    lda YDirDown2
    sta YDirDown1
    lda Temp
    sta YDirDown2

    ;swap horizontal velocities
    lda XVel1
    sta Temp
    lda XVel2
    sta XVel1
    lda Temp
    sta XVel2

    ;swap vertical velocities
    lda YVel1
    sta Temp
    lda YVel2
    sta YVel1
    lda Temp
    sta YVel2

    rts









SetCharacter subroutine
    lda counter
    and %00000001
    tay
    rts






; Fetches bitmap data for two digits of a
; BCD-encoded number, storing it in addresses
; FontBuf+x to FontBuf+4+x.
GetBCDBitmap subroutine
    ; First fetch the bytes for the 1st digit
    pha		; save original BCD number
    and #$0F	; mask out the least significant digit
    sta Temp
    asl
    asl
    adc Temp	; multiply by 5
    tay		; -> Y
    lda #5
    sta Temp	; count down from 5
.loop1
    lda DigitsBitmap,y
    and #$0F	; mask out leftmost digit
    sta FontBuf,x	; store leftmost digit
    iny
    inx
    dec Temp
    bne .loop1
    ; Now do the 2nd digit
    pla		; restore original BCD number
    lsr
    lsr
    lsr
    lsr		; shift right by 4 (in BCD, divide by 10)
    sta Temp
    asl
    asl
    adc Temp	; multiply by 5
    tay		; -> Y
    dex
    dex
    dex
    dex
    dex		; subtract 5 from X (reset to original)
    lda #5
    sta Temp	; count down from 5
.loop2
    lda DigitsBitmap,y
    and #$F0	; mask out leftmost digit
    ora FontBuf,x	; combine left and right digits
    sta FontBuf,x	; store combined digits
    iny
    inx
    dec Temp
    bne .loop2
    rts




; Bitmap pattern for digits
DigitsBitmap ;;{w:8,h:5,count:10,brev:1};;
        .byte $EE,$AA,$AA,$AA,$EE
        .byte $22,$22,$22,$22,$22
        .byte $EE,$22,$EE,$88,$EE
        .byte $EE,$22,$66,$22,$EE
        .byte $AA,$AA,$EE,$22,$22
        .byte $EE,$88,$EE,$22,$EE
        .byte $EE,$88,$EE,$AA,$EE
        .byte $EE,$22,$22,$22,$22
        .byte $EE,$AA,$EE,$AA,$EE
        .byte $EE,$AA,$EE,$22,$EE





;===============================================================================
; Sound Effects
; -------------
; Sound effect routines are stored in another source code file.
; In order to use them, you need to allocate 2 RAM variables:
;
;   SFX_LEFT:       ds 1
;   SFX_RIGHT:      ds 1
;
; and include the other file:
;           include sfx.asm
;
; Once per frame call SFX_UPDATE.  Preferably at the same instance every frame
; so the durations don't vary, so call it at either the start of VerticalBlank
;   VerticalBlank:
;           jsr SFX_UPDATE
;
; or at the start of OverScan:
;
;   OverScan:
;           ... ; turn off video and set timer, then call
;           jsr SFX_UPDATE
;           ... 
;
; To trigger a sound effect, load Y with the effect and call SFX_TRIGGER
;
;           ldy #??
;           jsr SFX_TRIGGER
;
; If you'd like to silence output output at any time, call SFX_OFF:
;           jsr SFX_OFF
;===============================================================================
        include sfx.asm

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Bank 0 epilogue
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    org  $1FFA
    rorg $FFFA
    BANK_VECTORS
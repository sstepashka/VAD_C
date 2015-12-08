//
//  VAD.h
//  VAD
//
//  Created by Kuragin Dmitriy on 15/10/15.
//  Copyright © 2015 Kuragin Dmitriy. All rights reserved.
//

#ifndef VAD_h
#define VAD_h

#include <stdio.h>

typedef enum {
    VADStateUnknown = -1,
    VADStateInProgress = 1,
    VADStateEndOfSpeech = 2
} VADState;

typedef struct VAD* VADRef;

VADRef createVAD();
VADState processVADFrameNormalized(const VADRef, const short *frame, unsigned int frames_count);
VADState processVADFrame16(const VADRef, const short *frame, unsigned int frames_count);
VADState processVADFrame24(const VADRef, const short *frame, unsigned int frames_count);
void resetVAD(const VADRef);
void destroyVAD(VADRef);

#endif /* VAD_h */

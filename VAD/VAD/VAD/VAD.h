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
    VADStateError = -2,
    VADStateUnknown = -1,
    VADStateInProgress = 1,
    VADStateEndOfSpeech = 2,
    VADStateNoSpeech = 3
} VADState;

typedef struct VADContext *VADContextRef;

VADContextRef VADContextCreate();
void VADContextRelease(VADContextRef context);

VADState VADContextAnalyseFrames(const VADContextRef context, const short *frames, size_t frames_count);
void VADContextReset(const VADContextRef context);

#endif /* VAD_h */

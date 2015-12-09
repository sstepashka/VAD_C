//
//  VADContext.h
//  VAD
//
//  Created by Kuragin Dmitriy on 15/10/15.
//  Copyright © 2015 Kuragin Dmitriy. All rights reserved.
//

#ifndef VAD_CONTEXT_H
#define VAD_CONTEXT_H

#include <stdio.h>

#include "VADState.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct VADContext *VADContextRef;

    VADContextRef VADContextCreate();
    void VADContextRelease(VADContextRef context);

    VADState VADContextAnalyseFrames(const VADContextRef context, const short *frames, size_t frames_count);
    void VADContextReset(const VADContextRef context);

#ifdef __cplusplus
}
#endif

#endif /* VAD_CONTEXT_H */

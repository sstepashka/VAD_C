//
//  VADCheck.h
//  VAD
//

#ifndef VADCheck_h
#define VADCheck_h

#include <stdio.h>

#include "VADState.h"

typedef enum {
    AudioFileEncoding_16bit_PCM_big_endian_16000_mono = 0,
    AudioFileEncoding_16bit_PCM_intel_endian_16000_mono = 1
} AudioFileEncoding;

VADState VADAnalyseAudioFile(const char *const filename, AudioFileEncoding encoding);

#endif /* VADCheck_h */

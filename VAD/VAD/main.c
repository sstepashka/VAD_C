//
//  main.c
//  VAD
//
//  Created by Kuragin Dmitriy on 15/10/15.
//  Copyright © 2015 Kuragin Dmitriy. All rights reserved.
//

#include <stdio.h>

#include "VADCheck.h"

int main(int argc, const char * argv[]) {

    VADAnalyseAudioFile("count-hammer.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);
    VADAnalyseAudioFile("count.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);
    VADAnalyseAudioFile("count1.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);
    VADAnalyseAudioFile("sound1.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);

    // XXX: This test fails, but it doesn't seem to be critical for now.
    VADAnalyseAudioFile("window.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);

    VADAnalyseAudioFile("z01.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);
    VADAnalyseAudioFile("z03.raw", AudioFileEncoding_16bit_PCM_intel_endian_16000_mono);

    return 0;
}

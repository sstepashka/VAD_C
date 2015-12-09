//
//  VADCheck.c
//  VAD
//

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "VADCheck.h"

static const char *const VADSubpath_16bit_PCM_big_endian_16000_mono =
    "/16bit_PCM_big_endian_16000_mono/";
static const char *const VADSubpath_16bit_PCM_intel_endian_16000_mono =
    "/16bit_PCM_intel_endian_16000_mono/";

VADState VADAnalyseAudioFile(const char *const filename, AudioFileEncoding encoding) {
    if (filename == NULL) {
        return VADStateError;
    }

    char *cwd = getcwd(NULL, 0);
    const char *const subpath = (encoding == AudioFileEncoding_16bit_PCM_big_endian_16000_mono) ?
        VADSubpath_16bit_PCM_big_endian_16000_mono : VADSubpath_16bit_PCM_intel_endian_16000_mono;

    size_t const cwd_length = strlen(cwd);
    size_t const subpath_length = strlen(subpath);
    size_t const filename_length = strlen(filename);

    char *filePath = (char *)malloc((cwd_length + subpath_length + filename_length + 1) * sizeof(char));
    memcpy(filePath, cwd, cwd_length * sizeof(char));
    memcpy(filePath + cwd_length, subpath, subpath_length * sizeof(char));
    memcpy(filePath + cwd_length + subpath_length, filename, filename_length * sizeof(char));
    memcpy(filePath + cwd_length + subpath_length + filename_length, "\0", sizeof(char));

    free(cwd);
    cwd = NULL;

    FILE *input = fopen(filePath, "rb");

    VADState state = VADStateUnknown;
    if (input != NULL) {
        VADContextRef context = VADContextCreate();
        if (context != NULL) {
            printf("--->>> Analysing: %s\n", filePath);

            size_t const frames_count = 160;
            short *frames = (short *)malloc(frames_count * sizeof(short));

            while (!feof(input)) {
                if (fread(frames, sizeof(short), frames_count, input) != frames_count) {
                    abort();
                }

                state = VADContextAnalyseFrames(context, frames, frames_count);
                if ((state != VADStateUnknown) && (state != VADStateInProgress)) {
                    break;
                }
            }

            free(frames);
            frames = NULL;

            VADContextRelease(context);
            printf("<<<--- Analysis has been finished with result: %i\n", state);
        }

        fclose(input);
        input = NULL;
    }
    else {
        printf("File %s doesn't exist\n", filePath);
        state = VADStateError;
    }
    free(filePath);
    printf("\n");

    return state;
}

//
//  VAD.c
//  VAD
//
//  Created by Kuragin Dmitriy on 15/10/15.
//  Copyright © 2015 Kuragin Dmitriy. All rights reserved.
//

#include "VAD.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define CHUNK_SIZE 160

struct VAD {
    VADState state;
    short *buffer;
    unsigned int frames_count;
    
    float energy_factor;
    unsigned int first_frames_count;
    unsigned int noise_frames_count;
    
    float noise_energy;
};

void freeBuffer(VADRef self) {
    if(self->buffer) {
        free(self->buffer);
        self->buffer = NULL;
    }
    
    self->frames_count = 0;
}

VADRef createVAD()
{
    VADRef self = (VADRef) malloc(sizeof(struct VAD));

    self->buffer = NULL;
    self->frames_count = 0;
    
    resetVAD(self);
    
    return self;
}

void appendBufferVAD(const VADRef self, const short *frame, unsigned int frames_count) {
    if (!frame) return;
    if (!frames_count) return;
    
    unsigned int current_frames_count = self->frames_count;
    unsigned int received_frames_count = frames_count;
    unsigned int total_frames_count = current_frames_count + received_frames_count;
    
    short *res = (short *)malloc(sizeof(short) * total_frames_count);
    
    if (current_frames_count) {
        memcpy(res, self->buffer, current_frames_count * sizeof(short));
        memcpy(res + current_frames_count, frame, received_frames_count * sizeof(short));
        free(self->buffer);
        self->buffer = res;
    } else {
        memcpy(res, frame, received_frames_count * sizeof(short));
        self->buffer = res;
    }
    
    self->frames_count = total_frames_count;
}

void popDataChunk(VADRef self, short *data) {
    unsigned int chunk_size = CHUNK_SIZE;
    
    if (!data) {
        exit(-1);
    }
    
    if (self->frames_count < chunk_size) {
        exit(-2);
    }
    
    unsigned int current_size = self->frames_count;
    unsigned int result_size = current_size - chunk_size;
    
    short *res = (short *)malloc(sizeof(short) * result_size);
    memcpy(res, self->buffer + chunk_size, result_size * sizeof(short));
    memcpy(data, self->buffer, chunk_size * sizeof(short));
    
    free(self->buffer);
    self->buffer = res;
    self->frames_count = result_size;
}

float energyFrame(float *frame, unsigned int size) {
    float energy = 0;
    for (int i = 0; i < size; i++) {
        energy += frame[i] * frame[i];
    }
    
    return energy / (float)size;
}

float *normalize(short *frame, unsigned int size) {
    float *normalized = (float *)malloc(sizeof(float) * size);
    
    for (int i = 0; i < size; i++) {
        normalized[i] = frame[i] / SHRT_MAX;
    }
    
    return normalized;
}

unsigned int cross_zero_count_frame(short *frame, unsigned int size) {
    int sign = 0;
    int lastsign = 0;
    
    unsigned int cross_zero_count = 0;
    
    for (int i = 0; i < size; i++) {
        if (frame[i] > 0) {
            sign = 1;
        } else {
            sign = -1;
        }
        
        if (lastsign != 0 && sign != lastsign) {
            cross_zero_count ++;
            lastsign = sign;
        }
    }
    
    return cross_zero_count;
}

VADState processChunk(const VADRef self, short *chunk, unsigned int size) {
    float *normalized = normalize(chunk, size);

    float energy = energyFrame(normalized, size);
    
    unsigned int cross_zero_count = cross_zero_count_frame(chunk, size);
    
    bool is_frame_active = false;
    
    if (self->first_frames_count < self->noise_frames_count) {
        self->first_frames_count ++;
        self->noise_energy += energy / (float)self->noise_frames_count;
        
        is_frame_active = false;
    } else {
        if (cross_zero_count >= 5 && cross_zero_count <= 15) {
            if (energy >= fmaxf(self->noise_energy, 0.001818) * self->energy_factor) {
                is_frame_active = true;
            }
        }
    }
    
    free(normalized);
    
    VADState state = VADStateUnknown;
    
    return state;
}

VADState processVADFrame(const VADRef self, const short *frame, unsigned int frames_count)
{
    if (self->state == VADStateEndOfSpeech) {
        return VADStateEndOfSpeech;
    } else {
        appendBufferVAD(self, frame, frames_count);
        
        while (self->frames_count > CHUNK_SIZE && self->state != VADStateEndOfSpeech) {
            short *chunk = (short *)malloc(sizeof(short) * CHUNK_SIZE);
            popDataChunk(self, chunk);
            self->state = processChunk(self, chunk, CHUNK_SIZE);
            free(chunk);
        }
    }

    return self->state;
}

void resetVAD(const VADRef self)
{
    self->state = VADStateUnknown;
    freeBuffer(self);
    
    self->frames_count = 0;
    self->energy_factor = 3.1f;
    self->first_frames_count = 0;
    self->noise_frames_count = 15;
    self->noise_energy = 0.f;
    
    self->state = VADStateUnknown;
}

void destroyVAD(VADRef self)
{
    freeBuffer(self);
    free(self);
    
    self = NULL;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

// Constants
#define SAMPLE_RATE 44100  // 44.1 kHz sample rate
#define DURATION 5         // Duration in seconds
#define AMPLITUDE 32767    // Maximum amplitude for 16-bit audio
#define FREQUENCY 440      // Frequency in Hz (for all waveforms)
#define WAVETABLE_SIZE 512 // Length of the wavetable


// Function to generate sine wave sample
int16_t sine_wave(double t) {
    return (int16_t)(AMPLITUDE * sin(2 * M_PI * FREQUENCY * t));
}

// Function to generate square wave sample
int16_t square_wave(double t) {
    return (int16_t)(AMPLITUDE * (sin(2 * M_PI * FREQUENCY * t) >= 0 ? 1 : -1));
}

// Function to generate triangle wave sample
int16_t triangle_wave(double t) {
    return (int16_t)(AMPLITUDE * (2 * fabs(2 * (t * FREQUENCY - floor(t * FREQUENCY + 0.5))) - 1));
}

// Function to initialize the wavetable with 'interesting' data
void initialize_wavetable(uint8_t wavetable[WAVETABLE_SIZE]) {
    srand(time(NULL));  // Seed the random number generator

    // Fill the wavetable with interesting data: mix sine wave with random noise
    for (int i = 0; i < WAVETABLE_SIZE; i++) {
        double t = (double)i / WAVETABLE_SIZE;  // Normalize index to [0, 1]
        
        // Generate a sine wave component
        double sine_wave = 0.5 * sin(2 * M_PI * t * 10); // 10 Hz sine wave component
        
        // Generate some random noise
        double noise = ((rand() % 256) - 128) / 128.0; // Random noise in range [-1, 1]
        
        // Combine sine wave and noise to form interesting data
        double combined_signal = sine_wave + noise;
        
        // Map the combined signal to a byte in the range [0, 255]
        wavetable[i] = (uint8_t)((combined_signal + 1) * 127.5); // Normalize to [0, 255]
    }
}


// Function to generate wavetable-based sample
int16_t wavetable_sample(uint8_t wavetable[WAVETABLE_SIZE], double t) {
    // Determine the index in the wavetable based on the time
    double index = fmod(t * SAMPLE_RATE, WAVETABLE_SIZE); // Wrap around the wavetable length
    int table_index = (int)index;
    
    // Map the wavetable byte to a 16-bit signed value
    return (int16_t)(AMPLITUDE * ((double)wavetable[table_index] / 255.0 * 2.0 - 1.0)); // Normalize to [-1, 1]
}


// Bytebeat function to choose waveform based on user
int16_t wavegen(double t, int wave_select) {
    
    if (wave_select == 0) {
        return sine_wave(t);
    } else if (wave_select == 1) {
        return square_wave(t);
    } else {
        return triangle_wave(t);
    }
}

int main() {
    // Pipe audio data to stdout (sox will read this)
	int wave = 2;
    uint8_t wavetable[WAVETABLE_SIZE];
	initialize_wavetable(wavetable);

    for (int i = 0; i < SAMPLE_RATE * DURATION; i++) {
        double t = (double)i / SAMPLE_RATE;  // Time in seconds

        //int16_t sample = wavegen(t, wave);
        int16_t sample = wavetable_sample(wavetable, t);

        // Write the sample as little-endian 16-bit PCM to stdout
        fwrite(&sample, sizeof(int16_t), 1, stdout);
	
    	// Waveform selection based on time (0 = sine, 1 = square, 2 = triangle)
    	//wave = (int)(t * 0.8) % 3;
    }

    return 0;
}


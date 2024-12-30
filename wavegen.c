#include <stdio.h>
#include <math.h>
#include <stdint.h>

// Constants
#define SAMPLE_RATE 44100  // 44.1 kHz sample rate
#define DURATION 5         // Duration in seconds
#define AMPLITUDE 32767    // Maximum amplitude for 16-bit audio
#define FREQUENCY 440      // Frequency in Hz (for all waveforms)

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

    for (int i = 0; i < SAMPLE_RATE * DURATION; i++) {
        double t = (double)i / SAMPLE_RATE;  // Time in seconds
        int16_t sample = wavegen(t, wave);

        // Write the sample as little-endian 16-bit PCM to stdout
        fwrite(&sample, sizeof(int16_t), 1, stdout);
	
    	// Waveform selection based on time (0 = sine, 1 = square, 2 = triangle)
    	//wave = (int)(t * 0.8) % 3;
    }

    return 0;
}


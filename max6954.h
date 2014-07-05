#ifndef max6954_h
#define max6954_h

#include "Arduino.h"

#define POUND      24
#define YEN        25
#define AE         26
#define DEG        27
#define HALFM      28
#define CROSS      29
#define ROCKETUP   30
#define ROCKETDOWN 31
#define DEL        127

class MAX6954 {
  public:

    MAX6954(uint8_t out, uint8_t in, uint8_t clk, uint8_t c1, uint8_t c2);

    uint8_t data_in, data_out, clock, chip1, chip2;

    void begin();
    void set_digit_type_all_16segment();
    void set_digit_type_all_14segment();
    void read(byte address, uint8_t cs);
    void write(byte address, byte value);
    void write_chip1(byte address, byte value);
    void write_chip2(byte address, byte value);
    void control_intensity(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7);
    void write_string(char str[], int row, int col);
    void write_string1(char str[]);
    void write_string2(char str[]);
    void write_int1(int value);
    void enable_individual_segment_brightness();
    void enable_global_segment_brightness();
    void set_global_brightness(int i);
    void write_lowercase(char c, int row, int col);
    void write_lowercase_string(char string[], int row, int col);
    void disable_decode_mode();
    void enable_decode_mode();
    void turn_off_individual_segments();
    void individual_segment_test();
};

#endif

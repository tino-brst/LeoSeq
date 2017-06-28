// instrument definitions

   #define HIGH_Q            0   // MIDI Note: 27
   #define SLAP              1   // MIDI Note: 28
   #define SCRATCH_PUSH      2   // MIDI Note: 29
   #define SCRATCH_PULL      3   // MIDI Note: 30
   #define STICKS            4   // MIDI Note: 31
   #define SQUARE_CLICK      5   // MIDI Note: 32
   #define METRONOME_CLICK   6   // MIDI Note: 33
   #define METRONOME_BELL    7   // MIDI Note: 34
   #define BASS_DRUM_2       8   // MIDI Note: 35
   #define BASS_DRUM_1       9   // MIDI Note: 36
   #define SIDE_STICK        10  // MIDI Note: 37
   #define SNARE_DRUM_1      11  // MIDI Note: 38
   #define HAND_CLAP         12  // MIDI Note: 39
   #define SNARE_DRUM_2      13  // MIDI Note: 40
   #define LOW_TOM_2         14  // MIDI Note: 41
   #define CLOSED_HI_HAT     15  // MIDI Note: 42
   #define LOW_TOM_1         16  // MIDI Note: 43
   #define PEDAL_HI_HAT      17  // MIDI Note: 44
   #define MID_TOM_2         18  // MIDI Note: 45
   #define OPEN_HI_HAT       19  // MIDI Note: 46
   #define MID_TOM_1         20  // MIDI Note: 47
   #define HIGH_TOM_2        21  // MIDI Note: 48
   #define CRASH_CYMBAL_1    22  // MIDI Note: 49
   #define HIGH_TOM_1        23  // MIDI Note: 50
   #define RIDE_CYMBAL_1     24  // MIDI Note: 51
   #define CHINESE_CYMBAL    25  // MIDI Note: 52
   #define RIDE_BELL         26  // MIDI Note: 53
   #define TAMBOURINE        27  // MIDI Note: 54
   #define SPLASH_CYMBAL     28  // MIDI Note: 55
   #define COWBELL           29  // MIDI Note: 56
   #define CRASH_CYMBAL_2    30  // MIDI Note: 57
   #define VIBRA_SLAP        31  // MIDI Note: 58
   #define RIDE_CYMBAL_2     32  // MIDI Note: 59
   #define HIGH_BONGO        33  // MIDI Note: 60
   #define LOW_BONGO         34  // MIDI Note: 61
   #define MUTE_HIGH_CONGA   35  // MIDI Note: 62
   #define OPEN_HIGH_CONGA   36  // MIDI Note: 63
   #define LOW_CONGA         37  // MIDI Note: 64
   #define HIGH_TIMBALE      38  // MIDI Note: 65
   #define LOW_TIMBALE       39  // MIDI Note: 66
   #define HIGH_AGOGO        40  // MIDI Note: 67
   #define LOW_AGOGO         41  // MIDI Note: 68
   #define CABASA            42  // MIDI Note: 69
   #define MARACAS           43  // MIDI Note: 70
   #define SHORT_WHISTLE     44  // MIDI Note: 71
   #define LONG_WHISTLE      45  // MIDI Note: 72
   #define SHORT_GUIRO       46  // MIDI Note: 73
   #define LONG_GUIRO        47  // MIDI Note: 74
   #define CLAVES            48  // MIDI Note: 75
   #define HIGH_WOOD_BLOCK   49  // MIDI Note: 76
   #define LOW_WOOD_BLOCK    50  // MIDI Note: 77
   #define MUTE_CUICA        51  // MIDI Note: 78
   #define OPEN_CUICA        52  // MIDI Note: 79
   #define MUTE_TRIANGLE     53  // MIDI Note: 80
   #define OPEN_TRIANGLE     54  // MIDI Note: 81
   #define SHAKER            55  // MIDI Note: 82
   #define JINGLE_BELL       56  // MIDI Note: 83
   #define BELLTREE          57  // MIDI Note: 84
   #define CASTANETS         58  // MIDI Note: 85
   #define MUTE_SURDO        59  // MIDI Note: 86

// ============================================================================

int getInstrumentNote(int kitNumber);

char* getInstrumentName();

int instrumentsAvailableCount();

int getNextInstrument(int kitNumber);

int getPreviousInstrument(int kitNumber);
#pragma once
#include "messages.h"
#include "primitives/string_utils.h"

// Given a pulse lens from the base station, this class determines the phase for the current cycle sweep
// Phases are:
//   0) Base (A), horizontal sweep (j0, j1)
//   1) Base (A), vertical sweep (k0, k1)

class CyclePhaseClassifier {
  public:
    CyclePhaseClassifier();

  	// Process serial output for pulse length for current cycle (identified by cycle_id)
  	void process_signal_length(uint32_t cycle_idx, const TimeDelta &pulse_lens);

  	// Get current phase. -1 if unknown (fix failed)
  	int get_phase(uint32_t cycle_idx);

  	// Get updated data bit from current cycle's sync pulse.
  	// Caller needs to check that bits were updated for the current cycle
  	// by checking OOTXFrameBit.cycle_id == cycle_id;
  	OOTXFrameBit get_data_bit(uint32_t cycle_idx, const TimeDelta &signal_lens);

  	// Reset state of this classifier -- called if cycle fix lost/failed
  	void reset();

    // Print debug information.
    virtual bool debug_cmd(HashedWord *input_words);
    virtual void debug_print(PrintStream &stream);
  private:
    float expected_pulse_len(bool skip, bool data, bool axis);

    uint32_t prev_full_cycle_idx_;
    uint32_t phase_history_;

    int fix_level_;
    uint32_t phase_shift_;

    float pulse_base_len_;
    DataFrameBit bits_[num_base_stations];

    float average_error_;
    bool debug_print_state_;
};

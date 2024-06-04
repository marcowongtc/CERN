#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <sstream>
#include <cmath>
#include <TFile.h>
#include <TTree.h>

using namespace std;

// Return the times when the clock cuts the zero level in a certain channel
// For finding transition from up to down, set "up_to_down = true", and vice versa
// Argument "threshold" is the voltage level of the transition, "buffer" is the minimum time difference (in ns) between two consecutive transitions  
vector<float> times_cuts_zero(TTree *tree, const int& evtctr, vector<float>* clocktime, vector<float>* ch, const float& start_ns, const float& end_ns, bool up_to_down=true, const float& threshold=0.05, const float& buffer=0.9) {

    tree->GetEntry(evtctr);
    vector<float> cut_times = {};

    for (int k = 0; k < clocktime->size()-1; k++) {
        
        // Focus on the required time range
        float clocktime_ns = clocktime->at(k)*1e9;
        if (clocktime_ns < start_ns) continue;
        if (clocktime_ns > end_ns) break;
        
        //Find the up-to-down transitions
        if (up_to_down) {
            if (ch->at(k) >= threshold && ch->at(k+1) < threshold) {
                if (cut_times.size() > 0) {
                    if (clocktime_ns - cut_times.back() > buffer) cut_times.push_back(clocktime_ns);   
                }
                else 
                    cut_times.push_back(clocktime_ns);
            }
        }
        //Find the down-to-up transitions
        else {
            if (ch->at(k) <= threshold && ch->at(k+1) > threshold) {
                if (cut_times.size() > 0) {
                    if (clocktime_ns - cut_times.back() > buffer)  cut_times.push_back(clocktime_ns);
                }
                else
                    cut_times.push_back(clocktime_ns);
            }
        }
    }

    return cut_times;
}

// Identify the extrema
// For finding minima, set "find_min = true", and vice versa
// If setting "pos_max_neg_min = true", the maxima will be required to have positive voltage, and the minima will be required to have negative voltage
// Argument "neighbor_points" are the number of neighboring points so that the extrema is the global extrema in this range
vector<float> find_extrema(TTree *tree, const int& evtctr, vector<float>* clocktime, vector<float>* ch, const float& start_ns, const float& end_ns, bool find_min=true, bool pos_max_neg_min=true, const float& buffer=0.9, const int& neighbor_points=30) {

    tree->GetEntry(evtctr);
    vector<float> extrema = {};

    for (int k = neighbor_points; k < clocktime->size()-neighbor_points; k++) {
        
        // Focus on the required time range
        float clocktime_ns = clocktime->at(k)*1e9;
        if (clocktime_ns < start_ns) continue;
        if (clocktime_ns > end_ns) break;
        
        // Find the minima
        if (find_min) {
            if (pos_max_neg_min && ch->at(k) > 0) continue;
            bool isMin = true;
            for (int m = 1; m <= 2*neighbor_points+1; m++) {
                if ( ch->at(k) > ch->at(k-neighbor_points+m-1) ) {
                    isMin = false;
                }
            }
            if (isMin) {
                if (extrema.size() > 0) {
                    if (clocktime_ns - extrema.back() > buffer) extrema.push_back(clocktime_ns);
                }
                else
                    extrema.push_back(clocktime_ns);
            }
        }
        
        // Find the maxima
        else {
            if (pos_max_neg_min && ch->at(k) < 0) continue;
            bool isMax = true;
            for (int m = 1; m <= 2*neighbor_points+1; m++) {
                if ( ch->at(k) < ch->at(k-neighbor_points+m-1) ) {
                    isMax = false;
                }
            }
            if (isMax) {
                if (extrema.size() > 0) {
                    if (clocktime_ns - extrema.back() > buffer) extrema.push_back(clocktime_ns);
                }
                else
                    extrema.push_back(clocktime_ns);
            }
        }
    } 
        
    return extrema;
}

// Turn an 8-element vector of '0' and '1' into decimal
int vector_to_decimal(const vector<int>& input_vec) {

    if (input_vec.size() != 8) return -1;

    int result = 0;
    int place_value = 1;
    for (int i = input_vec.size() - 1; i >= 0; i--) {
        result += input_vec[i] * place_value;
        place_value *= 2;
    }
    return result;
}

void decode(TTree *tree,
            int& event_number,
            vector<float>* clocktime,
            vector<vector<float>*> decode_chs,
            vector<float>* injected_signal_ch,
            vector<float>* disc_signal_ch,
            const string& toWrite_output_name,
            const float& start_ns,
            const float& end_ns,
            vector<vector<float>*> clock_chs={},
            bool fix_bit_size=true,
            bool writeFile=true,
            bool writeScreenOutput=true,
            const float& disc_signal_threshold=0.65,
            const float& injected_signal_threshold=0.008,
            const float& threshold=0.05,
            const float& buffer=0.9 ) {
    
    // Check if the parameters are OK
    if (!clock_chs.empty() && decode_chs.size() != clock_chs.size()) {
        cout << "The numbers of decode channels and clock channels are not the same. Please check." << endl;
        return;
    }
    
    // Output file and output tree
    TFile *toWrite_outputFile;
    TTree *decode_tree;
    int cycle;   // The number added to the trailing decimal when it is smaller than the leading decimal
    float vco_freq;
    float bit_size;
    int decoding_signal_match;  // Whether the number of manchester errors found equals the number of signals produced (0 -> not equal; 1 -> equal)
    vector<int> *need_investigation_vec = new vector<int>;  // Whether the word/decoding is reliable (0 -> reliable; 1 -> not reliable, need further investigation)
    vector<int> *outside_range_vec = new vector<int>;  // Sometimes the word is incomplete due to limited range of the window, 1 -> incomplete
    vector<int> *leading0_vec = new vector<int>;  // The least significant bit
    vector<int> *leading1_vec = new vector<int>;
    vector<int> *leading2_vec = new vector<int>;
    vector<int> *leading3_vec = new vector<int>;
    vector<int> *leading4_vec = new vector<int>;
    vector<int> *leading5_vec = new vector<int>;
    vector<int> *leading6_vec = new vector<int>;
    vector<int> *leading7_vec = new vector<int>;  // The most significant bit
    vector<int> *trailing0_vec = new vector<int>; // The least significant bit
    vector<int> *trailing1_vec = new vector<int>;
    vector<int> *trailing2_vec = new vector<int>;
    vector<int> *trailing3_vec = new vector<int>;
    vector<int> *trailing4_vec = new vector<int>;
    vector<int> *trailing5_vec = new vector<int>;
    vector<int> *trailing6_vec = new vector<int>;
    vector<int> *trailing7_vec = new vector<int>; // The most significant bit
    vector<int> *leading_decimal_vec = new vector<int>;
    vector<int> *trailing_decimal_vec = new vector<int>;
    vector<float> *leading_ns_vec = new vector<float>;
    vector<float> *trailing_ns_vec = new vector<float>;
    vector<float> *time_width_vec = new vector<float>;

    //The signal that goes downward in oscilloscope (not sure if this is the correct name)
    vector<float> *injected_signal_time_width = new vector<float>;
    vector<float> *injected_signal_leading_ns = new vector<float>;
    vector<float> *injected_signal_trailing_ns = new vector<float>;
    
    //The signal that goes upward in oscilloscope (not sure if this is the correct name)
    vector<float> *disc_signal_time_width = new vector<float>;
    vector<float> *disc_signal_leading_ns = new vector<float>;
    vector<float> *disc_signal_trailing_ns = new vector<float>;
    
    vector<float> *reset_discSignal_time_difference = new vector<float>;    // The time between the discriminator signal and the reset just before it
    
    if (writeFile) {
        toWrite_outputFile = new TFile(toWrite_output_name.c_str(), "RECREATE");
        decode_tree = new TTree("tree", "decode_output");
        decode_tree->Branch("event_number", &event_number, "event_number/I");
        decode_tree->Branch("cycle", &cycle, "cycle/I");
        decode_tree->Branch("vco_freq", &vco_freq, "vco_freq/F");
        decode_tree->Branch("bit_size", &bit_size, "bit_size/F");
        decode_tree->Branch("need_investigation", "vector<int>", &need_investigation_vec);
        decode_tree->Branch("outside_range", "vector<int>", &outside_range_vec); 
        decode_tree->Branch("leading7", "vector<int>", &leading7_vec);
        decode_tree->Branch("leading6", "vector<int>", &leading6_vec);
        decode_tree->Branch("leading5", "vector<int>", &leading5_vec);
        decode_tree->Branch("leading4", "vector<int>", &leading4_vec);
        decode_tree->Branch("leading3", "vector<int>", &leading3_vec);
        decode_tree->Branch("leading2", "vector<int>", &leading2_vec);
        decode_tree->Branch("leading1", "vector<int>", &leading1_vec);
        decode_tree->Branch("leading0", "vector<int>", &leading0_vec);
        decode_tree->Branch("trailing7", "vector<int>", &trailing7_vec);
        decode_tree->Branch("trailing6", "vector<int>", &trailing6_vec);
        decode_tree->Branch("trailing5", "vector<int>", &trailing5_vec);
        decode_tree->Branch("trailing4", "vector<int>", &trailing4_vec);
        decode_tree->Branch("trailing3", "vector<int>", &trailing3_vec);
        decode_tree->Branch("trailing2", "vector<int>", &trailing2_vec);
        decode_tree->Branch("trailing1", "vector<int>", &trailing1_vec);
        decode_tree->Branch("trailing0", "vector<int>", &trailing0_vec);
        decode_tree->Branch("leading_decimal", "vector<int>", &leading_decimal_vec);
        decode_tree->Branch("trailing_decimal", "vector<int>", &trailing_decimal_vec);
        decode_tree->Branch("leading_ns", "vector<float>", &leading_ns_vec);
        decode_tree->Branch("trailing_ns", "vector<float>", &trailing_ns_vec);        
        decode_tree->Branch("time_width", "vector<float>", &time_width_vec);
        if (disc_signal_ch) {
            decode_tree->Branch("disc_signal_time_width", "vector<float>", &disc_signal_time_width);
            decode_tree->Branch("disc_signal_leading_ns", "vector<float>", &disc_signal_leading_ns);
            decode_tree->Branch("disc_signal_trailing_ns", "vector<float>", &disc_signal_trailing_ns);
            decode_tree->Branch("decoding_signal_match", &decoding_signal_match, "decoding_signal_match/I");
        }
        if (injected_signal_ch) {
            decode_tree->Branch("injected_signal_time_width", "vector<float>", &injected_signal_time_width);  
            decode_tree->Branch("injected_signal_leading_ns", "vector<float>", &injected_signal_leading_ns);
            decode_tree->Branch("injected_signal_trailing_ns", "vector<float>", &injected_signal_trailing_ns);
        }
        if (!clock_chs.empty() && disc_signal_ch)
            decode_tree->Branch("reset_discSignal_time_difference", "vector<float>", &reset_discSignal_time_difference);
    }
    
    // Write the screen output to a txt file
    ofstream outfile;
    streambuf* coutbuf = nullptr;
    if (writeScreenOutput) {
        outfile.open(toWrite_output_name.substr(0, toWrite_output_name.find_last_of("."))+"_screenOutput.txt");
        coutbuf = cout.rdbuf(outfile.rdbuf());
    }
    
    float bit_size_fixed = -1;
    
    // Loop over the events
    for (int evtctr = 0; evtctr < tree->GetEntries(); ++evtctr) {
        
        //if (evtctr < 0 || evtctr > 20) continue;   //for testing
        
        tree->GetEntry(evtctr);
        
        cout << endl << "Event " << evtctr << " =================================================" << endl;
        
        vector<vector<float>> reset_global;
        vector<vector<float>> manch_err_global;
        float duration_global;
        
        for (int decode_ch = 0; decode_ch < decode_chs.size(); decode_ch++) {
            
            // =========== Calculate the bit size (in ns) =========== //
            if (fix_bit_size && bit_size_fixed > 0) {
                bit_size = bit_size_fixed;
            }
            else { 
                vector<float> cut_times = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], start_ns, end_ns, true, threshold, buffer); 
                if (cut_times.size() < 2) {
                    cout << "Cannot calculate the bit size for this channel in event " << evtctr << endl;
                    continue;
                }  
                
                vector<float> diff = {};
                for (int i = 0; i < cut_times.size()-1; ++i) {
                    diff.push_back(cut_times[i+1] - cut_times[i]);
                }
                
                // Find 3 consecutive time differences which are similar to each other
                vector<float> diff_forBitSize = {};
                int i = 0;
                while (i < diff.size()-2) {
                    if (abs(diff[i+2]-diff[i+1]) < 0.05 && abs(diff[i+1]-diff[i]) < 0.05 && abs(diff[i+2]-diff[i+1]-diff[i+1]+diff[i]) < 0.05) {
                        for (int j = i; j < diff.size()-1; j++) {
                            i = j + 1;
                            if (abs(diff[j+1]-diff[j]) < 0.05 && diff[j] < 4) diff_forBitSize.push_back(diff[j]);  //Assume bit size < 4 ns
                            else break;
                        }
                    }
                    else i++;
                }

                if (diff_forBitSize.size() < 2) {
                    cout << "Cannot calculate the bit size for this channel in event " << evtctr << endl;
                    continue;
                } 
                bit_size = round( accumulate(diff_forBitSize.begin(), diff_forBitSize.end(), 0.)/diff_forBitSize.size() / 0.04 ) * 0.04;
                bit_size_fixed = bit_size;
            }
            
            cout << "   Bit size: " << bit_size << " ns" << endl;
            vco_freq = 1/bit_size*8;
            cycle = floor(vco_freq*100);
            
            // =========== Find the manchester errors =========== //
            // Find consecutive transitions which is much longer than a usual bit and check if it looks like a manchester error (3 semiperiod at 0 and 1 semiperiod at 1)
            vector<float> manch_code_cut_times = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], start_ns, end_ns, true, threshold, buffer);
            cout << "Locating manchester errors ..." << endl;
            vector<float> manch_error = {};
            for (int j = 0; j < manch_code_cut_times.size()-1; j++) {
                float diff = manch_code_cut_times[j+1] - manch_code_cut_times[j];
                if (diff/bit_size >= 1.9) {
                    if (find_extrema(tree, evtctr, clocktime, decode_chs[decode_ch], manch_code_cut_times[j+1]-bit_size, manch_code_cut_times[j+1], false, true).empty())
                        continue;
                    float manch_error_endtime = find_extrema(tree, evtctr, clocktime, decode_chs[decode_ch], manch_code_cut_times[j+1]-bit_size, manch_code_cut_times[j+1], false, true)[0];
                    vector<float> check_cuts = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], manch_code_cut_times[j], manch_error_endtime, false, 0, buffer);
                    if (check_cuts.size() != 1) continue;
                    if ((manch_code_cut_times[j+1] - check_cuts[0]) / bit_size > 0.5) continue;
                    manch_error.push_back(check_cuts[0]);
                }
            }
            
            cout << "   Manchester errors (ns): ";
            
            if (manch_error.empty()) {
                cout << "   No manchester error found for this channel in event " << evtctr << endl;
                continue;
            }
            
            for (auto x:manch_error) {
                cout << x << "  ";
            }  
            if (!manch_error.empty()) cout << endl;
            
            manch_err_global.push_back(manch_error);
            
            // =========== Estimate the reset duration if possible =========== //
            float duration = -1;
            vector<float> reset = {};
            
            // Identify two consecutive minima between which the voltages are all negative (feature of a reset) 
            vector<float> clock_cut_times = times_cuts_zero(tree, evtctr, clocktime, clock_chs[decode_ch], start_ns, end_ns, true, threshold, buffer);
            vector<float> minima_vec = find_extrema(tree, evtctr, clocktime, clock_chs[decode_ch], start_ns, end_ns, true, true, 1.4, 20); 
            if (clock_chs[decode_ch] == decode_chs[decode_ch]) {  //Find the stable "no-signal" region in the decode channels if the data clock is not available
                for (auto err:manch_error) {
                    clock_cut_times.erase(remove_if(clock_cut_times.begin(), clock_cut_times.end(), [=](int x){return x > err-2*bit_size && x < err+23*bit_size;}), clock_cut_times.end());
                    minima_vec.erase(remove_if(minima_vec.begin(), minima_vec.end(), [=](int x){return x > err-2*bit_size && x < err+23*bit_size;}), minima_vec.end());
                }
            }

            if (clock_cut_times.size() > 1 && minima_vec.size() > 1) {
                for (int i = 1; i < minima_vec.size()-1; ++i) {
                    int min_start, min_end;
                    bool is_reset = true;
                    for (int k = 0; k < clocktime->size(); k++) {
                        if (abs(minima_vec[i] - clocktime->at(k)*1e9) < 0.0001) min_start = k;
                        if (abs(minima_vec[i+1] - clocktime->at(k)*1e9) < 0.0001) min_end = k;
                    }
                    for (int k = min_start; k <= min_end; k++) {
                        if (clock_chs[decode_ch]->at(k) > 0.1) is_reset = false;
                    }
                    
                    if (!is_reset) continue;                    
                    
                    // Set the starting time of the reset to be the transition just before the consecutive minima
                    int reset_start = 0;
                    for (int m = 0; m < clock_cut_times.size(); ++m) {
                        if (clock_cut_times[m] < minima_vec[i] && abs(minima_vec[i] - clock_cut_times[m]) < abs(minima_vec[i] - clock_cut_times[reset_start]))
                            reset_start = m;
                    }
                    if (abs(clock_cut_times[reset_start] - minima_vec[i]) > bit_size) continue;
                    reset.push_back(clock_cut_times[reset_start]);
                }
                if (reset.size() > 0) {
                    if (reset.size() == 1) {
                        // WARNING: only one reset is found. It's not guaranteed to be a reset in case of some weird fluctuation. 
                        float reset_to_add = reset[0];
                        reset = {};
                        while (reset_to_add > start_ns) {
                            reset.insert(reset.begin(), reset_to_add);
                            reset_to_add -= 50;
                        }
                        reset_to_add = reset.back() + 50;
                        while (reset_to_add < end_ns) {
                            reset.push_back(reset_to_add);
                            reset_to_add += 50;
                        }
                    }
                    if (reset.size() >= 2) {
                        // Find two consecutive reset times separated by roughly 50 ns each. Then, use the first reset time as the starting point to calculate all the reset times afterwards by adding 50 ns continously
                        int starting = 0;
                        bool can_use;
                        while (starting < reset.size()-1) {
                            can_use = false;
                            for (int i = starting; i <= starting+1; ++i) {
                                if (i+1 == reset.size()) break;
                                float separation = abs(reset[i+1]-reset[i])/50;
                                if ((separation - round(separation)) < 0.01) can_use = true;
                            }
                            if (can_use) break;
                            starting += 1;
                        }
                        reset = {};
                        if (can_use) {
                            float reset_to_add = reset[starting];
                            while (reset_to_add > start_ns) {
                                reset.insert(reset.begin(), reset_to_add);
                                reset_to_add -= 50;
                            }
                            reset_to_add = reset.back() + 50;
                            while (reset_to_add < end_ns) {
                                reset.push_back(reset_to_add);
                                reset_to_add += 50;
                            }
                        }
                    }
                    // Calculate the duration of reset (taken as the mean of the durations of all the resets)
                    if (!reset.empty()) {
                        clock_cut_times = times_cuts_zero(tree, evtctr, clocktime, clock_chs[decode_ch], start_ns, end_ns, true, threshold, buffer);
                        duration = 0;
                        int num = -1;
                        for (int i = 0; i < reset.size(); i++) {
                            int closest_cut_time_index = 0;
                            float closest_distance = abs(clock_cut_times[0] - reset[i]);
                            for (int j = 1; j < clock_cut_times.size(); j++) {
                                if (abs(clock_cut_times[j] - reset[i]) < closest_distance) {
                                    closest_distance = abs(clock_cut_times[j] - reset[i]);
                                    closest_cut_time_index = j;
                                }
                            }
                            if (closest_cut_time_index + 1 >= clock_cut_times.size()) break;
                            duration += (clock_cut_times[closest_cut_time_index+1] - clock_cut_times[closest_cut_time_index]);
                            num = i+1;
                        }
                        duration = duration/num;
                        duration_global = duration;
                    }
                }
            }
            cout << "   Reset times: ";
            for (int i = 0; i < (reset.size() < 4 ? reset.size() : 4); i++) {
                cout << reset[i] << "  ";
            }
            cout << "..." << endl;
            cout << "   Duration: " << duration << " ns" << endl;
            reset_global.push_back(reset);

            for (int sigCtr = 0; sigCtr < manch_error.size(); sigCtr++) {
                
                int need_investigation = 0;
                int leading_decimal, trailing_decimal;
                float time_width;
                
                // Locate the bit boundaries, not including the resets
                int bit_count = 0;
                float bit_start = manch_error[sigCtr];
                float bit_end = bit_start + bit_size;
                int starting_bit = 3;
                bool out = false;
                cout << "   Start of each bit (not including resets): " << endl;
                while(bit_count < 16+starting_bit) {   
                    if (bit_end > clocktime->at(clocktime->size()-1)*1e9) {
                        out = true;
                        break;
                    }
                    
                    if (bit_count >= starting_bit) cout << "        " << bit_start << ", " << endl;
                    
                    bit_count += 1;
                    bit_start = bit_end;
                    bit_end = bit_end + bit_size;
                }

                if (out) {
                    need_investigation_vec->push_back(0);
                    outside_range_vec->push_back(1);
                    leading7_vec->push_back(-1);
                    leading6_vec->push_back(-1);
                    leading5_vec->push_back(-1);
                    leading4_vec->push_back(-1);
                    leading3_vec->push_back(-1);
                    leading2_vec->push_back(-1);
                    leading1_vec->push_back(-1);
                    leading0_vec->push_back(-1);
                    trailing7_vec->push_back(-1);
                    trailing6_vec->push_back(-1);
                    trailing5_vec->push_back(-1);
                    trailing4_vec->push_back(-1);
                    trailing3_vec->push_back(-1);
                    trailing2_vec->push_back(-1);
                    trailing1_vec->push_back(-1);
                    trailing0_vec->push_back(-1);
                    leading_decimal_vec->push_back(-1);
                    trailing_decimal_vec->push_back(-1);
                    leading_ns_vec->push_back(-1);
                    trailing_ns_vec->push_back(-1);
                    time_width_vec->push_back(-1);
                    continue;
                }
                
                cout << "        " << bit_start << ", " << endl;
                cout << "        " << bit_end << ", " << endl;
                
                //Fine tuning: modify the bit size by: (1) the reset duration, or (2) aligning the clock after the transmission (when resets are not avilable)
                int num_reset_in_word = 1;
                float tune;
                if (duration > 0) {
                    tune = duration - bit_size;
                }
                else {
                    bool found = false;
                    float fraction = 0.25;
                    vector<float> for_alignment;
                    while (!found && bit_end+fraction*bit_size < end_ns) {
                        for_alignment = times_cuts_zero(tree, evtctr,  clocktime, decode_chs[decode_ch], bit_end, bit_end+fraction*bit_size, false, 0);
                        if (for_alignment.size() == 1 && for_alignment[0] != bit_end-fraction*bit_size && for_alignment[0] != bit_end+fraction*bit_size) found = true;
                        fraction += 0.01;
                    }
                    if (for_alignment.size() != 0) {
                        tune = for_alignment[0] - bit_end;
                        if (tune > 1.5) {
                            num_reset_in_word = 2;
                        }
                    }
                }
                cout << "   Tuning: " << tune << endl << endl;
                
                // Decode
                vector<int> leading = {};
                vector<int> trailing = {};
                bit_count = 0;
                bit_start = manch_error[sigCtr];
                bit_end = bit_start + bit_size;
                float transition_previous;
                
                bool need_restart = false;
                vector<bool> has_transition_between_transition = {};
                
                cout << "   Transitions: " << endl;
                while(bit_count < 16+starting_bit) {
                    bool valid = false;
                    if (bit_end > clocktime->at(clocktime->size()-1)*1e9) break;

                    if (bit_count == 0) {
                        vector<float> up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], bit_start+bit_size*0.2, bit_end-bit_size*0.1, true, 0, 0.2); 
                        vector<float> down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], bit_start+bit_size*0.2, bit_end-bit_size*0.1, false, 0, 0.2);
                        if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                            valid = true;
                            transition_previous = down_to_up[0];
                        }
                        else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                            valid = true;
                            transition_previous = up_to_down[0];
                        }
                        else 
                            break;
                    }
                    else {
                        float transition = transition_previous + bit_size;
                        if (reset.size() > 0) {
                            for (int i = 0; i < reset.size(); i++) {
                                if (((reset[i]-transition_previous)/bit_size > 0 && (reset[i]-transition_previous)/bit_size < 0.35) || ((transition_previous-reset[i])/bit_size > 0 && (transition_previous-reset[i])/bit_size < 0.65)) {
                                    transition += tune;
                                    break;
                                }
                            }
                        }
                        vector<float> up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.2, transition+bit_size*0.2, true, 0, 0.2);
                        vector<float> down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.2, transition+bit_size*0.2, false, 0, 0.2);
                        if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                            valid = true;
                            transition = down_to_up[0];
                            if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(1);
                            if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(1);
                        }
                        else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                            valid = true;
                            transition = up_to_down[0];
                            if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(0);
                            if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(0);
                        }
                        else if (up_to_down.size() == 1 && down_to_up.size() == 1) {
                            valid = true;
                            if (abs(up_to_down[0]-transition) < abs(down_to_up[0]-transition)) {
                                transition = up_to_down[0];
                                if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(0);
                                if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(0);
                            }
                            else {
                                transition = down_to_up[0];
                                if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(1);
                                if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(1);
                            }
                        }
                        else if (up_to_down.size() == 0 && down_to_up.size() == 0) {
                            transition = transition + tune/num_reset_in_word;
                            up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.3, transition+bit_size*0.3, true, 0, 0.2); 
                            down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.3, transition+bit_size*0.3, false, 0, 0.2);
                            if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                                valid = true;
                                transition = down_to_up[0];
                                if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(1);
                                if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(1);
                            }
                            else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                                valid = true;
                                transition = up_to_down[0];
                                if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(0);
                                if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(0);
                            }
                            else if (up_to_down.size() == 1 && down_to_up.size() == 1) {
                                valid = true;
                                if (abs(up_to_down[0]-transition) < abs(down_to_up[0]-transition)) {
                                    transition = up_to_down[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(0);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(0);
                                }
                                else {
                                    transition = down_to_up[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(1);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(1);
                                }
                            }
                        }      
                        vector<float> transition_between_transition_UpToDown = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition_previous+0.04, transition-0.04, true, 0, 0.2); 
                        vector<float> transition_between_transition_DownToUp = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition_previous+0.04, transition-0.04, false, 0, 0.2);
                        if (transition_between_transition_UpToDown.size() == 0 && transition_between_transition_DownToUp.size() == 0)
                             has_transition_between_transition.push_back(false);
                        else {
                            has_transition_between_transition.push_back(true);
                            if ((transition_between_transition_UpToDown.size() != 0 && (transition_between_transition_UpToDown[0]-transition_previous)/(transition-transition_previous) < 0.4) || (transition_between_transition_DownToUp.size() != 0 && (transition_between_transition_DownToUp[0]-transition_previous)/(transition-transition_previous) < 0.4)) {
                                need_restart = true;
                                break;
                            }
                        }   
                        cout << "        " << transition << "," << endl;                     
                        transition_previous = transition;
                    }
                    if (!valid) {
                        cout << "Problems in bit " << bit_count-starting_bit << endl;
                        break;
                    }
                    bit_count += 1;
                }
                
                // Decode again in case the decoding is wrong due to inaccurate reset information
                if (need_restart) {
                    int lastFalseIndex = -1;
                    for (int i = 0; i < has_transition_between_transition.size(); i++) {
                        if (!has_transition_between_transition[i]) 
                            lastFalseIndex = i;
                    }
                    leading = {};
                    trailing = {};
                    bit_count = 0;
                    bit_start = manch_error[sigCtr];
                    bit_end = bit_start + bit_size;
                    
                    while(bit_count < 16+starting_bit) {
                        bool valid = false;
                        if (bit_end > clocktime->at(clocktime->size()-1)*1e9) break;
    
                        if (bit_count == 0) {
                            vector<float> up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], bit_start+bit_size*0.2, bit_end-bit_size*0.1, true, 0, 0.2); 
                            vector<float> down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], bit_start+bit_size*0.2, bit_end-bit_size*0.1, false, 0, 0.2);
                            if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                                valid = true;
                                transition_previous = down_to_up[0];
                            }
                            else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                                valid = true;
                                transition_previous = up_to_down[0];
                            }
                            else 
                                break;
                        }
                        else {
                            float transition = transition_previous + bit_size;
                            vector<float> up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.15, transition+bit_size*0.15, true, 0, 0.2);
                            vector<float> down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.15, transition+bit_size*0.15, false, 0, 0.2);
                            if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                                valid = true;
                                transition = down_to_up[0];
                                if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(1);
                                if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(1);
                            }
                            else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                                valid = true;
                                transition = up_to_down[0];
                                if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(0);
                                if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(0);
                            }
                            else if (up_to_down.size() == 1 && down_to_up.size() == 1) {
                                valid = true;
                                if (abs(up_to_down[0]-transition) < abs(down_to_up[0]-transition)) {
                                    transition = up_to_down[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(0);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(0);
                                }
                                else {
                                    transition = down_to_up[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(1);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(1);
                                }
                            }
                            else if (up_to_down.size() == 0 && down_to_up.size() == 0) {
                                transition = transition + tune/num_reset_in_word;
                                up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.25, transition+bit_size*0.25, true, 0, 0.2); 
                                down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition-bit_size*0.25, transition+bit_size*0.25, false, 0, 0.2);
                                if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                                    valid = true;
                                    transition = down_to_up[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(1);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(1);
                                }
                                else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                                    valid = true;
                                    transition = up_to_down[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(0);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(0);
                                }
                                else if (up_to_down.size() == 1 && down_to_up.size() == 1) {
                                    valid = true;
                                    if (abs(up_to_down[0]-transition) < abs(down_to_up[0]-transition)) {
                                        transition = up_to_down[0];
                                        if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(0);
                                        if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(0);
                                    }
                                    else {
                                        transition = down_to_up[0];
                                        if (bit_count >= starting_bit && bit_count < 8+starting_bit && bit_count != lastFalseIndex + 1) leading.push_back(1);
                                        if (bit_count >= starting_bit && bit_count >= 8+starting_bit && bit_count != lastFalseIndex + 1) trailing.push_back(1);
                                    }
                                }
                            }      
                            
                            if (bit_count == lastFalseIndex + 1) {
                                valid = false;
                                up_to_down = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition+0.04, transition+bit_size*0.8, true, 0, 0.2); 
                                down_to_up = times_cuts_zero(tree, evtctr, clocktime, decode_chs[decode_ch], transition+0.04, transition+bit_size*0.8, false, 0, 0.2);
                                if (up_to_down.size() == 0 && down_to_up.size() == 1) {
                                    valid = true;
                                    transition = down_to_up[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(1);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(1);
                                }
                                else if (up_to_down.size() == 1 && down_to_up.size() == 0) {
                                    valid = true;
                                    transition = up_to_down[0];
                                    if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(0);
                                    if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(0);
                                }
                                else if (up_to_down.size() == 1 && down_to_up.size() == 1) {
                                    valid = true;
                                    if (abs(up_to_down[0]-transition) < abs(down_to_up[0]-transition)) {
                                        transition = up_to_down[0];
                                        if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(0);
                                        if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(0);
                                    }
                                    else {
                                        transition = down_to_up[0];
                                        if (bit_count >= starting_bit && bit_count < 8+starting_bit) leading.push_back(1);
                                        if (bit_count >= starting_bit && bit_count >= 8+starting_bit) trailing.push_back(1);
                                    }
                                }
                            }
                            cout << transition << "," << endl;              
                            transition_previous = transition;
                        }
                        if (!valid) {
                            cout << "Problems in bit " << bit_count-starting_bit << endl;
                            break;
                        }
                        bit_count += 1;
                    }
                }
                
                
                // Output
                leading_decimal = vector_to_decimal(leading);
                trailing_decimal = vector_to_decimal(trailing);
                if (trailing_decimal <= leading_decimal && abs(trailing_decimal-leading_decimal) < 20) need_investigation = 1;
                
                cout << "Leading word: ";
                for (auto x:leading) {
                    cout << x;
                }
                cout << " -> " << leading_decimal << " -> " << leading_decimal*bit_size/16 << " ns" << endl;
                
                cout << "Trailing word: ";
                for (auto x:trailing) {
                    cout << x;
                }
                cout << " -> " << trailing_decimal << " -> " << trailing_decimal*bit_size/16 << " ns" << endl;
                
                if (leading_decimal == -1 || trailing_decimal == -1) {
                    cout << "Invalid decoding" << endl;
                    need_investigation_vec->push_back(1);
                    outside_range_vec->push_back(0);
                    leading7_vec->push_back(-1);
                    leading6_vec->push_back(-1);
                    leading5_vec->push_back(-1);
                    leading4_vec->push_back(-1);
                    leading3_vec->push_back(-1);
                    leading2_vec->push_back(-1);
                    leading1_vec->push_back(-1);
                    leading0_vec->push_back(-1);
                    trailing7_vec->push_back(-1);
                    trailing6_vec->push_back(-1);
                    trailing5_vec->push_back(-1);
                    trailing4_vec->push_back(-1);
                    trailing3_vec->push_back(-1);
                    trailing2_vec->push_back(-1);
                    trailing1_vec->push_back(-1);
                    trailing0_vec->push_back(-1);
                    leading_decimal_vec->push_back(-1);
                    trailing_decimal_vec->push_back(-1);
                    leading_ns_vec->push_back(-1);
                    trailing_ns_vec->push_back(-1);
                    time_width_vec->push_back(-1);
                    continue;
                }                                                                
                                
                time_width = (trailing_decimal - leading_decimal)*(bit_size/16);
                if (time_width < 0) {                
                    time_width = time_width + cycle*(bit_size/16);
                }
                if (time_width > 30) need_investigation = 1;  // Too long, probably something goes wrong
                cout << "Time = " << time_width << " ns " << endl;
                cout << "Need investigation: " << (need_investigation ? "true" : "false") << endl;

                need_investigation_vec->push_back(need_investigation);
                outside_range_vec->push_back(0);
                leading7_vec->push_back(leading[0]);  // Most significant bit
                leading6_vec->push_back(leading[1]);
                leading5_vec->push_back(leading[2]);
                leading4_vec->push_back(leading[3]);
                leading3_vec->push_back(leading[4]);
                leading2_vec->push_back(leading[5]);
                leading1_vec->push_back(leading[6]);
                leading0_vec->push_back(leading[7]);  // Least significant bit
                trailing7_vec->push_back(trailing[0]);
                trailing6_vec->push_back(trailing[1]);
                trailing5_vec->push_back(trailing[2]);
                trailing4_vec->push_back(trailing[3]);
                trailing3_vec->push_back(trailing[4]);
                trailing2_vec->push_back(trailing[5]);
                trailing1_vec->push_back(trailing[6]);
                trailing0_vec->push_back(trailing[7]);
                leading_decimal_vec->push_back(leading_decimal);
                trailing_decimal_vec->push_back(trailing_decimal);
                leading_ns_vec->push_back(leading_decimal*bit_size/16);
                trailing_ns_vec->push_back(trailing_decimal*bit_size/16);
                time_width_vec->push_back(time_width);
            }
        }
        
        
        // Read the signal channels
        vector<int> disc_signal_leading_candidates_valid = {};
        vector<int> disc_signal_trailing_candidates_valid = {};
        
        if (disc_signal_ch) {
            vector<int> disc_signal_leading_candidates = {};
            for (int k = 0; k < clocktime->size()-1 ; k++) {
                if (disc_signal_ch->at(k) <= disc_signal_threshold && disc_signal_ch->at(k+1) > disc_signal_threshold) {
                    disc_signal_leading_candidates.push_back(k);
                }
            }
 
            if (disc_signal_leading_candidates.empty()) continue;
            
            for (auto disc_signal_leading_candidate:disc_signal_leading_candidates) {
                vector<int> disc_signal_trailing_candidates = {};
                for (int k = disc_signal_leading_candidate; k < clocktime->size()-1 ; k++) {
                    if (disc_signal_ch->at(k) >= disc_signal_threshold && disc_signal_ch->at(k+1) < disc_signal_threshold) {
                        bool isValid = true;
                        for (int counter = disc_signal_leading_candidate+1; counter < k; counter++) {
                            if (disc_signal_ch->at(counter) < disc_signal_threshold) isValid = false;
                        }
                        if (isValid) disc_signal_trailing_candidates.push_back(k);
                    }
                }
                
                int disc_signal_trailing_candidate_valid = -1;
                for (auto disc_signal_trailing_candidate:disc_signal_trailing_candidates) {
                    float disc_signal_width = (clocktime->at(disc_signal_trailing_candidate)-clocktime->at(disc_signal_leading_candidate))*1e9;
                    if (disc_signal_width > 1 && disc_signal_width < 10) {
                         if (disc_signal_trailing_candidate_valid == -1 || disc_signal_trailing_candidate > disc_signal_trailing_candidate_valid)
                             disc_signal_trailing_candidate_valid = disc_signal_trailing_candidate;
                    }
                }
                if (disc_signal_trailing_candidate_valid != -1) {
                    disc_signal_leading_candidates_valid.push_back(disc_signal_leading_candidate);
                    disc_signal_trailing_candidates_valid.push_back(disc_signal_trailing_candidate_valid);
                }
            }
        }

        vector<int> injected_signal_leading_candidates_valid = {};
        vector<int> injected_signal_trailing_candidates_valid = {};
        
        if (injected_signal_ch) {
            vector<int> injected_signal_trailing_candidates = {};
            for (auto disc_signal_leading_candidate_valid:disc_signal_leading_candidates_valid) {
                for (int k = disc_signal_leading_candidate_valid; k > max(disc_signal_leading_candidate_valid-1000,0); k--) {
                    if (injected_signal_ch->at(k) >= injected_signal_threshold && injected_signal_ch->at(k-1) < injected_signal_threshold) {
                        injected_signal_trailing_candidates.push_back(k);
                    }
                }
            }
    
            if (injected_signal_trailing_candidates.empty()) continue;
    
            for (auto injected_signal_trailing_candidate:injected_signal_trailing_candidates) {
                vector<int> injected_signal_leading_candidates = {};
                for (int k = injected_signal_trailing_candidate; k > 0; k--) {
                    if (injected_signal_ch->at(k) <= injected_signal_threshold && injected_signal_ch->at(k-1) > injected_signal_threshold) {
                        bool isValid = true;
                        for (int counter = injected_signal_trailing_candidate-1; counter > k; counter--) {
                            if (injected_signal_ch->at(counter) > injected_signal_threshold) isValid = false;
                        }
                        if (isValid) injected_signal_leading_candidates.push_back(k);
                    }
                }
                int injected_signal_leading_candidate_valid = -1;
                for (auto injected_signal_leading_candidate:injected_signal_leading_candidates) {
                    float injected_signal_width = (clocktime->at(injected_signal_trailing_candidate)-clocktime->at(injected_signal_leading_candidate))*1e9;
                    if (injected_signal_width > 1 && injected_signal_width < 10) {
                        if (injected_signal_leading_candidate_valid == -1 || injected_signal_leading_candidate < injected_signal_leading_candidate_valid)
                            injected_signal_leading_candidate_valid = injected_signal_leading_candidate;
                    }
                }
                if (injected_signal_leading_candidate_valid != -1) {
                    injected_signal_leading_candidates_valid.push_back(injected_signal_leading_candidate_valid);
                    injected_signal_trailing_candidates_valid.push_back(injected_signal_trailing_candidate);
                }
            }
        }
        
        for (int i = 0; i < injected_signal_leading_candidates_valid.size(); i++) {
            injected_signal_leading_ns->push_back(clocktime->at(injected_signal_leading_candidates_valid[i])*1e9);
            injected_signal_trailing_ns->push_back(clocktime->at(injected_signal_trailing_candidates_valid[i])*1e9);
            injected_signal_time_width->push_back(clocktime->at(injected_signal_trailing_candidates_valid[i])*1e9-clocktime->at(injected_signal_leading_candidates_valid[i])*1e9);
        }

        for (int i = 0; i < disc_signal_leading_candidates_valid.size(); i++) {
            disc_signal_leading_ns->push_back(clocktime->at(disc_signal_leading_candidates_valid[i])*1e9);
            disc_signal_trailing_ns->push_back(clocktime->at(disc_signal_trailing_candidates_valid[i])*1e9);
            disc_signal_time_width->push_back(clocktime->at(disc_signal_trailing_candidates_valid[i])*1e9-clocktime->at(disc_signal_leading_candidates_valid[i])*1e9);
        }

        decoding_signal_match = ((disc_signal_time_width->size() == time_width_vec->size()) ? 1 : 0);

        bool reset_valid = true;
        for (auto reset:reset_global) {
            if (reset.size() < 2) {
                reset_valid = false;
                break;
            }
        }
        if (!clock_chs.empty() && disc_signal_ch) {
            if (reset_valid && decoding_signal_match) {
                int mancherr_count = 0;
                for (int i = 0; i < manch_err_global.size(); i++) {
                    for (int j = 0; j < manch_err_global[i].size(); j++) {
                        float closest = reset_global[i][0];
                        for (int k = 0; k < reset_global[i].size(); k++) {
                            if (abs(disc_signal_leading_ns->at(mancherr_count)-reset_global[i][k]) < abs(disc_signal_leading_ns->at(mancherr_count)-closest))
                                closest = reset_global[i][k];
                        }
                        if (closest > disc_signal_leading_ns->at(mancherr_count))
                            closest -= 50;
                        reset_discSignal_time_difference->push_back(abs(disc_signal_leading_ns->at(mancherr_count)-closest-duration_global/2));
                        mancherr_count += 1;
                    }
                }
            }
            else {
                for (int i = 0; i < disc_signal_time_width->size(); i++) {
                    reset_discSignal_time_difference->push_back(-1);
                }
            }
        }
        
        

        if (writeFile) {
            decode_tree->Fill();
        }
        
        need_investigation_vec->clear();
        outside_range_vec->clear();
        leading7_vec->clear(); leading6_vec->clear(); leading5_vec->clear(); leading4_vec->clear(); leading3_vec->clear(); leading2_vec->clear(); leading1_vec->clear(); leading0_vec->clear();
        trailing7_vec->clear(); trailing6_vec->clear(); trailing5_vec->clear(); trailing4_vec->clear(); trailing3_vec->clear(); trailing2_vec->clear(); trailing1_vec->clear(); trailing0_vec->clear();
        leading_decimal_vec->clear();
        trailing_decimal_vec->clear();
        leading_ns_vec->clear();
        trailing_ns_vec->clear();
        time_width_vec->clear();
        disc_signal_time_width->clear();
        disc_signal_leading_ns->clear();
        disc_signal_trailing_ns->clear();
        injected_signal_time_width->clear();
        injected_signal_leading_ns->clear();
        injected_signal_trailing_ns->clear();
        reset_discSignal_time_difference->clear();
    }
    
    if (writeFile) {
        toWrite_outputFile->cd();
        decode_tree->Write();
        toWrite_outputFile->Close();
    } 
    
    if (writeScreenOutput) {
        cout.rdbuf(coutbuf);
        outfile.close();
    }
}



int Main() {

    string merged_file_name = "merged_data.root";
    string treename = "tree";
    
    TFile *file = new TFile(merged_file_name.c_str(), "READ");
    if (!file->IsOpen() || file->IsZombie()) {
      cout << "Error: Cannot open file " << merged_file_name << endl;
    }
    TTree* tree = (TTree*)file->Get(treename.c_str());
    if (!tree) {
      cout << "Error: Cannot find " << treename << " in file " << merged_file_name << endl;
      cout << "Please check the tree name in 'read_root'." << endl;
    }
    
    int event_number;
    tree->SetBranchAddress("event_number", &event_number);
    vector<float>* clocktime = nullptr;
    tree->SetBranchAddress("time", &clocktime);
    vector<float>* decode_ch = nullptr;
    tree->SetBranchAddress("CH3", &decode_ch);
    vector<float>* injected_signal_ch = nullptr;
    tree->SetBranchAddress("CH2", &injected_signal_ch);
    vector<float>* clock_ch = nullptr;
    tree->SetBranchAddress("CH4", &clock_ch);
    vector<float>* disc_signal_ch = nullptr;
    tree->SetBranchAddress("CH1", &disc_signal_ch);
    
    tree->GetEntry(0);
    float start_ns = clocktime->at(0)*1e9;
    float end_ns = clocktime->at(clocktime->size()-1)*1e9;
    
    decode(tree, event_number, clocktime, {decode_ch}, injected_signal_ch, disc_signal_ch, "decode_output.root", start_ns, end_ns, {decode_ch}, true, true, true, 0.4, 0.007);
    
    file->Close();
    delete file;
    
    return 0;
}









/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include <interrupts.hpp>

int main(int argc, char **argv)
{

    // vectors is a C++ std::vector of strings that contain the address of the ISR
    // delays  is a C++ std::vector of ints that contain the delays of each device
    // the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;     //!< string to store single line of trace file
    std::string execution; //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/

    const int CONTEXT_SAVE = 20;
    const int CONTEXT_RESTORE = 10;
    const int ISR_ACTIVITY = 50;
    const int ISR_ADDRESS = 1;

    const int IRET = 1;
    const int SWITCHKERNEL = 1;

    int current_time = 0; // simulation clock

    /******************************************************************/

    // parse each line of the input trace file
    while (std::getline(input_file, trace))
    {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/

        // ---------------- CPU BURST ----------------
        if (activity == "CPU")
        {
            execution += std::to_string(current_time) + ", " +
                         std::to_string(duration_intr) + ", CPU burst\n";
            current_time += duration_intr;
        }

         // ---------- SYSCALL ----------
        else if (activity == "SYSCALL") {
            int dev = duration_intr;

            auto [intr_exec, new_time] = intr_boilerplate(current_time, dev, CONTEXT_SAVE, vectors);
            execution += intr_exec;
            current_time = new_time;

            execution += std::to_string(current_time) + ", " +
                         std::to_string(ISR_ACTIVITY) + ", ISR for device " + std::to_string(dev) + "\n";
            current_time += ISR_ACTIVITY;

            execution += std::to_string(current_time) + ", " +
                         std::to_string(CONTEXT_RESTORE) + ", context restored\n";
            current_time += CONTEXT_RESTORE;

        }

        // ---------- END_IO ----------
        else if (activity == "END_IO") {
            int dev = duration_intr;

            auto [intr_exec, new_time] = intr_boilerplate(current_time, dev, CONTEXT_SAVE, vectors);
            execution += intr_exec;
            current_time = new_time;

            execution += std::to_string(current_time) + ", " +
                         std::to_string(ISR_ACTIVITY) + ", ISR for END_IO device " + std::to_string(dev) + "\n";
            current_time += ISR_ACTIVITY;

            execution += std::to_string(current_time) + ", " +
                         std::to_string(CONTEXT_RESTORE) + ", context restored\n";
            current_time += CONTEXT_RESTORE;

          
        }

        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}

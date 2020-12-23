#include "rtmath.h"

namespace rt {
    // This must be unique to the thread to avoid stalling the thread
    // while another thread is using it.
    thread_local RNG<double, 0.0, 1.0> tls_double_unit_rng;

    double randDouble() {
        return tls_double_unit_rng.generate();
    }
}
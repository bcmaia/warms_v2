#include "Sim.hpp"

#include "utils/Term.hpp"

int main () {
    // bool 
    // std::string s;

    // try {

        uint64_t seed = 74670;
        sim::Sim simulation (seed);
        simulation.run();

    /*catch (const std::exception& e) {
        term::Term::instance().cleanup();
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        term::Term::instance().cleanup();
        std::cout << "Error: " << e.what() << std::endl;
    // } */ //catch (...) {
    //     // Catch any other unhandled exceptions and perform cleanup
    //     //term::Term::instance().cleanup();
    //     std::cout << "Unknown error occurred" << std::endl;
    //     here: goto here;

    // }

    return 0;
}
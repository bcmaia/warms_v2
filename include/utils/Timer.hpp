#pragma once


#include <chrono>
#include <thread>
#include <cstdint>
#include <string>
#include <iomanip> 


namespace timer {
    using duration_t = std::chrono::duration<double, std::ratio<1>>;

    class Timer {
        public:
            Timer() {
                start_time = std::chrono::high_resolution_clock::now();
                old_time = std::chrono::high_resolution_clock::now();
                new_time = std::chrono::high_resolution_clock::now();
            }

            double delta_time () const {
                duration_t duration = old_time - new_time;
                return duration.count();
            }

            double up_time () const {
                duration_t duration = new_time - start_time;
                return duration.count();
            }

            double mean_time() const {
                return up_time() / static_cast<double>(frame_count);
            }

            uint64_t get_frame_count() const {return frame_count;}

            void measure_time() {
                old_time = new_time;
                new_time = std::chrono::high_resolution_clock::now();
            }

            void start_measurement () {
                old_time = std::chrono::high_resolution_clock::now();
            }

            void end_measurement () {
                new_time = std::chrono::high_resolution_clock::now();
                frame_count++;
            }

            void sync (const double min_delta_time) const {
                double sleep_time = min_delta_time - delta_time();
                if (0.0 >= sleep_time) return;
                std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
            }

            std::string to_str() const {
                // Calculate the FPS
                double fps = 1.0 / delta_time();
                
                // Set the precision to 2 decimal places
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << fps;
                std::string fps_str = ss.str();

                std::stringstream ss1;
                ss1 << std::fixed << std::setprecision(1) << up_time();
                std::string up_time_str = ss1.str();

                // Format the timer data as a string
                return (
                    "<delta-time: " + std::to_string(delta_time()) +
                    ", mean-time: " + std::to_string(mean_time()) +
                    ", up-time: " + up_time_str +
                    ", fps: " + fps_str + // Use the formatted FPS string
                    ", frame-amount: " + std::to_string(frame_count) + ">"
                );
            }

        private:
            std::chrono::high_resolution_clock::time_point start_time;
            std::chrono::high_resolution_clock::time_point old_time;
            std::chrono::high_resolution_clock::time_point new_time;

            uint64_t frame_count = 0;
    };
}
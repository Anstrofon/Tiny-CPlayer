#include <chrono>
#include <string>

class Timer {
public:

    Timer()
    {

    };

    void reset() {
        is_running = true;
        accumulated_time = 0;
        start_time = std::chrono::high_resolution_clock::now();
    }

    void pause() {
        if (is_running) {
            auto current_time = std::chrono::high_resolution_clock::now();
            accumulated_time += std::chrono::duration<double>(current_time - start_time).count();
            is_running = false;
        }
    }

    void resume() {
        if (!is_running) {
            start_time = std::chrono::high_resolution_clock::now();
            is_running = true;
        }
    }

    void add_seconds(double seconds) {
        if (is_running) 
        {
            // Якщо таймер працює, оновлюємо accumulated_time з урахуванням поточного часу
            auto current_time = std::chrono::high_resolution_clock::now();
            accumulated_time += std::chrono::duration<double>(current_time - start_time).count();
            start_time = current_time; // Оновлюємо start_time
        }
        accumulated_time += seconds; // Додаємо задані секунди
    }

    double elapsedSeconds() const {
        if (is_running) {
            auto current_time = std::chrono::high_resolution_clock::now();
            return accumulated_time + std::chrono::duration<double>(current_time - start_time).count();
        } else {
            return accumulated_time;
        }
    }

    double elapsedMinutes() const {
        return elapsedSeconds() / 60.0;
    }

    std::string printElapsedTime(bool& first_run) const {
        if (first_run) {
            return "00:00";
        }
        double seconds = elapsedSeconds();
        int minutes = static_cast<int>(seconds / 60);
        int seconds_d = static_cast<int>(seconds) % 60;

        std::string result;
        if(minutes < 10)
        {
            result = "0" + std::to_string(minutes) + ":";
        }
        else 
        {
            result = std::to_string(minutes) + ":";
        }

        if(seconds_d < 10)
        {
            result += "0" + std::to_string(seconds_d);
        }
        else 
        {
            result += std::to_string(seconds_d);
        }
        
        return result; //std::to_string(minutes) + ":" + std::to_string(seconds_d);
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    double accumulated_time = 0; // Час, накопичений перед паузою
    bool is_running = true; // Стан таймера: працює чи на паузі
};

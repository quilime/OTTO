#include <atomic>
#include <condition_variable>
#include "application.hpp"

#include "services/audio_manager.hpp"
#include "services/engine_manager.hpp"
#include "services/log_manager.hpp"
#include "services/preset_manager.hpp"
#include "services/state_manager.hpp"
#include "services/ui_manager.hpp"
#include "services/clock_manager.hpp"

namespace otto::services {

  Application::Application(ServiceStorage<LogManager>::Factory log_fact,
                           ServiceStorage<StateManager>::Factory state_fact,
                           ServiceStorage<PresetManager>::Factory preset_fact,
                           ServiceStorage<AudioManager>::Factory audio_fact,
                           ServiceStorage<ClockManager>::Factory clock_fact,
                           ServiceStorage<UIManager>::Factory ui_fact,
                           ServiceStorage<EngineManager>::Factory engine_fact)
    : log_manager(std::move(log_fact)),
      state_manager(std::move(state_fact)),
      preset_manager(std::move(preset_fact)),
      audio_manager(std::move(audio_fact)),
      clock_manager(std::move(clock_fact)),
      ui_manager(std::move(ui_fact)),
      engine_manager(std::move(engine_fact))
  {
    _current = this;
    events.post_init.fire();
  }

  Application::~Application()
  {
    events.pre_exit.fire();
  }

  void Application::exit(ErrorCode ec) noexcept
  {
    _is_running = false;
    _error_code = ec;
    LOGI("Exit called with code {}", util::underlying(ec));
  }

  bool Application::running() noexcept
  {
    return _is_running;
  }

  Application::ErrorCode Application::error() noexcept
  {
    return _error_code;
  }

  void Application::handle_signal(int signal) noexcept
  {
    Application::current()._is_running = false;
    Application::current()._error_code = ErrorCode::signal_recieved;
  }

  Application& Application::current() noexcept
  {
    return static_cast<Application&>(*_current);
  }
} // namespace otto::services

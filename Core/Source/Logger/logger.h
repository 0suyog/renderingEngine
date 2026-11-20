#pragma once

#include <filesystem>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

inline std::string_view relativeFile(std::string_view path) {
#ifdef ENGINE_CODE_SOURCE_DIR
  constexpr std::string_view root = ENGINE_CODE_SOURCE_DIR;
  if (path.starts_with(root)) {
    path.remove_prefix(root.size());
    while (!path.empty() && (path.front() == '/' || path.front() == '\\'))
      path.remove_prefix(1);
  }
#endif
  return path;
}

enum class LOGLEVEL { LOG, INFO, DEBUG, WARN, ERROR };

class Logger {
public:
  Logger() : m_out(&std::cout), m_errout(&std::cerr), m_owns(false) {}

  Logger(std::ostream *outStream, std::ostream *errStream)
      : m_out(outStream), m_errout(errStream), m_owns(false) {}

  explicit Logger(std::ostream *outStream) : Logger(outStream, outStream) {}

  Logger(const std::filesystem::path &outPath,
         const std::filesystem::path &errPath)
      : m_out(new std::ofstream(outPath)), m_errout(new std::ofstream(errPath)),
        m_owns(true), stripColorEscapeCode(true) {}

  explicit Logger(const std::filesystem::path &path)
      : m_out(new std::ofstream(path)), m_errout(m_out), m_owns(true),
        stripColorEscapeCode(true) {}

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  ~Logger() {
    if (m_owns) {
      if (m_errout == m_out) {
        delete m_out;
      } else {
        delete m_out;
        delete m_errout;
      }
    }
  }

  // Global logger used by the LOG/INFO/DEBUG/WARN/ERROR macros.
  static Logger &global() {
    static Logger instance;
    return instance;
  }

  template <typename... T>
  void print(LOGLEVEL ll, const std::source_location &loc,
             fmt::format_string<T...> str, T &&...args) {
    write(ll, loc, false, fmt::format(str, std::forward<T>(args)...));
  }

  template <typename... T>
  void println(LOGLEVEL ll, const std::source_location &loc,
               fmt::format_string<T...> str, T &&...args) {
    write(ll, loc, true, fmt::format(str, std::forward<T>(args)...));
  }

  template <typename... T>
  void log(const std::source_location &loc, fmt::format_string<T...> str,
           T &&...args) {
    println(LOGLEVEL::LOG, loc, str, std::forward<T>(args)...);
  }

  template <typename... T>
  void info(const std::source_location &loc, fmt::format_string<T...> str,
            T &&...args) {
    println(LOGLEVEL::INFO, loc, str, std::forward<T>(args)...);
  }

  template <typename... T>
  void warn(const std::source_location &loc, fmt::format_string<T...> str,
            T &&...args) {
    println(LOGLEVEL::WARN, loc, str, std::forward<T>(args)...);
  }

  template <typename... T>
  void error(const std::source_location &loc, fmt::format_string<T...> str,
             T &&...args) {
    println(LOGLEVEL::ERROR, loc, str, std::forward<T>(args)...);
  }

  template <typename... T>
  void debug(const std::source_location &loc, fmt::format_string<T...> str,
             T &&...args) {
    println(LOGLEVEL::DEBUG, loc, str, std::forward<T>(args)...);
  }

  bool stripColorEscapeCode = false;

private:
  void write(LOGLEVEL ll, const std::source_location &loc, bool newline,
             std::string msg) {
    std::ostream *out = m_out;
    fmt::terminal_color color = fmt::terminal_color::white;

    switch (ll) {
    case LOGLEVEL::LOG:
      color = fmt::terminal_color::white;
      msg = "[LOG] " + msg;
      break;
    case LOGLEVEL::INFO:
      color = fmt::terminal_color::blue;
      msg = "[INFO] " + msg;
      break;
    case LOGLEVEL::DEBUG:
      color = fmt::terminal_color::bright_cyan;
      msg = "[DEBUG] " + msg;
      break;
    case LOGLEVEL::WARN:
      out = m_errout;
      msg = "[WARN] " + msg;
      color = fmt::terminal_color::bright_yellow;
      break;
    case LOGLEVEL::ERROR:
      out = m_errout;
      msg = "[ERROR] " + msg;
      color = fmt::terminal_color::bright_red;
      break;
    }

    std::string line =
        fmt::format("{}:{} {}", relativeFile(loc.file_name()), loc.line(), msg);
    if (newline)
      line += '\n';
    if (stripColorEscapeCode) {
      fmt::print(*out, "{}", line);
    } else {
      fmt::print(*out, "{}", fmt::styled(line, fmt::fg(color)));
    }
  }

  std::ostream *m_out;
  std::ostream *m_errout;
  bool m_owns = true;
};

#ifndef LOG
#define LOG(...)                                                               \
  Logger::global().log(std::source_location::current(), __VA_ARGS__)
#endif
#ifndef LOG_INFO
#define LOG_INFO(...)                                                          \
  Logger::global().info(std::source_location::current(), __VA_ARGS__)
#endif
#ifndef LOG_DEBUG
#define LOG_DEBUG(...)                                                         \
  Logger::global().debug(std::source_location::current(), __VA_ARGS__)
#endif
#ifndef LOG_WARN
#define LOG_WARN(...)                                                          \
  Logger::global().warn(std::source_location::current(), __VA_ARGS__)
#endif
#ifndef LOG_ERROR
#define LOG_ERROR(...)                                                         \
  Logger::global().error(std::source_location::current(), __VA_ARGS__)
#endif

#define LOG_TO(logger, ...)                                                    \
  (logger).log(std::source_location::current(), __VA_ARGS__)
#define LOG_INFO_TO(logger, ...)                                               \
  (logger).info(std::source_location::current(), __VA_ARGS__)
#define LOG_DEBUG_TO(logger, ...)                                              \
  (logger).debug(std::source_location::current(), __VA_ARGS__)
#define LOG_WARN_TO(logger, ...)                                               \
  (logger).warn(std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR_TO(logger, ...)                                              \
  (logger).error(std::source_location::current(), __VA_ARGS__)

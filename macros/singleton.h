#pragma once

#define DISALLOW_COPY_AND_ASSIGN(classname)         \
  classname(const classname&) = delete;             \
  classname& operator=(const classname&) = delete;

#define DECLARE_SINGLETON(classname)                         \
 public:                                                     \
  static classname* Instance(bool create_if_needed = true) { \
    static classname* instance = nullptr;                    \
    static std::once_flag flag;                              \
    std::call_once(flag, [create_if_needed] {                \
      if (create_if_needed) {                                \
        instance = new (std::nothrow) classname();           \
      }                                                      \
    });                                                      \
    return instance;                                         \
  }                                                          \
                                                             \
 private:                                                    \
  classname() = default;                                     \
  DISALLOW_COPY_AND_ASSIGN(classname)

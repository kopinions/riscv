#ifndef COMMAND_LINE_HPP
#define COMMAND_LINE_HPP

#include <uvm>
#include <string>
#include <algorithm>
#include <array>
 
class command_line_argument {
 public:
  using callback = void (*)(const std::string& arg);

  command_line_argument(const std::string argument_name, callback argument_callback)
    noexcept: m_name{argument_name}, m_callback{argument_callback} {};

  bool match(const std::string &argument_name) const noexcept {
    return argument_name.rfind(m_name, 0) == 0;
  };

  void operator()(const std::string& arg) const {
    m_callback(arg.substr(m_name.size(), arg.size() - m_name.size()));
  };

 private:
  const std::string m_name;
  callback m_callback;
};


using verbosity_item = std::pair<uvm::uvm_verbosity, std::string>;

static const std::array<verbosity_item, 5> g_verbosity{{
    {uvm::UVM_NONE,     "UVM_NONE"},
    {uvm::UVM_LOW,      "UVM_LOW"},
    {uvm::UVM_MEDIUM,   "UVM_MEDIUM"},
    {uvm::UVM_HIGH,     "UVM_HIGH"},
    {uvm::UVM_FULL,     "UVM_FULL"}
}};

static auto
split_3(const std::string& arg) -> std::array<std::string, 3> {
    auto pos1 = arg.find(',');
    if (pos1 == std::string::npos) {
        throw std::runtime_error(arg + " invalid format");
    }

    auto pos2 = arg.find(',', pos1 + 1);
    if (pos2 == std::string::npos) {
        throw std::runtime_error(arg + " invalid format");
    }

    return {{
        arg.substr(0, pos1),
        arg.substr(pos1 + 1, pos2 - pos1 - 1),
        arg.substr(pos2 + 1)
    }};
}

static const std::array<command_line_argument, 4> g_argument{{
    {
        "+UVM_TESTNAME=", [] (const std::string& arg) {
            uvm::uvm_factory::get()->create_component_by_name(
                    arg, "", "uvm_test_top");
        },
    },
    {
        "+UVM_VERBOSITY=", [] (const std::string& arg) {
            auto it = std::find_if(g_verbosity.cbegin(), g_verbosity.cend(),
                [&arg] (const verbosity_item& item) {
                    return (arg == item.second);
                }
            );

            if (g_verbosity.cend() != it) {
                uvm::uvm_set_verbosity_level(it->first);
            }
            else {
                throw std::runtime_error(arg + " invalid verbosity level");
            }
        },
    },
    {
        "+uvm_set_config_string=", [] (const std::string& arg) {
            auto value = split_3(arg);
            uvm::uvm_set_config_string(value[0], value[1], value[2]);
        }
    },
    {
        "+uvm_set_config_int=", [] (const std::string& arg) {
            auto value = split_3(arg);
            uvm::uvm_set_config_int(value[0], value[1], std::stoi(value[2]));
        }
    }
}};

  
class command_line {
public:
  command_line(int argc, char* argv[]) {
     if ((argc < 2) || (nullptr == argv)) {
        return;
    }

    --argc;
    ++argv;

    for (int i = 0; i < argc; ++i) {
      auto arg = std::string(argv[i]);

        auto it = std::find_if(g_argument.cbegin(), g_argument.cend(),
            [arg] (const command_line_argument& argument) {
                return argument.match(arg);
            }
        );

        if (it != g_argument.cend()) {
	  (*it)(arg);
        }
    }
  }
};

#endif /* COMMAND_LINE_HPP */

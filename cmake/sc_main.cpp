#include <sysc/kernel/sc_except.h>
#include <sysc/kernel/sc_externs.h>
#include <sysc/utils/sc_report.h>
#include <sysc/utils/sc_utils_ids.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

static int g_argc = 0;
static char** g_argv = nullptr;

namespace sc_core {
void pln();
}

int sc_core::sc_argc() { return g_argc; }

const char* const* sc_core::sc_argv() { return g_argv; }

int sc_core::sc_elab_and_sim(int argc, char* argv[]) {
  int status = EXIT_FAILURE;
  g_argc = argc;
  g_argv = argv;
  std::vector<char*> argv_call;

  for (int i = 0; i < argc; i++) {
    argv_call.push_back(argv[i]);
  }

  try {
    pln();

    status = sc_main(argc, &argv_call[0]);
  } catch (const sc_report& report) {
    std::cout << report.what() << std::endl;
  } catch (...) {
    std::unique_ptr<sc_report> err_p{sc_handle_exception()};
    if (err_p) {
      std::cout << err_p->what() << std::endl;
    }
  }

  if (sc_report_handler::get_count(SC_ID_IEEE_1666_DEPRECATION_) > 0) {
    std::stringstream ss;

#define MSGNL "\n             "
#define CODENL "\n  "

    ss << "You can turn off warnings about" MSGNL "IEEE 1666 deprecated features by placing this method call" MSGNL
          "as the first statement in your sc_main() function:\n" CODENL
          "sc_core::sc_report_handler::set_actions( "
          "\""
       << SC_ID_IEEE_1666_DEPRECATION_
       << "\"," CODENL
          "                                         " /* indent param */
          "sc_core::SC_DO_NOTHING );"
       << std::endl;

    SC_REPORT_INFO(SC_ID_IEEE_1666_DEPRECATION_, ss.str().c_str());
  }

  return status;
}

int main(int argc, char* argv[]) { return sc_core::sc_elab_and_sim(argc, argv); }

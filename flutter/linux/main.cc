#include <dlfcn.h>
#include "my_application.h"

#define RUSTDESK_LIB_PATH "libirisremote.so"
// #define RUSTDESK_LIB_PATH "/usr/lib/irisremote/libirisremote.so"
typedef bool (*IrisRemoteCoreMain)();
bool gIsConnectionManager = false;

bool flutter_irisremote_core_main() {
   void* libirisremote = dlopen(RUSTDESK_LIB_PATH, RTLD_LAZY);
   if (!libirisremote) {
     fprintf(stderr,"load libirisremote.so failed\n");
     return true;
   }
   auto core_main = (IrisRemoteCoreMain) dlsym(libirisremote,"irisremote_core_main");
   char* error;
   if ((error = dlerror()) != nullptr) {
       fprintf(stderr, "error finding irisremote_core_main: %s", error);
       return true;
   }
   return core_main();
}

int main(int argc, char** argv) {
  if (!flutter_irisremote_core_main()) {
      return 0;
  }
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--cm") == 0) {
      gIsConnectionManager = true;
    }
  }
  g_autoptr(MyApplication) app = my_application_new();
  return g_application_run(G_APPLICATION(app), argc, argv);
}

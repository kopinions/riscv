
#include <systemc>
	int sc_main(int argc, char* argv[]) {
	#if (IEEE_1666_SYSTEMC >= 201101L)
	return 0;
	#endif
	return 1;
	}

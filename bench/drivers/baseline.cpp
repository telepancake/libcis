// baseline.cpp - empty program, no library exercised.
//
// Establishes the fixed .text overhead that every libcis binary carries
// (src/support.cpp runtime glue + whatever the freestanding link pulls in).
// Subtract this from a project's number to read its marginal code size.
int main() { return 0; }

#ifndef RAND64_SW_H
#define RAND64_SW_H

void setFile(char *name);
void software_rand64_init (void);
unsigned long long software_rand64 (void);
void software_rand64_fini (void);

#endif

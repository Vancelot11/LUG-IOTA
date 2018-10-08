#include"header.h"

void init(struct GGA*);
void printGGA(struct GGA*);
void getDat(struct GGA*, char[]);
void finalize(struct GGA*);

int main()
{
	struct GGA ggaDat;
	char buf[BUFSIZE];

	init(&ggaDat);
	getDat(&ggaDat, buf);
	finalize(&ggaDat);
}

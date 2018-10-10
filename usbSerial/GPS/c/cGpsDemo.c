#include"header.h"

void init(struct GGA*);
void printGGA(struct GGA*);
void getDat(struct GGA*);
void finalize(struct GGA*);

int main()
{
	struct GGA ggaDat;

	init(&ggaDat);
	for(;;)
	{
		getDat(&ggaDat);
		printGGA(&ggaDat);
	}
	finalize(&ggaDat);
}

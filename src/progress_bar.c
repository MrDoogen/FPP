#include <stdio.h>
#include <unistd.h>

void progress_bar (int done_progress, int total_progress) {
	char line[] = "[----------------------------------------]";
	const float size_line = 40;
	const float step = 100 / size_line;
	float percent = done_progress * 100 / total_progress;
	int done_steps = percent / step;

	for (int i = 1; i <= done_steps; i++) {
		line[i] = '=';
	}

	printf("%s", line);
	printf(" %0.f%%\r", percent);

	if (done_steps == size_line) {
		printf("\n");
	}

	fflush(stdout);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME    "ws2txt"
#define VERSION "1.0.2"

static void usage(char *exec) {
	int length = 0;

	length = (int)strlen(exec);

	printf("%s (v%s)\n\n", NAME, VERSION);
	printf("Usage: %s <in.doc> [<out.txt>]\n", exec);
	printf("%*s [-h,--help]\n", length + 7, " ");
}

int main(int argc, char *argv[]) {
	int rc = 0, i = 0, ch = 0, echo = 1;
	size_t input_size = 0, output_size = 0;
	char *exec = NULL;
	char *input_filename = NULL, *output_filename = NULL;
	char *input_text = NULL, *output_text = NULL;
	FILE *input = NULL, *output = NULL;

	exec = argv[0];

	if (argc < 2) {
		usage(exec);

		rc = 1;
		goto cleanup;
	}

	if (argc > 2) {
		echo = 0;
	}

	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		usage(exec);

		rc = 1;
		goto cleanup;
	}

	input_filename = argv[1];
	input = fopen(input_filename, "rb");

	if (!input) {
		printf("Could not open %s\n", input_filename);

		rc = 1;
		goto cleanup;
	}

	(void)fseek(input, 0, SEEK_END);
	input_size = (size_t)ftell(input);
	(void)fseek(input, 0, SEEK_SET);

	if (input_size == 0) {
		printf("%s is empty\n", input_filename);

		rc = 1;
		goto cleanup;
	}

	input_text = malloc(sizeof(char) * (input_size + 1));
	output_text = malloc(sizeof(char) * (input_size + 1));

	if (!input_text || !output_text) {
		printf("Memory error\n");

		rc = 1;
		goto cleanup;
	}

	if (fread(input_text, 1, input_size, input) != input_size) {
		printf("Could not read %s\n", input_filename);

		rc = 1;
		goto cleanup;
	}

	for (i = 0, output_size = 0; i < (int)input_size; i++) {
		ch = (int)input_text[i];

		if (ch == 26) {
			continue;
		}

		if (ch < 0) {
			ch = (ch + 256) % 128;
		}

		output_text[output_size++] = (char)ch;
	}

	if (echo == 0) {
		output_filename = argv[2];
		output = fopen(output_filename, "w+");

		if (!output) {
			printf("Could not open %s\n", output_filename);

			rc = 1;
			goto cleanup;
		}
	} else {
		output = stdout;
	}

	if (fwrite(output_text, 1, output_size, output) != output_size) {
		printf("Could not write %s\n", output_filename);

		rc = 1;
		goto cleanup;
	}

	if (echo != 0 && output != NULL) {
		printf("\n");
	} else {
		printf("Wrote file %s\n", output_filename);
	}

cleanup:
	if (input) {
		(void)fclose(input);
	}

	if (output) {
		(void)fclose(output);
	}

	if (input_text) {
		free(input_text);
	}

	if (output_text) {
		free(output_text);
	}

	return rc;
}
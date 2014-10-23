/*
 * Utility for converting WordStar 4.0 .DOC files to plaintext
 *
 * Copyright (C) 2014, Kevin Selwyn <kevinselwyn at gmail dot com>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#if !defined(__APPLE__)
	#include <malloc.h>
#endif

#define VERSION "1.0.1"

int main(int argc, char *argv[]) {
	int rc = 0, i = 0, ch = 0, echo = 1;
	size_t input_size = 0, output_size = 0;
	char *input_filename = NULL, *output_filename = NULL;
	char *input_text = NULL, *output_text = NULL;
	FILE *input = NULL, *output = NULL;

	if (argc < 2) {
		printf("ws2txt (v%s)\n\nUsage: ws2txt <in.doc> <out.txt>\n", VERSION);
		rc = 1;
		goto cleanup;
	}

	if (argc > 2) {
		echo = 0;
	}

	input_filename = argv[1];
	input = fopen(input_filename, "r");

	if (!input) {
		printf("Could not open input %s\n", input_filename);
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

	input_text = malloc(sizeof(char) * input_size);
	output_text = malloc(sizeof(char) * input_size);

	if (!input_text || !output_text) {
		printf("Memory error\n");
		rc = 1;
		goto cleanup;
	}

	if (fread(input_text, 1, input_size, input) != input_size) {
		printf("Unable to read %s\n", input_filename);
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
			printf("Could not open output %s\n", output_filename);
			rc = 1;
			goto cleanup;
		}
	} else {
		output = stdout;
	}

	if (fwrite(output_text, 1, output_size, output) != output_size) {
		printf("Error writing %s\n", output_filename);
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

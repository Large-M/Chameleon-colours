/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

/* magic to return the ratio between two numbers */
#define RATIO(a,b) (((a)>(b) ? 1.0*(b)/(a) : ((a) > 0 ? 1.0*(a)/(b) : 0)))

struct color_t {
	char   hex[8];
	int    rgb[3];
	double val;
	struct color_t *next;
};

struct color_t *head = NULL;
struct color_t **sorted = NULL;
struct color_t **messed = NULL;

void
usage(char *argv0) {
	fprintf(stderr, "usage: %s [mask]\n", argv0);
}

/*
 * converts an hexadecimal representation of a color into a 3 dimensionnal
 * array (RGB decomposition)
 */
void
hex2rgb(char *hex, int *rgb)
{
	int i;
	char tmp[2];
	for (i = 0; i < 3; i++) {
		strncpy(tmp, hex + 1 + 2 * i, 2);
		rgb[i] = strtol(tmp, NULL, 16);
	}
}

/*
 * Returns the "ratio" of the negative color for an RGB representation.
 *
 * For mask 4 (100)b which is red. It will return the purity ratio of its
 * negative color: cyan (#00ffff). For example:
 *
 * #ff2d2d  255,45,45  45/45 = 1.0 (this is a "full" cyan)
 * #ff3c1e  255,6°,30  60/30 = 0.5 (this is a "half" cyan)
 */
double
color_ratio(int *rgb, uint8_t mask)
{
	switch(mask) {
	case 4: return RATIO(rgb[1],rgb[2]);
	case 2: return RATIO(rgb[0],rgb[2]);
	case 1: return RATIO(rgb[0],rgb[1]);
	case 3: return RATIO(rgb[1],rgb[2]);
	case 5: return RATIO(rgb[0],rgb[2]);
	case 6: return RATIO(rgb[0],rgb[1]);
	}
	return 0;
}

/*
 * Calculate a score for the dominance of the color given by `mask` (3 bit
 * representation of the RGB color, eg: 101 is magenta).
 * This will calculate the score of the color regarding the mask, and return
 * this value for comparison with other colors. "pure" refers to the color
 * represented by the mask, "negative" refers to the opposite of this color.
 *
 * We differentiate 3 types of masks:
 * + 100, 010, 001
 * + 110, 101, 011
 * + 111, 000
 *
 * First group will be the pure color, divided by the negative ratio.
 * Second group will be the pure color multiplied by the pure ratio.
 * Last group is just a plain sum of all colors.
 *
 * (Check function color_ratio() for an explanation on the ratio)
 */
double
color_dominant(int *rgb, uint8_t mask)
{
	double pure = 0, negative = 0, ratio = 0;

	int prgb[3];
	int nrgb[3];

	/* get the score of the current value */
	prgb[0] = rgb[0] * ((mask & 4)>>2);
	prgb[1] = rgb[1] * ((mask & 2)>>1);
	prgb[2] = rgb[2] * ((mask & 1)>>0);

	/* score of the negative value */
	nrgb[0] = rgb[0] * (((mask ^ 4)>>2)&1);
	nrgb[1] = rgb[1] * (((mask ^ 2)>>1)&1);
	nrgb[2] = rgb[2] * (((mask ^ 1)>>0)&1);

	/*
	 * calculate the average of either pure or negative color depending on
	 * the mask's value
	 */
	switch (mask) {
	case 1:
	case 2:
	case 4:
		pure     =  prgb[0] + prgb[1] + prgb[2];
		negative = (nrgb[0] + nrgb[1] + nrgb[2])/2;
		ratio    = 1 + negative * color_ratio(nrgb, mask);
		return ((pure - negative) / ratio);
	case 3:
	case 5:
	case 6:
		pure     = (prgb[0] + prgb[1] + prgb[2])/2;
		negative = nrgb[0] + nrgb[1] + nrgb[2];
		ratio    = 1 + pure * color_ratio(prgb, mask);
		return ((pure - negative) * ratio);
	}

	pure     = prgb[0] + prgb[1] + prgb[2];
	negative = nrgb[0] + nrgb[1] + nrgb[2];
	return pure - negative;
}

/* create a color node, and add it to the list */
struct color_t *
color_new(char *hex, uint8_t mask)
{
	struct color_t *new = NULL;

	new = malloc(sizeof(struct color_t));
	if (new == NULL)
		return NULL;

	strncpy(new->hex, hex, 8);
	hex2rgb(hex, new->rgb);
	new->val = color_dominant(new->rgb, mask);
	return new;
}

/*
 * Takes an unsorted list of colors as an argument, and sort it depending on
 * the mask value.
 * The mask is a 3 bit representation of the RGB composition you want to use to
 * sort colors, eg mask 011 will return the brightess cyan first, and darkest
 * red last.
 */
struct color_t *
color_sort(struct color_t *cur, struct color_t *new, uint8_t mask)
{
	if (cur == NULL) {
		new->next = NULL;
	} else {
		if (new->val <= cur->val) {
			cur->next = color_sort(cur->next, new, mask);
			return cur;
		} else {
			new->next = cur;
		}
	}
	return new;
}

/*
 * print the content of our list in the format:
 * <HEX>	<RGB>	<SCORE>
 */
void
color_print(struct color_t *node)
{
	struct color_t *tmp = NULL;
	for (tmp=node; tmp; tmp=tmp->next) {
		printf("%s\t%d,%d,%d\t%f\n",
				tmp->hex,
				tmp->rgb[0],
				tmp->rgb[1],
				tmp->rgb[2],
				tmp->val );
	}
}

int
main(int argc, char *argv[])
{
	char hex[8];
	uint8_t mask = 7;

	/* print whitest by default */
	if (argc > 1) {
		if (strncmp(argv[1], "-h", 2) == 0)
			usage(argv[0]);
		else
			mask = atoi(argv[1]);
	}

	while (fgets(hex, 8, stdin)) {
		if (hex[0] == '#') {
			head = color_sort(head, color_new(hex, mask), mask);
		}
	}
	color_print(head);
	return 0;
}

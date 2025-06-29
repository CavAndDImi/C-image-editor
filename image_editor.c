// Cavescu Dumitru-Andrei, 312CA
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define NMAX 15
#define ADR_MAX 100

typedef struct
{
	int red;
	int green;
	int blue;
} pixel;

int clamp(double x, int min, int max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

void eliberare_spatiu_load(pixel ***mat, int height)
{
	if (*mat)
	{
		for (int i = 0; i < height; i++)
		{
			if ((*mat)[i])
				free((*mat)[i]);
			(*mat)[i] = NULL;
		}
		free(*mat);
		*mat = NULL;
	}
}

void eliberare_spatiu(int ***mat, int height)
{
	if (*mat)
	{
		for (int i = 0; i < height; i++)
		{
			if ((*mat)[i])
			{
				free((*mat)[i]);
				(*mat)[i] = NULL;
			}
		}
		free(*mat);
		*mat = NULL;
	}
}

int READ_DET(int *height, int *width, int *rgb, char type_p[3], FILE *loaded)
{
	// Scoate endline-ul din string
	type_p[strcspn(type_p, "\n")] = '\0';

	fseek(loaded, 3, SEEK_SET);
	fscanf(loaded, "%d", width);
	fscanf(loaded, "%d", height);
	fscanf(loaded, "%d", rgb);
	return 0;
}

int READ_PGM(int height, int width, int bin, FILE *loaded, int ***mat)
{
	*mat = (int **)malloc(height * sizeof(int *));

	// Programare defensiva
	if (!(*mat))
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
	{
		(*mat)[i] = (int *)malloc(width * sizeof(int));
		if ((*mat)[i] == NULL)
		{
			printf("Oh-oh... Nu a putut fi facuta citirea...\n");
			return 0;
		}
	}

	if (bin == 0)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				fscanf(loaded, "%d", &(*mat)[i][j]);
			}
		}
	}
	else
	{
		// Nu stiu si nu conteaza dc dar far e naspa
		unsigned char aux;
		fread(&aux, sizeof(unsigned char), 1, loaded);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				unsigned char valoare;
				fread(&valoare, sizeof(unsigned char), 1, loaded);
				(*mat)[i][j] = valoare;
			}
		}
	}
	return 0;
}

int SAVE_PGM(int height, int width, int rgb, FILE *loaded, int ***mat,
			 char ascii[6])
{
	if (ascii)
	{
		if (strcmp(ascii, "ascii") == 0)
		{
			fprintf(loaded, "P2\n");
			fprintf(loaded, "%d %d\n", width, height);
			fprintf(loaded, "%d\n", rgb);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					fprintf(loaded, "%d ", (*mat)[i][j]);
				}
				fprintf(loaded, "\n");
			}
		}
		else
		{
			fwrite("P5\n", sizeof(char), 3, loaded);
			fprintf(loaded, "%d %d\n%d\n", width, height, rgb);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					unsigned char value = (unsigned char)(*mat)[i][j];
					fwrite(&value, sizeof(unsigned char), 1, loaded);
				}
			}
		}
	}
	else
	{
		fwrite("P5\n", sizeof(char), 3, loaded);
		fprintf(loaded, "%d %d\n%d\n", width, height, rgb);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				unsigned char value = (unsigned char)(*mat)[i][j];
				fwrite(&value, sizeof(unsigned char), 1, loaded);
			}
		}
	}

	return 0;
}

int READ_PPM(int height, int width, int bin, FILE *loaded, pixel ***mat)
{
	*mat = (pixel **)malloc(height * sizeof(pixel *));

	// Programare defensiva
	if (!(*mat))
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
	{
		(*mat)[i] = (pixel *)malloc(width * sizeof(pixel));
		if ((*mat)[i] == NULL)
		{
			printf("Oh-oh... Nu a putut fi facuta citirea...\n");
			return 0;
		}
	}

	if (bin == 0)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				fscanf(loaded, "%d", &((*mat)[i][j].red));
				fscanf(loaded, "%d", &((*mat)[i][j].green));
				fscanf(loaded, "%d", &((*mat)[i][j].blue));
			}
		}
	}
	else
	{
		// Nu stiu si nu conteaza dc dar fara e naspa
		unsigned char aux;
		fread(&aux, sizeof(unsigned char), 1, loaded);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				unsigned char rgb[3];
				rgb[0] = (*mat)[i][j].red;
				rgb[1] = (*mat)[i][j].green;
				rgb[2] = (*mat)[i][j].blue;
				fread(rgb, sizeof(unsigned char), 3, loaded);
				(*mat)[i][j].red = rgb[0];
				(*mat)[i][j].green = rgb[1];
				(*mat)[i][j].blue = rgb[2];
			}
		}
	}
	return 0;
}

int SAVE_PPM(int height, int width, int rgb, FILE *loaded, pixel ***mat,
			 char ascii[6])
{
	if (ascii)
	{
		if (strcmp(ascii, "ascii") == 0)
		{
			fprintf(loaded, "P3\n");
			fprintf(loaded, "%d %d\n", width, height);
			fprintf(loaded, "%d\n", rgb);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					fprintf(loaded, "%d %d ", (*mat)[i][j].red, (*mat)[i][j].green);
					fprintf(loaded, "%d ", (*mat)[i][j].blue);
				}
				fprintf(loaded, "\n");
			}
		}
		else
		{
			fwrite("P6\n", sizeof(char), 3, loaded);
			fprintf(loaded, "%d %d\n%d\n", width, height, rgb);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					unsigned char rgb[3];
					rgb[0] = (*mat)[i][j].red;
					rgb[1] = (*mat)[i][j].green;
					rgb[2] = (*mat)[i][j].blue;
					fwrite(rgb, sizeof(unsigned char), 3, loaded);
				}
			}
		}
	}
	else
	{
		fwrite("P6\n", sizeof(char), 3, loaded);
		fprintf(loaded, "%d %d\n%d\n", width, height, rgb);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				unsigned char rgb[3];
				rgb[0] = (*mat)[i][j].red;
				rgb[1] = (*mat)[i][j].green;
				rgb[2] = (*mat)[i][j].blue;
				fwrite(rgb, sizeof(unsigned char), 3, loaded);
			}
		}
	}
	return 0;
}

FILE *Load(char adresa[ADR_MAX], FILE *loaded, int *bin, char test[3])
{
	if (loaded)
		fclose(loaded);
	FILE *out = fopen(adresa, "r");

	if (!out)
		return NULL;
	fgets(test, 3, out);

	// Scoate endline-ul din string
	test[strcspn(test, "\n")] = '\0';

	if (strcmp(test, "P5") == 0 || strcmp(test, "P6") == 0)
	{
		fclose(out);
		out = fopen(adresa, "rb");
		*bin = 1;
	}
	return out;
}

int ROTATE_PGM(int x_0, int y_0, int x_1, int y_1, int *height, int *width, int ***mat, int unghi)
{
	if ((x_1 - x_0) == (y_1 - y_0))
	{
		int n = x_1 - x_0;
		int **res = (int **)malloc(sizeof(int *) * n);

		// Programare defensiva
		if (!res)
		{
			printf("Oh-oh... Nu a putut fi facuta citirea...\n");
			return 0;
		}

		for (int i = 0; i < n; i++)
			res[i] = malloc(sizeof(int) * n);

		unghi /= 90;
		if (unghi < 0)
			unghi = 4 + unghi;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				res[i][j] = 0;
			}
		}

		for (int k = 0; k < unghi; k++)
		{
			for (int i = y_0; i < y_1; i++)
			{
				for (int j = x_0; j < x_1; j++)
				{
					res[j - x_0][y_1 - i - 1] = (*mat)[i][j];
				}
			}

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					(*mat)[i + y_0][j + x_0] = res[i][j];
				}
			}
		}

		eliberare_spatiu(&res, n);
	}
	else
	{
		unghi /= 90;
		if (unghi < 0)
			unghi = 4 + unghi;

		for (int k = 0; k < unghi; k++)
		{
			int **res = (int **)malloc(sizeof(int *) * x_1);

			// Programare defensiva
			if (!res)
			{
				printf("Oh-oh... Nu a putut fi facuta citirea...\n");
				return 0;
			}

			for (int i = 0; i < x_1; i++)
				res[i] = malloc(sizeof(int) * y_1);

			for (int i = 0; i < x_1; i++)
			{
				for (int j = 0; j < y_1; j++)
				{
					res[i][j] = (*mat)[y_1 - j - 1][i];
				}
			}

			eliberare_spatiu(mat, y_1);

			*mat = (int **)malloc(x_1 * sizeof(int *));

			// Programare defensiva
			if (!(*mat))
			{
				printf("Oh-oh... Nu a putut fi facuta citirea...\n");
				return 0;
			}

			for (int i = 0; i < x_1; i++)
			{
				(*mat)[i] = (int *)malloc(y_1 * sizeof(int));
				if ((*mat)[i] == NULL)
				{
					printf("Oh-oh... Nu a putut fi facuta citirea...\n");
					return 0;
				}
			}

			for (int i = x_0; i < x_1; i++)
			{
				for (int j = y_0; j < y_1; j++)
				{
					(*mat)[i][j] = res[i][j];
				}
			}

			eliberare_spatiu(&res, x_1);

			int aux = y_1;
			y_1 = x_1;
			x_1 = aux;
			*height = y_1;
			*width = x_1;
		}
	}

	return 0;
}

int ROTATE_PPM(int x_0, int y_0, int x_1, int y_1, int *height, int *width,
			   pixel ***mat, int unghi)
{
	if ((x_1 - x_0) == (y_1 - y_0))
	{
		int n = x_1 - x_0;
		pixel **res = (pixel **)malloc(sizeof(pixel *) * n);

		// Programare defensiva
		if (!res)
		{
			printf("Oh-oh... Nu a putut fi facuta citirea...\n");
			return 0;
		}

		for (int i = 0; i < n; i++)
			res[i] = malloc(sizeof(pixel) * n);
		unghi /= 90;
		if (unghi < 0)
			unghi = 4 + unghi;

		for (int i = 0; i < unghi; i++)
		{
			for (int i = y_0; i < y_1; i++)
			{
				for (int j = x_0; j < x_1; j++)
				{
					res[j - x_0][y_1 - i - 1] = (*mat)[i][j];
				}
			}

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < n; j++)
				{
					(*mat)[i + y_0][j + x_0] = res[i][j];
				}
			}
		}

		eliberare_spatiu_load(&res, n);
	}
	else
	{
		unghi /= 90;
		if (unghi < 0)
			unghi = 4 + unghi;

		for (int k = 0; k < unghi; k++)
		{
			pixel **res = (pixel **)malloc(sizeof(pixel *) * x_1);

			// Programare defensiva
			if (!res)
			{
				printf("Oh-oh... Nu a putut fi facuta citirea...\n");
				return 0;
			}

			for (int i = 0; i < x_1; i++)
				res[i] = malloc(sizeof(pixel) * y_1);

			for (int i = 0; i < x_1; i++)
			{
				for (int j = 0; j < y_1; j++)
				{
					res[i][j] = (*mat)[y_1 - j - 1][i];
				}
			}

			eliberare_spatiu_load(mat, y_1);

			*mat = (pixel **)malloc(x_1 * sizeof(pixel *));

			// Programare defensiva
			if (!(*mat))
			{
				printf("Oh-oh... Nu a putut fi facuta citirea...\n");
				return 0;
			}

			for (int i = 0; i < x_1; i++)
			{
				(*mat)[i] = (pixel *)malloc(y_1 * sizeof(pixel));
				if ((*mat)[i] == NULL)
				{
					printf("Oh-oh... Nu a putut fi facuta citirea...\n");
					return 0;
				}
			}

			for (int i = x_0; i < x_1; i++)
			{
				for (int j = y_0; j < y_1; j++)
				{
					(*mat)[i][j] = res[i][j];
				}
			}

			eliberare_spatiu_load(&res, x_1);

			int aux = y_1;
			y_1 = x_1;
			x_1 = aux;
			*height = y_1;
			*width = x_1;
		}
	}

	return 0;
}

int CROP(int *x_0, int *y_0, int *x_1, int *y_1, int *height, int *width,
		 int ***mat)
{
	int x = *x_1 - *x_0;
	int y = *y_1 - *y_0;

	if (x < 0)
	{
		x = -x;
	}

	if (y < 0)
	{
		y = -y;
	}

	int **crop = (int **)malloc(sizeof(int *) * y);

	// Programare defensiva
	if (!crop)
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < y; i++)
	{
		crop[i] = (int *)malloc(sizeof(int) * x);
	}

	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			crop[i][j] = (*mat)[*y_0 + i][*x_0 + j];
		}
	}

	eliberare_spatiu(mat, *height);

	(*mat) = (int **)malloc(sizeof(int *) * y);
	for (int i = 0; i < y; i++)
	{
		(*mat)[i] = malloc(sizeof(int) * x);
	}

	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			(*mat)[i][j] = crop[i][j];
		}
	}

	eliberare_spatiu(&crop, y);

	*height = y;
	*width = x;

	*x_0 = 0;
	*x_1 = *width;
	*y_0 = 0;
	*y_1 = *height;

	return 0;
}

int CROP_LOAD(int *x_0, int *y_0, int *x_1, int *y_1, int *height, int *width,
			  pixel ***mat)
{
	int x = *x_1 - *x_0;
	int y = *y_1 - *y_0;

	if (x < 0)
	{
		x = -x;
	}

	if (y < 0)
	{
		y = -y;
	}

	pixel **new_mat = (pixel **)malloc(sizeof(pixel *) * y);

	for (int i = 0; i < y; i++)
	{
		new_mat[i] = (pixel *)malloc(sizeof(pixel) * x);
		for (int j = 0; j < x; j++)
		{
			new_mat[i][j] = (*mat)[*y_0 + i][*x_0 + j];
		}
	}

	eliberare_spatiu_load(mat, *height);

	*mat = new_mat;

	*height = y;
	*width = x;

	*x_0 = 0;
	*x_1 = *width;
	*y_0 = 0;
	*y_1 = *height;

	return 0;
}

int EQUALIZE(int height, int width, int ***mat)
{
	int **aux = (int **)malloc(sizeof(int *) * height);
	// Programare defensiva
	if (!aux)
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
	{
		aux[i] = (int *)malloc(width * sizeof(int));
		if (!aux[i])
		{
			printf("Oh-oh... Nu a putut fi facuta citirea...\n");
			return 0;
		}
	}

	double area = height * width;
	int apartitii[256] = {0};

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			apartitii[(*mat)[i][j]]++;
		}
	}

	for (int i = 1; i < 256; i++)
	{
		apartitii[i] += apartitii[i - 1];
	}

	for (int i = 0; i < 256; i++)
	{
		apartitii[i] = (int)(255.0 * apartitii[i] / area + 0.5);
	}

	for (int k = 0; k < height; k++)
	{
		for (int l = 0; l < width; l++)
		{
			int pixel_value = (*mat)[k][l];
			aux[k][l] = clamp(apartitii[pixel_value], 0, 255);
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(*mat)[i][j] = aux[i][j];
		}
	}

	eliberare_spatiu(&aux, height);

	return 0;
}

int EDGE(int x_0, int y_0, int x_1, int y_1, int h, int w, pixel ***mat)
{
	int height = y_1 - y_0;
	int width = x_1 - x_0;

	pixel **res = (pixel **)malloc(sizeof(pixel *) * height);

	// Programare defensiva
	if (!res)
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
		res[i] = malloc(sizeof(pixel) * width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j + x_0 == 0 || j + x_0 == w - 1 || i + y_0 == 0 || i + y_0 == h - 1)
			{
				res[i][j] = (*mat)[i + y_0][j + x_0];
			}
			else
			{
				res[i][j].red = 0;
				res[i][j].green = 0;
				res[i][j].blue = 0;
				for (int k = i - 1; k <= i + 1; k++)
				{
					for (int l = j - 1; l <= j + 1; l++)
					{
						if (i == k && j == l)
						{
							res[i][j].red += (*mat)[i + y_0][j + x_0].red * 8;
							res[i][j].green += (*mat)[i + y_0][j + x_0].green * 8;
							res[i][j].blue += (*mat)[i + y_0][j + x_0].blue * 8;
						}
						else
						{
							res[i][j].red += (*mat)[k + y_0][l + x_0].red * (-1);
							res[i][j].green += (*mat)[k + y_0][l + x_0].green * (-1);
							res[i][j].blue += (*mat)[k + y_0][l + x_0].blue * (-1);
						}
					}
				}
				res[i][j].red = clamp(res[i][j].red, 0, 255);
				res[i][j].green = clamp(res[i][j].green, 0, 255);
				res[i][j].blue = clamp(res[i][j].blue, 0, 255);
			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(*mat)[i + y_0][j + x_0] = res[i][j];
		}
	}

	eliberare_spatiu_load(&res, height);

	return 0;
}

int SHARPEN(int x_0, int y_0, int x_1, int y_1, int h, int w, pixel ***mat)
{
	int height = y_1 - y_0;
	int width = x_1 - x_0;

	pixel **res = (pixel **)malloc(sizeof(pixel *) * height);

	// Programare defensiva
	if (!res)
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
		res[i] = malloc(sizeof(pixel) * width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j + x_0 == 0 || j + x_0 == w - 1 || i + y_0 == 0 || i + y_0 == h - 1)
			{
				res[i][j] = (*mat)[i + y_0][j + x_0];
			}
			else
			{
				res[i][j].red = 0;
				res[i][j].green = 0;
				res[i][j].blue = 0;
				for (int k = i - 1; k <= i + 1; k++)
				{
					for (int l = j - 1; l <= j + 1; l++)
					{
						if (k + y_0 >= 0 && k + y_0 < h && l + x_0 >= 0 && l + x_0 < w)
						{
							if (i == k && j == l)
							{
								res[i][j].red += (*mat)[k + y_0][l + x_0].red * 5;
								res[i][j].green += (*mat)[k + y_0][l + x_0].green * 5;
								res[i][j].blue += (*mat)[k + y_0][l + x_0].blue * 5;
							}
							else if ((i == k && j != l) || (i != k && j == l))
							{
								res[i][j].red += (*mat)[k + y_0][l + x_0].red * (-1);
								res[i][j].green += (*mat)[k + y_0][l + x_0].green * (-1);
								res[i][j].blue += (*mat)[k + y_0][l + x_0].blue * (-1);
							}
						}
					}
				}
				res[i][j].red = clamp(res[i][j].red, 0, 255);
				res[i][j].green = clamp(res[i][j].green, 0, 255);
				res[i][j].blue = clamp(res[i][j].blue, 0, 255);
			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(*mat)[i + y_0][j + x_0] = res[i][j];
		}
	}

	eliberare_spatiu_load(&res, height);

	return 0;
}

int BLUR(int x_0, int y_0, int x_1, int y_1, int h, int w, pixel ***mat)
{
	int height = y_1 - y_0;
	int width = x_1 - x_0;

	pixel **res = (pixel **)malloc(sizeof(pixel *) * height);

	// Programare defensiva
	if (!res)
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
		res[i] = malloc(sizeof(pixel) * width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j + x_0 == 0 || j + x_0 == w - 1 || i + y_0 == 0 || i + y_0 == h - 1)
			{
				res[i][j] = (*mat)[i + y_0][j + x_0];
			}
			else
			{
				res[i][j].red = 0;
				res[i][j].green = 0;
				res[i][j].blue = 0;
				for (int k = i - 1; k <= i + 1; k++)
				{
					for (int l = j - 1; l <= j + 1; l++)
					{
						res[i][j].red += (*mat)[k + y_0][l + x_0].red;
						res[i][j].green += (*mat)[k + y_0][l + x_0].green;
						res[i][j].blue += (*mat)[k + y_0][l + x_0].blue;
					}
				}

				float red = res[i][j].red * 1.0 / 9;
				float green = res[i][j].green * 1.0 / 9;
				float blue = res[i][j].blue * 1.0 / 9;

				res[i][j].red = (int)(red + 0.5);
				res[i][j].green = (int)(green + 0.5);
				res[i][j].blue = (int)(blue + 0.5);
			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(*mat)[i + y_0][j + x_0] = res[i][j];
		}
	}

	eliberare_spatiu_load(&res, height);

	return 0;
}

int GAUSSIAN_BLUR(int x_0, int y_0, int x_1, int y_1, int h, int w, pixel ***mat)
{
	int height = y_1 - y_0;
	int width = x_1 - x_0;

	pixel **res = (pixel **)malloc(sizeof(pixel *) * height);

	// Programare defensiva
	if (!res)
	{
		printf("Oh-oh... Nu a putut fi facuta citirea...\n");
		return 0;
	}

	for (int i = 0; i < height; i++)
		res[i] = malloc(sizeof(pixel) * width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j + x_0 == 0 || j + x_0 == w - 1 || i + y_0 == 0 || i + y_0 == h - 1)
			{
				res[i][j] = (*mat)[i + y_0][j + x_0];
			}
			else
			{
				res[i][j].red = 0;
				res[i][j].green = 0;
				res[i][j].blue = 0;
				for (int k = i - 1; k <= i + 1; k++)
				{
					for (int l = j - 1; l <= j + 1; l++)
					{
						if (i == k && j == l)
						{
							res[i][j].red += (*mat)[k + y_0][l + x_0].red * 4;
							res[i][j].green += (*mat)[k + y_0][l + x_0].green * 4;
							res[i][j].blue += (*mat)[k + y_0][l + x_0].blue * 4;
						}
						else if ((i == k && j != l) || (i != k && j == l))
						{
							res[i][j].red += (*mat)[k + y_0][l + x_0].red * (2);
							res[i][j].green += (*mat)[k + y_0][l + x_0].green * (2);
							res[i][j].blue += (*mat)[k + y_0][l + x_0].blue * (2);
						}
						else
						{
							res[i][j].red += (*mat)[k + y_0][l + x_0].red;
							res[i][j].green += (*mat)[k + y_0][l + x_0].green;
							res[i][j].blue += (*mat)[k + y_0][l + x_0].blue;
						}
					}
				}

				float red = res[i][j].red * 1.0 / 16;
				float green = res[i][j].green * 1.0 / 16;
				float blue = res[i][j].blue * 1.0 / 16;

				res[i][j].red = (int)(red + 0.5);
				res[i][j].green = (int)(green + 0.5);
				res[i][j].blue = (int)(blue + 0.5);
			}
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(*mat)[i + y_0][j + x_0] = res[i][j];
		}
	}

	eliberare_spatiu_load(&res, height);

	return 0;
}

int main(void)
{
	int ok = 1, rgb, height, width, **mat = NULL, bin = 0, x_0, x_1, y_0, y_1;
	char type_p[3];
	pixel **mat_load = NULL;
	FILE *img_loaded = NULL;

	while (ok > 0)
	{
		char t[NMAX + ADR_MAX];
		fgets(t, NMAX + ADR_MAX, stdin);
		t[strcspn(t, "\n")] = '\0'; // Scoate endline-ul din string
		char s[NMAX];
		sscanf(t, "%s", s);
		if (strcmp(s, "LOAD") == 0)
		{
			eliberare_spatiu(&mat, height);
			eliberare_spatiu_load(&mat_load, height);
			bin = 0;
			char adr[ADR_MAX], aux[4];
			sscanf(t, "%s %s", aux, adr);
			FILE *test;
			test = fopen(adr, "r");
			if (test)
			{
				fclose(test);
				img_loaded = Load(adr, img_loaded, &bin, type_p);
				if (img_loaded)
				{
					eliberare_spatiu(&mat, height);
					eliberare_spatiu_load(&mat_load, height);
					printf("Loaded %s\n", adr);
					READ_DET(&height, &width, &rgb, type_p, img_loaded);
					if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
					{
						READ_PGM(height, width, bin, img_loaded, &mat);
					}
					else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
					{
						READ_PPM(height, width, bin, img_loaded, &mat_load);
					}
					x_0 = 0, x_1 = width;
					y_0 = 0, y_1 = height;
				}
			}
			else
			{
				printf("Failed to load %s\n", adr);
				img_loaded = NULL;
			}
		}
		else if (strcmp(t, "SELECT ALL") == 0 || strcmp(t, "SELECT ALL ") == 0)
		{
			if (!img_loaded)
			{
				printf("No image loaded\n");
			}
			else
			{
				x_0 = 0;
				x_1 = width;
				y_0 = 0;
				y_1 = height;
				printf("Selected ALL\n");
			}
		}
		else if (strcmp(s, "SELECT") == 0)
		{
			int t_x_0, t_x_1, t_y_0, t_y_1;
			char aux[NMAX];
			int n = sscanf(t, "%s %d %d %d %d", aux, &t_x_0, &t_y_0, &t_x_1, &t_y_1);
			if (n != 5)
			{
				printf("Invalid command\n");
			}
			else
			{
				if (t_x_0 > t_x_1)
				{
					int aux = t_x_1;
					t_x_1 = t_x_0;
					t_x_0 = aux;
				}
				if (t_y_0 > t_y_1)
				{
					int aux = t_y_1;
					t_y_1 = t_y_0;
					t_y_0 = aux;
				}
				if (!img_loaded)
				{
					printf("No image loaded\n");
				}
				else if ((t_y_1 > height || t_x_1 > width || t_y_0 < 0 || t_x_0 < 0) ||
						 (t_x_0 == t_x_1 || t_y_0 == t_y_1))
				{
					printf("Invalid set of coordinates\n");
					x_0 = 0;
					x_1 = width;
					y_0 = 0;
					y_1 = height;
				}
				else
				{
					x_0 = t_x_0;
					x_1 = t_x_1;
					y_0 = t_y_0;
					y_1 = t_y_1;
					printf("Selected %d %d %d %d\n", x_0, y_0, x_1, y_1);
				}
			}
		}
		else if (strcmp(s, "HISTOGRAM") == 0)
		{
			int st, bin, test;
			char aux[NMAX];
			int n = sscanf(t, "%s %d %d %d", aux, &st, &bin, &test);
			if (!img_loaded)
			{
				printf("No image loaded\n");
			}
			else if (n != 3)
			{
				printf("Invalid command\n");
			}
			else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
			{
				printf("Black and white image needed\n");
			}
			else
			{
				int *hist = (int *)malloc(sizeof(int) * bin);
				int max = 0;
				for (int i = 0; i < bin; i++)
				{
					hist[i] = 0;
					for (int k = 0; k < height; k++)
					{
						for (int l = 0; l < width; l++)
						{
							if ((256 * i / bin) <= mat[k][l] && mat[k][l] < (256 * (i + 1) / bin))
								hist[i]++;
						}
					}
					if (hist[i] > max)
					{
						max = hist[i];
					}
				}

				int *stel = (int *)malloc(sizeof(int) * bin);

				for (int i = 0; i < bin; i++)
				{
					if (max > 0 && st > 0)
					{
						float rez = 1.0 * hist[i] / (1.0 * max / st);
						stel[i] = (int)rez;
					}
					else
					{
						stel[i] = 0;
					}
					printf("%d\t|\t", stel[i]);
					for (int j = 0; j < stel[i]; j++)
					{
						printf("*");
					}
					printf("\n");
				}

				free(hist);
				free(stel);
			}
		}
		else if (strcmp(t, "EQUALIZE") == 0)
		{
			if (!img_loaded)
			{
				printf("No image loaded\n");
			}
			else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
			{
				printf("Black and white image needed\n");
			}
			else
			{
				EQUALIZE(height, width, &mat);
				printf("Equalize done\n");
			}
		}
		else if (strcmp(s, "ROTATE") == 0)
		{
			int unghi;
			char aux[7];
			sscanf(t, "%s %d", aux, &unghi);
			if (!img_loaded)
			{
				printf("No image loaded\n");
			}
			else if (unghi % 90 != 0)
			{
				printf("Unsupported rotation angle\n");
			}
			else if (((y_1 - y_0) != (x_1 - x_0)) && (x_1 != width && y_1 != height && x_0 != 0 && y_0 != 0))
			{
				printf("The selection must be square\n");
			}
			else
			{
				if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
				{
					ROTATE_PGM(x_0, y_0, x_1, y_1, &height, &width, &mat, unghi);
				}
				else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
				{
					ROTATE_PPM(x_0, y_0, x_1, y_1, &height, &width, &mat_load, unghi);
				}
				printf("Rotated %d\n", unghi);
			}
		}
		else if (strcmp(t, "CROP") == 0)
		{
			if (!img_loaded)
			{
				printf("No image loaded\n");
			}
			else
			{
				if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
				{
					CROP(&x_0, &y_0, &x_1, &y_1, &height, &width, &mat);
				}
				else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
				{
					CROP_LOAD(&x_0, &y_0, &x_1, &y_1, &height, &width, &mat_load);
				}
				printf("Image cropped\n");
			}
		}
		else if (strcmp(s, "APPLY") == 0)
		{
			if (img_loaded)
			{
				char com[ADR_MAX], aux[NMAX];
				int n = sscanf(t, "%s %s", aux, com);
				if (n == 1)
				{
					printf("Invalid command\n");
				}
				else if (strcmp(com, "EDGE") != 0 && strcmp(com, "SHARPEN") != 0 && strcmp(com, "BLUR") != 0 && strcmp(com, "GAUSSIAN_BLUR") != 0)
				{
					printf("APPLY parameter invalid\n");
				}
				else if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
				{
					printf("Easy, Charlie Chaplin\n");
				}
				else
				{
					if (strcmp(com, "EDGE") == 0)
					{
						EDGE(x_0, y_0, x_1, y_1, height, width, &mat_load);
					}
					else if (strcmp(com, "SHARPEN") == 0)
					{
						SHARPEN(x_0, y_0, x_1, y_1, height, width, &mat_load);
					}
					else if (strcmp(com, "BLUR") == 0)
					{
						BLUR(x_0, y_0, x_1, y_1, height, width, &mat_load);
					}
					else
					{
						GAUSSIAN_BLUR(x_0, y_0, x_1, y_1, height, width, &mat_load);
					}
					printf("APPLY %s done\n", com);
				}
			}
			else
			{
				char adr[ADR_MAX], aux[NMAX];
				sscanf(t, "%s %s", aux, adr);
				printf("No image loaded\n");
			}
		}
		else if (strcmp(s, "SAVE") == 0)
		{
			if (img_loaded)
			{
				char adr[ADR_MAX], ascii[6], aux[NMAX];
				int n = sscanf(t, "%s %s %s", aux, adr, ascii);
				FILE *in = NULL;
				if (n == 2)
				{
					in = fopen(adr, "wb");
					if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
					{
						SAVE_PGM(height, width, rgb, in, &mat, NULL);
					}
					else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
					{
						SAVE_PPM(height, width, rgb, in, &mat_load, NULL);
					}
					fclose(in);
				}
				else if (n == 3 && strcmp(ascii, "ascii") == 0)
				{
					in = fopen(adr, "w");
					if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
					{
						SAVE_PGM(height, width, rgb, in, &mat, ascii);
					}
					else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
					{
						SAVE_PPM(height, width, rgb, in, &mat_load, ascii);
					}
					fclose(in);
				}
				else
				{
					in = fopen(adr, "wb");
					if (strcmp("P2", type_p) == 0 || strcmp("P5", type_p) == 0)
					{
						SAVE_PGM(height, width, rgb, in, &mat, NULL);
					}
					else if (strcmp("P3", type_p) == 0 || strcmp("P6", type_p) == 0)
					{
						SAVE_PPM(height, width, rgb, in, &mat_load, NULL);
					}
					fclose(in);
				}
				printf("Saved %s\n", adr);
			}
			else
			{
				char adr[ADR_MAX], ascii[6], aux[NMAX];
				sscanf(t, "%s %s %s", aux, adr, ascii);
				printf("No image loaded\n");
			}
		}
		else if (strcmp(s, "EXIT") == 0)
		{
			ok = 0;
			if (!img_loaded)
			{
				printf("No image loaded\n");
			}
			eliberare_spatiu(&mat, height);
			eliberare_spatiu_load(&mat_load, height);
		}
		else
		{
			printf("Invalid command\n");
		}
	}

	if (img_loaded)
		fclose(img_loaded);

	return 0;
}

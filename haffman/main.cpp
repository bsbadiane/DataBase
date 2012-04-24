/*
 * main.cpp
 *
 *  Created on: 24.04.2012
 *      Author: night
 */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

short father[512];
unsigned short code[256], heap_length;
unsigned long compress_charcount, file_size, heap[257];
unsigned char code_length[256];
long frequency_count[512]; // встречаемость символов
short decomp_tree[512]; // дерево декодирования

FILE *ifile, *ofile; // входной и выходной файлы

//=========================================================
/* ФУНКЦИИ ДЛЯ КОМПРЕССИИ */

//---------------------------------------------------------
// реорганизация структкры heap
//void reheap(unsigned short heap_entry) {
//	unsigned short index;
//	//unsigned short flag = 1;
//	bool flag = true;
//
//	unsigned long heap_value;
//
//	heap_value = heap[heap_entry];
//
//	//используется линейная древовидная структура
//	while ((heap_entry <= (heap_length >> 1)) && (flag)) {
//		index = heap_entry << 1;
//
//		if (index < heap_length)
//			if (frequency_count[heap[index]]
//					>= frequency_count[heap[index + 1]])
//				index++;
//
//		if (frequency_count[heap_value] < frequency_count[heap[index]])
//			//flag--;
//			flag = false;
//		else {
//			heap[heap_entry] = heap[index];
//			heap_entry = index;
//		}
//	}
//
//	heap[heap_entry] = heap_value;
//}
void resort() {
	std::sort(heap + 1, heap+heap_length+1,
			[](unsigned long first, unsigned long second) -> bool {
				return frequency_count[first] > frequency_count[second];
	});
}
//---------------------------------------------------------
// Сжатие данных
void compress_image() {
	unsigned int thebyte = 0;
	short loop1;
	unsigned short current_code;
	unsigned long loop;

	unsigned short current_length, dvalue;
	unsigned long curbyte = 0;
	short curbit = 7;

	for (loop = 0L; loop < file_size; loop++) {
		dvalue = (unsigned short) getc (ifile);
		current_code = code[dvalue];
		current_length = (unsigned short) code_length[dvalue];

		for (loop1 = current_length - 1; loop1 >= 0; --loop1) {
			if ((current_code >> loop1) & 1)
				thebyte |= (char) (1 << curbit);

			if (--curbit < 0) {
				putc(thebyte, ofile);
				thebyte = 0;
				curbyte++;
				curbit = 7;
			}
		}
	}
	putc(thebyte, ofile);
	compress_charcount = ++curbyte;
}
//---------------------------------------------------------
// Генерация таблицы кодирования
unsigned short generate_code_table() {
	unsigned short loop;
	unsigned short current_length;
	unsigned short current_bit;

	unsigned short bitcode;
	short parent;

	for (loop = 0; loop < 256; loop++)
		if (frequency_count[loop]) {
			current_length = bitcode = 0;
			current_bit = 1;
			parent = father[loop];

			while (parent) {
				if (parent < 0) {
					bitcode += current_bit;
					parent = -parent;
				}
				parent = father[parent];
				current_bit <<= 1;
				current_length++;
			}

			code[loop] = bitcode;

			if (current_length > 16)
				return (0);
			else
				code_length[loop] = (unsigned char) current_length;
		} else
			code[loop] = code_length[loop] = 0;

	return (1);
}
//---------------------------------------------------------
// Построение дерева кодирования
void build_code_tree() {
	unsigned short findex;
	unsigned long heap_value1, heap_value2;

	while (heap_length != 1) {
		//heap_value = heap[1];
		heap_value1 = heap[heap_length];
		heap_value2 = heap[heap_length-1];
		--heap_length;
		//heap[1] = heap[heap_length--];

		//reheap(1);
		findex = heap_length + 255;

		frequency_count[findex] = frequency_count[heap_value1]
				+ frequency_count[heap_value2];
		father[heap_value1] = findex;
		//father[heap[1]] = -findex;
		father[heap_value2] = -findex;
		heap[heap_length] = findex;

		//reheap(1);
		resort();
	}

	father[256] = 0;
}
//---------------------------------------------------------
// построение heap'а по частотам встречаемости при инициализации
void build_initial_heap() {
	unsigned short loop;
	heap_length = 0;

	for (loop = 0; loop < 256; loop++)
		if (frequency_count[loop])
			heap[++heap_length] = (unsigned long) loop;

//	for (loop = heap_length; loop > 0; loop--)
//		reheap(loop);
	resort();
}
//---------------------------------------------------------
// подсчет количества встречаемости каждого символа
void get_frequency_count() {
	unsigned long loop;

	for (loop = 0; loop < file_size; loop++)
		frequency_count[getc (ifile)]++;
}
//=========================================================
/* ФУНКЦИИ ДЛЯ КОМПРЕССИИ */

//---------------------------------------------------------
// Построение дерева декомпрессии
void build_decomp_tree() {
	unsigned short loop1;
	unsigned short current_index;

	unsigned short loop;
	unsigned short current_node = 1;

	decomp_tree[1] = 1;

	for (loop = 0; loop < 256; loop++) {
		if (code_length[loop]) {
			current_index = 1;
			for (loop1 = code_length[loop] - 1; loop1 > 0; loop1--) {
				current_index = (decomp_tree[current_index] << 1)
						+ ((code[loop] >> loop1) & 1);
				if (!(decomp_tree[current_index]))
					decomp_tree[current_index] = ++current_node;
			}
			decomp_tree[(decomp_tree[current_index] << 1) + (code[loop] & 1)] =
					-loop;
		}
	}
}
//---------------------------------------------------------
// декомпрессия данных
void decompress_image() {
	unsigned short cindex = 1;
	char curchar;
	short bitshift;

	unsigned long charcount = 0L;

	while (charcount < file_size) {
		curchar = (char) getc (ifile);

		for (bitshift = 7; bitshift >= 0; --bitshift) {
			cindex = (cindex << 1) + ((curchar >> bitshift) & 1);

			if (decomp_tree[cindex] <= 0) {
				putc((int) (-decomp_tree[cindex]), ofile);

				if ((++charcount) == file_size)
					bitshift = 0;
				else
					cindex = 1;
			} else
				cindex = decomp_tree[cindex];
		}
	}
}
//=========================================================
// компрессия файла
void Encode() {
	fseek(ifile, 0L, 2);
	file_size = (unsigned long) ftell(ifile);
	fseek(ifile, 0L, 0);
	get_frequency_count();
	build_initial_heap();
	build_code_tree();
	if (!generate_code_table())
		printf("ERROR!  Code value out of range. Cannot compress.\n");
	else {
		fwrite(&file_size, sizeof(file_size), 1, ofile);
		fwrite(code, sizeof(unsigned short)/*2*/, 256, ofile);
		fwrite(code_length, sizeof(unsigned char)/*1*/, 256, ofile);
		fseek(ifile, 0L, 0);
		compress_image();
	};
}
//=========================================================
// декомпрессия файла
void Decode() {
	fread(&file_size, sizeof(file_size), 1, ifile);
	fread(code, sizeof(unsigned short)/*2*/, 256, ifile);
	fread(code_length, sizeof(unsigned char)/*1*/, 256, ifile);
	build_decomp_tree();
	decompress_image();
	//fclose(ofile);

}
//=========================================================
// главная функция
int main(int argc, char* argv[]) {
	// проверка аргументов
	if (argc < 4 || argc > 4) {
		printf("Bad arguments.\n");
		return -1;
	}

	// открытие входного файла
	if ((ifile = fopen(argv[2], "rb")) == NULL) {
		printf("Error input file.\n");
		return -1;
	};

	// открытие выходного файла
	if ((ofile = fopen(argv[3], "wb")) == NULL) {
		printf("Error output file.\n");
		fclose(ifile);
		return -1;
	};

	if (tolower(argv[1][0]) == 'e')
		Encode(); // коппрессия
	else if (tolower(argv[1][0]) == 'd')
		Decode(); // декомпрессия

	fclose(ofile);
	fclose(ifile);

	printf("Done.\n");

	return 0;

}

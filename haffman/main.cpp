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
unsigned short code[256], char_count;
unsigned long compress_charcount, file_size, sorted_chars[257];
unsigned char code_length[256];
long frequency_count[512]; // встречаемость символов
short decomp_tree[512]; // дерево декодирования

FILE *inputfile, *outputfile; // входной и выходной файлы

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
	std::sort(sorted_chars + 1, sorted_chars+char_count+1,
			[](unsigned long first, unsigned long second) -> bool {
				return frequency_count[first] > frequency_count[second];
	});
}
//---------------------------------------------------------
// Сжатие данных
void compress_data() {
	unsigned int current_byte = 0;
	short bit;
	unsigned short current_code;
	unsigned long file_pos;

	unsigned short current_length, dvalue;
	unsigned long curbyte = 0;
	short curbit = 7;

	for (file_pos = 0L; file_pos < file_size; file_pos++) {
		dvalue = (unsigned short) getc (inputfile);
		current_code = code[dvalue];
		current_length = (unsigned short) code_length[dvalue];

		for (bit = current_length - 1; bit >= 0; --bit) {
			if ((current_code >> bit) & 1)
				current_byte |= (char) (1 << curbit);

			if (--curbit < 0) {
				putc(current_byte, outputfile);
				current_byte = 0;
				curbyte++;
				curbit = 7;
			}
		}
	}
	putc(current_byte, outputfile);
	compress_charcount = ++curbyte;
}
//---------------------------------------------------------
// Генерация таблицы кодирования
unsigned short create_codetable() {
	unsigned short char_code;
	unsigned short current_length;
	unsigned short current_bit;

	unsigned short bitcode;
	short parent;

	for (char_code = 0; char_code < 256; char_code++)
		if (frequency_count[char_code]) {
			current_length = bitcode = 0;
			current_bit = 1;
			parent = father[char_code];

			while (parent) {
				if (parent < 0) {
					bitcode += current_bit;
					parent = -parent;
				}
				parent = father[parent];
				current_bit <<= 1;
				current_length++;
			}

			code[char_code] = bitcode;

			if (current_length > 16)
				return (0);
			else
				code_length[char_code] = (unsigned char) current_length;
		} else
			code[char_code] = code_length[char_code] = 0;

	return (1);
}
//---------------------------------------------------------
// Построение дерева кодирования
void create_codetree() {
	unsigned short fother_index;
	unsigned long heap_value1, heap_value2;

	while (char_count != 1) {
		//heap_value = heap[1];
		heap_value1 = sorted_chars[char_count];
		heap_value2 = sorted_chars[char_count-1];
		--char_count;
		//heap[1] = heap[heap_length--];

		//reheap(1);
		fother_index = char_count + 255;

		frequency_count[fother_index] = frequency_count[heap_value1]
				+ frequency_count[heap_value2];
		father[heap_value1] = fother_index;
		//father[heap[1]] = -findex;
		father[heap_value2] = -fother_index;
		sorted_chars[char_count] = fother_index;

		//reheap(1);
		resort();
	}

	father[256] = 0;
}
//---------------------------------------------------------
// построение heap'а по частотам встречаемости при инициализации
void create_sorted_chars() {
	unsigned short char_code;
	char_count = 0;

	for (char_code = 0; char_code < 256; char_code++)
		if (frequency_count[char_code])
			sorted_chars[++char_count] = (unsigned long) char_code;

//	for (loop = heap_length; loop > 0; loop--)
//		reheap(loop);
	resort();
}
//---------------------------------------------------------
// подсчет количества встречаемости каждого символа
void get_frequency_count() {
	unsigned long file_pos;

	for (file_pos = 0; file_pos < file_size; file_pos++)
		frequency_count[getc (inputfile)]++;
}
//=========================================================
/* ФУНКЦИИ ДЛЯ КОМПРЕССИИ */

//---------------------------------------------------------
// Построение дерева декомпрессии
void create_decomp_tree() {
	unsigned short bit;
	unsigned short current_index;

	unsigned short char_code;
	unsigned short current_node = 1;

	decomp_tree[1] = 1;

	for (char_code = 0; char_code < 256; char_code++) {
		if (code_length[char_code]) {
			current_index = 1;
			for (bit = code_length[char_code] - 1; bit > 0; bit--) {
				current_index = (decomp_tree[current_index] << 1)
						+ ((code[char_code] >> bit) & 1);
				if (!(decomp_tree[current_index]))
					decomp_tree[current_index] = ++current_node;
			}
			decomp_tree[(decomp_tree[current_index] << 1) + (code[char_code] & 1)] =
					-char_code;
		}
	}
}
//---------------------------------------------------------
// декомпрессия данных
void decompress_data() {
	unsigned short node_index = 1;
	char curchar;
	short bitshift;

	unsigned long charcount = 0L;

	while (charcount < file_size) {
		curchar = (char) getc (inputfile);

		for (bitshift = 7; bitshift >= 0; --bitshift) {
			node_index = (node_index << 1) + ((curchar >> bitshift) & 1);

			if (decomp_tree[node_index] <= 0) {
				putc((int) (-decomp_tree[node_index]), outputfile);

				if ((++charcount) == file_size)
					bitshift = 0;
				else
					node_index = 1;
			} else
				node_index = decomp_tree[node_index];
		}
	}
}
//=========================================================
// компрессия файла
void Encode() {
	fseek(inputfile, 0L, 2);
	file_size = (unsigned long) ftell(inputfile);
	fseek(inputfile, 0L, 0);
	get_frequency_count();
	create_sorted_chars();
	create_codetree();
	if (!create_codetable())
		printf("Ошибка. Превышен допустимый размер кода.\n");
	else {
		fwrite(&file_size, sizeof(file_size), 1, outputfile);
		fwrite(code, sizeof(unsigned short)/*2*/, 256, outputfile);
		fwrite(code_length, sizeof(unsigned char)/*1*/, 256, outputfile);
		fseek(inputfile, 0L, 0);
		compress_data();
	};
}
//=========================================================
// декомпрессия файла
void Decode() {
	fread(&file_size, sizeof(file_size), 1, inputfile);
	fread(code, sizeof(unsigned short)/*2*/, 256, inputfile);
	fread(code_length, sizeof(unsigned char)/*1*/, 256, inputfile);
	create_decomp_tree();
	decompress_data();
	//fclose(ofile);

}
//=========================================================
// главная функция
int main(int argc, char* argv[]) {
	// проверка аргументов
	if (argc < 4 || argc > 4) {
		printf("Неверные аргументы.\n");
		return -1;
	}

	// открытие входного файла
	if ((inputfile = fopen(argv[2], "rb")) == NULL) {
		printf("Ошибка входного файла.\n");
		return -1;
	};

	// открытие выходного файла
	if ((outputfile = fopen(argv[3], "wb")) == NULL) {
		printf("Ошибка выходного файла.\n");
		fclose(inputfile);
		return -1;
	};

	if (tolower(argv[1][0]) == 'e')
		Encode(); // коппрессия
	else if (tolower(argv[1][0]) == 'd')
		Decode(); // декомпрессия

	fclose(outputfile);
	fclose(inputfile);

	printf("Сделано.\n");

	return 0;

}

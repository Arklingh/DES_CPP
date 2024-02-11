#pragma once
#include "Encryption.h"
#include "Key.h"
#include "InformationBlock.h"
#ifndef DES_H
#define DES_H
#define SHIFT(X,XCOUNT,Y,YCOUNT,MASK) (X<<XCOUNT)|((Y>>YCOUNT)&MASK)// Макрос для зсувів уліво
class DES : Encryption<Block_64b>
{

private:
	key Keys;
	Block_64b Block;



	//=======================================================================================================================
	//Початкова перестановка байт
	inline void IP()
	{
		//Оголошення регістрових змінних для прискорення зсувів
		register unsigned int LowPart = Block.LowPartOfBlock;
		register unsigned int HightPart = Block.HighPartOfBlock;
		//змінні для зберігання результатів і зменшення кількості розіменувань
		unsigned int NewLowPart, NewHightPart;
		//Зсуви байт
		// Byte 7
		NewHightPart = (HightPart >> 30);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 22, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 14, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 6, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 30, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 22, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 14, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 6, 1);

		//Byte 6
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 28, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 20, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 12, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 4, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 28, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 20, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 12, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 4, 1);

		//Byte 5
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 26, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 18, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 10, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 2, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 26, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 18, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 10, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 2, 1);


		//Byte 4
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 24, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 16, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 8, 1);
		NewHightPart = (NewHightPart << 1) | (HightPart & 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 24, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 16, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 8, 1);
		NewHightPart = (NewHightPart << 1) | (LowPart & 1);

		//Byte 3		
		NewLowPart = (HightPart >> 31);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 23, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 15, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 7, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 31, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 23, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 15, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 7, 1);
		//Byte 2
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 29, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 21, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 13, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 5, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 29, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 21, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 13, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 5, 1);

		//Byte 1
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 27, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 19, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 11, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 3, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 27, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 19, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 11, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 3, 1);

		//Byte 0
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 25, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 17, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 9, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 1, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 25, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 17, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 9, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 1, 1);
		//запис результату зсувів у вхідний блок
		Block.LowPartOfBlock = NewLowPart;
		Block.HighPartOfBlock = NewHightPart;
	}
	//=======================================================================================================================

	//=======================================================================================================================
	//Кінцева перестановка байт
	inline void IPRevers()
	{
		//Оголошення регістрових змінних для прискорення зсувів
		register unsigned int LowPart = Block.LowPartOfBlock;
		register unsigned int HightPart = Block.HighPartOfBlock;
		//змінні для зберігання результатів і зменшення кількості розіменувань
		unsigned int NewLowPart, NewHightPart;

		//Byte 7
		NewHightPart = (LowPart >> 31);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 31, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 23, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 23, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 15, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 15, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 7, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 7, 1);

		//Byte 6
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 30, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 30, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 22, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 22, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 14, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 14, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 6, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 6, 1);

		//Byte 5
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 29, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 29, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 21, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 21, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 13, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 13, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 5, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 5, 1);

		//Byte 4
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 28, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 28, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 20, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 20, 1);

		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 12, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 12, 1);
		NewHightPart = SHIFT(NewHightPart, 1, LowPart, 4, 1);
		NewHightPart = SHIFT(NewHightPart, 1, HightPart, 4, 1);

		//Byte 3
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 27, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 27, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 19, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 19, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 11, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 11, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 3, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 3, 1);

		//Byte 2
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 26, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 26, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 18, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 18, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 10, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 10, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 2, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 2, 1);


		//Byte 1
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 25, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 25, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 17, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 17, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 9, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 9, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 1, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 1, 1);

		//Byte 0
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 24, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 24, 1);
		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 16, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 16, 1);

		NewLowPart = SHIFT(NewLowPart, 1, LowPart, 8, 1);
		NewLowPart = SHIFT(NewLowPart, 1, HightPart, 8, 1);
		NewLowPart = (NewLowPart << 1) | (LowPart & 1);
		NewLowPart = (NewLowPart << 1) | (HightPart & 1);

		//запис результату зсувів у вхідний блок
		Block.LowPartOfBlock = NewLowPart;
		Block.HighPartOfBlock = NewHightPart;
	}
	//=======================================================================================================================



	//=======================================================================================================================
	//Функція F
	inline unsigned int F(unsigned int i)
	{
		register unsigned int LowPart = Block.LowPartOfBlock;
		unsigned long long NewLowPart;
		//=====================================
		// Розширення Е
		//Byte 5
		NewLowPart = LowPart & 1;
		NewLowPart = (NewLowPart << 5) | ((LowPart >> 27) & 0x1F);
		NewLowPart = (NewLowPart << 2) | ((LowPart >> 27) & 0x03);

		//Byte 4
		NewLowPart = (NewLowPart << 4) | ((LowPart >> 23) & 0x0F);
		NewLowPart = (NewLowPart << 4) | ((LowPart >> 21) & 0x0F);

		//Byte 3
		NewLowPart = (NewLowPart << 2) | ((LowPart >> 19) & 0x03);
		NewLowPart = (NewLowPart << 6) | ((LowPart >> 15) & 0x3F);

		//Byte 2 - 1 -0
		NewLowPart = (NewLowPart << 6) | ((LowPart >> 11) & 0x3F);
		NewLowPart = (NewLowPart << 6) | ((LowPart >> 7) & 0x3F);
		NewLowPart = (NewLowPart << 6) | ((LowPart >> 3) & 0x3F);
		NewLowPart = (NewLowPart << 5) | ((LowPart) & 0x1F);
		NewLowPart = (NewLowPart << 1) | (LowPart >> 31);

		//XOR розширеного блоку Е з ключем раунду
		NewLowPart = NewLowPart ^ (Keys.GetKey(i)).Full_Block;


		extern unsigned char S[]; //Оголошення раніше ініціалізованого загального SBox "а з файлу Tables.h
		LowPart = 0;
		//Подача байт у SBox'и
		//S1
		LowPart = (LowPart << 4) | S[((NewLowPart >> 42) & 63)];

		//S2
		LowPart = (LowPart << 4) | S[((NewLowPart >> 36) & 63) + 64];

		//S3
		LowPart = (LowPart << 4) | S[((NewLowPart >> 30) & 63) + 128];

		//S4
		LowPart = (LowPart << 4) | S[((NewLowPart >> 24) & 63) + 192];

		//S5
		LowPart = (LowPart << 4) | S[((NewLowPart >> 18) & 63) + 256];

		//S6
		LowPart = (LowPart << 4) | S[((NewLowPart >> 12) & 63) + 320];

		//S7
		LowPart = (LowPart << 4) | S[((NewLowPart >> 6) & 63) + 384];

		//S8
		LowPart = (LowPart << 4) | S[(NewLowPart & 63) + 448];

		//============================================================
		// Перестановка бітів P
		NewLowPart = 0;

		//Byte 3
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 16) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 25) & 1);
		NewLowPart = (NewLowPart << 2) | ((LowPart >> 11) & 3);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 3) & 1);

		NewLowPart = (NewLowPart << 1) | ((LowPart >> 20) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 4) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 15) & 1);

		//Byte 2
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 31) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 17) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 9) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 6) & 1);

		NewLowPart = (NewLowPart << 1) | ((LowPart >> 27) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 14) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 1) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 22) & 1);

		//Byte 1
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 30) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 24) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 8) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 18) & 1);

		NewLowPart = (NewLowPart << 1) | ((LowPart >> 0) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 5) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 29) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 23) & 1);

		//Byte 0
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 13) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 19) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 2) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 26) & 1);

		NewLowPart = (NewLowPart << 1) | ((LowPart >> 10) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 21) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 28) & 1);
		NewLowPart = (NewLowPart << 1) | ((LowPart >> 7) & 1);

		return NewLowPart;


	}
	//=======================================================================================================================






public:
	//=======================================================================================================================
	//Функція шифрування
	Block_64b EncryptTheFile(Block_64b Bl)
	{
		Block.Full_Block = Bl.Full_Block;//ініціалізація блоку для роботи алгоритму вхідним блоком
		IP();//Початкова перестановка байт

		unsigned int R;
		for (int i = 0; i < 15; i++)//цикл шифрування (або дешифрування, залежить від режиму роботи) на 15 ітерацій, 16 окремо, оскільки інша
		{
			R = Block.LowPartOfBlock;//Сохранение младшей 32-х битной части входяшего блока		
			Block.LowPartOfBlock = Block.HighPartOfBlock ^ F(i);// збереження результату XOR "а старшої частини з результатом функції F (F "шифрує" молодшу частину) у молодшу частину
			Block.HighPartOfBlock = R; // Запис молодшої частини на місце старшої
		}


		Block.HighPartOfBlock = Block.HighPartOfBlock ^ F(15);	//Остання ітерація, відрізняється тим, що молодша і старша частина не міняються місцями

		///////////////
		IPRevers();//Кінцева перестановка байт
		return Block;//Повернення зашифрованого блоку
	}

	//=======================================================================================================================


	//=======================================================================================================================
	//Конструктор
	DES()
	{
		Block.Full_Block = 0;
	};

	DES(key& K)
	{
		Keys = K;//Встановлення ключів раунду для шифрування
		Block.Full_Block = 0;

	}
	//=======================================================================================================================

	//=======================================================================================================================
	//setKeys 
	void setKeys(key& K)
	{
		Keys = K;	//Встановлення ключів раунду для шифрування
	}
	//=======================================================================================================================

};
#endif
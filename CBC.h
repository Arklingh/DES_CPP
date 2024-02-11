#pragma once
#include "Encryption.h"
#include "DES.h"
#ifndef CBC_H
#define CBC_H

class CBC :  Encryption<Block_64b>
{
private:
	DES* Algoritm; //покажчик на алгоритм шифрування
	Block_64b T, PrevBlock;
	bool isEncrypt, FirstIter; //прапорці.

public:

	//=======================================================================================================================
	//Конструктор
	CBC(DES* Alg, unsigned long long Vect, bool isEnc)
	{
		Algoritm = Alg; //Тип алгоритму
		T.Full_Block = Vect;//Початковий вектор
		PrevBlock.Full_Block = 0;//Обнулення попереднього блоку
		isEncrypt = isEnc;//встановлення режиму шифрування
		FirstIter = true;//встановлення прапорця першої ітерації
	}

	CBC()//Конструктор за замовчуванням, встановлює все в нуль, а прапорці в тру
	{
		Algoritm = 0;
		T.Full_Block = 0;
		PrevBlock.Full_Block = 0;
		isEncrypt = true;
		FirstIter = true;
	};
	//=======================================================================================================================

	//=======================================================================================================================
	//Set Keys
	void setKeys(DES* Alg, unsigned long long Vect, bool isEnc)
	{
		Algoritm = Alg;//Тип алгоритму
		T.Full_Block = Vect;//Початковий вектор
		PrevBlock.Full_Block = 0;//ООбнулення попереднього блоку
		isEncrypt = isEnc;//встановлення режиму шифрування
		FirstIter = true;//встановлення прапорця першої ітерації
	}
	//=======================================================================================================================


	//=======================================================================================================================
	//Функція шифрування
	Block_64b EncryptTheFile(Block_64b Bl)
	{
		if (isEncrypt)//Перевірка режиму роботи, шифрування-дешифрування
		{
			T.Full_Block = T.Full_Block ^ Bl.Full_Block;//На 1 ітерації XOR початкового вектора з блоком даних, на наступних - попереднього блоку з новим блоком
			T = Algoritm->EncryptTheFile(T);//Шифрування отриманого блоку заданим алгоритмом
		}
		else
		{
			//Дешифрування
			if (FirstIter)//Якщо перша ітерація
			{
				PrevBlock.Full_Block = Bl.Full_Block;//Запис вхідного блоку як попереднього 
				T.Full_Block = T.Full_Block ^ ((Algoritm->EncryptTheFile(Bl).Full_Block));//XOR початкового вектора з дешифрованим вхідним блоком
				FirstIter = false;
			}
			else
			{
				T.Full_Block = PrevBlock.Full_Block;//запис у вихідний блок значення попереднього блоку
				PrevBlock.Full_Block = Bl.Full_Block;//Запис вхідного блоку як попереднього
				T.Full_Block = T.Full_Block ^ ((Algoritm->EncryptTheFile(Bl).Full_Block));//Запис у вихідний блок результат XOR "а попереднього блоку з дешифрованим вхідним блоком
			}
		}
		return T;
	}

	//=======================================================================================================================

};
#endif
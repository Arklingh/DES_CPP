#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <time.h>
#include "InformationBlock.h"
#include "Key.h"
#include "DES.h"
#include "3DES.h"
#include "FileProcessing.h"
#include "CBC.h"
#include "ConstantTables.h"

#define MOD_CBC 1
#define MOD_ECB 2
#define ADES 1
#define ADES_EEE3 2
#define ADES_EDE3 3
using namespace std;

//
////=======================================================================================================================
////Перевірка ключа
bool CheckTextKey(string& Key)
{
	if (Key.size() != 16) //перевірка, чи є довжина ключа рівною 16
		return false;
	for (unsigned int i = 0; i < 16; ++i) //цикл перевірки, щоб у ключі були тільки цифри 1-9, і букви a-f, A-F
	{
		if (!(((Key[i] >= '0') & (Key[i] < 58)) || ((Key[i] > 64) & (Key[i] < 71)) || ((Key[i] > 96) & (Key[i] < 103))))
			return false;
		else cin.putback(Key[15 - i]);
	}
	return true;
}

Block_64b Readkey(unsigned int KNumber)
{
	string  TextKey;
	Block_64b value;
	while (1)
	{
		if (KNumber < 4)
			cout << "Введите " << KNumber << " шестнадцатеричный ключ:\n";
		else
			cout << "Введите начальный шестнадцатеричный вектор, состоящий из 16 символов:\n";
		cin >> TextKey; //зчитуємо ключ у рядок
		if (CheckTextKey(TextKey))//перевіряємо чи правильно введений ключ
		{
			cin >> hex >> value.Full_Block;//записуємо ключ у 16-ричному вигляді
			return value;
		}
		else
			cout << "Нужно ввести 16 символов (от 0 до 9 и a,b,c,d,e,f,A,B,C,D,E,F).\n ";
	}

}



////=======================================================================================================================
////Вибираємо файли
void FileWork(LPOPENFILENAME inpFile, LPOPENFILENAME outpFile)
{
	while (true)
		if (!GetOpenFileName(inpFile)) //поки не вибереться вхідний файл виводить повідомлення
		{
			std::cout << "\nФайл не выбран! Выберите исходный файл.";
		}
		else break;
	while (true)
		if (!GetSaveFileName(outpFile))//поки не вибереться вихідний файл виводить повідомлення
		{
			std::cout << "\nФайл не выбран! Выберите файл для записи результата.";
		}
		else break;
}
////=======================================================================================================================
//
////=======================================================================================================================
//// Ініціалізація структури для отримання імені файлу
void MakeOpenFileStruct(LPOPENFILENAME OFN, TCHAR* FileName, bool isRead)
{
	OFN->lStructSize = sizeof(OPENFILENAME);
	OFN->hwndOwner = NULL;
	OFN->lpstrFilter = __TEXT("All Files\0*.*\0Text Files\0*.txt\0\0");
	OFN->lpstrCustomFilter = NULL;
	OFN->nFilterIndex = 1;
	*FileName = 0;
	OFN->lpstrFile = FileName;
	OFN->nMaxFile = MAX_PATH;
	OFN->lpstrFileTitle = NULL;
	OFN->lpstrInitialDir = NULL;
	OFN->lpstrTitle = __TEXT("Открыть файл...");
	OFN->Flags = (isRead ? (OFN_FILEMUSTEXIST) : (OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT)) | OFN_EXPLORER | OFN_NOCHANGEDIR;//якщо вибирається файл для зчитування, то прапорці OFN_FILEMUSTEXIST| OFN_EXPLORER | OFN_NOCHANGEDIR якщо для запису OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT| OFN_EXPLORER | OFN_NOCHANGEDIR
	OFN->lpstrDefExt = __TEXT("txt");
}
////=======================================================================================================================
//	
//
//
//
//
void main()
{
	setlocale(LC_ALL, "Russian");//для консолі
	TCHAR  InPutFile[MAX_PATH], OutPutFile[MAX_PATH];
	system("color F0");
	OPENFILENAME inpFile, outpFile;
	unsigned int AlgType = 0, ModType = 0;
	Block_64b Vector;//вхідний вектор
	bool isEncrypt;//прапорець (шифрування/дешифрування)
	key Keys[3];//масив ключів
	DWORD Time;//для перевірки часу виконання програми
	DES Des;
	DES_EEE3 DES3;
	CBC Cbc;
	Filer<Block_64b> filer;
	//Ініціалізація структур.
	MakeOpenFileStruct(&inpFile, InPutFile, true);//заповнюємо структуру для отримання вхідного файлу
	MakeOpenFileStruct(&outpFile, OutPutFile, false);//заповнюємо структуру для отримання вихідного файлу
	FileWork(&inpFile, &outpFile);//Вибираємо вихідні файли.

	while (!AlgType) // поки не буде обрано алгоритм, виводиться повідомлення про вибір алгоритму
	{
		cout << "Выберите алгоритм (Вводите строго числа от 1 до 3!): \n";
		cout << ADES << ": DES \n";
		cout << ADES_EEE3 << ": 3DES-EEE \n";
		cout << ADES_EDE3 << ": 3DES-EDE \n";
		cin >> AlgType;
		if ((AlgType < 0) || (AlgType > 3)) //якщо алгоритм обраний неправильно, то вибір заново
		{
			cout << "Введите число от 1 до 3! \n";
			AlgType = 0;
		}
	}

	while (!ModType)// поки не буде обрано метод шифрування виводитися повідомлення про вибір методу
	{
		cout << "Выберите режим работы: \n";
		cout << MOD_CBC << ": CBC \n";
		cout << MOD_ECB << ": ECB \n";
		cin >> ModType;
		if ((ModType < 0) || (ModType > 2)) // якщо метод обрано неправильно, то вибір заново
		{
			cout << "Введите число от 1 до 2! \n";
			ModType = 0;
		}
	}
	//вибираємо шифруємо або дешифруємо
	cout << "Выберите действие: \n";
	cout << "0: Дешифрование \n";
	cout << "1: Шифрование \n";
	cin >> isEncrypt;


	Keys[0].SetKey(Readkey(1));//генеруємо перший ключ
	if (AlgType != ADES)//Якщо алгоритм 3DES
	{
		Keys[1].SetKey(Readkey(2));//генеруємо другий ключ
		Keys[2].SetKey(Readkey(3));//генеруємо третій ключ
		if (AlgType == ADES_EDE3)//Якщо алгоритм DES_EDE3 перевертаємо другий ключ
			Keys[1].ReversKeys();
	}


	if (ModType == MOD_CBC)//якщо режим шифрування CBC заповнюємо вхідний вектор
		Vector.Full_Block = Readkey(4).Full_Block;

	if (!isEncrypt) //Якщо дешифруємо
	{
		Keys[0].ReversKeys();//Перевертаємо ключ 1
		if (AlgType != ADES)//Якщо алгоритм 3DES
		{
			Keys[1].ReversKeys();//Перевертаємо ключ 2
			Keys[2].ReversKeys();//Перевертаємо ключ 3
		}
	}


	if (ModType == MOD_CBC)//якщо режим шифрування CBC
		if (AlgType == ADES)//Алгоритм - DES
		{
			Des.setKeys(Keys[0]);//Встановлюємо перший ключ
			Cbc.setKeys(&Des, Vector.Full_Block, isEncrypt);//Налаштовуємо режим CBC (покажчик на алгоритм шифрування, початковий вектор, режим роботи)
			filer.SetAlg((Encryption<Block_64b>*)(&Cbc));//Налаштування класу роботи з файлом			
		}

		else//Якщо 3DES
		{
			DES3.setKeys((Keys[0]), (Keys[1]), (Keys[2]), isEncrypt);//Встановлюємо ключі
			Cbc.setKeys(&DES3, Vector.Full_Block, isEncrypt);//Налаштовуємо режим CBC (покажчик на алгоритм шифрування, початковий вектор, режим роботи)
			filer.SetAlg((Encryption<Block_64b>*) & Cbc);//Приводимо покажчик на клас СВС до покажчика на батьківський клас Encryption з типом даних Block_64
		}
	else//якщо режим шифрування ECB
		if (AlgType == ADES)//Алгоритм - DES
		{
			Des.setKeys(Keys[0]);//Встановлюємо перший ключ
			filer.SetAlg((Encryption<Block_64b>*) & Des);//Приводимо покажчик на клас Des до покажчика на батьківський клас Encryption з типом даних Block_64				
		}

		else//Якщо 3DES
		{
			DES3.setKeys((Keys[0]), (Keys[1]), (Keys[2]), isEncrypt);//Встановлюємо ключі
			filer.SetAlg((Encryption<Block_64b>*) & DES3);//Приводимо покажчик на клас DES3 до покажчика на батьківський клас Encryption з типом даних Block_64		
		}

	Time = GetTickCount(); //Стартуємо таймер виконання програми
	filer.Begin(InPutFile, OutPutFile, isEncrypt);//Починаємо виконувати програму
	Time = GetTickCount() - Time;//Підраховуємо час, витрачений на виконання в мілісекундах
	cout << "Програма завершилась за:" << Time << " мс" << endl;
	system("pause");
}

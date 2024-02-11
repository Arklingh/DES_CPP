#include <Windows.h>
#include <iostream>
#include "DES.h"
#ifndef FILER_H
#define FILER_H
#define DEFAULT_BUFFER_SIZE 4096
#define WRITE_BUFFER 2

template <class BlockType> class Filer
{
private:
	BlockType* OutBuffer; //Створюємо буфер виведення
	BlockType* InpBuffer[2];//Створюємо 2 буфери введення
	Encryption<BlockType>* Alg; //Покажчик на алгоритм шифрування
	DWORD BufferSize;//Обчислюваний розмір буфера, що обчислюється
	unsigned char* Buffers[3]; //Масив покажчиків на буфери



//=======================================================================================================================
//Визначає розмір кластера (буфера)
	unsigned int GetClusterSize(TCHAR* inpFile)
	{
		DWORD SectorsPerCluster = 0, BytesPerSector = 0, NumberOfFreeClusters = 0, TotalNumberOfClusters = 0;
		TCHAR Disk[3] = { inpFile[0],inpFile[1],inpFile[2] };//Беремо перші 3 символи їхнього імені файлу (тобто ім'я диска)
		if (!GetDiskFreeSpace(Disk, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters))
			return DEFAULT_BUFFER_SIZE;
		//Якщо функція спрацювала, то множимо кількість секторів на кластер на кількість байт на секторі та отримуємо розмір кластера
		else return SectorsPerCluster * BytesPerSector;
	}
	//=======================================================================================================================


	//=======================================================================================================================
	//Визначаємо розмір файлу
	unsigned int GettingSizeOfFile(HANDLE hFile)
	{
		DWORD getting;
		return GetFileSize(hFile, &getting);//повертає розмір буфера		 
	}






public:
	unsigned int static const INPUTFILE_IS_NOT_OPEN = 23;
	unsigned int static const OUTPUTFILE_IS_NOT_OPEN = 24;

	//=============================================================================================
	//Конструктор
	Filer<BlockType>(Encryption<BlockType>* Algoritm)
	{
		Alg = Algoritm;//Встановлення покажчика на алгоритм шифрування		
	}

	Filer<BlockType>()
	{
		Alg = 0;
	};
	//==============================================================================================
	void SetAlg(Encryption<BlockType>* Algoritm)
	{
		Alg = Algoritm;
	}

	//=============================================================================================
	//Деструктор
	~Filer<BlockType>()
	{

	}
	//==============================================================================================





	//=============================================================================================
	//Функція шифрування файлу
	unsigned int Begin(TCHAR* inpFile, TCHAR* outptFile, bool isEncrypt)
	{
		BufferSize = GetClusterSize(inpFile);//Дізнаємося розмір Кластера
		//Створюємо 3 масиви розміром кластера
		Buffers[0] = new unsigned char[BufferSize];
		Buffers[1] = new unsigned char[BufferSize];
		Buffers[2] = new unsigned char[BufferSize];
		//Приведення покажчиків на створені масиви до покажчиків на масив об'єктів типу BlockType
		OutBuffer = (BlockType*)((LPVOID*)(&Buffers[WRITE_BUFFER][0]));
		InpBuffer[0] = (BlockType*)((LPVOID*)(&Buffers[0][0]));
		InpBuffer[1] = (BlockType*)((LPVOID*)(&Buffers[1][0]));

		OVERLAPPED OverlInPut, OverlOutPut;
		//Заповнення структур для асинхронної роботи з файлами
		OverlInPut.Internal = OverlInPut.InternalHigh = OverlInPut.Offset = OverlInPut.OffsetHigh = 0;
		OverlInPut.hEvent = CreateEvent(0, FALSE, FALSE, 0);

		OverlOutPut.Internal = OverlOutPut.InternalHigh = OverlOutPut.Offset = OverlOutPut.OffsetHigh = 0;
		OverlOutPut.hEvent = CreateEvent(0, FALSE, FALSE, 0);

		HANDLE hInPutFile, hOutPutFile;

		//Налаштування початкових значень для змінних
		unsigned int BufferNumber = 0, NewBufferNumber = 0, CountOfIterI, CountOfIterJ, CountOfLastIter;
		DWORD ReadedBytes, WritedBytes;
		unsigned char isNastyBlock;
		BOOL  ResOfRead = FALSE, ResOfWrite = FALSE;
		BlockType* TempBuf;

		//Відкриття файлу для читання
		hInPutFile = CreateFile(inpFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if (hInPutFile == INVALID_HANDLE_VALUE)
			return INPUTFILE_IS_NOT_OPEN;

		//Открытие файла для записи
		hOutPutFile = CreateFile(outptFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
		if (hOutPutFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hInPutFile);
			return OUTPUTFILE_IS_NOT_OPEN;
		}

		//Розмір файлу
		DWORD FSize = GettingSizeOfFile(hInPutFile);

		//Обчислюємо Кількість ітерацій
		ReadedBytes = (FSize % BufferSize) ? 1 : 0;//Перевірка розміру файлу на кратність розміру кластера
		CountOfIterI = (FSize / BufferSize + ReadedBytes) - 1;//Кількість звичайних ітерація
		CountOfIterJ = BufferSize / sizeof(BlockType);//кількість блоків в одному буфері

		//Визначаємо необхідність розширення останнього блоку
		isNastyBlock = FSize % sizeof(BlockType);//Якщо є залишок від ділення ,то необхідно доповнити останній блок

		//Перше зчитування з файлу		
		ResOfRead = ReadFile(hInPutFile, Buffers[BufferNumber], BufferSize, &ReadedBytes, &OverlInPut);
		if (!ResOfRead)
			GetOverlappedResult(hInPutFile, &OverlInPut, &ReadedBytes, TRUE);//Чекаємо закінчення читання файлу

		//=================================================	
		//Шифруємо на 1 ітерацію менше ніж потрібно.
		for (unsigned int i = 0; i < CountOfIterI; ++i)
		{
			NewBufferNumber = !BufferNumber;//Міняємо буфери для читання місцями
			OverlInPut.Offset += BufferSize;//зсув покажчика у файлі читання на кількість прочитаних байт
			ResOfRead = ReadFile(hInPutFile, (LPVOID)Buffers[NewBufferNumber], BufferSize, &ReadedBytes, &OverlInPut);//Читання з файлу в новий буфер (нульовий або перший)
			if (i)//Усі ітерації крім найпершої
			{
				if (!ResOfWrite)//Якщо файл не встиг записатися, то чекаємо закінчення запису файлу
					WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
				OverlOutPut.Offset += BufferSize;//зсув покажчика у файлі запису на кількість записаних байт
			}

			//Шифрування буфера
			TempBuf = InpBuffer[BufferNumber];//Присвоюємо минулий буфер для читання тимчасовому буферу. Збільшує швидкість за рахунок зменшення кількості розіменувань
			for (unsigned int j = 0; j < CountOfIterJ; ++j)
			{
				OutBuffer[j] = Alg->EncryptTheFile(TempBuf[j]);//Шифрування буфера
			}
			if (i != CountOfIterI - 1)//Для всіх ітерацій, крім останньої, перевіряємо, чи записався буфер у файл, якщо ні, то чекаємо
				ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], BufferSize, &WritedBytes, &OverlOutPut);
			BufferNumber = NewBufferNumber;
			if (!ResOfRead)//Чекаємо закінчення читання файлу
				GetOverlappedResult(hInPutFile, &OverlInPut, &ReadedBytes, TRUE);
		}
		//=================================================	
		TempBuf = InpBuffer[BufferNumber];	//Присвоюємо минулий буфер для читання тимчасовому буферу.
		CountOfIterJ = ReadedBytes / sizeof(BlockType) + (ReadedBytes % sizeof(BlockType) ? 1 : 0);
		if (isEncrypt)//Якщо шифруємо
		{
			if (CountOfIterI)//записуємо передостанній блок якщо файл більший за розмір буфера
			{
				ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], BufferSize, &WritedBytes, &OverlOutPut);
				if (!ResOfWrite)
					WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
				OverlOutPut.Offset += BufferSize;
			}
			// Шифруємо останній блок
			for (unsigned int j = 0; j < CountOfIterJ; ++j)
				OutBuffer[j] = Alg->EncryptTheFile(TempBuf[j]);
			//Якщо потрібно розширити останній блок
			if (isNastyBlock)
			{
				//Записуємо вихідний розмір розширеного блоку в кінець буфера
				*(unsigned char*)(&OutBuffer[CountOfIterJ]) = isNastyBlock;
				//Збільшуємо кількість байт для запису на 1
				ReadedBytes += sizeof(BlockType) - isNastyBlock + 1;//Обчислюємо кількість байт якими доповнили останній блок +1
			}

		}
		//Дешифруємо останній блок
		else
		{
			if (ReadedBytes > 1)//Якщо зчитано більше 1 байта
			{
				if (CountOfIterI)//записуємо передостанній блок якщо файл більший за розмір буфера
				{
					ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], BufferSize, &WritedBytes, &OverlOutPut);
					if (!ResOfWrite)
						WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
					OverlOutPut.Offset += BufferSize;
				}
				// Дешифруємо останній блок	
				for (unsigned int j = 0; j < CountOfIterJ; ++j)
					OutBuffer[j] = Alg->EncryptTheFile(TempBuf[j]);
				//Чи потрібно прибирати розширення останнього блоку
				if (FSize & 1)
				{
					isNastyBlock = *(unsigned char*)(&TempBuf[CountOfIterJ - 1]);//Зчитуємо вихідний розмір останнього блоку
					//Зменшуємо кількість байт для запису
					ReadedBytes = ReadedBytes - (sizeof(BlockType) - isNastyBlock) - 1;
				}
			}
			else
			{
				isNastyBlock = *(unsigned char*)(&TempBuf[0]);
				//Зменшуємо кількість байт для запису
				ReadedBytes = BufferSize - (sizeof(BlockType) - isNastyBlock);
			}

		}
		//Записуємо останній буфер і очікуємо закінчення його запису
		ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], ReadedBytes, &WritedBytes, &OverlOutPut);
		if (!ResOfWrite)
			WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
		OverlOutPut.Offset += ReadedBytes;
		//Закриваємо файли та хендли для подій у структурах OVERLAPPED
		CloseHandle(hOutPutFile);
		CloseHandle(hInPutFile);
		CloseHandle(OverlOutPut.hEvent);
		CloseHandle(OverlInPut.hEvent);

		return 0;
	}

};
#endif

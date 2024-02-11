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
	BlockType* OutBuffer; //��������� ����� ���������
	BlockType* InpBuffer[2];//��������� 2 ������ ��������
	Encryption<BlockType>* Alg; //�������� �� �������� ����������
	DWORD BufferSize;//������������ ����� ������, �� ������������
	unsigned char* Buffers[3]; //����� ��������� �� ������



//=======================================================================================================================
//������� ����� �������� (������)
	unsigned int GetClusterSize(TCHAR* inpFile)
	{
		DWORD SectorsPerCluster = 0, BytesPerSector = 0, NumberOfFreeClusters = 0, TotalNumberOfClusters = 0;
		TCHAR Disk[3] = { inpFile[0],inpFile[1],inpFile[2] };//������ ����� 3 ������� ������� ���� ����� (����� ��'� �����)
		if (!GetDiskFreeSpace(Disk, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters))
			return DEFAULT_BUFFER_SIZE;
		//���� ������� ����������, �� ������� ������� ������� �� ������� �� ������� ���� �� ������ �� �������� ����� ��������
		else return SectorsPerCluster * BytesPerSector;
	}
	//=======================================================================================================================


	//=======================================================================================================================
	//��������� ����� �����
	unsigned int GettingSizeOfFile(HANDLE hFile)
	{
		DWORD getting;
		return GetFileSize(hFile, &getting);//������� ����� ������		 
	}






public:
	unsigned int static const INPUTFILE_IS_NOT_OPEN = 23;
	unsigned int static const OUTPUTFILE_IS_NOT_OPEN = 24;

	//=============================================================================================
	//�����������
	Filer<BlockType>(Encryption<BlockType>* Algoritm)
	{
		Alg = Algoritm;//������������ ��������� �� �������� ����������		
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
	//����������
	~Filer<BlockType>()
	{

	}
	//==============================================================================================





	//=============================================================================================
	//������� ���������� �����
	unsigned int Begin(TCHAR* inpFile, TCHAR* outptFile, bool isEncrypt)
	{
		BufferSize = GetClusterSize(inpFile);//ĳ������� ����� ��������
		//��������� 3 ������ ������� ��������
		Buffers[0] = new unsigned char[BufferSize];
		Buffers[1] = new unsigned char[BufferSize];
		Buffers[2] = new unsigned char[BufferSize];
		//���������� ��������� �� ������� ������ �� ��������� �� ����� ��'���� ���� BlockType
		OutBuffer = (BlockType*)((LPVOID*)(&Buffers[WRITE_BUFFER][0]));
		InpBuffer[0] = (BlockType*)((LPVOID*)(&Buffers[0][0]));
		InpBuffer[1] = (BlockType*)((LPVOID*)(&Buffers[1][0]));

		OVERLAPPED OverlInPut, OverlOutPut;
		//���������� �������� ��� ���������� ������ � �������
		OverlInPut.Internal = OverlInPut.InternalHigh = OverlInPut.Offset = OverlInPut.OffsetHigh = 0;
		OverlInPut.hEvent = CreateEvent(0, FALSE, FALSE, 0);

		OverlOutPut.Internal = OverlOutPut.InternalHigh = OverlOutPut.Offset = OverlOutPut.OffsetHigh = 0;
		OverlOutPut.hEvent = CreateEvent(0, FALSE, FALSE, 0);

		HANDLE hInPutFile, hOutPutFile;

		//������������ ���������� ������� ��� ������
		unsigned int BufferNumber = 0, NewBufferNumber = 0, CountOfIterI, CountOfIterJ, CountOfLastIter;
		DWORD ReadedBytes, WritedBytes;
		unsigned char isNastyBlock;
		BOOL  ResOfRead = FALSE, ResOfWrite = FALSE;
		BlockType* TempBuf;

		//³������� ����� ��� �������
		hInPutFile = CreateFile(inpFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if (hInPutFile == INVALID_HANDLE_VALUE)
			return INPUTFILE_IS_NOT_OPEN;

		//�������� ����� ��� ������
		hOutPutFile = CreateFile(outptFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
		if (hOutPutFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hInPutFile);
			return OUTPUTFILE_IS_NOT_OPEN;
		}

		//����� �����
		DWORD FSize = GettingSizeOfFile(hInPutFile);

		//���������� ʳ������ ��������
		ReadedBytes = (FSize % BufferSize) ? 1 : 0;//�������� ������ ����� �� �������� ������ ��������
		CountOfIterI = (FSize / BufferSize + ReadedBytes) - 1;//ʳ������ ��������� ��������
		CountOfIterJ = BufferSize / sizeof(BlockType);//������� ����� � ������ �����

		//��������� ����������� ���������� ���������� �����
		isNastyBlock = FSize % sizeof(BlockType);//���� � ������� �� ������ ,�� ��������� ��������� ������� ����

		//����� ���������� � �����		
		ResOfRead = ReadFile(hInPutFile, Buffers[BufferNumber], BufferSize, &ReadedBytes, &OverlInPut);
		if (!ResOfRead)
			GetOverlappedResult(hInPutFile, &OverlInPut, &ReadedBytes, TRUE);//������ ��������� ������� �����

		//=================================================	
		//������� �� 1 �������� ����� �� �������.
		for (unsigned int i = 0; i < CountOfIterI; ++i)
		{
			NewBufferNumber = !BufferNumber;//̳����� ������ ��� ������� ������
			OverlInPut.Offset += BufferSize;//���� ��������� � ���� ������� �� ������� ���������� ����
			ResOfRead = ReadFile(hInPutFile, (LPVOID)Buffers[NewBufferNumber], BufferSize, &ReadedBytes, &OverlInPut);//������� � ����� � ����� ����� (�������� ��� ������)
			if (i)//�� �������� ��� ��������
			{
				if (!ResOfWrite)//���� ���� �� ����� ����������, �� ������ ��������� ������ �����
					WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
				OverlOutPut.Offset += BufferSize;//���� ��������� � ���� ������ �� ������� ��������� ����
			}

			//���������� ������
			TempBuf = InpBuffer[BufferNumber];//���������� ������� ����� ��� ������� ����������� ������. ������ �������� �� ������� ��������� ������� �����������
			for (unsigned int j = 0; j < CountOfIterJ; ++j)
			{
				OutBuffer[j] = Alg->EncryptTheFile(TempBuf[j]);//���������� ������
			}
			if (i != CountOfIterI - 1)//��� ��� ��������, ��� ��������, ����������, �� ��������� ����� � ����, ���� �, �� ������
				ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], BufferSize, &WritedBytes, &OverlOutPut);
			BufferNumber = NewBufferNumber;
			if (!ResOfRead)//������ ��������� ������� �����
				GetOverlappedResult(hInPutFile, &OverlInPut, &ReadedBytes, TRUE);
		}
		//=================================================	
		TempBuf = InpBuffer[BufferNumber];	//���������� ������� ����� ��� ������� ����������� ������.
		CountOfIterJ = ReadedBytes / sizeof(BlockType) + (ReadedBytes % sizeof(BlockType) ? 1 : 0);
		if (isEncrypt)//���� �������
		{
			if (CountOfIterI)//�������� ������������ ���� ���� ���� ������ �� ����� ������
			{
				ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], BufferSize, &WritedBytes, &OverlOutPut);
				if (!ResOfWrite)
					WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
				OverlOutPut.Offset += BufferSize;
			}
			// ������� ������� ����
			for (unsigned int j = 0; j < CountOfIterJ; ++j)
				OutBuffer[j] = Alg->EncryptTheFile(TempBuf[j]);
			//���� ������� ��������� ������� ����
			if (isNastyBlock)
			{
				//�������� �������� ����� ����������� ����� � ����� ������
				*(unsigned char*)(&OutBuffer[CountOfIterJ]) = isNastyBlock;
				//�������� ������� ���� ��� ������ �� 1
				ReadedBytes += sizeof(BlockType) - isNastyBlock + 1;//���������� ������� ���� ����� ��������� ������� ���� +1
			}

		}
		//��������� ������� ����
		else
		{
			if (ReadedBytes > 1)//���� ������� ����� 1 �����
			{
				if (CountOfIterI)//�������� ������������ ���� ���� ���� ������ �� ����� ������
				{
					ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], BufferSize, &WritedBytes, &OverlOutPut);
					if (!ResOfWrite)
						WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
					OverlOutPut.Offset += BufferSize;
				}
				// ��������� ������� ����	
				for (unsigned int j = 0; j < CountOfIterJ; ++j)
					OutBuffer[j] = Alg->EncryptTheFile(TempBuf[j]);
				//�� ������� ��������� ���������� ���������� �����
				if (FSize & 1)
				{
					isNastyBlock = *(unsigned char*)(&TempBuf[CountOfIterJ - 1]);//������� �������� ����� ���������� �����
					//�������� ������� ���� ��� ������
					ReadedBytes = ReadedBytes - (sizeof(BlockType) - isNastyBlock) - 1;
				}
			}
			else
			{
				isNastyBlock = *(unsigned char*)(&TempBuf[0]);
				//�������� ������� ���� ��� ������
				ReadedBytes = BufferSize - (sizeof(BlockType) - isNastyBlock);
			}

		}
		//�������� ������� ����� � ������� ��������� ���� ������
		ResOfWrite = WriteFile(hOutPutFile, (LPVOID)Buffers[WRITE_BUFFER], ReadedBytes, &WritedBytes, &OverlOutPut);
		if (!ResOfWrite)
			WaitForSingleObject(OverlOutPut.hEvent, INFINITE);
		OverlOutPut.Offset += ReadedBytes;
		//��������� ����� �� ������ ��� ���� � ���������� OVERLAPPED
		CloseHandle(hOutPutFile);
		CloseHandle(hInPutFile);
		CloseHandle(OverlOutPut.hEvent);
		CloseHandle(OverlInPut.hEvent);

		return 0;
	}

};
#endif

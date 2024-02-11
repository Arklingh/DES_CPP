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
////�������� �����
bool CheckTextKey(string& Key)
{
	if (Key.size() != 16) //��������, �� � ������� ����� ����� 16
		return false;
	for (unsigned int i = 0; i < 16; ++i) //���� ��������, ��� � ����� ���� ����� ����� 1-9, � ����� a-f, A-F
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
			cout << "������� " << KNumber << " ����������������� ����:\n";
		else
			cout << "������� ��������� ����������������� ������, ��������� �� 16 ��������:\n";
		cin >> TextKey; //������� ���� � �����
		if (CheckTextKey(TextKey))//���������� �� ��������� �������� ����
		{
			cin >> hex >> value.Full_Block;//�������� ���� � 16-������� ������
			return value;
		}
		else
			cout << "����� ������ 16 �������� (�� 0 �� 9 � a,b,c,d,e,f,A,B,C,D,E,F).\n ";
	}

}



////=======================================================================================================================
////�������� �����
void FileWork(LPOPENFILENAME inpFile, LPOPENFILENAME outpFile)
{
	while (true)
		if (!GetOpenFileName(inpFile)) //���� �� ���������� ������� ���� �������� �����������
		{
			std::cout << "\n���� �� ������! �������� �������� ����.";
		}
		else break;
	while (true)
		if (!GetSaveFileName(outpFile))//���� �� ���������� �������� ���� �������� �����������
		{
			std::cout << "\n���� �� ������! �������� ���� ��� ������ ����������.";
		}
		else break;
}
////=======================================================================================================================
//
////=======================================================================================================================
//// ����������� ��������� ��� ��������� ���� �����
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
	OFN->lpstrTitle = __TEXT("������� ����...");
	OFN->Flags = (isRead ? (OFN_FILEMUSTEXIST) : (OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT)) | OFN_EXPLORER | OFN_NOCHANGEDIR;//���� ���������� ���� ��� ����������, �� �������� OFN_FILEMUSTEXIST| OFN_EXPLORER | OFN_NOCHANGEDIR ���� ��� ������ OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT| OFN_EXPLORER | OFN_NOCHANGEDIR
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
	setlocale(LC_ALL, "Russian");//��� ������
	TCHAR  InPutFile[MAX_PATH], OutPutFile[MAX_PATH];
	system("color F0");
	OPENFILENAME inpFile, outpFile;
	unsigned int AlgType = 0, ModType = 0;
	Block_64b Vector;//������� ������
	bool isEncrypt;//��������� (����������/������������)
	key Keys[3];//����� ������
	DWORD Time;//��� �������� ���� ��������� ��������
	DES Des;
	DES_EEE3 DES3;
	CBC Cbc;
	Filer<Block_64b> filer;
	//����������� ��������.
	MakeOpenFileStruct(&inpFile, InPutFile, true);//���������� ��������� ��� ��������� �������� �����
	MakeOpenFileStruct(&outpFile, OutPutFile, false);//���������� ��������� ��� ��������� ��������� �����
	FileWork(&inpFile, &outpFile);//�������� ������ �����.

	while (!AlgType) // ���� �� ���� ������ ��������, ���������� ����������� ��� ���� ���������
	{
		cout << "�������� �������� (������� ������ ����� �� 1 �� 3!): \n";
		cout << ADES << ": DES \n";
		cout << ADES_EEE3 << ": 3DES-EEE \n";
		cout << ADES_EDE3 << ": 3DES-EDE \n";
		cin >> AlgType;
		if ((AlgType < 0) || (AlgType > 3)) //���� �������� ������� �����������, �� ���� ������
		{
			cout << "������� ����� �� 1 �� 3! \n";
			AlgType = 0;
		}
	}

	while (!ModType)// ���� �� ���� ������ ����� ���������� ���������� ����������� ��� ���� ������
	{
		cout << "�������� ����� ������: \n";
		cout << MOD_CBC << ": CBC \n";
		cout << MOD_ECB << ": ECB \n";
		cin >> ModType;
		if ((ModType < 0) || (ModType > 2)) // ���� ����� ������ �����������, �� ���� ������
		{
			cout << "������� ����� �� 1 �� 2! \n";
			ModType = 0;
		}
	}
	//�������� ������� ��� ���������
	cout << "�������� ��������: \n";
	cout << "0: ������������ \n";
	cout << "1: ���������� \n";
	cin >> isEncrypt;


	Keys[0].SetKey(Readkey(1));//�������� ������ ����
	if (AlgType != ADES)//���� �������� 3DES
	{
		Keys[1].SetKey(Readkey(2));//�������� ������ ����
		Keys[2].SetKey(Readkey(3));//�������� ����� ����
		if (AlgType == ADES_EDE3)//���� �������� DES_EDE3 ����������� ������ ����
			Keys[1].ReversKeys();
	}


	if (ModType == MOD_CBC)//���� ����� ���������� CBC ���������� ������� ������
		Vector.Full_Block = Readkey(4).Full_Block;

	if (!isEncrypt) //���� ���������
	{
		Keys[0].ReversKeys();//����������� ���� 1
		if (AlgType != ADES)//���� �������� 3DES
		{
			Keys[1].ReversKeys();//����������� ���� 2
			Keys[2].ReversKeys();//����������� ���� 3
		}
	}


	if (ModType == MOD_CBC)//���� ����� ���������� CBC
		if (AlgType == ADES)//�������� - DES
		{
			Des.setKeys(Keys[0]);//������������ ������ ����
			Cbc.setKeys(&Des, Vector.Full_Block, isEncrypt);//����������� ����� CBC (�������� �� �������� ����������, ���������� ������, ����� ������)
			filer.SetAlg((Encryption<Block_64b>*)(&Cbc));//������������ ����� ������ � ������			
		}

		else//���� 3DES
		{
			DES3.setKeys((Keys[0]), (Keys[1]), (Keys[2]), isEncrypt);//������������ �����
			Cbc.setKeys(&DES3, Vector.Full_Block, isEncrypt);//����������� ����� CBC (�������� �� �������� ����������, ���������� ������, ����� ������)
			filer.SetAlg((Encryption<Block_64b>*) & Cbc);//��������� �������� �� ���� ��� �� ��������� �� ����������� ���� Encryption � ����� ����� Block_64
		}
	else//���� ����� ���������� ECB
		if (AlgType == ADES)//�������� - DES
		{
			Des.setKeys(Keys[0]);//������������ ������ ����
			filer.SetAlg((Encryption<Block_64b>*) & Des);//��������� �������� �� ���� Des �� ��������� �� ����������� ���� Encryption � ����� ����� Block_64				
		}

		else//���� 3DES
		{
			DES3.setKeys((Keys[0]), (Keys[1]), (Keys[2]), isEncrypt);//������������ �����
			filer.SetAlg((Encryption<Block_64b>*) & DES3);//��������� �������� �� ���� DES3 �� ��������� �� ����������� ���� Encryption � ����� ����� Block_64		
		}

	Time = GetTickCount(); //�������� ������ ��������� ��������
	filer.Begin(InPutFile, OutPutFile, isEncrypt);//�������� ���������� ��������
	Time = GetTickCount() - Time;//ϳ��������� ���, ���������� �� ��������� � ����������
	cout << "�������� ����������� ��:" << Time << " ��" << endl;
	system("pause");
}

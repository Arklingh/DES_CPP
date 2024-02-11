#pragma once
#include "Encryption.h"
#include "DES.h"
#ifndef CBC_H
#define CBC_H

class CBC :  Encryption<Block_64b>
{
private:
	DES* Algoritm; //�������� �� �������� ����������
	Block_64b T, PrevBlock;
	bool isEncrypt, FirstIter; //��������.

public:

	//=======================================================================================================================
	//�����������
	CBC(DES* Alg, unsigned long long Vect, bool isEnc)
	{
		Algoritm = Alg; //��� ���������
		T.Full_Block = Vect;//���������� ������
		PrevBlock.Full_Block = 0;//��������� ������������ �����
		isEncrypt = isEnc;//������������ ������ ����������
		FirstIter = true;//������������ �������� ����� ��������
	}

	CBC()//����������� �� �������������, ���������� ��� � ����, � �������� � ���
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
		Algoritm = Alg;//��� ���������
		T.Full_Block = Vect;//���������� ������
		PrevBlock.Full_Block = 0;//���������� ������������ �����
		isEncrypt = isEnc;//������������ ������ ����������
		FirstIter = true;//������������ �������� ����� ��������
	}
	//=======================================================================================================================


	//=======================================================================================================================
	//������� ����������
	Block_64b EncryptTheFile(Block_64b Bl)
	{
		if (isEncrypt)//�������� ������ ������, ����������-������������
		{
			T.Full_Block = T.Full_Block ^ Bl.Full_Block;//�� 1 �������� XOR ����������� ������� � ������ �����, �� ��������� - ������������ ����� � ����� ������
			T = Algoritm->EncryptTheFile(T);//���������� ���������� ����� ������� ����������
		}
		else
		{
			//������������
			if (FirstIter)//���� ����� ��������
			{
				PrevBlock.Full_Block = Bl.Full_Block;//����� �������� ����� �� ������������ 
				T.Full_Block = T.Full_Block ^ ((Algoritm->EncryptTheFile(Bl).Full_Block));//XOR ����������� ������� � ������������ ������� ������
				FirstIter = false;
			}
			else
			{
				T.Full_Block = PrevBlock.Full_Block;//����� � �������� ���� �������� ������������ �����
				PrevBlock.Full_Block = Bl.Full_Block;//����� �������� ����� �� ������������
				T.Full_Block = T.Full_Block ^ ((Algoritm->EncryptTheFile(Bl).Full_Block));//����� � �������� ���� ��������� XOR "� ������������ ����� � ������������ ������� ������
			}
		}
		return T;
	}

	//=======================================================================================================================

};
#endif
#include "DES.h"
#include "Key.h"
#ifndef DES_EEE3_H
#define DES_EEE3_H

class DES_EEE3 : public DES
{
private:
	DES Des[2];
	bool isEncrypt;
public:






	Block_64b EncryptTheFile(Block_64b Bl)
	{
		if (isEncrypt)//���������� ������ 3 ��������� ��������� ���������� (��� � ������ �������) ������� �� ������ ������
			return Des[1].EncryptTheFile(Des[0].EncryptTheFile(DES::EncryptTheFile(Bl)));
		else
			return DES::EncryptTheFile(Des[0].EncryptTheFile(Des[1].EncryptTheFile(Bl)));
	}


	DES_EEE3(key& K1, key& K2, key& K3, bool isEnc) :DES(K1)//�������� ������ ������ ��� 1-�� (������������) ���������
	{
		Des[0].setKeys(K2);//�������� ������ ������ ��� 2-�� ���������
		Des[1].setKeys(K3);//�������� ������ ������ ��� 3-�� ���������
		isEncrypt = isEnc;//������������ ������ ������ ��������� (����������-������������)
	}
	DES_EEE3() :DES() {}
	//=======================================================================================================================

	//=======================================================================================================================
	//Set Keys
	void setKeys(key& K1, key& K2, key& K3, bool isEnc)
	{
		DES::setKeys(K1);//�������� ������ ������ ��� 1-�� (������������) ���������
		Des[0].setKeys(K2);//�������� ������ ������ ��� 2-�� ���������
		Des[1].setKeys(K3);//�������� ������ ������ ��� 3-�� ���������
		isEncrypt = isEnc;//������������ ������ ������ ��������� (����������-������������)
	}
	//=======================================================================================================================
};
#endif
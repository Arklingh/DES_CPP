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
		if (isEncrypt)//Послідовний виклик 3 однакових алгоритмів шифрування (але з різними ключами) залежно від режиму роботи
			return Des[1].EncryptTheFile(Des[0].EncryptTheFile(DES::EncryptTheFile(Bl)));
		else
			return DES::EncryptTheFile(Des[0].EncryptTheFile(Des[1].EncryptTheFile(Bl)));
	}


	DES_EEE3(key& K1, key& K2, key& K3, bool isEnc) :DES(K1)//Завдання ключів раунду для 1-го (батьківського) алгоритму
	{
		Des[0].setKeys(K2);//Завдання ключів раунду для 2-го алгоритму
		Des[1].setKeys(K3);//Завдання ключів раунду для 3-го алгоритму
		isEncrypt = isEnc;//Встановлення режиму роботи алгоритму (шифрування-дешифрування)
	}
	DES_EEE3() :DES() {}
	//=======================================================================================================================

	//=======================================================================================================================
	//Set Keys
	void setKeys(key& K1, key& K2, key& K3, bool isEnc)
	{
		DES::setKeys(K1);//Завдання ключів раунду для 1-го (батьківського) алгоритму
		Des[0].setKeys(K2);//Завдання ключів раунду для 2-го алгоритму
		Des[1].setKeys(K3);//Завдання ключів раунду для 3-го алгоритму
		isEncrypt = isEnc;//Встановлення режиму роботи алгоритму (шифрування-дешифрування)
	}
	//=======================================================================================================================
};
#endif
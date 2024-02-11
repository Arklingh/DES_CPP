#pragma once
#include "InformationBlock.h"
#ifndef KEY_H
#define KEY_H
#define SHIFT(X,XCOUNT,Y,YCOUNT,MASK) (X<<XCOUNT)|((Y>>YCOUNT)&MASK)//Макрос для зсувів уліво
#define C_D_SIZE 28

class key {
	private:
		Block_64b RKey[16];

		inline void LeftShift(unsigned int &Low, unsigned int &Hight, unsigned int count)
		{
			Low = ((Low << count) | Low >> (C_D_SIZE - count)) & 0xFFFFFFF;
			Hight = ((Hight << count) | Hight >> (C_D_SIZE - count)) & 0xFFFFFFF;
		}

		inline unsigned long long H(unsigned int RL, unsigned int RH)
		{

			register unsigned long long CD = RH;
			CD = ((CD << 32) | (RL << 4)) >> 4;
			unsigned long long Key = 0;
			extern unsigned char H[];
			for (int i = 0; i < 48; i++)
			{
				Key = SHIFT(Key, 1, CD, H[i], 1);
			}
			return Key;
		}

		void GenerateDesKey(unsigned long long Key)
		{
			register unsigned long long  LowKeyPart = Key;
			unsigned int RoundLowPart = 0, RoundHightPart = 0;
			extern unsigned char C0[], D0[], Iter[]; //Створюємо C0 і D0
			for (int i = 0; i < 28; i++)
			{
				RoundHightPart = SHIFT(RoundHightPart, 1, LowKeyPart, C0[i], 1);
				RoundLowPart = SHIFT(RoundLowPart, 1, LowKeyPart, D0[i], 1);
			}
			//=============================================================================
			//Генерація 16-ти ключів раундів
			for (int i = 0; i < 16; i++)
			{
				LeftShift(RoundLowPart, RoundHightPart, Iter[i]);
				RKey[i].Full_Block = H(RoundLowPart, RoundHightPart);
			}
		}

	public:
		static bool CheckKey(Block_64b &K, unsigned int KeyNumber)
		{
			//список слабких ключів для 3Des
			unsigned long long BadKeys3Des[] =
			{
			  0x01FE01FE01FE01FE,
			  0x1FE01FE01FE01FE0,
			  0x01E001E001F101F1,
			  0x1FFE1FFE0EFE0EFE,
			  0x011F011F010E010E,
			  0xE0FEE0FEF1FEF1FE,
			  0xFE01FE01FE01FE01,
			  0xE0F1E0F1E0F1E0F1,
			  0xE001E001F101F101,
			  0xFE1FFE1FFE0EFE0E,
			  0x1F011F010E010E01,
			  0xFEE0FEE0FEF1FEF1
			};
			unsigned long long BadKeysDes[] =
			{
				0x0101010101010101,
				0xFEFEFEFEFEFEFEFE,
				0x1F1F1F1F0E0E0E0E,
				0xE0E0E0E0F1F1F1F1
			};

			switch (KeyNumber)
			{
			case 1:
				for (unsigned int i = 0; i < 4; ++i)
					if (K.Full_Block == BadKeysDes[i])
						return false;
				break;

			case 2:
				for (unsigned int i = 0; i < 6; ++i)
					if (K.Full_Block == BadKeys3Des[i])
						return false;
				break;

			case 3:
				for (unsigned int i = 6; i < 12; ++i)
					if (K.Full_Block == BadKeys3Des[i])
						return false;
				break;

			default:
				return false;
			}
			return true;
		}

		void SetKey(Block_64b Key)
		{
			GenerateDesKey(Key.Full_Block);
		}
		
		key() {};

		key(Block_64b &Key)
		{
			GenerateDesKey(Key.Full_Block);//Ключі Раунду
		}

		void ReversKeys()
		{
			Block_64b TimeKey[16];
			for (int i = 0; i < 16; i++)
				TimeKey[i] = RKey[15 - i];
			for (int i = 0; i < 16; i++)
				RKey[i] = TimeKey[i];
		}

		Block_64b	GetKey(unsigned int i)
		{
			return RKey[i];
		}

		inline void operator= (key &K)
		{
			for (int i = 0; i < 16; ++i)
				RKey[i] = K.RKey[i];
		}
};
#endif
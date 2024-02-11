#pragma once
#ifndef BLOCK_H
#define BLOCK_H
union Block_64b
{
	unsigned long long Full_Block; //Блок повністю
	struct 
	{
		unsigned int LowPartOfBlock; // Молодші 32 байти
		unsigned int HighPartOfBlock; // Старші 32 байти
	};
};
#endif
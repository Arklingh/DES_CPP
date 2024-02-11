#pragma once
#include "InformationBlock.h"
#ifndef ENCRYPTION_H
#define ENCRYPTION_H
template<class T> class Encryption 
{
   public:
	   virtual T EncryptTheFile(T Block) = 0;

};
#endif
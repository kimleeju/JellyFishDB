#include "SkipList.h"
#include "ConcurrentSkipList.h"

string SkipList::randomString()
{ // maxLength and charIndex can be customized, but I've also initialized them.
    //uint length = rand() % maxLength + 1; // length of the string is a random value that can be up to 'l' characters.
    uint maxLength = 15; 
    std::string charIndex = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    uint length = random()%(maxLength -1) + 1;
    uint indexesOfRandomChars[maxLength]; // array of random values that will be used to iterate through random indexes of 'charIndex'
    for (uint i = 0; i < length; ++i) // assigns a random number to each index of "indexesOfRandomChars"
        indexesOfRandomChars[i] = rand() % charIndex.length();

    string randomString = ""; // random string that will be returned by this function
    for (uint i = 0; i < length; ++i)// appends a random amount of random characters to "randomString"
    {
        randomString += charIndex[indexesOfRandomChars[i]];
    } 
   return randomString;
}


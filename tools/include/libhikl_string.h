#ifndef __LIBHIKL_STRING_H__
#define __LIBHIKL_STRING_H__

void * memcpy(void *pDestination, const void *pSource, int num);
//------------------------------------------------------------------------------
/// Fills a memory region with the given value. Returns a pointer to the
/// memory region.
/// \param pBuffer  Pointer to the start of the memory region to fill
/// \param value    Value to fill the region with
/// \param num      Size to fill in bytes
//------------------------------------------------------------------------------
void * memset(void *pBuffer, int value, int num);

//-----------------------------------------------------------------------------
/// Search a character in the given string.
/// Returns a pointer to the character location.
/// \param pString   Pointer to the start of the string to search.
/// \param character The character to find.
//-----------------------------------------------------------------------------
char * strchr(const char *pString, int character);

//-----------------------------------------------------------------------------
/// Return the length of a given string
/// \param pString Pointer to the start of the string.
//-----------------------------------------------------------------------------
int strlen(const char *pString);

//-----------------------------------------------------------------------------
/// Search a character backword from the end of given string.
/// Returns a pointer to the character location.
/// \param pString   Pointer to the start of the string to search.
/// \param character The character to find.
//-----------------------------------------------------------------------------
char * strrchr(const char *pString, int character);
//-----------------------------------------------------------------------------
/// Copy from source string to destination string
/// Return a pointer to the destination string
/// \param pDestination Pointer to the destination string.
/// \param pSource      Pointer to the source string.
//-----------------------------------------------------------------------------
char * strcpy(char *pDestination, const char *pSource);

//-----------------------------------------------------------------------------
/// Compare the first specified bytes of 2 given strings
/// Return 0 if equals
/// Return >0 if 1st string > 2nd string
/// Return <0 if 1st string < 2nd string
/// \param pString1 Pointer to the start of the 1st string.
/// \param pString2 Pointer to the start of the 2nd string.
/// \param count    Number of bytes that should be compared.
//-----------------------------------------------------------------------------
int strncmp(const char *pString1, const char *pString2, int count);

//-----------------------------------------------------------------------------
/// Copy the first number of bytes from source string to destination string
/// Return the pointer to the destination string.
/// \param pDestination Pointer to the start of destination string.
/// \param pSource      Pointer to the start of the source string.
/// \param count        Number of bytes that should be copied.
//-----------------------------------------------------------------------------
char * strncpy(char *pDestination, const char *pSource, int count);


#endif /* __LIBHIKL_STRING_H__ */

#ifndef __STRING_SEARCH_H__
#define __STRING_SEARCH_H__

#include <Windows.h>
#include <mbstring.h>

bool IsSimplifiedCH();

// DWORD IsAscii(byte* pbMem,bool& bAscii);
// bool  IsByteAscii(byte byAsc);
// bool IsAsciiEx(byte* pbMem, DWORD& dwLen);
// bool IsAsciiRepeat(byte* pbMem, DWORD dwLength, DWORD& dwRepeatLen);

bool IsSimplifiedCH(WORD wChar);
bool IsByteAscii(byte byAsc);
DWORD GetUnicodeAsciiLen(const BYTE* pMem, DWORD dwBufLen);
DWORD IsAscii( BYTE* pbMem, BOOL& bAscii );
// �ж��Ƿ����ַ�,���������ַ�
bool IsAsciiOrUnicodeString(byte* pbMem, DWORD& dwLen, BOOL& bIsUnicode, DWORD dwMaxCount);

/*
*	@brief
*       �ж��Ƿ����ַ�����unicode
*	@param
*       pbMem       ���ж��ڴ�
*       dwMemLen    pbMem�Ĵ�С
*       dwLen       ������ַ�����unicode,���ų���(in byte)
*       bIsUnicode  ����Ƿ���unicode
*	@ret
*       �Ƿ���unicode��ascii
*	@comment
*       
*/
BOOL IsAsciiOrUnicodeStringEx(byte* pbMem,
                              DWORD dwMemLen, 
                              DWORD& dwLen, 
                              BOOL& bIsUnicode);

#define MAX_CHECK_STRING_LEN    4096

#endif

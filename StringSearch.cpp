#include "StringSearch.h"

bool IsSimplifiedCH(WORD wChar)
{
	byte bHigh = 0;
	byte bLow = 0;

	bHigh = wChar & 0xff;
	bLow = wChar /  0x100;

	if ((bHigh >= 0xa1 && bHigh <= 0xf7) && (bLow >= 0xa1 && bLow <= 0xfe))
	{
		return true;
	}

	return false;
}

// �ж��ַ����Ƿ���ascii
bool IsByteAscii(byte byAsc)
{
	return (byAsc >= 32 && byAsc <= 127) || _ismbcprint((byAsc) != 0);
// 	return (byAsc >= 32 && byAsc <= 127) 
// 		|| _ismbcprint((byAsc) != 0 
// 		|| byAsc == 0x0d 
// 		|| byAsc == 0x0a
// 		|| byAsc == 0x09);
}

/*
bool  IsByteAscii(byte byAsc)
{
	if (_ismbcprint((byAsc) != 0))
	{
		return true;
	}
	return true;
}
*/

DWORD GetUnicodeAsciiLen(const BYTE* pMem, DWORD dwBufLen)
{
    DWORD dwReturnLen = 0;

    for (dwReturnLen = 0; dwReturnLen < dwBufLen; dwReturnLen += 2)
    {
        if (pMem[dwReturnLen + 1] != 0)
        {
            break;
        }
        else
        {
            if (pMem[dwReturnLen] == 0)
            {
                break;
            }
            else if (!IsByteAscii(pMem[dwReturnLen]) && pMem[dwReturnLen] != 0x20)  // �ж�pMem[i]�Ƿ����ַ�����ո�
            {
                break;
            }
        }
    }

    return dwReturnLen;
}

DWORD IsAscii( BYTE* pbMem,BOOL& bAscii )
{
	bAscii = false;
	WORD wChar = 0;
	DWORD dwLen = 0;

	try
	{
		do 
		{
			wChar = *(WORD*)(pbMem + dwLen);
			if (IsSimplifiedCH(wChar))
			{
				dwLen += 2;
			}
			else if (_ismbcprint((byte)wChar) !=0 ||
					(byte)wChar == 0x0d ||
					(byte)wChar == 0x0a ||
					(byte)wChar == 0x09)
			{
				dwLen++;
			}
			else
			{
				if (dwLen != 0 && (byte)wChar == 0)
				{
					dwLen++;
					bAscii = true;
				}

				break;
			}
		} while (true);
	}
	catch(...)
	{
		return 0;
	}

	return dwLen;
}

// �ж��Ƿ����ַ�,���������ַ�
bool IsAsciiOrUnicodeString(byte* pbMem, DWORD& dwLen, BOOL& bIsUnicode, DWORD dwMaxCount)
{
	bool bAscii = false;
	byte byChar = 0;
	WORD wChar = 0;

	dwLen = 0;
    bIsUnicode = FALSE;

    try
    {
        dwLen = GetUnicodeAsciiLen(pbMem, dwMaxCount);
    }
    catch(...)
    {
    }

    if (dwLen > 10)
    {
        bIsUnicode = TRUE;
        return true;
    }
	dwLen = 0;
	try
	{
		do 
		{
			if (dwLen >= dwMaxCount)
			{
                bAscii = true;
				break;
			}

			byChar = pbMem[dwLen];
			wChar = *(WORD*)(pbMem + dwLen);
			if (IsSimplifiedCH(wChar))
			{
				dwLen += 2;
			}
			else if (IsByteAscii(byChar))
			{
				dwLen++;
			}
			else
			{
				if (dwLen != 0 && (((byte)wChar == 0) 
					|| (byte)wChar  == 0x0d 
					|| (byte)wChar  == 0x0a
					|| (byte)wChar  == 0x09
					|| wChar == 0x0a0d))
//				if (dwLen != 0 && (((byte)wChar == 0)))
				{
					bAscii = true;
				}
				else
				{
					dwLen = 0;
				}

				break;
			}
		} while (true);
	}
	catch(...)
	{
        bAscii = false;
	}

	return bAscii;
}


// �ж��Ƿ����ַ�,���������ַ�
BOOL IsAsciiOrUnicodeStringEx(byte* pbMem,
                              DWORD dwMemLen, 
                              DWORD& dwLen, 
                              BOOL& bIsUnicode)
{
    BOOL bAscii = FALSE;
    byte byChar = 0;
    WORD wChar = 0;

    dwLen = 0;
    bIsUnicode = FALSE;

    try
    {
        dwLen = GetUnicodeAsciiLen(pbMem, dwMemLen);
    }
    catch(...)
    {
    }

    if (dwLen > 10)
    {
        bIsUnicode = TRUE;
        return TRUE;
    }

    //
    // ��UNICODE�ַ���
    //

    dwLen = 0;

    while (dwLen < dwMemLen)
    {
        if (dwLen > 4096)
        {
            break;
        }

        byChar = pbMem[dwLen];

        if (dwLen < dwMemLen - 1)
        {
            //
            // ��ʣ�³���2�ֽ�,��Ҫ�ж��Ƿ���SimplifiedCH
            //

            wChar = *(WORD*)(pbMem + dwLen);

            if (IsSimplifiedCH(wChar))
            {
                //
                // ��SimplifiedCH,����ֱ�Ӽ�2,������һ���ж�
                //
                dwLen += 2;
                continue;
            }
        }
        else
        {
            //
            // ֻʣ��һ���ֽ�,������wCharΪ���һ���ֽ��Ա�����ж�
            //

            wChar = byChar;
        }

        if (IsByteAscii(byChar))
        {
            dwLen++;
        }
        else
        {
            if (dwLen != 0 && (((byte)wChar == 0) 
                || (byte)wChar == 0x0d 
                || (byte)wChar == 0x0a
                || (byte)wChar == 0x09
                || wChar == 0x0a0d))
            {
                dwLen++;
                bAscii = TRUE;
            }
            else
            {
                dwLen = 0;
            }

            break;
        }
    }

    if (dwLen == dwMemLen)
    {
        //
        // ������е�byte����ascii��SimplifiedCH,�����ý��ΪTRUE
        //

        bAscii = TRUE;
    }

    return bAscii;
}

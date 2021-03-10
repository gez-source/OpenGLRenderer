/*
Filename:		Text.cpp
Purpose:		Implements the CText class.
Created:		Kimanzi Mati	June 7, 2002 12:55
Last Edited:	Kimanzi Mati	June 7, 2002 12:55	Original code

Copyright (C) 2002 Kimanzi Musili Mati

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the
	Free Software Foundation, Inc.,
	59 Temple Place, Suite 330,
	Boston, MA 02111-1307
	USA
For a copy of the GNU Lesser General Public License on the web, visit:
	http://www.opensource.org/licenses/lgpl-license.html
*/

//#include "stdafx.h"
#include "Text.h"

//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
//#endif

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

#include <ctype.h> 
void ToUp (char *p)
{
    while(*p)
    { 
        *p=toupper( *p ); 
        p++; 
    }
}
void ToLow (char *p)
{
    while(*p)
    { 
        *p=tolower( *p ); 
        p++; 
    }
}
int string_length(char *pointer)
{
   int c = 0;
 
   while( *(pointer + c) != '\0' )
      c++;
 
   return c;
}
void reverse(char *s)
{
   int length, c;
   char *begin, *end, temp;
 
   length = string_length(s);
   begin  = s;
   end    = s;
 
   for (c = 0; c < length - 1; c++)
      end++;
 
   for (c = 0; c < length/2; c++)
   {        
      temp   = *end;
      *end   = *begin;
      *begin = temp;
 
      begin++;
      end--;
   }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CText::CText()
{
	m_Text = (char *) malloc(1000 * sizeof(char));
	if (m_Text != 0)
		std::strcpy(m_Text, "");
}

CText::~CText()
{
	free(m_Text);
}


CText::CText(CText &x) {
	unsigned int	d = strlen(x.m_Text) + 1;
	if (d < 1000) d = 1000;
	m_Text = (char *) malloc(d * sizeof(char));
	if (m_Text != 0)
		strcpy(m_Text, x.m_Text);	
}

CText::CText(std::string sName) {
	unsigned int	d = sName.length() + 1;
	if (d < 1000) d = 1000;
	m_Text = (char *) malloc(d * sizeof(char));
	if (m_Text != 0)
		strcpy(m_Text, sName.c_str());
}

CText::CText(char ch) {
	m_Text = (char *) malloc(1000 * sizeof(char));
	if (m_Text != 0) {
		m_Text[0] = ch;
		m_Text[1] = '\0';
	}
}

CText::CText(char * sName) {
	unsigned int	d = strlen(sName) + 1;
	unsigned int	len = strlen(sName);
	if (d < 1000) d = 1000;
	m_Text = (char *) malloc(d * sizeof(char));
	for (unsigned int c = 0; c < len; c++)
		m_Text[c] = sName[c];
	m_Text[len] = '\0';
}

int CText::operator=(CText &x) {
	unsigned int	d = strlen(x.m_Text) + 1;
	if (m_Text == 0) {
		if (d < 1000) d = 1000;
		m_Text = (char *) malloc(d * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, d * sizeof(char));
	strcpy(m_Text, x.m_Text);	
	return 0;
}

int CText::operator=(char * str) {
	unsigned int	d = strlen(str) + 1;
	if (m_Text == 0) {
		if (d < 1000) d = 1000;
		m_Text = (char *) malloc(d * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, d * sizeof(char));
	strcpy(m_Text, str);
	return 0;
}

int CText::operator=(char str){
    m_Text[0] = str;
    m_Text[1] = '\0';
	return 0;
}

int CText::operator =(unsigned short *x) {
	unsigned int	d = wcslen((const wchar_t*)x) + 1;
	unsigned int	len = wcslen((const wchar_t*)x);
	if (m_Text == 0) {
		if (d < 1000) d = 1000;
		m_Text = (char *) malloc(d * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, d * sizeof(char));
	for (unsigned int c = 0; c < len; c++)
		m_Text[c] = x[c];
	m_Text[len] = '\0';
	return 0;
}

CText CText::operator+(CText x) {
	CText	pStr( *this );
	unsigned int	e = strlen(x.m_Text) + 1;
	if (e < 1000) e = 1000;
	unsigned int	d = strlen(pStr.m_Text) + strlen(x.m_Text) + 1;

	if (pStr.m_Text == 0) {
		pStr.m_Text = (char *) malloc(e * sizeof(char));
		pStr.m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(pStr.m_Text) * sizeof(char))
			pStr.m_Text = (char *) realloc(pStr.m_Text, d * sizeof(char));
	strcat(pStr.m_Text, x.m_Text);
	return pStr;
}

CText CText::operator+(std::string str) {
	CText	pStr( *this );
	unsigned int	e = str.length() + 1;
	if (e < 1000) e = 1000;
	unsigned int	d = strlen(pStr.m_Text) + str.length() + 1;
	
	if (pStr.m_Text == 0) {
		pStr.m_Text = (char *) malloc(e * sizeof(char));
		pStr.m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(pStr.m_Text) * sizeof(char))
			pStr.m_Text = (char *) realloc(pStr.m_Text, d * sizeof(char));
	strcat(pStr.m_Text, str.c_str());
	return pStr;
}

CText CText::operator+(char *str) {
	CText	pStr( *this );
	unsigned int	e = wcslen((const wchar_t*)str) + 1;
	unsigned int	len = wcslen((const wchar_t*)str);
	if (e < 1000) e = 1000;
	unsigned int	d = strlen(pStr.m_Text) + wcslen((const wchar_t*)str) + 1;
	
	if (pStr.m_Text == 0) {
		pStr.m_Text = (char *) malloc(e * sizeof(char));
		pStr.m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(pStr.m_Text) * sizeof(char))
			pStr.m_Text = (char *) realloc(pStr.m_Text, d * sizeof(char));
	unsigned int	offset = strlen(pStr.m_Text);
	for (unsigned int c = 0; c < len; c++)
		pStr.m_Text[c + offset] = str[c];
	pStr.m_Text[len + offset] = '\0';
	return pStr;
}

CText CText::operator+=(CText x) {
	unsigned int	e = strlen(x.m_Text) + 1;
	if (e < 1000) e = 1000;
	unsigned int	d = strlen(m_Text) + strlen(x.m_Text) + 1;

	if (m_Text == 0) {
		m_Text = (char *) malloc(e * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, d * sizeof(char));
	strcat(m_Text, x.m_Text);
	return *this;
}

CText CText::operator+=(std::string str) {
	unsigned int	e = str.length() + 1;
	if (e < 1000) e = 1000;
	unsigned int	d = strlen(m_Text) + str.length() + 1;

	if (m_Text == 0) {
		m_Text = (char *) malloc(e * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, d * sizeof(char));
	strcat(m_Text, str.c_str());
	return *this;
}

CText CText::operator+=(char str) {
	unsigned int	e = strlen(m_Text) + 2;
	if (m_Text == 0) {
		m_Text = (char *) malloc(1000 * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (e > 1000 && e > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, e * sizeof(char));
	unsigned int	d = strlen(m_Text);
	m_Text[d] = str;
	m_Text[d + 1] = '\0';
	return *this;
}

CText CText::operator+=(char *str) {
	unsigned int	e = wcslen((const wchar_t*)str) + 1;
	unsigned int	len = wcslen((const wchar_t*)str);
	if (e < 1000) e = 1000;
	unsigned int	d = strlen(m_Text) + wcslen((const wchar_t*)str) + 1;
	if (m_Text == 0) {
		m_Text = (char *) malloc(e * sizeof(char));
		m_Text[0] = '\0';
	} else
		if (d > 1000 && d > strlen(m_Text) * sizeof(char))
			m_Text = (char *) realloc(m_Text, d * sizeof(char));
	unsigned int	offset = strlen(m_Text);
	for (unsigned int c = 0; c < len; c++)
		m_Text[c + offset] = str[c];
	m_Text[len + offset] = '\0';
	return *this;
}

CText::operator char*() {
	return m_Text;
}

bool CText::Compare(char * s) {
	return strcmp(m_Text, s);
}

bool CText::CompareNoCase(char * s) {
	CText	a(this->m_Text);
	CText	b(s);

	a.MakeLower();
	b.MakeLower();

	return strcmp(a.m_Text, b.m_Text);
}

void CText::MakeLower() {
    ToLow(m_Text);
}

void CText::MakeUpper() {
	ToUp(m_Text);
}

void CText::MakeReverse() {
	reverse(m_Text);
}

//_bstr_t CText::AllocSysString() {
//	return _bstr_t(m_Text);
//}

int	CText::GetLength() {
	return strlen(m_Text);
}

char CText::GetAt(int i) {
	return m_Text[i];
}

void CText::SetAt(int i, char ch) {
	m_Text[i] = ch;
}

CText& CText::Mid(int i) {
	CText	pStr( *this );

	strcpy(pStr.m_Text, (m_Text + i));

	return pStr;
}

CText& CText::Mid(int i, int count) {
	if (count < 0) count = 0;

	CText	pStr( *this );

	strcpy(pStr.m_Text, (m_Text + i));

	unsigned int	d = strlen(pStr);
	if (count > d)
		count = d;

	pStr.m_Text[count] = '\0';
	return pStr;
}

CText CText::Left(int count) {
	if (count < 0){
        CText empty ("");
        return empty;
    }

	CText	pStr( *this );

	unsigned int	d = strlen(pStr);
	if (count > d)
		count = d;

	pStr.m_Text[count] = '\0';
	return pStr;
}

CText CText::Right(int count) {
	if (count < 0) count = 0;

	CText	pStr( *this );

	unsigned int	d = strlen(pStr);
	if (count > d)
		count = 0;

	strcpy(pStr.m_Text, (pStr.m_Text + strlen(pStr.m_Text) - count));
	return pStr;
}

bool CText::Replace(char * sOld, char * sNew) {
	if (strcmp(sOld, sNew) == 0) return true;

	bool	bRet = false;
	while (true) {
		int	i = this->Find(sOld);
		if (i == -1) break;

		bRet = true;

		*this = this->Left(i) + sNew + this->Mid(i + strlen(sOld));
	}

	return bRet;
}

void CText::TrimLeft() {
	unsigned int	i;
	for (i = 0; i < strlen(m_Text); i++)
		if (m_Text[i] != ' ' && m_Text[i] != '\r' && m_Text[i] != '\n' && m_Text[i] != '\t')
			break;

	strcpy(m_Text, (m_Text + i));
}

void CText::TrimRight() {
	unsigned int	i;
	for (i = strlen(m_Text) - 1; i >= 0; i--)
		if (m_Text[i] != ' ' && m_Text[i] != '\r' && m_Text[i] != '\n' && m_Text[i] != '\t')
			break;

	m_Text[i + 1] = '\0';

	strcpy(m_Text, m_Text);
}

int CText::Find(char * sText, int start) {
	if (start >= strlen(m_Text)) return -1;

	if (start < 0) start = 0;

	char	*pOrig = (char *) malloc((strlen(m_Text) + 1) * sizeof(char));
	strcpy(pOrig, (m_Text + start));

	char *pStr = strstr(pOrig, sText);
	if (pStr == NULL) {
		free(pOrig);
		return -1;
	}

	int	ret = -1;

	if (int(pStr) >= int(pOrig))
		ret = start + int (pStr) - int(pOrig);

	free(pOrig);

	return ret;
}

int CText::Find(char sChar, int start) {
	if (start >= strlen(m_Text)) return -1;

	if (start < 0) start = 0;

	char	*pOrig = (char *) malloc((strlen(m_Text) + 1) * sizeof(char));
	strcpy(pOrig, (m_Text + start));

	char *pStr = strstr(pOrig, &sChar);
	if (pStr == NULL) {
		free(pOrig);
		return -1;
	}

	int	ret = -1;

	if (int(pStr) >= int(pOrig))
		ret = start + int(pStr) - int(pOrig);

	free(pOrig);

	return ret;
}

int CText::ReverseFind(char sChar) {
	char *x = strrchr(m_Text, sChar);
	if (x == 0)
		return -1;
	else
		return x - m_Text;
}

int CText::ReverseFind(char *sText) {
	int	iLen = strlen(sText);	// length of string to be found
	if (iLen < 1) return -1;

	char	ch = sText[0];		// ch = 1st char of string to be found
	CText	s(*this);			// s = initial value is this CText object

	char *x;					// x = pointer to the last instance of ch in s
	do {
		x = strrchr(s.m_Text, ch);	// x = result of reverse find for ch in s
		if (x == 0)					// NULL means no ch was not found in s!!
			return -1;
		else {
			if (s.Mid(x - s.m_Text, iLen).Compare(sText) == 0)
				// since this instance of ch is actually also the instance of sText in s
				// then we have found the last instance of sText in s.
				// So return the x - 1st char of s as the position of sText in s!!
				return x - s.m_Text;
			else {
				// since this instance of ch is NOT also the instance of sText in s
				// set s to be only the chars left of x - s.m_Text (i.e. the position of ch in s)!!
				s = s.Left(x - s.m_Text);
			}
		}
	} while (x > 0);

	return -1;
}

std::string CText::ToString(){
    return std::string(m_Text);
}

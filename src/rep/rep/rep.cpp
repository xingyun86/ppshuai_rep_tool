// rep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <iconv.h>
__inline static
int iconv_trans(std::string & tob, const std::string& fromb, const std::string& to, const std::string& from)
{
	size_t fromlen = fromb.size();
	size_t tolen = fromb.size() * sizeof(void *);
	char* pfrom = (char*)(fromb.data());
	char* pto = (tob.resize(tolen, '\0'), (char*)(tob.data()));
	
	iconv_t cd = iconv_open(to.data(), from.data());
	if (cd == (iconv_t)-1)
	{
		perror("iconv_open");
		return (-1);
	}
	size_t ret = iconv(cd, &pfrom, &fromlen, &pto, &tolen);
	if (ret == -1)
	{
		perror("iconv");
		return (-1);
	}
	tob = tob.c_str();
	return iconv_close(cd);
}
__inline static
int unicode_to_utf8(std::string& tob, const std::string& fromb)
{
	return iconv_trans(tob, fromb, "UTF-8//IGNORE", "UNICODE");
}
__inline static
int utf8_to_unicode(std::string& tob, const std::string& fromb)
{
	return iconv_trans(tob, fromb, "UNICODE//IGNORE", "UTF-8");
}
__inline static
int gb2312_to_utf8(std::string& tob, const std::string& fromb)
{
	return iconv_trans(tob, fromb, "UTF-8//IGNORE", "GB2312");
}
__inline static
int utf8_to_gb2312(std::string& tob, const std::string& fromb)
{
	return iconv_trans(tob, fromb, "GB2312//IGNORE", "UTF-8");
}
__inline static
size_t file_reader(std::string& data, const std::string& filename, const std::string& mode = "rb")
{
#define DATA_BASE_SIZE	0x10000

	FILE* pF = 0;
	size_t size = 0;

	pF = fopen(filename.c_str(), mode.c_str());
	if (pF)
	{
		while (!feof(pF))
		{
			data.resize(data.size() + DATA_BASE_SIZE);
			size += fread((void*)(data.c_str() + data.size() - DATA_BASE_SIZE), sizeof(char), DATA_BASE_SIZE, pF);
		}
		data.resize(size);
		fclose(pF);
		pF = 0;
	}

	return size;

#undef DATA_BASE_SIZE
}
__inline static
size_t file_writer(const std::string& data, const std::string& filename, const std::string& mode = "wb")
{
	FILE* pF = 0;
	size_t size = 0;

	pF = fopen(filename.c_str(), mode.c_str());
	if (pF)
	{
		size = fwrite((void*)(data.c_str()), sizeof(char), data.size(), pF);
		fclose(pF);
		pF = 0;
	}

	return size;
}
__inline static
std::string string_replace_all(std::string& strData, const std::string& strDst, const std::string& strSrc, std::string::size_type stPos = 0)
{
	while ((stPos = strData.find(strSrc, stPos)) != std::string::npos)
	{
		strData.replace(stPos, strSrc.length(), strDst);
		stPos += strDst.length() - strSrc.length();
	}

	return strData;
}

int main(int argc, char ** argv)
{
	if (argc == 6)
	{
		const char* srcfile = argv[1];
		const char* dstfile = argv[2];
		const char* srcdata = argv[3];
		const char* dstdata = argv[4];
		const char* codepage = argv[5];
		std::string data("");
		std::string srcStr(srcdata);
		std::string dstStr(dstdata);
		file_reader(data, srcfile);

		if ((std::string("utf-8").compare(codepage) == 0) 
			|| 
			(std::string("UTF-8").compare(codepage) == 0))
		{
			gb2312_to_utf8(srcStr, srcdata);
			gb2312_to_utf8(dstStr, dstdata);
		}
		string_replace_all(data, dstStr, srcStr);
		file_writer(data, dstfile);
	}
	else
	{
		std::cout << "Usage:\n\trep [src file] [dst file] [src str] [dst str] [codepage=gb2312|utf-8]\n";
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

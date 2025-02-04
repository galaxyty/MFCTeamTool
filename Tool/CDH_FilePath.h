#pragma once
#include "Include.h"

class CDH_FilePath
{
public:
	static CString Convert_RelativePath(CString strFullPath);

	static void		DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathInfoList);
	static int		DirFileCount(const wstring& wstrPath);
};

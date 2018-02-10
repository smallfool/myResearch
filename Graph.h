#pragma once
#include "Lock.h"

class CGraph
{
public:
	CGraph();
	~CGraph();
	static ATOM Register();

	static LRESULT CALLBACK GraphWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class CGraphData
{
public:
	CGraphData();
	~CGraphData();

public:
	CLock m_Lock;

	UINT	m_Index;
	COLORREF m_Color;
};

struct CSimpleGraphData
{
	TCHAR* Name;
	double GraphData;
	UINT ID;
	UINT Index;
};

extern CGraphData gpGraphClassData[];
extern CSimpleGraphData gptrIOGraphData;
extern CSimpleGraphData gptrNetGraphData;
extern CSimpleGraphData gptrDiskGraphData;

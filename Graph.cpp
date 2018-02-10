#include "stdafx.h"
#include "..\include\Graph.h"


CGraph::CGraph()
{
}


CGraph::~CGraph()
{
}
ATOM CGraph::Register()
{
	return (ATOM)(0);
}

LRESULT CALLBACK CGraph::GraphWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//////////////////////////////////////////////////////////////
CGraphData gpGraphClassData[6];
CSimpleGraphData gptrIOGraphData;
CSimpleGraphData gptrNetGraphData;
CSimpleGraphData gptrDiskGraphData;

CGraphData::CGraphData()
{

}

CGraphData::~CGraphData()
{

}
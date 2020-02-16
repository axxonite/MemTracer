// MainTracer.h
//

#pragma once

#include "Vector.h"

struct CDataNode
{
	union
	{
		DWORD	dwHeapBlock;
		DWORD	dwModuleAddr;
	};
	DWORD				dwBlockSize;
	CVector<CDataNode*>	PointedBy;
	CVector<CDataNode*>	PointsTo;
	CVector<DWORD>		PtrOffsets;

	DWORD				dwSolutionDist;
	bool				bFlag;
	bool				bFlag2;
	DWORD				dwTag;
	HTREEITEM			hTreeItem;
	DWORD				dwDataPaths;

	inline CDataNode();
};

struct CModuleInfo : public CDataNode
{
	char*	pName;

	CModuleInfo()											{ pName = NULL;							};
	~CModuleInfo()											{ delete[] pName;						};

};

struct CTodoItem
{
	CDataNode*	pNode;
	CTodoItem*	pNext;
	DWORD		dwDepth;
	HTREEITEM	hTreeItem;

	CTodoItem()												{ pNext = NULL;							};
};

struct CSegmentInfo
{
	DWORD	dwStart;
	DWORD	dwLength;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CMainTracer
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class CMainTracer
{

public :

	CMainTracer();

//	========================================== OPERATIONS =========================================

	bool	StartTrace( DWORD dwProcessID, DWORD dwTargetAddress, CTreeCtrl* pTreeCtrl );

//	========================================== ACCESS =============================================

protected :

	HANDLE					m_hProcess;
	DWORD					m_dwProcessID;
	CTreeCtrl*				m_pTreeCtrl;
	CVector<CModuleInfo*>	m_Modules;
	DWORD					m_dwTarget;
	
	CTodoItem*				m_pFirstItem;
	CTodoItem*				m_pLastItem;
	CTodoItem*				m_pTodoFreeList;

	CVector<CDataNode*>		m_Nodes;

	CVector<CSegmentInfo>	m_SegmentMap;
	BYTE*					m_Snapshot[0xFFFFF];
	DWORD					m_dwTotalDataPaths;

	inline	CTodoItem*	AllocTodoItem();

	void	CountDataPaths( CDataNode* pNode, DWORD& dwValidLinks );

	void	FillTree();

	void	FindPotentialRoots();

	void	TagSolution( CDataNode* pNode, DWORD dwDist );

	bool	TraceForward();

private :

};

CDataNode::CDataNode() :
	bFlag		( false			),
	bFlag2		( false			),
	dwTag		( 0xFFFFFFFF	),
	hTreeItem	( 0				),
	dwDataPaths	( 0				)
{
}

CTodoItem* CMainTracer::AllocTodoItem()
{
	if ( m_pTodoFreeList )
	{
		CTodoItem* pTodo = m_pTodoFreeList;
		m_pTodoFreeList = pTodo->pNext;
		pTodo->pNext = NULL;
		return pTodo;
	}
	else return new CTodoItem;
}
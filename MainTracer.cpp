// MainTracer.cpp
//

#include "stdafx.h"
#include "MainTracer.h"
#include "MainFrm.h"
#include <tlhelp32.h>
#include <assert.h>

static DWORD s_dwRefCount = 0;
static DWORD s_dwLevel = 0;
static DWORD s_dwCircularRefs = 0;

// ==========================================================================
CMainTracer::CMainTracer()
{
	m_Nodes.SetGrowSize( 64 );
}

// ==========================================================================
void CMainTracer::CountDataPaths( CDataNode* pNode, DWORD& dwValidLinks )
{
	return;

	dwValidLinks = 0;
	if ( pNode->dwSolutionDist == 0 )
	{
		dwValidLinks = 1;
		pNode->dwDataPaths++;
		m_dwTotalDataPaths++;
		return;
	}
	pNode->bFlag2 = true;
	for ( DWORD i = 0; i < pNode->PointsTo.size(); i++ )
	{
		if ( ( pNode->PointsTo[i]->dwSolutionDist != 0xFFFFFFFF ) && ( !pNode->PointsTo[i]->bFlag2 ) )
		{
			DWORD dwChildValidLinks;
			CountDataPaths( pNode->PointsTo[i], dwChildValidLinks );
			if ( dwChildValidLinks > 0 ) dwValidLinks++;
		}
	}

	if ( dwValidLinks > 0 ) pNode->dwDataPaths++;										// <-- Everything is circular starting from here

	pNode->bFlag2 = false;
}

// ==========================================================================
void CMainTracer::FillTree()
{
	CTodoItem* pTodo = m_pFirstItem;
	for ( DWORD i = 0; i < pTodo->pNode->PointsTo.size(); i++ )
	{
		CDataNode* pChildNode = pTodo->pNode->PointsTo[i];
		char msg[200];
		HTREEITEM hItem;
		if ( pChildNode->dwSolutionDist != 0xFFFFFFFF ) 
		{
			sprintf(	msg, "Ref. to heap block 0x%.8x - size %.8x, at offset %.8x - %d hops, %d/%d datapaths %s", pChildNode->dwHeapBlock, pChildNode->dwBlockSize, 
						pTodo->pNode->PtrOffsets[i], pChildNode->dwSolutionDist, pChildNode->dwDataPaths, m_dwTotalDataPaths, pChildNode->bFlag ? "(alias)" : "" );
			hItem = m_pTreeCtrl->InsertItem( msg, pTodo->hTreeItem );
			m_pTreeCtrl->SetItemState( hItem, TVIS_BOLD, TVIS_BOLD );
		}
		else
		{
			sprintf(	msg, "Ref. to heap block 0x%.8x - size %.8x, at offset %.8x", pChildNode->dwHeapBlock, pChildNode->dwBlockSize, 
						pTodo->pNode->PtrOffsets[i], pChildNode->bFlag ? "(alias)" : "" );
			hItem = m_pTreeCtrl->InsertItem( msg, pTodo->hTreeItem );
		}

		if ( !pChildNode->bFlag )
		{
			pChildNode->hTreeItem = hItem;
			pChildNode->bFlag = true;
		
			CTodoItem* pNewTodo = AllocTodoItem();
			pNewTodo->pNode		= pChildNode;
			pNewTodo->dwDepth	= pTodo->dwDepth + 1;
			pNewTodo->hTreeItem	= hItem;
			m_pLastItem->pNext = pNewTodo;
			m_pLastItem = pNewTodo;
		}
		else m_pTreeCtrl->SetItemData( hItem, (DWORD)pChildNode->hTreeItem );
	}

	m_pFirstItem	= pTodo->pNext;
	pTodo->pNext	= m_pTodoFreeList;
	m_pTodoFreeList	= pTodo;
}

// ==========================================================================
void CMainTracer::FindPotentialRoots()
{
	char msg[100];
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, m_dwProcessID );
	if ( !hSnapshot ) return;

	MODULEENTRY32 module;
	module.dwSize = sizeof( module );
	BOOL bResult = Module32First( hSnapshot, &module );

	DWORD dwTotalCount = 0;
	while ( bResult )
	{
		if ( strcmp( module.szModule, "bbr.dll" ) != 0 )
		{
			bResult = Module32Next( hSnapshot, &module );
			continue;
		}
		DWORD			dwCount		= 0;
		DWORD			dwEnd		= (DWORD)module.modBaseAddr + (DWORD)module.modBaseSize;
		DWORD			dwEndPage	= ( dwEnd & 0xFFFFF000 ) / 0x1000;
		CModuleInfo*	pModule		= NULL;
		for ( DWORD i = ( (DWORD)module.modBaseAddr & 0xFFFFF000 ) / 0x1000; i < dwEndPage; i++ )
		{
			if ( m_Snapshot[i] )
			{
				BYTE* pPtr = m_Snapshot[i];

				for ( DWORD k = 0; k < 0x1000; k ++, pPtr++ )
				{
					// Check if that address might be a heap block

					DWORD* pPtr2 = (DWORD*)pPtr;
					if ( *pPtr2 > 0x80000000 || *pPtr2 < 0x10000 ) continue;
					
					DWORD dwPage = ( ( *pPtr2 - 12 ) & 0xFFFFF000 ) / 0x1000;
					if ( !m_Snapshot[dwPage] ) continue;
					DWORD* pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr2 - 12 ) & 0x00000FFF]);
					if ( *pTemp == 0x00000000 )
					{
						dwPage = ( ( *pPtr2 - 16 ) & 0xFFFFF000 ) / 0x1000;
						if ( !m_Snapshot[dwPage] ) continue;
						pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr2 - 16 ) & 0x00000FFF]);
						if ( *pTemp  == 0x00000000 )
						{
							dwPage = ( ( *pPtr2 - 20 ) & 0xFFFFF000 ) / 0x1000;
							if ( !m_Snapshot[dwPage] ) continue;
							pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr2 - 20 ) & 0x00000FFF]);
							if ( *pTemp  == 0xFEEEFEEE || *pTemp  == 0xABABABAB )
							{
								dwPage = ( ( *pPtr2 - 24 ) & 0xFFFFF000 ) / 0x1000;
								if ( !m_Snapshot[dwPage] ) continue;
								pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr2 - 24 ) & 0x00000FFF]);
								if ( *pTemp  == 0xABABABAB )
								{
									// Yup, it is
									DWORD Addr = i * 0x1000 + k;

									if ( !pModule )
									{
										pModule = new CModuleInfo();
										pModule->dwModuleAddr = (DWORD)module.modBaseAddr;
										pModule->pName = strdup( module.szModule );
										pModule->dwSolutionDist = 0xFFFFFFFF;
										m_Modules.push_back( pModule );
										m_Nodes.push_back( pModule );
									}

									CDataNode* pNode = new CDataNode;
									pNode->dwHeapBlock = *pPtr2;
									pNode->PointedBy.push_back( pModule );
									pNode->dwSolutionDist = 0xFFFFFFFF;
									m_Nodes.push_back( pNode );
									pModule->PointsTo.push_back( pNode );
									pModule->PtrOffsets.push_back( Addr - pModule->dwModuleAddr );

									CTodoItem* pTodo	= new CTodoItem;
									pTodo->pNode		= pNode;
									pTodo->dwDepth		= 1;
									if ( !m_pFirstItem ) m_pFirstItem = pTodo;
									else m_pLastItem->pNext = pTodo;
									m_pLastItem = pTodo;
									dwCount++;
									dwTotalCount++;
									s_dwRefCount++;
								}
							}
						}
					}
				}
			}
		}

		if ( dwCount > 0 )
		{
			sprintf( msg, "Found %d roots in module '%s'\r\n", dwCount, module.szModule );
			CMainFrame::ConsoleOutput( msg );
		}

		bResult = Module32Next( hSnapshot, &module );
	}

	sprintf( msg, "\r\nTotal roots : %d\r\n", dwTotalCount );
	CMainFrame::ConsoleOutput( msg );

	CloseHandle( hSnapshot );
}

// ==========================================================================
bool CMainTracer::StartTrace( DWORD dwProcessID, DWORD dwTargetAddress, CTreeCtrl* pTreeCtrl )
{
	m_pTodoFreeList = NULL;
	m_dwTotalDataPaths = 0;

	ZeroMemory( m_Snapshot, sizeof( m_Snapshot ) );
	char msg[255];
	sprintf( msg, "Opening process id %.8x\r\n", dwProcessID );
	CMainFrame::ConsoleOutput( msg );

	m_dwProcessID	= dwProcessID;
	m_pTreeCtrl		= pTreeCtrl;
	m_dwTarget		= dwTargetAddress;
	m_hProcess = OpenProcess( PROCESS_VM_READ, TRUE, dwProcessID );
	if ( !m_hProcess ) return false;

	DWORD dwPages = 0;

	CSegmentInfo curSegment;
	curSegment.dwStart = 0;
	curSegment.dwLength = 0;

	CMainFrame::ConsoleOutput( "Taking memory snapshot...\r\n" );
	DWORD i, j;
	for ( i = 0x00000000, j = 0; i < 0x10000000; i+=0x1000, j++ )
	{
		BYTE buffer[0x1000];
		DWORD dwRead;
		if ( ReadProcessMemory( m_hProcess, (void*)i, &buffer, 0x1000, &dwRead ) )
		{
			m_Snapshot[j] = new BYTE[0x1004];
			memcpy( m_Snapshot[j], buffer, 0x1000 );
			
			if ( curSegment.dwLength == 0 ) curSegment.dwStart = i;
			curSegment.dwLength += 0x1000;
			if ( curSegment.dwLength >= 0x10000 )
			{
				m_SegmentMap.push_back( curSegment );
				curSegment.dwLength = 0;
			}

			dwPages++;
			//sprintf( msg, "Valid segment at 0x%.4x0000\r\n", i );
			//CMainFrame::ConsoleOutput( msg );
		}
		else
		{
			if ( curSegment.dwLength > 0 ) m_SegmentMap.push_back( curSegment );
			curSegment.dwLength = 0;
		}
	}
	if ( curSegment.dwLength > 0 ) m_SegmentMap.push_back( curSegment );
	
	sprintf( msg, "Found %d valid memory pages occupying up to %.1f megabytes\r\n", dwPages, (float)(dwPages / 256.0f) );
	CMainFrame::ConsoleOutput( msg );
	sprintf( msg, "in %d contiguous segments\r\n", m_SegmentMap.size() );
	CMainFrame::ConsoleOutput( msg );

	m_pFirstItem = NULL;
	FindPotentialRoots();


	while( m_pFirstItem ) TraceForward();


	CMainFrame::ConsoleOutput( "Counting datapaths...\r\n\r\n" );
	for ( i = 0; i < m_Modules.size(); i++ )
	{
		DWORD dwTemp;
		if ( m_Modules[i]->dwSolutionDist != 0xFFFFFFFF ) CountDataPaths( m_Modules[i], dwTemp );
	}

	sprintf( msg, "%d unique datapaths found.\r\n", m_dwTotalDataPaths );
	CMainFrame::ConsoleOutput( msg );

	CMainFrame::ConsoleOutput( "Closing process.\r\n\r\n" );
	CloseHandle( m_hProcess );

	CMainFrame::ConsoleOutput( "Building tree...\r\n\r\n" );

	HTREEITEM hItem;
	for ( i = 0; i < m_Modules.size(); i++ )
	{
		if ( m_Modules[i]->dwSolutionDist != 0xFFFFFFFF ) 
		{
			sprintf( msg, "Module '%s', base address 0x%.8x, %d hops", m_Modules[i]->pName, m_Modules[i]->dwModuleAddr, m_Modules[i]->dwSolutionDist );
			hItem = pTreeCtrl->InsertItem( msg );
			pTreeCtrl->SetItemState( hItem, TVIS_BOLD, TVIS_BOLD );
		}
		else
		{
			sprintf( msg, "Module '%s', base address 0x%.8x", m_Modules[i]->pName, m_Modules[i]->dwModuleAddr );
			hItem = pTreeCtrl->InsertItem( msg );
		}

		CTodoItem* pItem	= AllocTodoItem();
		pItem->pNode		= m_Modules[i];
		pItem->dwDepth		= 1;
		pItem->hTreeItem	= hItem;
		if ( !m_pFirstItem ) m_pFirstItem = pItem;
		else m_pLastItem->pNext = pItem;
		m_pLastItem = pItem;
	}


	while ( m_pFirstItem ) FillTree();


	// Cleanup
	for ( i = 0; i < m_Nodes.size(); i++ ) delete m_Nodes[i];
	m_Nodes.clear();

	CTodoItem* pItem = m_pTodoFreeList;
	while ( pItem )
	{
		CTodoItem* pNext = pItem->pNext;
		delete pItem;
		pItem = pNext;
	}

	CMainFrame::ConsoleOutput( "Freeing snapshot...\r\n\r\n" );
	//for ( i = 0; i < 0xFFFFF; i++ ) if ( m_Snapshot[i] ) delete[] m_Snapshot[i];
	CMainFrame::ConsoleOutput( "Snapshot freed.\r\n\r\n" );

	return true;
}

// ==========================================================================
void CMainTracer::TagSolution( CDataNode* pNode, DWORD dwDist )
{
	pNode->dwSolutionDist = dwDist;
	dwDist++;
	for ( DWORD i = 0; i < pNode->PointedBy.size(); i++ )
	{
		if ( pNode->PointedBy[i]->dwSolutionDist > dwDist ) 
		{
			TagSolution( pNode->PointedBy[i], dwDist );
		}
	}
}

DWORD s_dwTest = 0;


// ==========================================================================
bool CMainTracer::TraceForward()
{
	// :NOTE: We assume non-packed

	s_dwTest++;
	CTodoItem* pTodoItem = m_pFirstItem;

	char msg[100];
	if ( s_dwLevel != pTodoItem->dwDepth )
	{
		s_dwLevel = pTodoItem->dwDepth;
		sprintf(	msg, "Level %d references : %d, %d circular - %d tests.\r\n", s_dwLevel, s_dwRefCount, s_dwCircularRefs, s_dwTest );
		CMainFrame::ConsoleOutput( msg );
		s_dwRefCount = 0;
		s_dwCircularRefs = 0;
		s_dwTest = 0;
	}
	
	//if ( pTodoItem->dwDepth < 10 ) 
	{
		if ( pTodoItem->pNode->dwHeapBlock == m_dwTarget )
		{
			pTodoItem->pNode->dwSolutionDist = 0;
			//TagSolution( pTodoItem->pNode, 0 );
		}

		DWORD Addr = pTodoItem->pNode->dwHeapBlock - ( pTodoItem->pNode->dwHeapBlock % 4 );
		DWORD dwIndex = Addr & 0x00000FFF;
		DWORD dwPage;
		DWORD* pTemp;

		DWORD dwRefCount = 0;
		for ( DWORD i = ( Addr & 0xFFFFF000 ) / 0x1000; i <= 0xFFFFF; i++ )
		{
			if ( m_Snapshot[i] )
			{
				DWORD* pPtr = (DWORD*)&m_Snapshot[i][dwIndex];

				for ( DWORD k = dwIndex; k < 0x1000; k+= 4, pPtr++ )
				{
					// Check if that address might be a heap block
					// Is adress in range ?
					if ( *pPtr < 0x80000000 && *pPtr > 0x10000 )
					{		
						if ( *pPtr == m_dwTarget )
						{
							// Found it
							CMainFrame::ConsoleOutput( "Valid Data Path Found!!!!\r\n" );

							CDataNode* pNode = new CDataNode;
							pNode->dwHeapBlock = *pPtr;
							pNode->dwSolutionDist = 0;
							m_Nodes.push_back( pNode );
							//*pTemp = (DWORD)pNode;							// Store our data node ptr in no man's land
							pNode->PointedBy.push_back( pTodoItem->pNode );
							pTodoItem->pNode->PointsTo.push_back( pNode );
							Addr = i * 0x1000 + k;
							pTodoItem->pNode->PtrOffsets.push_back( Addr - pTodoItem->pNode->dwHeapBlock );

							TagSolution( pNode, 0 );
						}

						// Check for first tag
						dwPage = ( ( *pPtr - 12 ) & 0xFFFFF000 ) / 0x1000;
						if ( m_Snapshot[dwPage] )
						{
							pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr - 12 ) & 0x00000FFF]);
							DWORD* pTemp2 = pTemp;
							bool bCircularLink = ( *pTemp == 0xFFFFFFFF ); 
							if ( *pTemp == 0x00000000 || bCircularLink )
							{
								// Check for second tag
								dwPage = ( ( *pPtr - 16 ) & 0xFFFFF000 ) / 0x1000;
								if ( m_Snapshot[dwPage] )
								{
									pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr - 16 ) & 0x00000FFF]);
									if ( *pTemp  == 0x00000000 )
									{
										// Check for third tag
										dwPage = ( ( *pPtr - 20 ) & 0xFFFFF000 ) / 0x1000;
										if ( m_Snapshot[dwPage] )
										{
											pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr - 20 ) & 0x00000FFF]);
											if ( *pTemp  == 0xFEEEFEEE || *pTemp  == 0xABABABAB )
											{
												// Check for fourth tag
												dwPage = ( ( *pPtr - 24 ) & 0xFFFFF000 ) / 0x1000;
												if ( m_Snapshot[dwPage] )
												{
													pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr - 24 ) & 0x00000FFF]);
													if ( *pTemp  == 0xABABABAB )
													{
														// Yup, it's a heap block
														Addr = i * 0x1000 + k;
											
														dwPage = ( ( *pPtr - 8 ) & 0xFFFFF000 ) / 0x1000;
														if ( m_Snapshot[dwPage] )
														{
															pTemp = (DWORD*)&(m_Snapshot[dwPage][(*pPtr - 8 ) & 0x00000FFF]);

															CDataNode* pNode;
															if ( bCircularLink )
															{
																s_dwCircularRefs++;
																pNode = (CDataNode*)*pTemp;			// <--- spooky stuff
																assert( pNode->dwTag == 0xFFFFFFFF );
																if ( pNode->dwSolutionDist != 0xFFFFFFFF && pNode->dwSolutionDist + 1 < pTodoItem->pNode->dwSolutionDist )
																{
																	// We just connected to a closer solution path - propagate this fact
																	TagSolution( pTodoItem->pNode, pNode->dwSolutionDist + 1 );
																}
															}
															else
															{
																// Mark it as circular and store our own data in no man's land - safe since we have a snapshot
																*pTemp2 = 0xFFFFFFFF;

																pNode = new CDataNode;
																pNode->dwHeapBlock = *pPtr;
																pNode->dwSolutionDist = 0xFFFFFFFF;
																m_Nodes.push_back( pNode );
																*pTemp = (DWORD)pNode;							// Store our data node ptr in no man's land

																CTodoItem* pTodo = AllocTodoItem();
																pTodo->pNode		= pNode;
																pTodo->dwDepth		= pTodoItem->dwDepth + 1;
																m_pLastItem->pNext = pTodo;
																m_pLastItem = pTodo;
															}

															pNode->PointedBy.push_back( pTodoItem->pNode );
															pTodoItem->pNode->PointsTo.push_back( pNode );
															pTodoItem->pNode->PtrOffsets.push_back( Addr - pTodoItem->pNode->dwHeapBlock );
															dwRefCount++;
															s_dwRefCount++;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
			
					// Check for end of current heap block
					if ( *pPtr == 0xABABABAB ) 
					{
						Addr = i * 0x1000 + k;
						dwPage = ( ( Addr + 4 ) & 0xFFFFF000 ) / 0x1000;
						if ( m_Snapshot[dwPage] )
						{
							pTemp = (DWORD*)&(m_Snapshot[dwPage][(Addr + 4 ) & 0x00000FFF]);
							if ( *pTemp  == 0xABABABAB ) 
							{
								pTodoItem->pNode->dwBlockSize = Addr - pTodoItem->pNode->dwHeapBlock;

								// Recycle the todo item
								m_pFirstItem = pTodoItem->pNext;
								pTodoItem->pNext = m_pTodoFreeList;
								m_pTodoFreeList = pTodoItem;
								return true;
							}
						}
					}
				}
				dwIndex = 0;
			}
			else 
			{
				pTodoItem->pNode->dwBlockSize = 0;		// A 'false' block - we should remove it...
			}
		}
	}

	m_pFirstItem = pTodoItem->pNext;
	pTodoItem->pNext = m_pTodoFreeList;
	m_pTodoFreeList = pTodoItem;
	return true;
}
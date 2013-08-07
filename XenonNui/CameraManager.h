//-------------------------------------------------------------------------------------
// CameraManager.h
//  
// Manages the state and data associated with the natural input device.
//  
// Advanced Technology Group (ATG)
// Copyright (c) Microsoft Corporation. All rights reserved.
//-------------------------------------------------------------------------------------

#pragma once

#ifndef _CAMERA_MANAGER_H_
#define _CAMERA_MANAGER_H_

#include <xtl.h>
#include <xnamath.h>
#include <NuiApi.h>
#include <ATGNUIVisualization.h>

//-------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------

class CameraManager
{
public:
    CameraManager();
    virtual ~CameraManager();

    HRESULT InitializeCamera( D3DDevice* pd3dDevice );
    VOID ShutdownCamera();
    VOID DisplayPIP();
    BOOL CheckForNewSkeletonAndDepthMaps( FLOAT fElapsedTime, DWORD dwFilteringFlags);
    VOID ReleaseDepthMaps();

    BOOL FacingCamera() const;

protected:

    NUI_SKELETON_FRAME m_SkeletonData;

    DWORD m_dwCurrentFrameIndex;

    // Visualize the depth and color buffers
	CONST NUI_IMAGE_FRAME*      m_pDepthFrame320x240;
    CONST NUI_IMAGE_FRAME*      m_pDepthFrame80x60;
	ATG::NuiVisualization       m_pip;
	HANDLE                      m_hDepth320x240;
    HANDLE                      m_hDepth80x60;
    HANDLE                      m_hFrameEndEvent;


public:
    inline const NUI_SKELETON_FRAME* GetSkeletonFrame() const { return &m_SkeletonData; }
    
    inline INT GetActiveSkeletonSkeletonDataIndex ()
    {
        for ( INT iIndex = 0; iIndex < NUI_SKELETON_COUNT; ++iIndex )
        {
            if ( m_SkeletonData.SkeletonData[iIndex].eTrackingState == NUI_SKELETON_TRACKED ) 
            {
                return iIndex;
            } 
        }
        return -1;
    }
    inline const DWORD GetTrackedSkeletonTrackingID ()
    {
        for ( INT iIndex = 0; iIndex < NUI_SKELETON_COUNT; ++iIndex ) 
        {
            if ( m_SkeletonData.SkeletonData[iIndex].eTrackingState == NUI_SKELETON_TRACKED )
            {
                return m_SkeletonData.SkeletonData[iIndex].dwTrackingID;
            }
        }    
        return 0;
    }
    inline const NUI_SKELETON_DATA* GetTrackedSkeleton()
    {
        for ( INT iIndex = 0; iIndex < NUI_SKELETON_COUNT; ++iIndex ) 
        {
            if ( m_SkeletonData.SkeletonData[iIndex].eTrackingState == NUI_SKELETON_TRACKED )
            {
                return &m_SkeletonData.SkeletonData[iIndex];
            }
        }
        return NULL;
    };
    inline const NUI_IMAGE_FRAME* GetDepthFrame320x240()
    {
        if ( m_pDepthFrame320x240 == NULL ) return NULL;
        return m_pDepthFrame320x240;
    };

    inline IDirect3DTexture9 *GetDepthMap320x240()
    {
        if ( m_pDepthFrame320x240 == NULL ) return NULL;
        return m_pDepthFrame320x240->pFrameTexture;
    };
    inline const NUI_IMAGE_FRAME* GetDepthFrame80x60()
    {
        if ( m_pDepthFrame80x60 == NULL ) return NULL;
        return m_pDepthFrame80x60;
    };
    inline IDirect3DTexture9 *GetDepthMap80x60()
    {
        if ( m_pDepthFrame80x60 == NULL ) return NULL;
        return m_pDepthFrame80x60->pFrameTexture;
    };

};

#endif
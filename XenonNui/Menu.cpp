//--------------------------------------------------------------------------------------
// Menu.cpp
//
// This sample demonstrates the use of NUI Handles
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include <xtl.h>
#include <xnamath.h>
#include <xffb.h>
#include <xaudio2.h>
#include <AtgApp.h>
#include <AtgFont.h>
#include <AtgInput.h>
#include <AtgMesh.h>
#include <AtgResource.h>
#include <AtgUtil.h>
#include <AtgHelp.h>
#include <AtgSimpleShaders.h>
#include <AtgAudio.h>

#include "CameraManager.h"
#include "nuihandles.h"

static CONST FLOAT g_fTransparent = 0.5f;

static CONST INT g_iCursorSize = 80;

static CONST INT g_iMenuItems = 4;

XMVECTOR g_vListGripLocation = XMVectorSet( -0.3f, 0.4f, 0.0f, 0.0f );
XMVECTOR g_vListBackButtonLocation = XMVectorSet( 0.7f, 0.85f, 0.0f, 0.0f );
XMVECTOR g_vListJoystickLocation = XMVectorSet( 0.0f, -0.5f, 0.0f, 0.0f );
XMVECTOR g_vListStartButtonLocation = XMVectorSet( -0.7f, -0.55f, 0.0f, 0.0f );

NUI_HANDLES_ARMS g_HandlesArms;
NUI_HANDLES_GRIP g_ListGrip;
NUI_HANDLES_GRIP g_BackButtonGrip;
NUI_HANDLES_GRIP g_StartButtonGrip;
NUI_HANDLES_GRIP g_JoystickGrip;

#define NUM_GRIPS 4
static const NUI_HANDLES_GRIP* g_pGripList[NUM_GRIPS] = 
{
    &g_ListGrip,
    &g_BackButtonGrip,
    &g_StartButtonGrip,
    &g_JoystickGrip,
};

static const DWORD START_ARC_RAILID = 1;
static const DWORD BACK_ARC_RAILID = 1;
static const DWORD JOYSTICK_UP_RAILID = 1;
static const DWORD JOYSTICK_LEFT_RAILID = 2;
static const DWORD JOYSTICK_RIGHT_RAILID = 3;
static const DWORD LIST_RAILID = 1;

static const INT g_iStandardButtonMovementInPixels = 100;
static const FLOAT g_fMenuItemMovementInPixels = 100.0f;
static const FLOAT g_fRightArrowMovementInPixels = 8.0f;
static const INT g_iRightArrowOffsetInX = 25;
static const INT g_iTPadCenterOffsetInPixelsInX = -2;
static const INT g_iTPadCenterOffsetInPixelsInY = -30;
static const INT g_iSpaceBetweenRailAndMenu = 40;
static const INT g_iSpaceBetweenMenuItems = 15;
static const INT g_iNmPixelsToCenterOfFirstCircle = 20;    // for vertical rail

char	GameXexPath[] = "game:\\default.xex";

// The background color is what you can modify with handles.
static const INT g_iTotalColors = 20;
D3DCOLOR g_BackGroundColors[ g_iTotalColors ];

INT g_iCurrentPositionInColorList1      = g_iTotalColors / 2;
FLOAT g_fCurrentPositionInColorList1    = 0.505f;
INT g_iCurrentPositionInColorList2      = g_iTotalColors / 2;
FLOAT g_fCurrentPositionInColorList2    = 0.505f;

D3DCOLOR g_GlobalBackgroundColor1   = 0xff3f6385;
D3DCOLOR g_GlobalBackgroundColor2   = 0xffcebdad;
D3DCOLOR g_GripColor                = 0xFFFFFFFF;
D3DCOLOR g_CursorColor              = 0xFFFFFFFF;
D3DCOLOR g_CursorOnRailColor[2]     = { 0xffFA883F, 0xff91C942 };

// Each of these game states has some associated handles.
enum GameState
{
    GAME_STATE_GAME,
    GAME_STATE_MENU,
    GAME_STATE_CONFIRM,
    GAME_STATE_BACKGROUND,
    GAME_STATE_EXIT
};

enum Sound
{
    SOUND_ATTACH_HANDLE,
    SOUND_DETACH_HANDLE,
    SOUND_SELECT,
	SOUND_PROXIMITY,
	SOUND_ATTACH_HANDLE_VARIANT2,
	SOUND_DETACH_HANDLE_VARIANT2,
    SOUND_SELECT_VARIANT2,
	SOUND_PROXIMITY_LOOPED,
	SOUND_CURSOR_LOOPED,
    SOUND_COUNT
};

const CHAR* s_SoundFiles[ SOUND_COUNT ] = 
{
    "game:\\Media\\Sounds\\11_Btn_Interactive_Select.wav",
    "game:\\Media\\Sounds\\19_Btn_Interactive_Focus.wav",
    "game:\\Media\\Sounds\\12_Btn_Select.wav",
	"game:\\Media\\Sounds\\ProximityBlip2.wav",
	"game:\\Media\\Sounds\\07_Cursor_Hotspot_Enter.wav",
	"game:\\Media\\Sounds\\10_Cursor_Hotspot_Exit.wav",
	"game:\\Media\\Sounds\\11_Cursor_Direction_Select.wav",
	"game:\\Media\\Sounds\\09_Cursor_Inside_short_loop.wav",
	"game:\\Media\\Sounds\\04_Cursor_Prox_Closer_longer_loop.wav"
};

static const FLOAT SOUND_NOTIFY_MAX_PERIOD = .5f; // seconds
static const FLOAT SOUND_NOTIFY_MIN_PERIOD = .05f; // seconds
static const FLOAT SOUND_NOTIFY_MAX_DISTANCE = 250.0f;
static const FLOAT SOUND_NOTIFY_MIN_DISTANCE = 125.0f;
static const FLOAT SOUND_NOTIFY_MIN_FREQRATIO = .9f; // frequency to allow us to pitch a sound down
static const FLOAT SOUND_NOTIFY_MAX_FREQRATIO = 1.1f; // frequency to allow us to pitch a sound up




// start with the menu up.
GameState g_GameState = GAME_STATE_MENU;
DWORD g_dwTrackinIdInvokingSystemGesture = 0;

const CHAR* m_strVertexShaderPassThru =
"struct VS_OUTPUT_TEX_COLOR             \n"
"{                                      \n"
"    float4  Pos     : POSITION;        \n"
"    float4  Color   : COLOR;           \n"
"    float2  Tex     : TEXCOORD0;       \n"
"};                                     \n"
"struct VS_INPUT_TEX_COLOR               \n"
"{                                       \n"
"    float4  vPos    : POSITION0;        \n"       
"    float4  vColor  : COLOR0;           \n"
"    float2  vTex    : TEXCOORD0;        \n"
"};                                      \n"
"                                        \n"
"VS_OUTPUT_TEX_COLOR main( VS_INPUT_TEX_COLOR In ) \n"
"{                              \n"
"    VS_OUTPUT_TEX_COLOR Out;   \n"
"    Out.Pos = In.vPos;         \n"
"    Out.Color = In.vColor;     \n"
"    Out.Tex = In.vTex;         \n"
"    return Out;                \n"
"};                             \n";


const CHAR* m_strPixelShader =
" sampler TextureSampler0 : register(s0);                                   \n"
" float4     g_fDrawColor : register(c0);                                   \n"
"                                                                           \n"
"float4 main( float2 InTex : TEXCOORD0 ) : COLOR                            \n"
"{                                                                          \n"
"                                                                           \n"
"    float4 depthsample = tex2D( TextureSampler0, InTex );                  \n"
"    if ( depthsample.a > g_fDrawColor.a ) depthsample.a = g_fDrawColor.a;  \n"
"    return depthsample;                                                    \n"
"};                                                                         \n";

const CHAR* m_strPixelShaderQuad =
" sampler TextureSampler0 : register(s0);                                   \n"
" float4     g_fDrawColor : register(c0);                                   \n"
"                                                                           \n"
"float4 main( float2 InTex : TEXCOORD0 ) : COLOR                            \n"
"{                                                                          \n"
"                                                                           \n"
"    float4 depthsample = tex2D( TextureSampler0, InTex );                  \n"
"    if ( depthsample.a > g_fDrawColor.a ) depthsample.a = g_fDrawColor.a;  \n"
"    return float4( g_fDrawColor.xyz, depthsample.a );                      \n"
"};                                                                         \n";

static const D3DVERTEXELEMENT9 g_PosTexCoordVertexElements[] =
{
    { 0,     0, D3DDECLTYPE_FLOAT3,     0,  D3DDECLUSAGE_POSITION,  0 },
    { 0,    12, D3DDECLTYPE_FLOAT2,     0,  D3DDECLUSAGE_TEXCOORD,  0 },
    D3DDECL_END()
};

//--------------------------------------------------------------------------------------
// Callouts for labeling the gamepad on the help screen
//--------------------------------------------------------------------------------------l
ATG::HELP_CALLOUT g_HelpCallouts[] =
{
    { ATG::HELP_BACK_BUTTON,  ATG::HELP_PLACEMENT_2, L"Display\nhelp" }
};
static const DWORD  NUM_HELP_CALLOUTS = sizeof( g_HelpCallouts ) / sizeof( g_HelpCallouts[0] );

struct WidthHeight
{
    INT Width;
    INT Height;
};


//--------------------------------------------------------------------------------------
// Name: class Sample
// Desc: Main class to run this application. Most functionality is inherited
//       from the ATG::Application base class.
//--------------------------------------------------------------------------------------
class Sample : public ATG::Application
{
    ATG::Timer m_Timer;    // Timer
    ATG::Font m_Font;     // Font for drawing text
    ATG::Help m_Help;
    BOOL m_bDrawHelp;
    ATG::PackedResource m_Resource; // Bundled textures in a packed resource


    BOOL m_bDrawCursor;
    // Transform matrices
    XMMATRIX m_matView;
    XMMATRIX m_matProj;
   
    // Natural Input data
    CameraManager m_CameraManager;

    FLOAT m_fLeftTransparent;
    FLOAT m_fRightTransparent;

    HANDLE m_hNotification;

    // see functions for comments
    HRESULT InitAudio();
    void LoadWavFile( Sound value );
    void DestroyAudio();
    HRESULT PlaySound( Sound value );
    HRESULT StopSound( Sound value );
    VOID UpdateHandles( FLOAT fElapsedTimeSinceLastSTUpdate );
    VOID DrawHandlesAndUpdateHandlesGameState();
    VOID DrawImage( D3DRECT Rect, FLOAT fTransparent, D3DCOLOR color, LPDIRECT3DTEXTURE9 pTexture );
    VOID DrawImageColored( D3DRECT Rect, FLOAT fTransparent, D3DCOLOR color, LPDIRECT3DTEXTURE9 pTexture );
    VOID CalculateStandardButtonData( NUI_HANDLES_GRIP* pGrip, XMFLOAT2 fButtonDimensions,
                            NUI_HANDLES_GRIP **ppAttachedGrip, FLOAT* fAlpha, 
                            D3DRECT *rect, NUI_HANDLES_RAILSTATE* pRailState );
    
    VOID UpdateProximitySound( FLOAT fMinDistanceToHandle );

    XMFLOAT2 GetXYForVector( FXMVECTOR vNormalizedSpace );
    VOID FindClosestGrip( NUI_HANDLES_GRIP** pClosestGrip, FLOAT& fLeftTransparency, FLOAT& fRightTransparency );

    VOID DrawAndUpdateStateGame ();
    VOID DrawAndUpdateStateMenu ();
    VOID DrawAndUpdateStateConfirm ();
    VOID DrawAndUpdateStateBackground ();

public:

    Sample();
    ~Sample();

    virtual HRESULT     Initialize();
    virtual HRESULT     Update();
    virtual HRESULT     Render();

private:
    BOOL m_bStartSelected;
    BOOL m_bBackSelected;
    BOOL m_bHandleSelected;
    BOOL m_bMenuSelected[g_iMenuItems];
    BOOL m_bPlayerEngaged;

	// variables for handle sound implementations
	FLOAT m_fTimeOfLastProximityChirp;


    // sprites for menu system
    LPDIRECT3DTEXTURE9 m_pCursorTexture;
    LPDIRECT3DTEXTURE9 m_pBackTexture[2];
    LPDIRECT3DTEXTURE9 m_pStartTexture[2];
    LPDIRECT3DTEXTURE9 m_pJoystickTexture;
    LPDIRECT3DTEXTURE9 m_pRailTexture;
    LPDIRECT3DTEXTURE9 m_pRailCapTexture;
    LPDIRECT3DTEXTURE9 m_pRightArrowTexture;
    LPDIRECT3DTEXTURE9 m_pMenuTexture[4][2];
    LPDIRECT3DTEXTURE9 m_pBackgroundTexture;
    LPDIRECT3DTEXTURE9 m_pGripTexture[2];

    WidthHeight m_BackButtonDims;
    WidthHeight m_MenuButtonDims;
    WidthHeight m_MenuRailDims;
    WidthHeight m_MenuGripDims;
    WidthHeight m_MenuRailCapDims;
    WidthHeight m_RightArrowDims;
    WidthHeight m_CursorDims;
    WidthHeight m_JoystickDims;

    LPDIRECT3DVERTEXSHADER9      m_pVertexShader;
    LPDIRECT3DPIXELSHADER9       m_pPixelShader;
    LPDIRECT3DPIXELSHADER9       m_pPixelShaderColor;
    LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
    
    IXAudio2*                    m_pXAudio2;
    IXAudio2MasteringVoice*      m_pMasteringVoice;
    IXAudio2SourceVoice*         m_pSourceVoice[SOUND_COUNT];
    XAUDIO2_BUFFER               m_audioBuffers[SOUND_COUNT];
    BOOL                         m_fPlayingSound[SOUND_COUNT];
    BOOL                         m_fAttachedToHandle;
    INT                          m_iClosestSnapPosition;    // The closest menu position to snap to after disengage

};


//--------------------------------------------------------------------------------------
// Name: main()
// Desc: Entry point to the program
//--------------------------------------------------------------------------------------
VOID __cdecl main()
{
    Sample atgApp;
    ATG::GetVideoSettings( &atgApp.m_d3dpp.BackBufferWidth, &atgApp.m_d3dpp.BackBufferHeight );
    atgApp.m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_TWO;

    atgApp.Run();
}

struct MeshVertexPT
{
public:
    XMFLOAT3 Position;
    XMFLOAT2 TexCoord;
    static size_t Size()
    {
        return sizeof( MeshVertexPT );
    }
};

//--------------------------------------------------------------------------------------
// Name: CalculateStandardButtonData()
// Desc: calculates data for visualization of the Grip
//--------------------------------------------------------------------------------------
VOID Sample::CalculateStandardButtonData( NUI_HANDLES_GRIP *pGrip, XMFLOAT2 fButtonDimensions,
                               NUI_HANDLES_GRIP **ppAttachedGrip, FLOAT* fAlpha, 
                               D3DRECT *rect, NUI_HANDLES_RAILSTATE* pRailState )
{

    // convert -1 to 1 screen coordinates to 720p coodinates
    XMFLOAT2 fScreen = GetXYForVector( NuiHandlesGripGetScreenSpaceLocation( pGrip ) );
    XMFLOAT2 fButtonDimHalf;

    // Half the size of the sprite. Used for centering
    fButtonDimHalf.x = fButtonDimensions.x * 0.5f;
    fButtonDimHalf.y = fButtonDimensions.y * 0.5f;

    // textures goes from right to left
    if ( fButtonDimensions.x < 0 ) 
    {
        rect->x1 = (LONG)( fScreen.x + fButtonDimHalf.x );
        rect->x2 = (LONG)( fScreen.x - fButtonDimHalf.x );          
    }
    // texture goes from left to right
    else 
    {
        rect->x1 = (LONG)( fScreen.x - fButtonDimHalf.x );
        rect->x2 = (LONG)( fScreen.x + fButtonDimHalf.x );          
    }

    // center texture
    rect->y1 = (LONG)( fScreen.y - fButtonDimHalf.y );
    rect->y2 = (LONG)( fScreen.y + fButtonDimHalf.y ); 
    *ppAttachedGrip = NULL;

    // test to see if grip is attached.  
    if ( NuiHandlesGripGetIsAttached( pGrip ) )
    {
        *ppAttachedGrip = pGrip;       
    }

    // if we're attached then draw opaque and find the rail state
    if ( *ppAttachedGrip != NULL )
    {
        *fAlpha = 1.0f;

        DWORD dwRailId = NUI_HANDLES_INVALID_RAIL_ID;

        NuiHandlesGripGetCommittedRailId( *ppAttachedGrip, &dwRailId );

        NuiHandlesGripGetRailState( *ppAttachedGrip, dwRailId, pRailState );
    }
    else 
    {
        // if not attached draw transparent
        *fAlpha = g_fTransparent;
    }
}


//--------------------------------------------------------------------------------------
// Name: DrawImageColored()
// Desc: Draw a sprite with transparency and color
//--------------------------------------------------------------------------------------
VOID Sample::DrawImageColored( D3DRECT Rect, FLOAT fTransparent, D3DCOLOR color, LPDIRECT3DTEXTURE9 pTexture )
{
    MeshVertexPT* pVertexData = NULL;
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
    m_pd3dDevice->SetTexture( 0, pTexture );

    D3DXCOLOR d3dxColor ;
    d3dxColor.r = D3DCOLOR_GETRED( color ) / 255.0f;
    d3dxColor.g = D3DCOLOR_GETGREEN( color ) / 255.0f;
    d3dxColor.b = D3DCOLOR_GETBLUE( color ) / 255.0f;
    d3dxColor.a = fTransparent;
    m_pd3dDevice->SetPixelShaderConstantF( 0, d3dxColor, 1 );

    m_pd3dDevice->SetPixelShader( m_pPixelShaderColor );
    m_pd3dDevice->SetVertexShader( m_pVertexShader );
    m_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration );

    m_pd3dDevice->BeginVertices( D3DPT_RECTLIST, 3, sizeof( MeshVertexPT ), ( VOID** )&pVertexData );    
    assert( pVertexData != NULL );
    pVertexData[0].Position = XMFLOAT3( ( FLOAT )Rect.x1, ( FLOAT )Rect.y1, 0 );
    pVertexData[0].TexCoord = XMFLOAT2( 0, 0 );
    pVertexData[1].Position = XMFLOAT3( ( FLOAT )Rect.x2, ( FLOAT )Rect.y1, 0 );
    pVertexData[1].TexCoord = XMFLOAT2( 1, 0 );
    pVertexData[2].Position = XMFLOAT3( ( FLOAT )Rect.x1, ( FLOAT )Rect.y2, 0 );
    pVertexData[2].TexCoord = XMFLOAT2( 0, 1 );
    m_pd3dDevice->EndVertices();

    m_pd3dDevice->SetRenderState( D3DRS_VIEWPORTENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
            
}

//--------------------------------------------------------------------------------------
// Name: DrawImage()
// Desc: Draw a sprite with transparency and color
//--------------------------------------------------------------------------------------
VOID Sample::DrawImage( D3DRECT Rect, FLOAT fTransparent, D3DCOLOR color, LPDIRECT3DTEXTURE9 pTexture  )
{
    MeshVertexPT* pVertexData = NULL;
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

    m_pd3dDevice->SetTexture( 0, pTexture );

    D3DXCOLOR d3dxColor = color;
    d3dxColor.a = fTransparent;
    m_pd3dDevice->SetPixelShaderConstantF( 0, d3dxColor, 1 );

    m_pd3dDevice->SetPixelShader( m_pPixelShader );
    m_pd3dDevice->SetVertexShader( m_pVertexShader );
    m_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration );

    m_pd3dDevice->BeginVertices( D3DPT_RECTLIST, 3, sizeof( MeshVertexPT ), ( VOID** )&pVertexData );    
    assert( pVertexData != NULL );
    pVertexData[0].Position = XMFLOAT3( ( FLOAT )Rect.x1, ( FLOAT )Rect.y1, 0 );
    pVertexData[0].TexCoord = XMFLOAT2( 0, 0 );
    pVertexData[1].Position = XMFLOAT3( ( FLOAT )Rect.x2, ( FLOAT )Rect.y1, 0 );
    pVertexData[1].TexCoord = XMFLOAT2( 1, 0 );
    pVertexData[2].Position = XMFLOAT3( ( FLOAT )Rect.x1, ( FLOAT )Rect.y2, 0 );
    pVertexData[2].TexCoord = XMFLOAT2( 0, 1 );
    m_pd3dDevice->EndVertices();

    m_pd3dDevice->SetRenderState( D3DRS_VIEWPORTENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
            
}

//--------------------------------------------------------------------------------------
// Name: Initialize()
// Desc: Initialize app-dependent objects
//--------------------------------------------------------------------------------------
HRESULT Sample::Initialize()
{
    
    const FLOAT fNavRailLength = 40.0f;
    const FLOAT fNavRailDeadzone = 12.0f;
    const FLOAT fSelRailLength = 35.0f;
    const FLOAT fSelRailDeadzone = 10.0f;
    const FLOAT fToleranceAngle = XMConvertToRadians( 16.0f );

    m_bDrawHelp = FALSE;
    m_bPlayerEngaged = FALSE;
    m_bStartSelected = FALSE;
    m_bBackSelected = FALSE;
    m_bHandleSelected = FALSE;
    m_bDrawCursor = TRUE;
    m_fLeftTransparent = 0.0f;
    m_fRightTransparent = 0.0f;
    m_iClosestSnapPosition = -1;

    for ( INT i = 0; i < 3; i++ )
    {
        m_bMenuSelected[i] = FALSE;
    }

    HRESULT hr;

	m_fTimeOfLastProximityChirp = 0.f;

    if ( FAILED( hr = InitAudio() ) )
    {
        ATG_PrintError( "Couldn't initialize audio\n" );
        return hr;
    }

    // Initialize simple shaders and set the renderstates.
    ATG::SimpleShaders::Initialize( NULL, NULL );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    // Create the font
    if( FAILED( hr = m_Font.Create( "game:\\Media\\Fonts\\Arial_16.xpr" ) ) )
    {
        ATG_PrintError( "Couldn't create font\n" );
        return hr;
    }

    // Create the help
    if( FAILED( hr = m_Help.Create( "game:\\Media\\Help\\Help.xpr" ) ) )
    {
        ATG_PrintError( "Couldn't create help\n" );
        return hr;
    }

    // Confine text drawing to the title safe area
    m_Font.SetWindow( ATG::GetTitleSafeArea() );

    // Create the textures resource
    if( FAILED( hr = m_Resource.Create( "game:\\Media\\Resource.xpr" ) ) )
    {
        ATG_PrintError( "Couldn't create Resource.xpr\n" );
        return hr;
    }

    // Load the sprites for the menu
    m_pCursorTexture = m_Resource.GetTexture("Cursor");
    m_pBackTexture[0] = m_Resource.GetTexture("MenuBackNeutral");
    m_pBackTexture[1] = m_Resource.GetTexture("MenuBackSelected");  
    m_pStartTexture[0] = m_Resource.GetTexture("MenuStartNeutral");
    m_pStartTexture[1] = m_Resource.GetTexture("MenuStartSelected");
    m_pJoystickTexture = m_Resource.GetTexture("MenuTPad");
    m_pRailTexture = m_Resource.GetTexture("MenuUpDown");
    m_pRailCapTexture = m_Resource.GetTexture("MenuRailCap");
    m_pRightArrowTexture = m_Resource.GetTexture("MenuRightOn");
    m_pBackgroundTexture = m_Resource.GetTexture("BackGround");
    m_pGripTexture[0] = m_Resource.GetTexture("HandleNeutral");
    m_pGripTexture[1] = m_Resource.GetTexture("HandleSelected");

    m_pMenuTexture[0][0] = m_Resource.GetTexture("Menu1Neutral");
    m_pMenuTexture[1][0] = m_Resource.GetTexture("Menu2Neutral");
    m_pMenuTexture[2][0] = m_Resource.GetTexture("Menu3Neutral");
    m_pMenuTexture[3][0] = m_Resource.GetTexture("Menu4Neutral");

    m_pMenuTexture[0][1] = m_Resource.GetTexture("Menu1Selected");
    m_pMenuTexture[1][1] = m_Resource.GetTexture("Menu2Selected");
    m_pMenuTexture[2][1] = m_Resource.GetTexture("Menu3Selected");
    m_pMenuTexture[3][1] = m_Resource.GetTexture("Menu4Selected");


    D3DSURFACE_DESC SurfaceDESC;
    m_pBackTexture[0]->GetLevelDesc( 0, &SurfaceDESC );
    // Width is negative because button goes from right to left.
    m_BackButtonDims.Width = - ( (INT)SurfaceDESC.Width );
    m_BackButtonDims.Height = SurfaceDESC.Height;
    
    m_pRailTexture->GetLevelDesc( 0, &SurfaceDESC ); 
    m_MenuRailDims.Width = SurfaceDESC.Width;
    m_MenuRailDims.Height= SurfaceDESC.Height;

    m_pGripTexture[0]->GetLevelDesc( 0, &SurfaceDESC ); 
    m_MenuGripDims.Width = SurfaceDESC.Width;
    m_MenuGripDims.Height= SurfaceDESC.Height;

    m_pRailCapTexture->GetLevelDesc( 0, &SurfaceDESC ); 
    m_MenuRailCapDims.Width = SurfaceDESC.Width;
    m_MenuRailCapDims.Height= SurfaceDESC.Height;

    m_pRightArrowTexture->GetLevelDesc( 0, &SurfaceDESC ); 
    m_RightArrowDims.Width = SurfaceDESC.Width;
    m_RightArrowDims.Height= SurfaceDESC.Height;

    m_pMenuTexture[0][0]->GetLevelDesc( 0, &SurfaceDESC ); 
    m_MenuButtonDims.Width = SurfaceDESC.Width;
    m_MenuButtonDims.Height= SurfaceDESC.Height;

    m_pJoystickTexture->GetLevelDesc( 0, &SurfaceDESC ); 
    m_JoystickDims.Width = SurfaceDESC.Width;
    m_JoystickDims.Height= SurfaceDESC.Height;
    

    m_CursorDims.Width = g_iCursorSize;
    m_CursorDims.Height = g_iCursorSize;

    // Initialize the natural input device
    if( FAILED( hr = m_CameraManager.InitializeCamera( m_pd3dDevice ) ) )
    {
        ATG_PrintError( "Couldn't create the natural input device.\n" );
        return hr;
    }

    // Set the transform matrices
    FLOAT fAspectRatio = ( FLOAT )m_d3dpp.BackBufferWidth / ( FLOAT )m_d3dpp.BackBufferHeight;
    m_matProj = XMMatrixPerspectiveFovLH( XM_PI / 3, fAspectRatio, .10f, 10000.0f );
    
    NuiHandlesArmsInit( &g_HandlesArms );

    // Use the standard handle creation functions
    hr = NuiHandlesGripInit( &g_ListGrip );
    if ( FAILED( hr ) ) return E_FAIL;

    hr = NuiHandlesGripAddRailGearShift( 
            &g_ListGrip,                            // grip to add rail to
            LIST_RAILID,                            // pre-assigned rail ID
            XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f),   // direction of movement on primary track
            XMConvertToRadians( 35.0f ),            // angle one end of track to other
            XMConvertToRadians( 0.0f ),             // slot entrance angle
            XMConvertToRadians( 20.0f ),            // slot engage angle
            XMConvertToRadians( 5.0f ), 
            XMConvertToRadians( 16.0f ),
            g_iMenuItems,                           // number of slots
            0.0f,                                   // Scroll Zone Top
            0.0f );                                 // Scroll Zone Bottom
    if ( FAILED( hr ) ) return E_FAIL;


    hr = NuiHandlesGripInit( &g_JoystickGrip );
    if ( FAILED( hr ) ) return E_FAIL;


    // add the rail representing the right half (scroll right) of the grip
    hr = NuiHandlesGripAddRailArc(
                &g_JoystickGrip,                           
                JOYSTICK_RIGHT_RAILID,                     
                XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ),     
                XMConvertToRadians( fNavRailLength ),      
                XMConvertToRadians( fNavRailDeadzone ),
                fToleranceAngle);  
    if ( FAILED( hr ) )  return E_FAIL;

    // add the rail representing the left half (scroll left) of the grip
    hr = NuiHandlesGripAddRailArc(
                &g_JoystickGrip,                            
                JOYSTICK_LEFT_RAILID,                       
                XMVectorSet( -1.0f, 0.0f, 0.0f, 0.0f ),     
                XMConvertToRadians( fNavRailLength ),       
                XMConvertToRadians( fNavRailDeadzone ),
                fToleranceAngle);   
    if ( FAILED( hr ) )  return E_FAIL;

    // add the rail representing the vertical portion (selection) of the
    // grip
    hr = NuiHandlesGripAddRailArc(
                &g_JoystickGrip,                           
                JOYSTICK_UP_RAILID,                        
                XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ),     
                XMConvertToRadians( fSelRailLength ),      
                XMConvertToRadians( fSelRailDeadzone ),
                fToleranceAngle);   
    if ( FAILED( hr ) )  return E_FAIL;
    

    hr = NuiHandlesGripInit( &g_BackButtonGrip );
    if ( FAILED( hr ) )  return E_FAIL;
    
    hr = NuiHandlesGripAddRailArc( 
            &g_BackButtonGrip,                              // grip to add rail to
            BACK_ARC_RAILID,                                // pre-assigned rail ID
            XMVectorSet( -1.0f, 0.0f, 0.0f, 0.0f ),         // direction of movement
            XMConvertToRadians( 20.0f ),                    // length of the rail
            XMConvertToRadians( 4.0f ),                     // deadzone high angle limit
            fToleranceAngle);                   
    if ( FAILED( hr ) )  return E_FAIL;


    hr = NuiHandlesGripInit( &g_StartButtonGrip );
    if ( FAILED( hr ) )  return E_FAIL;

    NuiHandlesGripAddRailArc( &g_StartButtonGrip,
                                    START_ARC_RAILID, 
                                    XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f ),
                                    XMConvertToRadians( fSelRailLength ),
                                    XMConvertToRadians( fSelRailDeadzone ), 
                                    fToleranceAngle);
    
    if ( FAILED( hr ) )  return E_FAIL;



    hr = NuiHandlesGripSetScreenSpaceLocation( &g_BackButtonGrip,  g_vListBackButtonLocation ); 
    if ( FAILED( hr ) )  return E_FAIL;

    hr = NuiHandlesGripSetScreenSpaceLocation( &g_JoystickGrip,  g_vListJoystickLocation ); 
    if ( FAILED( hr ) )  return E_FAIL;

    hr = NuiHandlesGripSetScreenSpaceLocation( &g_StartButtonGrip, g_vListStartButtonLocation );
    if ( FAILED( hr ) )  return E_FAIL;

    hr = NuiHandlesGripSetScreenSpaceLocation( &g_ListGrip, g_vListGripLocation );
    if ( FAILED( hr ) )  return E_FAIL;

    // Only enable the grips for each particulare scene
    for ( UINT i = 0; i < NUM_GRIPS; i++ )
    {
        NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
    }
    NuiHandlesGripSetEnabled( &g_ListGrip, TRUE );
    

    if ( FAILED( hr ) ) 
    {
        return E_FAIL;
    }
    
    ID3DXBuffer* pShaderCode = NULL;
    ID3DXBuffer* pErrorMsg = NULL;

    // Create the vertex shader
    D3DXCompileShader( m_strVertexShaderPassThru, 
        ( UINT )strlen( m_strVertexShaderPassThru ),  NULL, NULL, "main", "vs.3.0", 0, 
        &pShaderCode, &pErrorMsg, NULL );
    m_pd3dDevice->CreateVertexShader( ( DWORD* )pShaderCode->GetBufferPointer(),
        &m_pVertexShader );
    pShaderCode->Release();

    // Create the pixel shader
    D3DXCompileShader( m_strPixelShader, 
        ( UINT )strlen( m_strPixelShader ), NULL, NULL, "main", "ps.3.0", 0,
        &pShaderCode, &pErrorMsg, NULL );        
    m_pd3dDevice->CreatePixelShader( ( DWORD* )pShaderCode->GetBufferPointer(),
        &m_pPixelShader );
    pShaderCode->Release();

    D3DXCompileShader( m_strPixelShaderQuad, 
        ( UINT )strlen( m_strPixelShader ), NULL, NULL, "main", "ps.3.0", 0,
        &pShaderCode, &pErrorMsg, NULL );        
    m_pd3dDevice->CreatePixelShader( ( DWORD* )pShaderCode->GetBufferPointer(),
        &m_pPixelShaderColor );
    pShaderCode->Release();

    m_pd3dDevice->CreateVertexDeclaration( g_PosTexCoordVertexElements, &m_pVertexDeclaration );

    // create some random colors for the backgrounds
    for ( int iIndex = 0; iIndex < g_iTotalColors; ++iIndex )
    {
        g_BackGroundColors[iIndex] = rand()%255;
        g_BackGroundColors[iIndex] <<= 8;
        g_BackGroundColors[iIndex] += rand()%255;
        g_BackGroundColors[iIndex] <<= 8;
        g_BackGroundColors[iIndex] += rand()%255;
        g_BackGroundColors[iIndex] <<= 8;
        g_BackGroundColors[iIndex] += rand()%255;
    }

    m_hNotification = XNotifyCreateListener( XNOTIFY_SYSTEM );
    if( m_hNotification == NULL || m_hNotification == INVALID_HANDLE_VALUE )
    { 
        ATG_PrintError( "Unable to create XNotify listener" );
    }

	PlaySound ( SOUND_CURSOR_LOOPED );

    for ( UINT i = 0; i < NUM_GRIPS; i++ )
    {
        NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Name: Update()
// Desc: Called once per frame, the call is the entry point for animating the scene.
//--------------------------------------------------------------------------------------
HRESULT Sample::Update()
{
    static FLOAT fElapsedTimeSinceLastSTUpdate = 0;

    
    DWORD dwNotificationID;
    ULONG_PTR ulParam;

    if( XNotifyGetNext( m_hNotification, 0, &dwNotificationID, &ulParam ) )
    {
        switch( dwNotificationID )
        {
            case XN_SYS_NUIPAUSE:
                g_GameState = GAME_STATE_MENU;
                g_dwTrackinIdInvokingSystemGesture = (DWORD)ulParam;
            break;

            //case XN_SYS_UI:
            //    m_bSystemUIShowing = ( BOOL )ulParam;
            //    break;
        }
    }

    // Get the elapsed time since previous rendered frame
    FLOAT fElapsedTime = ( FLOAT )m_Timer.GetElapsedTime();
    fElapsedTimeSinceLastSTUpdate += fElapsedTime;

    // Get the current gamepad state
    ATG::GAMEPAD* pGamepad = ATG::Input::GetMergedInput();

    // Toggle help
    if( pGamepad->wPressedButtons & XINPUT_GAMEPAD_BACK )
        m_bDrawHelp = !m_bDrawHelp;

    // Update the natural input device
    BOOL bNewSkeletonAndMaps = m_CameraManager.CheckForNewSkeletonAndDepthMaps( fElapsedTime, 0 );
    
    if ( bNewSkeletonAndMaps )
    {
        // If the player is facing the camera, the player is engadged with the menu
        m_bPlayerEngaged = m_CameraManager.FacingCamera();

        // NOTE: This API require the time delta since the last updated
        // skeleton data, which could be different from time delta
        // between game frames            
        UpdateHandles( fElapsedTimeSinceLastSTUpdate );
        fElapsedTimeSinceLastSTUpdate = 0.0f;
        m_CameraManager.ReleaseDepthMaps();
    }   

    // Check if we're exiting the sample
    if ( g_GameState == GAME_STATE_EXIT )
    {
        // Let the select sound complete first
        Sleep( 200 );
        XLaunchNewImage( "", 0 );
    }
    


    // Update the view from the person's position and facing direction.
    return S_OK;
}

VOID Sample::UpdateHandles( FLOAT fElapsedTimeSinceLastSTUpdate )
{
    // If player is not engaged, don't do any updates and make sure we're not attached to handles anymore
    if ( !m_bPlayerEngaged )
    {
        NuiHandlesGripForceDetach( &g_ListGrip );
        NuiHandlesGripForceDetach( &g_BackButtonGrip );
        NuiHandlesGripForceDetach( &g_StartButtonGrip );
        NuiHandlesGripForceDetach( &g_JoystickGrip );
        return;
    }

    LONG iElapsedTime = ( LONG ) ( fElapsedTimeSinceLastSTUpdate * 1000.0f );
    
    // Each frame, update the arms based on the joints
    // NOTE: This API requires raw/unfiltered joint positions.
    NuiHandlesArmsUpdate( &g_HandlesArms, m_CameraManager.GetActiveSkeletonSkeletonDataIndex(),
        m_CameraManager.GetSkeletonFrame(), m_CameraManager.GetDepthFrame320x240(), NULL,
        m_CameraManager.GetDepthFrame80x60(), NULL );

    //Next, we update the grips that we are using in this frame
    switch ( g_GameState ) 
    {
        case GAME_STATE_GAME:
        {
            NuiHandlesGripSetEnabled( &g_BackButtonGrip, TRUE );
            NuiHandlesGripUpdate( &g_BackButtonGrip, &g_HandlesArms );
            break;
        }
        case GAME_STATE_MENU:
        {
            NuiHandlesGripSetEnabled( &g_ListGrip, TRUE );
            NuiHandlesGripUpdate( &g_ListGrip, &g_HandlesArms );
            break;
        }
        case GAME_STATE_CONFIRM:
        {
            NuiHandlesGripSetEnabled( &g_BackButtonGrip, TRUE );
            NuiHandlesGripSetEnabled( &g_StartButtonGrip, TRUE );
            NuiHandlesGripUpdate( &g_BackButtonGrip, &g_HandlesArms );
            NuiHandlesGripUpdate( &g_StartButtonGrip, &g_HandlesArms );
            break;
        }

        case GAME_STATE_BACKGROUND:
        {
            NuiHandlesGripSetEnabled( &g_BackButtonGrip, TRUE );
            NuiHandlesGripSetEnabled( &g_JoystickGrip, TRUE );
            NuiHandlesGripUpdate( &g_BackButtonGrip, &g_HandlesArms );
            NuiHandlesGripUpdate( &g_JoystickGrip, &g_HandlesArms );
            break;
        }
    }

    // The update rail progress allows the handles API to determine the attached handle and update it's state
    NuiHandlesArmsUpdateRailProgress( &g_HandlesArms, iElapsedTime ); 

}

//--------------------------------------------------------------------------------------
// Name: GetXYForVector()
// Desc: Map normalized space to screen space.
//       
//--------------------------------------------------------------------------------------
XMFLOAT2 Sample::GetXYForVector( FXMVECTOR vNormalizedSpace )
{
    XMFLOAT2 rt;
    rt.x = XMVectorGetX( vNormalizedSpace ) * 640.0f + 640.0f;           
    rt.y = 720 - ( XMVectorGetY( vNormalizedSpace ) * 360.0f + 360.0f);
    return rt;
}

//--------------------------------------------------------------------------------------
// Name: FindClosestGrip()
// Desc: Finds the closest grip to the cursor and returns a transparency value
//--------------------------------------------------------------------------------------
VOID Sample::FindClosestGrip( NUI_HANDLES_GRIP** pClosestGrip, FLOAT& fLeftTransparency, FLOAT& fRightTransparency )
{
    FLOAT fMinDistanceLeft = 1000.0f;
    FLOAT fMinDistanceRight = 1000.0f;
    FLOAT fMinDistance = 1000.0f;
    *pClosestGrip = (NUI_HANDLES_GRIP*)g_pGripList[ 0 ];

    // Get the current cursor position
    XMFLOAT2 fLeftHandCursor = GetXYForVector( NuiHandlesArmGetScreenSpaceLocation( &g_HandlesArms, NUI_HANDLES_ARMS_HANDEDNESS_LEFT_ARM ) );
    XMFLOAT2 fRightHandCursor = GetXYForVector( NuiHandlesArmGetScreenSpaceLocation( &g_HandlesArms, NUI_HANDLES_ARMS_HANDEDNESS_RIGHT_ARM ) );

    for ( UINT i = 0; i < NUM_GRIPS; i++ )
    {
        NUI_HANDLES_GRIP* pGrip = (NUI_HANDLES_GRIP*)g_pGripList[ i ];

        if ( !NuiHandlesGripGetEnabled( pGrip ) )
        {
            continue;
        }

        XMFLOAT2 fGripPos = GetXYForVector( NuiHandlesGripGetScreenSpaceLocation( pGrip ) );

        // Calculate the distance from the current cursors to the grip
        // If getting close to the list, show that with another item so that the
        // user knows the list will get selected if he continues moving in the current
        // direction
        FLOAT dx = fLeftHandCursor.x - fGripPos.x;
        FLOAT dy = fLeftHandCursor.y - fGripPos.y;
        FLOAT fLeftDist = sqrtf( dx * dx + dy * dy );

        dx = fRightHandCursor.x - fGripPos.x;
        dy = fRightHandCursor.y - fGripPos.y;
        FLOAT fRightDist = sqrtf( dx * dx + dy * dy );

        if ( fLeftDist < fMinDistanceLeft )
        {
            fMinDistanceLeft = fLeftDist;
        }

        if ( fRightDist < fMinDistanceRight )
        {
            fMinDistanceRight = fRightDist;
        }

        FLOAT fDist = min( fLeftDist, fRightDist );
        if ( fDist < fMinDistance )
        {
            fMinDistance = fDist;
            *pClosestGrip = pGrip;
        }
    }

    
    fLeftTransparency = 0.0f;
    fRightTransparency = 0.0f;

    const FLOAT fDistMaxThreshold = 200.0f;
    const FLOAT fDistMinThreshold = 50.0f;

    if ( fMinDistanceLeft < fDistMaxThreshold )
    {
        fLeftTransparency = 1.0f - ( ( fMinDistanceLeft - fDistMinThreshold ) / ( fDistMaxThreshold - fDistMinThreshold ) );
    }

    if ( fMinDistanceRight < fDistMaxThreshold )
    {
        fRightTransparency = 1.0f - ( ( fMinDistanceRight - fDistMinThreshold ) / ( fDistMaxThreshold - fDistMinThreshold ) );
    }
	UpdateProximitySound ( fMinDistance );

}

//--------------------------------------------------------------------------------------
// Name: DrawAndUpdateStateGame()
// Desc: Draw game play mode
//--------------------------------------------------------------------------------------
VOID Sample::DrawAndUpdateStateGame()
{
    PIXBeginNamedEvent( 0, "Game" );
   
    UINT uWidth, uHeight;
    ATG::GetVideoSettings( &uWidth, &uHeight );

    FLOAT fProgress = 0.0f;

    NUI_HANDLES_GRIP* pClosestGrip = NULL;
    FindClosestGrip( &pClosestGrip, m_fLeftTransparent, m_fRightTransparent );

    m_Font.Begin();
        m_Font.SetScaleFactors( 2.0f, 2.0f );
        m_Font.DrawText( 0.0f, 150.0f, 0xffffffff, L"Launching game..." );
    m_Font.End();
	XLaunchNewImage((CHAR*)&GameXexPath, NULL);

    NUI_HANDLES_GRIP* pAttachedGrip;
    D3DRECT rect;
    FLOAT fAlpha;
    NUI_HANDLES_RAILSTATE RailState;

    // This function determines if the rail is attached, and calculates where the handle should be rendered.
    CalculateStandardButtonData( &g_BackButtonGrip, 
        XMFLOAT2( (FLOAT)m_BackButtonDims.Width, (FLOAT)m_BackButtonDims.Height ), 
        &pAttachedGrip, &fAlpha, &rect, &RailState );

    fAlpha = max( g_fTransparent, max( m_fLeftTransparent, m_fRightTransparent ) );
    if ( pAttachedGrip != NULL ) 
    {
        if ( !m_bBackSelected )
        {
            PlaySound( SOUND_ATTACH_HANDLE );
        }
        m_bBackSelected = TRUE;
        m_bDrawCursor = FALSE;
        fProgress = RailState.Arc.fProgress;
        LONG iProgress = (LONG)(fProgress * (FLOAT)g_iStandardButtonMovementInPixels );
        D3DRECT TransparentRect = rect;

        TransparentRect.x1 -= g_iStandardButtonMovementInPixels;
        TransparentRect.x2 -= g_iStandardButtonMovementInPixels;
        DrawImage( TransparentRect, g_fTransparent, 0xFF00FFFF, m_pBackTexture[ 0 ] );
        rect.x1 -=iProgress;
        rect.x2 -=iProgress;
    }
    else
    {
        if ( m_bBackSelected )
        {
            PlaySound( SOUND_DETACH_HANDLE );
        }

        m_bBackSelected = FALSE;
    }

    // render with transparency calculated above
    if ( pClosestGrip == &g_BackButtonGrip )
    {
        fAlpha = pAttachedGrip ? 1.0f : fAlpha;
    }
    else
    {
        fAlpha = g_fTransparent;
    }
    DrawImage( rect, pAttachedGrip ? 1.0f : fAlpha, 0xFF00FFFF, m_pBackTexture[ 1 ] ); 

    if ( pAttachedGrip != NULL && fProgress >=1.0f )
    {                
        if ( m_bBackSelected )
        {
            PlaySound( SOUND_SELECT );
        }
        // If the end of the rail is reached switch states.
        m_bBackSelected = FALSE;
        g_GameState = GAME_STATE_MENU; 
        PlaySound ( SOUND_CURSOR_LOOPED );

        NuiHandlesGripForceDetach( &g_BackButtonGrip );

        // Only enable the grips for each particulare scene
        for ( UINT i = 0; i < NUM_GRIPS; i++ )
        {
            NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
        }

    }
    PIXEndNamedEvent();

}


//--------------------------------------------------------------------------------------
// Name: DrawAndUpdateStateMenu()
// Desc: Draw main menu
//--------------------------------------------------------------------------------------
VOID Sample::DrawAndUpdateStateMenu ()
{
    PIXBeginNamedEvent( 0, "Menu" );
    
    UINT uWidth, uHeight;
    ATG::GetVideoSettings( &uWidth, &uHeight );

    FLOAT fAlpha = g_fTransparent;
    NUI_HANDLES_RAILSTATE RailState;
    memset( &RailState, 0, sizeof( NUI_HANDLES_RAILSTATE ) );

    NUI_HANDLES_GRIP* pClosestGrip = NULL;
    FindClosestGrip( &pClosestGrip, m_fLeftTransparent, m_fRightTransparent );

    // Render the background image
    D3DRECT BackgroundTextureRect = { 0, 0, uWidth, uHeight };
    DrawImage( BackgroundTextureRect, 1.0f, 0xffffffff, m_pBackgroundTexture );

    XMFLOAT2 fMenuPosition = GetXYForVector( g_vListGripLocation );
    XMFLOAT2 fGripPosition = GetXYForVector( NuiHandlesGripGetScreenSpaceLocation( &g_ListGrip ) );

    D3DRECT RailRect;
    RailRect.x1 = (LONG)fMenuPosition.x - m_MenuRailDims.Width / 2;
    RailRect.x2 = (LONG)fMenuPosition.x + m_MenuRailDims.Width / 2;
    RailRect.y1 = (LONG)fMenuPosition.y ; 
    RailRect.y2 = (LONG)fMenuPosition.y + m_MenuRailDims.Height ; 
    // center rail around the grip
    RailRect.y1 -= m_MenuGripDims.Height / 2;
    RailRect.y2 -= m_MenuGripDims.Height / 2;

    D3DRECT GripRect;
    GripRect.x1 = (LONG)fMenuPosition.x - m_MenuGripDims.Width / 2;
    GripRect.x2 = (LONG)fMenuPosition.x + m_MenuGripDims.Width / 2;
    GripRect.y1 = (LONG)fGripPosition.y - m_MenuGripDims.Height / 2; 
    GripRect.y2 = (LONG)fGripPosition.y + m_MenuGripDims.Height / 2; 

    // If never set, set to top of menu
    if ( m_iClosestSnapPosition < 0 )
    {
        m_iClosestSnapPosition = GripRect.y1;
    }

    LONG iOffset = 0;
    NUI_HANDLES_GRIP* pAttachedGrip = NULL;

    if ( NuiHandlesGripGetIsAttached( &g_ListGrip ) )
    {
        pAttachedGrip = &g_ListGrip;       
    }
    DWORD dwRailId = NUI_HANDLES_INVALID_RAIL_ID;

    // if we're attached then draw opaque and find the rail state
    if ( pAttachedGrip != NULL )
    {
        fAlpha = 1.0f;

        NuiHandlesGripGetCommittedRailId( pAttachedGrip, &dwRailId );
        NuiHandlesGripGetRailState( pAttachedGrip, dwRailId, &RailState );

    }
    LONG iTrackProgress = 0;
    LONG iSnapTrackProgress = 0;
    
    if ( pAttachedGrip != NULL ) 
    {
        if ( !m_bHandleSelected )
        {
            PlaySound( SOUND_ATTACH_HANDLE_VARIANT2 );
			StopSound( SOUND_CURSOR_LOOPED );
        }

        // move the menu based on an offset from the initial menu location
        GripRect.y1 = (LONG)fMenuPosition.y - m_MenuGripDims.Height / 2; 
        GripRect.y2 = (LONG)fMenuPosition.y + m_MenuGripDims.Height / 2; 

        m_bHandleSelected = TRUE;

        // Render the vertical rail
        DrawImage(RailRect, 1.0f, 0xffffffff, m_pRailTexture );

        iOffset = (LONG)(RailState.Gearshift.fToothProgress * g_fMenuItemMovementInPixels );

        m_bDrawCursor = FALSE;

        // We need to take into account the number of pixels from the top of the image to the center
        // of the cicle which is in the center of the menu item, on the top and abottom.
        FLOAT fHeight = (FLOAT)( m_MenuRailDims.Height - g_iNmPixelsToCenterOfFirstCircle * 2.0f );
        iTrackProgress = (LONG)( fHeight * RailState.Gearshift.fSpineProgressAtToothCommit );

        FLOAT fNewPositionY = XMVectorGetY( g_vListGripLocation ) - 
            m_MenuRailDims.Height / 360.0f * (  RailState.Gearshift.fSpineProgressAtToothCommit );
        
        NuiHandlesGripSetScreenSpaceLocation( &g_ListGrip, 
            XMVectorSet( XMVectorGetX( g_vListGripLocation ), fNewPositionY, 0.0f, 0.0f ) );

        NuiHandlesGripRailSetPersistentProgress( &g_ListGrip, dwRailId, RailState.Gearshift.fSpineProgressAtToothCommit );

        GripRect.y1 += iTrackProgress; 
        GripRect.y2 += iTrackProgress; 

        FLOAT fGear = ( (RailState.Gearshift.nIndex) / (FLOAT)( g_iMenuItems - 1.0f ) );
        iSnapTrackProgress = (LONG)( fHeight * fGear );


        // render the grip along the rail
        DrawImage( GripRect, 1.0f, g_GripColor, m_pGripTexture[ 0 ] );
        DrawImage( GripRect, 0.75f, g_CursorColor, m_pCursorTexture);
        
        // If the rail has reached the end of the slot change state and detach
        if ( RailState.Gearshift.fToothProgress >= 1.0f )
        {
            PlaySound( SOUND_SELECT_VARIANT2 );

            switch ( RailState.Gearshift.nIndex )
            {
                case 0 :
                {
                    g_GameState = GAME_STATE_CONFIRM;
                    break;
                }
                case 1 :
                {
                    g_GameState = GAME_STATE_BACKGROUND;
                    break;
                }
                case 2 :
                {
                    g_GameState = GAME_STATE_EXIT;
                    break;
                }
                case 3 : 
                {
                    XShowNuiGuideUI( g_dwTrackinIdInvokingSystemGesture );
                    break;
                }
            }

            // The handles API will not work correctly if we don't detach handles after selection
            NuiHandlesGripForceDetach( &g_ListGrip );

            // Only enable the grips for each particulare scene
            for ( UINT i = 0; i < NUM_GRIPS; i++ )
            {
                NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
            }

        }
        
    }
    else
    {
        if ( m_bHandleSelected )
        {
            PlaySound( SOUND_DETACH_HANDLE_VARIANT2 );
			PlaySound( SOUND_CURSOR_LOOPED );
        }

        m_bHandleSelected = FALSE;


        // Render the vertical rail
        FLOAT fTransparent = max( g_fTransparent, max( m_fLeftTransparent, m_fRightTransparent ) );
        DrawImage( RailRect, fTransparent, 0xffffffff, m_pRailTexture );

        // Snap the grip to the closest menu position where it got disengaged
        GripRect.y1 = m_iClosestSnapPosition;
        GripRect.y2 = GripRect.y1 + m_MenuGripDims.Height;

        // render the grip along the rail
        DrawImage( GripRect, fTransparent, g_GripColor, m_pGripTexture[ 0 ] );
    }

    static FLOAT fSinWave = 0.0f;
    fSinWave += 0.2f;

    D3DRECT MenuRect;
    MenuRect.x1 = (LONG)fMenuPosition.x ;
    MenuRect.x2 = (LONG)fMenuPosition.x + m_MenuButtonDims.Width;
    MenuRect.x1 += g_iSpaceBetweenRailAndMenu;
    MenuRect.x2 += g_iSpaceBetweenRailAndMenu;

    MenuRect.y1 = (LONG)fMenuPosition.y - m_MenuButtonDims.Height / 2;
    MenuRect.y2 = (LONG)fMenuPosition.y + m_MenuButtonDims.Height / 2;

    for ( UINT uIndex = 0; uIndex < g_iMenuItems; ++uIndex )
    {
        INT iSelected = 0;
        if ( m_bMenuSelected[uIndex] ) iSelected = 1;

        if ( pAttachedGrip != NULL && (UINT)RailState.Gearshift.nIndex == uIndex )
        {
            m_bMenuSelected[ uIndex ] = TRUE;

            D3DRECT RightArrowRect;
            RightArrowRect.x1 = (LONG)fMenuPosition.x - m_RightArrowDims.Width / 2;
            RightArrowRect.x2 = (LONG)fMenuPosition.x + m_RightArrowDims.Width / 2;
            RightArrowRect.y1 = (LONG)fMenuPosition.y - m_RightArrowDims.Height / 2; 
            RightArrowRect.y2 = (LONG)fMenuPosition.y + m_RightArrowDims.Height / 2; 
                       
            INT iSinOffset = (LONG)(sinf( fSinWave ) * g_fRightArrowMovementInPixels );
            RightArrowRect.x1 += iSinOffset + g_iRightArrowOffsetInX;
            RightArrowRect.x2 += iSinOffset + g_iRightArrowOffsetInX;
            RightArrowRect.y1 += iSnapTrackProgress;
            RightArrowRect.y2 += iSnapTrackProgress;
            DrawImage ( RightArrowRect, 1.0f, 0xffffffff, m_pRightArrowTexture );

            // Update the closest snap position so that if we disengage, we can use this value to
            // snap to the closest menu item
            m_iClosestSnapPosition = RightArrowRect.y1 - m_RightArrowDims.Height / 2;

            D3DRECT SelectedMenuItemRect = MenuRect;
            SelectedMenuItemRect.x1 += iOffset;
            SelectedMenuItemRect.x2 += iOffset;
            DrawImage( SelectedMenuItemRect, fAlpha, 0xFF00FFFF, m_pMenuTexture[uIndex][ iSelected  ] ); 
            // move the rect down for next button
        }
        else
        {

            DrawImage( MenuRect, fAlpha, 0xFF00FFFF, m_pMenuTexture[uIndex][ iSelected ] ); 
            // move the rect down for next button
            if ( m_bMenuSelected[ uIndex ] )
            {
                PlaySound( SOUND_DETACH_HANDLE_VARIANT2 );
            }
            m_bMenuSelected[ uIndex ] = FALSE;
        }
        MenuRect.y1 += m_MenuButtonDims.Height + g_iSpaceBetweenMenuItems;
        MenuRect.y2 += m_MenuButtonDims.Height + g_iSpaceBetweenMenuItems;
        
    }
    PIXEndNamedEvent();
}


//--------------------------------------------------------------------------------------
// Name: DrawAndUpdateStateConfirm()
// Desc: Draw confirm mode
//--------------------------------------------------------------------------------------
VOID Sample::DrawAndUpdateStateConfirm ()
{
    PIXBeginNamedEvent( 0, "Confirm" );

    UINT uWidth, uHeight;
    ATG::GetVideoSettings( &uWidth, &uHeight );

    D3DRECT rect;
    FLOAT fAlpha;
    FLOAT fProgress = 0.0f;
    NUI_HANDLES_RAILSTATE RailState;

    NUI_HANDLES_GRIP* pClosestGrip = NULL;
    FindClosestGrip( &pClosestGrip, m_fLeftTransparent, m_fRightTransparent );

    NUI_HANDLES_GRIP* pAttachedGrip;
    CalculateStandardButtonData( &g_BackButtonGrip, 
        XMFLOAT2( (FLOAT)m_BackButtonDims.Width, (FLOAT)m_BackButtonDims.Height ),
        &pAttachedGrip, &fAlpha, &rect, &RailState );
    fAlpha = max( g_fTransparent, max( m_fLeftTransparent, m_fRightTransparent ) );

    if ( pAttachedGrip != NULL ) 
    {
        if ( !m_bBackSelected )
        {
            PlaySound( SOUND_ATTACH_HANDLE );
        }

        m_bBackSelected = TRUE;
        m_bDrawCursor = FALSE;
        fProgress = RailState.Arc.fProgress;
        LONG iProgress = (LONG)(fProgress * (FLOAT)g_iStandardButtonMovementInPixels);
        D3DRECT TransparentRect = rect;
        TransparentRect.x1 -= g_iStandardButtonMovementInPixels;
        TransparentRect.x2 -= g_iStandardButtonMovementInPixels;
        DrawImage( TransparentRect, g_fTransparent, 0xFF00FFFF, m_pBackTexture[ 0 ] );
        rect.x1 -=iProgress;
        rect.x2 -=iProgress;
    }
    else
    {
        if ( m_bBackSelected )
        {
            PlaySound( SOUND_DETACH_HANDLE );
        }

        m_bBackSelected = FALSE;
    }

    if ( pClosestGrip == &g_BackButtonGrip )
    {
        fAlpha = pAttachedGrip ? 1.0f : fAlpha;
    }
    else
    {
        fAlpha = g_fTransparent;
    }
    DrawImage( rect, fAlpha, 0xFF00FFFF, m_pBackTexture[ 1 ] );
    if ( pAttachedGrip != NULL && fProgress >= 1.0f )
    {
        if ( m_bBackSelected )
        {
            PlaySound( SOUND_SELECT );
        }

        m_bBackSelected = FALSE;
        g_GameState = GAME_STATE_MENU; 
        PlaySound ( SOUND_CURSOR_LOOPED );
        NuiHandlesGripForceDetach( &g_BackButtonGrip );

        // Only enable the grips for each particulare scene
        for ( UINT i = 0; i < NUM_GRIPS; i++ )
        {
            NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
        }
    }

    CalculateStandardButtonData( &g_StartButtonGrip,
        XMFLOAT2( (FLOAT)m_BackButtonDims.Width, (FLOAT)m_BackButtonDims.Height ),
        &pAttachedGrip, &fAlpha, &rect, &RailState );

    fAlpha = max( g_fTransparent, max( m_fLeftTransparent, m_fRightTransparent ) );

    if ( pAttachedGrip != NULL ) 
    {
        if ( !m_bStartSelected )
        {
            PlaySound( SOUND_ATTACH_HANDLE );
        }

        m_bStartSelected = TRUE;
        m_bDrawCursor = FALSE;
        fProgress = RailState.Arc.fProgress;
        LONG iProgress = (LONG)(fProgress * (FLOAT)g_iStandardButtonMovementInPixels );
        D3DRECT TransparentRect = rect;
        TransparentRect.x1 += g_iStandardButtonMovementInPixels;
        TransparentRect.x2 += g_iStandardButtonMovementInPixels;
        DrawImage( TransparentRect, g_fTransparent, 0xFF00FFFF, m_pStartTexture[ 0 ] );
        rect.x1 +=iProgress;
        rect.x2 +=iProgress;
    }
    else
    {
        if ( m_bStartSelected )
        {
            PlaySound( SOUND_DETACH_HANDLE );
        }

        m_bStartSelected = FALSE;
    }

    if ( pClosestGrip == &g_StartButtonGrip )
    {
        fAlpha = pAttachedGrip ? 1.0f : fAlpha;
    }
    else
    {
        fAlpha = g_fTransparent;
    }

    DrawImage( rect, fAlpha, 0xFF00FFFF, m_pStartTexture[ 1 ] );
    if ( pAttachedGrip != NULL && fProgress >=1.0f )
    {
        if ( m_bStartSelected )
        {
            PlaySound( SOUND_SELECT );
        }

        m_bStartSelected = FALSE;
        g_GameState = GAME_STATE_GAME; 
        g_GlobalBackgroundColor1 = 0xff3f6385;
        g_GlobalBackgroundColor2 = 0xffcebdad;
        NuiHandlesGripForceDetach( &g_StartButtonGrip );

        // Only enable the grips for each particulare scene
        for ( UINT i = 0; i < NUM_GRIPS; i++ )
        {
            NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
        }
    }
    m_Font.Begin();
        m_Font.SetScaleFactors( 2.0f, 2.0f );
        m_Font.DrawText( 0.0f, 150.0f, 0xffffffff, L"To start a new game use the start button." );
        m_Font.DrawText( 0.0f, 200.0f, 0xffffffff, L"To go back to the menu use the back button." );
    m_Font.End();

    PIXEndNamedEvent();


}


//--------------------------------------------------------------------------------------
// Name: DrawAndUpdateStateBackground()
// Desc: Draw background mode
//--------------------------------------------------------------------------------------
VOID Sample::DrawAndUpdateStateBackground ()
{
    PIXBeginNamedEvent( 0, "Background" );

    UINT uWidth, uHeight;
    ATG::GetVideoSettings( &uWidth, &uHeight );

    // Cursor state

    D3DRECT rect;
    FLOAT fAlpha;
    FLOAT fProgress = 0.0f;
    NUI_HANDLES_RAILSTATE RailState;
    memset( &RailState, 0, sizeof(NUI_HANDLES_RAILSTATE) );
    NUI_HANDLES_GRIP* pClosestGrip = NULL;
    FindClosestGrip( &pClosestGrip, m_fLeftTransparent, m_fRightTransparent );

    NUI_HANDLES_GRIP* pAttachedGrip = NULL;

    CalculateStandardButtonData( &g_BackButtonGrip, 
        XMFLOAT2( (FLOAT)m_BackButtonDims.Width, (FLOAT)m_BackButtonDims.Height ),
        &pAttachedGrip, &fAlpha, &rect, &RailState );

    if ( pAttachedGrip == &g_BackButtonGrip ) 
    {
        if ( !m_bBackSelected )
        {
            PlaySound( SOUND_ATTACH_HANDLE );
        }

        m_bBackSelected = TRUE;
        m_bDrawCursor = FALSE;
        fProgress = RailState.Arc.fProgress;
        LONG iProgress = (LONG)(fProgress * (FLOAT)g_iStandardButtonMovementInPixels);
        D3DRECT TransparentRect = rect;
        TransparentRect.x1 -= g_iStandardButtonMovementInPixels;
        TransparentRect.x2 -= g_iStandardButtonMovementInPixels;
        DrawImage( TransparentRect, g_fTransparent, 0xFF00FFFF, m_pBackTexture[ 0 ] );
        rect.x1 -=iProgress;
        rect.x2 -=iProgress;
    }
    else
    {
        if ( m_bBackSelected )
        {
            PlaySound( SOUND_DETACH_HANDLE );
        }

        m_bBackSelected = FALSE;
    }

    // render with transparency calculated above
    if ( pClosestGrip == &g_BackButtonGrip )
    {
        fAlpha = pAttachedGrip ? 1.0f : fAlpha;
    }
    else
    {
        fAlpha = g_fTransparent;
    }
    DrawImage( rect, pAttachedGrip ? 1.0f : fAlpha, 0xFF00FFFF, m_pBackTexture[ 1 ] );

    if ( pAttachedGrip != NULL && fProgress >= 1.0f )
    {
        if ( m_bBackSelected )
        {
            PlaySound( SOUND_SELECT );
        }

        m_bBackSelected = FALSE;
        g_GameState = GAME_STATE_MENU; 
        PlaySound ( SOUND_CURSOR_LOOPED );
        NuiHandlesGripForceDetach( &g_BackButtonGrip );

        // Only enable the grips for each particulare scene
        for ( UINT i = 0; i < NUM_GRIPS; i++ )
        {
            NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
        }
    }     

    XMFLOAT2 fGripPos = GetXYForVector( NuiHandlesGripGetScreenSpaceLocation( &g_JoystickGrip ) );

    memset( &RailState, 0, sizeof(NUI_HANDLES_RAILSTATE) );

    if ( NuiHandlesGripGetIsAttached( &g_JoystickGrip ) )
    {
        pAttachedGrip = &g_JoystickGrip;       
        DWORD dwRailId;
        NuiHandlesGripGetCommittedRailId( pAttachedGrip, &dwRailId );
        NuiHandlesGripGetRailState( pAttachedGrip, dwRailId, &RailState );
    }

    // render with transparency calculated above
    if ( pClosestGrip == &g_JoystickGrip )
    {
        fAlpha = pAttachedGrip ? 1.0f : fAlpha;
    }
    else
    {
        fAlpha = g_fTransparent;
    }

    D3DRECT JoystickGripRect;
    JoystickGripRect.x1 = (LONG)fGripPos.x - m_JoystickDims.Width / 2 + g_iTPadCenterOffsetInPixelsInX;
    JoystickGripRect.x2 = (LONG)fGripPos.x + m_JoystickDims.Width / 2 + g_iTPadCenterOffsetInPixelsInX;
    JoystickGripRect.y1 = (LONG)fGripPos.y - m_JoystickDims.Height / 2 + g_iTPadCenterOffsetInPixelsInY;
    JoystickGripRect.y2 = (LONG)fGripPos.y + m_JoystickDims.Height / 2 + g_iTPadCenterOffsetInPixelsInY;

    DrawImage( JoystickGripRect, fAlpha, 0xFF00FFFF, m_pJoystickTexture );

    D3DRECT CursorRect;
    CursorRect.x1 = (LONG)fGripPos.x - m_MenuGripDims.Width / 2;
    CursorRect.x2 = (LONG)fGripPos.x + m_MenuGripDims.Width / 2;
    CursorRect.y1 = (LONG)fGripPos.y - m_MenuGripDims.Height / 2;
    CursorRect.y2 = (LONG)fGripPos.y + m_MenuGripDims.Height / 2;

    if ( pAttachedGrip == &g_JoystickGrip ) 
    {
        if ( !m_bHandleSelected )
        {
            PlaySound( SOUND_ATTACH_HANDLE );
        }
        m_bHandleSelected = TRUE;

        // calcualte the cursor location as progress along the rail                        
        if ( RailState.dwRailID == 1 )
        {
            LONG iProgress = (LONG)( RailState.Arc.fProgress * (FLOAT)g_iStandardButtonMovementInPixels );
            // rail ID 1 is the elect rail
            // move the cursor up along this rail
            CursorRect.y1 -= iProgress;
            CursorRect.y2 -= iProgress;
            if ( RailState.Arc.fProgress >= 0.75f )
            {
                // if the user selected then change state and detach handles
                PlaySound( SOUND_SELECT );
                g_GlobalBackgroundColor2 = g_BackGroundColors[g_iCurrentPositionInColorList2];
                NuiHandlesGripForceDetach( &g_JoystickGrip );

                // Only enable the grips for each particulare scene
                for ( UINT i = 0; i < NUM_GRIPS; i++ )
                {
                    NuiHandlesGripSetEnabled( (NUI_HANDLES_GRIP*)g_pGripList[ i ], FALSE );
                }
            }
        }
        else if ( RailState.dwRailID == 2 )
        {
            LONG iProgress = (LONG)( RailState.Arc.fProgress * 200.0f );
            // move cursor left
            CursorRect.x1 -= iProgress;
            CursorRect.x2 -= iProgress;
            //scroll the list based on the distance along the rail
            // make the list scroll logarithmically
            g_fCurrentPositionInColorList2 += ( (RailState.Arc.fProgress * RailState.Arc.fProgress) * 0.005f );
        }
        else if ( RailState.dwRailID == 3 )
        {
            LONG iProgress = (LONG)( RailState.Arc.fProgress * 200.0f );
            // move the cursor right
            CursorRect.x1 += iProgress;
            CursorRect.x2 += iProgress;
            // scroll the list left based on the position along the rail
            // make the list scroll logarithmically
            g_fCurrentPositionInColorList2 -= ( (RailState.Arc.fProgress * RailState.Arc.fProgress) * 0.005f );
        }
        // clamp the cursor between 0 and 1
        g_fCurrentPositionInColorList2 = min( 1.0f, max ( 0.0f, g_fCurrentPositionInColorList2 ) );
        DrawImage( CursorRect, 1.0f, g_GripColor, m_pGripTexture[ 1 ] );
        DrawImage( CursorRect, 0.75f, g_CursorColor, m_pCursorTexture);

        m_bDrawCursor = FALSE;
    }
    else 
    {
        if ( m_bHandleSelected )
        {
            PlaySound( SOUND_DETACH_HANDLE );
        }
        m_bHandleSelected = FALSE;

        // Render a ghost of the cursor to show the player where to attach to

        DrawImage( CursorRect, fAlpha, g_GripColor, m_pGripTexture[1] );
        
    }

    // This code renders the list of colors as they iterate by
    D3DRECT BackgroundColorRects;
    FLOAT f0To100Position = g_fCurrentPositionInColorList2 * (FLOAT)g_iTotalColors;
    g_iCurrentPositionInColorList2 =  (LONG)floor( f0To100Position );
    FLOAT fExcess = f0To100Position - (FLOAT)(g_iCurrentPositionInColorList2); 
    LONG iStartingOffset = (LONG)( fExcess * 200.0f );
    BackgroundColorRects.x1 = 440 + iStartingOffset - 600;
    BackgroundColorRects.y1 = 250 ;
    BackgroundColorRects.x2 = 640 + iStartingOffset - 600;
    BackgroundColorRects.y2 = 450 ;
    for ( int index = 3; index > -4; --index )
    {
        if ( g_iCurrentPositionInColorList2 + index >= 0
            && g_iCurrentPositionInColorList2 + index < g_iTotalColors )
        {
            DrawImageColored( BackgroundColorRects, 1.0f, 
                g_BackGroundColors[g_iCurrentPositionInColorList2 + index], m_pMenuTexture[0][ m_bMenuSelected[ 0 ] ] );
       
        }
        BackgroundColorRects.x1 += 200;
        BackgroundColorRects.x2 += 200;
    }          

    PIXEndNamedEvent();

}

//--------------------------------------------------------------------------------------
// Name: UpdateProximitySound()
// Desc: When we aren't attached to a handle, play and/or update audio "beacon" if the cursor is close to a grip
//--------------------------------------------------------------------------------------
VOID Sample::UpdateProximitySound( FLOAT fMinDistanceToHandle )
{

	if ( fMinDistanceToHandle < SOUND_NOTIFY_MAX_DISTANCE && !m_fAttachedToHandle )
	{
		if ( GAME_STATE_MENU != g_GameState ) // demonstrate proximity chirps on all pages but main menu
			{
				FLOAT chirp_freq = 0;
				FLOAT curr_time = ( FLOAT )m_Timer.GetAbsoluteTime();
				if ( fMinDistanceToHandle <= SOUND_NOTIFY_MIN_DISTANCE )
				{
					chirp_freq = SOUND_NOTIFY_MIN_PERIOD;
				}
				else
				{
					chirp_freq = ( (fMinDistanceToHandle - SOUND_NOTIFY_MIN_DISTANCE) * ( SOUND_NOTIFY_MAX_PERIOD - SOUND_NOTIFY_MIN_PERIOD ) / SOUND_NOTIFY_MAX_DISTANCE ) + SOUND_NOTIFY_MIN_PERIOD;
				}
				if ( ( m_fTimeOfLastProximityChirp + chirp_freq) <= curr_time) // send an acoustic ping to user
				{
					m_fTimeOfLastProximityChirp = curr_time;
					PlaySound ( SOUND_PROXIMITY );
					// also amplify volume based on distance
					FLOAT chirp_volume = 1.0;
					if ( fMinDistanceToHandle > SOUND_NOTIFY_MIN_DISTANCE )
						chirp_volume = .8f * (fMinDistanceToHandle - SOUND_NOTIFY_MAX_DISTANCE) / (SOUND_NOTIFY_MIN_DISTANCE - SOUND_NOTIFY_MAX_DISTANCE);
					m_pSourceVoice[ SOUND_PROXIMITY ]->SetVolume( chirp_volume, XAUDIO2_COMMIT_NOW );
					// and why not adjust the pitch too?
					// m_pSourceVoice[ SOUND_PROXIMITY ]->SetFrequencyRatio( SOUND_NOTIFY_MIN_DISTANCE / fMinDistance , XAUDIO2_COMMIT_NOW );
				}
			}
		else // in menu, show looping continuous proximity
		{
			// adjust pitch based on distance; simple linear curve from SOUND_NOTIFY_MAX_FREQRATIO at min distance to SOUND_NOTIFY_MIN_FREQRATIO at max distance
			FLOAT chirp_freqratio = SOUND_NOTIFY_MIN_FREQRATIO;
			if ( fMinDistanceToHandle > SOUND_NOTIFY_MIN_DISTANCE )
				chirp_freqratio = ( ( fMinDistanceToHandle + 1.0f) * ( SOUND_NOTIFY_MAX_FREQRATIO - SOUND_NOTIFY_MIN_FREQRATIO) / (SOUND_NOTIFY_MIN_DISTANCE - SOUND_NOTIFY_MAX_DISTANCE)) - SOUND_NOTIFY_MAX_FREQRATIO;

			// adjust volume based on distance; simple linear curve from 1.0 @ min distance to 0.0 @ max distance
			FLOAT chirp_volume = 1.0;
			if ( fMinDistanceToHandle > SOUND_NOTIFY_MIN_DISTANCE )
				chirp_volume = (fMinDistanceToHandle - SOUND_NOTIFY_MAX_DISTANCE) / (SOUND_NOTIFY_MIN_DISTANCE - SOUND_NOTIFY_MAX_DISTANCE);
			m_pSourceVoice [ SOUND_CURSOR_LOOPED ]->SetVolume ( chirp_volume, XAUDIO2_COMMIT_NOW );

		}
	}
	else
	{
		m_fTimeOfLastProximityChirp = 0.0f;
		m_pSourceVoice [ SOUND_CURSOR_LOOPED ]->SetVolume ( 0.0f, XAUDIO2_COMMIT_NOW );
	}

}


//--------------------------------------------------------------------------------------
// Name: DrawHandlesAndUpdateHandlesGameState()
// Desc: Render the correct handles and their affordances based on the game state.
// This function also does some state managment.  This could be done in a seperate update function
// to seperate rendering from control code.  It's done here for simplicity.
//--------------------------------------------------------------------------------------
VOID Sample::DrawHandlesAndUpdateHandlesGameState()
{
    PIXBeginNamedEvent( 0, "DrawHandles" );

    m_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 16 );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR );
    m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR );

    m_bDrawCursor = TRUE;
    m_fLeftTransparent = 0.0f;
    m_fRightTransparent = 0.0f;

    switch ( g_GameState ) 
    {
        case GAME_STATE_GAME:
        {
            DrawAndUpdateStateGame();

            break;
        }

        case GAME_STATE_MENU:
        {
            DrawAndUpdateStateMenu(); 

            break;
        }
        case GAME_STATE_CONFIRM:
        {
            DrawAndUpdateStateConfirm(); 

            break;
        }

        case GAME_STATE_BACKGROUND:
        {
            DrawAndUpdateStateBackground(); 

            break;
        }
    }

    m_fAttachedToHandle = !m_bDrawCursor;

    // if the player is not engaged / facing the camera, then don't render the cursor
    // When a handle is attached we don't draw the cursor because it's being rendered along the rail or as the rail
    if ( m_bPlayerEngaged && 
         m_bDrawCursor &&
         NuiHandlesArmsGetArmStatus( &g_HandlesArms ) == NUI_HANDLES_ARM_STATUS_ENGAGED
         )
    {
        PIXBeginNamedEvent( 0, "Cursor" );
        D3DRECT rect;
        // Get the current cursor position
        XMFLOAT2 fLeftHandCursor = GetXYForVector( NuiHandlesArmGetScreenSpaceLocation( &g_HandlesArms, NUI_HANDLES_ARMS_HANDEDNESS_LEFT_ARM ) );
        XMFLOAT2 fRightHandCursor = GetXYForVector( NuiHandlesArmGetScreenSpaceLocation( &g_HandlesArms, NUI_HANDLES_ARMS_HANDEDNESS_RIGHT_ARM ) );

        rect.x1 = (LONG)fLeftHandCursor.x - m_CursorDims.Width / 2;
        rect.x2 = (LONG)fLeftHandCursor.x + m_CursorDims.Width / 2;          
        rect.y1 = (LONG)fLeftHandCursor.y - m_CursorDims.Height / 2;
        rect.y2 = (LONG)fLeftHandCursor.y + m_CursorDims.Height / 2; 
        DrawImage( rect, 0.8f, g_CursorOnRailColor[0], m_pCursorTexture );

        rect.x2 = rect.x2 - m_CursorDims.Width / 4;
        rect.x1 = rect.x1 + m_CursorDims.Width / 4;
        rect.y1 = rect.y1 + m_CursorDims.Width / 4;
        rect.y2 = rect.y2 - m_CursorDims.Width / 4;
        DrawImageColored( rect, m_fLeftTransparent, g_CursorOnRailColor[0], m_pCursorTexture );

        rect.x1 = (LONG)fRightHandCursor.x - m_CursorDims.Width / 2;
        rect.x2 = (LONG)fRightHandCursor.x + m_CursorDims.Width / 2;          
        rect.y1 = (LONG)fRightHandCursor.y - m_CursorDims.Height / 2;
        rect.y2 = (LONG)fRightHandCursor.y + m_CursorDims.Height / 2; 
        DrawImage( rect, 0.8f, g_CursorOnRailColor[1], m_pCursorTexture );

        rect.x2 = rect.x2 - m_CursorDims.Width / 4;
        rect.x1 = rect.x1 + m_CursorDims.Width / 4;
        rect.y1 = rect.y1 + m_CursorDims.Width / 4;
        rect.y2 = rect.y2 - m_CursorDims.Width / 4;
        DrawImageColored( rect, m_fRightTransparent, g_CursorOnRailColor[1], m_pCursorTexture );

        PIXEndNamedEvent();
    }

    PIXEndNamedEvent();
}


//--------------------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3D rendering. This 
//       function sets up render states, clears the viewport, and renders the scene.
//--------------------------------------------------------------------------------------
HRESULT Sample::Render()
{

	   UINT uWidth, uHeight;
    ATG::GetVideoSettings( &uWidth, &uHeight );
	// Render the background image
    D3DRECT BackgroundTextureRect = { 0, 0, uWidth, uHeight };
    DrawImage( BackgroundTextureRect, 1.0f, 0xffffffff, m_pBackgroundTexture );

    // Show title, frame rate, and help
    m_Timer.MarkFrame();

    DrawHandlesAndUpdateHandlesGameState();

    m_CameraManager.DisplayPIP();

    // Render the HUD
    m_Font.Begin();
    m_Font.SetScaleFactors( 1.2f, 1.2f );
    m_Font.DrawText( 500.0f, 0.0f, 0xffffffff, L"XenonNui", ATGFONT_CENTER_X );
    m_Font.SetScaleFactors( 1.0f, 1.0f );
    m_Font.DrawText( 1024.0f, 25.0f, 0xffffff00, m_Timer.GetFrameRate(), ATGFONT_RIGHT );

	m_Font.SetScaleFactors( 1.0f, 1.0f );
    m_Font.DrawText( 0.0f, -1.25f, 0xffffffff, L"Richard, Alex, Lucy, Eve, Gideon and Simion", ATGFONT_LEFT);

	m_Font.SetScaleFactors( 1.0f, 1.0f );
    m_Font.DrawText( 0.0f, 1.25f, 0xffffffff, L"Build 0.39", ATGFONT_LEFT);
    
    // Render Smoothing and Tilt Correction options

    m_Font.End();

    if( m_bDrawHelp )
    {
        m_Help.Render( &m_Font, g_HelpCallouts, NUM_HELP_CALLOUTS );
    }


    // Present the scene
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

    return S_OK;
}


//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::InitAudio
// Desc: Initializes XAudio2, and loads our menu sounds into memory.
//----------------------------------------------------------------------------------------------------------------------
HRESULT Sample::InitAudio()
{
    HRESULT hr;

    // Create XAudio2

    if( FAILED( hr = XAudio2Create( &m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR ) ) )
        return hr;

    // Create a mastering voice

    if( FAILED( hr = m_pXAudio2->CreateMasteringVoice( &m_pMasteringVoice,
        XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL
        ) ) )
    {
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    LoadWavFile( SOUND_ATTACH_HANDLE );
    LoadWavFile( SOUND_DETACH_HANDLE );
    LoadWavFile( SOUND_SELECT );
	LoadWavFile( SOUND_PROXIMITY );
	LoadWavFile( SOUND_ATTACH_HANDLE_VARIANT2);
	LoadWavFile( SOUND_DETACH_HANDLE_VARIANT2);
	LoadWavFile( SOUND_SELECT_VARIANT2 );
	LoadWavFile( SOUND_PROXIMITY_LOOPED );
	LoadWavFile( SOUND_CURSOR_LOOPED );

    ATG::DebugSpew( "Initialized audio...\n" );

    return hr;
}

//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::LoadWavFile
// Desc: Loads a wav file from disk.
//----------------------------------------------------------------------------------------------------------------------
void Sample::LoadWavFile( Sound value )
{
    assert( value < SOUND_COUNT );

    HRESULT hr;

    XAUDIO2_BUFFER& buffer = m_audioBuffers[ value ];
    if ( buffer.pAudioData != NULL )
    {
        ATG_PrintError( "Loading wav file in already used slot\n" );
        return;
    }

    LPCSTR strAudioFile = s_SoundFiles[ value ];
    
    // Read the wave file
    ATG::WaveFile WaveFile;
    if( FAILED( hr = WaveFile.Open( strAudioFile ) ) )
    {
        ATG::FatalError( "Error %#X opening WAV file %s\n", hr, strAudioFile );
    }

    // Read the format header

    WAVEFORMATEXTENSIBLE wfx = {0};
    if( FAILED( hr = WaveFile.GetFormat( &wfx ) ) )
    {
        ATG::FatalError( "Error %#X reading WAV format from %s\n", hr, strAudioFile );
    }

    // Calculate how many bytes and samples are in the wave

    DWORD cbWaveSize = 0;
    WaveFile.GetDuration( &cbWaveSize );

    // Read the sample data into memory

    BYTE* pbWaveData = (BYTE*)malloc( cbWaveSize );
    if( FAILED( hr = WaveFile.ReadSample( 0, pbWaveData, cbWaveSize, &cbWaveSize ) ) )
    {
        ATG::FatalError( "Error %#X reading WAV data\n", hr );
    }

    buffer.pAudioData = pbWaveData;
    buffer.Flags = XAUDIO2_END_OF_STREAM;  // mark the audio buffer as "ok for the source voice to starve"
    buffer.AudioBytes = cbWaveSize;

	if( SOUND_PROXIMITY_LOOPED == value || SOUND_CURSOR_LOOPED == value)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	else
	{
	    buffer.LoopCount = 0;
	}

    // If this is the first WAV file we've loaded, create the source voice.
    // NOTE: all WAVs loaded by this function must be of the same format.

    assert( !m_pSourceVoice[ value ] );
    if( FAILED( hr = m_pXAudio2->CreateSourceVoice( &( m_pSourceVoice[ value ] ),
        ( WAVEFORMATEX* )&wfx, XAUDIO2_VOICE_USEFILTER) ) )
    {
        ATG::FatalError( "Error %#X creating source voice\n", hr );
    }
}


//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::DestroyAudio
// Desc: Destroys the audio engine, and releases any resources we have used.
//----------------------------------------------------------------------------------------------------------------------
void Sample::DestroyAudio()
{
    if ( m_pXAudio2 )
    {
        m_pXAudio2->StopEngine();
    }

    if ( m_pMasteringVoice )
    {
        m_pMasteringVoice->DestroyVoice();
        m_pMasteringVoice = NULL;
    }

    for( int i = 0; i < SOUND_COUNT; ++i )
    {
        if ( m_audioBuffers[ i ].pAudioData != NULL )
        {
            if ( m_pSourceVoice[ i ] )
            {
                m_pSourceVoice[ i ] ->DestroyVoice();
                m_pSourceVoice[ i ] = NULL;
            }

            free( const_cast<BYTE*>(m_audioBuffers[ i ].pAudioData) );
            m_audioBuffers[ i ].pAudioData = NULL;
        }
    }

    ::ZeroMemory( &m_audioBuffers, sizeof(m_audioBuffers) );

    if ( m_pXAudio2 )
    {
        m_pXAudio2->Release();
        m_pXAudio2 = NULL;
    }

    ATG::DebugSpew( "Shutdown audio...\n" );
}



//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::PlaySound
// Desc: Plays a specific one-shot sound.
//----------------------------------------------------------------------------------------------------------------------
HRESULT Sample::PlaySound( Sound value )
{
    HRESULT hr;

    if ( m_fPlayingSound[ value ] )
    {
        hr = StopSound( value );
        if ( FAILED( hr ) )
        {
            ATG_PrintError( "Couldn't play sound; StopSound failed\n" );
            return hr;
        }
    }

    hr = m_pSourceVoice[ value ]->SubmitSourceBuffer( &( m_audioBuffers[ value ] ), NULL );
    if ( FAILED( hr ) )
    {
        ATG::FatalError( "Couldn't submit source buffer %d\n", value );
    }

    if ( SOUND_CURSOR_LOOPED == value) // special case: for looping sounds, trigger muted as screen appears (sound won't be updated until title needs it)
		m_pSourceVoice[ value ]->SetVolume( 0.0f, XAUDIO2_COMMIT_NOW );

    __analysis_assume( value < SOUND_COUNT );

	hr = m_pSourceVoice[ value ]->Start( 0, XAUDIO2_COMMIT_NOW );

    if ( SUCCEEDED( hr ) )
    {
        m_fPlayingSound[ value ] = TRUE;
    }
    else
    {
        ATG_PrintError( "Couldn't start source voice\n" );
    }

    return hr;
}



//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::Sample
// Desc: Constructor for Sample object.
//----------------------------------------------------------------------------------------------------------------------
Sample::Sample()
: m_pXAudio2( NULL ),
  m_pMasteringVoice( NULL ),
  m_fAttachedToHandle( FALSE )
{
    ::ZeroMemory( m_pSourceVoice, sizeof( m_pSourceVoice ) );
    ::ZeroMemory( m_fPlayingSound, sizeof( m_fPlayingSound ) );
    ::ZeroMemory( m_audioBuffers, sizeof( m_audioBuffers ) );
}


//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::~Sample
// Desc: Destroys the sample
//----------------------------------------------------------------------------------------------------------------------
Sample::~Sample()
{
    DestroyAudio();
}


//----------------------------------------------------------------------------------------------------------------------
// Name: Sample::StopSound
// Desc: Stops a playing sound.
//----------------------------------------------------------------------------------------------------------------------
HRESULT Sample::StopSound( Sound value )
{
    HRESULT hr = S_OK;
    
    if ( m_fPlayingSound[ value ] )
    {

        hr = m_pSourceVoice[ value ]->Stop( 0, XAUDIO2_COMMIT_NOW );

        if ( SUCCEEDED( hr ) )
        {
            m_fPlayingSound[ value ] = FALSE;
        }
        else
        {
            ATG_PrintError( "Couldn't stop source voice\n" );
            return hr;
        }

        hr = m_pSourceVoice[ value ]->FlushSourceBuffers();
        if ( FAILED( hr ) )
        {
            ATG_PrintError( "Couldn't flush source buffers\n" );
        }

    }

    return hr;
}
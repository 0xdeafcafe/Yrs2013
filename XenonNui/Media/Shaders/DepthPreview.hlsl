
//--------------------------------------------------------------------------------------
// DepthPreview.hlsl
//
// This file contains the shaders for the depth preview visualization in the
// AtgNuiVisualization.
//
// Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#define NUI_DEPTHMAP_WIDTH 320
#define NUI_DEPTHMAP_HEIGHT 240

#define NUI_MINIPIP_SMOOTH_WIDTH 128
#define NUI_MINIPIP_SMOOTH_PITCH 128
#define NUI_MINIPIP_SMOOTH_HEIGHT 96

#define SEG_FACTOR 8.0f
#define NUI_MINIPIP_GLOW_START_SIZE 50
#define NUI_MINIPIP_GLOW_END_SIZE 10
#define NUI_MINIPIP_GLOW_START_RANGE 1.0f
#define NUI_MINIPIP_GLOW_END_RANGE 4.0f

// Constants
static const float4 g_vEngagedPlayerColor           = float4( 0.8f, 0.3f, 1.0f, 1.0f );
static const float4 g_vTrackedPlayerColor           = float4( 0.5f, 0.5f, 0.5f, 1.0f );
static const float4 g_vMemExportOffset              = float4(0.0, 1.0, 0.0, 0.0);
static const float4 g_vScreenToWorldOffsets         = float4(-319.500000, -239.500000, 0.001751, -0.001751);

struct VS_OUT
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

// Depth texture
sampler texDepth    : register(s0);

// Pixel shader constants
uniform bool    g_bPlayerEngaged        : register( b0 );
uniform float4  g_vColor                : register(c0);
uniform float4  g_vPlayerHeadPos        : register(c1);
uniform float4  g_vPlayerNeckPos        : register(c2);
uniform float4  g_vPlayerShoulderPos    : register(c3);
uniform float4  g_vPlayerPlane          : register(c4);
uniform float4  g_vPlayerLeftHandPos    : register(c5);
uniform float4  g_vPlayerRightHandPos   : register(c6);
uniform float4  g_vLeftHandPos[6]       : register(c8);
uniform float4  g_vRightHandPos[6]      : register(c14);
uniform float4  g_vOpacity              : register(c20);

// vertex shader constants
uniform float4  g_vMemExportStreamConstant : register(c0);

// 3x3 Gaussian filter
static const float g_Filter[ 9 ] = 
{
    1, 2, 1,
    2, 4, 2,
    1, 2, 1
};

//---------------------------------------------------------------------------------------------------------
// Name: GetAvgDepth
// Desc: Return depth and confidence
//---------------------------------------------------------------------------------------------------------
float2 GetAvgDepth( sampler tex, float2 texCoord )
{
    float Depth = 0;
    float Weight = 0;
    float CurrDepth[9];

    asm
    {
        tfetch2D CurrDepth[0].x, texCoord, tex, UseComputedLOD=false, OffsetX=-1, OffsetY=-1
        tfetch2D CurrDepth[1].x, texCoord, tex, UseComputedLOD=false, OffsetX=0,  OffsetY=-1
        tfetch2D CurrDepth[2].x, texCoord, tex, UseComputedLOD=false, OffsetX=1,  OffsetY=-1
        tfetch2D CurrDepth[3].x, texCoord, tex, UseComputedLOD=false, OffsetX=-1, OffsetY=0
        tfetch2D CurrDepth[4].x, texCoord, tex, UseComputedLOD=false, OffsetX=0,  OffsetY=0
        tfetch2D CurrDepth[5].x, texCoord, tex, UseComputedLOD=false, OffsetX=1,  OffsetY=0
        tfetch2D CurrDepth[6].x, texCoord, tex, UseComputedLOD=false, OffsetX=-1, OffsetY=1
        tfetch2D CurrDepth[7].x, texCoord, tex, UseComputedLOD=false, OffsetX=0,  OffsetY=1
        tfetch2D CurrDepth[8].x, texCoord, tex, UseComputedLOD=false, OffsetX=1,  OffsetY=1
    };

    for( int i = 0; i < 9; i++ )
    {
        Depth += ( CurrDepth[ i ] ? ( int )( CurrDepth[ i ] / SEG_FACTOR ) * g_Filter[ i ] : 0 );
        Weight += ( CurrDepth[ i ] ? g_Filter[ i ] : 0 );
    }

    if( Weight > 6 )
    {
        Depth = Depth / Weight;
        // Ensure weight (confidence) is in the 0 - 1.25f range.
        Weight = ( Weight - 6 ) / SEG_FACTOR;
        return float2( Depth, Weight );
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------
// Name: vs_DepthPreviewSmoothing
// Desc: memexport vertex shader that performs depth-map smoothing passes.
//---------------------------------------------------------------------------------------------------------
void vs_DepthPreviewSmoothing( int Index : INDEX )
{
    int y = Index / NUI_MINIPIP_SMOOTH_WIDTH;
    int x = Index - y * NUI_MINIPIP_SMOOTH_WIDTH;

    float2 TexCoord = float2( ( x + 0.5f ) / NUI_MINIPIP_SMOOTH_WIDTH, ( y + 0.5f ) / NUI_MINIPIP_SMOOTH_HEIGHT );

    // Output average depth, plus a maximum of 8.75 based on the weight from the average.
    float2 Value = GetAvgDepth( texDepth, TexCoord );
    float OutValue = 8.0f * floor( Value.x ) + 7.0f * saturate( Value.y );

    float iExportOffset = x + y * NUI_MINIPIP_SMOOTH_PITCH;

    asm
    {
        alloc export=1
        mad eA, iExportOffset, g_vMemExportOffset, g_vMemExportStreamConstant
        mov eM0, OutValue
    };
}

//---------------------------------------------------------------------------------------------------------
// Name: ScreenToWorldHelper
// Desc: Calculates world coordinates given 3D Nui coordinates.
//---------------------------------------------------------------------------------------------------------
float4 ScreenToWorldHelper( float4 screenspace )                   
{                                                            
   float screenMetersZ = ( float )screenspace.z / 1000.0f;     
   return float4(                                            
       ( screenspace.x + g_vScreenToWorldOffsets.x ) * g_vScreenToWorldOffsets.z * screenMetersZ,          
       ( screenspace.y + g_vScreenToWorldOffsets.y ) * g_vScreenToWorldOffsets.w * screenMetersZ,          
       screenMetersZ, 1.0f );                                
}    

//---------------------------------------------------------------------------------------------------------
// Name: ScreenToWorld
// Desc: Calculates world coordinates given Nui xy coordinates and depth
//---------------------------------------------------------------------------------------------------------
float4 ScreenToWorld( float2 Screen, float DepthValue )
{
     float4 ScreenPosition = float4( Screen.x * 2, Screen.y * 2, DepthValue, 1 );
     return ScreenToWorldHelper( ScreenPosition );
}

//---------------------------------------------------------------------------------------------------------
// Name: NormScreenToWorld
// Desc: Calculates world coordinates given normalized Nui xy coordinates and depth
//---------------------------------------------------------------------------------------------------------
float4 NormScreenToWorld( float2 NormScreen, float DepthValue )
{
    float2 Screen = NormScreen * float2( NUI_DEPTHMAP_WIDTH, NUI_DEPTHMAP_HEIGHT );
    float4 ScreenPosition = float4( Screen.x * 2, Screen.y * 2, DepthValue, 1 );
    return ScreenToWorldHelper( ScreenPosition );
}

//---------------------------------------------------------------------------------------------------------
// Name: AddLight
// Desc: Add light contribution to a pixel.
//---------------------------------------------------------------------------------------------------------
float3 AddLight( float3 Normal, float3 WorldPos, float3 LightPos, float3 DiffColor, float3 SpecColor, float SpecIdx, float Range )
{
    float Dist = length( LightPos - WorldPos );
    float3 LightDir = normalize( LightPos - WorldPos );

    float Diff = saturate( dot( Normal, LightDir ) );
    float Spec = dot( normalize( Normal + LightDir ), LightDir );
    float FallOff = pow( saturate( 1 - Dist / Range ), 2 );
    return ( Diff * DiffColor + pow( Spec, SpecIdx ) * SpecColor ) * FallOff;
}

//---------------------------------------------------------------------------------------------------------
// Name: ps_DepthPreview
// Desc: Pixel shader that produces the lit depth preview
//---------------------------------------------------------------------------------------------------------
float4 ps_DepthPreview( VS_OUT Input ) : COLOR
{
    float SegRatio = 0.7f;
    float3 Color = 0;

    float Depth = tex2D( texDepth, Input.TexCoord ) / 8.0f;
    float DepthValue = floor( Depth );
    float Confidence = Depth - DepthValue;
    float3 WorldPos = NormScreenToWorld( Input.TexCoord, DepthValue );

    // player segmentation & visualization

    if ( g_bPlayerEngaged )
    {
        if( DepthValue == 0 )
        {
            SegRatio = 0;
        }
        else
        {
            float DistToPlayerPlane = abs( dot( float4( WorldPos, 1 ), g_vPlayerPlane ) );
            SegRatio = 0.85f * saturate( 2 - DistToPlayerPlane / 0.2f );

            float d = distance( WorldPos, g_vPlayerHeadPos );
            SegRatio = SegRatio * ( 1.0f - saturate( d ) ) + 0.1f;
        }
    }

    if ( DepthValue != 0 )
    {
        float LeftDepth = 0;
        float RightDepth = 0;
        float TopDepth = 0;
        float BottomDepth = 0;
        float2 texcoord = Input.TexCoord;

        asm
        {
            tfetch2D LeftDepth.x,   texcoord, texDepth, OffsetX = -1
            tfetch2D RightDepth.x,  texcoord, texDepth, OffsetX = 1
            tfetch2D TopDepth.x,    texcoord, texDepth, OffsetY = -1
            tfetch2D BottomDepth.x, texcoord, texDepth, OffsetY = 1
        };

        LeftDepth /= 8.0f;
        RightDepth /= 8.0f;
        TopDepth /= 8.0f;
        BottomDepth /= 8.0f;
        
        float3 WorldPosLeft     = NormScreenToWorld( texcoord - float2( 1.0f / NUI_MINIPIP_SMOOTH_WIDTH, 0 ), LeftDepth );
        float3 WorldPosRight    = NormScreenToWorld( texcoord + float2( 1.0f / NUI_MINIPIP_SMOOTH_WIDTH, 0 ), RightDepth );
        float3 WorldPosTop      = NormScreenToWorld( texcoord - float2( 0.0f, 1.0f / NUI_MINIPIP_SMOOTH_HEIGHT ), TopDepth );
        float3 WorldPosBottom   = NormScreenToWorld( texcoord + float2( 0.0f, 1.0f / NUI_MINIPIP_SMOOTH_HEIGHT ), BottomDepth );

        float3 Normal = 0;
        float NumNormal = 0;
        if( LeftDepth != 0 && TopDepth != 0 )
        {
            Normal += normalize( cross( WorldPosLeft - WorldPos, WorldPosTop - WorldPos ) );
            ++NumNormal;
        }

        if( TopDepth != 0 && RightDepth != 0 )
        {
            Normal += normalize( cross( WorldPosTop - WorldPos, WorldPosRight - WorldPos ) ); 
            ++NumNormal;
        }

        if( RightDepth != 0 && BottomDepth != 0 )
        {
            Normal += normalize( cross( WorldPosRight - WorldPos, WorldPosBottom - WorldPos ) ); 
            ++NumNormal;
        }

        if( BottomDepth != 0 && LeftDepth != 0 )
        {
            Normal += normalize( cross( WorldPosBottom - WorldPos, WorldPosLeft - WorldPos ) );
            ++NumNormal;
        }

        if( NumNormal != 0 )
        {
            Normal = normalize( Normal );

            // lit with engaged hands

            float4 DiffColor = g_vEngagedPlayerColor;
            float4 SpecColor = 3 * DiffColor;

            if( g_vPlayerLeftHandPos.z != 0 )
            {
                Color += AddLight( Normal, WorldPos, g_vPlayerLeftHandPos, DiffColor, SpecColor, 32, 1.5f );
            }

            if( g_vPlayerRightHandPos.z != 0 )
            {
                Color += AddLight( Normal, WorldPos, g_vPlayerRightHandPos, DiffColor, SpecColor, 32, 1.5f );
            }

            if( g_bPlayerEngaged )
            {
                Color += 0.3f * AddLight( Normal, WorldPos, float3( 0.0f, 0.1f, 0.4f ), float4( 0.6f, 0.6f, 0.6f, 1.0f ), float4( 0.4f, 0.4f, 0.4f, 1.0f), 32, 10000 ); // no falloff
            }

            // lit with tracked hands

            DiffColor = g_vTrackedPlayerColor;
            SpecColor = 3 * DiffColor;

            for( int i = 0; i < 6; i++ )
            {
                if( g_vLeftHandPos[ i ].z != 0 )
                {
                    Color += AddLight( Normal, WorldPos, g_vLeftHandPos[ i ], DiffColor, SpecColor, 32, 0.75f );
                }

                if( g_vRightHandPos[ i ].z != 0 )
                {
                    Color += AddLight( Normal, WorldPos, g_vRightHandPos[ i ], DiffColor, SpecColor, 32, 0.75f );
                }
            }

            // ambient light

            float4 AmbientColor = float4( 0.7f, 0.7f, 0.7f, 1 );
            float3 ConstLightPos = float3( 0.385f, 0.0f, -0.923f );
            Color += saturate( dot( Normal, ConstLightPos ) ) * AmbientColor;
        }
    }
    return g_vOpacity * float4( Color * SegRatio * Confidence, 1.0f );
}


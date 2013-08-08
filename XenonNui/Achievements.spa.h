////////////////////////////////////////////////////////////////////
//
// Achievements.spa.h
//
// Auto-generated on Thursday, 08 August 2013 at 15:56:16
// Xbox LIVE Game Config project version 1.0.69.0
// SPA Compiler version 1.0.0.0
//
////////////////////////////////////////////////////////////////////

#ifndef __XENONNUI_SPA_H__
#define __XENONNUI_SPA_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// Title info
//

#define TITLEID_XENONNUI                            0x1ABC0A15

//
// Context ids
//
// These values are passed as the dwContextId to XUserSetContext.
//

#define CONTEXT_PLAYMODE                            0
#define CONTEXT_PADDLESIZE                          1
#define CONTEXT_BALLSPEED                           20
#define CONTEXT_POINTSTOEND                         30

//
// Context values
//
// These values are passed as the dwContextValue to XUserSetContext.
//

// Values for CONTEXT_PLAYMODE

#define CONTEXT_PLAYMODE_SINGLEPLAYER               0
#define CONTEXT_PLAYMODE_MULTIPLAYER                1

// Values for CONTEXT_PADDLESIZE

#define CONTEXT_PADDLESIZE_SMALL                    0
#define CONTEXT_PADDLESIZE_MEDIUM                   1
#define CONTEXT_PADDLESIZE_LARGE                    2

// Values for CONTEXT_BALLSPEED

#define CONTEXT_BALLSPEED_SLOW                      0
#define CONTEXT_BALLSPEED_FAST                      1

// Values for CONTEXT_POINTSTOEND

#define CONTEXT_POINTSTOEND_FIFTEEN                 0
#define CONTEXT_POINTSTOEND_TWENTYONE               1

// Values for X_CONTEXT_PRESENCE

#define CONTEXT_PRESENCE_SINGLEPLAYER               0
#define CONTEXT_PRESENCE_MULTIPLAYER                1
#define CONTEXT_PRESENCE_LOBBY                      2

// Values for X_CONTEXT_GAME_MODE

#define CONTEXT_GAME_MODE_SINGLEPLAYER              0
#define CONTEXT_GAME_MODE_MULTIPLAYER               1

//
// Property ids
//
// These values are passed as the dwPropertyId value to XUserSetProperty
// and as the dwPropertyId value in the XUSER_PROPERTY structure.
//

#define PROPERTY_GAMES                              0x10000011
#define PROPERTY_WINS                               0x10000022
#define PROPERTY_LOSSES                             0x10000033
#define PROPERTY_POINTS                             0x10000044
#define PROPERTY_GAMETIME                           0x10000055

//
// Achievement ids
//
// These values are used in the dwAchievementId member of the
// XUSER_ACHIEVEMENT structure that is used with
// XUserWriteAchievements and XUserCreateAchievementEnumerator.
//

#define ACHIEVEMENT_PLAYED1GAME                     100
#define ACHIEVEMENT_PLAYED10GAMES                   101
#define ACHIEVEMENT_PLAYED100GAMES                  102
#define ACHIEVEMENT_WON3INAROW                      200
#define ACHIEVEMENT_WON10INAROW                     201
#define ACHIEVEMENT_PONG_A_THON_I                   202

//
// AvatarAssetAward ids
//


//
// Stats view ids
//
// These are used in the dwViewId member of the XUSER_STATS_SPEC structure
// passed to the XUserReadStats* and XUserCreateStatsEnumerator* functions.
//

// Skill leaderboards for ranked game modes

#define STATS_VIEW_SKILL_RANKED_SINGLEPLAYER        0xFFFF0000
#define STATS_VIEW_SKILL_RANKED_MULTIPLAYER         0xFFFF0001

// Skill leaderboards for unranked (standard) game modes

#define STATS_VIEW_SKILL_STANDARD_SINGLEPLAYER      0xFFFE0000
#define STATS_VIEW_SKILL_STANDARD_MULTIPLAYER       0xFFFE0001

// Title defined leaderboards


//
// Stats view column ids
//
// These ids are used to read columns of stats views.  They are specified in
// the rgwColumnIds array of the XUSER_STATS_SPEC structure.  Rank, rating
// and gamertag are not retrieved as custom columns and so are not included
// in the following definitions.  They can be retrieved from each row's
// header (e.g., pStatsResults->pViews[x].pRows[y].dwRank, etc.).
//

//
// Matchmaking queries
//
// These values are passed as the dwProcedureIndex parameter to
// XSessionSearch to indicate which matchmaking query to run.
//


//
// Gamer pictures
//
// These ids are passed as the dwPictureId parameter to XUserAwardGamerTile.
//

#define GAMER_PICTURE_IMAGE_WON10INAROW             11
#define GAMER_PICTURE_IMAGE_PONGATHON1              12

//
// Strings
//
// These ids are passed as the dwStringId parameter to XReadStringsFromSpaFile.
//

#define SPASTRING_DUMMY                             99
#define SPASTRING_CTXTNAME_PLAY_MODE                101
#define SPASTRING_CTXTNAME_PADDLE_SIZE              102
#define SPASTRING_CTXTNAME_BALL_SPEED               103
#define SPASTRING_CTXTNAME_POINTS_TO_END            104
#define SPASTRING_ACHVNAME_PLAYED_1_GAME            110
#define SPASTRING_ACHVNAME_PLAYED_10_GAMES          111
#define SPASTRING_ACHVNAME_PLAYED_100_GAMES         112
#define SPASTRING_ACHVNAME_WON_3_IN_A_ROW           113
#define SPASTRING_ACHVNAME_WON_10_IN_A_ROW          114
#define SPASTRING_ACHVNAME_PONG_A_THON_I_VICTOR     115
#define SPASTRING_PROPNAME_GAMES                    200
#define SPASTRING_PROPNAME_WINS                     201
#define SPASTRING_PROPNAME_LOSSES                   202
#define SPASTRING_PROPNAME_POINTS                   203
#define SPASTRING_PROPNAME_GAME_TIME                204
#define SPASTRING_CTXTVAL_SINGLE_PLAYER             500
#define SPASTRING_CTXTVAL_MULTI_PLAYER              501
#define SPASTRING_CTXTVAL_SMALL                     502
#define SPASTRING_CTXTVAL_MEDIUM                    503
#define SPASTRING_CTXTVAL_LARGE                     504
#define SPASTRING_CTXTVAL_SLOW                      505
#define SPASTRING_CTXTVAL_FAST                      506
#define SPASTRING_CTXTVAL_15                        507
#define SPASTRING_CTXTVAL_21                        508
#define SPASTRING_RP_CONTEXT_0                      9000
#define SPASTRING_RP_CONTEXT_1                      9001
#define SPASTRING_RP_CONTEXT_2                      9002
#define SPASTRING_DUMMY_1                           10011
#define SPASTRING_DUMMY_2                           10012
#define SPASTRING_DUMMY_3                           10013
#define SPASTRING_DUMMY_4                           10014
#define SPASTRING_DUMMY_5                           10015
#define SPASTRING_DUMMY_6                           10016
#define SPASTRING_DUMMY_7                           10017
#define SPASTRING_DUMMY_8                           10018
#define SPASTRING_DUMMY_9                           10019
#define SPASTRING_DUMMY_10                          10020
#define SPASTRING_DUMMY_11                          10021
#define SPASTRING_DUMMY_12                          10022
#define SPASTRING_DUMMY_13                          10023


#ifdef __cplusplus
}
#endif

#endif // __XENONNUI_SPA_H__



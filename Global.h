#pragma once

// Grid dimensions
const int M = 50;
const int N = 80;

// Grid declaration
extern int grid[M][N];  // Declaration only, no initialization

// Tile size
extern int ts;  // Declaration only

// Offsets (extern declarations if used elsewhere)
extern const int offsetX;
extern const int offsetY;

// Game states
const int STATE_MAIN_SCREEN = 0;
const int STATE_LOGIN = 1;
const int STATE_MAIN_MENU = 2;
const int STATE_PLAYING_SINGLE = 3; // Updated from STATE_PLAYING
const int STATE_PLAYING_MULTI = 4;  // New state for multiplayer
const int STATE_PAUSED = 5;
const int STATE_START_MENU = 6;
const int STATE_SIGNUP = 7;
const int STATE_LEADERBOARD = 8;               // Added from secondary
const int STATE_MATCH_SELECTION = 9;          // Added from secondary
const int STATE_OFFLINE_MULTIPLAYER_SCREEN = 10; // Added from secondary
const int STATE_OFFLINE_MULTIPLAYER = 11;     // Added from secondary
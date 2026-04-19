#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "Enemy.h"
#include "Global.h"
#include "Player.h"
#include "Text.h"
#include "Game.h"         // Include MainMenu, PauseMenu, MatchSelectionScreen, LeaderboardScreen classes
#include "PlayerData.h"   // Include PlayerData for saving/loading players
#include "HashTable.h"
#include "FriendSystem.h" // Include FriendSystem for friend management

using namespace std;
using namespace sf;

// Global score texts
Text currentScoreText;
Text highScoreText;
Text powerupText; // Added to display powerup count

// Function to initialize score texts
bool initializeScoreTexts(Font& font) {
    // Initialize Current Score Text
    currentScoreText.setFont(font);
    currentScoreText.setCharacterSize(24);
    currentScoreText.setFillColor(Color::White);
    currentScoreText.setPosition(10, 10); // Top-left corner

    // Initialize High Score Text
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(Color::White);
    highScoreText.setPosition(10, 40); // Below Current Score

    // Initialize Powerup Text (positioned at bottom-left)
    powerupText.setFont(font);
    powerupText.setCharacterSize(24);
    powerupText.setFillColor(Color::White);
    powerupText.setPosition(10, 70);

    return true;
}

// Function to handle tile dropping
void drop(int y, int x)
{
    if (y < 0 || y >= M || x < 0 || x >= N)
        return;

    if (grid[y][x] == 0)
        grid[y][x] = -1;

    if (y > 0 && grid[y - 1][x] == 0)
        drop(y - 1, x);
    if (y < M - 1 && grid[y + 1][x] == 0)
        drop(y + 1, x);
    if (x > 0 && grid[y][x - 1] == 0)
        drop(y, x - 1);
    if (x < N - 1 && grid[y][x + 1] == 0)
        drop(y, x + 1);
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    int level = 1;
    int totalTiles = M * N;
    int filledTiles = 0;
    const int offsetX = (1920 - N * ts) / 2;
    const int offsetY = (1080 - M * ts) / 2;

    // Initialize the window in Fullscreen mode
    RenderWindow window(VideoMode(1920, 1080), "! XONIX !", Style::Fullscreen);
    window.setFramerateLimit(60);

    // Load fonts
    Font font2;
    if (!font2.loadFromFile("fonts/joystix.otf"))
    {
        cout << "Error loading joystix.otf" << endl;
        return -1;
    }
    Font font3;
    if (!font3.loadFromFile("fonts/Rovelink.otf"))
    {
        cout << "Error loading Rovelink.otf" << endl;
        return -1;
    }
    Font font4;
    if (!font4.loadFromFile("fonts/Noxis.otf"))
    {
        cout << "Error loading Noxis.otf" << endl;
        return -1;
    }
    Font font5;
    if (!font5.loadFromFile("fonts/Saphira.otf")) {
        cout << "Error loading Saphira.otf" << endl;
    }

    // Initialize Score Texts using font3 (for consistency with level and percentage texts)
    if (!initializeScoreTexts(font4)) {
        cout << "Error initializing score texts." << endl;
        return -1;
    }

    // Load textures with error checks
    Texture t1, t2, t3, t4;
    if (!t1.loadFromFile("images/tilesssss.png"))
    {
        cout << "Error loading tilesssss.png" << endl;
        return -1;
    }
    if (!t2.loadFromFile("images/gameover.png"))
    {
        cout << "Error loading gameover.png" << endl;
        return -1;
    }
    if (!t3.loadFromFile("images/enemy.png"))
    {
        cout << "Error loading enemy.png" << endl;
        return -1;
    }
    if (!t4.loadFromFile("images/youwin(2).png"))
    {
        cout << "Error loading youwin(2).png" << endl;
        return -1;
    }
    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sYouWin(t4);
    sGameover.setPosition(100, 100);
    sYouWin.setPosition(140, 60);
    sEnemy.setOrigin(20, 20);

    // Initialize texts (Assuming loadFontAndSetupText sets up other necessary SFML Text objects like levelCompleteText, levelText, etc.)
    if (!loadFontAndSetupText())
        return -1;

    // Use dynamic allocation for enemies (needed for powerup methods such as setSpeed and resetSpeed)
    Enemy* a = new Enemy[10];  // Array for up to 10 enemies

    int enemyCount = 2;
    int Game = 1;
    int x = 0, y = 0, dx = 0, dy = 0;
    float timer = 0, delay = 0.07f;
    Clock clock;

    // Powerup variables
    bool powerupActive = false;
    float powerupTimer = 0.0f;
    const float powerupDuration = 3.0f; // Powerup lasts for 3 seconds

    // Initialize PlayerData with filename and load players into a PlayerArray (primary code uses PlayerArray)
    PlayerData playerData("PlayerData.csv");
    cout << "\nOOOKKAAAYYY\n";
    PlayerArray* playerList = new PlayerArray(playerData.loadPlayers());
    cout << "\nDDDDOOONNNEE\n";

    // Current player pointer
    Player* currentPlayer = nullptr;

    // Define initial state
    // The following state values are assumed defined in Global.h; additional ones are:
    // STATE_MATCH_SELECTION, STATE_OFFLINE_MULTIPLAYER_SCREEN, STATE_LEADERBOARD
    int currentState = STATE_MAIN_SCREEN;

    // Instantiate Screens and Menus (including additional screens from second file)
    MainScreen mainScreen(window);
    LoginScreen loginScreen(window);
    MainMenu mainMenu(window, font3, font4);
    StartMenu startMenu(window, font3, font4, font5);
    SignUpScreen signUpScreen(window, font4, font3, font5);
    MatchSelectionScreen matchSelectionScreen(window, font4, font4); // Added match selection screen
    PauseMenu pauseMenu(window);
    LeaderboardScreen leaderboardScreen(window);               // Added leaderboard screen

    // Instantiate friend system and initialize hash table (from first file)
    HashTable hashTable;
    hashTable.setPlayerArray(playerList->getPlayers(), playerList->getSize());
    cout << "\nsize: " << playerList->getSize() << "\n";
    for (int i = 0; i < playerList->getSize(); ++i) {
        cout << "Adding: " << playerList->getPlayers()[i].getPlayerID() << " " << playerList->getPlayers()[i].getUsername() << endl;
        hashTable.insert(playerList->getPlayers()[i].getPlayerID(), i);
    }
    // Border setup for the grid
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    // Optional background for menu (from second file)
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("images/background_menu2.jpg")) {
        cout << "Error loading background_menu2.jpg" << endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);

    // ----------------------- MAIN GAME LOOP -----------------------
    while (window.isOpen())
    {
        switch (currentState)
        {
        case STATE_MAIN_SCREEN: {
            // Display opening screen and then proceed to start menu 
            if (mainScreen.display()) {
                currentState = STATE_START_MENU;
            }
            break;
        }
        case STATE_LOGIN: {
            // Display login screen; note: in this merged version, upon successful login we transition to match selection
            if (loginScreen.display(playerList)) {
                string username = loginScreen.getUsername();
                string password = loginScreen.getPassword();
                cout << "Attempting to log in as: " << username << endl;
                Player* foundPlayer = nullptr;
                for (int i = 0; i < playerList->getSize(); ++i) {
                    if (playerList->getPlayers()[i].getUsername() == username) {
                        if (playerList->getPlayers()[i].getPassword() == password) {
                            foundPlayer = &playerList->getPlayers()[i];
                            break;
                        }
                        else {
                            cout << "Incorrect password for username: " << username << endl;
                            break;
                        }
                    }
                }
                if (foundPlayer) {
                    currentPlayer = foundPlayer;
                    cout << "**Logged in as**: " << currentPlayer->getUsername() << endl;
                    // Transition to match selection instead of main menu so player can choose mode
                    currentState = STATE_MATCH_SELECTION;
                }
                else {
                    cout << "Login failed. Please try again or sign up." << endl;
                    currentState = STATE_LOGIN;
                }
            }
            break;
        }
        case STATE_SIGNUP: {
            if (signUpScreen.display(*playerList)) {
                string newUsername = signUpScreen.getUsername();
                string newPassword = signUpScreen.getPassword();
                string nickname = signUpScreen.getNickname();
                string profilePic = "default.png";

                bool exists = false;
                for (int i = 0; i < playerList->getSize(); ++i) {
                    if (playerList->getPlayers()[i].getUsername() == newUsername) {
                        exists = true;
                        break;
                    }
                }
                if (exists) {
                    cout << "Username already exists. Please choose a different username." << endl;
                    currentState = STATE_SIGNUP;
                }
                else {
                    int newPlayerID = 1;
                    for (int i = 0; i < playerList->getSize(); ++i) {
                        if (playerList->getPlayers()[i].getPlayerID() >= newPlayerID)
                            newPlayerID = playerList->getPlayers()[i].getPlayerID() + 1;
                    }
                    Player newPlayer(newPlayerID, newUsername, newPassword, nickname, profilePic);
                    playerList->addPlayer(newPlayer);
                    currentPlayer = &playerList->getPlayers()[playerList->getSize() - 1];
                    cout << "Signed up with Username: " << currentPlayer->getUsername() << endl;
                    if (!playerData.savePlayers(*playerList)) {
                        cout << "Error saving player data." << endl;
                    }
                    currentState = STATE_MATCH_SELECTION;
                }
            }
            break;
        }
        case STATE_START_MENU: {
            int startMenuSelection = startMenu.display();
            if (startMenuSelection == -1 || startMenuSelection == 3) {
                window.close();
                return 0;
            }
            else if (startMenuSelection == 1) {
                currentState = STATE_LOGIN;
            }
            else if (startMenuSelection == 2) {
                currentState = STATE_SIGNUP;
            }
            else if (startMenuSelection == 0) {
                // Directly start game in single-player mode if desired
                currentState = STATE_PLAYING_SINGLE;
            }
            break;
        }
        case STATE_MATCH_SELECTION: {
            // Display match selection screen to choose game mode
            int selection = matchSelectionScreen.display();
            if (selection == 1) { // Offline Multiplayer selected
                // (Example initialization for offline multiplayer)
                Player* player1 = new Player(1, "Player1", "password1", "P1Nick");
                Player* player2 = new Player(2, "Player2", "password2", "P2Nick");
                int enemyCountLocal = 2 + level;
                if (enemyCountLocal > 10) enemyCountLocal = 10;
                Enemy* enemyArray = new Enemy[enemyCountLocal];
                for (int i = 0; i < enemyCountLocal; ++i) {
                    enemyArray[i] = Enemy();
                }
                delete player1;
                delete player2;
                delete[] enemyArray;
                // Transition as needed; here we simply return to main menu
                currentState = STATE_MAIN_MENU;
            }
            else if (selection == 2) { // Multiplayer (future implementation)
                currentState = STATE_OFFLINE_MULTIPLAYER_SCREEN;
            }
            else if (selection == -1) {
                window.close();
                return 0;
            }
            break;
        }
        case STATE_OFFLINE_MULTIPLAYER_SCREEN: {
            // Placeholder for offline multiplayer screen (can be expanded)
            currentState = STATE_MATCH_SELECTION;
            break;
        }
        case STATE_LEADERBOARD: {
            leaderboardScreen.display();
            currentState = STATE_MAIN_MENU;
            break;
        }
        case STATE_MAIN_MENU: {
            mainMenu.display(*currentPlayer);
            int mainMenuSelection = mainMenu.getSelectedOption();
            if (mainMenuSelection == -1 || mainMenuSelection == 3) {
                window.close();
                return 0;
            }
            else if (mainMenuSelection == 1) {
                // Leaderboard functionality
                currentState = STATE_LEADERBOARD;
            }
            else if (mainMenuSelection == 2) {
                cout << "Settings selected. Feature not implemented." << endl;
            }
            else if (mainMenuSelection == 0) {
                currentState = STATE_PLAYING_SINGLE;
            }
            else if (mainMenuSelection == 4) { // Friends/Friend Requests
                RenderWindow friendsWindow(VideoMode(600, 500), "Friends & Requests", Style::Titlebar | Style::Close);
                cout << "\n\nBEFORE\n";
                currentPlayer->displayFriends();
                cout << "\n\nTABLE\n";
                hashTable.display();
                displayFriendSystem(friendsWindow, currentPlayer->getFriendList(), currentPlayer->getFriendRequestList(), currentPlayer->getPlayerID(), hashTable);
                if (!playerData.savePlayers(*playerList)) {
                    cerr << "Failed to save player data after adding a friend." << endl;
                }
                cout << "\n\nAFTER\n";
                currentPlayer->displayFriends();
            }
            else if (mainMenuSelection == 5) { // Log Out
                currentState = STATE_LOGIN;
            }
            break;
        }
        case STATE_PLAYING_SINGLE: {
            // Single player gameplay state; includes powerup handling
            int tilesInRow = 0;
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    window.close();

                if (e.type == Event::KeyPressed)
                {
                    if (e.key.code == Keyboard::Escape)
                    {
                        currentState = STATE_PAUSED;
                    }
                    if (e.key.code == Keyboard::L)
                    {
                        currentState = STATE_LEADERBOARD;
                    }
                    if (e.key.code == Keyboard::R)
                    {
                        if (currentPlayer != nullptr) {
                            currentPlayer->finalizeGame();
                        }
                        for (int i = 1; i < M - 1; i++)
                            for (int j = 1; j < N - 1; j++)
                                grid[i][j] = 0;
                        x = 10; y = 0;
                        dx = dy = 0;
                        enemyCount = min(2 + level, 10);
                        delay = max(0.03f, 0.07f - 0.01f * level);
                        Game = 1;
                    }
                    // Powerup activation using the 'P' key (added feature)
                    if (e.key.code == Keyboard::P) {
                        if (currentPlayer != nullptr && currentPlayer->getPowerUpCount() > 0 && !powerupActive) {
                            if (currentPlayer->usePowerUp()) {
                                powerupActive = true;
                                powerupTimer = 0.0f;
                                powerupText.setString("Your Powerups: " + to_string(currentPlayer->getPowerUpCount()));
                                // Freeze enemies
                                for (int i = 0; i < enemyCount; i++) {
                                    a[i].setSpeed(0.0f);
                                }
                                cout << "Powerup activated! Enemies are frozen for 3 seconds." << endl;
                            }
                        }
                        else if (currentPlayer != nullptr && currentPlayer->getPowerUpCount() <= 0) {
                            cout << "No powerups available!" << endl;
                        }
                        else if (powerupActive) {
                            cout << "Powerup is already active!" << endl;
                        }
                        else {
                            cout << "No player is currently logged in!" << endl;
                        }
                    }
                }
            }

            // Handle continuous movement key presses
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; }

            if (Game != 1)
                continue;

            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            // Update powerup timer (if active)
            if (powerupActive) {
                powerupTimer += time;
                float remainingTime = powerupDuration - powerupTimer;
                if (remainingTime > 0)
                    powerupText.setString("Freezing Enemies: " + to_string(static_cast<int>(remainingTime)) + "s");
                else {
                    powerupActive = false;
                    for (int i = 0; i < enemyCount; i++) {
                        a[i].resetSpeed();
                    }
                    if (currentPlayer != nullptr)
                        powerupText.setString("Your Powerups: " + to_string(currentPlayer->getPowerUpCount()));
                    cout << "Powerup effect ended. Enemies are unfrozen." << endl;
                }
            }
            else {
                if (currentPlayer != nullptr)
                    powerupText.setString("Your Powerups: " + to_string(currentPlayer->getPowerUpCount()));
            }

            if (timer > delay)
            {
                x += dx;
                y += dy;
                if (x < 0) x = 0;
                if (x > N - 1) x = N - 1;
                if (y < 0) y = 0;
                if (y > M - 1) y = M - 1;

                if (grid[y][x] == 2)
                    Game = 2;
                if (grid[y][x] == 0)
                    grid[y][x] = 2;
                timer = 0;
            }

            for (int i = 0; i < enemyCount; i++)
                a[i].move();

            if (grid[y][x] == 1)
            {
                dx = dy = 0;
                for (int i = 0; i < enemyCount; i++)
                    drop(a[i].getY() / ts, a[i].getX() / ts);

                tilesInRow = 0;
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == -1)
                            grid[i][j] = 0;
                        else {
                            if (grid[i][j] == 0 || grid[i][j] == 2)
                            {
                                grid[i][j] = 1;
                                tilesInRow++;
                            }
                        }
                if (tilesInRow > 0 && currentPlayer != nullptr) {
                    currentPlayer->tilesCaptured(tilesInRow);
                    currentScoreText.setString("Current Score: " + to_string(currentPlayer->getCurrentScore()));
                    if (currentPlayer->getHighScore() < currentPlayer->getCurrentScore()) {
                        currentPlayer->setHighScore(currentPlayer->getCurrentScore());
                        highScoreText.setString("High Score: " + to_string(currentPlayer->getHighScore()));
                        cout << "New high score for " << currentPlayer->getUsername() << ": " << currentPlayer->getHighScore() << endl;
                    }
                    if (!playerData.savePlayers(*playerList)) {
                        cout << "Error saving player data." << endl;
                    }
                }
            }

            for (int i = 0; i < enemyCount; i++)
                if (grid[a[i].getY() / ts][a[i].getX() / ts] == 2)
                    Game = 2;

            int count = 0;
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 1)
                        count++;
            filledTiles = count;

            float levelThreshold = 0.45f + 0.1f * (level * 0.5f);
            if (filledTiles >= (totalTiles * levelThreshold))
            {
                cout << "Level " << level << " complete!" << endl;
                levelCompleteText.setString("Level " + to_string(level) + " Complete!");

                Clock animClock;
                float animDuration = 3.0f; // 3 seconds
                int currentRow = M - 2;
                float rowDelay = 0.03f;
                Clock rowTimer;
                while (currentRow >= 1)
                {
                    if (rowTimer.getElapsedTime().asSeconds() >= rowDelay)
                    {
                        rowTimer.restart();
                        window.clear();
                        for (int i = 0; i < M; i++)
                        {
                            for (int j = 0; j < N; j++)
                            {
                                if (i == 0 || i == M - 1 || j == 0 || j == N - 1)
                                    sTile.setTextureRect(IntRect(0, 18, ts, ts));
                                else if (i >= currentRow)
                                {
                                    if (grid[i][j] == 1 || grid[i][j] == -1)
                                        sTile.setTextureRect(IntRect(0, 18, ts, ts));
                                }
                                else
                                    continue;
                                sTile.setPosition(offsetX + (j * ts), offsetY + (i * ts));
                                window.draw(sTile);
                            }
                        }
                        window.draw(levelCompleteText);
                        window.display();
                        currentRow--;
                    }
                    if (animClock.getElapsedTime().asSeconds() >= animDuration)
                        break;
                }
                sleep(seconds(1));
                level++;
                window.clear();
                levelCompleteText.setPosition(820, 500);
                levelCompleteText.setString("Level " + to_string(level));
                window.draw(levelCompleteText);
                window.display();
                sleep(seconds(2));
                for (int i = 1; i < M - 1; i++)
                    for (int j = 1; j < N - 1; j++)
                        grid[i][j] = 0;
                x = 10; y = 0;
                dx = dy = 0;
                enemyCount = min(2 + level, 10);
                delay = max(0.03f, 0.07f - 0.01f * level);
                Game = 1;
                if (currentPlayer != nullptr) {
                    currentPlayer->tilesCaptured(tilesInRow);
                    currentPlayer->addWin();
                    currentScoreText.setString("Current Score: " + to_string(currentPlayer->getCurrentScore()));
                    if (currentPlayer->getHighScore() < currentPlayer->getCurrentScore()) {
                        currentPlayer->setHighScore(currentPlayer->getCurrentScore());
                        highScoreText.setString("High Score: " + to_string(currentPlayer->getHighScore()));
                        cout << "New high score for " << currentPlayer->getUsername() << ": " << currentPlayer->getHighScore() << endl;
                    }
                    if (!playerData.savePlayers(*playerList)) {
                        cout << "Error saving player data." << endl;
                    }
                }
            }

            if (currentPlayer != nullptr) {
                currentScoreText.setString("Current Score: " + to_string(currentPlayer->getCurrentScore()));
                highScoreText.setString("High Score: " + to_string(currentPlayer->getHighScore()));
            }

            window.clear();
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                {
                    if (grid[i][j] == 0)
                        continue;
                    if (grid[i][j] == 1)
                        sTile.setTextureRect(IntRect(36, 0, ts, ts));
                    if (grid[i][j] == 2)
                        sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    sTile.setPosition(offsetX + (j * ts), offsetY + (i * ts));
                    window.draw(sTile);
                }

            sTile.setTextureRect(IntRect(0, 18, ts, ts));
            sTile.setPosition(offsetX + (x * ts), offsetY + (y * ts));
            window.draw(sTile);

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(offsetX + a[i].getX(), offsetY + a[i].getY());
                window.draw(sEnemy);
            }

            levelText.setString("Level: " + to_string(level));
            window.draw(levelText);
            levelLeft.setString(to_string(int(((filledTiles) / (float)totalTiles) * 100)) + "%");
            window.draw(levelLeft);
            levelTotal.setString("   " + to_string(int(levelThreshold * 100)));
            window.draw(levelTotal);

            if (currentPlayer != nullptr) {
                window.draw(currentScoreText);
                window.draw(highScoreText);
                window.draw(powerupText);
            }

            if (Game == 2)
            {
                if (currentPlayer != nullptr) {
                    currentPlayer->finalizeGame();
                    currentPlayer->addLoss();
                    if (!playerData.savePlayers(*playerList)) {
                        cout << "Error saving player data." << endl;
                    }
                }
                window.draw(sGameover);
            }
            else if (Game == 3)
            {
                window.draw(sYouWin);
            }
            window.display();
            break;
        }
        case STATE_PAUSED: {
            pauseMenu.display();
            int pauseMenuSelection = pauseMenu.getSelectedOption();
            if (pauseMenuSelection == -1)
            {
                window.close();
                delete[] a;
                return 0;
            }
            else if (pauseMenuSelection == 0)
            {
                currentState = STATE_PLAYING_SINGLE;
            }
            else if (pauseMenuSelection == 1)
            {
                String controlsText = "Controls:\n\n"
                    "Arrow Keys - Move\n"
                    "R - Reset Game\n"
                    "L - View Leaderboard\n"
                    "P - Activate Powerup\n"
                    "Escape - Pause/Resume Game\n";
                Text controlsDisplay;
                controlsDisplay.setFont(font3);
                controlsDisplay.setCharacterSize(30);
                controlsDisplay.setFillColor(Color::White);
                controlsDisplay.setString(controlsText);
                controlsDisplay.setPosition(600, 400);
                bool viewingControls = true;
                Clock controlClock;
                while (viewingControls && window.isOpen())
                {
                    Event controlEvent;
                    while (window.pollEvent(controlEvent))
                    {
                        if (controlEvent.type == Event::Closed)
                            window.close();
                        if (controlEvent.type == Event::KeyPressed) {
                            if (controlEvent.key.code == Keyboard::Escape)
                                viewingControls = false;
                        }
                    }
                    window.clear();
                    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
                    overlay.setFillColor(Color(0, 0, 0, 150));
                    window.draw(overlay);
                    window.draw(controlsDisplay);
                    window.display();
                }
            }
            else if (pauseMenuSelection == 2)
            {
                currentState = STATE_MAIN_MENU;
            }
            break;
        }
        default:
            break;
        } // end switch
    } // end while window.isOpen()

    if (!playerData.savePlayers(*playerList)) {
        cout << "Error saving player data." << endl;
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nfilled tiles: " << filledTiles;
    cout << "\ntotal tiles: " << totalTiles;

    delete[] a; // Cleanup allocated memory
    return 0;
}

void showOpeningScreen(RenderWindow& window, Font& font, Font& creditsFont)
{
    Text title("XONIX", font, 170);
    Text credits(" By Talha\nand Hassan", creditsFont, 40);
    Color neonBlue(0, 170, 255); // Darker cyan-like neon

    title.setFillColor(Color::Transparent);
    credits.setFillColor(Color::Transparent);
    title.setOutlineColor(Color::White);
    credits.setOutlineColor(Color::White);
    title.setOutlineThickness(0.4f);
    credits.setOutlineThickness(0.7f);

    FloatRect titleBounds = title.getLocalBounds();
    FloatRect creditsBounds = credits.getLocalBounds();
    Vector2f center(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    title.setPosition(380, 355 + 120);
    title.setRotation(355);
    credits.setPosition(380 + 730, 355 + 160);

    Clock clock;
    float opacity = 0;
    while (opacity < 255)
    {
        float dt = clock.restart().asSeconds();
        opacity += 100 * dt;
        if (opacity > 255) opacity = 255;
        Color fadeColor(neonBlue.r, neonBlue.g, neonBlue.b, static_cast<Uint8>(opacity));
        title.setFillColor(fadeColor);
        window.clear(Color::Black);
        window.draw(title);
        window.display();
    }
    sleep(seconds(0.5f));
    opacity = 0;
    while (opacity < 255)
    {
        float dt = clock.restart().asSeconds();
        opacity += 80 * dt;
        if (opacity > 255) opacity = 255;
        Color fadeColor(neonBlue.r, neonBlue.g, neonBlue.b, static_cast<Uint8>(opacity));
        credits.setFillColor(fadeColor);
        window.clear(Color::Black);
        window.draw(title);
        window.draw(credits);
        window.display();
    }
    opacity = 255;
    while (opacity > 0)
    {
        float dt = clock.restart().asSeconds();
        opacity -= 80 * dt;
        if (opacity < 0) opacity = 0;
        Color fadeColor(neonBlue.r, neonBlue.g, neonBlue.b, static_cast<Uint8>(opacity));
        title.setFillColor(fadeColor);
        credits.setFillColor(fadeColor);
        window.clear(Color::Black);
        window.draw(title);
        window.draw(credits);
        window.display();
    }
}
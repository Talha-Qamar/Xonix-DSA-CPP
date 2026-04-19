// Game.h
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "PlayerData.h" // Added to access PlayerData

using namespace std;
using namespace sf;

// MainScreen Class
class MainScreen {
private:
    RenderWindow& window;
    Font font;
    Font font2;
    Text xonix;
    Text playNowBtn;

    Texture backgroundTexture;
    Sprite backgroundSprite;

    Texture buttonTexture;
    Sprite playButtonSprite;

public:
    MainScreen(RenderWindow& win) : window(win) {

        font.loadFromFile("fonts/Rovelink.otf");
        font2.loadFromFile("fonts/Noxis.otf");

        // Load background texture
        if (!backgroundTexture.loadFromFile("images/background_menu2.jpg"))
        {
            cout << "Error loading background image" << endl;
        }

        backgroundSprite.setTexture(backgroundTexture);

        xonix.setFont(font);
        xonix.setString("XONIX");
        xonix.setCharacterSize(170);
        xonix.setFillColor(Color::White);
        xonix.setPosition(380, 475);
        xonix.setRotation(355);

        // Load Button Texture
        if (!buttonTexture.loadFromFile("images/buttons.png"))
        {
            cout << "Error loading buttons.png" << endl;
        }
        playButtonSprite.setTexture(buttonTexture);
        playButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
        playButtonSprite.setPosition(1110, 495);
        playButtonSprite.setScale(1.2f, 1.2f);

        playNowBtn.setFont(font2);
        playNowBtn.setString("Play Now!");
        playNowBtn.setCharacterSize(55);

        FloatRect textBounds = playNowBtn.getLocalBounds();
        playNowBtn.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        playNowBtn.setPosition(
            playButtonSprite.getPosition().x + playButtonSprite.getLocalBounds().width / 2.0f + 30,
            playButtonSprite.getPosition().y + playButtonSprite.getLocalBounds().height / 2.0f + 12
        ); // Adjust Y offset as needed
    }

    bool display() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed &&
                    event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(Mouse::getPosition(window));
                    if (playButtonSprite.getGlobalBounds().contains(mousePos)) {
                        return true; // Go to login screen
                    }
                }
            }

            // Hover effect (optional)
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (playButtonSprite.getGlobalBounds().contains(mousePos)) {
                // Optionally, change the appearance on hover
                playButtonSprite.setTextureRect(IntRect(0, 130, 300, 110)); // Lighten button
            }
            else {
                playButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
            }

            window.clear(Color::Black);

            window.draw(backgroundSprite);
            window.draw(xonix);
            window.draw(playButtonSprite);
            window.draw(playNowBtn);
            window.display();
        }
        return false;
    }
};


// LoginScreen Class
class LoginScreen {
private:
    RenderWindow& window;
    Font font;
    Font font2;
    Font font3;
    Text xonix;
    Text usernameLabel, passwordLabel;
    Text usernameInputText, passwordInputText;
    Text errorText; // Added for displaying error messages
    string usernameInput;
    string passwordInput;

    // Updated Play Now Button Components
    Text LoginBtn;
    Texture LoginButtonTexture;
    Sprite LoginButtonSprite;

    Texture backgroundTexture;
    Sprite backgroundSprite;

    // Focus flags
    bool isUsernameActive;
    bool isPasswordActive;

    // Visual indicators for active fields
    RectangleShape usernameBox;
    RectangleShape passwordBox;

public:
    LoginScreen(RenderWindow& win) : window(win), isUsernameActive(false), isPasswordActive(false) {
        // Load Fonts
        if (!font.loadFromFile("fonts/Noxis.otf")) {
            cout << "Error loading Noxis.otf" << endl;
        }

        if (!font2.loadFromFile("fonts/Rovelink.otf")) {
            cout << "Error loading Rovelink.otf" << endl;
        }
        if (!font3.loadFromFile("fonts/Saphira.otf")) {
            cout << "Error loading joystix.ttf" << endl;
        }
        // Load background texture
        if (!backgroundTexture.loadFromFile("images/background_menu2.jpg")) {
            cout << "Error loading background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Setup XONIX Text
        xonix.setFont(font2);
        xonix.setString("XONIX");
        xonix.setCharacterSize(170);
        xonix.setFillColor(Color::White);
        xonix.setPosition(380, 475);
        xonix.setRotation(355);

        // Setup Username Label
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(40);
        usernameLabel.setFillColor(Color::White);
        usernameLabel.setPosition(window.getSize().x / 2.0f + 140, 447);

        // Setup Password Label
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(40);
        passwordLabel.setFillColor(Color::White);
        passwordLabel.setPosition(window.getSize().x / 2.0f + 140, 581);

        // Setup Username Input Text
        usernameInputText.setFont(font3);
        usernameInputText.setCharacterSize(25);
        usernameInputText.setFillColor(Color::White);
        usernameInputText.setPosition(window.getSize().x / 2.0f + 150, 510);

        // Setup Password Input Text
        passwordInputText.setFont(font3);
        passwordInputText.setCharacterSize(30);
        passwordInputText.setFillColor(Color::White);
        passwordInputText.setPosition(window.getSize().x / 2.0f + 150, 645);

        // Setup Error Text
        errorText.setFont(font3);
        errorText.setCharacterSize(25);
        errorText.setFillColor(Color::Red);
        errorText.setPosition(window.getSize().x / 2.0f + 140, 700);
        errorText.setString(""); // Initialize with empty string

        // Setup input boxes
        usernameBox.setSize(Vector2f(400, 50));
        usernameBox.setPosition(window.getSize().x / 2.0f + 135, 500);
        usernameBox.setFillColor(Color::Transparent);
        usernameBox.setOutlineThickness(2);
        usernameBox.setOutlineColor(Color::White);

        passwordBox.setSize(Vector2f(400, 50));
        passwordBox.setPosition(window.getSize().x / 2.0f + 135, 630);
        passwordBox.setFillColor(Color::Transparent);
        passwordBox.setOutlineThickness(2);
        passwordBox.setOutlineColor(Color::White);

        // Load Play Now button texture
        if (!LoginButtonTexture.loadFromFile("images/buttons.png")) {
            cout << "Error loading buttons.png" << endl;
        }

        // Setup Play Now Button Sprite
        LoginButtonSprite.setTexture(LoginButtonTexture);
        LoginButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
        LoginButtonSprite.setPosition(1175, 750);
        LoginButtonSprite.setScale(0.8f, 0.8f);

        // Setup Play Now Button Text
        LoginBtn.setFont(font);
        LoginBtn.setString("Login");
        LoginBtn.setCharacterSize(40);
        LoginBtn.setFillColor(Color::White);

        FloatRect playNowTextBounds = LoginBtn.getLocalBounds();
        LoginBtn.setOrigin(playNowTextBounds.left + playNowTextBounds.width / 2.0f,
            playNowTextBounds.top + playNowTextBounds.height / 2.0f);
        LoginBtn.setPosition(
            LoginButtonSprite.getPosition().x + LoginButtonSprite.getGlobalBounds().width / 2.0f,
            LoginButtonSprite.getPosition().y + LoginButtonSprite.getGlobalBounds().height / 2.0f
        );
    }

    bool display() {
        // Clear previous inputs and error messages
        usernameInput.clear();
        passwordInput.clear();
        usernameInputText.setString("");
        passwordInputText.setString("");
        errorText.setString("");

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    // Check if username box is clicked
                    if (usernameBox.getGlobalBounds().contains(mousePos)) {
                        isUsernameActive = true;
                        isPasswordActive = false;
                        usernameBox.setOutlineColor(Color::Yellow);
                        passwordBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                    // Check if password box is clicked
                    else if (passwordBox.getGlobalBounds().contains(mousePos)) {
                        isUsernameActive = false;
                        isPasswordActive = true;
                        passwordBox.setOutlineColor(Color::Yellow);
                        usernameBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                    // Check if Login button is clicked
                    else if (LoginButtonSprite.getGlobalBounds().contains(mousePos)) {
                        // Attempt to submit if all fields are filled and valid
                        if (!usernameInput.empty() && !passwordInput.empty()) {
                            bool valid = true;
                            string errorMessage = "";

                            // Username validation: at least 3 characters, alphanumeric
                            if (usernameInput.length() < 3) {
                                errorMessage += "Username must be at least 3 characters long.\n";
                                valid = false;
                            }
                            else {
                                for (char c : usernameInput) {
                                    if (!isalnum(c) && c != '_') { // Allow underscore
                                        errorMessage += "Username can only contain alphanumeric characters and '_'.\n";
                                        valid = false;
                                        break;
                                    }
                                }
                            }

                            // Password validation: at least 8 characters, contains at least one number
                            if (passwordInput.length() < 8) {
                                errorMessage += "Password must be at least 8 characters long.\n";
                                valid = false;
                            }
                            else {
                                bool hasNumber = false;
                                for (char c : passwordInput) {
                                    if (isdigit(c)) {
                                        hasNumber = true;
                                        break;
                                    }
                                }
                                if (!hasNumber) {
                                    errorMessage += "Password must contain at least one number.\n";
                                    valid = false;
                                }
                            }

                            if (valid) {
                                // Authentication logic
                                PlayerData playerData("PlayerData.csv");
                                PlayerList players = playerData.loadPlayers();

                                bool userFound = false;

                                PlayerNode* current = players.getHead();
                                while (current != nullptr) {
                                    if (current->player.getUsername() == usernameInput &&
                                        current->player.getPassword() == passwordInput) {
                                        userFound = true;
                                        break;
                                    }
                                    current = current->next;
                                }

                                if (userFound) {
                                    // Successful login
                                    cout << "Successfully logged in as " << usernameInput << endl;
                                    return true;
                                }
                                else {
                                    // Set error message
                                    errorText.setString("Login failed: incorrect username or password.");
                                }
                            }
                            else {
                                // Display input validation error messages
                                errorText.setString(errorMessage);
                            }
                        }
                        else {
                            errorText.setString("Please enter both username and password.");
                        }
                    }
                    else {
                        // Clicked outside input fields and button
                        isUsernameActive = false;
                        isPasswordActive = false;
                        usernameBox.setOutlineColor(Color::White);
                        passwordBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                }

                if (event.type == Event::TextEntered) {
                    if (event.text.unicode == '\b') { // Backspace
                        if (isUsernameActive && !usernameInput.empty()) {
                            usernameInput.pop_back();
                            usernameInputText.setString(usernameInput);
                        }
                        if (isPasswordActive && !passwordInput.empty()) {
                            passwordInput.pop_back();
                            // Mask password input
                            passwordInputText.setString(string(passwordInput.size(), '*'));
                        }
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                        // Optionally handle Enter key for submission
                    }
                    else if (event.text.unicode < 128) { // Printable characters
                        char enteredChar = static_cast<char>(event.text.unicode);
                        if (isUsernameActive) {
                            if (isalnum(enteredChar) || enteredChar == '_') { // Allow alphanumerics and underscore
                                usernameInput += enteredChar;
                                usernameInputText.setString(usernameInput);
                            }
                        }
                        if (isPasswordActive) {
                            passwordInput += enteredChar;
                            // Mask password input
                            passwordInputText.setString(string(passwordInput.size(), '*'));
                        }
                    }
                }
            }

            // Handle hover effect for Login button
            Vector2f currentMousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (LoginButtonSprite.getGlobalBounds().contains(currentMousePos)) {
                LoginButtonSprite.setTextureRect(IntRect(0, 130, 300, 110)); // Hovered state
            }
            else {
                LoginButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
            }

            window.clear(Color::Black);
            window.draw(backgroundSprite);
            window.draw(xonix);
            window.draw(usernameLabel);
            window.draw(passwordLabel);
            window.draw(usernameBox);
            window.draw(passwordBox);
            window.draw(usernameInputText);
            window.draw(passwordInputText);
            window.draw(errorText); // Draw error messages
            // Draw Login button
            window.draw(LoginButtonSprite);
            window.draw(LoginBtn);
            window.display();
        }
        return false;
    }

    string getUsername() const {
        return usernameInput;
    }

    string getPassword() const {
        return passwordInput;
    }
};

// StartMenu Class
class StartMenu {
private:
    RenderWindow& window;
    Font& sfont1;
    Font& sfont2;
    Font& sfont3;
    Text xonix;
    Text loginBtn, signUpBtn, exitBtn;

    Texture backgroundTexture;
    Sprite backgroundSprite;

    Texture buttonTexture;
    Sprite loginButtonSprite, signUpButtonSprite, exitButtonSprite;

public:
    StartMenu(RenderWindow& win, Font& fnt, Font& fnt2, Font& fnt3)
        : window(win), sfont1(fnt), sfont2(fnt2), sfont3(fnt3) {

        if (!backgroundTexture.loadFromFile("images/background_menu2.jpg")) {
            cout << "Error loading background image for StartMenu" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        xonix.setFont(sfont1);
        xonix.setString("XONIX");
        xonix.setCharacterSize(170);
        xonix.setFillColor(Color::White);
        xonix.setPosition(380, 475);
        xonix.setRotation(355);

        if (!buttonTexture.loadFromFile("images/buttons.png")) {
            cout << "Error loading buttons.png for StartMenu" << endl;
        }

        // Login button setup
        loginButtonSprite.setTexture(buttonTexture);
        loginButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
        loginButtonSprite.setPosition(1110, 395);
        //loginButtonSprite.setScale(1.2f, 1.2f);

        loginBtn.setFont(sfont2);
        loginBtn.setString("Login");
        loginBtn.setCharacterSize(40);
        loginBtn.setFillColor(Color::White);
        FloatRect loginTextBounds = loginBtn.getLocalBounds();
        loginBtn.setOrigin(loginTextBounds.left + loginTextBounds.width / 2.0f,
            loginTextBounds.top + loginTextBounds.height / 2.0f);
        loginBtn.setPosition(
            loginButtonSprite.getPosition().x + loginButtonSprite.getGlobalBounds().width / 2.0f,
            loginButtonSprite.getPosition().y + loginButtonSprite.getGlobalBounds().height / 2.0f
        );

        // Sign Up button setup
        signUpButtonSprite.setTexture(buttonTexture);
        signUpButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
        signUpButtonSprite.setPosition(1110, 535);
        //signUpButtonSprite.setScale(1.2f, 1.2f);

        signUpBtn.setFont(sfont2);
        signUpBtn.setString("Sign Up");
        signUpBtn.setCharacterSize(40);
        signUpBtn.setFillColor(Color::White);
        FloatRect signUpTextBounds = signUpBtn.getLocalBounds();
        signUpBtn.setOrigin(signUpTextBounds.left + signUpTextBounds.width / 2.0f,
            signUpTextBounds.top + signUpTextBounds.height / 2.0f);
        signUpBtn.setPosition(
            signUpButtonSprite.getPosition().x + signUpButtonSprite.getGlobalBounds().width / 2.0f,
            signUpButtonSprite.getPosition().y + signUpButtonSprite.getGlobalBounds().height / 2.0f
        );

        // Exit button setup
        exitButtonSprite.setTexture(buttonTexture);
        exitButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
        exitButtonSprite.setPosition(1110, 675);
        //exitButtonSprite.setScale(1.2f, 1.2f);

        exitBtn.setFont(sfont2);
        exitBtn.setString("Exit");
        exitBtn.setCharacterSize(40);
        exitBtn.setFillColor(Color::White);
        FloatRect exitTextBounds = exitBtn.getLocalBounds();
        exitBtn.setOrigin(exitTextBounds.left + exitTextBounds.width / 2.0f,
            exitTextBounds.top + exitTextBounds.height / 2.0f);
        exitBtn.setPosition(
            exitButtonSprite.getPosition().x + exitButtonSprite.getGlobalBounds().width / 2.0f,
            exitButtonSprite.getPosition().y + exitButtonSprite.getGlobalBounds().height / 2.0f
        );
    }

    // Display the start menu and return the user's choice
    int display() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

                    if (loginButtonSprite.getGlobalBounds().contains(mousePos)) {
                        return 1;
                    }
                    else if (signUpButtonSprite.getGlobalBounds().contains(mousePos)) {
                        return 2;
                    }
                    else if (exitButtonSprite.getGlobalBounds().contains(mousePos)) {
                        return -1;
                    }
                }
            }

            // Hover effect (optional)
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (loginButtonSprite.getGlobalBounds().contains(mousePos)) {
                loginButtonSprite.setTextureRect(IntRect(0, 130, 300, 110));
            }
            else {
                loginButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
            }

            if (signUpButtonSprite.getGlobalBounds().contains(mousePos)) {
                signUpButtonSprite.setTextureRect(IntRect(0, 130, 300, 110));
            }
            else {
                signUpButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
            }

            if (exitButtonSprite.getGlobalBounds().contains(mousePos)) {
                exitButtonSprite.setTextureRect(IntRect(0, 130, 300, 110));
            }
            else {
                exitButtonSprite.setTextureRect(IntRect(0, 0, 300, 110));
            }

            // Draw
            window.clear();
            window.draw(backgroundSprite);
            window.draw(xonix);
            window.draw(loginButtonSprite);
            window.draw(signUpButtonSprite);
            window.draw(exitButtonSprite);
            window.draw(loginBtn);
            window.draw(signUpBtn);
            window.draw(exitBtn);
            window.display();
        }

        return 0; // fallback in case window is closed
    }
};

// SignUpScreen Class
class SignUpScreen {
private:
    RenderWindow& window;
    Font& font;
    Font& font2;
    Font& font3;
    Text xonix;
    Text usernameLabel, passwordLabel, nicknameLabel;
    Text usernameInputText, passwordInputText, nicknameInputText;
    Text errorText; // Added for displaying error messages
    string usernameInput;
    string passwordInput;
    string nicknameInput;
    Text signUpBtn;

    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture buttonTexture;
    Sprite SignUpButtonSprite;

    // Focus flags
    bool isUsernameActive;
    bool isPasswordActive;
    bool isNicknameActive;

    // Visual indicators for active fields
    RectangleShape usernameBox;
    RectangleShape passwordBox;
    RectangleShape nicknameBox;

public:
    SignUpScreen(RenderWindow& win, Font& fnt, Font& fnt2, Font& fnt3) : window(win), font(fnt), font2(fnt2), font3(fnt3),
        isUsernameActive(false), isPasswordActive(false), isNicknameActive(false) {
        // Load background texture
        if (!backgroundTexture.loadFromFile("images/background_menu2.jpg"))
        {
            cout << "Error loading background image for SignUpScreen" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Setup XONIX Text
        xonix.setFont(font2);
        xonix.setString("XONIX");
        xonix.setCharacterSize(170);
        xonix.setFillColor(Color::White);
        xonix.setPosition(380, 475);
        xonix.setRotation(355);

        // Setup Username Label
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(40);
        usernameLabel.setFillColor(Color::White);
        usernameLabel.setPosition(window.getSize().x / 2.0f + 140, 395); // Adjust Y position

        // Setup Password Label
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(40);
        passwordLabel.setFillColor(Color::White);
        passwordLabel.setPosition(window.getSize().x / 2.0f + 140, 510); // Adjust Y position

        // Setup Nickname Label
        nicknameLabel.setFont(font);
        nicknameLabel.setString("Nickname:");
        nicknameLabel.setCharacterSize(40);
        nicknameLabel.setFillColor(Color::White);
        nicknameLabel.setPosition(window.getSize().x / 2.0f + 140, 625); // Adjust Y position

        // Setup Username Input Text
        usernameInputText.setFont(font3);
        usernameInputText.setCharacterSize(25);
        usernameInputText.setFillColor(Color::White);
        usernameInputText.setPosition(window.getSize().x / 2.0f + 150, 460); // Positioned below Username label

        // Setup Password Input Text
        passwordInputText.setFont(font3);
        passwordInputText.setCharacterSize(25);
        passwordInputText.setFillColor(Color::White);
        passwordInputText.setPosition(window.getSize().x / 2.0f + 150, 577); // Positioned below Password label

        // Setup Nickname Input Text
        nicknameInputText.setFont(font3);
        nicknameInputText.setCharacterSize(25);
        nicknameInputText.setFillColor(Color::White);
        nicknameInputText.setPosition(window.getSize().x / 2.0f + 150, 687); // Positioned below Nickname label

        // Setup Error Text
        errorText.setFont(font3);
        errorText.setCharacterSize(20);
        errorText.setFillColor(Color::Red);
        errorText.setPosition(window.getSize().x / 2.0f + 140, 750);
        errorText.setString(""); // Initialize with empty string

        // Setup input boxes
        usernameBox.setSize(Vector2f(400, 50));
        usernameBox.setPosition(window.getSize().x / 2.0f + 135, 447);
        usernameBox.setFillColor(Color::Transparent);
        usernameBox.setOutlineThickness(2);
        usernameBox.setOutlineColor(Color::White);

        passwordBox.setSize(Vector2f(400, 50));
        passwordBox.setPosition(window.getSize().x / 2.0f + 135, 565);
        passwordBox.setFillColor(Color::Transparent);
        passwordBox.setOutlineThickness(2);
        passwordBox.setOutlineColor(Color::White);

        nicknameBox.setSize(Vector2f(400, 50));
        nicknameBox.setPosition(window.getSize().x / 2.0f + 135, 675);
        nicknameBox.setFillColor(Color::Transparent);
        nicknameBox.setOutlineThickness(2);
        nicknameBox.setOutlineColor(Color::White);

        if (!buttonTexture.loadFromFile("images/buttons.png"))
        {
            cout << "Error loading buttons.png" << endl;
        }
        SignUpButtonSprite.setTexture(buttonTexture);
        SignUpButtonSprite.setTextureRect(IntRect(0, 0, 300, 200));
        SignUpButtonSprite.setPosition(1175, 850);
        SignUpButtonSprite.setScale(0.8f, 0.8f);

        signUpBtn.setFont(font);
        signUpBtn.setString("Sign Up");
        signUpBtn.setCharacterSize(40);

        FloatRect textBounds = signUpBtn.getLocalBounds();
        signUpBtn.setOrigin(textBounds.left + textBounds.width / 2.0f + 30, textBounds.top + textBounds.height / 2.0f);
        signUpBtn.setPosition(
            SignUpButtonSprite.getPosition().x + SignUpButtonSprite.getGlobalBounds().width / 2.0f + 30,
            SignUpButtonSprite.getPosition().y + SignUpButtonSprite.getGlobalBounds().height / 2.0f - 35
        ); // Adjust Y offset as needed
    }

    bool display() {
        // Clear previous inputs and error messages
        usernameInput.clear();
        passwordInput.clear();
        nicknameInput.clear();
        usernameInputText.setString("");
        passwordInputText.setString("");
        nicknameInputText.setString("");
        errorText.setString("");

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    // Check if username box is clicked
                    if (usernameBox.getGlobalBounds().contains(mousePos)) {
                        isUsernameActive = true;
                        isPasswordActive = false;
                        isNicknameActive = false;
                        usernameBox.setOutlineColor(Color::Yellow);
                        passwordBox.setOutlineColor(Color::White);
                        nicknameBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                    // Check if password box is clicked
                    else if (passwordBox.getGlobalBounds().contains(mousePos)) {
                        isUsernameActive = false;
                        isPasswordActive = true;
                        isNicknameActive = false;
                        passwordBox.setOutlineColor(Color::Yellow);
                        usernameBox.setOutlineColor(Color::White);
                        nicknameBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                    // Check if nickname box is clicked
                    else if (nicknameBox.getGlobalBounds().contains(mousePos)) {
                        isUsernameActive = false;
                        isPasswordActive = false;
                        isNicknameActive = true;
                        nicknameBox.setOutlineColor(Color::Yellow);
                        usernameBox.setOutlineColor(Color::White);
                        passwordBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                    // Check if Sign Up button is clicked
                    else if (SignUpButtonSprite.getGlobalBounds().contains(mousePos)) {
                        // Attempt to submit if all fields are filled and valid
                        if (!usernameInput.empty() && !passwordInput.empty() && !nicknameInput.empty()) {
                            bool valid = true;
                            string errorMessage = "";

                            // Username validation: at least 3 characters, alphanumeric and underscore
                            if (usernameInput.length() < 3) {
                                errorMessage += "Username must be at least 3 characters long.\n";
                                valid = false;
                            }
                            else {
                                for (char c : usernameInput) {
                                    if (!isalnum(c) && c != '_') {
                                        errorMessage += "Username can only contain alphanumeric characters and '_'.\n";
                                        valid = false;
                                        break;
                                    }
                                }
                            }

                            // Password validation: at least 8 characters, contains at least one number
                            if (passwordInput.length() < 8) {
                                errorMessage += "Password must be at least 8 characters long.\n";
                                valid = false;
                            }
                            else {
                                bool hasNumber = false;
                                for (char c : passwordInput) {
                                    if (isdigit(c)) {
                                        hasNumber = true;
                                        break;
                                    }
                                }
                                if (!hasNumber) {
                                    errorMessage += "Password must contain at least one number.\n";
                                    valid = false;
                                }
                            }

                            // Nickname validation: non-empty, no spaces
                            if (nicknameInput.find(' ') != string::npos) {
                                errorMessage += "Nickname cannot contain spaces.\n";
                                valid = false;
                            }

                            if (valid) {
                                // Successful sign up
                                cout << "User " << usernameInput << " signed up successfully with nickname " << nicknameInput << endl;
                                return true; // Move to the next page
                            }
                            else {
                                // Display error messages on screen
                                errorText.setString(errorMessage);
                            }
                        }
                        else {
                            errorText.setString("Please fill in all fields.");
                        }
                    }
                    else {
                        // Clicked outside input fields and button
                        isUsernameActive = false;
                        isPasswordActive = false;
                        isNicknameActive = false;
                        usernameBox.setOutlineColor(Color::White);
                        passwordBox.setOutlineColor(Color::White);
                        nicknameBox.setOutlineColor(Color::White);
                        errorText.setString(""); // Clear error message
                    }
                }

                if (event.type == Event::TextEntered) {
                    if (event.text.unicode == '\b') { // Backspace
                        if (isUsernameActive && !usernameInput.empty()) {
                            usernameInput.pop_back();
                            usernameInputText.setString(usernameInput);
                        }
                        if (isPasswordActive && !passwordInput.empty()) {
                            passwordInput.pop_back();
                            // Mask password input
                            passwordInputText.setString(string(passwordInput.size(), '*'));
                        }
                        if (isNicknameActive && !nicknameInput.empty()) {
                            nicknameInput.pop_back();
                            nicknameInputText.setString(nicknameInput);
                        }
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                        // Optionally handle Enter key for submission
                    }
                    else if (event.text.unicode < 128) { // Printable characters
                        char enteredChar = static_cast<char>(event.text.unicode);
                        if (isUsernameActive) {
                            if (isalnum(enteredChar) || enteredChar == '_') { // Allow alphanumerics and underscore
                                usernameInput += enteredChar;
                                usernameInputText.setString(usernameInput);
                            }
                        }
                        if (isPasswordActive) {
                            passwordInput += enteredChar;
                            // Mask password input
                            passwordInputText.setString(string(passwordInput.size(), '*'));
                        }
                        if (isNicknameActive) {
                            if (enteredChar != ' ') { // Disallow spaces in nickname
                                nicknameInput += enteredChar;
                                nicknameInputText.setString(nicknameInput);
                            }
                        }
                    }
                }
            }

            // Handle hover effect for Sign Up button
            Vector2f currentMousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (SignUpButtonSprite.getGlobalBounds().contains(currentMousePos)) {
                SignUpButtonSprite.setTextureRect(IntRect(0, 130, 300, 110)); // Hovered state
            }
            else {
                SignUpButtonSprite.setTextureRect(IntRect(0, 0, 300, 110)); // Normal state
            }

            window.clear(Color::Black);
            window.draw(backgroundSprite);
            window.draw(xonix);
            window.draw(usernameLabel);
            window.draw(passwordLabel);
            window.draw(nicknameLabel);
            window.draw(usernameBox);
            window.draw(passwordBox);
            window.draw(nicknameBox);
            window.draw(usernameInputText);
            window.draw(passwordInputText);
            window.draw(nicknameInputText);
            window.draw(errorText); // Draw error messages
            // Draw Sign Up button below the nickname box
            window.draw(SignUpButtonSprite);
            window.draw(signUpBtn);
            window.display();
        }
        return false;
    }

    string getUsername() const {
        return usernameInput;
    }

    string getPassword() const {
        return passwordInput;
    }

    string getNickname() const {
        return nicknameInput;
    }
};

// MainMenu Class
class MainMenu
{
private:
    RenderWindow& window;
    Texture backgroundTexture;
    Sprite backgroundSprite;

    Texture menuOptionsTexture;      // Texture containing all main menu option images
    Sprite menuOptionSprites[4];     // Sprites for each main menu option

    // Separate IntRects for normal and hovered states for each main menu option
    IntRect menuOptionRectsNormal[4];
    IntRect menuOptionRectsHovered[4];

    Font font, font2;
    Text menuText[4];
    Text xonix;

    int selectedOption;

public:
    MainMenu(RenderWindow& win, Font& fnt, Font& fnt2)
        : window(win), font(fnt), font2(fnt2) {

        if (!backgroundTexture.loadFromFile("images/background_menu2.jpg")) {
            cout << "Error loading background image for StartMenu" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        xonix.setFont(font);
        xonix.setString("XONIX");
        xonix.setCharacterSize(170);
        xonix.setFillColor(Color::White);
        xonix.setPosition(380, 475);
        xonix.setRotation(355);

        // Initialize text
        menuText[0].setFont(font2);
        menuText[0].setString("Start");
        menuText[0].setCharacterSize(40);
        menuText[0].setFillColor(Color::White);
        menuText[0].setPosition(window.getSize().x / 2.0f + 140 + 88, 385);

        menuText[1].setFont(font2);
        menuText[1].setString("Leaderboard");
        menuText[1].setCharacterSize(38);
        menuText[1].setFillColor(Color::White);
        menuText[1].setPosition(window.getSize().x / 2.0f + 140 + 25, 495);

        menuText[2].setFont(font2);
        menuText[2].setString("Inventory");
        menuText[2].setCharacterSize(40);
        menuText[2].setFillColor(Color::White);
        menuText[2].setPosition(window.getSize().x / 2.0f + 140 + 42, 605);

        menuText[3].setFont(font2);
        menuText[3].setString("Exit");
        menuText[3].setCharacterSize(40);
        menuText[3].setFillColor(Color::White);
        menuText[3].setPosition(window.getSize().x / 2.0f + 140 + 98, 715);

        // Load menu options texture
        if (!menuOptionsTexture.loadFromFile("images/buttons.png"))
        {
            cout << "Error loading menu options texture" << endl;
        }

        // Define IntRects for each main menu option's normal and hovered states
        // Menu Option 0: Start Game
        menuOptionRectsNormal[0] = IntRect(0, 0, 300, 110);          // Start_Normal
        menuOptionRectsHovered[0] = IntRect(0, 130, 300, 110);       // Start_Hovered
        menuOptionSprites[0].setTexture(menuOptionsTexture);
        menuOptionSprites[0].setTextureRect(menuOptionRectsNormal[0]);
        menuOptionSprites[0].setPosition(window.getSize().x / 2.0f + 140, 355);

        // Menu Option 1: Leaderboard
        menuOptionRectsNormal[1] = IntRect(0, 0, 300, 110);         // Leaderboard_Normal
        menuOptionRectsHovered[1] = IntRect(0, 130, 300, 110);     // Leaderboard_Hovered
        menuOptionSprites[1].setTexture(menuOptionsTexture);
        menuOptionSprites[1].setTextureRect(menuOptionRectsNormal[1]);
        menuOptionSprites[1].setPosition(window.getSize().x / 2.0f + 140, 465);

        // Menu Option 2: Inventory
        menuOptionRectsNormal[2] = IntRect(0, 0, 300, 110);         // Inventory_Normal
        menuOptionRectsHovered[2] = IntRect(0, 130, 300, 110);     // Inventory_Hovered
        menuOptionSprites[2].setTexture(menuOptionsTexture);
        menuOptionSprites[2].setTextureRect(menuOptionRectsNormal[2]);
        menuOptionSprites[2].setPosition(window.getSize().x / 2.0f + 140, 575);

        // Menu Option 3: Exit
        menuOptionRectsNormal[3] = IntRect(0, 0, 300, 110);         // Exit_Normal
        menuOptionRectsHovered[3] = IntRect(0, 130, 300, 110);     // Exit_Hovered
        menuOptionSprites[3].setTexture(menuOptionsTexture);
        menuOptionSprites[3].setTextureRect(menuOptionRectsNormal[3]);
        menuOptionSprites[3].setPosition(window.getSize().x / 2.0f + 140, 685);

        selectedOption = -1;
    }

    void display()
    {
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                    for (int i = 0; i < 4; ++i)
                    {
                        FloatRect bounds = menuOptionSprites[i].getGlobalBounds();
                        if (bounds.contains(mousePos))
                        {
                            selectedOption = i;
                            return; // Exit the menu loop
                        }
                    }
                }
            }

            // Highlight main menu option if mouse is over it
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

            for (int i = 0; i < 4; ++i)
            {
                FloatRect bounds = menuOptionSprites[i].getGlobalBounds();
                if (bounds.contains(mousePos))
                {
                    menuOptionSprites[i].setTextureRect(menuOptionRectsHovered[i]);
                }
                else
                {
                    menuOptionSprites[i].setTextureRect(menuOptionRectsNormal[i]);
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(xonix);

            for (int i = 0; i < 4; ++i)
            {
                window.draw(menuOptionSprites[i]);
                window.draw(menuText[i]);
            }

            window.display();
        }

        return; // Ensure function returns even if window is closed
    }

    int getSelectedOption()
    {
        return selectedOption;
    }
};

// PauseMenu Class
class PauseMenu
{
    RenderWindow& window;
    Texture backgroundTexture;
    Sprite backgroundSprite;

    Texture pauseOptionsTexture;      // Texture containing all pause menu option images
    Sprite pauseOptionSprites[3];     // Sprites for each pause menu option

    // Separate IntRects for normal and hovered states for each pause menu option
    IntRect pauseOptionRectsNormal[3];
    IntRect pauseOptionRectsHovered[3];

    Font font, font2;
    Text menuText[3];
    Text xonix;

    int selectedOption;

public:
    PauseMenu(RenderWindow& win) : window(win), selectedOption(-1)
    {
        // Load background texture
        if (!backgroundTexture.loadFromFile("images/background_menu2.jpg"))
        {
            cout << "Error loading background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Load fonts
        if (!font.loadFromFile("fonts/Noxis.otf")) {
            cout << "Font loading failed!" << std::endl;
        }
        if (!font2.loadFromFile("fonts/Rovelink.otf")) {
            cout << "Font loading failed!" << std::endl;
        }

        xonix.setFont(font2);
        xonix.setString("XONIX");
        xonix.setCharacterSize(170);
        xonix.setFillColor(Color::White);
        xonix.setPosition(380, 475);
        xonix.setRotation(355);

        // Initialize text
        menuText[0].setFont(font);
        menuText[0].setString("Resume");
        menuText[0].setCharacterSize(40);
        menuText[0].setFillColor(Color::White);
        menuText[0].setPosition(window.getSize().x / 2.0f + 140 + 78, 405);

        menuText[1].setFont(font);
        menuText[1].setString("Controls");
        menuText[1].setCharacterSize(38);
        menuText[1].setFillColor(Color::White);
        menuText[1].setPosition(window.getSize().x / 2.0f + 140 + 60, 515);

        menuText[2].setFont(font);
        menuText[2].setString("Quit");
        menuText[2].setCharacterSize(40);
        menuText[2].setFillColor(Color::White);
        menuText[2].setPosition(window.getSize().x / 2.0f + 140 + 105, 625);

        // Load pause options texture
        if (!pauseOptionsTexture.loadFromFile("images/buttons.png"))
        {
            cout << "Error loading pause options texture" << endl;
        }

        // Define IntRects for each pause menu option's normal and hovered states
        // Menu Option 0: Resume
        pauseOptionRectsNormal[0] = IntRect(0, 0, 300, 110);          // Resume_Normal
        pauseOptionRectsHovered[0] = IntRect(0, 130, 300, 110);       // Resume_Hovered
        pauseOptionSprites[0].setTexture(pauseOptionsTexture);
        pauseOptionSprites[0].setTextureRect(pauseOptionRectsNormal[0]);
        pauseOptionSprites[0].setPosition(window.getSize().x / 2.0f + 140, 375);

        // Menu Option 1: Controls
        pauseOptionRectsNormal[1] = IntRect(0, 0, 300, 110);         // Controls_Normal
        pauseOptionRectsHovered[1] = IntRect(0, 130, 300, 110);     // Controls_Hovered
        pauseOptionSprites[1].setTexture(pauseOptionsTexture);
        pauseOptionSprites[1].setTextureRect(pauseOptionRectsNormal[1]);
        pauseOptionSprites[1].setPosition(window.getSize().x / 2.0f + 140, 485);

        // Menu Option 2: Quit
        pauseOptionRectsNormal[2] = IntRect(0, 0, 300, 110);         // Quit_Normal
        pauseOptionRectsHovered[2] = IntRect(0, 130, 300, 110);     // Quit_Hovered
        pauseOptionSprites[2].setTexture(pauseOptionsTexture);
        pauseOptionSprites[2].setTextureRect(pauseOptionRectsNormal[2]);
        pauseOptionSprites[2].setPosition(window.getSize().x / 2.0f + 140, 595);
    }

    void display()
    {
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                        for (int i = 0; i < 3; ++i)
                        {
                            FloatRect bounds = pauseOptionSprites[i].getGlobalBounds();
                            if (bounds.contains(mousePos))
                            {
                                selectedOption = i;
                                return; // Exit the menu loop
                            }
                        }
                    }
                }
            }

            // Highlight pause menu option if mouse is over it
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

            for (int i = 0; i < 3; ++i)
            {
                FloatRect bounds = pauseOptionSprites[i].getGlobalBounds();
                if (bounds.contains(mousePos))
                {
                    pauseOptionSprites[i].setTextureRect(pauseOptionRectsHovered[i]);
                }
                else
                {
                    pauseOptionSprites[i].setTextureRect(pauseOptionRectsNormal[i]);
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(xonix);

            for (int i = 0; i < 3; ++i)
            {
                window.draw(pauseOptionSprites[i]);
                window.draw(menuText[i]);
            }

            window.display();
        }
    }

    int getSelectedOption()
    {
        return selectedOption;
    }
};
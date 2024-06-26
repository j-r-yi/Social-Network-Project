#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"

#include <string>
#include <set>
#include <vector>

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);


    char userFile[]  = "/Users/joshuayi/Desktop/csci62/HW8/HW8/users.txt";
    char postFile[] = "/Users/joshuayi/Desktop/csci62/HW8/HW8/posts.txt";
    char passwordFile[] = "/Users/joshuayi/Desktop/csci62/HW8/HW8/passwords.txt";
    network_ = Network();
    network_.readUsers(userFile);
    network_.readPosts(postFile);
    network_.readPasswords(passwordFile);

    loggedInUser_ = nullptr;
    displayedUser_ = nullptr;


    ui->usernameLabel->setVisible(false);
    ui->recentPostsLabel->setVisible(false);
    ui->addFriendButton->setVisible(false);
    ui->suggestFriendsTable->setVisible(false);
    ui->recentPostsLabel->setVisible(false);
    ui->backButton->setVisible(false);
    ui->friendsTable->setVisible(false);
    ui->backToLogInButton->setVisible(false);
    ui->signUpButton->setVisible(false);
    ui->logoutButton->setVisible(false);
    ui->searchBarButton->setVisible(false);
    ui->searchBarLabel->setVisible(false);
    ui->searchBarTextEdit->setVisible(false);
    ui->friendsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->suggestFriendsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->passwordSignUpTextEdit->setVisible(false);
    ui->passwordSignUpLabel->setVisible(false);
    ui->searchFriendLabel->setVisible(false);

    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkWindow::login);
    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::clickedFriend);
    connect(ui->backButton, &QPushButton::clicked, this, &SocialNetworkWindow::back);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::addFriend);
    connect(ui->suggestFriendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::clickedSuggestedFriend);
    connect(ui->toSignUpButton, &QPushButton::clicked, this, &SocialNetworkWindow::toSignUp);
    connect(ui->backToLogInButton, &QPushButton::clicked, this, &SocialNetworkWindow::backToLogIn);
    connect(ui->signUpButton, &QPushButton::clicked, this, &SocialNetworkWindow::signUp);
    connect(ui->logoutButton, &QPushButton::clicked, this, &SocialNetworkWindow::logout);
    connect(ui->searchBarButton, &QPushButton::clicked, this, &SocialNetworkWindow::initializeNewUserPage);
    connect(ui->passwordSignUpTextEdit, &QTextEdit::textChanged, this, &SocialNetworkWindow::checkPassword);
    connect(ui->searchBarTextEdit, &QTextEdit::textChanged, this, &SocialNetworkWindow::searchFriend);
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}


void SocialNetworkWindow::login(){
    std::string userName = ui->loginTextEdit->toPlainText().toStdString();
    int userId = network_.getId(userName);
    User * user1 = network_.getUser(userId);
    if(userId == -1){
        ui->loginLabel->setText(QString::fromStdString("User Invalid! Enter a new name."));
    }
    else{
        std::cout << userName << std::endl;
        if(ui->passwordTextEdit->toPlainText().toStdString() == user1->getPassword()){
            loggedInUser_ = user1;
            displayedUser_ = loggedInUser_;
            std::cout<< ui->passwordTextEdit->toPlainText().toStdString() << std::endl;
            updateUI();
        }
        else{
            ui->loginPasswordLabel->setText(QString::fromStdString("Password incorrect! Try again."));
        }
    }
}

void SocialNetworkWindow::clickedFriend(int row, int col){
    QTableWidgetItem * friendClicked = ui->friendsTable->item(row, col);
    if(!friendClicked){
        return;
    }
    std::string stringName =friendClicked->text().toStdString();
    int friendID = network_.getId(stringName);
    displayedUser_ = network_.getUser(friendID);
    updateUI();
}

void SocialNetworkWindow::clickedSuggestedFriend(int row, int col){
    QTableWidgetItem * friendClicked = ui->suggestFriendsTable->item(row, col);
    if(!friendClicked){
        return;
    }
    std::string stringName =friendClicked->text().toStdString();
    int friendID = network_.getId(stringName);
    loggedInUser_->addFriend(friendID);
    network_.getUser(friendID)->addFriend(loggedInUser_->getId());
    updateUI();

    // Should also update users.txt to be saved for next use
    char userFile[]  = "/Users/joshuayi/Desktop/csci62/HW8/HW8/users.txt";
    network_.writeUsers(userFile);
}

void SocialNetworkWindow::back(){
    displayedUser_ = loggedInUser_;
    updateUI();
}


void SocialNetworkWindow::addFriend(){
    int displayedUserID = displayedUser_->getId();
    int loggedInUserID = loggedInUser_->getId();

    displayedUser_->addFriend(loggedInUserID);
    loggedInUser_->addFriend(displayedUserID);

    updateUI();

    // Should also update users.txt to be saved for next use
    char userFile[]  = "/Users/joshuayi/Desktop/csci62/HW8/HW8/users.txt";
    network_.writeUsers(userFile);
}


void SocialNetworkWindow::displayFriends(User * u){
    std::set<int> friends = u->getFriends();
    ui->friendsTable->setColumnCount(1);
    ui->friendsTable->setRowCount(friends.size());
    ui->friendsTable->setHorizontalHeaderLabels(QStringList() << "Friends: ");
    ui->friendsTable->horizontalHeader()->setStretchLastSection(true);
    int row = 0;
    for(auto f : friends){
        QString name = QString::fromStdString(network_.getUser(f)->getName());
        QTableWidgetItem * twi = new QTableWidgetItem(name);
        ui->friendsTable->setItem(row, 0, twi);
        ++row;
    }

}

void SocialNetworkWindow::displayPosts(User * u){
    bool showPublicOnly = (u == loggedInUser_ ? false : true);
    std::string posts = "Posts: \n";
    posts += u->getPostsString(5, showPublicOnly);
    ui->recentPostsLabel->setText(QString::fromStdString(posts));
}

void SocialNetworkWindow::displaySuggestFriends(int u){
    int score;
    std::vector<int> suggestedFriends = network_.suggestFriends(u,score);
    ui->suggestFriendsTable->setColumnCount(1);
    ui->suggestFriendsTable->setRowCount(suggestedFriends.size());
    ui->suggestFriendsTable->setHorizontalHeaderLabels(QStringList() << "Suggested Friends: ");
    ui->suggestFriendsTable->horizontalHeader()->setStretchLastSection(true);
    int row = 0;

    if(suggestedFriends.empty()){
        return;
    }

    for(auto f : suggestedFriends){
        QString name = QString::fromStdString(network_.getUser(f)->getName());
        QTableWidgetItem * twi = new QTableWidgetItem(name);
        ui->suggestFriendsTable->setItem(row, 0, twi);
        ++row;
    }
}

void SocialNetworkWindow::hideLogIn(){
    ui->loginLabel->setVisible(false);
    ui->loginButton->setVisible(false);
    ui->loginTextEdit->setVisible(false);
    ui->loginPasswordLabel->setVisible(false);
    ui->passwordTextEdit->setVisible(false);
    ui->toSignUpButton->setVisible(false);
    ui->loginMainLabel->setVisible(false);
    ui->backToLogInButton->setVisible(false);
    ui->signUpButton->setVisible(false);
    ui->passwordSignUpTextEdit->setVisible(false);
    ui->passwordSignUpLabel->setVisible(false);
}

void SocialNetworkWindow::updateUI(){
    // One display that controls most of the UI for displaying user page
    // If login page is visible, then hide it and bring up the user page
    hideLogIn();

    if(ui->usernameLabel->isVisible()==false){
        ui->usernameLabel->setVisible(true);
        ui->friendsTable->setVisible(true);
        ui->recentPostsLabel->setVisible(true);
        ui->suggestFriendsTable->setVisible(true);
    }

    if(loggedInUser_ == nullptr){
        ui->loginLabel->setVisible(true);
        ui->loginTextEdit->setVisible(true);
        ui->loginPasswordLabel->setVisible(true);
        ui->passwordTextEdit->setVisible(true);
        ui->loginTextEdit->clear();
        ui->passwordTextEdit->clear();
        ui->recentPostsLabel->setVisible(false);
        ui->suggestFriendsTable->setVisible(false);
        ui->friendsTable->setVisible(false);
        ui->usernameLabel->setVisible(false);
        ui->toSignUpButton->setVisible(true);
        ui->loginButton->setVisible(true);
        ui->logoutButton->setVisible(false);
        ui->loginMainLabel->setVisible(true);
    }

    // If the current displayed user is the same as the logged in user
    if(displayedUser_ != loggedInUser_){
        std::string name = displayedUser_->getName() + "'s Profile";
        ui->usernameLabel->setText(QString::fromStdString(name));
        ui->suggestFriendsTable->setVisible(false);
        ui->addFriendButton->setVisible(true);
        ui->backButton->setVisible(true);
        ui->logoutButton->setVisible(false);
    }else if(displayedUser_ == loggedInUser_ && loggedInUser_ != nullptr){
        ui->usernameLabel->setText(QString::fromStdString("My Profile"));
        ui->suggestFriendsTable->setVisible(true);
        ui->addFriendButton->setVisible(false);
        ui->backButton->setVisible(false);
        ui->logoutButton->setVisible(true);
    }

    if(displayedUser_ != nullptr){
        displayFriends(displayedUser_);
        displayPosts(displayedUser_);
        displaySuggestFriends(displayedUser_->getId());
    }

}


void SocialNetworkWindow::backToLogIn(){
    ui->loginLabel->setText(QString::fromStdString("Enter your name: "));
    ui->loginPasswordLabel->setText(QString::fromStdString("Enter your password: "));
    ui->loginPasswordLabel->setVisible(true);
    ui->backToLogInButton->setVisible(false);
    ui->signUpButton->setVisible(false);
    ui->loginButton->setVisible(true);
    ui->toSignUpButton->setVisible(true);
    ui->loginMainLabel->setText(QString::fromStdString("Login Page"));
    ui->passwordSignUpTextEdit->setVisible(false);
    ui->passwordSignUpLabel->setVisible(false);
    ui->passwordTextEdit->setVisible(true);
    ui->loginTextEdit->clear();
    ui->passwordTextEdit->clear();
}


void SocialNetworkWindow::toSignUp(){
    ui->loginLabel->setText(QString::fromStdString("Set Username: (Firstname Lastname)"));
    ui->loginButton->setVisible(false);
    ui->loginPasswordLabel->setWordWrap(true);
    ui->passwordSignUpLabel->setWordWrap(true);
    ui->passwordSignUpTextEdit->clear();
    ui->passwordSignUpLabel->setText(QString::fromStdString("Set Password *Must Include At Least 1 number, 1 capital letter, 1 lower letter, and 1 special symbol"));
    ui->toSignUpButton->setVisible(false);
    ui->backToLogInButton->setVisible(true);
    ui->signUpButton->setVisible(true);
    ui->loginMainLabel->setText(QString::fromStdString("Sign Up Page"));
    ui->passwordSignUpTextEdit->setVisible(true);
    ui->passwordSignUpLabel->setVisible(true);
    ui->passwordTextEdit->setVisible(false);
    ui->loginPasswordLabel->setVisible(false);
    ui->loginTextEdit->clear();
}


void SocialNetworkWindow::signUp(){

    // If password has at least one number, one special character, and one upper case character, we create user with password
    if(checkPassword() && !ui->loginTextEdit->toPlainText().isEmpty()){
        std::string tempPassword = ui->passwordSignUpTextEdit->toPlainText().toStdString();
        std::string userName = ui->loginTextEdit->toPlainText().toStdString();
        User * newUser = new User(network_.numUsers(), userName, -1, -1, std::set<int>{});
        std::string password_ = tempPassword;
        newUser->setPassword(password_);
        network_.addUser(newUser);

        loggedInUser_ = newUser;
        displayedUser_ = newUser;
        hideLogIn();

        char userFile[]  = "/Users/joshuayi/Desktop/csci62/HW8/HW8/users.txt";
        char passwordFile[] = "/Users/joshuayi/Desktop/csci62/HW8/HW8/passwords.txt";
        network_.writeUsers(userFile);
        network_.writePasswords(passwordFile);

        ui->searchBarButton->setVisible(true);
        ui->searchBarLabel->setVisible(true);
        ui->searchBarTextEdit->setVisible(true);
        ui->searchFriendLabel->setVisible(true);

    }
}


bool SocialNetworkWindow::checkPassword(){
    bool hasNum = false;
    bool hasSpecialChar = false;
    bool hasCapt = false;
    bool hasLower = false;

    std::string tempPassword = ui->passwordSignUpTextEdit->toPlainText().toStdString();
    int passwordSize = tempPassword.size();

    for(int i = 0; i < passwordSize; ++i){
        char tempC = tempPassword[i];
        // Check if character is lower case letter if so continue
        if(tempC >= 97 && tempC <= 122){ // Check lower case letter
            hasLower = true;
        }else if(tempC >= 48 && tempC <= 57){ // Check if number
            hasNum = true;
        }else if(tempC >= 65 && tempC <= 90){ // Check if upper case letter
            hasCapt = true;
        }else{ // If none of the above triggers, must be special character, set to true
            hasSpecialChar = true;
        }
    }

    std::string errorMessage = "";
    if(ui->loginTextEdit->toPlainText().isEmpty()){
        errorMessage += "Username must not be empty \n";
    }
    if(!hasNum){
        errorMessage += "Must have at least one number \n";
    }
    if(!hasSpecialChar){
        errorMessage += "Must have at least one special character \n";
    }
    if(!hasCapt){
        errorMessage += "Must have at least one capital letter \n";
    }
    if(!hasLower)
    {
        errorMessage += "Must have at least one lower letter \n";
    }

    ui->passwordSignUpLabel->setText(QString::fromStdString(errorMessage));

    if(errorMessage.empty()){
        return true;
    }else{
        return false;
    }
}



void SocialNetworkWindow::logout(){
    loggedInUser_ = displayedUser_ = nullptr;
    ui->loginLabel->setText(QString::fromStdString("Enter your username: "));
    ui->loginPasswordLabel->setText(QString::fromStdString("Enter your password: "));
    ui->loginMainLabel->setVisible(true);
    updateUI();
}


void SocialNetworkWindow::initializeNewUserPage(){
    std::string userName = ui->searchBarTextEdit->toPlainText().toStdString();
    int userId = network_.getId(userName);
    if(userId == -1){
        ui->searchBarLabel->setText(QString::fromStdString("User Invalid! Enter a new name."));
    }else{
        loggedInUser_->addFriend(userId);
        network_.getUser(userId)->addFriend(loggedInUser_->getId());
        updateUI();
        ui->searchBarButton->setVisible(false);
        ui->searchBarLabel->setVisible(false);
        ui->searchBarTextEdit->setVisible(false);
        ui->searchFriendLabel->setVisible(false);
    }
}



void SocialNetworkWindow::searchFriend(){
    std::string search = ui->searchBarTextEdit->toPlainText().toStdString();
    std::string result = "Are you looking for: \n";
    for(int i = 0; i<network_.numUsers(); ++i){
        if(network_.getUser(i)->getName().find(search) != std::string::npos){
            result += network_.getUser(i)->getName();
            result += "\n";
        }
    }
    ui->searchFriendLabel->setText(QString::fromStdString(result));
}

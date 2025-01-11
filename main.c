#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define USER_DATA_FILE "user_data.txt"

struct User
{
    int id;
    char cardNumber[20];
    char name[50];
    char password[50];
    char gender;
    int balance;
};


struct User user;
void displayMainMenu();
void displayUserMenu();
void header();
void clearPage();
bool doesFileExist(const char *fileName);
bool createNewFile(const char *fileName);
int generateNextUserId();
void saveNewUser(const char *fileName);
void registerNewUser(const char *fileName, const char *userName, const char *password, char gender);
bool isAccountExists(const char *fileName, const char *cardNumber);
void loginUser(const char *fileName, const char *cardNumber, const char *password);
void depositMoney();
void withdrawMoney();
void View_blance();
void check_out();

int main()
{
    displayMainMenu();
    return 0;
}

void header()
{
    printf("****************************************\n");
    printf("*              Total Bank              *\n");
    printf("****************************************\n");
}
void clearPage()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void displayMainMenu()
{
    clearPage();
    int option;
    header();
    printf("1. Create a New Account\n");
    printf("2. I Already Have an Account\n");
    printf("****************************************\n");
    printf("Select an option: ");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
    {
        char userName[50];
        char password[50];
        char gender;
        printf("Enter your username: ");
        scanf("%s", userName);
        printf("Enter your password: ");
        scanf("%s", password);
        printf("Enter your gender (f/m): ");
        scanf(" %c", &gender);
        registerNewUser(USER_DATA_FILE, userName, password, gender);
        break;
    }
    case 2:
    {
        char cardNumber[20];
        char password[50];
        printf("Enter your Account Number: ");
        scanf("%s", cardNumber);
        printf("Enter your password: ");
        scanf("%s", password);
        loginUser(USER_DATA_FILE, cardNumber, password);
        break;
    }
    default:
        printf("Invalid option selected.\n");
    }
}

void displayUserMenu()
{
    clearPage();
    int option;

    char salutation[5];

    if (user.gender == 'f')
    {
        strcpy(salutation, "Mrs.");
    }
    else if (user.gender == 'm')
    {
        strcpy(salutation, "Mr.");
    }
    else
    {
        strcpy(salutation, "User");
    }

    header();
    printf("Hello %s %s\n", salutation, user.name);
    printf("1. Deposit Money\n");
    printf("2. Withdraw Money\n");
    printf("3. View Balance\n");
    printf("4. Check out\n");
    printf("****************************************\n");
    printf("Select an option: ");

    // Read user input
    if (scanf("%d", &option) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        // Clear the input buffer
        while (getchar() != '\n')
            ;
        return;
    }

    // Handle the user's choice
    switch (option)
    {
    case 1:
        depositMoney();
        break;
    case 2:
        withdrawMoney();
        break;
    case 3:
        View_blance();
        break;
    case 4:
        check_out();
        break;
    default:
        printf("Invalid option selected.\n");
    }
}

bool doesFileExist(const char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool createNewFile(const char *fileName)
{
    FILE *newFile = fopen(fileName, "w");
    if (newFile == NULL)
    {
        printf("Error creating file: %s\n", fileName);
        return false;
    }
    fclose(newFile);
    return true;
}

int generateNextUserId()
{
    FILE *file = fopen(USER_DATA_FILE, "r");
    int lastId = 0;
    if (file)
    {
        char line[128];
        while (fgets(line, sizeof(line), file))
        {
            int userId;
            sscanf(line, "%d", &userId);
            if (userId > lastId)
            {
                lastId = userId;
            }
        }
        fclose(file);
    }
    return lastId + 1;
}

void saveNewUser(const char *fileName)
{
    if (!doesFileExist(fileName))
    {
        createNewFile(fileName);
    }
    FILE *file = fopen(fileName, "a");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", fileName);
        return;
    }

    fprintf(file, "%d %s %s %s %c %d\n", user.id, user.cardNumber, user.name, user.password, user.gender, user.balance);

    fclose(file);
}

void registerNewUser(const char *fileName, const char *userName, const char *password, char gender)
{

    static bool seeded = false;
    if (!seeded)
    {
        srand((unsigned int)time(NULL));
        seeded = true;
    }

    user.id = generateNextUserId();

    for (int i = 0, j = 0; i < 19; i++)
    {
        if (i == 4 || i == 9 || i == 14)
        {
            user.cardNumber[i] = '-';
        }
        else
        {
            user.cardNumber[i] = '0' + (rand() % 10);
            j++;
        }
    }
    user.cardNumber[19] = '\0';

    strncpy(user.name, userName, sizeof(user.name) - 1);
    user.name[sizeof(user.name) - 1] = '\0';
    strncpy(user.password, password, sizeof(user.password) - 1);
    user.password[sizeof(user.password) - 1] = '\0';
    user.gender = gender;
    user.balance = 0;
    if (!isAccountExists(fileName, user.cardNumber))
    {
        clearPage();
        char back;

        saveNewUser(fileName);
        header();
        printf("*******************************\n");
        printf("Your account number: %s\n", user.cardNumber);
        printf("Your account password: %s\n", user.password);
        printf("*******************************\n");
        printf("Back to main menu (y/n): ");

        while (getchar() != '\n')
            ;
        back = getchar();

        if (back == 'y' || back == 'Y')
        {
            displayMainMenu();
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        printf("An account with this card number already exists.\n");
    }
}

bool isAccountExists(const char *fileName, const char *cardNumber)
{
    if (doesFileExist(fileName))
    {
        FILE *file = fopen(fileName, "r");
        if (file == NULL)
        {
            printf("Error opening file: %s\n", fileName);
            return false;
        }

        char line[128];
        while (fgets(line, sizeof(line), file))
        {
            char fileCardNumber[20];
            sscanf(line, "%*d %19s", fileCardNumber);
            if (strcmp(fileCardNumber, cardNumber) == 0)
            {
                fclose(file);
                return true;
            }
        }

        fclose(file);
    }
    return false;
}

void loginUser(const char *fileName, const char *cardNumber, const char *password)
{
    if (doesFileExist(fileName))
    {
        FILE *file = fopen(fileName, "r");
        if (file == NULL)
        {
            printf("Error opening file: %s\n", fileName);
            return;
        }

        char line[128];

        char formattedAccountNumber[20];
        for (int i = 0, j = 0; i < 20; i++)
        {
            if (i == 4 || i == 9 || i == 14)
            {
                formattedAccountNumber[i] = '-';
            }
            else
            {
                formattedAccountNumber[i] = cardNumber[j++];
            }
        }
        formattedAccountNumber[19] = '\0';

        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%d %19s %49s %49s %c %d", &user.id, user.cardNumber, user.name, user.password, &user.gender, &user.balance);

            if (strcmp(user.cardNumber, formattedAccountNumber) == 0 && strcmp(user.password, password) == 0)
            {
                fclose(file);
                displayUserMenu(user);
                return;
            }
        }

        printf("Account not found or incorrect password.\n");
        fclose(file);
    }
    else
    {
        printf("File does not exist: %s\n", fileName);
    }
}

void depositMoney()
{
    clearPage();
    int amount;
    header();
    printf("Enter amount: ");
    if (scanf("%d", &amount) != 1 || amount <= 0)
    {
        printf("Invalid input. Please enter a positive number.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    FILE *file = fopen(USER_DATA_FILE, "r+");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", USER_DATA_FILE);
        return;
    }

    char line[128];
    char newContent[10000] = "";
    int userId;
    bool userFound = false;

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d %*s %*s %*s %*c %d", &userId, &user.balance);

        if (userId == user.id)
        {
            user.balance += amount;
            userFound = true;
            sprintf(line, "%d %s %s %s %c %d\n", user.id, user.cardNumber, user.name, user.password, user.gender, user.balance);
        }

        strcat(newContent, line);
    }

    if (userFound)
    {
        char back;
        freopen(USER_DATA_FILE, "w", file);
        fputs(newContent, file);
        printf("Money added successfully\n");
        printf("Back to main menu (y/n): ");

        while (getchar() != '\n')
            ;
        back = getchar();

        if (back == 'y' || back == 'Y')
        {
            displayUserMenu(user);
        }
        else
        {
            exit(0);
        }
    }
    else
    {
        printf("User not found.\n");
    }

    fclose(file);
}

void withdrawMoney()
{
    clearPage();
    header();
    int amount;
    printf("Enter amount: ");
    if (scanf("%d", &amount) != 1 || amount <= 0)
    {
        printf("Invalid input. Please enter a positive number.\n");
        while (getchar() != '\n');
        return;
    }

    if (amount > user.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    FILE *file = fopen(USER_DATA_FILE, "r+");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", USER_DATA_FILE);
        return;
    }

    char line[128];
    char newContent[10000] = "";
    int userId;
    bool userFound = false;

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d %*s %*s %*s %*c %d", &userId, &user.balance);

        if (userId == user.id)
        {
            user.balance -= amount;
            userFound = true;
            sprintf(line, "%d %s %s %s %c %d\n", user.id, user.cardNumber, user.name, user.password, user.gender, user.balance);
        }

        strcat(newContent, line);
    }

    if (userFound)
    {
        char back;
        freopen(USER_DATA_FILE, "w", file);
        fputs(newContent, file);
        printf("Money withdrawn successfully\n");
        printf("Back to main menu (y/n): ");

        while (getchar() != '\n')
            ;
        back = getchar();

        if (back == 'y' || back == 'Y')
        {
            displayUserMenu(user);
        }
        else
        {
            printf("User not found.\n");
        }

        fclose(file);
    }
}


void View_blance()
{
    clearPage();
    header();
    char back;
    printf("*******************************\n");
    printf("Your current balance is: %d\n", user.balance);
    printf("*******************************\n");
    printf("Back to main menu (y/n): ");

    while (getchar() != '\n')
        ;
    back = getchar();

    if (back == 'y' || back == 'Y')
    {
        displayUserMenu();
    }
    else
    {
        exit(0);
    }
}

void check_out() {
    memset(&user, 0, sizeof(user));

    clearPage();
    header();
    displayMainMenu();

}

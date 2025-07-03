/******************************************************************************
This is to certify that this project is my own work, based on my personal
efforts in studying and applying the concepts learned. I have constructed
the functions and their respective algorithms and corresponding code by
myself. The program was run, tested, and debugged by my own efforts. I
further certify that I have not copied in part or whole or otherwise
plagiarized the work of other students and/or persons.
                                    
                                            Isaac Nathan Roman, ID#12208397
******************************************************************************/

/*  Description: Gummiphone, a messaging system
    Programmed by: Isaac Nathan Roman, S19B
    Last modified: 
    Version: 1.0
    Acknowledgements: Sir Art for the skeleton code  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// based from program constraints
#define MAX_USERS 30
#define MAX_CHAR_MSG 1001 // +1 for null char
#define MAX_MSG 1001 // +1 for null char
#define MAX_CHAR_PASS 20  
#define MAX_CHAR_SEC 101
#define MAX_SECURITY_QUESTIONS 10

typedef char string[101];
typedef char msg[MAX_CHAR_MSG];

struct userTag
{
    int ID;
    string name;
    string username;
    string password;
    string description;
    string securityQuestion;
    string securityAnswer;
    int connectionCount;
    int connections[MAX_USERS];
    int isDeleted;
};

struct adminTag
{
    string password;
    int exists;
    int securityRequestCount;
    int securityRequests[MAX_USERS];
    int processedRequestCount;
    int processedRequests[MAX_USERS];
    int securityQuestionCount;
    string securityQuestions[MAX_SECURITY_QUESTIONS];
};

struct messageTag
{
    string subject;
    msg content;
    int senderID;
    int recieverCount; // 1 - personal, more than 1 - group, (-1) - announcement
    int recieverID[MAX_USERS];
};

typedef struct userTag user[MAX_USERS];
typedef struct adminTag administrator;
typedef struct messageTag message[MAX_MSG];

// misc. functions
void loadUsers(user users, int *userCount);
void saveUsers(user users, int userCount);
void loadAdmin(administrator *admin);
void saveAdmin(administrator admin);
void loadMessages(message msgs, int *msgCount);
void saveMessages(message msgs, int msgCount);
int controlAccess(int loggedIn, int exitApp);
void handleRequest(user users, int userIndex, administrator *admin);

// main menu functions
void mainMenu(int *option);
void userLogin(user users, int userCount, int *userIndex, int *loggedIn);
void createAccount(user users, int *userCount, administrator admin);
void refreshPasswordUser(user users, int userCount, administrator *admin);

// user menu functions
void userMenu(int *option, user users, int userIndex);
int selectUser(user users, int userCount);
void viewProfile(user users, int userIndex, int userProfileID);
void browseUsers(message msgs, user users, int userCount, int *msgCount, int userIndex);
void sendMessage(user users, int userCount, int userIndex, message msgs, int *msgCount, int recieverIDArr[], int recieverCount);
void messageModule(message msgs, user users, int userCount, int *msgCount, int userIndex);
void printMessage(message msgs, user users, int userIndex, int msgIndex, int checkingType);
int isMsgRelated(user users, message msgs, int msgIndex, int userIndex, int relationType);
void checkInbox(user users, message msgs, int *msgCount, int userIndex, int userCount);
void checkSent(user users, message msgs, int msgCount, int userIndex);
void checkAnnouncements(user users, message msgs, int *msgCount, int userIndex, int userCount);
void modifyAccount(user users, int userIndex);
void modifySecurity(user users, int userIndex);
void connectionsModule(user users, int userIndex, int userCount);
void addConnections(user users, int userIndex, int userCount);
void removeConnections(user users, int userIndex, int userCount);
void viewPersonalConnections(user users, int userIndex);
void viewUserConnections(user users, int userIndex, int userCount);

// admin menu functions
void adminMenu(int *option);
void adminLogin(administrator admin, int *loggedIn);
void usersModule(user users, message msgs, int *msgCount, int *userCount, administrator *admin);
void viewUsers(user users, int userCount);
void modifyUsers(user users, int userCount);
void securityQuestions(administrator *admin);
void adminPassModule(administrator *admin);
void refreshPasswordAdmin(user users, int userCount, administrator *admin);
void messagesModule(user users, message msgs, int *msgCount, int userCount);
void messagesAdmin(user users, message msgs, int *msgCount, int userCount, int ifDelete);
void deleteUser(message msgs, user users, int *userCount, int *msgCount);

int main(void)
{
    srand(time(NULL)); // for security question

    user users;
    administrator admin;
    message msgs;
    
    int exitApp = 0, userCount = 0, userIndex, option, loggedIn = 0, msgCount = 0;

    loadUsers(users, &userCount);
    loadAdmin(&admin);
    loadMessages(msgs, &msgCount);

    while(controlAccess(loggedIn, exitApp) != 3) // app runs til exit
    { 
        while(controlAccess(loggedIn, exitApp) == 0)
        { 
            loggedIn = 0;
            userIndex = -1;
            mainMenu(&option);

            switch(option)
            {
                case 1: userLogin(users, userCount, &userIndex, &loggedIn);
                        break;
                case 2: createAccount(users, &userCount, admin);
                        break;
                case 3: refreshPasswordUser(users, userCount, &admin);
                        break;
                case 4: adminLogin(admin, &loggedIn);
                        break;
                case 5: exitApp = 1;
                        break;
            }
        }

        while(controlAccess(loggedIn, exitApp) == 1)
        {
            userMenu(&option, users, userIndex);
            switch(option)
            {
            case 1: messageModule(msgs, users, userCount, &msgCount, userIndex);
                    break;
            case 2: checkInbox(users, msgs, &msgCount, userIndex, userCount);
                    break;
            case 3: checkSent(users, msgs, msgCount, userIndex);
                    break;
            case 4: checkAnnouncements(users, msgs, &msgCount, userIndex, userCount);
                    break;
            case 5: modifyAccount(users, userIndex);
                    break;
            case 6: modifySecurity(users, userIndex);
                    break;
            case 7: connectionsModule(users, userIndex, userCount);
                    break;
            case 8: browseUsers(msgs, users, userCount, &msgCount, userIndex);
                    break;
            case 9: loggedIn = 0;
                    break;
            }
        } 
        
    // ADMIN MENU ---------------------------------------------------
        while(controlAccess(loggedIn, exitApp) == 2)
        {
            adminMenu(&option);
            switch(option)
            {
                case 1: usersModule(users, msgs, &msgCount, &userCount, &admin);
                        break;
                case 2: messagesModule(users, msgs, &msgCount, userCount);
                        break;
                case 3: adminPassModule(&admin);
                        break; 
                case 4: loggedIn = 0;
                        break;
            }
        }
    }    

    saveUsers(users, userCount);
    saveAdmin(admin);
    saveMessages(msgs, msgCount);

    return 0;
}

/* 
DESCRIPTION:
    This function loads the users struct with values from users.txt

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param *userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void
loadUsers(user users, int *userCount)
{
    int i, j;

    FILE *pFile;
    string strLine;
    pFile = fopen("users.txt", "rt");

    if(pFile != NULL)
    {
        fgets(strLine, sizeof(strLine), pFile);
        // reads how many users in file.txt
        *userCount = atoi(strLine);

        // load user data to users[i]
        for(i = 0; i < *userCount; i++)
        {
            fgets(strLine, sizeof(strLine), pFile);
            users[i].ID = atoi(strLine);
            
            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[i].name, strLine);

            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[i].username, strLine);

            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[i].password, strLine);

            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[i].description, strLine);

            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[i].securityQuestion, strLine);

            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[i].securityAnswer, strLine);
            
            fgets(strLine, sizeof(strLine), pFile);
            users[i].connectionCount = atoi(strLine);

            for(j = 0; j < users[i].connectionCount; j++)
            {
                fgets(strLine, sizeof(strLine), pFile);
                users[i].connections[j] = atoi(strLine);
            }

            fgets(strLine, sizeof(strLine), pFile);
            users[i].isDeleted = atoi(strLine);
        }
        fclose(pFile);
    }
    else
        *userCount = 0;
    
}

/* 
DESCRIPTION:
    This function saves all of the user's values to users.txt

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything; void;
*/
void 
saveUsers(user users, int userCount)
{
    int i, j;
    FILE *pFile;

    pFile = fopen("users.txt", "wt");

    if(pFile != NULL)
    {    
        fprintf(pFile, "%d\n", userCount);

        for(i = 0; i < userCount; i++) // saves user data in users.txt
        {
            fprintf(pFile, "%d\n", users[i].ID);
            fprintf(pFile, "%s\n", users[i].name);
            fprintf(pFile, "%s\n", users[i].username);
            fprintf(pFile, "%s\n", users[i].password);
            fprintf(pFile, "%s\n", users[i].description);
            fprintf(pFile, "%s\n", users[i].securityQuestion);
            fprintf(pFile, "%s\n", users[i].securityAnswer);
            fprintf(pFile, "%d\n", users[i].connectionCount);
            for(j = 0; j < users[i].connectionCount; j++)
                fprintf(pFile, "%d\n", users[i].connections[j]);

            fprintf(pFile, "%d\n", users[i].isDeleted);
        }

        fclose(pFile);
    }
    else
        printf("Unable to generate users.txt file.\n");
}

/* 
DESCRIPTION:
    This function loads the admin struct with values from admin.txt

PRECONDITION:
    N/A

PARAMETERS:
    @param *admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void
loadAdmin(administrator *admin)
{
    int i;
    FILE *pFile;
    string strLine;
    pFile = fopen("admin.txt", "rt");

    if(pFile != NULL)
    {
        fgets(strLine, sizeof(strLine), pFile);
        strLine[strlen(strLine)-1] = '\0';
        strcpy(admin->password, strLine);

        fgets(strLine, sizeof(strLine), pFile);
        admin->exists = atoi(strLine);

        fgets(strLine, sizeof(strLine), pFile);
        admin->securityRequestCount = atoi(strLine);

        for(i = 0; i < admin->securityRequestCount; i++)
        {
            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            admin->securityRequests[i]= atoi(strLine);
        }
        
        fgets(strLine, sizeof(strLine), pFile);
        admin->processedRequestCount = atoi(strLine);
        
        for(i = 0; i < admin->processedRequestCount; i++)
        {
            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            admin->processedRequests[i]= atoi(strLine);
        }

        fgets(strLine, sizeof(strLine), pFile);
        admin->securityQuestionCount = atoi(strLine);

        for(i = 0; i < admin->securityQuestionCount; i++)
        {
            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(admin->securityQuestions[i], strLine);
        }
    }
    else if(!admin->exists)
    {
        system("cls");
        printf("\n[WELCOME TO GUMMIPHONE]\n\n");
        printf("Before the system goes online, please provide an admin password:\n\n");

        printf("Input: ");

        fflush(stdin);
        fgets(strLine, sizeof(strLine), stdin);
        strLine[strlen(strLine)-1] = '\0';
        strcpy(admin->password, strLine);
        admin->exists = 1;

        // initialize admin variables
        admin->securityQuestionCount = 1;
        strcpy(admin->securityQuestions[0], "What is your favorite color?");
        
        admin->securityRequestCount = 0;
        admin->securityRequests[0] = -1;
        admin->processedRequestCount = 0;
        admin->processedRequests[0] = -1;
    }
    
}

/* 
DESCRIPTION:
    This function saves the admin's values to admin.txt

PRECONDITION:
    N/A

PARAMETERS:
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything; void.
*/
void
saveAdmin(administrator admin)
{
    int i;
    FILE *pFile;
    pFile = fopen("admin.txt", "wt");

    if(pFile != NULL)
    {    
        fprintf(pFile, "%s\n", admin.password);
        fprintf(pFile, "%d\n", admin.exists);

        fprintf(pFile, "%d\n", admin.securityRequestCount);
        for(i = 0; i < admin.securityRequestCount; i++)
            fprintf(pFile, "%d\n", admin.securityRequests[i]);

        fprintf(pFile, "%d\n", admin.processedRequestCount);
        for(i = 0; i < admin.processedRequestCount; i++)
            fprintf(pFile, "%d\n", admin.processedRequests[i]);

        fprintf(pFile, "%d\n", admin.securityQuestionCount);
        for(i = 0; i < admin.securityQuestionCount; i++)
            fprintf(pFile, "%s\n", admin.securityQuestions[i]);

        fclose(pFile);
    }
    else
        printf("Unable to generate admin.txt file.\n");
}

/* 
DESCRIPTION:
    This function loads all the message values from  msgs.txt

PRECONDITION:
    N/A

PARAMETERS:
    @param msgs - Refers the messages struct variable.
    @param *msgCount - Refers how many messages exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void
loadMessages(message msgs, int *msgCount)
{
    int i, j;

    FILE *pFile;
    msg strMsg;
    string strLine;
    pFile = fopen("msgs.txt", "rt");

    if(pFile != NULL)
    {   
        // reads how many msgs in msgs.txt
        fgets(strLine, sizeof(strLine), pFile);
        *msgCount = atoi(strLine);

        for(i = 0; i < *msgCount; i++)
        {
            // get subject
            fgets(strLine, sizeof(strLine), pFile);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(msgs[i].subject, strLine);

            // get content
            fgets(strMsg, sizeof(strMsg), pFile);
            strMsg[strlen(strMsg)-1] = '\0';
            strcpy(msgs[i].content, strMsg);

            // get senderID
            fgets(strLine, sizeof(strLine), pFile);
            msgs[i].senderID = atoi(strLine);

            // get recieverCount
            fgets(strLine, sizeof(strLine), pFile);
            msgs[i].recieverCount = atoi(strLine);

            // get recieverID
            for(j = 0; j < msgs[i].recieverCount; j++)
            {
                fgets(strLine, sizeof(strLine), pFile);
                msgs[i].recieverID[j] = atoi(strLine);
            }
        }
        fclose(pFile);
    }
    else
        *msgCount = 0;
}

/* 
DESCRIPTION:
    This function saves all the message values to msgs.txt

PRECONDITION:
    N/A

PARAMETERS:
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.

RETURN:
    This function does not return anything; void.
*/
void
saveMessages(message msgs, int msgCount)
{
    int i, j;

    FILE *pFile;
    
    pFile = fopen("msgs.txt", "wt");

    if(pFile != NULL)
    {
        fprintf(pFile, "%d\n", msgCount); 

        for(i = 0; i < msgCount; i++) // saves messages to msgs.txt
        {
            fprintf(pFile, "%s\n", msgs[i].subject);
            fprintf(pFile, "%s\n", msgs[i].content);
            fprintf(pFile, "%d\n", msgs[i].senderID);
            fprintf(pFile, "%d\n", msgs[i].recieverCount);
            for(j = 0; j < msgs[i].recieverCount; j++)
                fprintf(pFile, "%d\n", msgs[i].recieverID[j]);            
        }
        fclose(pFile);
    }
    else
        printf("Unable to generate msgs.txt file.\n");
}

/* 
DESCRIPTION:
    This function determines which menu the user should be in.

PRECONDITION:
    Negative value parameters are not used by function.

PARAMETERS:
    @param loggedIn - Refers the login status of a user.
    @param exitApp - Determines if the program should end or not.

RETURN:
    This function does not return anything; void.
*/
int
controlAccess(int loggedIn, int exitApp)
{ 
    int accessKey;

    if(loggedIn == 0 && exitApp == 0)
        accessKey = 0; // stay in main menu
    else if(loggedIn == 1 && exitApp == 0)
        accessKey = 1; // access user menu
    else if(loggedIn == 2 && exitApp == 0)
        accessKey = 2; //access admin menu
    else if(exitApp == 1)
        accessKey = 3; // exit app

    return accessKey;
}

/* 
DESCRIPTION:
    This function determines which main menu function is executed.

PRECONDITION:
    N/A

PARAMETERS:
    @param *option - Refers to which function is executed on
RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void
mainMenu(int *option)
{
    do{ // prompt main menu modules
        system("cls");
        printf("\n[MAIN PAGE]\n\n");
        printf("[1] - User Login\n");
        printf("[2] - Create Account\n");
        printf("[3] - Forgot Password\n");
        printf("[4] - Admin Login\n");
        printf("[5] - Exit Gummiphone\n\n");

        printf("[ ]: ");
        scanf("%d", option);
        printf("\n");
    } while(*option < 1 || *option > 5);

}

/* 
DESCRIPTION:
    This function calculates which user is trying to login.

PRECONDITION:
    userIndex cannot be a negative value.

PARAMETERS:
    @param users

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
userLogin(user users, int userCount, int *userIndex, int *loggedIn)
{
    string name, password;
    int i, bFlag = 0, tempIndex;
    char choice;

    printf("\n[USER LOGIN]\n\n");

    while(!bFlag)
    {
        // prompt username & password
        printf("Account Username: ");
        fflush(stdin);
        fgets(name, sizeof(name), stdin);
        name[strlen(name)-1] = '\0';

        printf("Account Password: ");
        fflush(stdin);
        fgets(password, sizeof(password), stdin);
        password[strlen(password)-1] = '\0';

        for(i = 0; i < userCount && bFlag != 1; i++)
        {
            if(strcmp(users[i].username, name) == 0) // if user exists,
            {
                bFlag = 1;
                tempIndex = i;
            }
        }

        if(bFlag) // check password.
        {
            if(strcmp(users[tempIndex].password, password) != 0)
                bFlag = 0;
        }

        // if invalid credentials, try again or return to main
        if(!bFlag)
        {
            printf("\nInvalid Login Credentials.\n\n");
            printf("[A]gain    [R]eturn\n\n");

            fflush(stdin);
            printf("[ ]: ");
            do{
                choice = getch();
                printf("\n");

                if(choice == 'A' || choice == 'a')
                    bFlag = 0;
                else if(choice == 'R' || choice == 'r'){
                    bFlag = 1;
                    tempIndex = -1;
                }
            } while(choice != 'A' && choice != 'a' && choice != 'R' && choice != 'r');
            printf("\n");
        }
    }

    if(users[tempIndex].isDeleted == 1)
        *userIndex = -1;
    else
        *userIndex = tempIndex;
    if(*userIndex != -1)
        *loggedIn = 1;
}

/* 
DESCRIPTION:
    This function creates accounts in the system.

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
createAccount(user users, int *userCount, administrator admin)
{   
    string strLine;
    char ret;
    int i, bFlag = 0;
    
    system("cls");

    if(*userCount >= MAX_USERS)
    { // if max. users > 30
        printf("\nMaximum Users Reached.\n");

        do{
            printf("\n[R]eturn\n\n");
            printf("[ ]: ");
            ret = getch();
            printf("\n");
        }while(ret != 'r' && ret != 'R');
    }
    else if(*userCount < MAX_USERS)
    {
        printf("\n[CREATE ACCOUNT]\n\n");

        do{ // prompt name
            printf("Name: ");
            fflush(stdin);
            fgets(strLine, sizeof(strLine), stdin);
            strLine[strlen(strLine)-1] = '\0';
            strcpy(users[*userCount].name, strLine);

            if(strlen(strLine) > 100)
                printf("Maximum characters exceeded.\n\n");
        } while(strlen(strLine) > 100);

        do{
            bFlag = 0;
            do{ // prompt for unique username
                printf("Username: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strlen(strLine) > 100)
                    printf("Maximum characters exceeded.\n\n");

            } while(strlen(strLine) > 100);

            // check if username in users[] already exists
            for(i = 0; i < *userCount && bFlag != 1; i++)
            {
                if(strcmp(strLine, users[i].username) == 0 && i != *userCount)
                    bFlag = 1;
            }
            if(!bFlag)
                strcpy(users[*userCount].username, strLine);
            else
                printf("Username is already taken.\n");
        } while(bFlag);

        // get password

        do{ // prompt password
            printf("Password: ");
            fflush(stdin);
            fgets(strLine, sizeof(strLine), stdin);
            strLine[strlen(strLine)-1] = '\0';

            if(strlen(strLine) > MAX_CHAR_PASS)
                printf("Exceeds 20 characters. Enter a shorter password.\n");

        } while(strlen(strLine) > MAX_CHAR_PASS);

        strcpy(users[*userCount].password, strLine);

        do{ // checks for valid re-entering of password
            printf("Re-enter Password: ");
            fflush(stdin);
            fgets(strLine, sizeof(strLine), stdin); 
            strLine[strlen(strLine)-1] = '\0';

            if(strcmp(users[*userCount].password, strLine) != 0)
                printf("Password does not match.\n");

        } while(strcmp(users[*userCount].password, strLine) != 0);

        int random = rand() % admin.securityQuestionCount;
        // assign security question
        printf("Security Question: %s\n", admin.securityQuestions[random]);
        fflush(stdin);
        fgets(strLine, sizeof(strLine), stdin); 
        strLine[strlen(strLine)-1] = '\0';
        strcpy(users[*userCount].securityAnswer, strLine);

        // set misc. account details to default values
        strcpy(users[*userCount].description, "DEFAULT USER");
        strcpy(users[*userCount].securityQuestion, admin.securityQuestions[random]);
        users[*userCount].connectionCount = 0;
        users[*userCount].connections[0] = -1; // initalize to not get garbage values (-1 cannot be an user[i].ID)
        users[*userCount].isDeleted = 0;

        // generate userID
        users[*userCount].ID = *userCount;
        (*userCount)++;

        system("cls");
        printf("\n[ACCOUNT CREATED SUCESSFULLY!]\n\n");

        printf("[ID#%d Account Details]\n", users[(*userCount)-1].ID);
        printf("Name: %s\n", users[(*userCount)-1].name);
        printf("Username: %s\n", users[(*userCount)-1].username);
        printf("Password: %s\n", users[(*userCount)-1].password);
        printf("\n[R]eturn\n\n");
        printf("[ ]: ");

        do{
            ret = getch();
        }while(ret != 'r' && ret != 'R');
    }

}

/* 
DESCRIPTION:
    This function determines which user menu function executes.

PRECONDITION:
    User must be logged in.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param *option - Refers to which function is executed on a given menu.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
userMenu(int *option, user users, int userIndex)
{    
    do{ // prompt user modules
        system("cls");

        printf("\n[USER #%d: %s]\n\n", users[userIndex].ID, users[userIndex].username);
        printf("[1] - Compose Message\n");
        printf("[2] - Inbox\n");
        printf("[3] - Sent\n");        
        printf("[4] - View Announcements\n");
        printf("[5] - Modify Personal Contents\n");
        printf("[6] - Modify Account Security\n");
        printf("[7] - Modify Personal Connections\n");
        printf("[8] - Browse Users\n");
        printf("[9] - Log Out\n\n");

        printf("[ ]: ");
        scanf("%d", option);
        printf("\n");
    } while(*option < 1 || *option > 9);
}

/* 
DESCRIPTION:
    This function provides a list of users and provides their user ID.

PRECONDITION:
    userCount must not be 0.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
int 
selectUser(user users, int userCount)
{  
    char choice;
    int i = 0, bFlag = 0, selectedUserID;

    do{ // fetches list of users and returns selected user's ID
        system("cls");

        printf("\n[SELECT USER]\n\n");

        if(users[i].isDeleted == 0)
            printf("User #%d: %s\nName: %s\n\n", users[i].ID, users[i].username, users[i].name);
        else if(users[i].isDeleted == 1)
            printf("[DELETED USER]\n\n");
        
        printf("[P]revious  [N]ext  [S]elect\n\n");    
        printf("[ ]: ");
        choice = getch();
        printf("\n");

        if((choice == 'P' || choice == 'p') && i > 0)
            i--;
        else if((choice == 'N' || choice == 'n') && i < userCount-1)
            i++;
        else if(choice == 'S' || choice == 's'){
            if(users[i].isDeleted == 0)
            {
                selectedUserID = users[i].ID;
                bFlag = 1;
            }
            else        
            {
                printf("\n[R]eturn\n\n");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');
            }
        }
    } while(bFlag != 1);

    return selectedUserID;
}

/* 
DESCRIPTION:
    This function views the profile of a user.

PRECONDITION:
    userProfileID must not be from a deleted user.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param userProfileID - Refers to a specified user ID.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
viewProfile(user users, int userIndex, int userProfileID)
{
    int exit = 0;
    char choice;

    while(!exit)
    {
        system("cls");
        printf("\n[USER'S PROFILE]\n\n");    
        printf("User #%d: %s\n", users[userProfileID].ID, users[userProfileID].username);
        printf("Name: %s\n", users[userProfileID].name);
        printf("Description: %s\n\n", users[userProfileID].description);

        printf("[V]iew User's Connections       [R]eturn\n");    
        printf("\n[ ]: ");
        choice = getch();
        printf("\n");

        if(choice == 'V' || choice == 'v')
            viewPersonalConnections(users, userProfileID);
        else if(choice == 'R' || choice == 'r')
            exit = 1;
    }
}

/* 
DESCRIPTION:
    This function provides multiple options to interact with a user.

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
browseUsers(message msgs, user users, int userCount, int *msgCount, int userIndex)
{        
    char choice;
    int i = 0, j = 0, bFlag = 0, cFlag = 0, dFlag = 0, recieverIDArr[MAX_USERS];
    string strLine;

    while(bFlag != 1) 
    {
        system("cls");

        // prints user details
        printf("\n[USER LIST]\n\n");
        if(users[i].isDeleted == 0)
        {
            printf("User #%d: %s\n", users[i].ID, users[i].username);
            printf("Name: %s\n\n", users[i].name);
        }
        else
            printf("[DELETED USER]\n\n");

        // prompt choice
        printf("[P]revious   [N]ext    [M]essage    [A]dd     [V]iew     [F]ilter    [R]eturn\n");    
        printf("\n[ ]: ");
        choice = getch();
        printf("\n");

        // options based from choices above
        if((choice == 'P' || choice == 'p') && i > 0)
            i--;
        else if((choice == 'N' || choice == 'n') && i < userCount-1)
            i++;
        else if(choice == 'M' || choice == 'm')
        {
            if(users[i].ID != users[userIndex].ID && users[i].isDeleted != 1) // prevents self-messaging and deleted-user messaging
            { 
                recieverIDArr[0] = users[i].ID;
                sendMessage(users, userCount, userIndex, msgs, msgCount, recieverIDArr, 1);    
            }
        }
        else if(choice == 'A' || choice == 'a')
        {        
            // check if selected user is added already
            if(users[i].isDeleted != 1)
            {
                for(i = 0; i < users[userIndex].connectionCount; i++)
                {
                    if(users[i].ID == users[userIndex].connections[i])
                        cFlag = 1;
                    else if(users[i].ID == users[userIndex].ID)
                        cFlag = 2;
                }

                if(cFlag == 0) // if not yet added, add
                {
                    users[userIndex].connections[users[userIndex].connectionCount] = users[i].ID;
                    users[userIndex].connectionCount++;
                }
                else if(cFlag == 1)
                {
                    printf("\nSelected user is added to your connections already.\n");
                                    
                    printf("\n[A]dd     [R]eturn\n\n");
                    printf("[ ]: ");
                    choice = getch();
                    printf("\n");
                    
                    if(choice == 'R' || choice == 'r')
                        bFlag = 1;
                }
                else if(cFlag == 2)
                {
                    printf("\nYou cannot add yourself as a connection.\n");
                                    
                    printf("\n[A]dd     [R]eturn\n\n");
                    printf("[ ]: ");
                    choice = getch();
                    printf("\n");
                    
                    if(choice == 'R' || choice == 'r')
                        bFlag = 1;
                }
            }
        }
        else if(choice == 'V' || choice == 'v')
        {
            if(users[i].isDeleted != 1)
                viewProfile(users, userIndex, i);
        }
        else if(choice == 'F' || choice == 'f')
        {

            // prompt for a username
            printf("\nUsername: ");
            fflush(stdin);
            fgets(strLine, sizeof(strLine), stdin);
            strLine[strlen(strLine)-1] = '\0';

            dFlag = 0;
            // check for entered username in users[i]
            for(j = 0; j < userCount && !dFlag; j++)
            {   
                if(strcmp(users[j].username, strLine) == 0)
                {
                    i = j;
                    dFlag = 1;
                }
            }

            if(!dFlag || users[i].isDeleted == 1)
            {
                printf("\nUser does not exist.\n");
                printf("\n[R]eturn\n\n");
                printf("[ ]: ");
                do{
                    choice = getch();    
                } while(choice != 'R' && choice != 'r');
                printf("\n");
            }
        }
        else if(choice == 'R' || choice == 'r')
            bFlag = 1;
    }
}

/* 
DESCRIPTION:
    This function creates personal messages, group messages, and announcements.

PRECONDITION:
    recievrIDAr[] must not contain the ID of a deleted user.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param userCount - Refers to how many users exist in the system.
    @param recieverIDArr[] - Refers to an array of recieverIDs of the message.
    @param recieverCount - Refers to how many recievers a message has.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
sendMessage(user users, int userCount, int userIndex, message msgs, int *msgCount, int recieverIDArr[],  int recieverCount)
{
    int i, j, x, allow = 1, cFlag = 0, msgSpaces, deletedUser = 0;
    msg msgContent;

    // prevents deleted user messaging
    if(users[userIndex].isDeleted == 1)
        deletedUser = 1;
    for(x = 0; x < recieverCount; x++)
    {
        if(users[recieverIDArr[x]].isDeleted == 1)
            deletedUser = 1;
    }

    if(deletedUser != 1)
    {
        if(*msgCount > MAX_MSG)
            printf("Maximum messages reached in system.\n\n");
        else{ // if maximum messages not yet reached
            // assign senderID
            msgs[*msgCount].senderID = users[userIndex].ID;

            // assigned recieverID depending on message's type
            if(recieverCount == 1)
            { // if personal
                msgs[*msgCount].recieverID[0] = recieverIDArr[0];
                msgs[*msgCount].recieverCount = recieverCount;
            }
            else if(recieverCount > 1)
            { // if group
                for(i = 0; i < recieverCount; i++)
                    msgs[*msgCount].recieverID[i] = recieverIDArr[i];
                
                msgs[*msgCount].recieverCount = recieverCount;
            }
            else if(recieverCount == -1)
            {
                msgs[*msgCount].recieverID[0] = -1;
                msgs[*msgCount].recieverCount = recieverCount;
            } // if announcement
                
            // messaging yourself is not allowed
            for(j = 0; j < recieverCount; j++){
                if(msgs[*msgCount].recieverID[j] == users[userIndex].ID)
                    allow = 0;
            }

            if(!allow)
                printf("\nYou cannot add yourself as a recipient.\n");

            if(allow)
            {
                do{ // prompt message subject
                    printf("\nSubject: ");
                    fflush(stdin);
                    fgets(msgContent, sizeof(msgContent), stdin);
                    msgContent[strlen(msgContent)-1] = '\0';
                    strcpy(msgs[*msgCount].subject, msgContent);
                    
                    msgSpaces = 0; // counts if all char in a message is spaces
                    for(i = 0; i < strlen(msgs[*msgCount].subject); i++)
                    {
                        if(msgs[*msgCount].subject[i] == ' ')
                            msgSpaces++;

                    }

                    // defense against invalid input 
                    if(strlen(msgs[*msgCount].subject) > 100)
                    {
                        printf("Maximum characters exceeded.\n");
                        cFlag = 1;
                    }
                    else if(strlen(msgs[*msgCount].subject) == 0)
                    {
                        printf("No message written. Enter message.\n");
                        cFlag = 1;
                    }
                    else if(strlen(msgs[*msgCount].subject) == msgSpaces)
                    { 
                        printf("No message written. Enter message.\n");
                        cFlag = 1;
                    }
                    else
                        cFlag = 0;

                } while(cFlag); // max. string length

                do{ // prompt message content
                    cFlag = 0;
                    printf("\nMessage: ");
                    fflush(stdin);
                    fgets(msgContent, sizeof(msgContent), stdin);
                    msgContent[strlen(msgContent)-1] = '\0';
                    strcpy(msgs[*msgCount].content, msgContent);
                    
                    msgSpaces = 0; // counts if all char in a message is spaces
                    for(i = 0; i < strlen(msgs[*msgCount].content); i++)
                    {
                        if(msgs[*msgCount].content[i] == ' ')
                            msgSpaces++;
                    }

                    // defense against invalid input 
                    if(strlen(msgs[*msgCount].content) > 1000)
                    {
                        printf("Maximum characters exceeded.\n");
                        cFlag = 1;
                    }
                    else if(strlen(msgs[*msgCount].content) == 0)
                    {
                        printf("No message written. Enter message.\n");
                        cFlag = 1;
                    }
                    else if(strlen(msgs[*msgCount].content) == msgSpaces)
                    { 
                        printf("No message written. Enter message.\n");
                        cFlag = 1;
                    }
                    else
                        cFlag = 0;
                } while(cFlag); // max. msgChars length

                (*msgCount)++;
            }
        }
    }
}

/* 
DESCRIPTION:
    This function determines which parameters to send to sendMessage(); and it
    also serves as a message menu.

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
messageModule(message msgs, user users, int userCount, int *msgCount, int userIndex)
{
    int i, option, recieverCount, bFlag, exit, recieverIDArr[MAX_USERS], counterID, duplicate;
    char choice;

    exit = 0;
    while(exit != 1)
    {
        do{ // prompt for message module
            system("cls");
            printf("\n[MESSAGE]\n\n");
            printf("[1] - Personal Message\n");
            printf("[2] - Group Message\n");
            printf("[3] - Announcement\n");
            printf("[4] - Return\n\n");
            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");
        } while(option < 1 || option > 4);
        
        recieverCount = 0;
        bFlag = 0;
        counterID = 0;

    // following options determine which parameters to pass in sendMessage();

        if(option == 1)  // personal message
        {
            while(!bFlag)
            {
                recieverIDArr[0] = selectUser(users, userCount);
                if(recieverIDArr[0] != users[userIndex].ID)
                {
                    recieverCount = 1;
                    sendMessage(users, userCount, userIndex, msgs, msgCount, recieverIDArr, recieverCount);    
                    bFlag = 1;
                }
                else
                    printf("\nYou cannot add yourself as a recipient.\n");

                if(!bFlag) // return to module or message different recipient
                {
                    printf("\n[M]essage   [R]eturn\n\n");
                    printf("[ ]: ");
                    choice = getch();
                    printf("\n");
                    
                    if(choice == 'R' || choice == 'r')
                        bFlag = 1;
                }
            }
        }
        else if(option == 2) // group message
        {  
            counterID = 0;
            while(recieverCount < MAX_USERS && bFlag != 1)
            {
                int tempID = selectUser(users, userCount);
                
                if(users[userIndex].ID != tempID) // prevents self-messaging
                { 
                    if(recieverCount == 0){ // if first selection, add
                        recieverIDArr[counterID] = tempID;
                        counterID++;
                        recieverCount++;
                    }
                    else if(recieverCount > 0){ // if not first, check if selected user is a duplicate
                        duplicate = 0;
                        for(i = 0; i < counterID && duplicate != 1; i++)
                        {
                            if(recieverIDArr[i] == tempID)
                                duplicate = 1;
                        }

                        if(duplicate == 0) // non-duplicates are assigned
                        { 
                            recieverIDArr[counterID] = tempID;
                            counterID++;
                            recieverCount++;
                        }
                        else if(duplicate == 1)
                            printf("\nYou cannot add a duplicate recipient.\n");
                    }
                }
                else
                    printf("\nYou cannot add yourself as a recipient.\n");
                
                printf("\n[A]dd   [M]essage     [R]eturn\n\n");
                printf("[ ]: ");
                choice = getch();
                printf("\n");
                
                if(choice == 'R' || choice == 'r')
                    bFlag = 1;
                else if((choice == 'M' || choice == 'm') && recieverCount > 1)
                {
                    sendMessage(users, userCount, userIndex, msgs, msgCount, recieverIDArr, recieverCount);
                    bFlag = 1;
                }
                else if((choice == 'M' || choice == 'm') && recieverCount <= 1)
                {
                    printf("\nYou must have more than 1 recipient.\n");
                    printf("\n[A]dd\n\n");
                    printf("[ ]: ");
                    choice = getch();
                    printf("\n");
                } 
            }
        }
        else if(option == 3) // announcements
        { 
            recieverCount = -1;
            sendMessage(users, userCount, userIndex, msgs, msgCount, recieverIDArr, recieverCount);
        }

        if(option == 4)
            exit = 1;
    } 
}

/* 
DESCRIPTION:
    This function prints the details of a message.

PRECONDITION:
    A message must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param msgs - Refers the messages struct variable.
    @param msgIndex - Refers to which message is printed.
    @param userIndex - Refers to which user is logged in.
    @param checkingType - Refers to what type a message is.
    
RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
printMessage(message msgs, user users, int userIndex, int msgIndex, int checkingType)
{
    int i;

    // checkingType is 1 = inbox
    // checkingType is 2 = sent
    // checkingType is -1 = announcement
    
    // print sender
    if(checkingType != 2)
        printf("Sender: %s\n", users[msgs[msgIndex].senderID].username);

    // print recipient/s
    if(checkingType != -1)
    {
        printf("Recipient/s: ");
        if(msgs[msgIndex].recieverCount == -1)
            printf("ALL");
        else
        {
            for(i = 0; i < msgs[msgIndex].recieverCount; i++)
            {
                if(users[msgs[msgIndex].recieverID[i]].ID != users[userIndex].ID && i != msgs[msgIndex].recieverCount-1)
                    printf("%s, ", users[msgs[msgIndex].recieverID[i]].username);
                else if(users[msgs[msgIndex].recieverID[i]].ID != users[userIndex].ID && i == msgs[msgIndex].recieverCount-1)
                    printf("%s", users[msgs[msgIndex].recieverID[i]].username);
            }
        }
    }
    else if(checkingType == -1)
        printf("Recipient/s: ALL");
    
    printf("\n\n");

    // print subject and message
    printf("[%s]\n", msgs[msgIndex].subject);
    printf("---------------\n");
    printf("%s\n", msgs[msgIndex].content);
    printf("---------------\n");
    printf("\n\n");
}

/* 
DESCRIPTION:
    This function determines the relation of a message to a user.

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param msgs - Refers the messages struct variable.
    @param msgIndex - Refers to which message is printed.
    @param userIndex - Refers to which user is logged in.
    @param relationType - Refers to what message relation a user is checking for.
    
RETURN:
    This function returns 1 if the message is related to the user.
*/
int 
isMsgRelated(user users, message msgs, int msgIndex, int userIndex, int relationType)
{
    int i, bFlag, isRelated = 0;

    // check if user wants to check if msg is related in terms of sender/reciever

    if(relationType == 1) // check if msg is recieved by user
    { 
        for(i = 0; i < msgs[msgIndex].recieverCount && bFlag != 1; i++)
        {
            if(msgs[msgIndex].recieverID[i] == users[userIndex].ID)
            {
                isRelated = 1;
                bFlag = 1;
            }
        }
    }
    else if(relationType == 2) // check if msg is sent by user
    { 
        if(msgs[msgIndex].senderID == users[userIndex].ID)
            isRelated = 1;
    }
    else if(relationType == -1) // check if msg is announcement
    {
        if(msgs[msgIndex].recieverCount == -1)
            isRelated = 1;
    }

    return isRelated;
}

/* 
DESCRIPTION:
    This function prints the inbox messages of a specified user.

PRECONDITION:
    A message must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
checkInbox(user users, message msgs, int *msgCount, int userIndex, int userCount)
{
    int i, j, hasRecieved, whichMSG, msgIndex, related, bFlag, recieverIDArr[MAX_USERS], recieverCount;
    char choice;

    // check user has recieved a message,
    hasRecieved = 0;
    for(i = 0; i < *msgCount; i++)
    {
        for(j = 0; j < msgs[i].recieverCount; j++)
        {
            if(msgs[i].recieverID[j] == users[userIndex].ID)
                hasRecieved++;
        }
    }

    whichMSG = 1;
    msgIndex = (*msgCount)-1;
    bFlag = 0;

    if(hasRecieved == 0){ // if user has not yet recieved a message,
        do{
            system("cls");
            printf("\n[INBOX (0/0)]\n\n");
            printf("No messages recieved yet.\n");
            printf("\n[R]eturn\n\n");
            printf("[ ]: ");
            choice = getch();
        } while(choice != 'R' && choice != 'r');
    }
    else if(hasRecieved > 0){ // if user has recieved,

        do{ // find most recent message recieved by user
            related = isMsgRelated(users, msgs, msgIndex, userIndex, 1);
            if(!related)
                msgIndex--;
        } while(!related);
        
        while(!bFlag) // loop inbox
        {
            system("cls");
            printf("\n[INBOX (%d/%d)]\n\n", whichMSG, hasRecieved);

            printMessage(msgs, users, userIndex, msgIndex, 1);

            // prompt user for choice
            printf("[P]revious    [N]ext     [M]essage    [R]eturn\n");
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            // options listed above
            related = 0;
            if((choice == 'P' || choice == 'p') && whichMSG > 1)
            { // check for the next most recent recieved message
                while(!related)
                {
                    msgIndex++;
                    related = isMsgRelated(users, msgs, msgIndex, userIndex, 1);
                }
                whichMSG--;
    
            }
            else if((choice == 'N' || choice == 'n') && whichMSG < hasRecieved)
            { // check for next least recent recieved message
                while(!related) 
                {
                    msgIndex--;    
                    related = isMsgRelated(users, msgs, msgIndex, userIndex, 1);
                }
                whichMSG++;
            }
            else if(choice == 'M' || choice == 'm')
            { // reply
                for(i = 0; i < msgs[i].recieverCount; i++)
                    recieverIDArr[i] = msgs[msgIndex].recieverID[i];
                
                recieverCount = msgs[msgIndex].recieverCount;
                sendMessage(users, userCount, userIndex, msgs, msgCount, recieverIDArr, recieverCount);    
                
            }
            else if(choice == 'R' || choice == 'r')
                bFlag = 1;
        
        }
    }

}

/* 
DESCRIPTION:
    This function prints the sent messages of a specified user.

PRECONDITION:
    A message must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
checkSent(user users, message msgs, int msgCount, int userIndex)
{    
    int i, hasSent, whichMSG, msgIndex, related, bFlag;
    char choice;

    // check user has sent a message,
    hasSent = 0;
    for(i = 0; i < msgCount; i++){
        if(msgs[i].senderID == users[userIndex].ID)
            hasSent++;
    }

    whichMSG = 1;
    msgIndex = msgCount-1;
    bFlag = 0;
    
    if(hasSent == 0){ // if user has not yet sent a message,
        do{
            system("cls");
            printf("\n[SENT (0/0)]\n\n");
            printf("No messages sent yet.\n");
            printf("\n[R]eturn\n\n");
            printf("[ ]: ");
            choice = getch();
        } while(choice != 'R' && choice != 'r');
    }
    else if(hasSent > 0) // if user has sent a messsage,
    {
        do{ // find most recent message sent by user
            related = isMsgRelated(users, msgs, msgIndex, userIndex, 2);
            if(related == 0)
                msgIndex--;
        } while(related != 1);
        
        while(bFlag != 1) // loop sent
        {
            system("cls");
            printf("\n[SENT (%d/%d)]\n\n", whichMSG, hasSent);

            printMessage(msgs, users, userIndex, msgIndex, 2);

            // prompt choice
            printf("[P]revious    [N]ext    [R]eturn\n");
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            related = 0;
            // options above
            if((choice == 'P' || choice == 'p') && whichMSG > 1)
            {
                while(!related) // find next most recent message sent by user
                {
                    msgIndex++;
                    related = isMsgRelated(users, msgs, msgIndex, userIndex, 2);
                }
                whichMSG--;
            }
            else if((choice == 'N' || choice == 'n') && whichMSG < hasSent)
            {
                while(!related) // find least recent message sent by user
                {
                    msgIndex--;
                    related = isMsgRelated(users, msgs, msgIndex, userIndex, 2);
                }
                whichMSG++;
            }
            else if(choice == 'R' || choice == 'r')
                bFlag = 1;
        }
    }
}

/* 
DESCRIPTION:
    This function prints all announcements.

PRECONDITION:
    A message must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param userIndex - Refers to which user is logged in.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
checkAnnouncements(user users, message msgs, int *msgCount, int userIndex, int userCount)
{
    int i, whichMSG, msgIndex, hasAnnouncements, bFlag, related, recieverIDArr[MAX_USERS], recieverCount;
    char choice;

    // check announcement count
    hasAnnouncements = 0;
    for(i = 0; i < *msgCount; i++){
        if(msgs[i].recieverCount == -1)
            hasAnnouncements++;
    }

    whichMSG = 1;
    msgIndex = (*msgCount)-1;
    bFlag = 0;

    if(hasAnnouncements == 0) // if no announcements made yet,
    {
        do{
            system("cls");
            printf("\n[ANNOUNCEMENTS (0/0)]\n\n");
            printf("No announcements made yet.\n");
            printf("\n[R]eturn\n\n");
            printf("[ ]: ");
            choice = getch();
        } while(choice != 'R' && choice != 'r');
    }
    else if(hasAnnouncements > 0) // if an announcement has been made,
    {
        do{ // find most recent announcement
            related = isMsgRelated(users, msgs, msgIndex, userIndex, -1);
            if(!related)
                msgIndex--;
        } while(!related);

        while(!bFlag) // loop announcements
        {
            system("cls");      
            printf("\n[ANNOUNCEMENTS (%d/%d)]\n\n", whichMSG, hasAnnouncements);

            printMessage(msgs, users, userIndex, msgIndex, -1);

            // prompt option
            printf("[P]revious    [N]ext     [M]essage    [R]eturn\n");
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            related = 0;
            if((choice == 'P' || choice == 'p') && whichMSG > 1){
                while(!related) // find next most recent announcement
                {
                   msgIndex++;
                    related = isMsgRelated(users, msgs, msgIndex, userIndex, -1);
                }
                whichMSG--;

            }
            else if((choice == 'N' || choice == 'n') && whichMSG < hasAnnouncements){
                while(!related) // find next least recent announcement
                {
                    msgIndex--;
                    related = isMsgRelated(users, msgs, msgIndex, userIndex, -1);
                }
                whichMSG++;
            }
            else if(choice == 'M' || choice == 'm')
            {
                recieverCount = msgs[msgIndex].recieverCount;
                sendMessage(users, userCount, userIndex, msgs, msgCount, recieverIDArr, recieverCount);
            }
            else if(choice == 'R' || choice == 'r')
                bFlag = 1;
        }
    }
}

/* 
DESCRIPTION:
    This function modifies a user's name & description

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param msgs - Refers the messages struct variable.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
modifyAccount(user users, int userIndex)
{
    int bFlag, cFlag, option, verify;
    string strLine, passwordVerify;
    char choice;

    do{
        verify = 0;
        bFlag = 0;
        do{
            system("cls");
            printf("\n[MODIFY PERSONAL CONTENTS]\n\n");   

            printf("[1] - Modify Name\n");
            printf("[2] - Modify Description\n");
            printf("[3] - Return\n\n");

            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");

        }while(option < 1 && option > 3);

        if(option == 1){
            do{
                system("cls");
                printf("\n[MODIFY NAME]\n\n");  
                printf("Current Name: %s\n\n", users[userIndex].name);
            
            // prompt for new name
                printf("Change Name: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strlen(strLine) > 100)
                    printf("Maximum characters exceeded.\n\n");

            } while(strlen(strLine) > 100);

            do{
                cFlag = 0;

                printf("\nVerify Password: ");
                fflush(stdin);
                fgets(passwordVerify, sizeof(passwordVerify), stdin);
                passwordVerify[strlen(passwordVerify)-1] = '\0';
                
                if(strcmp(passwordVerify, users[userIndex].password) == 0){
                    verify = 1;
                    cFlag = 1;
                }
                    
                else{
                    printf("Incorrect Password.\n");
                    printf("\n[A]gain    [R]eturn\n\n");
                    choice = getch();
                    printf("[]: \n");

                    if(choice == 'R' || choice == 'r')
                        cFlag = 1;
                }
            } while(cFlag != 1);

            // change name variable
            if(verify == 1)
                strcpy(users[userIndex].name, strLine);
        }
        else if(option == 2){
            do{
                system("cls");
                printf("\n[MODIFY DESCRIPTION]\n\n");  
                printf("Current Description: %s\n\n", users[userIndex].description);
            
            // prompt for new description
                printf("Change Description: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strlen(strLine) > 100)
                    printf("Maximum characters exceeded.\n\n");

            } while(strlen(strLine) > 100);

            do{
                cFlag = 0;

                printf("\nVerify Password: ");
                fflush(stdin);
                fgets(passwordVerify, sizeof(passwordVerify), stdin);
                passwordVerify[strlen(passwordVerify)-1] = '\0';
                
                if(strcmp(passwordVerify, users[userIndex].password) == 0){
                    verify = 1;
                    cFlag = 1;
                }
                    
                else{
                    printf("Incorrect Password.\n");
                    printf("\n[A]gain    [R]eturn\n\n");
                    choice = getch();
                    printf("[]: \n");

                    if(choice == 'R' || choice == 'r')
                        cFlag = 1;
                }
            } while(cFlag != 1);

            // change description variable
            if(verify == 1)
            strcpy(users[userIndex].description, strLine);
            
        }
        else if(option == 3)
            bFlag = 1;
    } while(bFlag != 1);


}

/* 
DESCRIPTION:
    This function modifies a user's security answer & password.

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
modifySecurity(user users, int userIndex)
{    
    string strLine, strLine2;
    int i, bFlag, cFlag, msgSpaces, option, verify;
    char choice;

    bFlag = 0;
    while(!bFlag)
    {
        do{ // prompt for option
            system("cls");
            printf("\n[MODIFY ACCOUNT SECURITY]\n\n");   

            printf("[1] - Modify Password\n");
            printf("[2] - Modify Security Answer\n");
            printf("[3] - Return\n\n");

            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");

        } while(option < 1 && option > 3);

        verify = 0;
        cFlag = 0;
        if(option == 1) // change password
        { 
            system("cls");
            printf("\n[MODIFY PASSWORD]\n\n");

            while(verify != 1 && cFlag != 1)
            { // enter old password once,
                printf("\nEnter Old Password: ");  
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strcmp(users[userIndex].password, strLine) == 0) 
                    verify = 1;
                else
                {
                    printf("\nIncorrect password.\n\n");
                    
                    printf("[A]gain    [R]eturn\n\n");
                    printf("[ ]: ");
                    choice = getch();
                    
                    if(choice == 'R' || choice == 'r')
                        cFlag = 1;
                }
            }

            if(verify) // if correct,
            { // prompt new password. (max. 20 chars)
                do{
                    printf("Enter New Password: ");  
                    fflush(stdin);
                    fgets(strLine, sizeof(strLine), stdin);
                    strLine[strlen(strLine)-1] = '\0';

                    msgSpaces = 0; // counts if all char in a message is spaces
                    for(i = 0; i < strlen(strLine); i++)
                    {
                        if(strLine[i] == ' ')
                            msgSpaces++;

                    }
                    
                    if(strlen(strLine) > MAX_CHAR_PASS)
                        printf("Exceeds 20 characters. Enter a shorter password.\n\n");
                    else if(strlen(strLine) == 0 || msgSpaces == strlen(strLine))
                        printf("No password written.\n\n");
                    else
                        cFlag = 1;

                } while(!cFlag);

                do{ // re-enter new password,
                    cFlag = 0;
                    printf("Re-Enter New Password: ");  
                    fflush(stdin);
                    fgets(strLine2, sizeof(strLine2), stdin);
                    strLine2[strlen(strLine2)-1] = '\0';
                    
                    if(strcmp(strLine2, strLine) != 0)
                    {
                        printf("\n[A]gain    [R]eturn\n\n");
                        printf("[ ]: ");
                        choice = getch();
                    
                        if(choice == 'R' || choice == 'r')
                            cFlag = 1;
                    }
                    else if(strcmp(strLine2, strLine) == 0) 
                    {  // if correct, password change occurs.
                        strcpy(users[userIndex].password, strLine);
                        cFlag = 1;
                    }
                } while(!cFlag);
            }
        }
        else if(option == 2) // change security question
        { // enter new secu answer twice (prompted with the same secu question)

            while(!cFlag)
            {        
                system("cls");
                printf("\n[MODIFY SECURITY ANSWER]\n\n");
                printf("Assigned Security Question: %s\n", users[userIndex].securityQuestion);

                do{ // prompt new answer
                    printf("Enter New Answer: ");  
                    fflush(stdin);
                    fgets(strLine, sizeof(strLine), stdin);
                    strLine[strlen(strLine)-1] = '\0';

                    msgSpaces = 0; // counts if all char in a message is spaces
                        for(i = 0; i < strlen(strLine); i++)
                        {
                            if(strLine[i] == ' ')
                                msgSpaces++;

                        }
                    // input validation
                    if(strlen(strLine) > 101)
                        printf("Maximum characters exceeded. Enter a shorter answer.\n\n");
                    else if(strlen(strLine) == msgSpaces || strlen(strLine) == 0)
                        printf("No answer written.\n\n");
                    else
                        verify = 1;
                } while(!verify);

                if(verify)
                {
                    while(!cFlag)
                    {
                        printf("Re-Enter New Answer: ");  
                        fflush(stdin);
                        fgets(strLine2, sizeof(strLine2), stdin);
                        strLine2[strlen(strLine2)-1] = '\0';

                        if(strcmp(strLine2, strLine) != 0)
                        {
                            printf("Answers do not match.");
                            printf("\n[A]gain    [R]eturn\n\n");
                            printf("[ ]: ");
                            choice = getch();
                            printf("\n\n");
                    
                            if(choice == 'R' || choice == 'r')
                            cFlag = 1;
                        }
                        else
                        {   
                            strcpy(users[userIndex].securityAnswer, strLine);
                            cFlag = 1;
                        }
                    }
                }
            }
        }
        else if(option == 3)
            bFlag = 1;
    }
}

/* 
DESCRIPTION:
    This function determines which connections function executes.

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
connectionsModule(user users, int userIndex, int userCount)
{
    int option, exit;
    exit = 0;
    
    while(!exit)
    {
        do{ // prompt for option
            system("cls");
            printf("\n[PERSONAL CONNECTIONS]\n\n");
            printf("[1] - Add a personal connection\n");
            printf("[2] - View personal connections\n");
            printf("[3] - Remove a personal connection\n");
            printf("[4] - View user connections\n");
            printf("[5] - Return\n\n");

            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");
        } while(option < 1 || option > 5);

        switch(option)
        {
            case 1: addConnections(users, userIndex, userCount);
                    break;
            case 2: viewPersonalConnections(users, userIndex);
                    break;
            case 3: removeConnections(users, userIndex, userCount);
                    break;
            case 4: viewUserConnections(users, userIndex, userCount);
                    break;
            case 5: exit = 1;
                    break;
        }
    }
}

/* 
DESCRIPTION:
    This function adds a personal connection via. selectUsers() or username.

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
addConnections(user users, int userIndex, int userCount)
{
    int i, option, exit, bFlag, cFlag, dFlag, tempID, exists;
    char choice;
    string strLine;

    exit = 0;
    while(!exit)
    {
        do{ // prompt for option
            system("cls");
            printf("\n[ADD A PERSONAL CONNECTION]\n\n");
            printf("[1] - Select a User\n");
            printf("[2] - Type Username\n");
            printf("[3] - Return\n\n");

            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");
        } while(option < 1 || option > 3);

        if(option == 1) // add via selectUsers();
        {
            bFlag = 0;
            while(!bFlag)
            {
                // select a user to add
                cFlag = 0;
                do{
                    tempID = selectUser(users, userCount);
                } while(users[tempID].isDeleted == 1);
                
                // check if selected user is added already
                if(tempID == users[userIndex].ID)
                    cFlag = 2;
                else
                {
                    for(i = 0; i < users[userIndex].connectionCount; i++)
                    {
                        if(tempID == users[userIndex].connections[i])
                            cFlag = 1;
                        else if(tempID == users[userIndex].ID)
                            cFlag = 2;
                    }
                }

                if(cFlag == 0) // if not yet added, add
                {
                    users[userIndex].connections[users[userIndex].connectionCount] = tempID;
                    users[userIndex].connectionCount++;
                }
                else if(cFlag == 1)
                    printf("\nSelected user is added to connections already.\n");
                else if(cFlag == 2)
                    printf("\nYou cannot add this user as a connection.\n");
                
                printf("\n[A]dd     [R]eturn\n\n");
                printf("[ ]: ");
                choice = getch();
                printf("\n");
                
                if(choice == 'R' || choice == 'r')
                    bFlag = 1;
            }

        }
        else if(option == 2) // add via username
        {
            bFlag = 0;
            while(!bFlag)
            {
                // prompt for a username
                printf("Username: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                dFlag = 0;
                // check for entered username in users[i]
                for(i = 0; i < userCount && !dFlag; i++)
                {   
                    if(strcmp(users[i].username, strLine) == 0)
                    {
                        if(strcmp(users[userIndex].username, strLine) == 0) // you cannot add yourself
                        {
                            exists = 2;
                            dFlag = 1;
                        }
                        else
                        {
                            exists = 1;
                            users[userIndex].connections[users[userIndex].connectionCount] = users[i].ID;
                            users[userIndex].connectionCount++;
                            printf("\n");
                            dFlag = 1;

                        }
                    }                            
                    else
                        exists = 0;
                }

                if(exists == 0) // if username does not exist
                    printf("\nUsername does not exist.\n\n");
                else if(exists == 2) // else if you add yourself,
                    printf("\nYou cannot add this user as a connection.\n\n");

                // prompt user for option
                printf("[A]dd    [R]eturn\n\n"); 
                printf("[ ]: ");
                do{
                    choice = getch();
                } while ((choice != 'R' && choice != 'r') && (choice != 'A' && choice != 'a'));
                printf("\n\n");

                if(choice == 'R' || choice == 'r')
                    bFlag = 1;
            }
        }
        else if(option == 3)
            exit = 1;
        
    }
}

/* 
DESCRIPTION:
    This function removes a personal connection via. selectUsers();

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
removeConnections(user users, int userIndex, int userCount)
{
    int i, j, exit, connectionIndex;
    char choice;

    exit = 0;

    if(users[userIndex].connectionCount == 0)
    {
        do{
            system("cls");
            printf("\n[REMOVE CONNECTIONS]\n\n");
            printf("No personal connections.\n\n");
            
            printf("[R]eturn\n\n");
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            if(choice == 'R' || choice == 'r')
                exit = 1;
        } while(choice != 'R' && choice != 'r');
    }
    
    i = 0, j = 0;
    while(!exit)
    {
        if(users[userIndex].connectionCount != 0)
        {
            connectionIndex = users[userIndex].connections[i];

            system("cls");
            printf("\n[REMOVE A PERSONAL CONNECTION]\n\n");
            printf("User #%d: %s\nName: %s\n\n", users[connectionIndex].ID, users[connectionIndex].username, users[connectionIndex].name);

            printf("[P]revious    [N]ext     [S]elect    [R]eturn\n\n");    
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            if((choice == 'P' || choice == 'p') && i > 0)
                i--;
            else if((choice == 'N' || choice == 'n') && i < users[userIndex].connectionCount)
                i++;
            else if(choice == 'S' || choice == 's')
            {
                if(i+1 != users[userIndex].connectionCount)
                { // dont execute if in borders of the connections array
                    for(j = i; j < users[userIndex].connectionCount; j++)
                        users[userIndex].connections[j] = users[userIndex].connections[j+1];
                
                }
                users[userIndex].connectionCount--;
            }
            else if(choice == 'R' || choice == 'r')
                exit = 1;
        }
        else // all personal connections are removed,
        {
            do{
                system("cls");
                printf("\n[REMOVE A PERSONAL CONNECTION]\n\n");
                printf("No personal connections.\n\n");
                printf("[R]eturn\n\n");    
                printf("[ ]: ");
                choice = getch();
                printf("\n");
            } while(choice != 'R' && choice != 'r');
                exit = 1;
        }
    
    }
}

/* 
DESCRIPTION:
    This function views a user's personal connections

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void
viewPersonalConnections(user users, int userIndex)
{
    int i = 0, exit = 0;
    char choice;

    // if users does not have connections
    if(users[userIndex].connectionCount == 0)
    {
        do{
            system("cls");
            printf("\n[PERSONAL CONNECTIONS]\n\n");
            printf("No personal connections.\n\n");
            
            printf("[R]eturn\n\n");
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            if(choice == 'R' || choice == 'r')
                exit = 1;
        } while(choice != 'R' && choice != 'r');
    }
    
    // view personal connections
    while(!exit)
    {
        system("cls");
        printf("\n[PERSONAL CONNECTIONS]\n\n");
        
        for(i = 0; i < users[userIndex].connectionCount; i++)
            printf("%d.) %s\n", i+1, users[users[userIndex].connections[i]].username);
        
        printf("\n");
        printf("[R]eturn\n\n");    
        printf("[ ]: ");
        choice = getch();
        printf("\n");

        if(choice == 'R' || choice == 'r')
            exit = 1;
    }
}

/* 
DESCRIPTION:
    This function shows all the connections a user is in.

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param userIndex - Refers to which user is logged in.

RETURN:
    This function does not return anything; void.
*/
void 
viewUserConnections(user users, int userIndex, int userCount)
{
    int i = 0, j = 0, exit = 0, userIDs[MAX_USERS], inConnections = 0;
    char choice;

    // traverse through users[i].connections[j] and check if userIndex is there
    for(i = 0; i < userCount; i++)
    {
        for(j = 0; j < users[i].connectionCount; j++)
        {
            if(users[i].connections[j] == users[userIndex].ID)
            {
                userIDs[inConnections] = users[i].ID;
                inConnections++;
            }
        }
    }

    // if user is not in any connections
    if(inConnections == 0)
    {
        do{
            system("cls");
            printf("\n[USER CONNECTIONS]\n\n");
            printf("No user connections.\n\n");
            
            printf("[R]eturn\n\n");
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            if(choice == 'R' || choice == 'r')
                exit = 1;
        } while(choice != 'R' && choice != 'r');
    }
    
    // view personal connections
    while(!exit)
    {
        system("cls");
        printf("\n[USER CONNECTIONS]\n\n");
        
        for(i = 0; i < inConnections; i++)
            printf("%d.) %s\n", i+1, users[userIDs[i]].username);
        
        printf("\n");
        printf("[R]eturn\n\n");    
        printf("[ ]: ");
        choice = getch();
        printf("\n");

        if(choice == 'R' || choice == 'r')
            exit = 1;
    }
}

/* 
DESCRIPTION:
    This function serves as an admin login interface.

PRECONDITION:
    An admin must be exist.

PARAMETERS:
    @param admin - Refers the only existing admin variable.
    @param loggedIn - Refers the login status of a user.

RETURN:
    This function does not return anything as the parameters passed
    are changed through pointer; void.
*/
void 
adminLogin(administrator admin, int *loggedIn)
{
    string password;
    int bFlag = 0;
    char choice;

    printf("\n[ADMIN LOGIN]\n\n");

    while(!bFlag)
    {
        // prompt password
        printf("Admin Password: ");
        fflush(stdin);
        fgets(password, sizeof(password), stdin);
        password[strlen(password)-1] = '\0';

        if(strcmp(admin.password, password) == 0){
            *loggedIn = 2;
            bFlag = 1;
        }   

        // if invalid credentials, try again or return to main
        if(!bFlag)
        {
            printf("\nInvalid Login Credentials.\n\n");
            printf("[A]gain    [R]eturn\n\n");
            printf("[ ]: ");
            do{
                choice = getch();
                printf("\n");

                if(choice == 'A' || choice == 'a')
                    bFlag = 0;
                else if(choice == 'R' || choice == 'r')
                    bFlag = 1;
            } while(choice != 'A' && choice != 'a' && choice != 'R' && choice != 'r');
            printf("\n");
        }
    }
}

/* 
DESCRIPTION:
    This function serves as the admin's menu and indicates which
    admin function executes.

PRECONDITION:
    An admin must be exist.

PARAMETERS:
   @param *option - Refers to which function is executed on a given menu.

RETURN:
    This function does not return anything as the parameters passed
    are changed through pointer; void.
*/
void 
adminMenu(int *option)
{    
    do{ // prompt admin modules
        system("cls");

        printf("\n[ADMIN ACCESS]\n\n");
        printf("[1] - Users Module\n");
        printf("[2] - Messages Module\n");
        printf("[3] - Modify Admin Password\n");        
        printf("[4] - Log Out\n\n");

        printf("[ ]: ");
        scanf("%d", option);
        printf("\n");
    } while(*option < 1 || *option > 4);
}

/* 
DESCRIPTION:
    This function serves as the admin's user module menu and indicates which
    user module function executes.

PRECONDITION:
    An admin must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param admin - Refers the only existing admin variable.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.

RETURN:
    This function does not return anything; void.
*/
void 
usersModule(user users, message msgs, int *msgCount, int *userCount, administrator *admin)
{
    int option, exit = 0;
    
    while(!exit)
    {
        do{ // prompt admin's user modules
            system("cls");

            printf("\n[USER MODULE]\n\n");
            printf("[1] - View Users\n");
            printf("[2] - Modify Users\n");
            printf("[3] - Refresh User Account Password\n");      
            printf("[4] - Delete Users\n");
            printf("[5] - Security Questions\n");       
            printf("[6] - Return\n\n");

            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");
        } while(option < 1 || option > 6);

        switch(option)
        {
            case 1: viewUsers(users, *userCount);
                    break;
            case 2: modifyUsers(users, *userCount);
                    break;
            case 3: refreshPasswordAdmin(users, *userCount, admin);
                    break;
            case 4: deleteUser(msgs, users, userCount, msgCount);
                    break;
            case 5: securityQuestions(admin);
                    break;
            case 6: exit = 1;
                    break;
        }
    }
}

/* 
DESCRIPTION:
    This function views all users in the system.

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything; void.
*/
void 
viewUsers(user users, int userCount)
{
    int i = 0, j = 0, exit = 0;
    char choice;

    if(userCount == 0)
    {
        do{
            system("cls");
            printf("\n[VIEW REGISTERED USERS]\n\n");
            printf("No registered users.\n\n");

            printf("[R]eturn\n");    
            printf("\n[ ]: ");
            choice = getch();
            printf("\n");
        } while(choice != 'R' && choice != 'r');
        exit = 1;
    }

    while(!exit)
    {
        system("cls");
        printf("\n[VIEW REGISTERED USERS]\n\n");

        printf("User #%d: %s\n", users[i].ID, users[i].username);
        printf("Name: %s\n", users[i].name);
         printf("Description: %s\n", users[i].description);

        // cipher password
        string cipheredPass = "";
        for(j = 0; j < strlen(users[i].password); j++)
            cipheredPass[j] = (users[i].password[j])+3;
        
        printf("Password: %s\n", cipheredPass);
        if(users[i].isDeleted == 0)
            printf("Status: Unlocked\n\n");
        else if(users[i].isDeleted == 1)
            printf("Status: Locked\n\n");

        // prompt choice
        printf("[P]revious      [N]ext       [V]iew Connections       [R]eturn\n");    
        fflush(stdin);
        printf("\n[ ]: ");
        choice = getch();
        printf("\n");

        // options based from choices above
        if((choice == 'P' || choice == 'p') && i > 0)
            i--;
        else if((choice == 'N' || choice == 'n') && i < userCount-1)
            i++;
        else if(choice == 'V' || choice == 'v')
            viewPersonalConnections(users, i);
        else if(choice == 'R' || choice == 'r')
            exit = 1;
    }

}

/* 
DESCRIPTION:
    This function provides the admin with full modification over the data
    of all users in the system.

PRECONDITION:
    A user must be exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
modifyUsers(user users, int userCount)
{
    int i, bFlag, tempID, exit = 0, isSelected;
    char choice;
    string strLine;

    if(userCount == 0)
    {
        do{
            system("cls");
            printf("\n[MODIFY REGISTERED USERS]\n\n");
            printf("No registered users.\n\n");

            printf("[R]eturn\n");    
            printf("\n[ ]: ");
            choice = getch();
            printf("\n");
        } while(choice != 'R' && choice != 'r');
        exit = 1;
    }

    isSelected = 0;
    while(!exit)
    {
        if(isSelected == 0)
        {
            tempID = selectUser(users, userCount);
            isSelected = 1;
        }

        system("cls");
        printf("\n[MODIFY REGISTERED USER]\n\n");
        
        printf("User #%d: %s\n", users[tempID].ID, users[tempID].username);
        printf("Name: %s\n\n", users[tempID].name);

        printf("[N]ame      [U]sername      [D]escription       [C]onnections        [R]eturn\n");    
        
        fflush(stdin);
        printf("\n[ ]: ");
        choice = getch();
        printf("\n");

        if(choice == 'N' || choice == 'n')
        {
            do{            
                // prompt for new name
                printf("\nChange Name: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strlen(strLine) > 100)
                    printf("Maximum characters exceeded.\n\n");
            } while(strlen(strLine) > 100);
            
            strcpy(users[tempID].name, strLine);
        }
        else if(choice == 'U' || choice == 'u')
        {
            bFlag = 0;          
                // prompt for new username
            printf("\nChange Username: ");
            while(!bFlag)
            {
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strlen(strLine) > 100)
                {
                    printf("Maximum characters exceeded.\n\n");

                    printf("[R]eturn\n\n");
                    printf("[ ]: ");
                    do{
                        fflush(stdin);
                        choice = getch();
                    }while(choice != 'R' && choice != 'r');
                    bFlag = 1;
                }
                
                for(i = 0; i < userCount; i++)
                {
                    if(strcmp(strLine, users[i].username) == 0)
                    {
                        printf("Username taken.\n");

                        printf("[R]eturn\n\n");
                        printf("[ ]: ");
                        do{
                            fflush(stdin);
                            choice = getch();
                        }while(choice != 'R' && choice != 'r');
                        bFlag = 1;
                    }
                }
                if(!bFlag)
                {
                    strcpy(users[tempID].username, strLine);
                    bFlag = 1;
                }
            }
        }
        else if(choice == 'D' || choice == 'd')
        {
            do{
            // prompt for new description
                printf("\nChange Description: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strlen(strLine) > 100)
                    printf("Maximum characters exceeded.\n\n");

            } while(strlen(strLine) > 100);
            strcpy(users[tempID].description, strLine);
        }
        else if(choice == 'C' || choice == 'c')
        {
            printf("\n[A]dd      [R]emove\n");    
            printf("\n[ ]: ");
            do{
                choice = getch();
                printf("\n");
            } while(choice != 'A' && choice != 'a' && choice != 'R' && choice != 'r');

            if(choice == 'A' || choice == 'a')
                addConnections(users, tempID, userCount);
            if(choice == 'R' || choice == 'r')
                removeConnections(users, tempID, userCount);
        }
        else if(choice == 'R' || choice == 'r')
            exit = 1;
    }
}

/* 
DESCRIPTION:
    This function provides the user the option to reset their password.

PRECONDITION:
    N/A

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
refreshPasswordUser(user users, int userCount, administrator *admin)
{
    int i = 0, bFlag = 0, exit = 0, userIndex = -1;
    string strLine;
    char choice;

    while(!exit)
    {
        system("cls");
        printf("\n[REFRESH PASSWORD]\n\n");

        userIndex = -1;
        // ask for username
        printf("\nEnter username: ");
        fflush(stdin);
        fgets(strLine, sizeof(strLine), stdin);
        strLine[strlen(strLine)-1] = '\0';

        for(i = 0; i < userCount; i++) // check if user exists
        {
            if(strcmp(strLine, users[i].username) == 0)
                userIndex = i;
        }

        if(userIndex == -1) // if username does not exist, prompt to try again or return
        {
            printf("\nUser does not exist.\n\n");
            printf("[A]gain    [R]eturn\n\n");

            printf("[ ]: ");
            do{
                fflush(stdin);
        
                choice = getch();

                if(choice == 'R' || choice == 'r'){
                    exit = 1;
                }
            } while(choice != 'R' && choice != 'r' && choice != 'A' && choice != 'a');
            printf("\n");
        }
        else if(userIndex != -1 && !bFlag) // if username exists, prompt for their security question
        {
            bFlag = 1;
            while(bFlag)
            {
                printf("\nSecurity Question: %s\n", users[userIndex].securityQuestion);
                printf("\nEnter security answer: ");
                fflush(stdin);
                fgets(strLine, sizeof(strLine), stdin);
                strLine[strlen(strLine)-1] = '\0';

                if(strcmp(strLine, users[userIndex].securityAnswer) == 0) // if correct
                {
                    handleRequest(users, userIndex, admin);
                    exit = 1;
                    bFlag = 0;
                }
                else // if incorrect,
                {
                    printf("\nIncorrect security answer.\n\n");
                    printf("[A]gain    [R]eturn\n\n");
                    printf("[ ]: ");
                    do{
                        choice = getch();
                        if(choice == 'R' || choice == 'r'){
                            bFlag = 0;
                        }
                    } while(choice != 'A' && choice != 'a' && choice != 'R' && choice != 'r');
                    printf("\n");
                }
            }
        }
    }
}

/* 
DESCRIPTION:
    This function provides the admin the ability to refresh a user's password given a
    user's request.

PRECONDITION:
    A request for password reset must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
refreshPasswordAdmin(user users, int userCount, administrator *admin)
{    
    int i, j, exit, isProcessed;
    char choice;

    // check if there are password request
    if(admin->securityRequestCount == 0) // if none, print and return
    {
        system("cls");
        printf("\n[PASSWORD REQUESTS]\n\n");
        printf("No users have requested for a password reset.\n\n");

        printf("[R]eturn\n\n");
        printf("[ ]: ");
        do{
            choice = getch();
            if(choice == 'R' || choice == 'r')
                exit = 1;
        } while(choice != 'R' && choice != 'r');
    }
    else if(admin->securityRequestCount != 0)
    {
        i = 0;
        exit = 0;
        while(!exit)
        {
            system("cls");
            printf("\n[PASSWORD REQUESTS]\n\n");

            // select from list of admin.securityRequests
            printf("User [#%d]: %s\n",  users[admin->securityRequests[i]].ID, users[admin->securityRequests[i]].username);

            printf("\n[P]revious    [N]ext     [S]elect    [R]eturn\n\n");    
            printf("[ ]: ");
            choice = getch();
            printf("\n");

            if((choice == 'P' || choice == 'p') && i > 0)
                i--;
            else if((choice == 'N' || choice == 'n') && i < (admin->securityRequestCount)-1)
                i++;
            else if(choice == 'S' || choice == 's')
            {
                // if selected, check if their request is processed already,
                isProcessed = 0;
                for(j = 0; j < admin->processedRequestCount; j++)
                {
                    if(admin->securityRequests[i] == admin->processedRequests[j])
                    {
                        printf("\nUser is processed already.\n");
                        isProcessed = 1;
                    }

                }
                
                if(!isProcessed){
                    admin->processedRequests[i] = admin->securityRequests[i];
                    admin->processedRequestCount++;
                }
                
                printf("\n[A]gain     [R]eturn\n\n");
                printf("[ ]: ");
                do{
                    choice = getch();
                    if(choice == 'R' || choice == 'r')
                        exit = 1;
                } while(choice != 'R' && choice != 'r' && choice != 'A' && choice != 'a');
            }
            else if(choice == 'R' || choice == 'r')
                exit = 1;
        }
    }

}

/* 
DESCRIPTION:
    This function determines the status of a user's password reset.

PRECONDITION:
    A request for password reset must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
handleRequest(user users, int userIndex, administrator *admin)
{
    int i, exit = 0, option, requestSent, isProcessed, userSecurityIndex, userProcessedIndex;
    string strLine; 
    char choice;

    while(!exit)
    {
        // check if user has sent a request & if admin has processed the request.
        requestSent = 0;
        isProcessed = 0;
        for(i = 0; i < admin->securityRequestCount; i++)
        {
            if(users[userIndex].ID == admin->securityRequests[i])
            {
                requestSent = 1;
                userSecurityIndex = i;
            }
        }
        for(i = 0; i < admin->processedRequestCount; i++)
        {
            if(users[userIndex].ID == admin->processedRequests[i])
            {
                isProcessed = 1;
                userProcessedIndex = i;
            }
        }
        
        do{
            system("cls");
            printf("\n[REQUEST RESET]\n\n");
            printf("[1] - Send Request\n");
            printf("[2] - Update on Request\n");
            printf("[3] - Return\n\n");
            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");
        } while(option < 1 || option > 3);
    
        if(option == 1)
        {
            if(!requestSent)  // if request not yet sent,
            {
                admin->securityRequests[admin->securityRequestCount] = userIndex;
                admin->securityRequestCount++;

                printf("Request sent.\n\n");
                printf("[R]eturn\n\n");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');
            }
            else
            {
                printf("Request has been sent already.\n\n");
                printf("[R]eturn\n\n");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');
            }
        }
        else if(option == 2)
        {
            if(!requestSent)
            {
                printf("No request sent yet.\n\n");
                printf("[R]eturn\n\n");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');
            }
            else if(requestSent && !isProcessed)
            {
                printf("Request is being processed.\n\n");
                printf("[R]eturn\n\n");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');
            }            
            else if(requestSent && isProcessed)
            {
                printf("Request is processed.\n\n");
                        
                do{ // prompt password
                    printf("Enter Password: ");
                    fflush(stdin);
                    fgets(strLine, sizeof(strLine), stdin);
                    strLine[strlen(strLine)-1] = '\0';

                    if(strlen(strLine) > MAX_CHAR_PASS)
                        printf("Exceeds 20 characters. Enter a shorter password.\n");

                } while(strlen(strLine) > MAX_CHAR_PASS);

                strcpy(users[userIndex].password, strLine);
                
                printf("\nPassword Changed.\n\n");

                // shifts the array to remove user in the ff. arrays
                if(i+1 != admin->processedRequestCount) // dont execute if in borders of the processedRequests array
                { 
                    for(i = userProcessedIndex; i < admin->processedRequestCount; i++)
                        admin->processedRequests[i] = admin->processedRequests[i+1];
                }
                if(i+1 != admin->securityRequestCount) // dont execute if in borders of the securityRequests array
                { 
                    // remove userIndex in processedrequest and sentrequest and decrement their count
                    for(i = userSecurityIndex; i < admin->securityRequestCount; i++)
                        admin->securityRequests[i] = admin->securityRequests[i+1];
                }
                admin->securityRequestCount--;
                admin->processedRequestCount--;

                printf("[R]eturn\n\n");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');

            }
        }
        else if(option == 3)
            exit = 1;
    }
}

/* 
DESCRIPTION:
    This function allows the admin to modify security questions.

PRECONDITION:
    A request for password reset must exist.

PARAMETERS:
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
securityQuestions(administrator *admin)
{
    int i, j, bFlag, option, exit = 0, valid, questionIndex;
    char choice;
    string strLine;

    while(!exit)
    {
        bFlag = 0;
        do{
        system("cls");
        printf("\n[SECURITY QUESTIONS]\n\n");
        printf("[1] - View Security Questions\n");  
        printf("[2] - Add Security Question\n");
        printf("[3] - Remove Security Question\n");      
        printf("[4] - Return\n\n");

        printf("[ ]: ");
        scanf("%d", &option);
        printf("\n");
        } while(option < 1 || option > 4);


    if(option == 1)
    {   
        while(!bFlag)
        {
            system("cls");
            printf("\n[VIEW SECURITY QUESTIONS]\n\n");
            
            for(i = 0; i < admin->securityQuestionCount; i++)
                printf("%d.) %s\n", i+1, admin->securityQuestions[i]);
            
            printf("\n[R]eturn\n\n");
            do{
                printf("[ ]: ");
                choice = getch();
            } while(choice != 'R' && choice != 'r');

            if(choice == 'R' || choice == 'r')
                bFlag = 1;
        }
    }
    else if(option == 2)
    {
        if(MAX_SECURITY_QUESTIONS == admin->securityQuestionCount)
        {
            system("cls");
            printf("\n[ADD SECURITY QUESTIONS]\n\n");
            printf("Maximum security questions reached.\n");

            printf("\n[R]eturn\n\n");
            do{
                printf("[ ]: ");
                choice = getch();
            } while(choice != 'R' && choice != 'r');

            if(choice == 'R' || choice == 'r')
                bFlag = 1;
        }
        while(!bFlag)
        {
            valid = 1;
            system("cls");
            printf("\n[ADD SECURITY QUESTIONS]\n\n");

            printf("Enter New Question: ");

            fflush(stdin);
            fgets(strLine, sizeof(strLine), stdin);
            strLine[strlen(strLine)-1] = '\0';

            for(i = 0; i < admin->securityQuestionCount; i++)
            {
                if(strcmp(strLine, admin->securityQuestions[i]) == 0)
                {
                    printf("Question already exists.\n");
                    valid = 0;
                }
            }

            if(valid)
            {
                strcpy(admin->securityQuestions[admin->securityQuestionCount], strLine);
                admin->securityQuestionCount++;
            }

            printf("\n[A]dd    [R]eturn\n\n");
            do{
                printf("[ ]: ");
                choice = getch();
            } while(choice != 'R' && choice != 'r' && choice != 'A' && choice != 'a');

            if(choice == 'R' || choice == 'r')
                bFlag = 1;            
        }

    }
    else if(option == 3)
    {
        questionIndex = 0;
        while(!bFlag)
        {
            if(admin->securityQuestionCount != 1)
            {
                system("cls");
                printf("\n[REMOVE SECURITY QUESTION]\n\n");
                printf("%s\n", admin->securityQuestions[questionIndex]);

                printf("\n[P]revious    [N]ext     [S]elect    [R]eturn\n\n");    
                printf("[ ]: ");
                choice = getch();
                printf("\n");

                if((choice == 'P' || choice == 'p') && questionIndex > 0)
                    questionIndex--;
                else if((choice == 'N' || choice == 'n') && questionIndex < (admin->securityQuestionCount)-1)
                    questionIndex++;
                else if(choice == 'S' || choice == 's')
                {
                    if(questionIndex+1 != admin->securityQuestionCount)
                    { // dont execute if in borders of the connections array
                        for(j = questionIndex; j < admin->securityQuestionCount; j++)
                        strcpy(admin->securityQuestions[j], admin->securityQuestions[j+1]);
                    
                    }
                    admin->securityQuestionCount--;
                }
                else if(choice == 'R' || choice == 'r')
                    bFlag = 1;
            }
            else // all personal connections are removed,
            {
                do{
                    system("cls");
                    printf("\n[REMOVE SECURITY QUESTION]\n\n");
                    printf("You cannot remove any more security questions.\n\n");
                    printf("[R]eturn\n\n");    
                    printf("[ ]: ");
                    choice = getch();
                    printf("\n");
                } while(choice != 'R' && choice != 'r');
                    bFlag = 1;
            }
        }
    }
    else if(option == 4)
        exit = 1;

    }
}

/* 
DESCRIPTION:
    This function changes the admin password.

PRECONDITION:
    The admin must exist.

PARAMETERS:
    @param admin - Refers the only existing admin variable.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
adminPassModule(administrator *admin)
{
    int exit = 0, valid;
    char choice;
    string strLine, strLine2;

    while(!exit)
    {
        valid = 0;
        system("cls");
        printf("\n[CHANGE ADMIN PASSWORD]\n\n");
        
        printf("\nEnter Old Password: ");  
        fflush(stdin);
        fgets(strLine, sizeof(strLine), stdin);
        strLine[strlen(strLine)-1] = '\0';

        if(strcmp(strLine, admin->password) == 0)
            valid = 1;
        
        if(!valid)
        {
            printf("Incorrect password.\n");
            printf("\n[A]gain    [R]eturn\n\n");
            fflush(stdin);
            printf("[ ]: ");
            do{
                choice = getch();
                printf("\n");

                if(choice == 'R' || choice == 'r'){
                    exit = 1;
                }
            } while(choice != 'A' && choice != 'a' && choice != 'R' && choice != 'r');
            printf("\n");
        }
        else if(valid)
        {
            printf("\nEnter New Password: ");  
            fflush(stdin);
            fgets(strLine, sizeof(strLine), stdin);
            strLine[strlen(strLine)-1] = '\0';

            printf("\nRe-enter New Password: ");  
            fflush(stdin);
            fgets(strLine2, sizeof(strLine2), stdin);
            strLine2[strlen(strLine2)-1] = '\0';
            printf("\n");

            if(strcmp(strLine, strLine2) == 0)
            {
                strcpy(admin->password, strLine2);
                exit = 1;
            }
            else
            {
                printf("Passwords do not match.\n");
                printf("\n[A]gain    [R]eturn\n\n");
                printf("[ ]: ");
                do{
                    choice = getch();

                    if(choice == 'R' || choice == 'r'){
                        valid = 0;
                    }
                } while(choice != 'A' && choice != 'a' && choice != 'R' && choice != 'r');
                printf("\n");   
            }
        }
    } 
}

/* 
DESCRIPTION:
    This function provides the admin full control with all the messages that 
    exist in the system.

PRECONDITION:
    A message must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.

RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void
messagesModule(user users, message msgs, int *msgCount, int userCount)
{
    int exit = 0, option = 0, tempID = 0;
    char choice;

    while(!exit)
    {
        do{
            system("cls");
            printf("\n[MESSAGES MODULE]\n\n");

            printf("[1] - View Messages\n");
            printf("[2] - Filter Messages\n");
            printf("[3] - Delete Messages\n");         
            printf("[4] - Return\n\n");

            printf("[ ]: ");
            scanf("%d", &option);
            printf("\n");
        } while(option < 1 || option > 4);

        if(option == 1)
            messagesAdmin(users, msgs, msgCount, userCount, 0);
        else if(option == 2)
        {
            printf("\nFilter Messages By:\n\n");
            printf("[S]ender    [R]eciever\n\n");

            printf("[ ]: ");
            do{
                choice = getch();
            } while(choice != 'S' && choice != 's' && choice != 'R' && choice != 'r');

            tempID = selectUser(users, userCount);

            if(choice == 'S' || choice == 's')
                checkSent(users, msgs, *msgCount, tempID);
            else if(choice == 'R' || choice == 'r')
                checkInbox(users, msgs, msgCount, tempID, userCount);
        }
        else if(option == 3)
            messagesAdmin(users, msgs, msgCount, userCount, 1);
        else if(option == 4)
            exit = 1;
    }


}

/* 
DESCRIPTION:
    This function provides the admin full control with all the messages that 
    exist in the system.

PRECONDITION:
    A message must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    @param ifDelete - Refers if a message is to be deleted.
    
RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
messagesAdmin(user users, message msgs, int *msgCount, int userCount, int ifDelete)
{
    int i = 0, j = 0, exit = 0, msgIndex = 0;
    char choice;

    if(*msgCount == 0){ // exit if no messages in system
        system("cls");
        printf("\n[ALL MESSAGES]\n\n");
        printf("\nNo messages in the system.\n\n");
        do{
            printf("[R]eturn\n\n");    
            printf("[ ]: ");
            choice = getch();
            printf("\n");
        } while(choice != 'R' && choice != 'r');
        exit = 1;
    }

    while(!exit)
    {
        system("cls");
        printf("\n[ALL MESSAGES]\n\n");

        if(*msgCount != 0)
        {
            printf("Sender: %s\n", users[msgs[msgIndex].senderID].username);
            if(msgs[msgIndex].recieverCount == -1)
                printf("Recipient/s: ALL\n");
            else
            {
                printf("Recipient/s: ");
                for(i = 0; i < msgs[msgIndex].recieverCount; i++)
                {
                    if(i != (msgs[msgIndex].recieverCount)-1)
                        printf("%s, ", users[msgs[msgIndex].recieverID[i]].username);
                    else
                        printf("%s\n", users[msgs[msgIndex].recieverID[i]].username);
                }                   
            }
            printf("\n");

            printf("[%s]\n", msgs[msgIndex].subject);
            printf("-------------\n");
            printf("%s\n", msgs[msgIndex].content);
            printf("-------------\n\n");
    
            if(ifDelete)
            {
                printf("[P]revious     [N]ext      [D]elete     [R]eturn\n\n");
                fflush(stdin);
                printf("[ ]: ");
                do{
                    choice = getch();
                } while(choice != 'P' && choice != 'p' && choice != 'N' && choice != 'n' && choice != 'R' && choice != 'r' && choice != 'D' && choice != 'd');
            }
            else
            {
                printf("[P]revious     [N]ext      [R]eturn\n\n");
                fflush(stdin);
                printf("[ ]: ");
                do{
                    choice = getch();
                } while(choice != 'P' && choice != 'p' && choice != 'N' && choice != 'n' && choice != 'R' && choice != 'r');
            }
        }

        if((choice == 'P' || choice == 'p') && msgIndex > 0)
            msgIndex--;
        else if((choice == 'N' || choice == 'n') && msgIndex < (*msgCount)-1)
            msgIndex++;
        else if((choice == 'D' || choice == 'd') && ifDelete == 1)
        {
            if(*msgCount != 0)
            {
                for(j = msgIndex; j < *msgCount; j++)
                msgs[j] = msgs[j+1];

                (*msgCount)--;
            }
            else
            {
                printf("\nNo messages in system.\n\n");
                printf("[R]eturn\n\n");
                printf("[ ]: ");
                do{
                    choice = getch();
                } while(choice != 'R' && choice != 'r');
            }


        }
        else if(choice == 'R' || choice == 'r')
            exit = 1;

    }
}

/* 
DESCRIPTION:
    This function deletes a user in the system.

PRECONDITION:
    A user must exist.

PARAMETERS:
    @param users - Refers to the userTag variable.
    @param userCount - Refers to how many users exist in the system.
    @param msgs - Refers the messages struct variable.
    @param msgCount - Refers how many messages exist in the system.
    
RETURN:
    This function does not return anything as the parameters passed are changed through
    pointer; void.
*/
void 
deleteUser(message msgs, user users, int *userCount, int *msgCount)
{
    int i, j, x, exit = 0, tempID;
    char choice;

    if(*userCount == 0) // if no users
    {
        system("cls");
        printf("\n[DELETE USER]\n\n");
        printf("No users in the system.\n\n");
        printf("[R]eturn\n\n");
        printf("[ ]: ");
        do{
            choice = getch();
        } while(choice != 'R' && choice != 'r');
        exit = 1;
    }

    while(!exit)
    {
        tempID = selectUser(users, *userCount);

        system("cls");

        if(users[tempID].isDeleted == 1)
        {
            printf("\n[USER IS ALREADY DELETED]\n\n");
            do{
                choice = getch();
            } while(choice != 'R' && choice != 'r');
            exit = 1;
        }
        else if(users[tempID].isDeleted == 0)
        {
            printf("\n\n[USER DELETED]\n\n");

            strcpy(users[tempID].username, "DELETED USER.");
            strcpy(users[tempID].name, "DELETED USER.");
            strcpy(users[tempID].password, "DELETED USER.");
            strcpy(users[tempID].description, "DELETED USER.");
            strcpy(users[tempID].securityQuestion, "DELETED USER.");
            strcpy(users[tempID].securityAnswer, "DELETED USER.");
            
            for(i = 0; i < users[tempID].connectionCount; i++)
                users[tempID].connections[i] = -1;

            users[tempID].connectionCount = 0;

            users[tempID].isDeleted = 1;

            for(i = 0; i < *msgCount; i++)
            {
                for(j = 0; j < msgs[i].recieverCount; j++)
                {
                    if(users[msgs[i].recieverID[j]].isDeleted)
                    {
                        x = j;
                        do{
                           msgs[i].recieverID[x] = msgs[i].recieverID[x+1];
                        } while(x != (msgs[i].recieverCount)-1);
                        msgs[i].recieverCount--;
                    }
                }
            }

            printf("[R]eturn\n\n");
            printf("[ ]: ");
            do{
                choice = getch();
            } while(choice != 'R' && choice != 'r');
            exit = 1;
        }
    }

}



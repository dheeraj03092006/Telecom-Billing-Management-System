#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PHONENUMBER_LENGTH 20
#define MAX_NAME_LENGTH 50
#define MAX_PACKAGE_LENGTH 50
#define MAX_TIMESTAMP_LENGTH 50
#define MAX_EXPIRATION_LENGTH 50
#define MAX_PAYMENT_METHOD_LENGTH 20

struct subscriber {
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    char name[MAX_NAME_LENGTH];
    char state[18];
    float amount;
    char package[MAX_PACKAGE_LENGTH];
    char timestamp[MAX_TIMESTAMP_LENGTH];
    char expiration[MAX_EXPIRATION_LENGTH];
    char paymentMethod[MAX_PAYMENT_METHOD_LENGTH];
};

// Function prototypes
void listRecords();
void searchRecords();
void storePaymentInfo(char *phonenumber, float amount, char *package, char *timestamp, char *expiration, char *paymentMethod);
void payment();
void registerSubscriber();
void displayPackages();
void displayPaymentRecords();
void checkExpiration();
void topUp();

void listRecords() {
    struct subscriber s;
    FILE *f;
    int i;

    f = fopen("hello.txt", "rb");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    system("cls"); // This clears the screen, platform dependent (Windows)
    printf("Phone Number\t\tUser Name\t\t\tState\t\tTimestamp\t\tExpiration\n");
    for (i = 0; i < 120; i++)
        printf("-");
    printf("\n");

    while (fread(&s, sizeof(s), 1, f) == 1) {
        printf("%-10s\t\t%-20s\t\t%s\t%20s\t%20s\n", s.phonenumber, s.name, s.state, s.timestamp, s.expiration);
    }

    printf("\n");
    for (i = 0; i < 120; i++)
        printf("-");

    fclose(f);
}

void searchRecords() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    int found = 0;

    f = fopen("hello.txt", "rb");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter Phone Number to search in our database: ");
    scanf("%s", phonenumber);

    printf("Search Results:\n");
    printf("Phone Number\t\tUser Name\t\t\tState\t\tTimestamp\t\tExpiration\n");
    for (int i = 0; i < 120; i++)
        printf("-");

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            printf("\n%-10s\t\t%-20s\t\t%s\t%20s\t%20s\n", s.phonenumber, s.name, s.state, s.timestamp, s.expiration);
            found = 1;
        }
    }

    if (!found) {
        printf("\nRequested Phone Number Not found in our database\n");
    }

    printf("\n");
    for (int i = 0; i < 120; i++)
        printf("-");

    fclose(f);
}

void storePaymentInfo(char *phonenumber, float amount, char *package, char *timestamp, char *expiration, char *paymentMethod) {
    FILE *paymentFile;
    paymentFile = fopen("poisaa_records.txt", "a");
    if (paymentFile == NULL) {
        printf("Error opening payment records file.\n");
        exit(1);
    }

    fprintf(paymentFile, "%-10s\t\t%.2f\t\t%s\t\t%s\t\t%s\t\t%s\n", phonenumber, amount, package, timestamp, expiration, paymentMethod);
    fclose(paymentFile);
}

void payment() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    char paymentMethod[MAX_PAYMENT_METHOD_LENGTH];
    int found = 0;

    f = fopen("hello.txt", "r+");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter your phone number: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            found = 1;
            printf("\nPhone No.: %s", s.phonenumber);
            printf("\nName\t: %s", s.name);
            printf("\nState\t: %s", s.state);
            printf("\nCurrent Balance: %.2f\n", s.amount);
            printf("\n\nAvailable Packages:\n");
            displayPackages();
            printf("\nEnter the package number you want to recharge: ");
            scanf("%s", s.package);

            float packageCost = 0;
            if (strcmp(s.package, "1") == 0) packageCost = 200;
            else if (strcmp(s.package, "2") == 0) packageCost = 300;
            else if (strcmp(s.package, "3") == 0) packageCost = 500;
            else if (strcmp(s.package, "4") == 0) packageCost = 700;
            else if (strcmp(s.package, "5") == 0) packageCost = 1100;

            if (s.amount < packageCost) {
                printf("Insufficient balance. Please top-up your account.\n");
                fclose(f);
                return;
            }

            printf("\nSelect Payment Method:\n");
            printf("1: Credit/Debit Card\n");
            printf("2: UPI\n");
            printf("3: Bank Transfer\n");
            printf("Enter your choice: ");
            int paymentChoice;
            scanf("%d", &paymentChoice);

            switch (paymentChoice) {
                case 1:
                    printf("Enter Credit/Debit Card Number: ");
                    char cardNumber[20];
                    scanf("%s", cardNumber);
                    printf("Enter Card Holder Name: ");
                    char cardHolderName[MAX_NAME_LENGTH];
                    getchar();  // to clear the newline left by previous scanf
                    fgets(cardHolderName, sizeof(cardHolderName), stdin);
                    cardHolderName[strcspn(cardHolderName, "\n")] = '\0';
                    printf("Enter Expiry Date (MM/YY): ");
                    char cardExpiry[8];
                    scanf("%s", cardExpiry);
                    printf("Enter CVV: ");
                    char cvv[4];
                    scanf("%s", cvv);
                    strcpy(paymentMethod, "Credit/Debit Card");
                    break;
                case 2:
                    printf("Enter UPI ID: ");
                    char upiID[50];
                    scanf("%s", upiID);
                    strcpy(paymentMethod, "UPI");
                    break;
                case 3:
                    printf("Enter Bank Account Number: ");
                    char bankAccount[20];
                    scanf("%s", bankAccount);
                    printf("Enter IFSC Code: ");
                    char ifscCode[12];
                    scanf("%s", ifscCode);
                    strcpy(paymentMethod, "Bank Transfer");
                    break;
                default:
                    printf("Invalid payment method.\n");
                    fclose(f);
                    return;
            }

            s.amount -= packageCost;

            time_t t = time(NULL);
            char *timeString = ctime(&t);
            timeString[strcspn(timeString, "\n")] = '\0';
            strcpy(s.timestamp, timeString);

            struct tm exp_tm = *localtime(&t);
            int validity_days = 0;

            if (strcmp(s.package, "1") == 0) validity_days = 30;
            else if (strcmp(s.package, "2") == 0) validity_days = 30;
            else if (strcmp(s.package, "3") == 0) validity_days = 90;
            else if (strcmp(s.package, "4") == 0) validity_days = 180;
            else if (strcmp(s.package, "5") == 0) validity_days = 365;

            exp_tm.tm_mday += validity_days;
            mktime(&exp_tm);
            strftime(s.expiration, MAX_EXPIRATION_LENGTH, "%Y-%m-%d %H:%M:%S", &exp_tm);

            printf("\nRecharge successful.\n");
            printf("Expiration Date: %s\n", s.expiration);
            printf("Remaining Balance: %.2f\n", s.amount);

            storePaymentInfo(s.phonenumber, packageCost, s.package, s.timestamp, s.expiration, paymentMethod);

            fseek(f, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);

            break;
        }
    }

    if (!found) {
        printf("Subscriber with the given phone number not found. Please register first.\n");
    }

    fclose(f);
}

void registerSubscriber() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    char name[MAX_NAME_LENGTH];
    char state[18];

    f = fopen("hello.txt", "ab");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter your phone number: ");
    scanf("%19s", phonenumber);
    printf("\nEnter your name: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    printf("\nEnter your state: ");
    fgets(state, sizeof(state), stdin);
    state[strcspn(state, "\n")] = '\0';

    strcpy(s.phonenumber, phonenumber);
    strcpy(s.name, name);
    strcpy(s.state, state);

    time_t t = time(NULL);
    char *timeString = ctime(&t);
    timeString[strcspn(timeString, "\n")] = '\0';
    strcpy(s.timestamp, timeString);

    s.amount = 0;
    strcpy(s.package, "N/A");
    strcpy(s.expiration, "N/A");

    fwrite(&s, sizeof(s), 1, f);

    printf("Registration successful.\n");

    fclose(f);
}

void displayPackages() {
    printf("+---------+------------+----------+----------+\n");
    printf("| Package | Data Limit | Price    | Validity |\n");
    printf("+---------+------------+----------+----------+\n");
    printf("|    1    | 1 GB/Day   | Rs. 200  | 30 Days  |\n");
    printf("|    2    | 2 GB/Day   | Rs. 300  | 30 Days  |\n");
    printf("|    3    | 2 GB/Day   | Rs. 500  | 3 Months |\n");
    printf("|    4    | 2 GB/Day   | Rs. 700  | 6 Months |\n");
    printf("|    5    | 2 GB/Day   | Rs. 1100 | 1 Year   |\n");
    printf("+---------+------------+----------+----------+\n");
}

void displayPaymentRecords() {
    FILE *paymentFile;
    char ch;
    paymentFile = fopen("poisaa_records.txt", "r");
    if (paymentFile == NULL) {
        printf("Error opening payment records file.\n");
        exit(1);
    }

    printf("Phone Number\t\tAmount\t\tPackage\t\tTimestamp\t\tExpiration\t\tPayment Method\n");
    for (int i = 0; i < 140; i++)
        printf("-");
    printf("\n");

    while ((ch = fgetc(paymentFile)) != EOF) {
        putchar(ch);
    }
    printf("\n");
    for (int i = 0; i < 140; i++)
        printf("-");

    fclose(paymentFile);
}

void checkExpiration() {
    struct subscriber s;
    FILE *f;
    time_t now = time(NULL);
    struct tm now_tm = *localtime(&now);
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    int found = 0;

    f = fopen("hello.txt", "rb");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter your phone number to check expiration: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            found = 1;
            struct tm exp_tm;
            strptime(s.expiration, "%Y-%m-%d %H:%M:%S", &exp_tm);

            if (difftime(mktime(&now_tm), mktime(&exp_tm)) > 0) {
                printf("Your recharge has expired. Please recharge to continue services.\n");
            } else {
                printf("Your recharge is still valid. Expiration Date: %s\n", s.expiration);
            }
            break;
        }
    }

    if (!found) {
        printf("Subscriber with the given phone number not found. Please register first.\n");
    }

    fclose(f);
}

void topUp() {
    struct subscriber s;
    FILE *f;
    char phonenumber[MAX_PHONENUMBER_LENGTH];
    float topupAmount;
    int found = 0;

    f = fopen("hello.txt", "r+");
    if (f == NULL) {
        printf("Error opening records file.\n");
        exit(1);
    }

    printf("Enter your phone number to top-up: ");
    scanf("%s", phonenumber);

    while (fread(&s, sizeof(s), 1, f) == 1) {
        if (strcmp(s.phonenumber, phonenumber) == 0) {
            found = 1;
            printf("\nPhone No.: %s", s.phonenumber);
            printf("\nName\t: %s", s.name);
            printf("\nState\t: %s", s.state);
            printf("\nCurrent Balance: %.2f", s.amount);
            printf("\n\nEnter the top-up amount: ");
            scanf("%f", &topupAmount);
            s.amount += topupAmount;

            time_t t = time(NULL);
            char *timeString = ctime(&t);
            timeString[strcspn(timeString, "\n")] = '\0';
            strcpy(s.timestamp, timeString);

            printf("\nTop-up successful. Updated balance: %.2f\n", s.amount);

            storePaymentInfo(s.phonenumber, topupAmount, "Top-up", s.timestamp, s.expiration, "Top-up");

            fseek(f, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, f);

            break;
        }
    }

    if (!found) {
        printf("Subscriber with the given phone number not found. Please register first.\n");
    }

    fclose(f);
}

int main() {
    int choice;

    printf("\n\n\n\n\n\n\n\n\n**");
    printf("\n\t\t------WELCOME TO THE GEETHANJALI TELECOM BILLING MANAGEMENT SYSTEM-----");
    printf("\n\t\t*");

    do {
        printf("\n Enter\n");
        printf(" 1 : for list of records\n");
        printf(" 2 : for payment\n");
        printf(" 3 : for searching records.\n");
        printf(" 4 : for registering a new subscriber\n");
        printf(" 5 : for displaying available packages\n");
        printf(" 6 : for payments records\n");
        printf(" 7 : to check recharge expiration\n");
        printf(" 8 : for top-up\n");
        printf(" 9 : for exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                listRecords();
                break;
            case 2:
                payment();
                break;
            case 3:
                searchRecords();
                break;
            case 4:
                registerSubscriber();
                break;
            case 5:
                displayPackages();
                break;
            case 6:
                displayPaymentRecords();
                break;
            case 7:
                checkExpiration();
                break;
            case 8:
                topUp();
                break;
            case 9:
                printf("\n\n\t\t\t\tTHANK YOU FOR USING GEETHANJALI TELECOM SERVICES\n");
                exit(0);
            default:
                printf("Incorrect Input\n");
                break;
        }
    } while (choice != 9);

    return 0;
}
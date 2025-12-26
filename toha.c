#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------- Structures -------- */
struct Account {
    int accountNumber;
    char name[50];
    char password[20];
    float balance;
};

struct Admin {
    char id[10];
    char name[50];
    char password[20];
};

/* -------- Function Prototypes -------- */
void initAdmin();
void adminLogin();
void adminMenu();
void viewAllAccounts();
void updateAdminInfo();

void createAccount();
void loginSystem();
void accountMenu(struct Account acc);
void depositMoney(struct Account acc);
void withdrawMoney(struct Account acc);
void updateAccount(struct Account updatedAcc);
void updateAccountInfo(struct Account acc);
int generateAccountNumber();
int getAccountByNumber(int accNo, struct Account *acc);

/* -------- Main Function -------- */
int main() {
    int choice;
    initAdmin();   // 🔹 auto create admin if not exists

    while (1) {
        printf("\n===== Bank Account System =====\n");
        printf("1. Create Account\n");
        printf("2. User Login\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: loginSystem(); break;
            case 3: adminLogin(); break;
            case 4: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

/* ================= ADMIN PART ================= */

/* -------- Initialize Admin -------- */
void initAdmin() {
    FILE *fp = fopen("admin.txt", "r");
    if (fp != NULL) {
        fclose(fp);
        return;   // admin already exists
    }

    fp = fopen("admin.txt", "w");
    struct Admin admin = {"0001", "admin", "admin"};
    fprintf(fp, "%s %s %s\n", admin.id, admin.name, admin.password);
    fclose(fp);
}

/* -------- Admin Login -------- */
void adminLogin() {
    struct Admin admin;
    FILE *fp = fopen("admin.txt", "r");
    char id[10], pass[20];

    if (!fp) {
        printf("Admin data not found!\n");
        return;
    }

    fscanf(fp, "%s %s %s", admin.id, admin.name, admin.password);
    fclose(fp);

    printf("\n--- Admin Login ---\n");
    printf("Admin ID: ");
    scanf("%s", id);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(id, admin.id) == 0 &&
        strcmp(pass, admin.password) == 0) {
        printf("Admin Login Successful!\n");
        adminMenu();
    } else {
        printf("Invalid Admin Credentials!\n");
    }
}

/* -------- Admin Menu -------- */
void adminMenu() {
    int choice;

    while (1) {
        printf("\n===== Admin Menu =====\n");
        printf("1. View All User Accounts\n");
        printf("2. Update Admin Info\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAllAccounts(); break;
            case 2: updateAdminInfo(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}

/* -------- Update Admin Info -------- */
void updateAdminInfo() {
    struct Admin admin;
    FILE *fp = fopen("admin.txt", "r");
    if (!fp) return;

    fscanf(fp, "%s %s %s", admin.id, admin.name, admin.password);
    fclose(fp);

    printf("New Admin Name: ");
    scanf("%s", admin.name);
    printf("New Password: ");
    scanf("%s", admin.password);

    fp = fopen("admin.txt", "w");
    fprintf(fp, "%s %s %s\n", admin.id, admin.name, admin.password);
    fclose(fp);

    printf("Admin info updated successfully!\n");
}

/* -------- View All Accounts -------- */
void viewAllAccounts() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;
    int count = 0;

    if (!fp) {
        printf("No user accounts found!\n");
        return;
    }

    printf("\nAccNo\tName\tBalance\n");

    while (fscanf(fp, "%d %s %s %f",
           &acc.accountNumber, acc.name,
           acc.password, &acc.balance) != EOF) {

        printf("%d\t%s\t%.2f\n",
               acc.accountNumber, acc.name, acc.balance);
        count++;
    }

    fclose(fp);
    printf("\nTotal Users: %d\n", count);
}

/* ================= USER PART ================= */

/* -------- Create Account -------- */
void createAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "a");
    if (!fp) return;

    acc.accountNumber = generateAccountNumber();
    acc.balance = 0.0;

    printf("Enter Name (no space): ");
    scanf("%s", acc.name);
    printf("Set Password: ");
    scanf("%s", acc.password);

    fprintf(fp, "%d %s %s %.2f\n",
            acc.accountNumber, acc.name,
            acc.password, acc.balance);

    fclose(fp);
    printf("Account Created! Acc No: %d\n", acc.accountNumber);
}

/* -------- Generate Account Number -------- */
int generateAccountNumber() {
    FILE *fp;
    struct Account acc;
    int last = 1000;

    fp = fopen("accounts.txt", "r");
    if (!fp) return last + 1;

    while (fscanf(fp, "%d %s %s %f",
           &acc.accountNumber, acc.name,
           acc.password, &acc.balance) != EOF)
        last = acc.accountNumber;

    fclose(fp);
    return last + 1;
}

/* -------- User Login -------- */
void loginSystem() {
    FILE *fp = fopen("accounts.txt", "r");
    struct Account acc;
    int accNo, found = 0;
    char pass[20];

    if (!fp) return;

    printf("Account Number: ");
    scanf("%d", &accNo);
    printf("Password: ");
    scanf("%s", pass);

    while (fscanf(fp, "%d %s %s %f",
           &acc.accountNumber, acc.name,
           acc.password, &acc.balance) != EOF) {

        if (acc.accountNumber == accNo &&
            strcmp(acc.password, pass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) accountMenu(acc);
    else printf("Invalid Login!\n");
}

/* -------- Account Menu -------- */
void accountMenu(struct Account acc) {
    int choice;

    while (1) {
        getAccountByNumber(acc.accountNumber, &acc);

        printf("\n1.Deposit  2.Withdraw  3.View  4.Update  5.Logout\n");
        scanf("%d", &choice);

        if (choice == 1) depositMoney(acc);
        else if (choice == 2) withdrawMoney(acc);
        else if (choice == 3)
            printf("Balance: %.2f\n", acc.balance);
        else if (choice == 4) updateAccountInfo(acc);
        else if (choice == 5) return;
    }
}

/* -------- Deposit / Withdraw / Update -------- */
void depositMoney(struct Account acc) {
    float a; scanf("%f", &a);
    acc.balance += a; updateAccount(acc);
}
void withdrawMoney(struct Account acc) {
    float a; scanf("%f", &a);
    if (a <= acc.balance) {
        acc.balance -= a;
        updateAccount(acc);
    }
}
void updateAccountInfo(struct Account acc) {
    scanf("%s %s", acc.name, acc.password);
    updateAccount(acc);
}

/* -------- Update Account File -------- */
void updateAccount(struct Account updatedAcc) {
    FILE *fp = fopen("accounts.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");
    struct Account acc;

    while (fscanf(fp, "%d %s %s %f",
           &acc.accountNumber, acc.name,
           acc.password, &acc.balance) != EOF) {

        if (acc.accountNumber == updatedAcc.accountNumber)
            fprintf(tmp, "%d %s %s %.2f\n",
                    updatedAcc.accountNumber,
                    updatedAcc.name,
                    updatedAcc.password,
                    updatedAcc.balance);
        else
            fprintf(tmp, "%d %s %s %.2f\n",
                    acc.accountNumber,
                    acc.name,
                    acc.password,
                    acc.balance);
    }
    fclose(fp); fclose(tmp);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

/* -------- Reload Latest Account -------- */
int getAccountByNumber(int accNo, struct Account *acc) {
    FILE *fp = fopen("accounts.txt", "r");
    if (!fp) return 0;

    while (fscanf(fp, "%d %s %s %f",
           &acc->accountNumber, acc->name,
           acc->password, &acc->balance) != EOF) {
        if (acc->accountNumber == accNo) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

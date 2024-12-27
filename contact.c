hello













#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining maximum sizes for contact information
#define MAX_NAME_LENGTH 100
#define MAX_PHONE_LENGTH 15
#define MAX_EMAIL_LENGTH 100
#define MAX_ADDRESS_LENGTH 200

// Structure to store contact information
typedef struct {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
} Contact;

// Function prototypes
void displayMenu();
void addContact(Contact **contacts, int *count);
void displayContacts(Contact *contacts, int count);
void searchContact(Contact *contacts, int count);
void deleteContact(Contact **contacts, int *count);
void saveContactsToFile(Contact *contacts, int count);
int loadContactsFromFile(Contact **contacts);

int main() {
    Contact *contacts = NULL;
    int count = loadContactsFromFile(&contacts);
    int choice;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline character after entering a number

        switch (choice) {
            case 1:
                addContact(&contacts, &count);
                break;
            case 2:
                displayContacts(contacts, count);
                break;
            case 3:
                searchContact(contacts, count);
                break;
            case 4:
                deleteContact(&contacts, &count);
                break;
            case 5:
                saveContactsToFile(contacts, count);
                printf("Contacts saved successfully!\n");
                break;
            case 6:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
                break;
        }
    } while (choice != 6);

    // Free allocated memory
    free(contacts);
    return 0;
}

// Function to display the menu
void displayMenu() {
    printf("\n***** Contact Management System *****\n");
    printf("1. Add Contact\n");
    printf("2. Display Contacts\n");
    printf("3. Search Contact\n");
    printf("4. Delete Contact\n");
    printf("5. Save Contacts\n");
    printf("6. Exit\n");
}

// Function to add a contact
void addContact(Contact **contacts, int *count) {
    Contact newContact;
    printf("Enter name: ");
    fgets(newContact.name, MAX_NAME_LENGTH, stdin);
    newContact.name[strcspn(newContact.name, "\n")] = '\0'; // Remove newline character

    printf("Enter phone number: ");
    fgets(newContact.phone, MAX_PHONE_LENGTH, stdin);
    newContact.phone[strcspn(newContact.phone, "\n")] = '\0';

    printf("Enter email: ");
    fgets(newContact.email, MAX_EMAIL_LENGTH, stdin);
    newContact.email[strcspn(newContact.email, "\n")] = '\0';

    printf("Enter address: ");
    fgets(newContact.address, MAX_ADDRESS_LENGTH, stdin);
    newContact.address[strcspn(newContact.address, "\n")] = '\0';

    // Reallocate memory for new contact
    *contacts = realloc(*contacts, (*count + 1) * sizeof(Contact));
    (*contacts)[*count] = newContact;
    (*count)++;
    printf("Contact added successfully!\n");
}

// Function to display all contacts
void displayContacts(Contact *contacts, int count) {
    if (count == 0) {
        printf("No contacts available.\n");
        return;
    }
    printf("\n***** All Contacts *****\n");
    for (int i = 0; i < count; i++) {
        printf("\nContact %d:\n", i + 1);
        printf("Name: %s\n", contacts[i].name);
        printf("Phone: %s\n", contacts[i].phone);
        printf("Email: %s\n", contacts[i].email);
        printf("Address: %s\n", contacts[i].address);
    }
}

// Function to search for a contact by name
void searchContact(Contact *contacts, int count) {
    if (count == 0) {
        printf("No contacts available.\n");
        return;
    }

    char searchName[MAX_NAME_LENGTH];
    printf("Enter name to search: ");
    fgets(searchName, MAX_NAME_LENGTH, stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(contacts[i].name, searchName) != NULL) {
            printf("\nContact found!\n");
            printf("Name: %s\n", contacts[i].name);
            printf("Phone: %s\n", contacts[i].phone);
            printf("Email: %s\n", contacts[i].email);
            printf("Address: %s\n", contacts[i].address);
            found = 1;
        }
    }
    if (!found) {
        printf("No contact found with the name %s.\n", searchName);
    }
}

// Function to delete a contact
void deleteContact(Contact **contacts, int *count) {
    if (*count == 0) {
        printf("No contacts available.\n");
        return;
    }

    char nameToDelete[MAX_NAME_LENGTH];
    printf("Enter name of contact to delete: ");
    fgets(nameToDelete, MAX_NAME_LENGTH, stdin);
    nameToDelete[strcspn(nameToDelete, "\n")] = '\0';

    int indexToDelete = -1;
    for (int i = 0; i < *count; i++) {
        if (strcmp((*contacts)[i].name, nameToDelete) == 0) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete == -1) {
        printf("No contact found with the name %s.\n", nameToDelete);
        return;
    }

    // Shift contacts after the deleted contact
    for (int i = indexToDelete; i < *count - 1; i++) {
        (*contacts)[i] = (*contacts)[i + 1];
    }

    // Shrink memory
    *contacts = realloc(*contacts, (*count - 1) * sizeof(Contact));
    (*count)--;
    printf("Contact deleted successfully.\n");
}

// Function to save contacts to a file
void saveContactsToFile(Contact *contacts, int count) {
    FILE *file = fopen("contacts.txt", "w");
    if (!file) {
        printf("Error opening file for saving contacts.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", contacts[i].name);
        fprintf(file, "%s\n", contacts[i].phone);
        fprintf(file, "%s\n", contacts[i].email);
        fprintf(file, "%s\n", contacts[i].address);
    }

    fclose(file);
}

// Function to load contacts from a file
int loadContactsFromFile(Contact **contacts) {
    FILE *file = fopen("contacts.txt", "r");
    if (!file) {
        printf("No previous contacts found.\n");
        return 0;
    }

    int count = 0;
    Contact *tempContacts = NULL;

    while (!feof(file)) {
        Contact newContact;
        if (fgets(newContact.name, MAX_NAME_LENGTH, file) && 
            fgets(newContact.phone, MAX_PHONE_LENGTH, file) && 
            fgets(newContact.email, MAX_EMAIL_LENGTH, file) && 
            fgets(newContact.address, MAX_ADDRESS_LENGTH, file)) {

            newContact.name[strcspn(newContact.name, "\n")] = '\0';
            newContact.phone[strcspn(newContact.phone, "\n")] = '\0';
            newContact.email[strcspn(newContact.email, "\n")] = '\0';
            newContact.address[strcspn(newContact.address, "\n")] = '\0';

            tempContacts = realloc(tempContacts, (count + 1) * sizeof(Contact));
            tempContacts[count] = newContact;
            count++;
        }
    }

    fclose(file);
    *contacts = tempContacts;
    return count;
}









#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h> // For atoi()

// Macros to stringify a #define value for use in scanf format string
#define STR(x) #x
#define STRINGIFY(x) STR(x)

#define MAX_DISEASES 6
#define MAX_TABLETS 4
#define MAX_COSMETICS 20 // Max number of suggested cosmetic items
#define MAX_FRACTURE_ITEMS 0 // Placeholder for future, if any fracture items are added
#define MAX_SELECTED_ITEMS 100
#define MAX_USERNAME_LENGTH 49
#define MAX_PASSWORD_LENGTH 49
#define MAX_NAME_LENGTH 49
#define MAX_PHONE_LENGTH 15
#define MAX_DISEASE_NAME_LENGTH 49
#define MAX_MED_NAME_LENGTH 40 // INCREASED THIS VALUE and updated scanf format
#define MAX_DOSAGE_LENGTH 50

// ANSI escape codes for text colors (used ONLY for console/terminal output)
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    char name[MAX_MED_NAME_LENGTH + 1];
    float price;
    char dosage[MAX_DOSAGE_LENGTH + 1]; // Dosage primarily for tablets
} Tablet;

typedef struct {
    char name[MAX_MED_NAME_LENGTH + 1];
    float price;
    char type[20]; // e.g., "Cleanser", "Sunscreen", "Moisturizer"
} CosmeticItem; // New struct for cosmetics with type

const char *disease_names[MAX_DISEASES] = {
    "Headache",
    "Body Ache",
    "Cold & Cough",
    "Fever",
    "Stomach Pain",
    "Allergy"
};

Tablet tablets[MAX_DISEASES][MAX_TABLETS] = {
    {{"Paracetamol", 20.0, "1 strip once a day at night"},
     {"Aspirin", 15.0, "1 strip after food"},
     {"Ibuprofen", 25.0, "1 strip twice a day"},
     {"Diclofenac", 30.0, "1 strip after meals"}},

    {{"Muscle Relaxant", 40.0, "1 strip once a day"},
     {"Ibuprofen", 25.0, "1 strip twice a day"},
     {"Paracetamol", 20.0, "1 strip once a day at night"},
     {"Naproxen", 35.0, "1 strip after food"}},

    {{"Cough Syrup", 50.0, "1 strip thrice a day"},
     {"Antihistamine", 30.0, "1 strip before sleep"},
     {"Paracetamol", 20.0, "1 strip once a day at night"},
     {"Vitamin C", 10.0, "1 strip daily"}},

    {{"Paracetamol", 20.0, "1 strip once a day at night"},
     {"Ibuprofen", 25.0, "1 strip twice a day"},
     {"Aspirin", 15.0, "1 strip after food"},
     {"Naproxen", 35.0, "1 strip after meals"}},

    {{"Antacid", 15.0, "1 strip after meals"},
     {"Omeprazole", 45.0, "1 strip daily"},
     {"Paracetamol", 20.0, "1 strip once a day at night"},
     {"Metronidazole", 40.0, "1 strip twice a day"}},

    {{"Antihistamine", 30.0, "1 strip before sleep"},
     {"Hydrocortisone", 50.0, "1 strip twice a day"},
     {"Loratadine", 25.0, "1 strip once daily"},
     {"Calamine Lotion", 10.0, "Apply externally twice a day"}}
};

// Suggested Cosmetic Items with their types
CosmeticItem cosmetic_items[MAX_COSMETICS] = {
    {"Himalaya Neem Face Wash", 120.0, "Cleanser"},
    {"Cetaphil Gentle Cleanser", 250.0, "Cleanser"},
    {"Minimalist Salicylic Face Cleanser", 299.0, "Cleanser"},
    {"Plum Green Tea Face Wash", 199.0, "Cleanser"},
    {"Neutrogena Deep Clean Cleanser", 280.0, "Cleanser"},
    {"Mamaearth Vitamin C Face Wash", 249.0, "Cleanser"},
    {"Dot & Key Barrier Face Wash", 349.0, "Cleanser"},
    {"Derma Co. Anti-Acne Face Wash", 399.0, "Cleanser"},
    {"Minimalist SPF 50 Sunscreen", 399.0, "Sunscreen"},
    {"Neutrogena Ultra Sheer Sunscreen", 450.0, "Sunscreen"},
    {"Aqualogica Radiance Sunscreen", 389.0, "Sunscreen"},
    {"Dot & Key Vitamin C Sunscreen", 425.0, "Sunscreen"},
    {"Mamaearth Light Indian Sunscreen", 349.0, "Sunscreen"},
    {"Lakme Sun Expert Matte Gel SPF50", 320.0, "Sunscreen"},
    {"Re'equil OMC Free Sunscreen", 695.0, "Sunscreen"},
    {"Dr. Sheth's Ceramide Sunscreen", 499.0, "Sunscreen"},
    {"Cetaphil Moisturizing Cream", 550.0, "Moisturizer"},
    {"Minimalist Vitamin B5 Moisturizer", 349.0, "Moisturizer"},
    {"Plum Green Tea Oil-Free Moisturizer", 299.0, "Moisturizer"},
    {"Dot & Key Hyaluronic Moisturizer", 475.0, "Moisturizer"}
};

// Structure to store selected items for the bill
typedef struct {
    char name[MAX_MED_NAME_LENGTH + 1];
    float price_per_unit;
    int quantity;
    char category[20]; // To store "Tablet", "Cosmetic", "Fracture Related", etc.
} BillItem;

void show_diseases() {
    printf(ANSI_COLOR_CYAN "\nSelect a disease:\n" ANSI_COLOR_RESET);
    for (int i = 0; i < MAX_DISEASES; i++) {
        printf("%d. %s\n", i + 1, disease_names[i]);
    }
}

// Function to display filtered cosmetic items and return a mapping
int show_filtered_cosmetics(const char* type_filter, int *mapping) {
    printf(ANSI_COLOR_CYAN "\nSuggested %s Items:\n" ANSI_COLOR_RESET, type_filter);
    int display_count = 0;
    for (int i = 0; i < MAX_COSMETICS; i++) {
        if (strcmp(cosmetic_items[i].type, type_filter) == 0) {
            printf("%d. %s - Rs. %.2f\n", display_count + 1, cosmetic_items[i].name, cosmetic_items[i].price);
            mapping[display_count] = i; // Store the actual index in cosmetic_items array
            display_count++;
        }
    }
    return display_count; // Return how many items were displayed
}

// Function to display all cosmetic items
void show_all_cosmetics() {
    printf(ANSI_COLOR_CYAN "\nAll Suggested Cosmetic Items:\n" ANSI_COLOR_RESET);
    for (int i = 0; i < MAX_COSMETICS; i++) {
        printf("%d. %s (%s) - Rs. %.2f\n", i + 1, cosmetic_items[i].name, cosmetic_items[i].type, cosmetic_items[i].price);
    }
}


int login() {
    char username[MAX_USERNAME_LENGTH + 1], password[MAX_PASSWORD_LENGTH + 1];
    printf(ANSI_COLOR_YELLOW "Please login to continue\n" ANSI_COLOR_RESET);
    printf("Username: ");
    scanf(" %49[^\n]", username);
    while (getchar() != '\n'); // Clear the input buffer
    printf("Password: ");
    scanf(" %49[^\n]", password);
    while (getchar() != '\n'); // Clear the input buffer

    FILE *f = fopen("login.txt", "r");
    if (!f) {
        printf(ANSI_COLOR_RED "Login file not found. Access denied.\n" ANSI_COLOR_RESET);
        return 0;
    }

    char file_user[MAX_USERNAME_LENGTH + 1], file_pass[MAX_PASSWORD_LENGTH + 1];
    int authenticated = 0;
    while (fscanf(f, "%49s %49s", file_user, file_pass) != EOF) {
        if (strcmp(username, file_user) == 0 && strcmp(password, file_pass) == 0) {
            authenticated = 1;
            break;
        }
    }
    fclose(f);

    if (!authenticated) {
        printf(ANSI_COLOR_RED "Invalid username or password.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_GREEN "Login successful!\n" ANSI_COLOR_RESET);
    }
    return authenticated;
}

float calculate_discount(float total_amount) {
    if (total_amount >= 500.0) {
        return 0.10; // 10% discount for >= 500
    } else if (total_amount >= 200.0) {
        return 0.05; // 5% discount for >= 200
    }
    return 0.0; // No discount
}

int main() {
    if (!login()) {
        return 1; // exit if login fails
    }

    char customer_name[MAX_NAME_LENGTH + 1];
    int age;
    char phone[MAX_PHONE_LENGTH + 1];

    printf(ANSI_COLOR_BLUE "\nWelcome to Medical Store!!!\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "HOPE YOU'RE DOING FINE :)\n" ANSI_COLOR_RESET);

    printf("Please enter your details:\n");
    printf("Name: ");
    scanf(" %49[^\n]", customer_name);
    while (getchar() != '\n'); // Clear the input buffer
    printf("Age: ");
    scanf("%d", &age);
    while (getchar() != '\n'); // Clear the input buffer
    
    // Phone Number Validation Loop
    int valid_phone = 0;
    while (!valid_phone) {
        printf("Phone Number (10 digits): ");
        scanf(" %14[^\n]", phone); // Read up to MAX_PHONE_LENGTH-1 chars
        while (getchar() != '\n'); // Clear buffer

        if (strlen(phone) == 10) {
            // Basic check if all are digits (without ctype.h)
            int all_digits = 1;
            for (int i = 0; i < 10; i++) {
                if (phone[i] < '0' || phone[i] > '9') {
                    all_digits = 0;
                    break;
                }
            }
            if (all_digits) {
                valid_phone = 1;
                printf(ANSI_COLOR_GREEN "Phone number accepted.\n" ANSI_COLOR_RESET);
            } else {
                printf(ANSI_COLOR_RED "Error: Phone number must contain only digits.\n" ANSI_COLOR_RESET);
            }
        } else {
            printf(ANSI_COLOR_RED "Error: Phone number must be exactly 10 digits long.\n" ANSI_COLOR_RESET);
        }
    }


    FILE *userfile = fopen("users.txt", "a");
    if (userfile) {
        fprintf(userfile, "Name: %s, Age: %d, Phone: %s\n", customer_name, age, phone);
        fclose(userfile);
    } else {
        printf(ANSI_COLOR_RED "Warning: Could not save user data!\n" ANSI_COLOR_RESET);
    }

    BillItem purchased_items[MAX_SELECTED_ITEMS];
    int item_count = 0;
    float overall_total_pre_discount = 0;
    char diseases_purchased[200] = ""; // Only for disease-specific tablets

    int overall_continue_shopping = 1;
    while (overall_continue_shopping) {
        printf(ANSI_COLOR_CYAN "\nSelect a Product Category:\n" ANSI_COLOR_RESET);
        printf("1. TABLETS (Disease-related medicines)\n");
        printf("2. COSMETICS\n");
        printf("3. FRACTURE RELATED Items\n");
        printf("0. Finish Shopping and Generate Bill\n");

        int category_choice;
        printf(ANSI_COLOR_YELLOW "Enter category choice (0-3): " ANSI_COLOR_RESET);
        scanf("%d", &category_choice);
        while (getchar() != '\n'); // Clear the input buffer

        if (category_choice == 0) {
            overall_continue_shopping = 0;
            break; // Exit overall shopping loop
        }

        switch (category_choice) {
            case 1: { // TABLETS (Existing disease logic)
                int continue_shopping_tablets = 1;
                while (continue_shopping_tablets) {
                    show_diseases();
                    printf("%d. Other (Enter your own disease)\n", MAX_DISEASES + 1);
                    printf("0. Finish selecting TABLETS for this bill\n"); // Changed message

                    int choice;
                    printf(ANSI_COLOR_YELLOW "Enter disease choice (0-%d): " ANSI_COLOR_RESET, MAX_DISEASES + 1);
                    scanf("%d", &choice);
                    while (getchar() != '\n'); // Clear the input buffer

                    int disease_index = -1;
                    char current_disease[MAX_DISEASE_NAME_LENGTH + 1];
                    
                    if (choice == 0) {
                        continue_shopping_tablets = 0;
                        break; // Exit tablet selection for current bill
                    }
                    
                    if (choice >= 1 && choice <= MAX_DISEASES) {
                        disease_index = choice - 1;
                        strcpy(current_disease, disease_names[disease_index]);
                    } else if (choice == MAX_DISEASES + 1) {
                        printf("Enter your disease name: ");
                        scanf(" %49[^\n]", current_disease);
                        while (getchar() != '\n'); // Clear the input buffer
                    } else {
                        printf(ANSI_COLOR_RED "Invalid selection. Please try again.\n" ANSI_COLOR_RESET);
                        continue;
                    }

                    // Only append to diseases_purchased if it's a tablet for a disease
                    if (strlen(diseases_purchased) == 0) {
                        strcpy(diseases_purchased, current_disease);
                    } else {
                        strcat(diseases_purchased, ", ");
                        strcat(diseases_purchased, current_disease);
                    }

                    printf(ANSI_COLOR_GREEN "\nSuggested Tablets for %s:\n" ANSI_COLOR_RESET, current_disease);
                    
                    int med_num_counter = 1;
                    int selection_map[MAX_DISEASES * MAX_TABLETS + 1][2]; 

                    if (disease_index != -1) { // Pre-defined disease: show specific tablets
                        for (int i = 0; i < MAX_TABLETS; i++) {
                            Tablet t = tablets[disease_index][i];
                            printf("%d. %s - Rs. %.2f - Dosage: %s\n", i + 1, t.name, t.price, t.dosage);
                        }
                    } else { // Custom disease: User adds their own medicines
                        printf("\nSince you chose a custom disease, please add the medicines you need.\n");
                        printf("You can also select from the list of ALL available medicines below:\n");
                        for (int i = 0; i < MAX_DISEASES; i++) {
                            for (int j = 0; j < MAX_TABLETS; j++) {
                                printf("%d. %s - Rs. %.2f - Dosage: %s\n", med_num_counter, tablets[i][j].name, tablets[i][j].price, tablets[i][j].dosage);
                                selection_map[med_num_counter][0] = i;
                                selection_map[med_num_counter][1] = j;
                                med_num_counter++;
                            }
                        }
                    }

                    int sel;
                    printf(ANSI_COLOR_YELLOW "\nSelect tablets by number (0 to finish selection for this disease), or enter a new medicine (e.g., 'new'):\n" ANSI_COLOR_RESET);
                    while (1) {
                        char input_buffer[10];
                        printf("Enter tablet number or 'new': ");
                        scanf(" %s", input_buffer);
                        while (getchar() != '\n'); // Clear the input buffer

                        if (strcmp(input_buffer, "0") == 0) {
                            break;
                        } else if (strcmp(input_buffer, "new") == 0 || strcmp(input_buffer, "NEW") == 0) {
                            if (item_count >= MAX_SELECTED_ITEMS) {
                                printf(ANSI_COLOR_RED "Maximum number of items reached. Cannot add more custom medicines.\n" ANSI_COLOR_RESET);
                                continue;
                            }
                            strcpy(purchased_items[item_count].category, "Tablet"); // Mark as Tablet
                            printf("Enter new medicine name: ");
                            // Corrected scanf format using STRINGIFY
                            scanf(" %" STRINGIFY(MAX_MED_NAME_LENGTH) "[^\n]", purchased_items[item_count].name);
                            while (getchar() != '\n'); // Clear the input buffer

                            printf("Enter price per strip for %s: Rs. ", purchased_items[item_count].name);
                            scanf("%f", &purchased_items[item_count].price_per_unit);
                            while (getchar() != '\n'); // Clear the input buffer

                            printf("Enter quantity of strips for %s: ", purchased_items[item_count].name);
                            scanf("%d", &purchased_items[item_count].quantity);
                            while (getchar() != '\n'); // Clear the input buffer

                            if (purchased_items[item_count].quantity <= 0) {
                                printf(ANSI_COLOR_RED "Quantity must be positive. Skipping this custom item.\n" ANSI_COLOR_RESET);
                                continue;
                            }
                            overall_total_pre_discount += (purchased_items[item_count].price_per_unit * purchased_items[item_count].quantity);
                            printf(ANSI_COLOR_GREEN "Added %s to your list.\n" ANSI_COLOR_RESET, purchased_items[item_count].name);
                            item_count++;
                            continue;
                        }
                        
                        sel = atoi(input_buffer);

                        Tablet selected_tablet;
                        int valid_selection = 0;

                        if (disease_index != -1) {
                            if (sel >= 1 && sel <= MAX_TABLETS) {
                                selected_tablet = tablets[disease_index][sel - 1];
                                valid_selection = 1;
                            }
                        } else {
                            if (sel >= 1 && sel < med_num_counter) {
                                selected_tablet = tablets[selection_map[sel][0]][selection_map[sel][1]];
                                valid_selection = 1;
                            }
                        }

                        if (!valid_selection) {
                            printf(ANSI_COLOR_RED "Invalid selection. Please enter a valid number or 'new'.\n" ANSI_COLOR_RESET);
                            continue;
                        }

                        if (item_count >= MAX_SELECTED_ITEMS) {
                            printf(ANSI_COLOR_RED "Maximum number of items reached. Cannot add more.\n" ANSI_COLOR_RESET);
                            break;
                        }
                        
                        int qty;
                        printf("Enter quantity of strips for %s: ", selected_tablet.name);
                        scanf("%d", &qty);
                        while (getchar() != '\n'); // Clear the input buffer

                        if (qty <= 0) {
                            printf(ANSI_COLOR_RED "Quantity must be positive. Skipping this item.\n" ANSI_COLOR_RESET);
                            continue;
                        }

                        strcpy(purchased_items[item_count].name, selected_tablet.name);
                        purchased_items[item_count].price_per_unit = selected_tablet.price;
                        purchased_items[item_count].quantity = qty;
                        strcpy(purchased_items[item_count].category, "Tablet"); // Mark as Tablet
                        overall_total_pre_discount += (selected_tablet.price * qty);
                        printf(ANSI_COLOR_GREEN "Added %s to your list.\n" ANSI_COLOR_RESET, purchased_items[item_count].name);
                        item_count++;
                    }
                    printf(ANSI_COLOR_BLUE "\n--- Done selecting TABLETS for %s. ---\n" ANSI_COLOR_RESET, current_disease);
                } // End of tablet selection loop
                printf(ANSI_COLOR_BLUE "\n--- Finished TABLETS selection. ---\n" ANSI_COLOR_RESET);
                break;
            } // End case 1

            case 2: { // COSMETICS
                printf(ANSI_COLOR_YELLOW "\n--- COSMETICS Selection ---\n" ANSI_COLOR_RESET);
                int add_more_cosmetics = 1;
                while(add_more_cosmetics) {
                    if (item_count >= MAX_SELECTED_ITEMS) {
                        printf(ANSI_COLOR_RED "Maximum number of items reached. Cannot add more items.\n" ANSI_COLOR_RESET);
                        break;
                    }
                    
                    printf(ANSI_COLOR_CYAN "\nWhat type of cosmetic are you looking for?\n" ANSI_COLOR_RESET);
                    printf("1. Cleanser\n");
                    printf("2. Sunscreen\n");
                    printf("3. Moisturizer\n");
                    printf("4. All Cosmetics (view all types)\n");
                    printf("0. Finish selecting COSMETICS\n");
                    printf("Enter your choice: ");

                    int cosmetic_type_choice;
                    scanf("%d", &cosmetic_type_choice);
                    while (getchar() != '\n'); // Clear the input buffer

                    if (cosmetic_type_choice == 0) {
                        add_more_cosmetics = 0;
                        break;
                    }

                    int cosmetic_map[MAX_COSMETICS]; // To map displayed index to actual item index
                    int num_displayed_cosmetics = 0;
                    char current_cosmetic_type[20] = "";

                    switch (cosmetic_type_choice) {
                        case 1: strcpy(current_cosmetic_type, "Cleanser"); break;
                        case 2: strcpy(current_cosmetic_type, "Sunscreen"); break;
                        case 3: strcpy(current_cosmetic_type, "Moisturizer"); break;
                        case 4: strcpy(current_cosmetic_type, "All"); break; // Indicates show all
                        default:
                            printf(ANSI_COLOR_RED "Invalid cosmetic type choice. Please try again.\n" ANSI_COLOR_RESET);
                            continue;
                    }
                    
                    if (strcmp(current_cosmetic_type, "All") == 0) {
                        show_all_cosmetics();
                        // For "All", the mapping is 1-to-1 with actual index
                        for (int i = 0; i < MAX_COSMETICS; i++) {
                            cosmetic_map[i] = i;
                        }
                        num_displayed_cosmetics = MAX_COSMETICS;
                    } else {
                        num_displayed_cosmetics = show_filtered_cosmetics(current_cosmetic_type, cosmetic_map);
                    }

                    printf("%d. Enter a new/custom cosmetic item\n", num_displayed_cosmetics + 1); // Option for custom
                    printf("0. Back to main cosmetics menu (or finish selecting)\n"); // Back to sub-category menu

                    int cosmetic_sel;
                    printf(ANSI_COLOR_YELLOW "Enter cosmetic item number (0 to go back, %d for new): " ANSI_COLOR_RESET, num_displayed_cosmetics + 1);
                    scanf("%d", &cosmetic_sel);
                    while (getchar() != '\n'); // Clear the input buffer

                    if (cosmetic_sel == 0) {
                        // Go back to the cosmetic type selection, don't exit overall cosmetic loop
                        continue;
                    } else if (cosmetic_sel >= 1 && cosmetic_sel <= num_displayed_cosmetics) {
                        // User selected a suggested cosmetic (filtered or all)
                        int actual_index = cosmetic_map[cosmetic_sel - 1]; // Use the mapping
                        strcpy(purchased_items[item_count].category, "Cosmetic");
                        strcpy(purchased_items[item_count].name, cosmetic_items[actual_index].name);
                        purchased_items[item_count].price_per_unit = cosmetic_items[actual_index].price;
                        
                        printf("Enter quantity for %s: ", purchased_items[item_count].name);
                        scanf("%d", &purchased_items[item_count].quantity);
                        while (getchar() != '\n'); // Clear the input buffer

                        if (purchased_items[item_count].quantity <= 0) {
                            printf(ANSI_COLOR_RED "Quantity must be positive. Skipping this item.\n" ANSI_COLOR_RESET);
                            continue;
                        }
                        overall_total_pre_discount += (purchased_items[item_count].price_per_unit * purchased_items[item_count].quantity);
                        printf(ANSI_COLOR_GREEN "Added %s to your list.\n" ANSI_COLOR_RESET, purchased_items[item_count].name);
                        item_count++;

                    } else if (cosmetic_sel == num_displayed_cosmetics + 1) {
                        // User wants to enter a new cosmetic item
                        strcpy(purchased_items[item_count].category, "Cosmetic");
                        printf("Enter new cosmetic item name: ");
                        // Corrected scanf format using STRINGIFY
                        scanf(" %" STRINGIFY(MAX_MED_NAME_LENGTH) "[^\n]", purchased_items[item_count].name);
                        while (getchar() != '\n'); // Clear the input buffer

                        printf("Enter price per unit for %s: Rs. ", purchased_items[item_count].name);
                        scanf("%f", &purchased_items[item_count].price_per_unit);
                        while (getchar() != '\n'); // Clear the input buffer

                        printf("Enter quantity for %s: ", purchased_items[item_count].name);
                        scanf("%d", &purchased_items[item_count].quantity);
                        while (getchar() != '\n'); // Clear the input buffer

                        if (purchased_items[item_count].quantity <= 0) {
                            printf(ANSI_COLOR_RED "Quantity must be positive. Skipping this item.\n" ANSI_COLOR_RESET);
                            continue;
                        }
                        overall_total_pre_discount += (purchased_items[item_count].price_per_unit * purchased_items[item_count].quantity);
                        printf(ANSI_COLOR_GREEN "Added %s to your list.\n" ANSI_COLOR_RESET, purchased_items[item_count].name);
                        item_count++;
                    } else {
                        printf(ANSI_COLOR_RED "Invalid selection. Please try again.\n" ANSI_COLOR_RESET);
                        continue;
                    }
                }
                printf(ANSI_COLOR_BLUE "\n--- Finished COSMETICS selection. ---\n" ANSI_COLOR_RESET);
                break;
            }

            case 3: { // FRACTURE RELATED Items
                printf(ANSI_COLOR_YELLOW "\n--- FRACTURE RELATED Items Selection (Enter '0' for item name to finish) ---\n" ANSI_COLOR_RESET);
                int add_more_fracture = 1;
                while(add_more_fracture) {
                    if (item_count >= MAX_SELECTED_ITEMS) {
                        printf(ANSI_COLOR_RED "Maximum number of items reached. Cannot add more items.\n" ANSI_COLOR_RESET);
                        break;
                    }
                    
                    strcpy(purchased_items[item_count].category, "Fracture Related"); // Mark category
                    printf("Enter Fracture Related item name: ");
                    // Corrected scanf format using STRINGIFY
                    scanf(" %" STRINGIFY(MAX_MED_NAME_LENGTH) "[^\n]", purchased_items[item_count].name);
                    while (getchar() != '\n'); // Clear the input buffer

                    if (strcmp(purchased_items[item_count].name, "0") == 0) {
                        add_more_fracture = 0;
                        // No need to decrement item_count here if '0' is the break condition
                        break;
                    }

                    printf("Enter price per unit for %s: Rs. ", purchased_items[item_count].name);
                    scanf("%f", &purchased_items[item_count].price_per_unit);
                    while (getchar() != '\n'); // Clear the input buffer

                    printf("Enter quantity for %s: ", purchased_items[item_count].name);
                    scanf("%d", &purchased_items[item_count].quantity);
                    while (getchar() != '\n'); // Clear the input buffer

                    if (purchased_items[item_count].quantity <= 0) {
                        printf(ANSI_COLOR_RED "Quantity must be positive. Skipping this item.\n" ANSI_COLOR_RESET);
                        continue;
                    }
                    overall_total_pre_discount += (purchased_items[item_count].price_per_unit * purchased_items[item_count].quantity);
                    printf(ANSI_COLOR_GREEN "Added %s to your list.\n" ANSI_COLOR_RESET, purchased_items[item_count].name);
                    item_count++;
                }
                printf(ANSI_COLOR_BLUE "\n--- Finished FRACTURE RELATED items selection. ---\n" ANSI_COLOR_RESET);
                break;
            }

            default:
                printf(ANSI_COLOR_RED "Invalid category choice. Please try again.\n" ANSI_COLOR_RESET);
                break;
        }
    }

    // Prompt to type 'finish' before generating the bill
    char finish_cmd[10];
    printf(ANSI_COLOR_YELLOW "\nShopping finished. Type 'finish' to generate your bill and exit: " ANSI_COLOR_RESET);
    scanf(" %9[^\n]", finish_cmd); // Read "finish"
    while (getchar() != '\n'); // Clear the input buffer


    // Generate Bill (NO color codes, original formatting for bill.txt)
    FILE *bill = fopen("bill.txt", "w");
    if (!bill) {
        printf(ANSI_COLOR_RED "Error writing bill!\n" ANSI_COLOR_RESET);
        return 1;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[50];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    float discount_percentage = calculate_discount(overall_total_pre_discount);
    float discount_amount = overall_total_pre_discount * discount_percentage;
    float final_total = overall_total_pre_discount - discount_amount;

    fprintf(bill, "--------------------------------------------------------------------------------\n"); // Increased width
    fprintf(bill, "                            MEDICAL STORE BILL\n");
    fprintf(bill, "--------------------------------------------------------------------------------\n");
    fprintf(bill, "Customer Name : %s\n", customer_name);
    // Print diseases only if any tablets were purchased for diseases
    if (strlen(diseases_purchased) > 0) {
        fprintf(bill, "Disease       : %s\n", diseases_purchased);
    }
    fprintf(bill, "Date & Time   : %s\n\n", time_str);
    fprintf(bill, "Items Purchased:\n"); // Changed from "Tablets Purchased"
    fprintf(bill, "--------------------------------------------------------------------------------\n"); // Increased width
    fprintf(bill, "| %-20s | %-25s | %-3s | %-10s | %-10s |\n", "Category", "Item Name", "Qty", "Price/Unit", "Subtotal"); // Adjusted widths
    fprintf(bill, "--------------------------------------------------------------------------------\n");

    for (int i = 0; i < item_count; i++) {
        float subtotal = purchased_items[i].price_per_unit * purchased_items[i].quantity;
        fprintf(bill, "| %-20s | %-25s | %-3d | Rs. %-5.2f | Rs. %-7.2f |\n", // Adjusted widths
                purchased_items[i].category, purchased_items[i].name, purchased_items[i].quantity, 
                purchased_items[i].price_per_unit, subtotal);
    }
    fprintf(bill, "--------------------------------------------------------------------------------\n");
    fprintf(bill, "Subtotal                     : Rs. %.2f\n", overall_total_pre_discount);
    fprintf(bill, "Discount (%.0f%%)               : - Rs. %.2f\n", discount_percentage * 100, discount_amount);
    fprintf(bill, "Total Amount Payable         : Rs. %.2f\n", final_total);
    fprintf(bill, "--------------------------------------------------------------------------------\n");
    fprintf(bill, "Thank you for your visit!\n");
    fclose(bill);

    printf(ANSI_COLOR_GREEN "\nBill generated in 'bill.txt'.\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_MAGENTA "Thank you for your purchase, %s!\n" ANSI_COLOR_RESET, customer_name);
    printf(ANSI_COLOR_BLUE "Exiting Medical Store Application.\n" ANSI_COLOR_RESET);

    return 0;
}
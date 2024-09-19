/***************************************************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts learned. I
have constructed the functions and their respective algorithms and corresponding code by myself. The program was run, tested,
and debugged by my own efforts. I further certify that I have not copied in part or whole or otherwise plagiarized the work of other
students and/or persons.
                                                                                      <Ang, Janell D.>, DLSU ID# <12205567>
                                                                                      <Dimla, Kian C.>, DLSU ID# <12206865>
****************************************************************************************************************************************/

#include <stdio.h>
#include <string.h>

typedef char string10[11];
typedef char string20[21];
typedef char string50[51];

struct userTag
{
    int       userID;
    long long contact;
    string20  password;
    string50  address;
    string20  name;
};

struct itemTag
{
    int      productID,
             quantity,
             userID;
    float    unitprice;
    string20 iname;
    string20 category;
    string50 idescription;
};

struct cartTag
{
    int productID[10],
        quantity[10],
        userID;
};

struct dateTag
{
    int month,
        day,
        year;
};

struct transactionTag
{
    struct itemTag item[5];
};

struct checkOutTag
{
    struct dateTag        date;
    struct transactionTag transaction[10];
    struct userTag        user;
};

struct transPrint
{
    int            userID,
                   sellerID[5];
    struct dateTag date;
    float          amountDue[5];            
};

int initUser(struct userTag *user, FILE *users);
int initItem(struct itemTag *item, FILE *items); 
int initCart(struct cartTag *cart, FILE *carts);
int checkUserID(int key, struct userTag *user, int n);
int checkItemID(int key, struct itemTag *item, int numItems);
int checkPassword(string20 key, struct userTag *user, int n);
int login(struct userTag *user, int n);
void fprintItem(struct itemTag *item, int numItems, FILE *items);
void fprintCart(struct cartTag *cart, int numCarts, FILE *carts);
int regUser(struct userTag *user, int n, FILE *users); 
int userMenu(struct userTag *user, int n, int loginID, struct itemTag *item, int numItems, FILE *items);
int sellMenu(int loginID, struct itemTag *item, int numItems, FILE *items);
int addNewItem(int loginID, struct itemTag *item, int numItems, FILE *items);
void editStock(int loginID, struct itemTag *item, int numItems, FILE *items);
void showMyProducts(int loginID, struct itemTag *item, int numItems);
void showLowStock(int loginID, struct itemTag *item, int numItems);
void buyMenu(struct userTag *user, int n, int loginID ,struct itemTag *item, int numItems, FILE *items);
void viewAllProducts(int loginID ,struct itemTag *item, int numItems);
void showSpecificSeller(int loginID ,struct itemTag *item, int numItems);
void searchByCategory(int loginID ,struct itemTag *item, int numItems);
void searchByName(int loginID ,struct itemTag *item, int numItems);
int addToCart(struct userTag *user, int n, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items);
void editCart(struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems);
void removeFromSeller(struct cartTag *cart, int numCarts, FILE *carts, int cartIndex, struct itemTag *item, int numItems);
void removeSpecific(struct cartTag *cart, int numCarts, FILE *carts, int cartIndex, struct itemTag *item, int numItems);
void editQuantity(struct cartTag *cart, int numCarts, FILE *carts, int cartIndex, struct itemTag *item, int numItems);
int fscanOld(struct itemTag *old, FILE *oldFile);
void checkOutMenu(struct userTag *user, int n, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items);
void checkOutAll(struct userTag *user, int n, struct checkOutTag *transaction, int cartIndex, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items, FILE *transactionFile);
void checkOutSeller(struct userTag *user, int n, struct checkOutTag *transaction, int cartIndex, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items, FILE *transactions);
void checkOutItem(struct userTag *user, int n, struct checkOutTag *transaction, int cartIndex, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items, FILE *transactions);
void adminMenu(struct userTag *user, int n, struct itemTag *item, int numItems);
void showAllUsers(struct userTag *user, int n);
void showAllSellers(struct userTag *user, int n, struct itemTag *item, int numItems);
int initTransactions(struct transPrint *temp);
void inputDate(struct dateTag *date);
int isBetween(struct dateTag *date, struct dateTag *dateTrans);
void showTotalSales(struct transPrint *temp, int numCheckOuts, struct dateTag *date);
void showSellerSales(struct transPrint *temp, int numCheckOuts, struct dateTag *date, struct userTag *user, int n);
void showShopaholics(struct transPrint *temp, int numCheckOuts, struct dateTag *date, struct userTag *user, int n);

int 
main()
{
    FILE *users,
         *items;
    int n, // # of users
        numItems, // # of items
        menu, 
        loop = 1;
    struct userTag user[100];
    struct itemTag item[100];
    int loginID;
    printf("Welcome to DaShop\n");
    users = fopen("users.txt", "r");
    n = initUser(user, users); // Number of users
    items = fopen("items.txt", "r");
    numItems = initItem(item, items); // Number of items
    while(menu && loop) // Loop for invalid inputs
    {
        printf("(0) Exit\n");
        printf("(1) Log In\n");
        printf("(2) Register\n");
        printf("(3) Admin Menu\n");
        printf("Choose an option: ");
        scanf("%d", &menu);
        printf("\n");
        if(menu < 0 || menu > 3)
            printf("Invalid input.\n");
        fflush(stdin);
        switch(menu)
        {
        case 1:
            loginID = login(user, n);
            if(loginID != -1)
                numItems = userMenu(user, n, loginID, item, numItems, items);
            break;
        
        case 2:
            n = regUser(user, n, users);
            break;
        
        case 3:
            adminMenu(user, n, item, numItems);
            break;
        }
    }
    
    if(!menu)
        printf("Thank you for shopping at DaShop!");

    return 0;
}

/* initUsers initializes an array of userTag structures by reading the contents of the file "users.txt". If the file exists, it opens the file in read mode and reads the data into the array. If the file does not exist, the code creates a new file in write mode. The function returns the number of users read from the file.
	@param user - a pointer to the userTag structure.
	@param users - a pointer  that is used to read the data from the  "users.txt" file.
	@returns an integer, which is the number of users read from the file.
	Pre-condition: There exists a file named "users.txt" in the current directory of the program.
*/

int 
initUser(struct userTag *user, FILE *users) //Reads contents of users.txt and stores into array
{
    int i = 0,
        n = 0;
    
    if(users == NULL) // Creates file if file doesn't exist
    {
        users = fopen("users.txt", "w");
        
        if(users == NULL)
        {
            printf("Unable to create file\n");
            i = 1;
        }
        else 
    	{
        	users = fopen("users.txt", "r");
        	i = 0;
    	}
    }
    	
    while(!i) // Reads users.txt and assigns to an array
    {
        fscanf(users, "%d %[^\n]", &user[n].userID, user[n].password);
        fscanf(users, " %[^\n]", user[n].name);
        fscanf(users, " %[^\n]", user[n].address);
        fscanf(users, "%lld", &user[n].contact);
        if(feof(users)) //If end of file is reached, end the loop
            i = 1;
        n++;
    }
 
    fclose(users); //Close file

    return n;
}

/* initItems initializes an array of itemTag structures by reading the contents of the file "items.txt". If the file exists, it opens the file in read mode and reads the data into the array. If the file does not exist, the code creates a new file in write mode. The function returns the number of items read from the file.
	@param item - a pointer to the itemTag structure.
	@param items - a pointer  that is used to read the data from the  "items.txt" file.
	@returns an integer, which is the number of items read from the file.
	Pre-condition: There exists a file named "items.txt" in the current directory of the program.
*/

int 
initItem(struct itemTag *item, FILE *items) //Reads contents of items.txt and stores into array
{
    int i = 0,
        n = 0;
    
    if(items == NULL) // If items.txt file doesn't exist
    {
        items = fopen("items.txt", "w");
        
        if(items == NULL)
        {
            printf("Unable to create file\n");
            i = 1;
        }
        else 
        {
			items = fopen("items.txt", "r");
        	i = 0;
    	}
    }
    
    while(!i) // Reads items.txt and assigns to an array
    {
        
        fscanf(items, "%d %d\n", &item[n].productID, &item[n].userID);
        fscanf(items, " %[^\n]", item[n].iname);
        fscanf(items, " %[^\n]", item[n].category);
        fscanf(items, " %[^\n]", item[n].idescription);
        fscanf(items, "%d %f\n", &item[n].quantity, &item[n].unitprice);
        if(feof(items)) //If end of file is reached, end the loop
        i = 1;
        n++;
    }

    fclose(items);

    return n;
}

/* initCart initializes an array of cartTag structures by reading the contents of the file "carts.txt". If the file exists, it opens the file in read mode and reads the data into the array. If the file does not exist, the code creates a new file in write mode. The function returns the number of carts read from the file.
	@param cart - a pointer to the cartTag structure.
	@param carts - a pointer  that is used to read the data from the  "carts.txt" file.
	@returns an integer, which is the number of carts read from the file.
	Pre-condition: There exists a file named "carts.txt" in the current directory of the program.
*/

int 
initCart(struct cartTag *cart, FILE *carts) //Reads contents of carts.txt and stores into array
{
    int i = 0,
        j,
        n = 0;
    	
    if(carts == NULL) // If carts.txt file doesn't exist
    {
        carts = fopen("carts.txt", "w");
        
        if(carts == NULL)
        {
            printf("Unable to create file\n");
            i = 1;
        }
        else 
        {
        	carts = fopen("carts.txt", "r");
			i = 0;
    	}
    }
    
    while(!i) // Read until end of file
    {
        fscanf(carts, "%d", &cart[n].userID);
        for(j = 0; j < 10; j++)
            fscanf(carts, "%d ", &cart[n].productID[j]);

        for(j = 0; j < 10; j++)
            fscanf(carts, "%d ", &cart[n].quantity[j]);

        if(feof(carts))
            i++; // End loop
        n++;
    }
    fclose(carts);

    return n;
}

/* checkUserID searches for a specific userID in an array of userTag structures.. The function returns the index of the userTag structure containing the matching userID if found, and returns -1 if the userID is not found in the array.
	@param key - an integer representing the userID to search for.
	@param user - a pointer to the array of userTag structures to search.
	@param n - an integer representing the number of elements in the user array.
	
	@returns an integer representing the index of the userTag structure containing the matching userID if found, and returns -1 if the userID is not found in the array.
	Pre-condition: The user array contains at least one element, and each element has a valid userID. The key parameter is a valid userID.
*/

int 
checkUserID(int key, struct userTag *user, int n) // Searches for userID with key
{
    int index = -1, // Index of userID
        i = 0;

    while(index == -1 && i < n)
    {
        if(key == user[i].userID)
            index = i;
        else 
            i++;
    }
    return index;
}

/* checkItemID is a function that searches for an itemID with a given key in an array of itemTag structures. The function returns the index of the itemTag structure that matches the key, or -1 if no match is found.
	@param key - an integer representing the itemID key to search for.
	@param item - a pointer to an array of itemTag structures.
	@param numItems - an integer representing the number of elements in the item array.
	@returns an integer, which is the index of the itemTag structure that matches the key, or -1 if no match is found.
	Pre-condition: The item array is not NULL and contains at least one element with a valid productID.
*/

int 
checkItemID(int key, struct itemTag *item, int numItems) // Searches for itemID with key
{
    int index = -1, // Index of itemID
        i = 0;

    while(index == -1 && i < numItems)
    {
        if(key == item[i].productID)
            index = i;
        else 
            i++;
    }
    return index;
}

/*  checkPassword is a function that searches for a password with a given key in an array of userTag structures. The function returns the index of the userTag structure that matches the key, or -1 if no match is found.
	@param key - a string representing the password key to search for.
	@param user - a pointer to an array of userTag structures.
	@param n - an integer representing the number of elements in the user array.
	@returns an integer, which is the index of the userTag structure that matches the key, or -1 if no match is found.
	Pre-condition: The user array is not NULL and contains at least one element with a valid password.
*/

int 
checkPassword(string20 key, struct userTag *user, int n) // Checks for password with key
{
    int index = -1, // Index of user's password
        i = 0;

    while(index == -1 && i < n)
    {
        if(strcmp(key, user[i].password) == 0)
            index = i;
        else 
            i++;
    }
    return index;
}

/* login is a function that allows the user to enter their user ID and password, and then checks if the entered values match with the userTag structures in an array of userTag structures. The function uses checkUserID and checkPassword functions to validate the user ID and password. If both checks pass, the function returns the user ID entered by the user, otherwise it returns -1.
	@param user - a pointer to an array of userTag structures.
	@param n - an integer representing the number of elements in the user array.
	@returns an integer, which is the user ID entered by the user if the login is successful, or -1 if the login fails.
	Pre-condition: user ID is an integer and the password is a string of characters with less than or equal to 20 characters.
*/

int 
login(struct userTag *user, int n) // Login page
{
    int tempID,
        nReturn;
    string20 tempPassword;

    printf("<Login>\n");
    printf("Enter User ID: ");
    scanf("%d", &tempID);
    printf("Enter password: ");
    fflush(stdin);
    scanf("%[^\n]", tempPassword);
    if(checkUserID(tempID, user, n) != -1 && checkPassword(tempPassword, user, n) == checkUserID(tempID, user, n)) // Password check
    {
        printf("<Logged in successfully!>\n\n");
        nReturn = tempID; // Return the loginID
    } 
    else
    {
        printf("<User ID and password do not match!>\n\n");
        nReturn = -1;
    }
    return nReturn;
}

/* fprintItem is a function that updates the contents of a file named "items.txt" by writing the values of an array of itemTag structures to the file. 
	@param item - a pointer to an array of itemTag structures.
	@param numItems - an integer representing the number of elements in the item array.
	@param items - a pointer to a FILE structure that represents the file "items.txt".
	@returns nothing.
	Pre-condition: The item array is not NULL and contains at least one valid element. The file "items.txt" exists.
*/

void 
fprintItem(struct itemTag *item, int numItems, FILE *items)
{
    int i;
    items = fopen("items.txt", "w");
    for(i = 0; i < numItems; i++) // Writes item[] values to items.txt file
    {
        fprintf(items, "%d %d\n", item[i].productID, item[i].userID);
        fprintf(items, "%s\n", item[i].iname);
        fprintf(items, "%s\n", item[i].category);
        fprintf(items, "%s\n", item[i].idescription);
        fprintf(items, "%d %.2f\n\n", item[i].quantity, item[i].unitprice);
    }
    fclose(items);
}

/* fprintCart is a function that updates the contents of a file named "carts.txt" by writing the values of an array of cartTag structures to the file.
	@param cart - a pointer to an array of cartTag structures.
	@param numCarts - an integer representing the number of elements in the cart array.
	@param carts - a pointer to a FILE structure that represents the file "carts.txt".
	@returns nothing.
	Pre-condition: The cart array is not NULL and contains at least one valid element. The file "carts.txt" exists.
*/

void 
fprintCart(struct cartTag *cart, int numCarts, FILE *carts) // Rewrites file with updated cart values
{
    int i,
        j;
    carts = fopen("carts.txt", "w"); // Print cart values to carts.txt file
    for(i = 0; i < numCarts; i++)
    {
        fprintf(carts, "%d\n", cart[i].userID);

        for(j = 0; j < 10; j++)
            fprintf(carts, "%d ", cart[i].productID[j]);

        fprintf(carts, "\n");

        for(j = 0; j < 10; j++)
            fprintf(carts, "%d ", cart[i].quantity[j]);

        fprintf(carts, "\n\n");
    }
    fclose(carts);
}

/* regUser is a function that registers a new user by taking input from the user and writing it to the "users.txt" file. The function writes the entered user details to the "users.txt" file and returns the number of registered users.
	@param user - a pointer to the userTag structure.
	@param n - an integer that represents the number of users already registered.
	@param users - a pointer to the file "users.txt.”
	@returns an integer, which is the number of registered users.
	Pre-condition: users.txt exists in the current directory of the program and user ID is an integer and the password is a string of characters with less than or equal to 20 characters.
*/

int 
regUser(struct userTag *user, int n, FILE *users)
{
    int i = 0,
        temp;
    printf("<Register>\n");
    while(i == 0) // Loop for valid userID
    {
        printf("Enter User ID: ");
        scanf("%d", &user[n].userID);
        temp = user[n].userID;
        if(checkUserID(temp, user, n) == -1) //If check passes, function ends
            i = 1;
        else
        printf("User ID already exists.\n");
    }
    printf("Enter password: ");
    fflush(stdin);
    scanf("%[^\n]", user[n].password);
    printf("Enter name: ");
    fflush(stdin);
    scanf("%[^\n]%*c", user[n].name);
    printf("Enter address: ");
    fflush(stdin);
    scanf("%[^\n]%*c", user[n].address);
    printf("Enter contact: ");
    fflush(stdin);
    scanf("%lld", &user[n].contact);
    printf("<Registered successfully!>\n\n");

    users = fopen("users.txt", "a"); // Adds user info to the text file
    fprintf(users, "%d ", user[n].userID);
    fprintf(users, "%s\n", user[n].password);
    fprintf(users, "%s\n", user[n].name);
    fprintf(users, "%s\n", user[n].address);
    fprintf(users, "%lld", user[n].contact);
    fprintf(users, "\n\n");
    fclose(users);

    return n + 1;
}

/* userMenu is a function that displays a menu of options for a logged-in user and calls other functions based on the user's choice. The function loops until the user chooses to log out.
	@param user - a pointer to the userTag structure.
	@param n - an integer that represents the number of users already registered.
	@param loginID - an integer that represents the ID of the logged-in user.
	@param item - a pointer to the itemTag structure.
	@param numItems - an integer that represents the number of items already registered.
	@param items - a pointer to the file "items.txt.”
	@returns an integer, which is the number of items in the inventory after the user has finished using the menu.
	Pre-condition: user input is an integer.
*/

int 
userMenu(struct userTag *user, int n, int loginID, struct itemTag *item, int numItems, FILE *items)
{
    int usermenu,
        loop = 1;

    while(usermenu && loop) // Loop for invalid inputs
    {
        printf("(0) Log Out\n");
        printf("(1) Sell Menu\n");
        printf("(2) Buy Menu\n");
        printf("Choose an option: ");
        scanf("%d", &usermenu);
        printf("\n");
        if(usermenu < 0 || usermenu > 2)
            printf("Invalid input.\n");
        fflush(stdin);
        switch(usermenu)
        {
        case 1:
            numItems = sellMenu(loginID, item, numItems, items);
            break;
        
        case 2:
            buyMenu(user, n, loginID, item, numItems, items);
            break;
        }
    }
    return numItems;
}

/* sellMenu is a function that displays a menu to the user to carry out various tasks related to the items they want to sell. The function returns the updated number of items.
	@param loginID - an integer that represents the unique ID of the user who is currently logged in.
	@param item - a pointer to the itemTag structure that contains information about the items.
	@param numItems - an integer that represents the number of items.
	@param items - a pointer to the file "items.txt.”
	@returns an integer, which is the updated number of items.
	Pre-condition: user input is an integer.
*/

int 
sellMenu(int loginID, struct itemTag *item, int numItems, FILE *items)
{
    int sellmenu = -1, 
        loop = 1;
    while(sellmenu && loop) // Loop for invalid input
    {
        printf("(0) Back\n");
        printf("(1) Add New Item\n");
        printf("(2) Edit Stock\n");
        printf("(3) Show My Products\n");
        printf("(4) Show My Low Stock Products\n");
        printf("Choose an option: ");
        scanf("%d", &sellmenu);
        printf("\n");
        if(sellmenu < 0 || sellmenu > 4) // If input is invalid
            printf("Invalid input.\n\n");
        fflush(stdin);
        switch(sellmenu)
        {
        case 1:
            numItems = addNewItem(loginID, item, numItems, items);
            break;
        
        case 2:
            editStock(loginID, item, numItems, items);
            break;
        
        case 3:
            showMyProducts(loginID, item, numItems);
            break;

        case 4:
            showLowStock(loginID, item, numItems);
            break;
        }
    }
    return numItems;
}

/* addNewItem is a function that allows a seller to add a new item to their list of products for sale. The function returns the updated number of items in the list.
	@param loginID - an integer representing the ID of the seller who is adding the new item.
	@param item - a pointer to an array of itemTag structures representing the current list of items.
	@param numItems - an integer representing the number of items in the list before the new item is added.
	@param items - a pointer to the file "items.txt" in append mode.
	@returns an integer, which is the updated number of items in the list.
	Pre-condition: There exists a file named "items.txt" in the current directory of the program.
*/

int 
addNewItem(int loginID, struct itemTag *item, int numItems, FILE *items)
{
    int i = 0,
        temp;
    printf("<Add New Item>\n");
    while(!i) // Loop for valid productID input
    {
        printf("Enter product ID: ");
        scanf("%d", &temp);
        if(temp == -1)
            i = 1;
        else if(temp >= 0)
        {
            if(numItems) // If there are items already
            {
                if(checkItemID(temp, item, numItems) == -1) // If check passes, loop ends
                {
                    item[numItems].productID = temp;
                    i = 1;
                }
                else
                    printf("Product ID already exists.\n");
            }
            else
                i = 1; // If there are no items yet
        }
        else
            printf("Invalid product ID.\n\n");
    }
    if(temp != -1)
    {
        printf("Enter item name: ");
        fflush(stdin);
        scanf("%[^\n]", item[numItems].iname);
        printf("Enter item category: ");
        fflush(stdin);
        scanf("%[^\n]%*c", item[numItems].category);
        printf("Enter item description: ");
        scanf("%[^\n]%*c", item[numItems].idescription);
        printf("Enter item quantity: ");
        scanf("%d", &item[numItems].quantity);
        printf("Enter item unit price: ");
        scanf("%f", &item[numItems].unitprice);
        printf("<Item added successfully!>\n\n");
        item[numItems].userID = loginID;

        items = fopen("items.txt", "a"); // Add item to the file
        fprintf(items, "%d %d\n", item[numItems].productID, item[numItems].userID);
        fprintf(items, "%s\n", item[numItems].iname);
        fprintf(items, "%s\n", item[numItems].category);
        fprintf(items, "%s\n", item[numItems].idescription);
        fprintf(items, "%d %.2f\n\n", item[numItems].quantity, item[numItems].unitprice);
        fclose(items);
        numItems++;
    }
    else
        printf("Going back...\n\n");

    return numItems;
}

/* fprintOld is a function that writes the old values of an item to the "old.txt" file. The function opens the "old.txt" file and writes the old values of the edited item to the file in the correct format.
	@param oldValues - a pointer to the array of itemTag structures containing the previous values of all items.
	@param numItems - an integer representing the total number of items.
	@param editIndex - an integer representing the index of the edited item.
	@returns nothing.
	Pre-condition: There exists a file named "old.txt" in the current directory of the program.
*/

void 
fprintOld(struct itemTag *oldValues, int numItems, int editIndex)
{
    FILE *oldFile;
    int i = 1;

    oldFile = fopen("old.txt", "a");
    if(oldFile == NULL)
    {
        printf("Error creating/reading file.\n\n");
        i = 0;
    }

    if(i)
    {
        fprintf(oldFile, "%d %d\n", oldValues[editIndex].productID, oldValues[editIndex].userID);
        fprintf(oldFile, "%s\n", oldValues[editIndex].iname);
        fprintf(oldFile, "%s\n", oldValues[editIndex].category);
        fprintf(oldFile, "%s\n", oldValues[editIndex].idescription);
        fprintf(oldFile, "%d %.2f\n\n", oldValues[editIndex].quantity, oldValues[editIndex].unitprice);
    }

    fclose(oldFile);
}

/* editStock is a function that allows a logged-in user to edit the details of a product they have added to the inventory.
	@param loginID - an integer that represents the ID of the logged-in user.
	@param item - a pointer to the array of itemTag structures that holds the details of all items in the inventory.
	@param numItems - an integer that represents the number of items in the inventory.
	@param items - a pointer to the file "items.txt" in write mode.
	@returns nothing.
	Pre-condition: There exists a file named "items.txt" in the current directory of the program.
*/

void 
editStock(int loginID, struct itemTag *item, int numItems, FILE *items)
{
    int i,
        j = 0,
        productID;
    struct itemTag temp[50]; // Array of the logged in user's products
    for(i = 0; i < numItems; i++) // Prints products of the logged in user
    {
        if(loginID == item[i].userID)
        {
            temp[j] = item[i];
            j++;
            printf("%d %d\n", item[i].productID, item[i].userID);
            printf("%s\n", item[i].iname);
            printf("%s\n", item[i].category);
            printf("%s\n", item[i].idescription);
            printf("%d %.2f\n\n", item[i].quantity, item[i].unitprice);
        }
    }
    printf("Enter product ID of the item to be edited: ");
    scanf("%d", &productID);

    if(productID == -1)
        printf("Going back...\n\n");
    else if(checkItemID(productID, temp, j) == -1)
        printf("Product ID doesn't exist.\n\n");
    else
    {
        int menu = -1,
            loop = 1,
            editIndex = checkItemID(productID, item, numItems); // Index of item to be edited
        int addQuant;
        float newPrice;
        string20 newName,
                 newCategory,
                 newDescription;
        struct itemTag oldValues[100];

        oldValues[editIndex] = item[editIndex];

        while(menu && loop) // Loop for invalid input
        {
            fflush(stdin);
            printf("(0) Finish Editing\n");
            printf("(1) Replenish\n");
            printf("(2) Change Price\n");
            printf("(3) Change Item Name\n");
            printf("(4) Change Category\n");
            printf("(5) Change Description\n");
            printf("Choose an option: ");
            scanf("%d", &menu);
            printf("\n");
            if(menu < 0 || menu > 5)
                printf("Invalid input.\n\n");
            fflush(stdin);
            switch(menu)
            {
            case 0:
                fprintItem(item, numItems, items);
                fprintOld(oldValues, numItems, editIndex);
                break;

            case 1:
                oldValues[editIndex].quantity = item[editIndex].quantity; // Old value is kept in an array
                printf("Enter the quantity to add: ");
                scanf("%d", &addQuant);
                printf("\n");
                item[editIndex].quantity += addQuant;
                break;

            case 2:
                oldValues[editIndex].unitprice = item[editIndex].unitprice; // Old value is kept in an array
                printf("Enter the new price: ");
                scanf("%f", &newPrice);
                printf("\n");
                item[editIndex].unitprice = newPrice;
                break;

            case 3:
                strcpy(oldValues[editIndex].iname, item[editIndex].iname); // Old value is kept in an array
                printf("Enter the new name: ");
                scanf("%[^\n]", newName);
                printf("\n");
                strcpy(item[editIndex].iname, newName);
                break;

            case 4:
                strcpy(oldValues[editIndex].category, item[editIndex].category); // Old value is kept in an array
                printf("Enter the new category: ");
                scanf("%[^\n]", newCategory);
                printf("\n");
                strcpy(item[editIndex].category, newCategory);
                break;

            case 5:
                strcpy(newDescription, item[editIndex].idescription); // Old value is kept in an array
                printf("Enter the new description: ");
                scanf("%[^\n]", newDescription);
                printf("\n");
                strcpy(item[editIndex].idescription, newDescription);
                break; 
            }
        }
    }
}

/* showMyProducts is a function that displays the products that belong to a particular user. @param loginID - an integer that represents the ID of the logged in user.
	@param item - a pointer to an array of itemTag structures that contain the details of all items.
	@param numItems - an integer that represents the number of items in the array.
	@returns nothing.
	Pre-condition: There exists an array of itemTag structures containing details of all items.
*/

void 
showMyProducts(int loginID, struct itemTag *item, int numItems)
{
    int i;
    printf(" _______________________________________________________________________\n");
    printf("| Product ID |     Item Name     |   Category   | Unit Price | Quantity |\n");
    for(i = 0; i < numItems; i++) // Prints products of the logged in user
    {
        if(loginID == item[i].userID)
        printf("| %-11d| %-18s| %-13s| %-11.2f| %-9d|\n", 
        item[i].productID, 
        item[i].iname, 
        item[i].category, 
        item[i].unitprice, 
        item[i].quantity);
    }
    printf(" _______________________________________________________________________\n\n");
}

/* showLowStock is a function that takes the login ID of a user, an array of items, and the number of items as inputs. The function finds all the items with a quantity less than 5 that belong to the user with the given login ID then displays the details of the low stock items.
	@param loginID - an integer that represents the login ID of the user.
	@param item - a pointer to an array of itemTag structures.
	@param numItems - an integer that represents the number of items in the array.
	@returns nothing.
	Pre-condition: There exists an array of itemTag structures named "item" with "numItems" elements.
*/

void 
showLowStock(int loginID, struct itemTag *item, int numItems)
{
    int i,
        j = 0;
    char cont;
    struct itemTag lowStocks[100];

    for(i = 0; i < numItems; i++) // Assign user's low stock items to an array
        if(loginID == item[i].userID && item[i].quantity < 5) // If logged in user matches item userID and if quantity is below 5
        {
            lowStocks[j] = item[i];
            j++;
        }
        
    i = 0;
    while(i < j) // Loop for next or exit
    {
        printf("%d\n", lowStocks[i].productID);
        printf("%s\n", lowStocks[i].iname);
        printf("%s\n", lowStocks[i].category);
        printf("%s\n", lowStocks[i].idescription);
        printf("%d %.2f\n\n", lowStocks[i].quantity, lowStocks[i].unitprice);

        printf("(N) Next Item\n");
        printf("(X) Exit\n");
        printf("Choose an option: ");
        fflush(stdin);
        scanf("%c", &cont); // Asks to continue or not
        printf("\n");
        if(cont == 'N' || cont == 'n')
            i++;
        else if(cont == 'X' || cont == 'x')
            i = j;
    }
}

/* buyMenu is a function that displays a menu of options related to buying items, such as viewing all products, showing products by a specific seller, searching products by category or name, adding to cart, editing cart, and checking out. 
	@param user - a pointer to the userTag structure.
	@param n - an integer that represents the number of users already registered.
	@param loginID - an integer that represents the login ID of the user.
	@param item - a pointer to an array of itemTag structures.
	@param numItems - an integer that represents the number of items in the array.
	@param items - a pointer to the file "items.txt" in read mode.
	@returns nothing.
	Pre-condition: There exists an array of itemTag structures named "item" with "numItems" elements, and a file named "carts.txt" in the current directory of the program.
*/

void 
buyMenu(struct userTag *user, int n, int loginID ,struct itemTag *item, int numItems, FILE *items)
{
    FILE *carts;
    struct cartTag cart[100];
    int buymenu = -1, 
        loop = 1,
        numCarts;

    while(buymenu && loop) // Loop for valid input
    {
        carts = fopen("carts.txt", "r");
        numCarts = initCart(cart, carts);
        printf("(0) Back\n");
        printf("(1) View all Products\n");
        printf("(2) Show all Products by a Specific Seller\n");
        printf("(3) Search Products by Category\n");
        printf("(4) Search Products by Name\n");
        printf("(5) Add to Cart\n");
        printf("(6) Edit Cart\n");
        printf("(7) Check Out Menu\n");
        printf("Choose an option: ");
        scanf("%d", &buymenu);
        printf("\n");
        if(buymenu < 0 || buymenu > 7)
            printf("Invalid input.\n\n");
        fflush(stdin);
        switch(buymenu)
        {
        case 1:
            viewAllProducts(loginID, item, numItems);
            break;
        
        case 2:
            showSpecificSeller(loginID, item, numItems);
            break;
        
        case 3:
            searchByCategory(loginID, item, numItems);
            break;

        case 4:
            searchByName(loginID, item, numItems);
            break;
        
        case 5:
            numCarts = addToCart(user, n, cart, numCarts, carts, loginID, item, numItems, items);
            break;

        case 6:
            editCart(cart, numCarts, carts, loginID, item, numItems);
            break;

        case 7:
            checkOutMenu(user, n, cart, numCarts, carts, loginID, item, numItems, items);
            break;
        }
    }
}

/* viewAllProducts is a function that displays all the products in the inventory except those that were uploaded by the currently logged in user. 
	@param loginID - an integer that represents the login ID of the current user.
	@param item - a pointer to an array of structures containing details of all the products.
	@param numItems - an integer that represents the number of products in the array.
	@returns nothing.
	Pre-condition: There exists an array of itemTag structures named "item" containing details of each product with "numItems" with the number of items greater than 0.
*/

void 
viewAllProducts(int loginID ,struct itemTag *item, int numItems)
{
    int i,
        j,
        k;
    struct itemTag temp,
                   tempItem[100];


    for(i = 0; i < numItems; i++) // Copy array to dummy array
        tempItem[i] = item[i];

    for(i = 0; i < numItems; i++) // Remove loginIDs and moves elements
        for(j = 0; j < numItems; j++)
            if(loginID == tempItem[j].userID)
            {
                for(k = j; k < numItems; k++)
                    tempItem[k] = tempItem[k+1];
                numItems--;
            }

    for(i = 0; i < numItems - 1; i++) // Bubble sort userID from lowest to highest
        for(j = 0; j < numItems - i - 1; j++) 
            if(tempItem[j].userID > tempItem[j+1].userID) 
            {
                temp = tempItem[j];
                tempItem[j] = tempItem[j+1];
                tempItem[j+1] = temp;
            }
    
    i = 0;
    while(i < numItems) // Loop for next or exit
    {
        if(i == 0 || tempItem[i].userID != tempItem[i-1].userID)
        {
            printf("Products of user %d:\n", tempItem[i].userID);
            printf(" _______________________________________________________________________\n");
            printf("| Product ID |     Item Name     |   Category   | Unit Price | Quantity |\n");
        }   
        printf("| %-11d| %-18s| %-13s| %-11.2f| %-9d|\n", 
        tempItem[i].productID, 
        tempItem[i].iname, 
        tempItem[i].category, 
        tempItem[i].unitprice, 
        tempItem[i].quantity);
        if(tempItem[i].userID == tempItem[i+1].userID) // If next item has the same seller
            i++;
        else
        {
            char cont;
            printf(" _______________________________________________________________________\n\n");
            printf("(N) Next Item\n");
            printf("(X) Exit\n");
            printf("Choose an option: ");
            fflush(stdin);
            scanf("%c", &cont); // Ask to continue or not
            printf("\n");
            if(cont == 'N' || cont == 'n')
                i++;
            else if(cont == 'X' || cont == 'x')
                i = numItems;
        }
    }
}

/* showSpecificSeller is a function that prompts the user to enter a seller ID and displays all the products listed by that seller. If the seller ID does not exist in the list of products, a message is displayed to inform the user that the seller ID does not exist.
	@param loginID - an integer representing the login ID of the user.
	@param item - a pointer to an array of structures of type itemTag that contains the details of all the products.
	@param numItems - an integer representing the number of items in the itemTag array.
	@returns nothing.
	Pre-condition: There exists an array of structures of type itemTag that contains the details of all the products. 
*/

void 
showSpecificSeller(int loginID ,struct itemTag *item, int numItems)
{
    int sellerID,
        i,
        valid = 0;
    printf("Enter seller ID: ");
    scanf("%d", &sellerID);
    printf("\n");
    for(i = 0; i < numItems; i++)
        if(sellerID == item[i].userID)
            valid = 1;

    if(valid == 1) // If ID entered is a seller
    {
        printf(" _______________________________________________________________________\n");
        printf("| Product ID |     Item Name     |   Category   | Unit Price | Quantity |\n");
        for(i = 0; i < numItems; i++) // Print products of seller ID
        {
            if(sellerID == item[i].userID)
                printf("| %-11d| %-18s| %-13s| %-11.2f| %-9d|\n", 
            item[i].productID, 
            item[i].iname, 
            item[i].category, 
            item[i].unitprice, 
            item[i].quantity);
        }
        printf(" _______________________________________________________________________\n\n");
    }
    else
        printf("Seller ID doesn't exist.\n\n");
}

/* searchByCategory is a function that searches for items in the inventory by category. It searches for items in the inventory that match the inputted category and assigns them to an array. 
	@param loginID - an integer that represents the ID of the logged-in user.
	@param item - a pointer to the itemTag structure.
	@param numItems - an integer that represents the number of items in the inventory.
	@returns nothing.
	Pre-condition: The inventory has at least one item with a valid category and the inputs are valid. 
*/

void 
searchByCategory(int loginID ,struct itemTag *item, int numItems)
{
    int i,
        j = 0;
    char cont;
    string20 catSearch;
    struct itemTag category[100];
    printf("Enter category to search: ");
    scanf("%[^\n]", catSearch);
    printf("\n");

    for(i = 0; i < numItems; i++) // Assign items of inputted category into an array
        if(loginID != item[i].userID && strcasecmp(catSearch, item[i].category) == 0) // Input check
        {
            category[j] = item[i];
            j++;
        }

    i = 0;
    while(i < j) // Loop for next or exit
    {
        printf("%d\n", category[i].productID);
        printf("%s\n", category[i].iname);
        printf("%s\n", category[i].category);
        printf("%s\n", category[i].idescription);
        printf("%d %.2f\n\n", category[i].quantity, category[i].unitprice);

        printf("(N) Next Item\n");
        printf("(X) Exit\n");
        printf("Choose an option: ");
        fflush(stdin);
        scanf("%c", &cont);
        printf("\n");
        if(cont == 'N' || cont == 'n')
            i++;
        else if(cont == 'X' || cont == 'x')
            i = j;
    }
    if(!j)
        printf("No items found.\n\n");
}

/* searchByName is a function that searches for items based on a keyword in item name.
	@param loginID - an integer that represents the ID of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: The imputed keyword is valid and exists within the array of itemTag structures.
*/

void 
searchByName(int loginID ,struct itemTag *item, int numItems)
{
    int i,
        j = 0;
    char cont;
    string20 nameSearch;
    struct itemTag name[100];
    printf("Enter keyword to search: ");
    scanf("%[^\n]", nameSearch);
    printf("\n");

    for(i = 0; i < numItems; i++) // Assign items of inputted keyword into an array
        if(loginID != item[i].userID && strstr(item[i].iname, nameSearch) != NULL) //!!Try to make case insensitive if possible
        {
            name[j] = item[i];
            j++;
        }

    i = 0;
    while(i < j) // Loop for next or exit
    {
        printf("%d\n", name[i].productID);
        printf("%s\n", name[i].iname);
        printf("%s\n", name[i].category);
        printf("%s\n", name[i].idescription);
        printf("%d %.2f\n\n", name[i].quantity, name[i].unitprice);

        printf("(N) Next Item\n");
        printf("(X) Exit\n");
        printf("Choose an option: ");
        fflush(stdin);
        scanf("%c", &cont);
        printf("\n");
        if(cont == 'N' || cont == 'n')
            i++;
        else if(cont == 'X' || cont == 'x')
            i = j;
    }
    if(!j)
        printf("No items found.\n\n");
}

/* addToCart is a function that allows a user to add an item to their shopping cart by prompting them to enter a valid product ID and quantity. The function then updates the shopping cart with the new item and quantity and returns the updated number of items in the cart.
	@param user - a pointer to the userTag structure.
	@param n - an integer that represents the number of users already registered.
	@param cart - a pointer to the cartTag structure.
	@param numCarts - an integer that represents the number of carts already created.
	@param carts - a pointer to the file "carts.txt" in read and write mode.
	@param loginID - an integer that represents the ID of the logged-in user.
	@param item - a pointer to the itemTag structure.
	@param numItems - an integer that represents the number of items already registered.
	@param items - a pointer to the file "items.txt" in read and write mode.
	@returns an integer, which is the number of items in the updated cart.
	Pre-condition: There exists files named "carts.txt" and "items.txt" in the current directory of the program. Input for product ID is valid. 
*/

int 
addToCart(struct userTag *user, int n, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items)
{
    int i,
        cartIndex = -1, // Index of the user's cart
        validID = 0, validQuant = 0,
        cartProductID, cartQuant,
        cartItems,
        itemIndex = -1; // Index of the item specified by the buyer

    for(i = 0; i < numCarts; i++) // Check if user has a cart
        if(loginID == cart[i].userID)
        {
            cartIndex = i;
            i = numCarts; // End loop
        }

    if(cartIndex == -1) // Assign a cart to the logged in user
    {
        cart[numCarts].userID = loginID;
        carts = fopen("carts.txt", "a");
        fprintf(carts, "%d\n-1 -1 -1 -1 -1 -1 -1 -1 -1 -1\n-1 -1 -1 -1 -1 -1 -1 -1 -1 -1\n\n", loginID);
        fclose(carts);
        carts = fopen("carts.txt", "r");
        numCarts = initCart(cart, carts);
        cartIndex = numCarts - 1;
    }

    for(i = 0; i < 10; i++) // Check how many items are in the user's cart
        if(cart[cartIndex].productID[i] == -1)
        {
            cartItems = i;
            i = 10;
        }

    if(cartItems == 9) // If user's cart is full
    {
        int fullCart;
        printf("Cart is full.\n\n");
        printf("(0) Back\n");
        printf("(1) Edit Cart\n");
        printf("(2) Check Out\n");
        printf("Choose an option: ");
        scanf("%d", &fullCart);
        printf("\n"); 
        if(fullCart < 0 || fullCart > 2)
            printf("Invalid input.\n\n");

        switch(fullCart)
        {
            case 1:
                editCart(cart, numCarts, carts, loginID, item, numItems);
                break;
            
            case 2:
                checkOutMenu(user, n, cart, numCarts, carts, loginID, item, numItems, items);
                break;
        }
    }
    else
    {
        while(!validID && cartProductID != -1) // Loop for valid product ID input
        {
            int inCart = 0;
            printf("Enter product ID: ");
            scanf("%d", &cartProductID);
            if(cartProductID != -1)
            {
                for(i = 0; i < cartItems; i++)
                    if(cartProductID == cart[cartIndex].productID[i]) // If inputted productID matches cart productID
                    {
                        inCart++;
                        i = cartItems; // End loop
                    }

                if(!inCart)
                    for(i = 0; i < numItems; i++)
                        if(loginID != item[i].userID && cartProductID == item[i].productID && item[i].quantity) // If item ID is valid
                        {
                            validID++;
                            itemIndex = i;
                            i = numItems; // End loop
                        }

                if(inCart)
                    printf("Item is already in your cart.\n\n");
                else if(!validID)
                    printf("Product ID is invalid.\n\n");
            }
            else
                validID = 1;
        }
        if(cartProductID != -1)
        {
            if(validID)
            {
                printf(" _______________________________________________________________________\n");
                printf("| Product ID |     Item Name     |   Category   | Unit Price | Quantity |\n");
                printf("| %-11d| %-18s| %-13s| %-11.2f| %-9d|\n", 
                item[itemIndex].productID, 
                item[itemIndex].iname, 
                item[itemIndex].category, 
                item[itemIndex].unitprice, 
                item[itemIndex].quantity);
                printf(" _______________________________________________________________________\n\n");
            }
            
            while(!validQuant) // Loop for valid quantity
            {
                printf("Enter quantity to purchase: ");
                scanf("%d", &cartQuant);
                printf("\n");
                if(cartQuant <= item[itemIndex].quantity && cartQuant > 0)
                    validQuant++;
                else
                    printf("Quantity is invalid.\n\n");
            }

            if(validID && validQuant)
            {
                cart[cartIndex].productID[cartItems] = cartProductID;
                cart[cartIndex].quantity[cartItems] = cartQuant;
                cartItems++;
            }

            fprintCart(cart, numCarts, carts);
            printf("Successfully added to cart.\n\n");
        }
    }

    if(cartProductID == -1)
        printf("Going back...\n\n");

    return numCarts;
}

/* editCart is a function that allows the user to edit their shopping cart.
	@param cart - a pointer to an array of cartTag structures that contains information about all shopping carts.
	@param numCarts - an integer that represents the number of shopping carts in the cart array.
	@param carts - a file pointer to the file that stores shopping cart information.
	@param loginID - an integer that represents the ID of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: The cart array and item array are not empty.
*/

void 
editCart(struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems)
{
    int i,
        j,
        cartIndex = -1, // Index of the logged in user's cart
        editCart = 1,
        loop = 1;

    printf("Cart of user %d:\n", loginID);
    for(i = 0; i < numCarts; i++) // Assign a value to cartIndex
        if(loginID == cart[i].userID)
        {
            cartIndex = i;
            i = numCarts; // End loop
        }

    while(editCart && loop) // Loop for valid input
    {
        printf(" ___________________________________________________________________________________\n");
        printf("| Seller ID | Product ID |     Item Name     |   Category   | Unit Price | Quantity |\n");

        for(i = 0; i < 10; i++)
            if(cart[cartIndex].productID[i] != -1) // If cart slot has an item in it
                for(j = 0; j < numItems; j++)
                    if(cart[cartIndex].productID[i] == item[j].productID) // If cart productID matches item productID
                    {
                        printf("| %-10d| %-11d| %-18s| %-13s| %-11.2f| %-9d|\n",
                        item[j].userID, 
                        item[j].productID, 
                        item[j].iname, 
                        item[j].category, 
                        item[j].unitprice, 
                        cart[cartIndex].quantity[i]);
                    } 

        printf(" ___________________________________________________________________________________\n\n");

        printf("(0) Finish Edit Cart\n");
        printf("(1) Remove all items from Seller\n");
        printf("(2) Remove Specific Item\n");
        printf("(3) Edit Quantity\n");
        printf("Choose an option: ");
        fflush(stdin);
        scanf("%d", &editCart);
        printf("\n");
        if(editCart != -1)
            if(editCart < 0 || editCart > 3)
                printf("Invalid input.\n");
        fflush(stdin);
        switch(editCart)
        {
        case -1:
            editCart = 0;
            printf("Going back...\n\n");
            break;

        case 1:
            removeFromSeller(cart, numCarts, carts, cartIndex, item, numItems);
            break;

        case 2:
            removeSpecific(cart, numCarts, carts, cartIndex, item, numItems);
            break;

        case 3:
            editQuantity(cart, numCarts, carts, cartIndex, item, numItems);
            break;
        }
    }
}

/* removeFromSeller is a function that removes all items from a specific seller from the shopping cart.
	@param cart - a pointer to an array of cartTag structures that contains information about all shopping carts.
	@param numCarts - an integer that represents the number of shopping carts in the cart array.
	@param carts - a file pointer to the file that stores shopping cart information.
	@param cartIndex - an integer that represents the index of the shopping cart of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: Seller has to be a user in the system.
*/

void 
removeFromSeller(struct cartTag *cart, int numCarts, FILE *carts, int cartIndex, struct itemTag *item, int numItems) 
{
    int i,
        j,
        k,
        worked = 0,
        deleteSellerID;

    printf("Enter seller ID to delete: ");
    scanf("%d", &deleteSellerID);
    printf("\n");
    if(deleteSellerID != -1)
    {
        for(i = 0; i < numItems; i++)
            if(deleteSellerID == item[i].userID) // If seller's ID is in the cart
                for(j = 0; j < 10; j++)
                    if(item[i].productID == cart[cartIndex].productID[j]) // If item is in cart
                        for(k = j; k < 9; k++) // Shift item back by 1
                        {
                            cart[cartIndex].productID[k] = cart[cartIndex].productID[k+1];
                            cart[cartIndex].quantity[k] = cart[cartIndex].quantity[k+1];
                            worked = 1;
                        }

        if(worked)
        {
            fprintCart(cart, numCarts, carts);
            printf("Successfully removed from cart.\n\n");
        }
        else
            printf("Invalid seller ID.\n\n");
    }
}

/* removeSpecific is a function that removes a specific item from the shopping cart.
	@param cart - a pointer to an array of cartTag structures that contains information about all shopping carts.
	@param numCarts - an integer that represents the number of shopping carts in the cart array.
	@param carts - a file pointer to the file that stores shopping cart information.
	@param cartIndex - an integer that represents the index of the shopping cart of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: Item has to exist within the system.
*/

void 
removeSpecific(struct cartTag *cart, int numCarts, FILE *carts, int cartIndex, struct itemTag *item, int numItems)
{
    int i,
        j,
        worked = 0,
        deleteProductID;

    printf("Enter product ID to remove: ");
    scanf("%d", &deleteProductID);
    printf("\n");

    if(deleteProductID != -1)
    {
        for(i = 0; i < numItems; i++)
            if(deleteProductID == cart[cartIndex].productID[i]) // If inputted ID and productID in cart match
                for(j = i; j < numItems - 1; j++) // Delete item and shift values back by 1
                {
                    cart[cartIndex].productID[j] = cart[cartIndex].productID[j + 1];
                    cart[cartIndex].quantity[j] = cart[cartIndex].quantity[j + 1];
                    worked = 1;
                }

        if(worked)
        {
            fprintCart(cart, numCarts, carts);
            printf("Successfully removed from cart.\n\n");
        }
        else
            printf("Invalid product ID.\n\n");
    }
    else
        printf("Going back...\n\n");
}

/* editQuantity is a function that allows the user to edit the quantity of a specific item in the shopping cart.
	@param cart - a pointer to an array of cartTag structures that contains information about all shopping carts.
	@param numCarts - an integer that represents the number of shopping carts in the cart array.
	@param carts - a file pointer to the file that stores shopping cart information.
	@param cartIndex - an integer that represents the index of the shopping cart of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: Quantity to be imputed is an integer.
*/

void 
editQuantity(struct cartTag *cart, int numCarts, FILE *carts, int cartIndex, struct itemTag *item, int numItems)
{
    int i,
        loop = 1,
        worked = 0,
        productID,
        maxQuantity,
        newQuantity;

    printf("Enter product ID to edit: ");
    scanf("%d", &productID);
    printf("\n");
    if(productID != -1)
    {
        for(i = 0; i < numItems; i++)
            if(productID == item[i].productID)
                maxQuantity = item[i].quantity; // Assign max quantity to the item's actual quantity

        for(i = 0; i < numItems; i++) // Look for product in cart
            if(cart[cartIndex].productID[i] == productID) 
            {
                while(loop)
                {
                    printf("Enter new quantity: ");
                    scanf("%d", &newQuantity);
                    printf("\n");
                    if(newQuantity <= maxQuantity && newQuantity > 0) // Quantity in bounds check
                        loop = 0;
                    if(loop)
                        printf("Invalid quantity.\n\n");
                }
                cart[cartIndex].quantity[i] = newQuantity;
                worked = 1;
            }

        if(worked)
        {
            fprintCart(cart, numCarts, carts);
            printf("Successfully edited.\n\n");
        }
        else
            printf("Invalid product ID.\n\n");
    }
    else
        printf("Going back...\n\n");
}

/* fscanOld is a function that reads data from a file and assigns it to an array of itemTag structures. The function returns the number of items read from the file.
	@param old - a pointer to an array of itemTag structures that will hold the information read from the file.
	@param oldFile - a pointer to a file containing the information to be read.
	@returns an integer representing the number of items read from the file.
	Pre-condition: The file specified by oldFile must exist and be readable.
*/

int 
fscanOld(struct itemTag *old, FILE *oldFile)
{
    int i = 0,
        numOld = 0;

    if(oldFile == NULL)
        i = 1; 

    while(!i) // Reads items.txt and assigns to an array
    {
        fscanf(oldFile, "%d %d\n", &old[numOld].productID, &old[numOld].userID);
        fscanf(oldFile, " %[^\n]", old[numOld].iname);
        fscanf(oldFile, " %[^\n]", old[numOld].category);
        fscanf(oldFile, " %[^\n]", old[numOld].idescription);
        fscanf(oldFile, "%d %f\n", &old[numOld].quantity, &old[numOld].unitprice);
        if(feof(oldFile)) //If end of file is reached, end the loop
            i = 1;
        numOld++;
    }

    fclose(oldFile);

    return numOld;
}

/* checkOutMenu handles the checkout process for a logged-in user.
	@param user - a pointer to a userTag structure that contains information about the logged-in user.
	@param n - an integer that represents the length of the username string in the userTag structure.
	@param cart - a pointer to an array of cartTag structures that contains information about the items in the logged-in user's cart.
	@param numCarts - an integer that represents the number of carts in the cart array.
	@param carts - a pointer to a file that contains information about all carts in the system.
	@param loginID - an integer that represents the ID of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@param items - a pointer to a file that contains information about all items available for sale.
	@returns nothing.
	Pre-condition: The cart has at least one item and the maximum number of items in the cart is 10.
*/

void 
checkOutMenu(struct userTag *user, int n, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items)
{
    FILE *oldFile, *transactions;
    struct itemTag old[100];
    struct checkOutTag transaction[100];
    int i,
        j,
        k,
        oldIndex = -1, // Index of old value in old[]
        newIndex = -1, // Index of new value in item[]
        cartIndex = -1, // Index of the logged in user's cart
        numOld = 0, // Number of old values
        loop = 1,
        checkOutMenu = 1,
        validDate, // Loop for valid date input
        maxDays; // Max days for each month

    oldFile = fopen("old.txt", "r");
    numOld = fscanOld(old, oldFile);
    transactions = fopen("transactions.txt", "a");
    fclose(transactions);

    for(i = 0; i < numCarts; i++) // Assign a value to cartIndex
        if(loginID == cart[i].userID)
        {
            cartIndex = i;
            i = numCarts; // End loop
        }

    validDate = 0;
    while(!validDate) // Loop for valid month
    {
        printf("Enter numerical month: ");
        scanf("%d", &transaction[cartIndex].date.month);
        if(transaction[cartIndex].date.month >= 1 && transaction[cartIndex].date.month <= 12)
            validDate = 1;
        else
            printf("Month is invalid.\n\n");
    }

    validDate = 0;
    while(!validDate) // Loop for valid day
    {
        printf("Enter day: ");
        scanf("%d", &transaction[cartIndex].date.day);

        maxDays = 31;
        if(transaction[cartIndex].date.month == 4 || transaction[cartIndex].date.month == 6 || transaction[cartIndex].date.month == 9 || transaction[cartIndex].date.month == 11) {
            maxDays = 30;
        } else if(transaction[cartIndex].date.month == 2) {
            maxDays = 28;
        }

        if(transaction[cartIndex].date.day >= 1 && transaction[cartIndex].date.day <= maxDays) 
            validDate = 1;
        else
            printf("Day is invalid for the entered month.\n\n");
    }
    printf("Enter year: ");
    scanf("%d", &transaction[cartIndex].date.year);
    printf("\n");


    if(numOld) // If there are old values, show old and new values
    {
        printf("     Table for Old and New values\n");
        printf("     _______________________________________________________________________\n");
        printf("    | Product ID |     Item Name     |   Category   | Unit Price | Quantity |\n");
        for(i = 0; i < 10; i++)
            for(j = 0; j < numOld; j++)
                if(cart[cartIndex].productID[i] == old[j].productID)
                {
                    oldIndex = j;
                    printf("Old | %-11d| %-18s| %-13s| %-11.2f| %-9d|\n", old[oldIndex].productID, old[oldIndex].iname, old[oldIndex].category, old[oldIndex].unitprice, old[oldIndex].quantity);
                    for(k = 0; k < numItems; k++)
                    {
                        if(old[oldIndex].productID == item[k].productID)
                        {
                            newIndex = k;
                            printf("New | %-11d| %-18s| %-13s| %-11.2f| %-9d|\n", item[newIndex].productID, item[newIndex].iname, item[newIndex].category, item[newIndex].unitprice, item[newIndex].quantity);
                            break;
                        }
                    }
                }
        printf("     _______________________________________________________________________\n");
        printf("     NOTE: You can still go back to Edit Cart.\n\n");
    }

    while(checkOutMenu && loop) // Loop for valid input
    {
        printf("(0) Exit Check Out\n");
        printf("(1) All\n");
        printf("(2) By a Specific Seller\n");
        printf("(3) Specific Item\n");
        printf("Choose an option: ");
        scanf("%d", &checkOutMenu);
        printf("\n");
        if(checkOutMenu < 0 || checkOutMenu > 3)
            printf("Invalid input.\n");
        fflush(stdin);
        switch(checkOutMenu) 
        {
        case 1:
            checkOutAll(user, n, transaction, cartIndex, cart, numCarts, carts, loginID, item, numItems, items, transactions);
            break;
        
        case 2:
            checkOutSeller(user, n, transaction, cartIndex, cart, numCarts, carts, loginID, item, numItems, items, transactions);
            break;

        case 3:
            checkOutItem(user, n, transaction, cartIndex, cart, numCarts, carts, loginID, item, numItems, items, transactions);
            break;
        }
    }
}

/* checkOutAll is a function that processes a cart's items, creates transactions, and updates the cart, item, and transaction files.
	@param user - a pointer to an array of userTag structures that contains information about all registered users.
	@param n - an integer that represents the number of users in the user array.
	@param transaction - a pointer to an array of checkOutTag structures that contains information about all transactions.
	@param cartIndex - an integer that represents the index of the cart to be checked out.
	@param cart - a pointer to an array of cartTag structures that contains information about all carts.
	@param numCarts - an integer that represents the number of carts in the cart array.
	@param carts - a file pointer to the carts file.
	@param loginID - an integer that represents the ID of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@param items - a file pointer to the items file.
	@param transactions - a file pointer to the transactions file.
	@returns nothing.
	Pre-condition: The cart at the specified index is not empty and contains valid product IDs and quantities.
*/

void 
checkOutAll(struct userTag *user, int n, struct checkOutTag *transaction, int cartIndex, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items, FILE *transactions)
{
    int i, 
        j, 
        k,
        a = 0,
        numTrans = 0, 
        numTransItem = 0, 
        sellerIndex = -1,
        currentSeller = -1,
        quantity,
        productID;
    float sellerTotal = 0.0,
          unitPrice,
          totalPrice;
    char *itemName;
    
    transactions = fopen("transactions.txt", "a");
    if(transactions == NULL) // If loop doesn't exist
    {
        transactions = fopen("transactions.txt", "w");
        if(transactions == NULL)
        {
            printf("Unable to create file\n");
            a = 1;
        } 
        else
            a = 0;
    } 
    else
        a = 0;
    
    if(!a)
	{
        fprintf(transactions, "%d %d %d %d\n", loginID, transaction[cartIndex].date.month, transaction[cartIndex].date.day, transaction[cartIndex].date.year);

        for(i = 0; i < n; i++) // Find the buyer
            if(loginID == user[i].userID)
            {
                transaction[cartIndex].user = user[i];
                i = n;
            }

        if(cart[cartIndex].productID[0] == -1)
            printf("Your cart is empty.\n\n");
        else
        {
            for(i = 0; i < 10; i++) // Create transactions
                for(j = 0; j < numItems; j++)
                    if(cart[cartIndex].productID[i] == item[j].productID)
                    {
                        if(currentSeller != item[j].userID) // Check if the seller is the same as the current transaction
                        {
                            currentSeller = item[j].userID;
                            numTrans++;
                            numTransItem = 0;
                        }

                        transaction[cartIndex].transaction[numTrans-1].item[numTransItem] = item[j];
                        transaction[cartIndex].transaction[numTrans-1].item[numTransItem].quantity = cart[cartIndex].quantity[i];
                        numTransItem++;
                        
                        if(numTransItem == 5) // If the current transaction is full, move to the next one
                        {
                            currentSeller = -1;
                            numTransItem = 0;
                        }
                    }

            printf("Transaction Date: %d/%d/%d\n\n", transaction[cartIndex].date.month, transaction[cartIndex].date.day, transaction[cartIndex].date.year);
            
            for(i = 0; i < numTrans; i++)
            {
                sellerTotal = 0.0;
                printf("Transaction %d:\n", i+1);
                printf(" __________________________________________________________________________\n");
                printf("| Product ID |         Name          | Quantity | Unit Price | Total Price |\n");
                for(j = 0; j < 5; j++)
                {
                    if(transaction[cartIndex].transaction[i].item[j].productID == 0)
                        j = 5;
                    if(j != 5)
                    {
                        quantity = transaction[cartIndex].transaction[i].item[j].quantity;
                        productID = transaction[cartIndex].transaction[i].item[j].productID;
                        itemName = transaction[cartIndex].transaction[i].item[j].iname;
                        unitPrice = transaction[cartIndex].transaction[i].item[j].unitprice;
                        totalPrice = unitPrice * quantity;
                        printf("| %-11d| %-22s| %-9d| %-11.2f| %-12.2f|\n",
                            productID, itemName, quantity, unitPrice, totalPrice);
                        sellerTotal += totalPrice;

                        for(k = 0; k < 10; k++) // Remove bought item from cart
                            if(cart[cartIndex].productID[k] == productID)
                            {
                                cart[cartIndex].productID[k] = -1;
                                cart[cartIndex].quantity[k] = -1;
                            }

                        for(k = 0; k < numItems; k++) // Remove from seller's quantity
                            if(item[k].productID == productID && item[k].userID == transaction[cartIndex].transaction[i].item[j].userID)
                            {
                                item[k].quantity -= quantity;
                            }
                    }
                }
                fprintf(transactions, "%d %.2f ", transaction[cartIndex].transaction[i].item[0].userID, sellerTotal);

                for(k = 0; k < n; k++)
                    if(transaction[cartIndex].transaction[i].item[0].userID == user[k].userID)
                    {
                        sellerIndex = k;
                        k = n;
                    }
                    
                printf(" __________________________________________________________________________\n");
                printf("Total amount due for seller %d (%s): %.2f\n", transaction[cartIndex].transaction[i].item[0].userID, user[sellerIndex].name, sellerTotal);
                printf("\n");
            }

            fprintCart(cart, numCarts, carts);
            fprintItem(item, numItems, items);
            fprintf(transactions, "-1 -1"); // To denote the end of transaction
            fprintf(transactions, "\n\n");
            fclose(transactions);
        }
	}	
}

/* checkOutSeller is a function that processes the checkout of items from a particular seller by a buyer.
	@param user - a pointer to an array of userTag structures that contains information about all users.
	@param n - an integer that represents the number of users in the user array.
	@param transaction - a pointer to an array of checkOutTag structures that contains information about all transactions.
	@param cartIndex - an integer that represents the index of the cart in the cart array.
	@param cart - a pointer to an array of cartTag structures that contains information about all carts.
	@param numCarts - an integer that represents the number of carts in the cart array.
	@param carts - a pointer to the carts.txt file.
	@param loginID - an integer that represents the ID of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@param items - a pointer to the items.txt file.
	@param transactions - a pointer to the transactions.txt file.
	@returns nothing.
	Pre-condition: The sellerID input by the user is valid and exists in the item array.
*/

void 
checkOutSeller(struct userTag *user, int n, struct checkOutTag *transaction, int cartIndex, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items, FILE *transactions)
{
    int i, 
        j, 
        k,
        a = 0,
        numTrans = 0, 
        numTransItem = 0,
        sellerID = -1,
        sellerIndex = -1,
        currentSeller = -1,
        isSeller = 0,
        isInCart = 0;
    float sellerTotal = 0.0;

    transactions = fopen("transactions.txt", "a");
    if(transactions == NULL) // If file doesn't exist
    {
        transactions = fopen("transactions.txt", "w"); // Create file
        if(transactions == NULL) // If file doesn't create
        {
            printf("Unable to create file\n");
            a = 1;
        } 
        else
            a = 0;
    } 
    else
        a = 0;

    while(!isSeller) // Loop for valid sellerID
    {
        printf("Enter seller ID to check out items: ");
        scanf("%d", &sellerID);
        for(i = 0; i < numItems; i++) 
            if(item[i].userID == sellerID) 
            {
                isSeller = 1;
                for(j = 0; j < 10; j++) // Check if seller has an item in the user's cart
                    if(cart[cartIndex].productID[j] == item[i].productID && cart[cartIndex].quantity[j] > 0)
                        isInCart = 1;
            }
        if(!isSeller)
            printf("Invalid seller ID.\n\n");
        else if(!isInCart)
        {
            printf("No items from this seller in the cart.\n\n");
            isSeller = 0;
        }
    }

    if(!a)
    {
        for(i = 0; i < n; i++) // Find the buyer
            if(loginID == user[i].userID)
            {
                transaction[cartIndex].user = user[i];
                i = n;
            }

        if(cart[cartIndex].productID[0] == -1)
            printf("Your cart is empty.\n\n");
        else
        {
            fprintf(transactions, "%d %d %d %d\n", loginID, transaction[cartIndex].date.month, transaction[cartIndex].date.day, transaction[cartIndex].date.year);
            for(i = 0; i < 10; i++) // Create transactions
                for(j = 0; j < numItems; j++)
                    if(cart[cartIndex].productID[i] == item[j].productID && item[j].userID == sellerID)
                    {
                        if(currentSeller != item[j].userID) // If item is the same as in the transaction
                        {
                            currentSeller = item[j].userID;
                            numTrans++;
                            numTransItem = 0;
                        }

                        transaction[cartIndex].transaction[numTrans-1].item[numTransItem] = item[j];
                        transaction[cartIndex].transaction[numTrans-1].item[numTransItem].quantity = cart[cartIndex].quantity[i];
                        numTransItem++;
                        
                        if(numTransItem == 5) // If transaction is full, move to next
                        {
                            currentSeller = -1;
                            numTransItem = 0;
                        }

                        cart[cartIndex].productID[i] = -1;
                        cart[cartIndex].quantity[i] = -1;
                        
                        for(k = i + 1; k < 10; k++) // Shift all items back by one
                        {
                            cart[cartIndex].productID[k - 1] = cart[cartIndex].productID[k];
                            cart[cartIndex].quantity[k - 1] = cart[cartIndex].quantity[k];
                        }
                        cart[cartIndex].productID[9] = -1;
                        cart[cartIndex].quantity[9] = -1;
                    }

            printf("Transaction Date: %d/%d/%d\n\n", transaction[cartIndex].date.month, transaction[cartIndex].date.day, transaction[cartIndex].date.year);
            
            for(i = 0; i < numTrans; i++)
            {
                sellerTotal = 0.0;
                printf("Transaction %d:\n", i+1);
                printf(" __________________________________________________________________________\n");
                printf("| Product ID |         Name          | Quantity | Unit Price | Total Price |\n");
                for(j = 0; j < 5; j++)
                {
                    if(transaction[cartIndex].transaction[i].item[j].productID == 0)
                        j = 5;
                    if(j != 5)
                    {
                        int quantity = transaction[cartIndex].transaction[i].item[j].quantity;
                        int productID = transaction[cartIndex].transaction[i].item[j].productID;
                        char* itemName = transaction[cartIndex].transaction[i].item[j].iname;
                        float unitPrice = transaction[cartIndex].transaction[i].item[j].unitprice;
                        float totalPrice = unitPrice * quantity;
                        printf("| %-11d| %-22s| %-9d| %-11.2f| %-12.2f|\n",
                            productID, itemName, quantity, unitPrice, totalPrice);
                        sellerTotal += totalPrice;

                        for(k = 0; k < numItems; k++) // Remove from seller's quantity
                            if(item[k].productID == productID && item[k].userID == transaction[cartIndex].transaction[i].item[j].userID)
                                item[k].quantity -= quantity;
                    }
                }
                fprintf(transactions, "%d %.2f ", transaction[cartIndex].transaction[i].item[0].userID, sellerTotal);

                for(k = 0; k < n; k++)
                    if(transaction[cartIndex].transaction[i].item[0].userID == user[k].userID)
                    {
                        sellerIndex = k;
                        k = n;
                    }
                    
                printf(" __________________________________________________________________________\n");
                printf("Total amount due for seller %d (%s): %.2f\n", transaction[cartIndex].transaction[i].item[0].userID, user[sellerIndex].name, sellerTotal);
                printf("\n");
            }
            fprintCart(cart, numCarts, carts);
            fprintItem(item, numItems, items);
            fprintf(transactions, "-1 -1"); // To denote the end of a transaction
            fprintf(transactions, "\n\n"); 
            fclose(transactions);
        }
    }
}

/* checkOutItem is a function that allows a user to check out items in their cart and creates a transaction record in transactions.txt file.
	@param user - a pointer to an array of userTag structures that contains information about all registered users.
	@param n - an integer that represents the number of users in the user array.
	@param transaction - a pointer to an array of checkOutTag structures that contains information about all transactions made by the user.
	@param cartIndex - an integer that represents the index of the user's cart in the cart array.
	@param cart - a pointer to an array of cartTag structures that contains information about all user carts.
	@param numCarts - an integer that represents the number of carts in the cart array.
	@param carts - a file pointer that points to the carts.txt file.
	@param loginID - an integer that represents the ID of the currently logged-in user.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@param items - a file pointer that points to the items.txt file.
	@param transactions - a file pointer that points to the transactions.txt file.
	@returns nothing.
	Pre-condition: The user has at least one item in their cart.
*/

void 
checkOutItem(struct userTag *user, int n, struct checkOutTag *transaction, int cartIndex, struct cartTag *cart, int numCarts, FILE *carts, int loginID, struct itemTag *item, int numItems, FILE *items, FILE *transactions)
{
    int i, 
        j, 
        k,
        a = 0,
        numTrans = 0, 
        numTransItem = 0,
        itemID = -1,
        itemIndex = -1,
        currentItem = -1,
        isItem = 0,
        sellerIndex = -1;
    float sellerTotal = 0.0;

    transactions = fopen("transactions.txt", "a");
    if(transactions == NULL) // If file doesn't exist
    {
        transactions = fopen("transactions.txt", "w"); // Create file
        if(transactions == NULL) // If file doesn't create
        {
            printf("Unable to create file\n");
            a = 1;
        } 
        else
            a = 0;
    } 
    else
        a = 0;

    while(!isItem) // Loop for valid itemID
    {
        printf("Enter item ID to check out: ");
        scanf("%d", &itemID);
        for(i = 0; i < numItems; i++) 
            if(item[i].productID == itemID) 
            {
                isItem = 1;
                itemIndex = i;
                i = numItems;
            }
        if(!isItem)
            printf("Invalid item ID.\n\n");
    }

    if(!a)
    {
        for(i = 0; i < n; i++) // Find the buyer
            if(loginID == user[i].userID)
            {
                transaction[cartIndex].user = user[i];
                i = n;
            }

        if(cart[cartIndex].productID[0] == -1)
            printf("Your cart is empty.\n\n");
        else
        {
            fprintf(transactions, "%d %d %d %d\n", loginID, transaction[cartIndex].date.month, transaction[cartIndex].date.day, transaction[cartIndex].date.year);
            for(i = 0; i < 10; i++) // Create transactions
                if(cart[cartIndex].productID[i] == itemID)
                {
                    if(currentItem != itemID) // If item is the same as in the transaction
                    {
                        currentItem = itemID;
                        numTrans++;
                        numTransItem = 0;
                    }

                    transaction[cartIndex].transaction[numTrans-1].item[numTransItem] = item[itemIndex];
                    transaction[cartIndex].transaction[numTrans-1].item[numTransItem].quantity = cart[cartIndex].quantity[i];
                    numTransItem++;
                    
                    if(numTransItem == 5) // If transaction is full, move to next
                    {
                        currentItem = -1;
                        numTransItem = 0;
                    }

                    cart[cartIndex].productID[i] = -1;
                    cart[cartIndex].quantity[i] = -1;
                    
                    for(k = i + 1; k < 10; k++) // Shift all items back by one
                    {
                        cart[cartIndex].productID[k - 1] = cart[cartIndex].productID[k];
                        cart[cartIndex].quantity[k - 1] = cart[cartIndex].quantity[k];
                    }
                    cart[cartIndex].productID[9] = -1;
                    cart[cartIndex].quantity[9] = -1;
                }

            printf("Transaction Date: %d/%d/%d\n\n", transaction[cartIndex].date.month, transaction[cartIndex].date.day, transaction[cartIndex].date.year);
            
            for(i = 0; i < numTrans; i++)
            {
                sellerTotal = 0.0;
                printf("Transaction %d:\n", i+1);
                printf(" __________________________________________________________________________\n");
                printf("| Product ID |         Name          | Quantity | Unit Price | Total Price |\n");
                for(j = 0; j < 5; j++)
                {
                    if(transaction[cartIndex].transaction[i].item[j].productID == 0)
                        j = 5;
                    if(j != 5)
                    {
                        int quantity = transaction[cartIndex].transaction[i].item[j].quantity;
                        int productID = transaction[cartIndex].transaction[i].item[j].productID;
                        char* itemName = transaction[cartIndex].transaction[i].item[j].iname;
                        float unitPrice = transaction[cartIndex].transaction[i].item[j].unitprice;
                        float totalPrice = unitPrice * quantity;
                        printf("| %-11d| %-22s| %-9d| %-11.2f| %-12.2f|\n",
                            productID, itemName, quantity, unitPrice, totalPrice);
                        sellerTotal += totalPrice;

                        for(k = 0; k < numItems; k++) // Remove from seller's quantity
                            if(item[k].productID == productID && item[k].userID == transaction[cartIndex].transaction[i].item[j].userID)
                                item[k].quantity -= quantity;
                    }
                }
                fprintf(transactions, "%d %.2f ", transaction[cartIndex].transaction[i].item[0].userID, sellerTotal); // Print transaction to transactions.txt file

                for(k = 0; k < n; k++) // Find seller name with seller's userID
                    if(transaction[cartIndex].transaction[i].item[0].userID == user[k].userID)
                    {
                        sellerIndex = k;
                        k = n;
                    }
                    
                printf(" __________________________________________________________________________\n");
                printf("Total amount due for seller %d (%s): %.2f\n", transaction[cartIndex].transaction[i].item[0].userID, user[sellerIndex].name, sellerTotal);
                printf("\n");
            }
            fprintCart(cart, numCarts, carts);
            fprintItem(item, numItems, items);
            fprintf(transactions, "-1 -1"); // To denote the end of a transaction
            fprintf(transactions, "\n\n");
            fclose(transactions);
        }
    }
}

/* initTransactions is a function that initializes transaction data from a file and stores it in a transPrint structure.
	@param temp - a pointer to an array of transPrint structures that will store transaction data.
	@returns an integer that represents the number of transactions initialized.
	Pre-condition: A file named "transactions.txt" exists in the current directory and contains valid transaction data in the format specified in the function.
*/

int 
initTransactions(struct transPrint *temp)
{
    FILE *transactions;
    int numTransactions = 0,
        numCheckOuts = 0,
        i = 0;

    transactions = fopen("transactions.txt", "r");
    if(transactions == NULL) // If file doesn't exist
    {
        printf("There are no transactions.\n\n");
        fclose(transactions);
        i = 1;
    }

    while(!i)
    {
        fscanf(transactions, "%d %d %d %d", &temp[numCheckOuts].userID, &temp[numCheckOuts].date.month, &temp[numCheckOuts].date.day, &temp[numCheckOuts].date.year);
        numTransactions = 0;
        fscanf(transactions, "%d %f", &temp[numCheckOuts].sellerID[numTransactions], &temp[numCheckOuts].amountDue[numTransactions]);
        for(numTransactions = 1; numTransactions < 5; numTransactions++) // Loop up to 4 aditional transactions
        {
            fscanf(transactions, "%d %f", &temp[numCheckOuts].sellerID[numTransactions], &temp[numCheckOuts].amountDue[numTransactions]);
            if(temp[numCheckOuts].sellerID[numTransactions] == -1) // If -1 is read, end of the transaction is read
                numTransactions = 5;
        }
        if(feof(transactions)) // End loop if end of file is reached
            i = 1;
        if(!i)
            numCheckOuts++;
    }

    return numCheckOuts;
}

/* inputDate is a function that takes user input to initialize a dateTag structure.
	@param date - a pointer to a dateTag structure that will be initialized with user input.
	@returns nothing.
	Pre-condition: date, month, and year are integers.
*/

void 
inputDate(struct dateTag *date)
{
    int validDate,
        maxDays,
        validOrder;
    // Input of date 1
    printf("Enter start date: \n");
    validDate = 0;
    while(!validDate) // Loop for valid month
    {
        printf("Enter numerical month: ");
        scanf("%d", &date[0].month);
        if(date[0].month >= 1 && date[0].month <= 12)
            validDate = 1;
        else
            printf("Month is invalid.\n\n");
    }

    validDate = 0;
    while(!validDate) // Loop for valid day
    {
        printf("Enter day: ");
        scanf("%d", &date[0].day);

        maxDays = 31;
        if(date[0].month == 4 || date[0].month == 6 || date[0].month == 9 || date[0].month == 11)
            maxDays = 30;
        else if(date[0].month == 2)
            maxDays = 28;

        if(date[0].day >= 1 && date[0].day <= maxDays) 
            validDate = 1;
        else
            printf("Day is invalid for the entered month.\n\n");
    }
    printf("Enter year: ");
    scanf("%d", &date[0].year);
    printf("\n");

    validOrder = 0;
    // Input of date 2
    while(!validOrder)
    {
        printf("Enter end date: \n");
        validDate = 0;
        while(!validDate) // Loop for valid month
        {
            printf("Enter numerical month: ");
            scanf("%d", &date[1].month);
            if(date[1].month >= 1 && date[1].month <= 12)
                validDate = 1;
            else
                printf("Month is invalid.\n\n");
        }

        validDate = 0;
        while(!validDate) // Loop for valid day
        {
            printf("Enter day: ");
            scanf("%d", &date[1].day);

            maxDays = 31;
            if(date[1].month == 4 || date[1].month == 6 || date[1].month == 9 || date[1].month == 11)
                maxDays = 30;
            else if(date[1].month == 2)
                maxDays = 28;

            if(date[1].day >= 1 && date[1].day <= maxDays) 
                validDate = 1;
            else
                printf("Day is invalid for the entered month.\n\n");
        }
        printf("Enter year: ");
        scanf("%d", &date[1].year);
        printf("\n");
        // Chronological order check
        if(date[1].year < date[0].year || (date[1].year == date[0].year && date[1].month < date[0].month) || (date[1].year == date[0].year && date[1].month == date[0].month && date[1].day <= date[0].day))
            printf("Error: Date 2 must be after Date 1.\n\n");
        else
            validOrder = 1;
    }
}

/* isBetween is a function that checks if a given date is within a specified date range.
	@param date - a pointer to an array of dateTag structures representing the start and end dates of the date range.
	@param dateTrans - a pointer to a dateTag structure representing the date to be checked.
	@returns an integer value of 1 if the dateTrans is within the date range, and 0 otherwise.
	Pre-condition: The date and dateTrans structures are valid and have correct date values.
*/

int 
isBetween(struct dateTag *date, struct dateTag *dateTrans)
{
    int nReturn;

    if(dateTrans->year >= date[0].year && dateTrans->year <= date[1].year)
    {
        if(dateTrans->year == date[0].year && dateTrans->month < date[0].month)
            nReturn = 0;
        else if(dateTrans->year == date[1].year && dateTrans->month > date[1].month)
            nReturn = 0;
        else if(dateTrans->year == date[0].year && dateTrans->month == date[0].month && dateTrans->day < date[0].day)
            nReturn = 0;
        else if(dateTrans->year == date[1].year && dateTrans->month == date[1].month && dateTrans->day > date[1].day)
            nReturn = 0;
        else
            nReturn = 1;
    }
    else
        nReturn = 0;
    
    return nReturn;
}

/* showTotalSales is a function that calculates the total sales between two given dates.
	@param temp - a pointer to an array of transPrint structures that contains information about all transactions.
	@param numCheckOuts - an integer that represents the number of transactions in the temp array.
	@param date - a pointer to an array of dateTag structures that represents the start and end date of the date range to be searched.
	@returns nothing.
	Pre-condition: The temp array contains valid transaction information and the date array contains valid start and end dates.
*/

void 
showTotalSales(struct transPrint *temp, int numCheckOuts, struct dateTag *date) 
{
    int i,
        j;
    float totalSales = 0;

    for(i = 0; i < numCheckOuts; i++) 
        if(isBetween(date, &temp[i].date)) // Within date range check
            for(j = 0; j < 5; j++) 
            {
                if(temp[i].sellerID[j] == -1)
                    j = 5;
                totalSales += temp[i].amountDue[j];
            }
    
    if(totalSales <= 0)
        printf("No transactions found.\n\n");
    else
        printf("Total sales between %d/%d/%d and %d/%d/%d: %.2f\n\n", date[0].month, date[0].day, date[0].year, date[1].month, date[1].day, date[1].year, totalSales);
}

/* showSellerSales is a function that displays the total sales for each seller within a provided date range.
	@param temp - a pointer to an array of transPrint structures that contains information about all transactions.
	@param numCheckOuts - an integer that represents the number of transactions in the transPrint array.
	@param date - a pointer to a dateTag structure that represents the date range to search for transactions.
	@param user - a pointer to an array of userTag structures that contains information about all registered users.
	@param n - an integer that represents the number of users in the user array.
	@returns nothing.
	Pre-condition: The input parameters are valid and the transactions, users, and date range are properly defined. 
*/

void 
showSellerSales(struct transPrint *temp, int numCheckOuts, struct dateTag *date, struct userTag *user, int n)
{
    int i, 
        j, 
        k, 
        found = 0;
    float totalSales;

    printf("Seller Sales:\n");
    printf(" _________________________________________________\n");
    printf("| Seller ID |         Name          | Total Sales |\n");

    for(i = 0; i < n - 1; i++) // Go through all users
    {
        totalSales = 0;
        found = 0;

        for(j = 0; j < numCheckOuts; j++)
            if(isBetween(date, &temp[j].date)) // Within date range check
                for(k = 0; k < 5; k++)
                    if(temp[j].sellerID[k] == user[i].userID && temp[j].amountDue[k] != -1) // If sellerID matches amountDue
                    {
                        totalSales += temp[j].amountDue[k];
                        found = 1;
                    }

        if(found)
            printf("| %-10d| %-22s| %-12.2f|\n", user[i].userID, user[i].name, totalSales);
    }

    printf(" _________________________________________________\n\n");

    if(!found) 
        printf("No transactions found between the provided dates.\n\n");
}

/* showShopaholics is a function that displays the total spending of buyers within a given date range.
	@param temp - a pointer to an array of transPrint structures that contains information about all transactions.
	@param numCheckOuts - an integer that represents the number of transactions in the temp array.
	@param date - a pointer to an array of dateTag structures that contains the start and end dates of the desired date range.
	@param user - a pointer to an array of userTag structures that contains information about all users.
	@param n - an integer that represents the number of users in the user array.
	@returns nothing.
	Pre-condition: The input array of transactions and user array are not empty.
*/

void 
showShopaholics(struct transPrint *temp, int numCheckOuts, struct dateTag *date, struct userTag *user, int n)
{
    int i, 
        j, 
        k, 
        found = 0;
    float totalSpending;

    printf("Shopaholics:\n");
    printf(" _________________________________________________\n");
    printf("| Buyer ID |         Name          |Total Spending|\n");

    for(i = 0; i < n - 1; i++) // Go through all users
    {
        totalSpending = 0;
        found = 0;

        for(j = 0; j < numCheckOuts; j++)
            if(isBetween(date, &temp[j].date)) // Within date range check
                for(k = 0; k < 5; k++)
                    if(temp[j].userID == user[i].userID && temp[j].amountDue[k] != -1) // If userID matches amountDue
                    {
                        totalSpending += temp[j].amountDue[k];
                        found = 1;
                        k = 5;
                    }

        if(found)
            printf("| %-9d| %-22s| %-13.2f|\n", user[i].userID, user[i].name, totalSpending);
    }

    printf(" _________________________________________________\n\n");

    if(!found) 
        printf("No transactions found between the provided dates.\n\n");
}

/* adminMenu is a function that provides an interface for administrative tasks.
	@param user - a pointer to an array of userTag structures that contains information about all users.
	@param n - an integer that represents the number of users in the user array.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: The admin password is valid.
*/

void 
adminMenu(struct userTag *user, int n, struct itemTag *item, int numItems)
{
	int option,
        i = 1,
        numCheckOuts; // Number of checkouts
    string20 tempPassword;
    struct transPrint temp[100]; // Array of check outs for printing
	
	printf("Input Admin Password: ");
	scanf("%[^\n]", tempPassword);
	
	if(strcmp(tempPassword, "H3LLo?") != 0) // Admin password check
		printf("Unauthorized access not allowed.\n\n");
	else if(strcmp(tempPassword, "H3LLo?") == 0)
	{
        numCheckOuts = initTransactions(temp);

        struct dateTag date[2];
        printf("<Successfully logged in!>\n\n");
		while(i)
		{
			printf("(0) Back\n");
			printf("(1) Show All Users\n");
			printf("(2) Show All Sellers\n");
			printf("(3) Show Total Sales in Given Duration\n");
			printf("(4) Show Sellers Sales\n");
			printf("(5) Show Shopaholics\n");
			printf("Choose an Option: ");
			scanf("%d", &option);
            printf("\n");
			
			switch(option)
			{
				case 0:
					i = 0;
					break;
        
				case 1: 
					showAllUsers(user, n);
					break;

                case 2:
					showAllSellers(user, n, item, numItems);
					break;

                case 3:
                    inputDate(date);
                    showTotalSales(temp, numCheckOuts, date);
                    break;

                case 4:
                    inputDate(date);
                    showSellerSales(temp, numCheckOuts, date, user, n);
                    break;

                case 5:
                    inputDate(date);
                    showShopaholics(temp, numCheckOuts, date, user, n);
                    break;
			}
		}
	}
}

/* showAllUsers is a function that displays a formatted list of all users.
	@param user - a pointer to an array of userTag structures that contains information about all users.
	@param n - an integer that represents the number of users in the user array.
	@returns nothing.
	Pre-condition: The user array is not empty and contains valid data for all users.
*/

void 
showAllUsers(struct userTag *user, int n)
{
    int i;

    printf("\nList of Users:\n");
    printf(" __________________________________________________________________________________________________________________________\n");
    printf("|   User ID   |      Password      |         Name         |                      Address                      |  Contact   |\n");

    for(i = 0; i < n - 1; i++) // Loops through users and prints information
    {
        printf("| %-12d| %-19s| %-21s| %-50s| %-10lld |\n",
        user[i].userID,
        user[i].password,
        user[i].name,
        user[i].address,
        user[i].contact);
    }

    printf(" __________________________________________________________________________________________________________________________\n\n");
}

/* showAllSellers is a function that displays a list of all sellers who have items for sale along with the number of items they are selling.
	@param user - a pointer to an array of userTag structures that contains information about all users in the program.
	@param n - an integer that represents the number of users in the user array.
	@param item - a pointer to an array of itemTag structures that contains information about all items available for sale.
	@param numItems - an integer that represents the number of items in the item array.
	@returns nothing.
	Pre-condition: The user and item arrays are not empty.
*/

void 
showAllSellers(struct userTag *user, int n, struct itemTag *item, int numItems)
{
    int i, j, itemCount[n];
    int sellers = 0; // Flag to check if there are any sellers with items for sale
    
    for(i = 0; i < n; i++)
    {
        itemCount[i] = 0; 
        
        for(j = 0; j < numItems; j++) // Loop to count items per user
            if(user[i].userID == item[j].userID)
                itemCount[i]++; 
        
        if(itemCount[i] > 0)
        {
            if(!sellers)
            {
                printf("\nList of Sellers:\n");
                printf(" ___________________________________________________________________________________________________________________________________________\n");
                printf("|   User ID   |      Password      |         Name         |                      Address                      |   Contact   | Items for Sale |\n");
                sellers = 1;
            }
            
            printf("| %-12d| %-19s| %-21s| %-50s| %-11lld | %-14d |\n",
                user[i].userID,
                user[i].password,
                user[i].name,
                user[i].address,
                user[i].contact,
                itemCount[i]);
        }
    }
    
    if(!sellers)
        printf("There are no registered sellers in the program.");
    else
        printf(" ___________________________________________________________________________________________________________________________________________\n\n");
}
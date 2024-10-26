#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
int adminSession = 0;
struct adminLogin
{
	char id[100];
	char pass[100];
};
struct empLogin
{
	char id[100];
	char pass[100];
};
struct empRecord
{
	char id[100];
	char name[100];
	int age;
	char email[100];
	char address[100];
	char phone[100];
	char role[100];
	char pass[100];
	int session;
};
struct custRecord
{
	char id[100];
	char name[100];
	int age;
	char email[100];
	char address[100];
	char phone[100];
	char pass[100];
	int session;
};
struct transactionRecord
{
        char id[100];
        char timestamp[100];
        char type[100];
        char aid[100];
        char cid[100];
        int amount;
};
struct accRecord
{
	char id[100];
	char cid[100];
	int balance;
	char status[100];
};
struct feedbackRecord
{
	char id[100];
	char cid[100];
	char feedback[800];
};
struct modifyEmp
{
	int choice;
	char id[100];
	int age;
	char field[100];
};
struct modifyCust
{
	int choice;
	char id[100];
	int age;
	char field[100];
};
struct updateRole
{
	char id[100];
	char role[100];
};
struct updatePassEmp
{
	char id[100];
	char pass[100];
};
struct loanApp
{
	char id[100];
	char aid[100];
	int amount;
	char eid[100];
	char status[100];
};
void showLoginType(int);
void shutdownSystem(int);
void adminLogout(int);
void adminAddEmployee(int);
void adminModifyCustomer(int);
void adminModifyEmployee(int);
void adminManageRoles(int);
void adminChangePasswordEmp(int);
void adminChangePasswordCust(int);
void empLogout(int, int);
void empAddCustomer(int);
void empChangePassword(int);
void empModifyCustomerDetails(int);
void empViewAssignedLoanApps(int);
void empProcessAssignedLoanApp(int);
void custLogout(int, int);
void custAddAccount(int);
void custViewBalance(int);
void custDeposit(int);
void custWithdraw(int);
void custTransfer(int);
void custTransactionHistory(int);
void custChangePassword(int);
void custFeedback(int);
void custApplyLoan(int);
void managerChangePassword(int);
void managerReviewFeedback(int);
void managerActivateAcc(int);
void managerDeactivateAcc(int);
void managerAssignLoanApp(int);

void adminMenu(int sock)
{
	while(1)
	{
		printf("Choose your action : \n");
		printf("1. Add new Bank Employee\n2. Modify Customer Details\n3. Modify Employee Details\n4. Manage Roles\n5. Change Password of Employee\n6. Change Password of Customer\n7. Logout\n8. Exit\n");
		printf("Enter your choice : ");
		int choice;
		scanf("%d", &choice);
		printf("\n");
		switch(choice)
		{
			case 1:adminAddEmployee(sock);
			       break;
			case 2:adminModifyCustomer(sock);
			       break;
			case 3:adminModifyEmployee(sock);
			       break;
			case 4:adminManageRoles(sock);
			       break;
			case 5:adminChangePasswordEmp(sock);
			       break;
			case 6:adminChangePasswordCust(sock);
			       break;
			case 7:adminLogout(sock);
			       break;
			case 8:shutdownSystem(sock);
			       break;
			default: printf("Invalid Choice\n");
		}
	}
}
void empMenu(int sock)
{
	while(1)
        {
                printf("Choose your action : \n");
                printf("1. Add new Customer\n2. Modify Customer Details\n3. View Assigned Loan Applications\n4. Process Assigned Loan Applications\n5. Change Password\n6. Logout\n7. Exit\n");
                printf("Enter your choice : ");
                int choice;
                scanf("%d", &choice);
                printf("\n");
                switch(choice)
                {
                        case 1:empAddCustomer(sock);
                               break;
                        case 2:empModifyCustomerDetails(sock);
			       break;
                        case 3:empViewAssignedLoanApps(sock);
                               break;
			case 4:empProcessAssignedLoanApp(sock);
                               break;
                        case 5:empChangePassword(sock);
			       break;
                        case 6:empLogout(sock, 0);
                               break;
                        case 7:empLogout(sock, 1);
			       shutdownSystem(sock);
                               break;
                        default: printf("Invalid Choice\n");
                }
        }
}
void custMenu(int sock)
{
	while(1)
        {
                printf("Choose your action : \n");
                printf("1. Open New Account\n2. View Account Balance\n3. Deposit Money\n4. Withdraw Money\n5. Transfer Money\n6. Apply for a Loan\n7. Change Password\n8. Add Feedback\n9. View Transaction History\n10. Logout\n11. Exit\n");
                printf("Enter your choice : ");
                int choice;
                scanf("%d", &choice);
                printf("\n");
                switch(choice)
                {
                        case 1:custAddAccount(sock);
                               break;
                        case 2:custViewBalance(sock);
                               break;
                        case 3:custDeposit(sock);
                               break;
                        case 4:custWithdraw(sock);
                               break;
                        case 5:custTransfer(sock);
                               break;
                        case 6:custApplyLoan(sock);
                               break;
                        case 7:custChangePassword(sock);
                               break;
                        case 8:custFeedback(sock);
                               break;
			case 9:custTransactionHistory(sock);
			       break;
			case 10:custLogout(sock, 0);
				break;
			case 11:custLogout(sock, 1);
				shutdownSystem(sock);
				break;
                        default: printf("Invalid Choice\n");
                }
        }
}
void managerMenu(int sock)
{
	while(1)
        {
                printf("Choose your action : \n");
                printf("1. Activate Customer Accounts\n2. Deactivate Customer Accounts\n3. Assign Loan Application to Employee\n4. Review Customer Feedback\n5. Change Password\n6. Logout\n7. Exit\n");
                printf("Enter your choice : ");
                int choice;
                scanf("%d", &choice);
                printf("\n");
                switch(choice)
                {
                        case 1:managerActivateAcc(sock);
                               break;
                        case 2:managerDeactivateAcc(sock);
                               break;
                        case 3:managerAssignLoanApp(sock);
                               break;
                        case 4:managerReviewFeedback(sock);
                               break;
                        case 5:managerChangePassword(sock);
                               break;
                        case 6:empLogout(sock, 0);
                               break;
                        case 7:empLogout(sock, 1);
                               shutdownSystem(sock);
                               break;
                        default: printf("Invalid Choice\n");
                }
        }
}
void adminAddEmployee(int sock)
{
	struct empRecord newemp;
	printf("\nEnter Employee Name : ");
	scanf(" %[^\n]s", newemp.name);
	printf("\nEnter Employee Age : ");
	scanf("%d", &newemp.age);
	printf("\nEnter Employee Email : ");
	scanf(" %[^\n]s", newemp.email);
	printf("\nEnter Employee Address : ");
	scanf(" %[^\n]s", newemp.address);
	printf("\nEnter Employee Phone : ");
	scanf(" %[^\n]s", newemp.phone);
	printf("\nEnter Employee Password : ");
	scanf(" %[^\n]s", newemp.pass);
	strcpy(newemp.role, "Employee");
	newemp.session = 0;

	printf("\n");
	int request_type = 101;
	write(sock, &request_type, sizeof(request_type));
        write(sock, &newemp, sizeof(newemp));

	int result;
        read(sock, &result, sizeof(result));

        if(result == -1)
        {
                printf("Employee addition has failed\n\n");
        }
        else
        {
                printf("Employee addition was successful\n\n");
        }
}
void adminModifyCustomer(int sock)
{
	struct modifyCust newData;
        printf("Enter Customer ID whose details are to be changed : ");
        scanf(" %s", newData.id);
        printf("\n");

        printf("Choose detail you want to change\n");
        printf("1. Name\n2. Age\n3. Email\n4. Address\n5. Phone\n");
        printf("Enter choice : ");
        scanf("%d", &newData.choice);
        printf("\n");

        int request_type = 102;
        write(sock, &request_type, sizeof(request_type));
        switch(newData.choice)
        {
                case 1:printf("Enter Name : ");
                       scanf(" %[^\n]s", newData.field);
                       printf("\n");
                       break;
                case 2:printf("Enter Age : ");
                       scanf("%d", &newData.age);
                       printf("\n");
                       break;
                case 3:printf("Enter Email : ");
                       scanf(" %[^\n]s", newData.field);
                       printf("\n");
                       break;
                case 4: printf("Enter Address : ");
                        scanf(" %[^\n]s", newData.field);
                        printf("\n");
                        break;
	        case 5:printf("Enter Phone : ");
                       scanf(" %[^\n]s", newData.field);
                       printf("\n");
                       break;
                default: printf("Invalid Choice\n");
                        return;
        }
        write(sock, &newData, sizeof(newData));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Customer Detail Modification successful\n");
        }
        else
        {
                printf("Customer Detail Modification failed\n");
        }
}
void adminModifyEmployee(int sock)
{
	struct modifyEmp newData;
	printf("Enter Employee ID whose details are to be changed : ");
	scanf(" %s", newData.id);
	printf("\n");

	printf("Choose detail you want to change\n");
	printf("1. Name\n2. Age\n3. Email\n4. Address\n5. Phone\n");
	printf("Enter choice : ");
	scanf("%d", &newData.choice);
	printf("\n");
	
	int request_type = 103;
	write(sock, &request_type, sizeof(request_type));
	switch(newData.choice)
	{
		case 1:printf("Enter Name : ");
		       scanf(" %[^\n]s", newData.field);
		       printf("\n");
		       break;
		case 2:printf("Enter Age : ");
		       scanf("%d", &newData.age);
		       printf("\n");
		       break;
		case 3:printf("Enter Email : ");
		       scanf(" %[^\n]s", newData.field);
		       printf("\n");
		       break;
		case 4: printf("Enter Address : ");
			scanf(" %[^\n]s", newData.field);
			printf("\n");
			break;
		case 5:printf("Enter Phone : ");
		       scanf(" %[^\n]s", newData.field);
		       printf("\n");
		       break;
		default: printf("Invalid Choice\n");
			return;
	}
	write(sock, &newData, sizeof(newData));
	int result;
	read(sock, &result, sizeof(result));
	if(result != -1)
	{
		printf("Employee Detail Modification successful\n");
	}
	else
	{
		printf("Employee Detail Modification failed\n");
	}
}
void adminManageRoles(int sock)
{
	struct updateRole new;
	printf("Enter the employee id whose role is to be changed : ");
	scanf(" %s", new.id);
	printf("\n");
	printf("Enter the new role : ");
	scanf(" %s", new.role);
	int request_type = 104;
	write(sock, &request_type, sizeof(request_type));
	write(sock, &new, sizeof(new));
	int result;
	read(sock, &result, sizeof(result));
	if(result != -1)
	{
		printf("Employee role has been successfully updated\n");
	}
	else
	{
		printf("Employee role updation failed\n");
	}
}
void adminChangePasswordEmp(int sock)
{
	struct updatePassEmp new;
        printf("Enter the employee id whose password is to be changed : ");
        scanf(" %s", new.id);
        printf("\n");
        printf("Enter the new password : ");
        scanf(" %s", new.pass);
        int request_type = 105;
        write(sock, &request_type, sizeof(request_type));
        write(sock, &new, sizeof(new));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Employee password has been successfully updated\n");
        }
        else
        {
                printf("Employee password updation failed\n");
        }
}
void adminChangePasswordCust(int sock)
{
	struct updatePassEmp new;
	printf("Enter the customer id whose password is to be changed : ");
        scanf(" %s", new.id);
        printf("\n");
        printf("Enter the new password : ");
        scanf(" %s", new.pass);
        int request_type = 106;
        write(sock, &request_type, sizeof(request_type));
        write(sock, &new, sizeof(new));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Customer password has been successfully updated\n");
        }
        else
        {
                printf("Customer password updation failed\n");
        }
}
void empAddCustomer(int sock)
{
	struct custRecord newcust;
        printf("\nEnter Customer Name : ");
        scanf(" %[^\n]s", newcust.name);
        printf("\nEnter Customer Age : ");
        scanf("%d", &newcust.age);
        printf("\nEnter Customer Email : ");
        scanf(" %[^\n]s", newcust.email);
        printf("\nEnter Customer Address : ");
        scanf(" %[^\n]s", newcust.address);
        printf("\nEnter Customer Phone : ");
        scanf(" %[^\n]s", newcust.phone);
        printf("\nEnter Customer Password : ");
        scanf(" %[^\n]s", newcust.pass);
        newcust.session = 0;

        printf("\n");
        int request_type = 201;
        write(sock, &request_type, sizeof(request_type));
        write(sock, &newcust, sizeof(newcust));

        int result;
        read(sock, &result, sizeof(result));

        if(result == -1)
        {
                printf("Customer addition has failed\n\n");
        }
        else
	{
                printf("Customer addition was successful\n\n");
        }
}
void empModifyCustomerDetails(int sock)
{
	struct modifyCust newData;
        printf("Enter Customer ID whose details are to be changed : ");
        scanf(" %s", newData.id);
        printf("\n");

        printf("Choose detail you want to change\n");
        printf("1. Name\n2. Age\n3. Email\n4. Address\n5. Phone\n");
        printf("Enter choice : ");
        scanf("%d", &newData.choice);
        printf("\n");

        int request_type = 202;
        write(sock, &request_type, sizeof(request_type));
        switch(newData.choice)
        {
                case 1:printf("Enter Name : ");
                       scanf(" %[^\n]s", newData.field);
                       printf("\n");
                       break;
                case 2:printf("Enter Age : ");
                       scanf("%d", &newData.age);
                       printf("\n");
                       break;
                case 3:printf("Enter Email : ");
                       scanf(" %[^\n]s", newData.field);
                       printf("\n");
                       break;
                case 4: printf("Enter Address : ");
                        scanf(" %[^\n]s", newData.field);
                        printf("\n");
                        break;
		case 5:printf("Enter Phone : ");
                       scanf(" %[^\n]s", newData.field);
                       printf("\n");
                       break;
                default: printf("Invalid Choice\n");
                        return;
        }
        write(sock, &newData, sizeof(newData));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Customer Detail Modification successful\n");
        }
        else
        {
                printf("Customer Detail Modification failed\n");
        }
}
void empViewAssignedLoanApps(int sock)
{
	int request_type = 203;
        struct loanApp arr[50];
        char limit[100];
        write(sock, &request_type, sizeof(request_type));
	char eid[100];
	printf("Enter your employee id: ");
	scanf(" %s", eid);
	write(sock, eid, sizeof(eid));
        read(sock, &limit, sizeof(limit));
        int lim;
        sscanf(limit, "%d", &lim);
        read(sock, &arr, sizeof(arr));
        int i;
        for(i=0;i<lim;i++)
        {
                //char datastring[600];
                //sprintf(datastring,"%s\t%s\t%s\t%s\t%s\t%d", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
                printf("%s\t%s\t%d\t%s\t%s", arr[i].id, arr[i].aid, arr[i].amount, arr[i].eid, arr[i].status);
                //printf("%s\n", datastring);
        }
        printf("\n");
}
void empProcessAssignedLoanApp(int sock)
{
	int request_type = 204;
        write(sock, &request_type, sizeof(request_type));
	char lid[100];
	printf("Enter Loan App ID : ");
	scanf(" %s", lid);
        write(sock, &lid, sizeof(lid));
        char result[100];
        read(sock, &result, sizeof(result));
        if(strcmp(result, "0") == 0)
        {
                printf("Loan ID : %s has been approved\n", lid);
        }
        else
        {
                printf("Loan ID : %s has been rejected\n", lid);
        }
}
void empChangePassword(int sock)
{
	struct updatePassEmp new;
        printf("Enter your employee id : ");
        scanf(" %s", new.id);
        printf("\n");
        printf("Enter the new password : ");
        scanf(" %s", new.pass);
        int request_type = 206;
        write(sock, &request_type, sizeof(request_type));
        write(sock, &new, sizeof(new));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Your password has been successfully updated\n");
        }
        else
        {
                printf("Your password updation failed\n");
        }

}
void custAddAccount(int sock)
{
	struct accRecord data;
	int request_type = 301;
	data.balance = 10000;
	write(sock, &request_type, sizeof(request_type));
        write(sock, &data, sizeof(data));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Account has been successfully created\n");
		char aid[100];
		read(sock, aid, sizeof(aid));
		printf("Account ID : %s\n", aid);
        }
        else
        {
                printf("Account creation failed\n");
        }
}
void custViewBalance(int sock)
{
	int request_type = 302;
	char aid[100];
	printf("Enter A/C Id : ");
	scanf("%s",aid);
	printf("\n");
	write(sock, &request_type, sizeof(request_type));
	write(sock, aid, sizeof(aid));
	char balance[100];
	read(sock, balance, sizeof(balance));
	int bal;
	sscanf(balance, "%d", &bal);
	if(bal == -1)
		printf("Account not found in your accounts\n");
	else
		printf("Your Account Balance : %d\n",bal);
}
void custDeposit(int sock)
{
	int request_type = 303;
	char aid[100];
	printf("Enter A/C Id : ");
	scanf(" %s", aid);
	char amt[100];
	printf("Enter Amount : ");
	scanf(" %s", amt);
	write(sock, &request_type, sizeof(request_type));
	write(sock, aid, sizeof(aid));
	write(sock, amt, sizeof(amt));
	char result[100];
	int res;
	read(sock, &result, sizeof(result));
	scanf(result, "%d", &res);
	if(res == -1)
		printf("Deposit failed\n");
	else
		printf("Deposit successful\n");
}
void custWithdraw(int sock)
{
	int request_type = 304;
	char aid[100];
	printf("Enter A/C Id : ");
	scanf(" %s", aid);
	char amt[100];
	printf("Enter Amount : ");
	scanf(" %s", amt);
	write(sock, &request_type, sizeof(request_type));
        write(sock, aid, sizeof(aid));
        write(sock, amt, sizeof(amt));
        char result[100];
        read(sock, &result, sizeof(result));
        if(strcmp(result, "-1") == 0)
                printf("Withdrawal failed due to account not being a valid account of yours\n");
        else
                printf("Withdrawal successful\n");
}
void custTransfer(int sock)
{
	int request_type = 305;
	char cid[100];
	printf("Enter your Customer ID : ");
	scanf(" %s", cid);
        char aid1[100];
        printf("Enter A/C Id 1: ");
        scanf(" %s", aid1);
	char aid2[100];
	printf("Enter A/C Id 2 : ");
	scanf(" %s", aid2);
        char amt[100];
        printf("Enter Amount : ");
        scanf(" %s", amt);
        write(sock, &request_type, sizeof(request_type));
	write(sock, cid, sizeof(cid));
        write(sock, aid1, sizeof(aid1));
	write(sock, aid2, sizeof(aid2));
        write(sock, amt, sizeof(amt));
        char result[100];
        read(sock, &result, sizeof(result));
        if(strcmp(result, "-1") == 0)
                printf("Transfer failed due to account not being a valid account of yours or not enough funds\n");
        else
                printf("Transfer successful\n");
}
void custTransactionHistory(int sock)
{
	int request_type = 309;
	struct transactionRecord arr[100];
	char limit[100];
	printf("Enter your customer id for authentication : ");
	char cid[100];
	scanf(" %s", cid);
	write(sock, &request_type, sizeof(request_type));
	write(sock, &cid, sizeof(cid));
	//memset(limit, '\0', sizeof(limit));
	read(sock, &limit, sizeof(limit));
	//perror("Error");
	//limit[100]='\0';
	int lim;
	sscanf(limit, "%d", &lim);
	read(sock, &arr, sizeof(arr));
	//printf("ID = %d\n", lim);
	int i;
	for(i=0;i<lim;i++)
	{
		//char datastring[600];
		//sprintf(datastring,"%s\t%s\t%s\t%s\t%s\t%d", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
		printf("%s\t%s\t%s\t%s\t%s\t%d\n", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
		//printf("%s\n", datastring);
	}
	printf("\n");
}
void custChangePassword(int sock)
{
	struct updatePassEmp new;
        printf("Enter your employee id : ");
        scanf(" %s", new.id);
        printf("\n");
        printf("Enter the new password : ");
        scanf(" %s", new.pass);
        int request_type = 307;
        write(sock, &request_type, sizeof(request_type));
        write(sock, &new, sizeof(new));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Your password has been successfully updated\n");
        }
        else
        {
                printf("Your password updation failed\n");
        }
}
void custFeedback(int sock)
{
	char feedback[800];
	printf("Enter your feedback (Max 800 chars.) : \n");
	scanf(" %[^\n]s", feedback);
	int request_type = 308;
	write(sock, &request_type, sizeof(request_type));
	write(sock, feedback, sizeof(feedback));
	int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Your feedback has been successfully recorded\n");
        }
        else
        {
                printf("Some error occurred while recording your feedback\n");
        }
}
void custApplyLoan(int sock)
{
	int request_type = 306;
	struct loanApp data;
	printf("Enter loan amount : ");
	scanf("%d", &data.amount);
	printf("Enter account id to take loan against : ");
	scanf("%s", data.aid);
	write(sock, &request_type, sizeof(request_type));
        write(sock, &data, sizeof(data));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Loan successfully applied\n");
        }
        else
        {
                printf("Some error occurred while applying for your loan\n");
        }
}
void customerLogin(int sock)
{
	int request_type = 300;
        struct empLogin data;
        printf("Enter User ID : ");
        scanf(" %s", data.id);
        printf("\n");
        printf("Enter Password : ");
        scanf(" %s", data.pass);

        write(sock, &request_type, sizeof(request_type));
        write(sock, &data, sizeof(data));

        int result;
        read(sock, &result, sizeof(result));

        if(result == 0)
        {
                printf("Invalid customer login credentials\n\n");
        }
        else
        {
                printf("Successfully logged in as Customer\n\n");
		custMenu(sock);
        }
}
void employeeLogin(int sock)
{
	int request_type = 200;
        struct empLogin data;
        printf("Enter User ID : ");
        scanf(" %s", data.id);
        printf("\n");
        printf("Enter Password : ");
        scanf(" %s", data.pass);

        write(sock, &request_type, sizeof(request_type));
        write(sock, &data, sizeof(data));

        int result;
        read(sock, &result, sizeof(result));

        if(result == 0)
        {
                printf("Invalid employee login credentials\n\n");
        }
        else
        {
                printf("Successfully logged in as Employee\n\n");
		empMenu(sock);
        }
}
void managerLogin(int sock)
{
	int request_type = 400;
        struct empLogin data;
        printf("Enter User ID : ");
        scanf(" %s", data.id);
        printf("\n");
        printf("Enter Password : ");
        scanf(" %s", data.pass);

        write(sock, &request_type, sizeof(request_type));
        write(sock, &data, sizeof(data));

        int result;
        read(sock, &result, sizeof(result));

        if(result == 0)
        {
                printf("Invalid manager login credentials\n\n");
        }
        else
        {
                printf("Successfully logged in as manager\n\n");
		managerMenu(sock);
        }
}
void managerChangePassword(int sock)
{
        struct updatePassEmp new;
        printf("Enter your employee id : ");
        scanf(" %s", new.id);
        printf("\n");
        printf("Enter the new password : ");
        scanf(" %s", new.pass);
        int request_type = 405;
        write(sock, &request_type, sizeof(request_type));
        write(sock, &new, sizeof(new));
        int result;
        read(sock, &result, sizeof(result));
        if(result != -1)
        {
                printf("Your password has been successfully updated\n");
        }
        else
        {
                printf("Your password updation failed\n");
        }

}
void managerReviewFeedback(int sock)
{
	int request_type = 404;
        struct feedbackRecord arr[50];
        char limit[100];
        write(sock, &request_type, sizeof(request_type));
	read(sock, &limit, sizeof(limit));
        int lim;
        sscanf(limit, "%d", &lim);
        read(sock, &arr, sizeof(arr));
        int i;
        for(i=0;i<lim;i++)
        {
                //char datastring[600];
                //sprintf(datastring,"%s\t%s\t%s\t%s\t%s\t%d", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
                printf("%s\t%s\t%s\n", arr[i].id, arr[i].cid, arr[i].feedback);
                //printf("%s\n", datastring);
        }
        printf("\n");
}
void managerActivateAcc(int sock)
{
	int request_type = 401;
        printf("Enter Acc ID : ");
        char aid[100];
	scanf(" %s", aid);
        printf("\n");

        write(sock, &request_type, sizeof(request_type));
        write(sock, &aid, sizeof(aid));

	char result[100];
        read(sock, result, sizeof(result));
        if(strlen(result) != 0)
        {
                printf("Error activating account\n\n");
        }
        else
        {
                printf("Successfully activated account\n\n");
        }

}
void managerDeactivateAcc(int sock)
{
        int request_type = 402;
        printf("Enter Acc ID : ");
        char aid[100];
        scanf(" %s", aid);
        printf("\n");

        write(sock, &request_type, sizeof(request_type));
        write(sock, &aid, sizeof(aid));

        int result;
        read(sock, &result, sizeof(result));

        if(result == 0)
        {
                printf("Error deactivating account\n\n");
        }
        else
        {
                printf("Successfully deactivated account\n\n");
        }

}
void managerAssignLoanApp(int sock)
{
	int request_type = 403;
        printf("Enter Loan App ID : ");
        char lid[100];
        scanf(" %s", lid);
        printf("\n");
	printf("Enter employedd id to assign to : ");
	char eid[100];
	scanf(" %s", eid);
	printf("\n");

        write(sock, &request_type, sizeof(request_type));
        write(sock, &lid, sizeof(lid));
	write(sock, &eid, sizeof(eid));

        char result[100];
        read(sock, &result, sizeof(result));

        if(strcmp(result, "0") != 0)
        {
                printf("Error assigning loan\n\n");
        }
        else
        {
                printf("Successfully assigned loan\n\n");
        }
}
void adminLogin(int sock)
{
	int request_type = 100;
	struct adminLogin data;
	printf("Enter User ID : ");
	scanf("%s", data.id);
	printf("\n");
	printf("Enter Password : ");
	scanf("%s", data.pass);

	write(sock, &request_type, sizeof(request_type));
	write(sock, &data, sizeof(data));
	
	int result;
	read(sock, &result, sizeof(result));

	if(result == 0)
	{
		printf("Invalid admin login credentials\n\n");
	}
	else
	{
		printf("Successfully logged in as Admin\n\n");
		adminMenu(sock);
	}
}
void adminLogout(int sock)
{
	int request_type = 107;
	write(sock, &request_type, sizeof(request_type));
	showLoginType(sock);
}
void empLogout(int sock, int type)
{
        int request_type = 207;
        write(sock, &request_type, sizeof(request_type));
	if(!type)
        	showLoginType(sock);
}
void custLogout(int sock, int type)
{
	int request_type = 310;
	write(sock, &request_type, sizeof(request_type));
	if(!type)
		showLoginType(sock);
}
void shutdownSystem(int sock)
{
	int request_type = 500;
        write(sock, &request_type, sizeof(request_type));
	sleep(1);
	exit(0);
}
void showLoginType(int sock)
{
	while(1)
	{
		printf("Choose your login type\n");
		printf("1. Customer\n2. Employee\n3. Manager\n4. Admin\n5. Exit\n");
		printf("\nEnter your choice : ");
		int choice;
		scanf("%d", &choice);
		if(choice<1 || choice >5)
		{
			printf("Invalid Choice\n\n");
			showLoginType(sock);
		}
		else
		{
			printf("\n");
			switch(choice)
			{
				case 1: customerLogin(sock);
					break;
				case 2: employeeLogin(sock);
					break;
				case 3: managerLogin(sock);
					break;
				case 4: adminLogin(sock);
					break;
				case 5: shutdownSystem(sock);
					break;
			}
		}
	}
}
int main()
{
	struct sockaddr_in server;
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8888);
	connect(sock, (struct sockaddr *)&server, sizeof(server));
	showLoginType(sock);
}

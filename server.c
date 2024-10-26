#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/signal.h>
#include<time.h>
int maxcustid = 10000;
int maxempid = 20000;
int maxaccid = 30000;
int maxtranid = 40000;
int maxfbid = 50000;
int maxloanid = 60000;
int adminSession = 0;
char empId[100];
char custId[100];
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
struct accRecord
{
	char id[100];
	char cid[100];
	int balance;
	char status[100];
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
void adminLogin(int conn)
{
	struct adminLogin data;
        read(conn, &data, sizeof(data));
        int result;
        if(adminSession == 1 || strcmp(data.id, "1000") != 0 || strcmp(data.pass, "#@4m1N#") != 0)
        	result = 0;
        else
                result = 1;
       write(conn, &result, sizeof(result));
}
void adminAddEmployee(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
	char datastring[800];
	struct empRecord oldData, data;
	struct flock lock;
	int pk;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
	lseek(fd, -800, SEEK_END);
        int result = read(fd, datastring, sizeof(datastring));
	printf("Result = %d\n",result);
	if(result != 0)
	{	
		//printf("%s\n", datastring);
		sscanf(datastring, "%d\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", &pk, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.role, oldData.pass, &oldData.session);
		pk++;
	}
	else
	{
		pk = 20000;
	}
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
	
	int offset = pk - 20000;
	lseek(fd, offset * 800, SEEK_SET);
        read(conn, &data, sizeof(data));
	sprintf(data.id, "%d", pk);
        memset(datastring, '\0', sizeof(datastring));
        sprintf(datastring, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", data.id, data.name, data.age, data.email, data.address, data.phone, data.role, data.pass, data.session);
	datastring[799] = '\n';
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = offset * 800;
	lock.l_len = 800;
	lock.l_pid = getpid();

	fcntl(fd, F_SETLK, &lock);
	result = write(fd, datastring, sizeof(datastring));
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	
	write(conn, &result, sizeof(result));
}
void adminModifyCustomer(int conn)
{
	int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        char datastring2[700];
        struct custRecord oldData, newData;
        struct modifyCust changedData;

        read(conn, &changedData, sizeof(changedData));
        int offset;
        sscanf(changedData.id, "%d", &offset);
        offset = offset - 10000;
        printf("offset = %d\n", offset);
        //printf("choice = %d\n", changedData.choice);
        //printf("field = %s\n", changedData.field);
        //printf("age = %d\n", changedData.age);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 700, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
	}
	sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.pass, &oldData.session);

        newData = oldData;
        switch(changedData.choice)
        {
                case 1: strcpy(newData.name, changedData.field);
                        break;
                case 2: newData.age = changedData.age;
                        break;
                case 3: strcpy(newData.email, changedData.field);
                        break;
                case 4: strcpy(newData.address, changedData.field);
                        break;
                case 5: strcpy(newData.phone, changedData.field);
                        break;
        }
        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[699] = '\n';
        lseek(fd, offset * 700, SEEK_SET);
        char empty[700];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 700, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
	close(fd);

        write(conn, &result ,sizeof(result));
}
void adminModifyEmployee(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
	char datastring[800];
	char datastring2[800];
	struct empRecord oldData, newData;
	struct modifyEmp changedData;
	
	read(conn, &changedData, sizeof(changedData));
	int offset;
	sscanf(changedData.id, "%d", &offset);
	offset = offset - 20000;
	printf("offset = %d\n", offset);
	printf("choice = %d\n", changedData.choice);
	printf("field = %s\n", changedData.field);
	//printf("age = %d\n", changedData.age);

	struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        
	lseek(fd, offset * 800, SEEK_SET);
	int result = read(fd, datastring, sizeof(datastring));
	if(result == -1)
	{
		write(conn, &result, sizeof(result));
		close(fd);
		return;
	}
	sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.role, oldData.pass, &oldData.session);
	
	newData = oldData;
	switch(changedData.choice)
	{
		case 1: strcpy(newData.name, changedData.field);
			break;
		case 2: newData.age = changedData.age;
			break;
		case 3: strcpy(newData.email, changedData.field);
			break;
		case 4: strcpy(newData.address, changedData.field);
			break;
		case 5: strcpy(newData.phone, changedData.field);
			break;
	}
	memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.role, newData.pass, newData.session);
	printf("Final update string : %s\n", datastring2);
	datastring2[799] = '\n';
        lseek(fd, offset * 800, SEEK_SET);
	char empty[800];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 800, SEEK_SET);
	result = write(fd, datastring2, sizeof(datastring2));
	printf("Result = %d\n", result);        
	lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock); 
        close(fd);

	write(conn, &result ,sizeof(result));
}
void adminManageRoles(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord oldData, newData;
        
	struct updateRole new;
        read(conn, &new, sizeof(new));
	int offset;
        sscanf(new.id, "%d", &offset);
        offset = offset - 20000;
        //printf("offset = %d\n", offset);
	//printf("role = %s\n", new.role);
	
	struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.role, oldData.pass, &oldData.session);

        newData = oldData;
	strcpy(newData.role, new.role);
	
	memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.role, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[799] = '\n';
        lseek(fd, offset * 800, SEEK_SET);
	char empty[800];
	memset(empty, '\0', sizeof(empty));
	write(fd, empty, sizeof(empty));
	lseek(fd, offset * 800, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result ,sizeof(result));
}
void adminUpdatePassEmp(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord oldData, newData;

        struct updatePassEmp new;
        read(conn, &new, sizeof(new));
        int offset;
        sscanf(new.id, "%d", &offset);
        offset = offset - 20000;
        //printf("offset = %d\n", offset);
        //printf("pass = %s\n", new.role);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
	if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.role, oldData.pass, &oldData.session);

        newData = oldData;
        strcpy(newData.pass, new.pass);

        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.role, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[799] = '\n';
        lseek(fd, offset * 800, SEEK_SET);
        char empty[800];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 800, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result ,sizeof(result));
}
void adminUpdatePassCust(int conn)
{
	int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        char datastring2[700];
        struct empRecord oldData, newData;

        struct updatePassEmp new;
        read(conn, &new, sizeof(new));
        int offset;
        sscanf(new.id, "%d", &offset);
        offset = offset - 10000;
        //printf("offset = %d\n", offset);
        //printf("pass = %s\n", new.role);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 700, SEEK_SET);
	int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.pass, &oldData.session);

        newData = oldData;
        strcpy(newData.pass, new.pass);

        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[699] = '\n';
        lseek(fd, offset * 700, SEEK_SET);
        char empty[700];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 700, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result ,sizeof(result));
}
void adminLogout(int conn)
{
	adminSession = 0;
}
void empLogin(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord record;

        struct empLogin data;
        read(conn, &data, sizeof(data));
	int offset;
        sscanf(data.id, "%d", &offset);
        offset = offset - 20000;
	
	struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
		result = 0;
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  record.id, record.name, &record.age, record.email, record.address, record.phone, record.role, record.pass, &record.session);
        
        if(record.session == 1 || strcmp(data.id, record.id) != 0 || strcmp(data.pass, record.pass) != 0)
                result = 0;
        else
	{
        	strcpy(empId, data.id);
		result = 1;
		record.session = 1;
		memset(datastring2, '\0', sizeof(datastring2));
        	sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", record.id, record.name, record.age, record.email, record.address, record.phone, record.role, record.pass, record.session);
        	printf("Final update string : %s\n", datastring2);
        	datastring2[799] = '\n';
        	lseek(fd, offset * 800, SEEK_SET);
        	char empty[800];
        	memset(empty, '\0', sizeof(empty));
        	write(fd, empty, sizeof(empty));
        	lseek(fd, offset * 800, SEEK_SET);
        	write(fd, datastring2, sizeof(datastring2));
        	printf("Result = %d\n", result);
        	lock.l_type = F_UNLCK;
        	fcntl(fd, F_SETLK, &lock);
        	close(fd);
	}
       write(conn, &result, sizeof(result));
}
void empAddCustomer(int conn)
{
        int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        struct custRecord oldData, data;
        struct flock lock;
        int pk;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -700, SEEK_END);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%d\t%s\t%d\t%s\t%s\t%s\t%s\t%d", &pk, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.pass, &oldData.session);
                pk++;
        }
        else
        {
                pk = 10000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
	
	int offset = pk - 10000;
        lseek(fd, offset * 700, SEEK_SET);
        read(conn, &data, sizeof(data));
        sprintf(data.id, "%d", pk);
        memset(datastring, '\0', sizeof(datastring));
        sprintf(datastring, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", data.id, data.name, data.age, data.email, data.address, data.phone, data.pass, data.session);
        datastring[699] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        result = write(fd, datastring, sizeof(datastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result, sizeof(result));
}
void empModifyCustomer(int conn)
{
	int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        char datastring2[700];
        struct custRecord oldData, newData;
        struct modifyCust changedData;

        read(conn, &changedData, sizeof(changedData));
        int offset;
        sscanf(changedData.id, "%d", &offset);
        offset = offset - 10000;
        printf("offset = %d\n", offset);
        //printf("choice = %d\n", changedData.choice);
        //printf("field = %s\n", changedData.field);
        //printf("age = %d\n", changedData.age);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 700, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
	}
	sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.pass, &oldData.session);

        newData = oldData;
        switch(changedData.choice)
        {
                case 1: strcpy(newData.name, changedData.field);
                        break;
                case 2: newData.age = changedData.age;
                        break;
                case 3: strcpy(newData.email, changedData.field);
                        break;
                case 4: strcpy(newData.address, changedData.field);
                        break;
                case 5: strcpy(newData.phone, changedData.field);
                        break;
        }
        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[699] = '\n';
        lseek(fd, offset * 700, SEEK_SET);
        char empty[700];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 700, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
	close(fd);

        write(conn, &result ,sizeof(result));
}
void empChangePassword(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord oldData, newData;

        struct updatePassEmp new;
        read(conn, &new, sizeof(new));
	if(strcmp(new.id, empId) != 0)
	{
		int res = -1;
		write(conn, &res, sizeof(res));
		return;
	}

        int offset;
        sscanf(new.id, "%d", &offset);
        offset = offset - 20000;
        //printf("offset = %d\n", offset);
        //printf("pass = %s\n", new.role);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
	
	lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.role, oldData.pass, &oldData.session);

        newData = oldData;
        strcpy(newData.pass, new.pass);

        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.role, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[799] = '\n';
        lseek(fd, offset * 800, SEEK_SET);
        char empty[800];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 800, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
	
	write(conn, &result, sizeof(result));
}
void empLogout(int conn)
{
	//strcpy(empId, "");
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord record;

        //read(conn, &data, sizeof(data));
        int offset;
        sscanf(empId, "%d", &offset);
	offset = offset - 20000;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
	sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  record.id, record.name, &record.age, record.email, record.address, record.phone, record.role, record.pass, &record.session);
	record.session = 0;
	
	memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", record.id, record.name, record.age, record.email, record.address, record.phone, record.role, record.pass, record.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[799] = '\n';
        lseek(fd, offset * 800, SEEK_SET);
        char empty[800];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 800, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

	strcpy(empId, "");	
	write(conn, &result, sizeof(result));
}
void custLogin(int conn)
{
	int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        char datastring2[700];
        struct custRecord record;

        struct empLogin data;
        read(conn, &data, sizeof(data));
        int offset;
        sscanf(data.id, "%d", &offset);
        offset = offset - 10000;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 700, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                result = 0;
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  record.id, record.name, &record.age, record.email, record.address, record.phone, record.pass, &record.session);
	if(record.session == 1 || strcmp(data.id, record.id) != 0 || strcmp(data.pass, record.pass) != 0)
                result = 0;
        else
        {
                strcpy(custId, data.id);
                result = 1;
                record.session = 1;
                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", record.id, record.name, record.age, record.email, record.address, record.phone, record.pass, record.session);
                printf("Final update string : %s\n", datastring2);
                datastring2[699] = '\n';
                lseek(fd, offset * 700, SEEK_SET);
                char empty[700];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 700, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
       write(conn, &result, sizeof(result));
}
void custLogout(int conn)
{
	int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        char datastring2[700];
        struct custRecord record;

        //read(conn, &data, sizeof(data));
        int offset;
        sscanf(custId, "%d", &offset);
        offset = offset - 10000;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 700, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  record.id, record.name, &record.age, record.email, record.address, record.phone, record.pass, &record.session);
        record.session = 0;

        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", record.id, record.name, record.age, record.email, record.address, record.phone, record.pass, record.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[699] = '\n';
	lseek(fd, offset * 700, SEEK_SET);
        char empty[700];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 700, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        strcpy(custId, "");
        write(conn, &result, sizeof(result));
}
void custAddAccount(int conn)
{
	int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
        struct accRecord oldData, data;
        struct flock lock;
        int pk;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -400, SEEK_END);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%d\t%s\t%d\t%s", &pk, oldData.cid, &oldData.balance, oldData.status);
                pk++;
        }
        else
        {
                pk = 30000;
        }
	lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        int offset = pk - 30000;
        lseek(fd, offset * 400, SEEK_SET);
        read(conn, &data, sizeof(data));
        sprintf(data.id, "%d", pk);
	strcpy(data.cid, custId);
	data.balance = 10000;
	strcpy(data.status, "Active");
        memset(datastring, '\0', sizeof(datastring));
        sprintf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
        datastring[399] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        result = write(fd, datastring, sizeof(datastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

	fd = open("transaction_records", O_RDWR, 0);
	struct transactionRecord oldtdata, tdata;
	char tdatastring[600];
	lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -600, SEEK_END);
        int result2 = read(fd, tdatastring, sizeof(tdatastring));
        printf("Result = %d\n",result2);
        if(result2 != 0)
        {
                //printf("%s\n", datastring);
                sscanf(tdatastring, "%d\t%s\t%s\t%s\t%s\t%d", &pk, oldtdata.timestamp, oldtdata.type, oldtdata.aid, oldtdata.cid, &oldtdata.amount);
                pk++;
        }
        else
        {
                pk = 40000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

	int toffset = pk - 40000;
	time_t curTime;
	time(&curTime);

        lseek(fd, toffset * 600, SEEK_SET);
        
	sprintf(tdata.id, "%d", pk);
        strcpy(tdata.timestamp, ctime(&curTime));
        strcpy(tdata.type, "Opened A/C");
	sprintf(tdata.aid, "%d", (offset+30000));
	tdata.amount = 10000;
	strcpy(tdata.cid, custId);
	memset(tdatastring, '\0', sizeof(tdatastring));
        sprintf(tdatastring, "%s\t%s\t%s\t%s\t%s\t%d", tdata.id, tdata.timestamp, tdata.type, tdata.aid, tdata.cid, tdata.amount);
        tdatastring[599] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = toffset * 600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        result2 = write(fd, tdatastring, sizeof(tdatastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

	if(result2 == -1)
		result == -1;

        write(conn, &result, sizeof(result));
	write(conn, data.id, sizeof(data.id));
}
void custViewBalance(int conn)
{
	char aid[100];
	read(conn, aid, sizeof(aid));
	int offset;
	sscanf(aid, "%d", &offset);
	offset = offset - 30000;

	int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
        struct accRecord data;
        struct flock lock;
        
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 400, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
	char balance[100];
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
		if(strcmp(custId, data.cid) != 0)
		{
			sprintf(balance, "%d", -1);
			write(conn, balance, sizeof(balance));
			return;
		}
		sprintf(balance, "%d", data.balance);
        }
	else
	{
		sprintf(balance, "%d", -1);
	}
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
	write(conn, balance, sizeof(balance));
}
void custDeposit(int conn)
{
	char aid[100];
	int pk;
	read(conn, aid, sizeof(aid));
	int offset;
        sscanf(aid, "%d", &offset);
        offset = offset - 30000;
	char amt[100];
	read(conn, amt, sizeof(amt));

        int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
	char datastring2[400];
        struct accRecord data;
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 400, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
	char response[100];
	int bal;
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
		if(strcmp(data.status, "Active") != 0)
		{
			strcpy(response, "-1");
			write(conn, response, sizeof(response));
			return;
		}
                sprintf(response, "%d", 0);		
		sscanf(amt, "%d", &bal);
		data.balance += bal;
		memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[399] = '\n';
                lseek(fd, offset * 400, SEEK_SET);
                char empty[400];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 400, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                sprintf(response, "%d", -1);
        }

	fd = open("transaction_records", O_RDWR, 0);
        struct transactionRecord oldtdata, tdata;
        char tdatastring[600];
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -600, SEEK_END);
        int result2 = read(fd, tdatastring, sizeof(tdatastring));
        printf("Result = %d\n",result2);
        if(result2 != 0)
        {
                //printf("%s\n", datastring);
                sscanf(tdatastring, "%d\t%s\t%s\t%s\t%s\t%d", &pk, oldtdata.timestamp, oldtdata.type, oldtdata.aid, oldtdata.cid, &oldtdata.amount);
                pk++;
        }
        else
        {
                pk = 40000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        int toffset = pk - 40000;
        time_t curTime;
        time(&curTime);

	lseek(fd, toffset * 600, SEEK_SET);

        sprintf(tdata.id, "%d", pk);
        strcpy(tdata.timestamp, ctime(&curTime));
        strcpy(tdata.type, "Deposit");
        sprintf(tdata.aid, "%d", (offset+30000));
        tdata.amount = bal;
        strcpy(tdata.cid, custId);
        memset(tdatastring, '\0', sizeof(tdatastring));
        sprintf(tdatastring, "%s\t%s\t%s\t%s\t%s\t%d", tdata.id, tdata.timestamp, tdata.type, tdata.aid, tdata.cid, tdata.amount);
        tdatastring[599] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = toffset * 600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        result2 = write(fd, tdatastring, sizeof(tdatastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, response, sizeof(response));
}
void custWithdrawal(int conn)
{
	char aid[100];
	int pk;
        read(conn, aid, sizeof(aid));
        int offset;
        sscanf(aid, "%d", &offset);
        offset = offset - 30000;
        char amt[100];
        read(conn, amt, sizeof(amt));

        int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
        char datastring2[400];
        struct accRecord data;
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 400, SEEK_SET);
	int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        char response[100];
	int bal;
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
		if(strcmp(data.cid, custId) != 0)
		{
			strcpy(response, "-1");
			write(conn, response, sizeof(response));
			return;
		}
		if(strcmp(data.status, "Active") != 0)
                {
                        strcpy(response, "-1");
                        write(conn, response, sizeof(response));
                        return;
                }
                sprintf(response, "%d", 0);
                sscanf(amt, "%d", &bal);
                data.balance -= bal;
		if(data.balance < 0)
			data.balance = 0;
				
                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[399] = '\n';
                lseek(fd, offset * 400, SEEK_SET);
                char empty[400];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 400, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                sprintf(response, "%d", -1);
        }
        
	fd = open("transaction_records", O_RDWR, 0);
        struct transactionRecord oldtdata, tdata;
        char tdatastring[600];
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -600, SEEK_END);
        int result2 = read(fd, tdatastring, sizeof(tdatastring));
        printf("Result = %d\n",result2);
        if(result2 != 0)
        {
                //printf("%s\n", datastring);
                sscanf(tdatastring, "%d\t%s\t%s\t%s\t%s\t%d", &pk, oldtdata.timestamp, oldtdata.type, oldtdata.aid, oldtdata.cid, &oldtdata.amount);
                pk++;
        }
        else
        {
                pk = 40000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
	
	int toffset = pk - 40000;
        time_t curTime;
        time(&curTime);

        lseek(fd, toffset * 600, SEEK_SET);

        sprintf(tdata.id, "%d", pk);
        strcpy(tdata.timestamp, ctime(&curTime));
        strcpy(tdata.type, "Withdrawal");
        sprintf(tdata.aid, "%d", (offset+30000));
        tdata.amount = bal;
        strcpy(tdata.cid, custId);
        memset(tdatastring, '\0', sizeof(tdatastring));
        sprintf(tdatastring, "%s\t%s\t%s\t%s\t%s\t%d", tdata.id, tdata.timestamp, tdata.type, tdata.aid, tdata.cid, tdata.amount);
        tdatastring[599] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = toffset * 600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        result2 = write(fd, tdatastring, sizeof(tdatastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
	
	write(conn, response, sizeof(response));
}
void custTransfer(int conn)
{
	//Withdrawal Part
	char cid[100], aid1[100], aid2[100];
        int pk;
	read(conn, cid, sizeof(cid));
        read(conn, aid1, sizeof(aid1));
	read(conn, aid2, sizeof(aid2));
        int offset1, offset2;
        sscanf(aid1, "%d", &offset1);
	sscanf(aid2, "%d", &offset2);
        offset1 = offset1 - 30000;
	offset2 = offset2 - 30000;
        char amt[100];
        read(conn, amt, sizeof(amt));

        int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
        char datastring2[400];
        struct accRecord data;
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset1 * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
	lseek(fd, offset1 * 400, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        char response[100];
        int bal;
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
                if(strcmp(data.cid, cid) != 0)
                {
                        strcpy(response, "-1");
                        write(conn, response, sizeof(response));
                        return;
                }
                if(strcmp(data.status, "Active") != 0)
                {
                        strcpy(response, "-1");
                        write(conn, response, sizeof(response));
                        return;
                }
                sscanf(amt, "%d", &bal);
                data.balance -= bal;
                if(data.balance < 0)
		{
			strcpy(response, "-1");
			data.balance += bal;
			write(conn, response, sizeof(response));
			return;
		}

                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[399] = '\n';
                lseek(fd, offset1 * 400, SEEK_SET);
                char empty[400];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset1 * 400, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                sprintf(response, "%d", -1);
        }

        fd = open("transaction_records", O_RDWR, 0);
        struct transactionRecord oldtdata, tdata;
        char tdatastring[600];
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -600, SEEK_END);
        int result2 = read(fd, tdatastring, sizeof(tdatastring));
        printf("Result = %d\n",result2);
        if(result2 != 0)
       {
                //printf("%s\n", datastring);
                sscanf(tdatastring, "%d\t%s\t%s\t%s\t%s\t%d", &pk, oldtdata.timestamp, oldtdata.type, oldtdata.aid, oldtdata.cid, &oldtdata.amount);
                pk++;
        }
        else
        {
                pk = 40000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        int toffset = pk - 40000;
        time_t curTime;
        time(&curTime);

        lseek(fd, toffset * 600, SEEK_SET);

        sprintf(tdata.id, "%d", pk);
        strcpy(tdata.timestamp, ctime(&curTime));
        strcpy(tdata.type, "Withdrawal");
        sprintf(tdata.aid, "%d", (offset1+30000));
        tdata.amount = bal;
        strcpy(tdata.cid, cid);
        memset(tdatastring, '\0', sizeof(tdatastring));
        sprintf(tdatastring, "%s\t%s\t%s\t%s\t%s\t%d", tdata.id, tdata.timestamp, tdata.type, tdata.aid, tdata.cid, tdata.amount);
        tdatastring[599] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;

	lock.l_len = 600;
        lock.l_pid = getpid();
	lock.l_start = toffset * 600;
        fcntl(fd, F_SETLK, &lock);
        result2 = write(fd, tdatastring, sizeof(tdatastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);


	//Deposit Part
	fd = open("account_records", O_RDWR, 0);

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset2 * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset2 * 400, SEEK_SET);
        result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
		if(strcmp(data.status, "Active") != 0)
                {
                        strcpy(response, "-1");
                        write(conn, response, sizeof(response));
                        return;
                }

                sscanf(amt, "%d", &bal);
              	data.balance += bal;

                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[399] = '\n';
                lseek(fd, offset2 * 400, SEEK_SET);
                char empty[400];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset2 * 400, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                sprintf(response, "%d", -1);
        }

        fd = open("transaction_records", O_RDWR, 0);
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -600;
        lock.l_len = 600;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -600, SEEK_END);
        result2 = read(fd, tdatastring, sizeof(tdatastring));
        printf("Result = %d\n",result2);
        if(result2 != 0)
       {
                //printf("%s\n", datastring);
                sscanf(tdatastring, "%d\t%s\t%s\t%s\t%s\t%d", &pk, oldtdata.timestamp, oldtdata.type, oldtdata.aid, oldtdata.cid, &oldtdata.amount);
                pk++;
        }
        else
        {
                pk = 40000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        toffset = pk - 40000;
        time(&curTime);

        lseek(fd, toffset * 600, SEEK_SET);

        sprintf(tdata.id, "%d", pk);
        strcpy(tdata.timestamp, ctime(&curTime));
        strcpy(tdata.type, "Deposit");
        sprintf(tdata.aid, "%d", (offset2+30000));
        tdata.amount = bal;
        strcpy(tdata.cid, data.cid);
        memset(tdatastring, '\0', sizeof(tdatastring));
        sprintf(tdatastring, "%s\t%s\t%s\t%s\t%s\t%d", tdata.id, tdata.timestamp, tdata.type, tdata.aid, tdata.cid, tdata.amount);
        tdatastring[599] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;

        lock.l_len = 600;
        lock.l_pid = getpid();
        lock.l_start = toffset * 600;
        fcntl(fd, F_SETLK, &lock);
        result2 = write(fd, tdatastring, sizeof(tdatastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
	strcpy(response, "0");
        write(conn, response, sizeof(response));
}
void custTransactionHistory(int conn)
{
	//char cid[100];
	//read(conn, &cid, sizeof(cid));
        int fd = open("transaction_records", O_RDWR, 0);
        struct transactionRecord arr[100];
	char currCid[100];
	read(conn, currCid, sizeof(currCid));
	struct transactionRecord data;
        char tdatastring[600];
	int i = 0;
	int result;
	while((result = read(fd, tdatastring, sizeof(tdatastring)))>0)
	{
		sscanf(tdatastring, "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d", data.id, data.timestamp, data.type, data.aid, data.cid, &data.amount);
		if(strcmp(data.cid, currCid) != 0)
			continue;
		
		printf("%s\n", tdatastring);	
		strcpy(arr[i].id, data.id);
		strcpy(arr[i].timestamp, data.timestamp);
		strcpy(arr[i].type, data.type);
		strcpy(arr[i].aid, data.aid);
		strcpy(arr[i].cid, data.cid);
		arr[i].amount = data.amount;
		//printf("%s\t%s\t%s\t%s\t%s\t%d\n", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
		i++;
	}
	close(fd);
	char limit[100];
	sprintf(limit, "%d", i);
	printf("%s\n", limit);	
	write(conn, limit, sizeof(limit));
	write(conn, arr, sizeof(arr));
}
void custChangePassword(int conn)
{
	int fd = open("customer_records", O_RDWR, 0);
        char datastring[700];
        char datastring2[700];
        struct empRecord oldData, newData;

        struct updatePassEmp new;
        read(conn, &new, sizeof(new));
        if(strcmp(new.id, custId) != 0)
        {
                int res = -1;
                write(conn, &res, sizeof(res));
                return;
        }

	int offset;
        sscanf(new.id, "%d", &offset);
        offset = offset - 10000;
        //printf("offset = %d\n", offset);
        //printf("pass = %s\n", new.role);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 700;
        lock.l_len = 700;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 700, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                write(conn, &result, sizeof(result));
	        close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.pass, &oldData.session);

        newData = oldData;
        strcpy(newData.pass, new.pass);

        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[699] = '\n';
        lseek(fd, offset * 700, SEEK_SET);
        char empty[700];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 700, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result ,sizeof(result));
}
void custFeedback(int conn)
{
	char feedback[800];
	read(conn, feedback, sizeof(feedback));
	int fd = open("feedback_statements", O_RDWR, 0);
	int pk;
        char oldfeedback[1000], newfeedback[1000];
	char oldfeedbackstr[800];
	struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -1000;
        lock.l_len = 1000;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -1000, SEEK_END);
        int result = read(fd, oldfeedback, sizeof(oldfeedback));
        printf("Result = %d\n",result);
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(oldfeedback, "%d\t%s", &pk, oldfeedbackstr);
                pk++;
        }
        else
        {
                pk = 50000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        int foffset = pk - 50000;
	lseek(fd, foffset * 1000, SEEK_SET);
        
	memset(newfeedback, '\0', sizeof(newfeedback));
        sprintf(newfeedback, "%d\t%s\t%s", (foffset + 50000), custId, feedback);
        newfeedback[999] = '\n';
        
	lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = foffset * 1000;
        lock.l_len = 1000;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        int result2 = write(fd, newfeedback, sizeof(newfeedback));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        if(result2 == -1)
                result == -1;

        write(conn, &result, sizeof(result));
}
void custApplyLoan(int conn)
{
	int fd = open("loan_apps", O_RDWR, 0);
        char datastring[500];
        struct loanApp oldData, data;
        struct flock lock;
        int pk;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = -500;
        lock.l_len = 500;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, -500, SEEK_END);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        if(result != 0)
        {
                //printf("%s\n", datastring);
                sscanf(datastring, "%d\t%s\t%d\t%s\t%s", &pk, oldData.aid, &oldData.amount, oldData.eid, oldData.status);
                pk++;
        }
        else
        {
                pk = 60000;
        }
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        int offset = pk - 60000;
        lseek(fd, offset * 500, SEEK_SET);
        read(conn, &data, sizeof(data));
        sprintf(data.id, "%d", pk);
	strcpy(data.eid, "-1");
	strcpy(data.status, "Pending");
        memset(datastring, '\0', sizeof(datastring));
        sprintf(datastring, "%s\t%s\t%d\t%s\t%s", data.id, data.aid, data.amount, data.eid, data.status);
        datastring[499] = '\n';
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 500;
        lock.l_len = 500;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        result = write(fd, datastring, sizeof(datastring));
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result, sizeof(result));

}
void managerLogin(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord record;

        struct empLogin data;
        read(conn, &data, sizeof(data));
        int offset;
        sscanf(data.id, "%d", &offset);
        offset = offset - 20000;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        if(result == -1)
        {
                result = 0;
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  record.id, record.name, &record.age, record.email, record.address, record.phone, record.role, record.pass, &record.session);
        if(record.session == 1 || strcmp(data.id, record.id) != 0 || strcmp(data.pass, record.pass) != 0 || strcmp(record.role, "Manager") != 0)
                result = 0;
        else
        {
                strcpy(empId, data.id);
                result = 1;
                record.session = 1;
                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", record.id, record.name, record.age, record.email, record.address, record.phone, record.role, record.pass, record.session);
                printf("Final update string : %s\n", datastring2);
                datastring2[799] = '\n';
                lseek(fd, offset * 800, SEEK_SET);
                char empty[800];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 800, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
       write(conn, &result, sizeof(result));
}
void managerChangePassword(int conn)
{
	int fd = open("employee_records", O_RDWR, 0);
        char datastring[800];
        char datastring2[800];
        struct empRecord oldData, newData;

        struct updatePassEmp new;
        read(conn, &new, sizeof(new));
        if(strcmp(new.id, empId) != 0)
        {
                int res = -1;
                write(conn, &res, sizeof(res));
                return;
        }

        int offset;
        sscanf(new.id, "%d", &offset);
        offset = offset - 20000;
        //printf("offset = %d\n", offset);
        //printf("pass = %s\n", new.role);

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 800;
        lock.l_len = 800;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);

        lseek(fd, offset * 800, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
	if(result == -1)
        {
                write(conn, &result, sizeof(result));
                close(fd);
                return;
        }
        sscanf(datastring, "%[^\t]\t%[^\t]\t%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d",  oldData.id, oldData.name, &oldData.age, oldData.email, oldData.address, oldData.phone, oldData.role, oldData.pass, &oldData.session);

        newData = oldData;
        strcpy(newData.pass, new.pass);

        memset(datastring2, '\0', sizeof(datastring2));
        sprintf(datastring2, "%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%d", newData.id, newData.name, newData.age, newData.email, newData.address, newData.phone, newData.role, newData.pass, newData.session);
        printf("Final update string : %s\n", datastring2);
        datastring2[799] = '\n';
        lseek(fd, offset * 800, SEEK_SET);
        char empty[800];
        memset(empty, '\0', sizeof(empty));
        write(fd, empty, sizeof(empty));
        lseek(fd, offset * 800, SEEK_SET);
        result = write(fd, datastring2, sizeof(datastring2));
        printf("Result = %d\n", result);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);

        write(conn, &result, sizeof(result));

}
void managerReviewFeedback(int conn)
{
	int fd = open("feedback_statements", O_RDWR, 0);
        struct feedbackRecord arr[50];
        struct feedbackRecord data;
	char tdatastring[1000];
        int i = 0;
        int result;
        while((result = read(fd, tdatastring, sizeof(tdatastring)))>0)
        {
                sscanf(tdatastring, "%s\t%s\t%s", data.id, data.cid, data.feedback);

                printf("%s\n", tdatastring);  
                strcpy(arr[i].id, data.id);
                strcpy(arr[i].cid, data.cid);
		strcpy(arr[i].feedback, data.feedback);
                //printf("%s\t%s\t%s\t%s\t%s\t%d\n", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
                i++;
        }
        char limit[100];
        sprintf(limit, "%d", i);
        write(conn, limit, sizeof(limit));
	printf("%s\n", limit);
	printf("%s\n", arr[0].id);
	printf("%s\n", arr[1].cid);
	printf("%s\n", arr[2].feedback);
        write(conn, arr, sizeof(arr));
}
void managerActivateAcc(int conn)
{
	char aid[100];
        read(conn, aid, sizeof(aid));
        int offset;
        sscanf(aid, "%d", &offset);
        offset = offset - 30000;

        int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
        char datastring2[400];
        struct accRecord data;
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 400, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        char response[100];
        if(result != 0)
        {
		sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
		strcpy(data.status, "Active");
                strcpy(response, "0");

                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[399] = '\n';
                lseek(fd, offset * 400, SEEK_SET);
                char empty[400];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 400, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
	        fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                strcpy(response, "-1");;
        }
	write(conn, response, sizeof(response));
}
void managerDeactivateAcc(int conn)
{
	char aid[100];
        read(conn, aid, sizeof(aid));
        int offset;
        sscanf(aid, "%d", &offset);
        offset = offset - 30000;

        int fd = open("account_records", O_RDWR, 0);
        char datastring[400];
        char datastring2[400];
        struct accRecord data;
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 400;
        lock.l_len = 400;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 400, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
        char response[100];
        if(result != 0)
        {
                sscanf(datastring, "%s\t%s\t%d\t%s", data.id, data.cid, &data.balance, data.status);
                strcpy(data.status, "Inactive");
                sprintf(response, "%d", 0);

                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s", data.id, data.cid, data.balance, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[399] = '\n';
                lseek(fd, offset * 400, SEEK_SET);
                char empty[400];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 400, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                sprintf(response, "%d", -1);
        }
        write(conn, &response, sizeof(response));
}
void managerAssignLoanApp(int conn)
{
	int fd = open("loan_apps", O_RDWR, 0);
        char datastring[500], datastring2[500];
	char lid[100];
	char eid[100];
        struct loanApp oldData, data;
        read(conn, &lid, sizeof(lid));
	read(conn, &eid, sizeof(eid));
	
	int offset;
	sscanf(lid, "%d", &offset);
	offset = offset - 60000;

	struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 500;
        lock.l_len = 500;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 500, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);
	
	char response[100];
	if(result != 0)
        {
                sscanf(datastring, "%s\t%s\t%d\t%s\t%s", data.id, data.aid, &data.amount, data.eid, data.status);
                strcpy(data.eid, eid);
		strcpy(data.status, "Assigned");
                sprintf(response, "%d", 0);

                memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s\t%s", data.id, data.aid, data.amount, data.eid, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[499] = '\n';
                lseek(fd, offset * 500, SEEK_SET);
                char empty[500];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 500, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                //sprintf(response, "%d", -1);
		strcpy(response, "-1");
        }
	strcpy(response, "0");
        write(conn, &response, sizeof(response));
}
void empViewAssignedLoanApps(int conn)
{
	int fd = open("loan_apps", O_RDWR, 0);
	char eid[100];
	read(conn, eid, sizeof(eid));
	struct loanApp arr[50];
        struct loanApp data;
        char tdatastring[1000];
        int i = 0;
        int result;
	while((result = read(fd, tdatastring, sizeof(tdatastring)))>0)
        {
                sscanf(tdatastring, "%s\t%s\t%d\t%s\t%s", data.id, data.aid, &data.amount, data.eid, data.status);
		if(strcmp(data.eid, eid) == 0)
		{
                	printf("%s\n", tdatastring);
                	strcpy(arr[i].id, data.id);
                	strcpy(arr[i].aid, data.aid);
			arr[i].amount = data.amount;
                	strcpy(arr[i].eid, data.eid);
			strcpy(arr[i].status, data.status);
                //printf("%s\t%s\t%s\t%s\t%s\t%d\n", arr[i].id, arr[i].timestamp, arr[i].type, arr[i].aid, arr[i].cid, arr[i].amount);
                	i++;
		}
        }
	char limit[100];
        sprintf(limit, "%d", i);
        write(conn, limit, sizeof(limit));
        write(conn, arr, sizeof(arr));
	
}
void empProcessAssignedLoanApp(int conn)
{
	int fd = open("loan_apps", O_RDWR, 0);
        char datastring[500], datastring2[500];
        char lid[100];
        
        struct loanApp oldData, data;
        read(conn, &lid, sizeof(lid));

        int offset;
        sscanf(lid, "%d", &offset);
        offset = offset - 60000;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = offset * 500;
        lock.l_len = 500;
        lock.l_pid = getpid();

        fcntl(fd, F_SETLK, &lock);
        lseek(fd, offset * 500, SEEK_SET);
        int result = read(fd, datastring, sizeof(datastring));
        printf("Result = %d\n",result);

        char response[100];
        if(result != 0)
        {
                sscanf(datastring, "%s\t%s\t%d\t%s\t%s", data.id, data.aid, &data.amount, data.eid, data.status);
                if(strcmp(data.eid, empId) != 0)
		{
			sprintf(response, "%d", -1);
			write(conn, response, sizeof(response));
			return;
		}
		
                
        	int aoffset;
        	sscanf(data.aid, "%d", &aoffset);
        	aoffset = aoffset - 30000;

        	int fd2 = open("account_records", O_RDWR, 0);
        	char adatastring[400];
        	struct accRecord adata;
        	struct flock lock2;

        	lock2.l_type = F_RDLCK;
        	lock2.l_whence = SEEK_SET;
        	lock2.l_start = aoffset * 400;
        	lock2.l_len = 400;
        	lock2.l_pid = getpid();

        	fcntl(fd2, F_SETLK, &lock2);
        	lseek(fd2, aoffset * 400, SEEK_SET);
        	int result2 = read(fd2, adatastring, sizeof(adatastring));
        	printf("Result = %d\n",result);
        	char balance[100];

		if(result2 != 0)
        	{
	                //printf("%s\n", datastring);
                	sscanf(adatastring, "%s\t%s\t%d\t%s", adata.id, adata.cid, &adata.balance, adata.status);
                 	sprintf(balance, "%d", adata.balance);
        	}
       	 	else
        	{
                	sprintf(balance, "%d", -1);
        	}
       	 	lock2.l_type = F_UNLCK;
        	fcntl(fd2, F_SETLK, &lock2);
        	
		int bal;
	  	sscanf(balance, "%d", &bal);
		if(data.amount > 10 * bal)
		{
			strcpy(data.status, "Rejected");
			sprintf(response, "%d", -1);
		}
		else
		{
			strcpy(data.status, "Accepted");
                        sprintf(response, "%d", 0);
		}
		
		memset(datastring2, '\0', sizeof(datastring2));
                sprintf(datastring2, "%s\t%s\t%d\t%s\t%s", data.id, data.aid, data.amount, data.eid, data.status);
                printf("Final update string : %s\n", datastring2);
                datastring2[499] = '\n';
                lseek(fd, offset * 500, SEEK_SET);
                char empty[500];
                memset(empty, '\0', sizeof(empty));
                write(fd, empty, sizeof(empty));
                lseek(fd, offset * 500, SEEK_SET);
                result = write(fd, datastring2, sizeof(datastring2));
                printf("Result = %d\n", result);
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
        }
        else
        {
                sprintf(response, "%d", -1);
        }
        write(conn, &response, sizeof(response));
}
void requestHandler(int conn)
{
	while(1)
	{
		int request_type;
		read(conn, &request_type, sizeof(request_type));
		printf("Request Type of incoming requst = %d\n",request_type);
		switch(request_type)
		{
			case 100: adminLogin(conn);
			  	  break;
			case 101: adminAddEmployee(conn);
				  break;
			case 102: adminModifyCustomer(conn);
				  break;
			case 103: adminModifyEmployee(conn);
				  break;
			case 104: adminManageRoles(conn);
				  break;
			case 105: adminUpdatePassEmp(conn);
				  break;
			case 106: adminUpdatePassCust(conn);
				  break;
			case 107: adminLogout(conn);
				  break;
			case 200: empLogin(conn);
				  break;
			case 201: empAddCustomer(conn);
				  break;
			case 202: empModifyCustomer(conn);
				  break;
			case 203: empViewAssignedLoanApps(conn);
				  break;
			case 204: empProcessAssignedLoanApp(conn);
				  break;
			case 206: empChangePassword(conn);
				  break;
			case 207: empLogout(conn);
				  break;
			case 300: custLogin(conn);
				  break;
			case 301: custAddAccount(conn);
				  break;
			case 302: custViewBalance(conn);
				  break;
			case 303: custDeposit(conn);
				  break;
			case 304: custWithdrawal(conn);
				  break;
			case 305: custTransfer(conn);
				  break;
			case 306: custApplyLoan(conn);
				  break;
			case 307: custChangePassword(conn);
				  break;
			case 308: custFeedback(conn);
				  break;
			case 309: custTransactionHistory(conn);
				  break;
			case 310: custLogout(conn);
				  break;
			case 400: managerLogin(conn);
				  break;
			case 401: managerActivateAcc(conn);
				  break;
			case 402: managerDeactivateAcc(conn);
				  break;
			case 403: managerAssignLoanApp(conn);
				  break;
			case 404: managerReviewFeedback(conn);
				  break;
			case 405: managerChangePassword(conn);
				  break;
			case 500: printf("Exiting request handler\n");
				  exit(0);
				  break;
		}
	}
}
int main()
{
	struct sockaddr_in server, client;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8888);
	bind(sock, (struct sockaddr *)&server, sizeof(server));
	listen(sock, 5);
	printf("Server started and listening for clients : \n");
	while(1)
	{
		socklen_t addrlen = sizeof(client);
		int connect = accept(sock, (struct sockaddr *)&client, &addrlen);
		if(!fork())
		{
			close(sock);
			printf("Forked request handler to service incoming request\n");
			requestHandler(connect);
			exit(0);
		}
		else
		{
			close(connect);
		}
	}
}	

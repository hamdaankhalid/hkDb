#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>

// a structure defining the schema of our database's objects
typedef struct {
	unsigned int key;
	char fname[16];
	char lname[16];
	unsigned int age;
} person_rec;

// opens database file for getting and inserting
int open_record(char *filename){
	int fd;
	 fd = open(filename, O_CREAT | O_RDWR | O_APPEND, 0644);

	if(fd == -1) perror("open_record");

	return fd;
}

// opens tdatatabse file for updating the file (delete)
int open_record_update(char *filename){
	int fd;
	fd = open(filename, O_CREAT | O_RDWR, 0644);

	if(fd == -1) perror("open_record");
	
	return fd;
}

void close_record(int fd){
	close(fd);
}

int insert_record(int fd, person_rec *rec){
	int ret;
	ret = write(fd, rec, sizeof(person_rec));
	return ret;
}

int get_record(int fd, person_rec *rec, int key){
	int ret;
	while( (ret = read(fd, rec, sizeof(person_rec))) != -1 )
	{
		if(ret == 0)
		{
			memset(rec, 0, sizeof(person_rec));
			break;
			return ret;
		}
		else if(key == rec->key)
		{
			return ret;
		}
	}
	
	memset(rec, 0, sizeof(person_rec));
	return ret;
}

int delete_record(int fd, int key){
	int ret;
	person_rec rec;
	off_t pos;

	pos = lseek(fd, 0, SEEK_SET);

	while( (ret = read(fd, &rec, sizeof(person_rec))) != -1 )
	{
		if(ret == 0)
		{
			return ret;
		}
		else if(key == rec.key)
		{
			lseek(fd, pos, SEEK_SET);
			rec.key = 0;
			ret = write(fd, &rec, sizeof(person_rec));
			return ret;
		}
		pos = lseek(fd, 0, SEEK_CUR);
	}
	return ret;
}

int main(int argc, char *argv[]){
	int fd;
	person_rec rec;




	if(argc > 1)
	{
		// insert data
		if(argc > 5 && !strcmp(argv[1],"insert")){
			fd = open_record("data1");
			rec.key = atoi(argv[2]);
			strcpy(rec.fname, argv[3]);
			strcpy(rec.lname, argv[4]);
			rec.age = atoi(argv[5]);
			insert_record(fd, &rec);
		}		
		// delete data
		if(argc > 2 && !strcmp(argv[1], "delete")){
			fd = open_record_update("data1");
			delete_record(fd, atoi(argv[2]));	
		}

		// print data
		if(argc > 2 && !strcmp(argv[1], "get")){
			fd = open_record("data1");
 		        get_record(fd, &rec, atoi(argv[2]));
			printf("KEY = %d\n", rec.key);
			printf("First Name = %s\n", rec.fname);
			printf("Last Name = %s\n", rec.lname);
			printf("Age = %d\n", rec.age);
 	    	}
	}
	
	close_record(fd);
	return 0;
}


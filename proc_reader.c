#include "proc_reader.h"

int list_process_directories(void) {
	DIR *dir = opendir("/proc");
	if (dir == NULL) {
		perror("Failure to open /proc");
		return -1;
	}
	struct dirent*entry;
	int p_count = 0;
    printf("Process directories in /proc:\n");
    printf("%-8s %-20s\n", "PID", "Type");
    printf("%-8s %-20s\n", "---", "----");

    
	while((entry = readdir(dir)) != NULL) {
		if(is_number(entry->d_name)) {
			printf("%-8s %-20s\n", entry->d_name, "process");
			p_count++;
		}
	}
 
	if(closedir(dir) == -1){
		perror("Failure to close /proc");
		return -1;
	}

	printf("Total %d process directories\n", p_count);
    return 0; // Replace with proper error handling
}

int read_process_info(const char* pid) {
    char filepath[256];

    
	snprintf(filepath, sizeof(filepath), "/proc/%s/status", pid);

    printf("\n--- Process Information for PID %s ---\n", pid);


	int status = read_file_with_syscalls(filepath);
	if(status != 0) {
		printf("Failed to  read %s\n", pid);
		return -1;
	}
		
    
	snprintf(filepath, sizeof(filepath), "/proc/%s/cmdline",pid);
    printf("\n--- Command Line ---\n");

    
	int cmd_status = read_file_with_syscalls(filepath);
	if (cmd_status != 0) {
		printf("Failed to read cmd %s\n", pid);
	}
    printf("\n"); // Add extra newline for readability

    return 0; // Replace with proper error handling
}

int show_system_info(void) {
    int line_count = 0;
    const int MAX_LINES = 10;

    printf("\n--- CPU Information (first %d lines) ---\n", MAX_LINES);

   
	FILE *cpu_file = fopen("/proc/cpuinfo", "r");
   
	char line[256];
	if(cpu_file == NULL) {
		printf("Failure to open /proc/cpuinfo\n");
	}else {
		line_count = 0;
		while(fgets(line, sizeof(line), cpu_file) != NULL && line_count < MAX_LINES){
			printf("%s", line);
			line_count++;
		}
		fclose(cpu_file);
	}

    printf("\n--- Memory Information (first %d lines) ---\n", MAX_LINES);

    
	FILE *mem_file = fopen("/proc/meminfo", "r");
	if(mem_file == NULL) {
		printf("Failure to open /proc/meminfo\n");
	}else {
		line_count = 0;
		while(fgets(line, sizeof(line), mem_file) != NULL & line_count < MAX_LINES) {
			printf("%s", line);
			line_count++;
		}
		fclose(mem_file);
	}

    
    

    return 0; // Replace with proper error handling
}

void compare_file_methods(void) {
    const char* test_file = "/proc/version";

    printf("Comparing file reading methods for: %s\n\n", test_file);

    printf("=== Method 1: Using System Calls ===\n");
    read_file_with_syscalls(test_file);

    printf("\n=== Method 2: Using Library Functions ===\n");
    read_file_with_library(test_file);

    printf("\nNOTE: Run this program with strace to see the difference!\n");
    printf("Example: strace -e trace=openat,read,write,close ./lab2\n");
}

int read_file_with_syscalls(const char* filename) {
    
	int filede;
	char buffer[1024];
	ssize_t bytes_read;
    
	filede = open(filename, O_RDONLY);
    
	if(filede == -1) {
		return -1;
	}
    
	while((bytes_read = read(filede, buffer, sizeof(buffer) - 1)) > 0){
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
	}
   
	if(bytes_read == -1) {
		close(filede);
		return -1;
	}
    
	if(close(filede) == -1) {
		return -1;
	}

    return 0; // Replace with proper error handling
}

int read_file_with_library(const char* filename) {
    
	FILE *file;
	char buffer[1024];
    
	file = fopen(filename, "r");
   
	if(file == NULL) {
		printf("Failed to open %s", filename);
		return -1;
	}
    
	while(fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer);
	}

	if(fclose(file) != 0) {
		return -1;
	}
    return 0; // Replace with proper error handling
}

int is_number(const char* str) {
    
	if(str == NULL || *str == '\0') {
		return 0;
	}
    
	int i = 0;
	while(str[i] != '\0') {
		if(!isdigit(str[i])) {
			return 0;
		}
		i++;
	}
   
	return 1;
    return 0; // Replace with actual implementation
}

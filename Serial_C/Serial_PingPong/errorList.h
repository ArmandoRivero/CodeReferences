#define SUCCESS_SERIAL_READ 1
#define SUCCESS_CREATE_SOCKET 2
#define SUCCESS_OPEN_SERIAL_PORT 3
#define SUCCESS_SERIAL_CLOSE 4
#define SUCCESS_SOCKET_CLOSE 5
#define SUCCESS_SOCKET_INIT 6
#define SUCCESS_SOCKET_ACCEPT 7
#define SUCCESS_SOCKET_READ 8
#define ERROR_SERIAL_READ -1
#define ERROR_CREATE_SOCKET -2
#define ERROR_OPEN_SERIAL_PORT -3
#define ERROR_SERIAL_CLOSE -4
#define ERROR_SOCKET_CLOSE -5
#define ERROR_SOCKET_INIT_OPT -6
#define ERROR_SOCKET_INIT_BIND -7
#define ERROR_SOCKET_INIT_LISTEN -8
#define ERROR_SOCKET_ACCEPT -9
#define ERROR_SOCKET_READ -10
#define ERROR_SOCKET_SHUTDOWN -11
#define WARNING_NO_ERROR_FOUND 0


#define NUM_OF_COMMANDS 6


#define COMMAND_NOT_RECOGNIZED -1

#define ACQU_US_EVERY_X_IMAGE 5

#define SAMPLE_ONLY_US 100 //Max (note that the micro sent max 100 samples)
#define BIT_DEPTH 8
#define DEFUALT_NUM_OF_BUF 100
#define CAMERA_TIEMOUT_mS 1000
#define MAX_SHUTTER_SPEED_US 3000
#define MAX_FPS 300
#define MAX_ITERATION_READ 5
#define MAX_IMAGES_PER_SEQUENCE 1000
#define MAX_MICRO_US_IMAGES 500
#define MAX_CONFIG_CAMERA_DIFF 10
#define MAX_ACQU_RETRY 5
enum codeComand{startAcqu = 0 ,WhoIsThere = 1,quit = 2};


void checkError(int result,int enbPrintSuc, char * string1,char * string2);


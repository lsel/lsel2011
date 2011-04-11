int serial_send (int fd, const char* msg, int max_ms);
int serial_recv (int fd, char* buf, int max_bytes, int max_ms);

int serial_open (const char* dev, 
		 const char* speed_str, 
		 const char* mode_str, int hwf, int swf);

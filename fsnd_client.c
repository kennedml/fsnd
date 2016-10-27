#include "fsnd_client.h"
#include <string>

int fsnd_client(char* file, bool is_verbose)
{
	int server_sockfd = 0;
	int kdc_sockfd;
	char *source;

	// Create nonce to send to KDC
	char nonce_str[64] = "";
	char ka[64] = "";

	//long seed = 5647892341;
	//unsigned long int nonce = generate_nonce(seed);
	printf("Please enter nonce for a: ");
	//scanf("%s", nonce_str);
	fgets(nonce_str, 64, stdin);

	printf("Please enter private key for a: ");
	//scanf("%s", ka);
	fgets(ka, 64, stdin);

	/* Remove trailing newline, if there. */
	if ((strlen(ka)>0) && (ka[strlen (ka) - 1] == '\n'))
		ka[strlen (ka) - 1] = '\0';

	// Change these to not be hard-coded
	//const char *kdc_host = "45.55.58.196";
	const char *kdc_host = "thing0.cs.uwec.edu";
	const char *kdc_port = "9286";

	printf("\nDialing kdc host\n");

	kdc_sockfd = socket_dial(kdc_host, kdc_port, is_verbose);

	// send nonce to KDC
	printf("Sending request for key to KDC using nonce: %s\n", nonce_str);
	write(kdc_sockfd, nonce_str, strlen(nonce_str));

	char response[320] = "";
	read(kdc_sockfd, response, 320);
	printf("Encrypted KDC response: %s\n", response);

	Blowfish ctx_a;
	ctx_a.Set_Passwd(ka);
	ctx_a.Decrypt(response,320);
	printf("Decrypted KDC response with client private key: %s\n", response);

	char *session_key = (char*)malloc(64);
	char *request = (char*)malloc(64);
	char *nonce = (char*)malloc(64);
	char *enc_b = (char*)malloc(128);

	memcpy(session_key, response, 64);
	memcpy(request, response + 64, 64);
	memcpy(nonce, response + 128, 64);
	memcpy(enc_b, response+192, 128);

	//char enc_b[128] = "";  
	//sprintf(enc_b, "%-128s", &response[192]); 
	//sprintf(enc_b, "%-128s", response[192]); 
	//memcpy(enc_b, &response[192], 128);
	printf("Session key encrypted with private key B from KDC:  %s\n", enc_b);


	server_sockfd = socket_dial("thing2.cs.uwec.edu", "9285", is_verbose);

	if (server_sockfd < 0)
	{
		printf("\n Error : Could not create socket connection with server \n");
		return 1;
	}

	write(server_sockfd, enc_b, 128);

	char *nonce_b = (char*)malloc(64);
	read(server_sockfd, nonce_b, 64);

	// null terminates session_key at end of key for blowfish password
	for (int i = 0; i < 64; i++){
		if(session_key[i] == ' '){
			session_key[i] = '\0';
			break;
		}
	}
	printf("Session Key: %s\n", session_key);

	Blowfish ctx_session;
	ctx_session.Set_Passwd(session_key);
	ctx_session.Decrypt(nonce_b, 64);
	printf("Nonce from b decrypted by session key: %s\n", nonce_b);

	//TODO: send function nonce to server, not working properly
	// null terminates nonce at end of key for function
	/* for (int i = 0; i < 64; i++){ */
	/*   if(nonce_b[i] == ' '){ */
	/*     nonce_b[i] = '\0'; */
	/*     break; */
	/*   } */
	/* } */

	/* unsigned long int f_bnonce = generate_nonce(atol(nonce_b)); */
	/* printf("function nonce: %lu\n", f_bnonce); */
	/* sprintf(nonce_b, "%-64lu", f_bnonce); */ 

	/* ctx_session.Encrypt(nonce_b, 64); */

	/* write(server_sockfd, nonce_b, 64); */

	char str[1024];
	char temp[1024];
	std::string hex_str;
	printf("Enter sample string: ");
	fgets(str, 1024, stdin);
	//scanf("%s", str);

	/* Remove trailing newline, if there. */
	if ((strlen(str)>0) && (str[strlen (str)] == '\n'))
		str[strlen (str)] = '\0';

	printf("Example string representation(ASCII): %s\n", str);

	int i, j;

	for(i = 0; i < (int)strlen(str); i++, j+=2){
		sprintf((char*)temp+j, "%02X", str[i]);
	}

	temp[j] = '\0';
	printf("Example string representation(Hexadecimal): %s\n", temp);    
	hex_str = temp;

	printf("len: %lu\n", hex_str.length());

	while(hex_str.length() % 8){
		hex_str += ' '; 
	}

	ctx_session.Encrypt((void*)hex_str.c_str(), hex_str.length());
	printf("Encrypted Hex string: %s\n", hex_str.c_str());

	printf("Writing encrypted hex string to socket\n");
	write(server_sockfd, hex_str.c_str(), (int)hex_str.length());

	FILE *fp;
	fp = fopen(file, "rb");
	if(!fp)
	{
		printf("Error opening file");
		return 1;
	}

	struct stat sb;
	int file_size = 0;

	int sum = 0;
	if (fp != NULL) {
		/* Go to the end of the file. */
		if (fseek(fp, offset, SEEK_END) == 0) {
			/* Get the size of the file. */
			// Get and Send File Size
			fstat(fileno(fp), &sb);

			/* if(verbose){ printf("File size: %ld\n", bufsize);} */

			if (bytes > 0){
				file_size = bytes;
				printf("CLIENT BYTES: %d FILESIZE: %d\n", bytes, file_size);
			}
			else{
				file_size = (int)sb.st_size;
			}

			/* Allocate our buffer to that size. */
			source = (char*)calloc(file_size, sizeof(char));

			/* Go back to the start of the file. */
			if (fseek(fp, offset, SEEK_SET) != 0) { /* Error */ }

			/* Read the entire file into memory. */
			int newLen;
			if((newLen = fread(source, sizeof(char), file_size,fp)) > 0)
			{
				int total = 0;
				do{
					int written = write(server_sockfd, source, newLen);
					if (written == -1){
						printf("Failed to write: %s\n", strerror(errno));
						return 1;
					}
					total += written;
					sum += total;
				}while(total < newLen);
			}

		}
	}
	free(session_key);
	free(nonce);
	free(request);
	free(enc_b);
	free(nonce_b);
	free(source);
	fclose(fp);
	close(server_sockfd);
	close(kdc_sockfd);
	if(verbose){printf("Sent %d Bytes Successfully!\n", sum);} 
	return 0;
}

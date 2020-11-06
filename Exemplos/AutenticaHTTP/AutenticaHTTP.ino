https://www.youtube.com/watch?v=u_QbLrZSBuU

bool validar_usuario (cha* linebuf) {
	
	char usuario_senha [] = "admin:admin";
	int t = strlen (usuario_senha);

	int tamanhoEnc = (((t - 1) / 3) *4);
	char out [tamanhoEnc];

	base64_encode (out, usuario_senha, t + 1);

	int desconta = 0;
	if ((t%3)) == 1 { desconta = 2; }
	if ((t%3)) == 2 { desconta = 1; }

	byte i;
	for (i = 0; i < (TamanhoEnc - desconta); i++) {
		out2[i] = out[i];
	}
	out2[i] = '\0';

	return( strstr (linebuf, out2) > 0);


}

void html_autenticado (EthernetClient & client ) {

	client.println (F("HTTP/1.1 200 ON \n"
					  "Content-type: text/html\n"
					  "Connection? keep-alive\n\n"
					  "<DOCTYPE HTML>"
					  "<html>"));

	client.println (F("<body>"));
	client.println (F("AQUI vai os dados da pagina"));
	client.println (F("</body>"));

	client.println (F("/html"));
}

void a3_to_a4 (unsigned char * a4, unsigned char * a3);
void a4_to_a3 (unsigned char * a3, unsigned char * a4);
unsigned char b64_lookup (char c);


int base64_encode (char* output,char *input, int inoutLen) {
	unsigned char b64_alphabet [] = "ABCDEFGHIJKLMNOPQRESTVWXTZabcdefgihjklmnopqrestvwxtz+/";
	int i = 0, j = 0, encLen = 0;
	unsigned char a3[3], a[4];

	while (inputLen--) {
		a3[i++] =* (input++);
		if (i == 3) {
			a3_to_a4 (a4, a3);
			for (i = 0; i < 4; i++)
				output[encLen++] = b64_alphabet[a4[j]];
			while ((i++ < 3))
				output[encLen++] = '=';
		}
		output[encLen] = '\n';
		return (encLen);
	}




#include <brwMqtt.h>


brwMqtt::brwMqtt () {

    setMqttUsar (false);
    setMqttHost ("brware.com.br");
    setMqttPort (1883);
    setMqttUser ("brware");
    setMqttPassWord ("SQRT(pi)!=314");
    




}




//------------------------------------------------------------------------------------------
//
//------------------  Funcoes de configuração (SET)
//
//------------------------------------------------------------------------------------------

void brwMqtt::setMqttID (String ID) {

    mqttID = ID; // iD da coneneão ( MAC da placa)

}




void brwMqtt::setMqttUsar (bool usar) {


	usarMQTT = usar;
}


void brwMqtt::setMqttHost (String host) {

	mqttHost = host;
}


void brwMqtt::setMqttPort (int port) {

	mqttPort = port;
}


void brwMqtt::setMqttUser (String user) {

	mqttUser = user;

}


void brwMqtt::setMqttPassWord (String pwd) {

	mqttPassWord = pwd;

}

void brwMqtt::setTopicoPub (String topico) {

	topicoPub = topico;
	

}


void brwMqtt::setTopicoSub (String topico) {

	topicoSub = topico;
	

}

void brwMqtt::setTopicoConnect (String topico) {

	topicoConnect  = topico;
	

}


void brwMqtt::setTopicoWill (String topico) {

	topicoWill  = topico;
	


}


void brwMqtt::setTopicoConfigPub (String topico) {

	topicoConfigPub  = topico;
	


}


void brwMqtt::setTopicoConfigSub (String topico) {

	topicoConfigSub  = topico;
	


}

//------------------------------------------------------------------------------------------
//
//------------------  Funcoes de configuração (GET
//
//------------------------------------------------------------------------------------------


String brwMqtt::getMqttID () {

    return (mqttID); // iD da coneneão ( MAC da placa)
    
}


bool brwMqtt::getMqttUsar () {

	return (usarMQTT);

}


String brwMqtt::getMqttHost () {

	return (mqttHost);
}


int brwMqtt::getMqttPort () {

	return (mqttPort);
}


String brwMqtt::getMqttUser () {

	return (mqttUser);

}


String brwMqtt::getMqttPassWord () {

	return (mqttPassWord);

}

String brwMqtt::getTopicoPub () {

	return (topicoPub);

}


String brwMqtt::getTopicoSub () {

	return (topicoSub);

}

String brwMqtt::getTopicoConnect () {

	return (topicoConnect);

}


String brwMqtt::getTopicoWill () {

	return (topicoWill);

}


String brwMqtt::getTopicoConfigPub () {

	return (topicoConfigPub);

}


String brwMqtt::getTopicoConfigSub () {

	return (topicoConfigSub);

}


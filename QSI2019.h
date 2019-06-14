/*
 * Autor: Mariano Fernández.
 * Versión: 0.2
 */

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>

#ifndef __QSI2019__

#define __QSI2019__

namespace QSI
{
    // Clases y estructuras.
    typedef struct login_str
    {
        String id = "";
        String password = "";
        login_str(char * ident, char * pass)
        {
            id = ident;
            password = pass;
        };
        login_str() {};
    } Login;

    // Funciones
    void conectarOrdenador();
    void conectarOrdenador(unsigned long baudios); // Conecta el monitor serie.
    void conectarWiFi(Login credenciales); // Nos conectamos al WiFi.
    bool estaConectadoWiFi(); // Comprobamos si el WiFi está conectado.
    void reconectarWiFi(); // Reconecta el WiFi con la última configuración.

    void configurarMQTT(char *IP, char * name, void call(char *topic, byte *payload, unsigned int length));
    void configurarMQTT(char *IP, char * name); // Configuramos la conexión MQTT.
    void conectarMQTT();
    void conectarMQTT(Login credenciales); // Nos conectamos al servidor MQTT.
    bool estaConectadoMQTT(); // Comprobamos si tenemos conexion con el servidor MQTT.
    void reconectarMQTT(); // Reconecta con el servidor MQTT con la ultima configuración.

    void suscribirMQTT(char* topic); // Suscripción a un topic.
    void publiMQTT(char* topic, char* msg); // Publica un mensaje a un topic.
    void revisarMQTT(); // Lanza el loop del servicio de MQTT.

    char * conversionATexto(byte * array);
};

#endif
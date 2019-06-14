#include <QSI2019.h>


namespace QSI
{
    namespace Constantes {
        const uint16_t TOPICS_MAXLEN = 128;
        const uint16_t PORT_MQTT = 1883;
    };

    // Variables
    namespace Variables
    {
        unsigned long bauds = 9600;
        char IP_Servidor_MQTT[15] = "0.0.0.0";
        char nombre_cliente_mqtt[128] = "";
        char **lista_topics;
        uint8_t num_topics = 0;
    };

    // Datos
    namespace Datos
    {
        WiFiClient esp8266w;
        PubSubClient * client;
        Login Credenciales_WiFi;
        Login Credenciales_MQTT;
    };
};

void QSI::conectarOrdenador()
{
    using namespace Variables;
    Serial.begin(bauds);
    Serial.println("Successfully established connection to the computer.");
};

void QSI::conectarOrdenador(unsigned long baudios)
{
    using namespace Variables;
    bauds = baudios;
    Serial.begin(bauds);
    Serial.println("Successfully established connection to the computer.");
};

void QSI::conectarWiFi(QSI::Login credenciales)
{
    using namespace Datos;
    Credenciales_WiFi = credenciales;
    Serial.print("Connecting to ");
    Serial.print(Credenciales_WiFi.id);
    Serial.println(".");
    WiFi.begin(Credenciales_WiFi.id, Credenciales_WiFi.password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println(".");
};

bool QSI::estaConectadoWiFi()
{
    return WiFi.status() == WL_CONNECTED;
};

void QSI::reconectarWiFi()
{
    using namespace Datos;
    Serial.print("Reconnecting to ");
    Serial.print(Credenciales_WiFi.id);
    Serial.println(".");
    WiFi.begin(Credenciales_WiFi.id, Credenciales_WiFi.password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println(".");
};

void QSI::configurarMQTT(char *IP, char *name, void call(char *topic, byte *payload, unsigned int length))
{
    using namespace Variables;
    using namespace Datos;
    using namespace Constantes;

    strcpy(IP_Servidor_MQTT, IP);
    strcpy(nombre_cliente_mqtt, name);

    client = new PubSubClient(esp8266w);
    client->setServer(IP_Servidor_MQTT, PORT_MQTT);
    client->setCallback(call);
};

void QSI::configurarMQTT(char *IP, char *name)
{
    using namespace Variables;
    using namespace Datos;
    using namespace Constantes;

    strcpy(IP_Servidor_MQTT, IP);
    strcpy(nombre_cliente_mqtt, name);

    client = new PubSubClient(esp8266w);
    client->setServer(IP_Servidor_MQTT, PORT_MQTT);
};

void QSI::conectarMQTT()
{
    using namespace Variables;
    using namespace Datos;

    while (!client->connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client->connect(nombre_cliente_mqtt))
        {
            Serial.println(" connected.");
        }
        else
        {
            Serial.print("Failed, rc = ");
            Serial.print(client->state());
            Serial.println(". It will try again in 5 seconds.");
            delay(5000);
        };
    };
};

void QSI::conectarMQTT(Login credenciales)
{
    using namespace Variables;
    using namespace Datos;

    Credenciales_MQTT = credenciales;

    while (!client->connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client->connect(nombre_cliente_mqtt, Credenciales_MQTT.id.c_str(), Credenciales_MQTT.password.c_str()))
        {
            Serial.println(" connected.");
        }
        else
        {
            Serial.print("Failed, rc = ");
            Serial.print(client->state());
            Serial.println(". It will try again in 5 seconds.");
            delay(5000);
        };
    };
};

bool QSI::estaConectadoMQTT()
{
    return Datos::client->connected();
};

void QSI::reconectarMQTT()
{
    using namespace Variables;
    using namespace Datos;

    while (!client->connected())
    {
        Serial.print("Attempting MQTT reconnection...");
        if (client->connect(nombre_cliente_mqtt, Credenciales_MQTT.id.c_str(), Credenciales_MQTT.password.c_str()))
        {
            Serial.println(" reconnected.");
        }
        else
        {
            Serial.print("Failed, rc = ");
            Serial.print(client->state());
            Serial.println(". It will try again in 5 seconds.");
            delay(5000);
        };
    };

    Serial.println("");

    for (auto i = 0; i < num_topics; ++i)
    {
        client->subscribe(lista_topics[i]);
        Serial.print("Subscribed to: ");
        Serial.print(lista_topics[i]);
        Serial.println(".");
    };
};

void QSI::suscribirMQTT(char *topic)
{
    using namespace Datos;
    using namespace Variables;
    using namespace Constantes;

    char **old_list = lista_topics;

    ++num_topics;
    lista_topics = new char *[num_topics];

    for (auto i = 0; i < num_topics; ++i)
    {
        lista_topics[i] = new char[TOPICS_MAXLEN];
        if (i == (num_topics - 1))
        {
            strcpy(lista_topics[i], topic);
        }
        else
        {
            strcpy(lista_topics[i], old_list[i]);
        };
    };

    delete[] old_list;

    client->subscribe(topic);
    Serial.print("Subscribed to: ");
    Serial.print(topic);
    Serial.println(".");
};

void QSI::publiMQTT(char *topic, char *msg)
{
    using namespace Datos;

    client->publish(topic, msg);
    Serial.println("");
    Serial.print("Published: \"");
    Serial.print(msg);
    Serial.println("\".");
    Serial.print("On: \"");
    Serial.print(topic);
    Serial.println("\".");
};

void QSI::revisarMQTT()
{
    using namespace Datos;
    client->loop();
};


char * QSI::conversionATexto(byte * cadenaBytes)
{
    return reinterpret_cast<char *> (cadenaBytes);
};
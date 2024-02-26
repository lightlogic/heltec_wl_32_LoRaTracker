#include <TinyGPSPlus.h>
#include "LoRaWan_APP.h"
#include "secrets.h"
#include <string>
#include <cstring>
#include <array>
#include <SoftwareSerial.h>

/* OTAA para*/
uint8_t devEui[] = {DEV_EUI};
uint8_t appEui[] = {APP_EUI};
uint8_t appKey[] = {APP_KEY};

/* ABP para*/
uint8_t nwkSKey[] = {0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC};
uint8_t appSKey[] = {0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC, 0xDC};
uint32_t devAddr = (uint32_t)0x007e6aeb;

/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = {0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = CLASS_A;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 180000;

/*OTAA or ABP*/
bool overTheAirActivation = true;

/*ADR enable*/
bool loraWanAdr = true;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;

/* Application port */
uint8_t appPort = 2;
/*!
 * Number of trials to transmit the frame, if the LoRaMAC layer did not
 * receive an acknowledgment. The MAC performs a datarate adaptation,
 * according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
 * to the following table:
 *
 * Transmission nb | Data Rate
 * ----------------|-----------
 * 1 (first)       | DR
 * 2               | DR
 * 3               | max(DR-1,0)
 * 4               | max(DR-1,0)
 * 5               | max(DR-2,0)
 * 6               | max(DR-2,0)
 * 7               | max(DR-3,0)
 * 8               | max(DR-3,0)
 *
 * Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
 * the datarate, in case the LoRaMAC layer did not receive an acknowledgment
 */
uint8_t confirmedNbTrials = 4;

// // A sample NMEA stream.
// const char *gpsStream =
// 	"$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
// 	"$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
// 	"$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
// 	"$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
// 	"$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
// 	"$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

static const int8_t RXPin = 38, TXPin = 39;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial gpsSerial(RXPin, TXPin);

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
	unsigned long start = millis();
	do
	{
		while (gpsSerial.available())
			gps.encode(gpsSerial.read());
	} while (millis() - start < ms);
}

// void encodeWGs84inHexArray(int lng, int lat, std::array<unsigned char, 8> hexArray){
void encodeWGs84inHexArray(const std::int32_t intlng, const std::int32_t intlat, std::array<unsigned char, 8U> &msgPayload)
{
	// Copy the first integer into the first 4 bytes of the array
	std::memcpy(msgPayload.data(), &intlng, sizeof(intlng));

	// Copy the second integer into the next 4 bytes of the array
	std::memcpy(msgPayload.data() + 4, &intlat, sizeof(intlat));
}

void displayGPSInfo()
{
	Serial.print(F("Location: "));
	if (gps.location.isValid())
	{
		Serial.print(gps.location.lat(), 6);
		Serial.print(F(","));
		Serial.print(gps.location.lng(), 6);
	}
	else
	{
		Serial.print(F("INVALID"));
	}

	Serial.print(F("  Date/Time: "));
	if (gps.date.isValid())
	{
		Serial.print(gps.date.month());
		Serial.print(F("/"));
		Serial.print(gps.date.day());
		Serial.print(F("/"));
		Serial.print(gps.date.year());
	}
	else
	{
		Serial.print(F("INVALID"));
	}

	Serial.print(F(" "));
	if (gps.time.isValid())
	{
		if (gps.time.hour() < 10)
			Serial.print(F("0"));
		Serial.print(gps.time.hour());
		Serial.print(F(":"));
		if (gps.time.minute() < 10)
			Serial.print(F("0"));
		Serial.print(gps.time.minute());
		Serial.print(F(":"));
		if (gps.time.second() < 10)
			Serial.print(F("0"));
		Serial.print(gps.time.second());
		Serial.print(F("."));
		if (gps.time.centisecond() < 10)
			Serial.print(F("0"));
		Serial.print(gps.time.centisecond());
	}
	else
	{
		Serial.print(F("INVALID"));
	}

	Serial.println();
}

/* Prepares the payload of the frame */
static void prepareTxFrame(uint8_t port)
{
	/*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
	 *appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
	 *if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
	 *if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
	 *for example, if use REGION_CN470,
	 *the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
	 */
	// appDataSize = 8;
	// appData[0] = 0x00;
	// appData[1] = 0x01;
	// appData[2] = 0x02;
	// appData[3] = 0x03;
	// appData[5] = 0x00;
	// appData[6] = 0x01;
	// appData[7] = 0x02;
	// appData[8] = 0x03;

	// const char *messageType = "0";
	// std::string messPayload;

	auto msgPayload = std::array<unsigned char, 8U>{};

	if (gps.location.isValid())
	{
		std::int32_t ilng = gps.location.lng() * 100000;
		// Serial.print(ilng);
		std::int32_t ilat = gps.location.lat() * 100000;
		// Serial.print(ilat);
		encodeWGs84inHexArray(ilng, ilat, msgPayload);

		// Serial.print(gps.location.lat(), 6);
		// Serial.print(F(","));
		// Serial.print(gps.location.lng(), 6);
		// messPayload += std::to_string(gps.location.lng()*100000) + " " + std::to_string(gps.location.lat()*100000);
	}
	// else
	// {
	// 	// appData was initialized as {0, 0, 0, 0, 0, 0, 0, 0} and is send a is.
	// };

	appDataSize = sizeof(msgPayload);

	for (int i = 0; i < sizeof(appData); i++)
	{
		appData[i] = msgPayload[i];
	}
}

RTC_DATA_ATTR bool firstrun = true;

void setup()
{
	Serial.begin(115200);
	gpsSerial.begin(GPSBaud);

	// while (!Serial && !gpsSerial)
	// {
	// 	delay(10);
	// }

	Serial.println(F("Serial setup done."));

	Mcu.begin();

	if (firstrun)
	{
		LoRaWAN.displayMcuInit();
		firstrun = false;
	}
	deviceState = DEVICE_STATE_INIT;
}

void loop()
{
	switch (deviceState)
	{
	case DEVICE_STATE_INIT:
	{
#if (LORAWAN_DEVEUI_AUTO)
		LoRaWAN.generateDeveuiByChipID();
#endif
		LoRaWAN.init(loraWanClass, loraWanRegion);
		break;
	}
	case DEVICE_STATE_JOIN:
	{
		LoRaWAN.displayJoining();
		LoRaWAN.join();
		break;
	}
	case DEVICE_STATE_SEND:
	{
		smartDelay(1000);
		LoRaWAN.displaySending();
		prepareTxFrame(appPort);
		LoRaWAN.send();
		deviceState = DEVICE_STATE_CYCLE;
		break;
	}
	case DEVICE_STATE_CYCLE:
	{
		// Schedule next packet transmission
		txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);
		LoRaWAN.cycle(txDutyCycleTime);
		deviceState = DEVICE_STATE_SLEEP;
		break;
	}
	case DEVICE_STATE_SLEEP:
	{
		LoRaWAN.displayAck();
		LoRaWAN.sleep(loraWanClass);
		break;
	}
	default:
	{
		deviceState = DEVICE_STATE_INIT;
		break;
	}
	}
}

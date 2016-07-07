#include "mraa.h"

#include <stdio.h>
#include <unistd.h>

#define SWEST_HELLO

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
// we have to use BCM numbering when initializing with wiringPiSetupSys
// when choosing a different pin number please use the BCM numbering, also
// update the Property Pages - Build Events - Remote Post-Build Event command 
// which uses gpio export for setup for wiringPiSetupSys

#ifdef SWEST_HELLO
struct morseChar_s {
	char character;
	int len;
	char* morse;
};

typedef struct morseChar_s MorseChar;

static MorseChar mc_e = { 'E', 1, "S" };
static MorseChar mc_h = { 'H',4,"SSSS" };
static MorseChar mc_l = { 'L',4,"SLSS" };
static MorseChar mc_o = { 'O',3,"LLL" };
static MorseChar mc_s = { 'S',3,"SSS" };
static MorseChar mc_t = { 'T',1,"L" };
static MorseChar mc_w = { 'W',3,"SLL" };
static MorseChar mc_0 = { '0',5,"LLLLL" };
static MorseChar mc_1 = { '1',5,"SLLLL" };
static MorseChar mc_2 = { '2',5,"SSLLL" };
static MorseChar mc_6 = { '6',5,"LSSSS" };
int slen = 300000;
int plen = slen;
int llen = 3 * slen;
int clen = 3 * slen;
int wlen = 7 * slen;

struct morseWord_s
{
	int NumOfChars;
	MorseChar** characters;
};
typedef struct morseWord_s MorseWord;

struct  morseSentense_s
{
	int NumOfWords;
	MorseWord** words;
};
typedef struct morseSentense_s MorseSentense;
static MorseWord etwest2016;
static MorseChar *etwestChar[10];
static MorseWord hello;
static MorseChar *helloChar[5];
static MorseSentense sentense;
static MorseWord* words[2];

MorseSentense* InitMorse()
{

	etwest2016.characters = &etwestChar[0];
	etwest2016.NumOfChars = 10;
	int index = 0;
	etwest2016.characters[index++] = &mc_e;
	etwest2016.characters[index++] = &mc_t;
	etwest2016.characters[index++] = &mc_w;
	etwest2016.characters[index++] = &mc_e;
	etwest2016.characters[index++] = &mc_s;
	etwest2016.characters[index++] = &mc_t;
	etwest2016.characters[index++] = &mc_2;
	etwest2016.characters[index++] = &mc_0;
	etwest2016.characters[index++] = &mc_1;
	etwest2016.characters[index++] = &mc_6;

	hello.NumOfChars = 5;
	hello.characters = &helloChar[0];
	index = 0;
	hello.characters[index++] = &mc_h;
	hello.characters[index++] = &mc_e;
	hello.characters[index++] = &mc_l;
	hello.characters[index++] = &mc_l;
	hello.characters[index++] = &mc_o;

	sentense.NumOfWords = 2;
	sentense.words = &words[0];
	sentense.words[0] = &hello;
	sentense.words[1] = &etwest2016;

	return &sentense;
}
#endif

int main(void)
{
	mraa_platform_t platform = mraa_get_platform_type();
	mraa_gpio_context d_pin = NULL;
	switch (platform) {
	case MRAA_INTEL_GALILEO_GEN1:
		d_pin = mraa_gpio_init_raw(3);
		break;
	case MRAA_INTEL_GALILEO_GEN2:
		d_pin = mraa_gpio_init(13);
		break;
	case MRAA_INTEL_EDISON_FAB_C:
		d_pin = mraa_gpio_init(13);
		break;
	default:
		fprintf(stderr, "Unsupported platform, exiting");
		return MRAA_ERROR_INVALID_PLATFORM;
	}
	if (d_pin == NULL) {
		fprintf(stderr, "MRAA couldn't initialize GPIO, exiting");
		return MRAA_ERROR_UNSPECIFIED;
	}

	// set the pin as output
	if (mraa_gpio_dir(d_pin, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
		fprintf(stderr, "Can't set digital pin as output, exiting");
		return MRAA_ERROR_UNSPECIFIED;
	};
#ifdef SWEST_HELLO
	MorseSentense*sentense = InitMorse();
#endif

	for (int i = 0; i<5; i++)
	{
#ifdef SWEST_HELLO
		for (int w = 0; w < sentense->NumOfWords; w++) {
			MorseWord* word = sentense->words[w];
			for (int c = 0; c < word->NumOfChars; c++) {
				MorseChar* character = word->characters[c];
				for (int m = 0; m < character->len; m++) {
					printf("%c\r\n", character->morse[m]);
					mraa_gpio_write(d_pin, 1);
					switch (character->morse[m])
					{
					case 'S':
						usleep(slen);
						break;
					case 'L':
						usleep(llen);
						break;
					default:
						break;
					}
					mraa_gpio_write(d_pin, 0);
					usleep(plen);
				}
				usleep(clen - plen);
			}
			usleep(wlen - clen);
		}
#else
		// loop forever toggling the on board LED every second
		mraa_gpio_write(d_pin, 0);
		sleep(1);
		mraa_gpio_write(d_pin, 1);
		sleep(1);
#endif
	}

	return MRAA_SUCCESS;
}

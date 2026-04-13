// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Qiangli_hub75_test.ino
    Created:	12/04/2026 02:04:18
    Author:     DESKTOP-BI0KSD8\User
*/
#include "ESP32-VirtualMatrixPanel-I2S-DMA.h"

// Define custom class derived from VirtualMatrixPanel
class CustomPxBasePanel : public VirtualMatrixPanel
{
public:
    using VirtualMatrixPanel::VirtualMatrixPanel; // inherit VirtualMatrixPanel's constructor(s)

protected:

    VirtualCoords getCoords(int16_t x, int16_t y);  // custom getCoords() method for specific pixel mapping
};

// custom getCoords() method for specific pixel mapping
inline VirtualCoords CustomPxBasePanel::getCoords(int16_t x, int16_t y) {

    coords = VirtualMatrixPanel::getCoords(x, y); // first call base class method to update coords for chaining approach

    if (coords.x == -1 || coords.y == -1) { // Co-ordinates go from 0 to X-1 remember! width() and height() are out of range!
        return coords;
    }

    uint8_t pxbase = panelResX;   // pixel base
    if (((coords.y / 6) % 2) == 0)
    {
        coords.x += ((coords.x / pxbase) + 1) * pxbase;
    }
    else
    {
        coords.x += (coords.x / pxbase) * pxbase;
    }

    coords.y = (coords.y / 12) * 6 + (coords.y % 6);
    return coords;
}

// Panel configuration
#define PANEL_RES_X 48 // Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 24 // Number of pixels tall of each INDIVIDUAL panel module.

// Use a single panel for tests
#define NUM_ROWS 1 // Number of rows of chained INDIVIDUAL PANELS
#define NUM_COLS 1 // Number of INDIVIDUAL PANELS per ROW

// Chain settings, do not cnahge
#define SERPENT true
#define TOPDOWN false
#define VIRTUAL_MATRIX_CHAIN_TYPE CHAIN_BOTTOM_RIGHT_UP

// placeholder for the matrix object
MatrixPanel_I2S_DMA* dma_display = nullptr;

// placeholder for the virtual display object
CustomPxBasePanel* FourScanPanel = nullptr;

/******************************************************************************
Setup!
******************************************************************************/
void setup()
{
	Serial.begin(115200);
	delay(200);
	while (!Serial) { ; } // wait for serial port to connect. Needed for native USB
	// Print file location and date of compilation for debugging purposes
	Serial.println("File: " __FILE__);
   	Serial.println("Date: " __DATE__ " " __TIME__);
	Serial.println("Init HUB75 48x24 with custom pixel mapping...");

    HUB75_I2S_CFG mxconfig(
        PANEL_RES_X * 2, // DO NOT CHANGE THIS
        PANEL_RES_Y / 2, // DO NOT CHANGE THIS
        NUM_ROWS * NUM_COLS // DO NOT CHANGE THIS
        //,_pins // Uncomment to enable custom pins
    );
    mxconfig.line_decoder = HUB75_I2S_CFG::SM5368;
    mxconfig.clkphase = false; // Change this if you see pixels showing up shifted wrongly by one column the left or right.

    // OK, now we can create our matrix object
    dma_display = new MatrixPanel_I2S_DMA(mxconfig);

    // let's adjust default brightness to about 75%
    dma_display->setBrightness8(40); // range is 0-255, 0 - 0%, 255 - 100%

    // Allocate memory and start DMA display
    if (not dma_display->begin())
        Serial.println("****** !KABOOM! I2S memory allocation failed ***********");

    dma_display->clearScreen();
    delay(100);

	Serial.println("DMA display initialized successfully!");    

    // create FourScanPanellay object based on our newly created dma_display object
    FourScanPanel = new CustomPxBasePanel((*dma_display), NUM_ROWS, NUM_COLS, PANEL_RES_X, PANEL_RES_Y, VIRTUAL_MATRIX_CHAIN_TYPE);
	FourScanPanel->setTextSize(1);
}

void loop() {
	// Print a Hello World message to the panel in 2 lines
    FourScanPanel->setCursor(0, 0);
    FourScanPanel->setTextColor(FourScanPanel->color565(0, 255, 0));
    FourScanPanel->print("HELLO");
    FourScanPanel->setCursor(0, 8);
    FourScanPanel->setTextColor(FourScanPanel->color565(128, 200, 20));
    FourScanPanel->print("WORLD");


    delay(2000);
    FourScanPanel->clearScreen();

    FourScanPanel->setCursor(0, 0);
    FourScanPanel->setTextColor(FourScanPanel->color565(255, 255, 0));
    FourScanPanel->print("ROUSIS");
    FourScanPanel->setCursor(0, 8);
    FourScanPanel->setTextColor(FourScanPanel->color565(0, 0, 255));
    FourScanPanel->print("SYSTEMS");
    FourScanPanel->setCursor(0, 16);
	FourScanPanel->setTextColor(FourScanPanel->color565(255, 0, 0));
	FourScanPanel->print("TEST");

    delay(2000);
    FourScanPanel->clearScreen();

    /*
    for (int i = 0; i < FourScanPanel->height(); i++)
    {
        for (int j = 0; j < FourScanPanel->width(); j++)
        {
            FourScanPanel->drawPixel(j, i, FourScanPanel->color565(255, 0, 0));
            delay(30);
        }
    }
    delay(2000);
    dma_display->clearScreen();
    */
} // end loop
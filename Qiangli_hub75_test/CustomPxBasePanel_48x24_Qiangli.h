// Define custom class derived from VirtualMatrixPanel
// For 48x24 panel 1:6 48x24, scan 1/6 with the CHIPSET : DP5135 +DP32020
// from Maxairidis order code: QL5-P6-RGB ME SKU 3-31-51

// Important!!!!!!!!!!!!!!
// must put this in the setup:
// mxconfig.clkphase = false; 
// mxconfig.line_decoder = HUB75_I2S_CFG::SM5368;
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

// Chain settings, do not cnahge
#define SERPENT true
#define TOPDOWN false
#define VIRTUAL_MATRIX_CHAIN_TYPE CHAIN_BOTTOM_RIGHT_UP

// placeholder for the matrix object
MatrixPanel_I2S_DMA* dma_display = nullptr;

// placeholder for the virtual display object
CustomPxBasePanel* FourScanPanel = nullptr;


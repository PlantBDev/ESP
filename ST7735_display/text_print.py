#A python script that displays text on the TFT-lcd

def print():
    import digitalio
    import board
    from PIL import Image, ImageDraw, ImageFont
    import adafruit_rgb_display.st7735 as st7735
    BORDER = 20
    FONTSIZE = 24

    #pins for SPI communication and reset
    cs_pin = digitalio.DigitalInOut(board.CE0)
    dc_pin = digitalio.DigitalInOut(board.D25)
    reset_pin = digitalio.DigitalInOut(board.D24)
    BAUDRATE = 24000000

    spi = board.SPI()

    disp = st7735.ST7735R(spi, rotation=90,
        cs=cs_pin,
        dc=dc_pin,
        rst=reset_pin,
        baudrate=BAUDRATE,
    )


    height = disp.width
    width = disp.height

    image = Image.new("RGB", (width, height))
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", FONTSIZE)

    text = "abcdefghijkl"
    #line 1
    (font_width, font_height) = font.getsize(text)
    draw.text((0,0),text,font=font,fill=(255,255,0),
    )
    #line 2
    draw.text((0, font_height),text,font=font,fill=(255,255,0),
    )
    #line 3
    draw.text((0,font_height*2),text,font=font,fill=(255,255,0),
    )
    #line 4
    draw.text((0,font_height*3),text,font=font,fill=(255,255,0),
    )
    #line 5 does not fit using this font size (24)

    """
    draw.text(
        (width // 2 - font_width // 2, height // 2 - font_height // 2),
        text,
        font=font,
        fill=(255, 255, 0),
    )"""

    disp.image(image)

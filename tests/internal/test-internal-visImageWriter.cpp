//
// Created by henry on 4/3/18.
//

#include "catch.hpp"

extern "C" {
    #include "visvid/visvid.h"
    #include "visvid/utils.h"
    #include "visImageWriter.h"
}
SCENARIO("visImageWriterSetup"){
    int height = 100;
    int width = 200;

    GIVEN("a visImageRGB is located on the stack"){

        visImageRGB imageRGB;

        WHEN("the RGB image is allocated to 100 pixels high by 200 pixels wide"){

            visImageRGB_Alloc(&imageRGB, width, height);
            THEN("the image plane is allocated"){
                CHECK(imageRGB.plane != nullptr);
            }

            THEN("The image has the correct dimensions"){
                CHECK(imageRGB.height == height);
                CHECK(imageRGB.width == width);

            }

            visImageRGB_FreeData(&imageRGB);


        }
    }

    GIVEN("a visImageRGB is located  on the heap"){

        visImageRGB *imageRGB;

        imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
        CHECK(nullptr != imageRGB  );

        WHEN("the RGB image is allocated to 100 pixels high by 200 pixels wide"){

            visImageRGB_Alloc(imageRGB, width, height);

            THEN("the image plane is allocated"){
                CHECK(imageRGB->plane != nullptr);
            }

            THEN("The image has the correct dimensions") {

                REQUIRE(imageRGB->height == height);
                REQUIRE(imageRGB->width == width);
            }

            visImageRGB_FreeData(imageRGB);
            CHECK(nullptr == imageRGB->plane);
        }


        free(imageRGB);
    }


}
SCENARIO("visImageWriter functions"){
    GIVEN("an RGB image 4 pixels wide by 5 high"){
        visImageRGB *imageRGB           = nullptr;
        const static int ALPHA_SHIFT    = 0;
        const static int BLUE_SHIFT     = 1;
        const static int GREEN_SHIFT    = 2;
        const static int RED_SHIFT      = 3;
        int width                       = 4;
        int height                      = 5;

        imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
        CHECK(visImageRGB_Alloc(imageRGB, width, height) == 0);

        WHEN("the image is filled with the color red"){

            for (int x = 0; x < imageRGB->width; ++x) {
                for (int y = 0; y < imageRGB->height; ++y) {
                    visImageRGB_WritePixel(imageRGB, x, y, 255, 0, 0, 255);
                }
            }

            THEN("every pixel in the image is red"){

                for (int y = 0; y < imageRGB->height; ++y) {
                    for (int x = 0; x < imageRGB->width; ++x) {

                        int offset = (y * imageRGB->pitch) + (x * sizeof(uint8_t) * 4);
                        int value = imageRGB->plane[offset + RED_SHIFT];

                        CHECK(value == 255);
                        CHECK(imageRGB->plane[offset + ALPHA_SHIFT] == 255);
                        CHECK(imageRGB->plane[offset + BLUE_SHIFT] == 0);
                        CHECK(imageRGB->plane[offset + GREEN_SHIFT] == 0);
                    }
                }
            }
        }
        WHEN("a ramp is drawn to the image"){

            for (int x = 0; x < imageRGB->width; ++x) {
                for (int y = 0; y < imageRGB->height; ++y) {
                    visImageRGB_WritePixel(imageRGB, x, y, 0, 0, 0, (uint8_t)x);
                }
            }

            THEN("the pxiels in image ramp"){

                for (int y = 0; y < imageRGB->height; ++y) {
                    for (int x = 0; x < imageRGB->width; ++x) {
                        int offset = (y * imageRGB->pitch) + (x * sizeof(uint8_t) * 4);

                        CHECK(imageRGB->plane[offset + ALPHA_SHIFT] == x);
                        CHECK(imageRGB->plane[offset + RED_SHIFT] == 0);
                        CHECK(imageRGB->plane[offset + BLUE_SHIFT] == 0);
                        CHECK(imageRGB->plane[offset + GREEN_SHIFT] == 0);
                    }
                }

            }
        }
        visImageRGB_FreeData(imageRGB);
        free(imageRGB);
    }
    GIVEN("an RGB image 100 pixels wide by 100 high"){

        const int       WIDTH  = 100;
        const int       HEIGHT = 100;

        visImageRGB     image;

        CHECK(visImageRGB_Alloc(&image, WIDTH, HEIGHT) == 0);


        WHEN("the image is filled with the color black"){
            uint8_t assigned_red = 0;
            uint8_t assigned_blue = 0;
            uint8_t assigned_green = 0;
            uint8_t assigned_alpha = 0;

            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {
                    CHECK(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha) == 0);
                }
            }
            THEN("every pixel in the image is black"){


                for (int x = 0; x < WIDTH; ++x) {
                    for (int y = 0; y < HEIGHT; ++y) {
                        uint8_t returned_red = 10;
                        uint8_t returned_blue = 10;
                        uint8_t returned_green = 10;
                        uint8_t returned_alpha = 10;

                        CHECK(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue,
                                                    &returned_alpha) == 0);

                        CHECK(assigned_red == returned_red);
                        CHECK(assigned_blue == returned_blue);
                        CHECK(assigned_green == returned_green);
                        CHECK(assigned_alpha == returned_alpha);
                    }
                }

            }
        }

        WHEN("the image is filled with a ramping value horizontally"){
            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {
                    auto assigned_red = x;
                    auto assigned_blue = x;
                    auto assigned_green = x;
                    auto assigned_alpha = x;

                    CHECK(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green, assigned_blue,
                                                 assigned_alpha) == 0);
                }
            }
            THEN("The image has it's values ramping up equal to the position of the x value"){
                for (int x = 0; x < WIDTH; ++x) {
                    for (int y = 0; y < HEIGHT; ++y) {

                        uint8_t returned_red = 10;
                        uint8_t returned_blue = 10;
                        uint8_t returned_green = 10;
                        uint8_t returned_alpha = 10;

                        CHECK(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha) == 0);

                        CHECK(x == returned_red);
                        CHECK(x == returned_blue);
                        CHECK(x == returned_green);
                        CHECK(x == returned_alpha);

                    }
                }
            }
        }
        WHEN("the image is filled with a ramping value vertically"){
            uint8_t assigned_red = 0;
            uint8_t assigned_blue = 0;
            uint8_t assigned_green = 0;
            uint8_t assigned_alpha = 0;



            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < HEIGHT; ++y) {
                    if(y >= 50){
                        assigned_red    = 254;
                        assigned_blue   = 254;
                        assigned_green  = 254;
                        assigned_alpha  = 254;

                    } else {
                        assigned_red    = 0;
                        assigned_blue   = 0;
                        assigned_green  = 0;
                        assigned_alpha  = 0;
                    }


                    CHECK(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha) == 0);

                }
            }
            THEN("The image has it's values ramping up equal to the position of the y value"){

                uint8_t returned_red = 0;
                uint8_t returned_blue = 0;
                uint8_t returned_green = 0;
                uint8_t returned_alpha = 0;

                for (int x = 0; x < image.width; ++x) {
                    for (int y = 0; y < image.height; ++y) {
                        CHECK(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha) == 0);

                        if(y >= 50){
                            CHECK(254 == returned_red);
                            CHECK(254 == returned_blue);
                            CHECK(254 == returned_green);
                            CHECK(254 == returned_alpha);

                        } else {

                            CHECK(0 == returned_red);
                            CHECK(0 == returned_blue);
                            CHECK(0 == returned_green);
                            CHECK(0 == returned_alpha);
                        }
                    }
                }
            }

        }
        visImageRGB_FreeData(&image);

    }
}
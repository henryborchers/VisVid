//
// Created by henry on 4/3/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
}
TEST_CASE("visImageWriterSetup"){
    int height = 100;
    int width = 200;

    SECTION("Stack"){
        visImageRGB imageRGB;
        visImageRGB_Alloc(&imageRGB, width, height);

        CHECK(imageRGB.plane != nullptr);
        REQUIRE(imageRGB.height == height);
        REQUIRE(imageRGB.width == width);
    }

    SECTION("Heap"){
        visImageRGB *imageRGB;
        imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
        CHECK(imageRGB != NULL);

        visImageRGB_Alloc(imageRGB, width, height);
        CHECK(imageRGB->plane != nullptr);
        REQUIRE(imageRGB->height == height);
        REQUIRE(imageRGB->width == width);

        visImageRGB_FreeData(imageRGB);
        CHECK(imageRGB->plane == NULL);
        free(imageRGB);
    }


}

TEST_CASE("visImageWriter Functions") {

    visImageRGB *imageRGB           = nullptr;
    const static int ALPHA_SHIFT    = 0;
    const static int BLUE_SHIFT     = 1;
    const static int GREEN_SHIFT    = 2;
    const static int RED_SHIFT      = 3;
    int width                       = 4;
    int height                      = 5;

    imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
    CHECK(visImageRGB_Alloc(imageRGB, width, height) == 0);

    SECTION("write_red_image"){
        for (int x = 0; x < imageRGB->width; ++x) {
            for (int y = 0; y < imageRGB->height; ++y) {
                visImageRGB_WritePixel(imageRGB, x, y, 255, 0, 0, 255);
            }
        }

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

    SECTION("write ramp image"){
        for (int x = 0; x < imageRGB->width; ++x) {
            for (int y = 0; y < imageRGB->height; ++y) {
                visImageRGB_WritePixel(imageRGB, x, y, 0, 0, 0, (uint8_t)x);
            }
        }

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

    free(imageRGB);
}

TEST_CASE("visImageRGB Write and read pixel"){
    const int       WIDTH  = 100;
    const int       HEIGHT = 100;

    visImageRGB     image;

    CHECK(visImageRGB_Alloc(&image, WIDTH, HEIGHT) == 0);

    SECTION("blackbox"){
        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {
                uint8_t assigned_red = 0;
                uint8_t returned_red = 10;
                uint8_t assigned_blue = 0;
                uint8_t returned_blue = 10;
                uint8_t assigned_green = 0;
                uint8_t returned_green = 10;
                uint8_t assigned_alpha = 0;
                uint8_t returned_alpha = 10;

                CHECK(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha) == 0);
                CHECK(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha) == 0);

                CHECK(assigned_red == returned_red);
                CHECK(assigned_blue == returned_blue);
                CHECK(assigned_green == returned_green);
                CHECK(assigned_alpha == returned_alpha);

            }
        }
    }
    SECTION("vertical ramp"){
        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {
                uint8_t assigned_red = y;
                uint8_t returned_red = 10;
                uint8_t assigned_blue = y;
                uint8_t returned_blue = 10;
                uint8_t assigned_green = y;
                uint8_t returned_green = 10;
                uint8_t assigned_alpha = y;
                uint8_t returned_alpha = 10;

                CHECK(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green, assigned_blue,
                                             assigned_alpha) == 0);
                CHECK(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue,
                                            &returned_alpha) == 0);

                CHECK(assigned_red == returned_red);
                CHECK(assigned_blue == returned_blue);
                CHECK(assigned_green == returned_green);
                CHECK(assigned_alpha == returned_alpha);

            }
        }
    }
    SECTION("horizontal ramp"){
        for (int x = 0; x < WIDTH; ++x) {
            for (int y = 0; y < HEIGHT; ++y) {
                uint8_t assigned_red = x;
                uint8_t returned_red = 10;
                uint8_t assigned_blue = x;
                uint8_t returned_blue = 10;
                uint8_t assigned_green = x;
                uint8_t returned_green = 10;
                uint8_t assigned_alpha = x;
                uint8_t returned_alpha = 10;

                CHECK(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha) == 0);
                CHECK(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha) == 0)
                        ;
                CHECK(assigned_red == returned_red);
                CHECK(assigned_blue == returned_blue);
                CHECK(assigned_green == returned_green);
                CHECK(assigned_alpha == returned_alpha);

            }
        }
    }
    SECTION("top bottom"){
        uint8_t assigned_red = 0;
        uint8_t returned_red = 0;
        uint8_t assigned_blue = 0;
        uint8_t returned_blue = 0;
        uint8_t assigned_green = 0;
        uint8_t returned_green = 0;
        uint8_t assigned_alpha = 0;
        uint8_t returned_alpha = 0;

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